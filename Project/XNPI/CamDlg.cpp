// CamDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "CamDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "Repository.h"
#include "MainFrm.h"
#include "XNPIDoc.h"
#include "SCAlgoBase.h"
#include "Compo.h"
#include "CamPart.h"
#include "CompoTracker.h"
#include "TabDetailLogDlg.h"
#include "SCFidAlgo.h"
#include "LineEquation.h"
#include "SCHEdgeDetAlgo.h"
#include "SCHeightAlgo.h"
#include "SCAlignAlgo.h"
#include "XNPIView.h"
#include "SCResult.h"
#include <UniUtility.h>
#include <stdlib.h>
#include "SCClientSocket.h"
#include <Matrox.h>
#include <numeric>    
#include "ToolPanelDlg.h"
#include "OpPanelDlg.h"

// CCamDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCamDlg, CDialogEx)

BOOL CCamDlg::m_bRun = FALSE;
CCamDlg::CCamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAM, pParent)
	, m_pMilCamView(NULL)
	, m_strCamName(_T(""))
	, m_pOverlayDC(NULL)
	, m_bInitialized(FALSE)
	, m_pSelectedCompo(NULL)
	, m_pGrabChannel(NULL)
	, m_iCamBuildNo(-1)
	, m_bRequestReconnect(FALSE)
	, m_pMilCamRec(NULL)
{
	m_strCamName = _T("Cam");
}

CCamDlg::~CCamDlg()
{
}

void CCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GV_AVG, mStaticAvgGV);
}


BEGIN_MESSAGE_MAP(CCamDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCamDlg 메시지 처리기입니다.


BOOL CCamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if (!m_pMilCamView)
	{
		m_pMilCamView = CRepository::Instance()->m_MilLib.NewImage(8);
		m_pMilCamRec = CRepository::Instance()->m_MilLib.NewImage(8);
		m_pMilCamRec->SetAttribute(M_IMAGE + M_DISP + M_COMPRESS + M_JPEG2000_LOSSY);
		m_pMilCamDraw = CRepository::Instance()->m_MilLib.NewImage(8);
		m_pMilCamProc = CRepository::Instance()->m_MilLib.NewImage(16);

		m_pMilBufAvgAdd = CRepository::Instance()->m_MilLib.NewImage(32);
		m_pMilBufAvgResult = CRepository::Instance()->m_MilLib.NewImage(16);
		m_pMilCamAvgResult_Histo = CRepository::Instance()->m_MilLib.NewImage(16);

		m_pMilTempBufAvgResult = CRepository::Instance()->m_MilLib.NewImage(16);
		m_pMilTempBufAvgResult_histo = CRepository::Instance()->m_MilLib.NewImage(16);
	}

	bool grabberIsOnline = !CStatus::Instance()->m_bIsOffLine;
	if (grabberIsOnline)
	{
		m_pGrabChannel = CRepository::Instance()->m_Grabber.NewChannel();
		//! Grabber h/w --> m_pMilCamProc --> m_pMilCamDraw --> m_pMilCamView

		//m_pGrabChannel->SetMilBuffer(m_pMilCamProc, m_pMilCamDraw, m_pMilCamView, m_pMilCamRec);
		//! AVG처리를 위해 추가된 2개의 버퍼를 위해 SetMilBuffer -> SetMilBufferWithAvg로 변경
		//m_pMilCamAvgResult_Histo 추가(avg된 이미지에 윈도우레벨링 적용한 화면을 저장하기 위해) - 0919
		m_pGrabChannel->SetMilBufferWithAvg(m_pMilCamProc, m_pMilCamDraw, m_pMilCamView, m_pMilCamRec, m_pMilBufAvgAdd, m_pMilBufAvgResult, m_pMilCamAvgResult_Histo, m_pMilTempBufAvgResult, m_pMilTempBufAvgResult_histo);

		m_pGrabChannel->SetImgEnhance(CStatus::Instance()->m_bUseImgEnhance);
		m_pGrabChannel->SetLegendCallBack(std::bind(&CCamDlg::LegendUpdate, this));
		m_pGrabChannel->Init();
		m_pGrabChannel->SetWndLvl(CStatus::Instance()->m_wlvLow, CStatus::Instance()->m_wlvHigh);
		m_pMilCamView->AllocColor2D(CSize(CAM_HRES, CAM_VRES));
	}
	else
	{
		m_pMilCamProc->Alloc2D(CSize(CAM_HRES, CAM_VRES));
		m_pMilCamView->AllocColor2D(CSize(CAM_HRES, CAM_VRES));
		m_pMilCamDraw->AllocColor2D(CSize(CAM_HRES, CAM_VRES));
		m_pMilCamRec->AllocColor2D(CSize(CAM_HRES, CAM_VRES));

		m_pMilBufAvgAdd->Alloc2D(CSize(CAM_HRES, CAM_VRES));
		m_pMilBufAvgResult->Alloc2D(CSize(CAM_HRES, CAM_VRES));
		m_pMilCamAvgResult_Histo->Alloc2D(CSize(CAM_HRES, CAM_VRES));

		//190401
		m_pMilTempBufAvgResult->Alloc2D(CSize(CAM_HRES, CAM_VRES));
		m_pMilTempBufAvgResult_histo->Alloc2D(CSize(CAM_HRES, CAM_VRES));
	}
	m_pMilDisp = CRepository::Instance()->m_MilLib.NewDisplay();
	m_pMilCamDraw->EnableGraph();
	m_pMilCamView->SelectWindow(m_pMilDisp, this);
	m_pMilDisp->EnableOverlay();
	m_pMilDisp->EnableGraph();
	m_pMilDisp->FilWindow();

	switch (CStatus::Instance()->m_ulDispMode)
	{
	case CStatus::DISP_AUTO:
		if (!CStatus::Instance()->m_bIsOffLine)
			m_pGrabChannel->SetAutoScale();
		break;
	case CStatus::DISP_MANUAL:
		break;
	}

	//	m_pMilDisp->DispCenter();
	//	m_pMilDisp->OutputText(10, 10, m_strCamTag.GetBuffer());



	m_gvAvgRegionWidth = CStatus::Instance()->m_gvAvgRegionWidth;
	m_gvAvgRegionHeight = CStatus::Instance()->m_gvAvgRegionHeight;

	//Avg GV 위치 조절
	CStatic *st = (CStatic *)GetDlgItem(IDC_STATIC_GV_AVG);
	st->GetClientRect(mRectStaticAvgGV); // Static 의 크기를 얻어옴
	st->MoveWindow(0, 765 - 13, 140, 13); // Static 의 크기 조절

	/*CString str;
	str.Format(_T("Avg Gray (%d*%d): "), m_gvAvgRegionWidth, m_gvAvgRegionHeight);
	mStaticAvgGV.SetWindowTextW(str);*/

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCamDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_pMilCamView)return;
	m_pMilDisp->EnableOverlay(FALSE);
	m_pMilDisp->EnableGraph(FALSE);
	m_pMilCamView->SelectWindow(m_pMilDisp, M_NULL, TRUE);
	m_pMilCamView->SelectWindow(m_pMilDisp, this);
	m_pMilDisp->EnableOverlay();
	m_pMilDisp->EnableGraph();
	//	m_pMilDisp->DispCenter();
	m_dblsizeZoomFactor = m_pMilDisp->GetZoomFactor();
}


void CCamDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (IsTeachingMode())
	{
		SetFocus();
		CCompo *pFindedCompo = NULL;
		if (CamViewHitTest(point, pFindedCompo) < 0)
		{
			//! 해당되는 것이 아무것도 없어서 새로 만들어야 함.
			if (CStatus::Instance()->GetCurAlgoToAddIndex() == CStatus::INSPFID)
			{
				if (IsThereFid())
				{
					AfxMessageBox(_T("There is Fid !"));
					CDialogEx::OnLButtonDown(nFlags, point);
					return;
				}
			}
			if (CStatus::Instance()->GetCurAlgoToAddIndex() == CStatus::INSPHEDET)
			{
				if (IsThereHEDet())
				{
					AfxMessageBox(_T("There is H.EdgeDetector !"));
					CDialogEx::OnLButtonDown(nFlags, point);
					return;
				}
			}

			AddCompoForView(point);
		}
		else
		{
			m_pSelectedCompo = pFindedCompo;
			if (pFindedCompo->m_pCompoTracker->Track(this, point, true))
			{
				pFindedCompo->m_pCompoTracker->m_rect.NormalizeRect();
				CRect rectImage;
				GetMapedRect(&rectImage);
				pFindedCompo->m_pCompoTracker->m_rectBuffer = rectImage;
			}
		}
		m_pMilDisp->ClearOverlay();
		SetOVLDC();
		DrawAllTrack(m_pOverlayDC);
		FreeOVLDC();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CCamDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CdblPoint posMilMouse;
	int GrayValue = 0;

	posMilMouse.dblPosX = point.x;
	posMilMouse.dblPosY = point.y;
	CStatus::Instance()->m_infoCamMousePos.nCamNo = m_iCamBuildNo;


	//kiy - 마우스 Region으로 밝기 평균값 구하기 0919
	long rgWidth = CStatus::Instance()->m_gvAvgRegionWidth;
	long rgHeight = CStatus::Instance()->m_gvAvgRegionHeight;

	long offsetW = rgWidth / 2;
	long offsetH = rgHeight / 2;

	CPoint ptMapped;
	ptMapped = GetMapedPoint(point);

	CRect rgRect = { ptMapped.x - offsetW, ptMapped.y - offsetH,
					 ptMapped.x + offsetW, ptMapped.y + offsetH };

	CStatus::Instance()->m_infoCamMousePos.posImage.dblPosX = ptMapped.x;
	CStatus::Instance()->m_infoCamMousePos.posImage.dblPosY = ptMapped.y;

	m_pMilCamProc->GetPixelGray(ptMapped.x, ptMapped.y, &GrayValue);
	CStatus::Instance()->m_infoCamMousePos.GrayValue = GrayValue;

	//범위 벗어난 경우는 -1 출력
	if (rgRect.top < 0 ||
		rgRect.left < 0 ||
		rgRect.bottom >(CAM_VRES - 1) ||
		rgRect.right >(CAM_HRES - 1))
	{
		if (CStatus::Instance()->m_bShowGrayAvg)
		{
			CString avgGV;
			avgGV.Format(_T("Avg Gray (%d*%d): %d"), m_gvAvgRegionWidth, m_gvAvgRegionHeight, -1);
			mStaticAvgGV.SetWindowTextW(avgGV);
		}
		else
			mStaticAvgGV.SetWindowTextW(_T(""));

		return;
	}

	int size = rgRect.Width() * rgRect.Height();

	USHORT* arr = new USHORT[size]();

	//m_pMilCamProc대신 Avg된 m_pMilBufAvgResult를 써야 하나? - 0920
	m_pMilBufAvgResult->Get2D(arr, rgRect);

	int sum = 0;
	int gvAvg = 0;

	for (int i = 0; i < size; i++)
		sum = arr[0] + sum;

	gvAvg = sum / size;
	//printf("sum: %d, Avg: %.2f\n", sum, gvAvg);

	delete[] arr;
	//--------------------------------------------


	//마우스 Region 사각형 그리기
	if (CStatus::Instance()->m_bShowGrayAvg)
	{
		//잔상 남을 가능성 있어서 일단 막아둠. - 0920

		//MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		//MgraColor(M_DEFAULT, M_COLOR_GREEN);
		//MgraRect(M_DEFAULT, m_pMilCamDraw->GetIDBuffer(), rgRect.top, rgRect.left, rgRect.bottom, rgRect.right);


		//CPoint & PointTL = mRectStaticAvgGV.TopLeft();
		//ScreenToClient(&PointTL);
		//CPoint & PointBR = mRectStaticAvgGV.BottomRight();
		//ScreenToClient(&PointBR);
		//InvalidateRect(mRectStaticAvgGV, FALSE); // TRUE, FALSE는 알아서.

		CString avgGV;
		avgGV.Format(_T("Avg Gray (%d*%d): %d"), m_gvAvgRegionWidth, m_gvAvgRegionHeight, gvAvg);
		mStaticAvgGV.SetWindowTextW(avgGV);
	}
	else
		mStaticAvgGV.SetWindowTextW(_T(""));

	CDialogEx::OnMouseMove(nFlags, point);
}


void CCamDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCamDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.

	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}

// 하나라도 걸리는것이 있으면 양수 걸리는것이 하나도 없으면 음수
int CCamDlg::CamViewHitTest(CPoint Point, CCompo * &pFindedCompo)
{
	int nCamNo = CStatus::Instance()->GetSelCamNo();
	CModelData *pModelData = CStatus::Instance()->GetModelData();
	POSITION pos;
	int nCountCam = (int)pModelData->GetList()->GetCount();
	if (!nCountCam)return -1;

	CPtrList *pList = GetCamList();
	if (pList)
	{
		CCompo *pCompo;
		int nCountCompo = (int)pList->GetCount();
		for (int i = 0; i < nCountCompo; i++)
		{
			pos = pList->FindIndex(i);
			pCompo = (CCompo *)(pList->GetAt(pos));

			int rvHitTest = pCompo->m_pCompoTracker->HitTest(Point);
			if (rvHitTest >= 0)
			{
				pFindedCompo = pCompo;
				return rvHitTest;
			}
		}
	}

	return -1;
}


CPtrList * CCamDlg::GetCamList()
{
	if (m_iCamBuildNo == -1)return NULL;
	int nCamNo = m_iCamBuildNo;
	CModelData *pModeData = CStatus::Instance()->GetModelData();
	int nCountCamNumber = (int)pModeData->GetList()->GetCount();
	CCamPart *pCamPart;
	POSITION pos;
	for (int i = 0; i < nCountCamNumber; i++)
	{
		pos = pModeData->GetList()->FindIndex(i);
		pCamPart = (CCamPart *)pModeData->GetList()->GetAt(pos);
		if (nCamNo == pCamPart->m_nCamNo) return pCamPart->GetList();
	}
	return 0;
}

BOOL CCamDlg::IsTeachingMode()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	return pView->IsTeachingMode();
}

void CCamDlg::AddCompoForView(CPoint Point)
{
	SetOVLDC();
	CPen myPen, oldPen;
	CRect realRect;
	CCompoTracker *pRectTracker = new CCompoTracker;
	if (pRectTracker->TrackRubberBand(this, Point, true))
	{
		pRectTracker->m_pAlgo = CStatus::Instance()->GetCurAlgoToAdd();

		pRectTracker->m_rect.NormalizeRect();

		realRect = pRectTracker->m_rect;
		GetMapedRect(&realRect);
		pRectTracker->m_rectBuffer = realRect;
		pRectTracker->m_nStyle = 0;
		pRectTracker->m_nStyle |= CRectTracker::solidLine;
		pRectTracker->m_nStyle |= CRectTracker::hatchedBorder;
		pRectTracker->m_nStyle |= CRectTracker::resizeOutside;

		CPtrList *pList = GetCamList();
		if (!pList)pList = AddCamPart();
		CCompo *pCompo = new CCompo;
		pCompo->m_pCompoTracker = pRectTracker;
		m_pSelectedCompo = pCompo;
		srand((unsigned int)time(NULL));
		CString strRefID;
		strRefID.Format(_T("%d"), rand());
		pCompo->m_strRef = strRefID;

		if (m_pOverlayDC)
		{
			Draw(m_pOverlayDC, pCompo->m_pCompoTracker);
			//			pCompo->m_pCompoTracker->Draw(m_pOverlayDC);
		}

		pList->AddTail(pCompo);
	}
	else
	{

	}
	FreeOVLDC();
}


BOOL CCamDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (IsTeachingMode())
	{
		CPtrList *pList = GetCamList();
		POSITION pos;
		if (pList)
		{
			int nCount = (int)pList->GetCount();
			CCompo *pCompo;
			for (int i = 0; i < nCount; i++)
			{
				pos = pList->FindIndex(i);
				pCompo = (CCompo *)(pList->GetAt(pos));
				if (pCompo->m_pCompoTracker->SetCursor(this, nHitTest))
				{
					return TRUE;
				}
			}
		}
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CCamDlg::DrawAllTrack()
{
	SetOVLDC();
	DrawAllTrack(m_pOverlayDC);
	FreeOVLDC();
}

void CCamDlg::DrawAllTrack(CDC * pDC)
{
	POSITION pos;
	CCompo *pCompo;
	if (pDC == NULL)return;
	CPtrList *pList = GetCamList();
	if (pList == NULL)return;
	if (!pList->GetCount())return;
	for (pos = GetCamList()->GetHeadPosition(); pos != NULL;)
	{
		pCompo = (CCompo *)(pList->GetNext(pos));
		CRect rectImage = pCompo->m_pCompoTracker->m_rect;

		if ((pCompo->m_pCompoTracker->m_rect.Width() > 0) && (pCompo->m_pCompoTracker->m_rect.Height() > 0))
		{
			GetMapedRect(&rectImage);
			pCompo->m_pCompoTracker->m_rectBuffer = rectImage;

			if (m_pSelectedCompo == pCompo)
			{
				pCompo->m_pCompoTracker->m_nStyle = 0;
				pCompo->m_pCompoTracker->m_nStyle |= CRectTracker::solidLine;
				pCompo->m_pCompoTracker->m_nStyle |= CRectTracker::hatchedBorder;
				pCompo->m_pCompoTracker->m_nStyle |= CRectTracker::resizeOutside;
			}
			else
			{
				pCompo->m_pCompoTracker->m_nStyle = 0;
				pCompo->m_pCompoTracker->m_nStyle |= CRectTracker::dottedLine;
				pCompo->m_pCompoTracker->m_nStyle |= CRectTracker::resizeOutside;
			}
			//juba			m_pOverlayDC->DPtoLP(pCompo->m_pCompoTracker->m_rect);

			Draw(m_pOverlayDC, pCompo->m_pCompoTracker);
			//juba			pCompo->m_pCompoTracker->Draw(pDC);
		}
	}
}


int CCamDlg::ClearOverlay()
{
	return 0;
}


CPtrList * CCamDlg::AddCamPart()
{
	int nCamNo = CStatus::Instance()->GetSelCamNo();
	CModelData *pModeData = CStatus::Instance()->GetModelData();
	CCamPart * pCamPart = new CCamPart;
	pCamPart->m_nCamNo = nCamNo;
	pModeData->GetList()->AddTail(pCamPart);
	return pCamPart->GetList();
}


void CCamDlg::FreeOVLDC()
{
	//if (!m_bInitialized)return;
	//if (!m_pMilDisp)return;
	m_pMilDisp->FreeDC();
	m_pMilDisp->Modified();
}


void CCamDlg::SetOVLDC()
{
	//m_pMainDlg->m_pMilCenterView->SelectWindow(this);
	//m_pMainDlg->m_pMilCenterView->GetDisplay()->Control(M_FILL_DISPLAY, M_ENABLE);
	//m_pMainDlg->m_pMilCenterView->GetDisplay()->EnableOverlay();
	//m_pMainDlg->m_pMilCenterView->GetDisplay()->EnableGraph();
	//if (!m_bInitialized)return;
	//if (!m_pMilDisp)return;
	m_pOverlayDC = m_pMilDisp->GetDC();
	//	m_pOverlayDC->SetMapMode(MM_ANISOTROPIC);

	CRect winRect;
	GetClientRect(&winRect);
	//m_pOverlayDC->SetViewportExt(winRect.Width(), winRect.Height());
	//m_pOverlayDC->SetWindowExt(
	//	winRect.Width() / m_dblsizeZoomFactor.dblCX, 
	//	winRect.Height() / m_dblsizeZoomFactor.dblCY);
	//m_pOverlayDC->SetViewportOrg(_xMargin, 0);
}


BOOL CCamDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//if (pMsg->hwnd == this)
	//{

	//}
	if (pMsg->wParam == VK_DELETE)
	{
		DeleteSelCompo();
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CCamDlg::DeleteSelCompo()
{
	POSITION pos;
	CCompo *pCompo;
	CPtrList *pList = GetCamList();
	if (pList == NULL)return;
	int nCount = (int)pList->GetCount();
	if (!nCount)return;
	for (int i = 0; i < nCount; i++)
	{
		pos = pList->FindIndex(i);
		pCompo = (CCompo *)(pList->GetAt(pos));
		if ((pCompo->m_pCompoTracker->m_rect.Width() > 0) && (pCompo->m_pCompoTracker->m_rect.Height() > 0))
		{
			if (m_pSelectedCompo == pCompo)
			{
				delete pCompo->m_pCompoTracker;
				pCompo->m_pCompoTracker = NULL;
				pList->RemoveAt(pos);
				delete pCompo;
				m_pSelectedCompo = NULL;
				break;
			}
		}
	}
	m_pMilDisp->ClearOverlay();
	SetOVLDC();
	DrawAllTrack(m_pOverlayDC);
	FreeOVLDC();
}

CPoint CCamDlg::GetMapedPoint(CPoint point)
{
	CRect rectCur;
	GetClientRect(&rectCur);
	CPoint ptMapped;
	double marginX = rectCur.Width() - CAM_HRES * m_dblsizeZoomFactor.dblCX;
	TRACE(_T("\nZoomFactor X:%f"), m_dblsizeZoomFactor.dblCX);
	double Xratio = (rectCur.Width() - marginX) / (double)CAM_HRES;
	double Yratio = rectCur.Height() / (double)CAM_VRES;

	ptMapped.x = (int)((point.x - marginX / 2) / Xratio);
	ptMapped.y = (int)(point.y / Yratio);
	if (ptMapped.x < 0)ptMapped.x = 0;
	if (ptMapped.y < 0)ptMapped.y = 0;
	if (ptMapped.x > (CAM_HRES - 1))ptMapped.x = CAM_HRES - 1;
	if (ptMapped.y > (CAM_VRES - 1))ptMapped.y = CAM_VRES - 1;

	return ptMapped;
}

void CCamDlg::GetMapedRect(CRect *pOrgRect)
{
	CRect rectCur;
	GetClientRect(&rectCur);
	double marginX = rectCur.Width() - CAM_HRES * m_dblsizeZoomFactor.dblCX;
	TRACE(_T("\nZoomFactor X:%f"), m_dblsizeZoomFactor.dblCX);
	double Xratio = (rectCur.Width() - marginX) / (double)CAM_HRES;
	double Yratio = rectCur.Height() / (double)CAM_VRES;

	pOrgRect->left = (int)((pOrgRect->left - marginX / 2) / Xratio);
	pOrgRect->top = (int)(pOrgRect->top / Yratio);
	pOrgRect->right = (int)((pOrgRect->right - marginX / 2) / Xratio);
	pOrgRect->bottom = (int)(pOrgRect->bottom / Yratio);
}

int CCamDlg::Draw(CDC * pDC, CCompoTracker *pCompoTracker)
{
	CPen myPen;

	switch (pCompoTracker->m_pAlgo->m_nInspType)
	{
	case 0:
		myPen.CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
		break;
	case 1:
		myPen.CreatePen(PS_SOLID, 2, RGB(0, 100, 255));
		break;
	case 2:
		myPen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		break;
	case 3:
		myPen.CreatePen(PS_SOLID, 2, RGB(153, 217, 234));
		break;
	case 4:
		myPen.CreatePen(PS_SOLID, 2, RGB(100, 50, 234));
		break;
	default:
		myPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	}
	CPen *pOldPen = pDC->SelectObject(&myPen);
	CBrush myBrush;
	CBrush *pOldBrush = NULL;
	CRect rectToDraw;
	rectToDraw = pCompoTracker->m_rect;

	GetMapedRect(&rectToDraw);
	int xpos = rectToDraw.left;
	int ypos = rectToDraw.top;
	//CString strOut = _T("Insp");
	//pDC->TextOutW(xpos, ypos, strOut);

	myBrush.CreateStockObject(NULL_BRUSH);
	pDC->SelectObject(&myBrush);
	pDC->Rectangle(&rectToDraw);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	myBrush.DeleteObject();
	myPen.DeleteObject();

	return 0;
}


int CCamDlg::LoadBufImage(CString strFileName, MIL_INT FileFormat)
{
	m_pMilCamProc->BufImport(strFileName, FileFormat);
	m_pMilCamView->Copy(m_pMilCamProc);
	m_pMilCamDraw->Copy(m_pMilCamProc);
	return 0;
}

int CCamDlg::LoadBufImage(CMilBuffer *pImage)
{
	m_pMilCamProc->Copy(pImage);
	m_pMilCamView->Copy(m_pMilCamProc);
	m_pMilCamDraw->Copy(m_pMilCamProc);
	return 0;
}


void CCamDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CSCAlgoDialog *pParamDlg;
	if (IsTeachingMode())
	{
		SetFocus();
		CCompo *pFindedCompo = NULL;
		if (CamViewHitTest(point, pFindedCompo) >= 0)
		{
			m_pSelectedCompo = pFindedCompo;
			pParamDlg = (CSCAlgoDialog *)pFindedCompo->m_pCompoTracker->m_pAlgo->m_pParamDlg;
			pFindedCompo->m_pCompoTracker->m_pAlgo->SetMilBuffer(m_pMilCamProc, m_pMilCamDraw);
			pFindedCompo->m_pCompoTracker->m_pAlgo->m_rectInsp = pFindedCompo->m_pCompoTracker->m_rectBuffer;
			pParamDlg->SetAlgo(pFindedCompo->m_pCompoTracker->m_pAlgo);
			if (pParamDlg) pParamDlg->ShowWindow(SW_SHOW);
		}
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

// RV: -1 no Inspect data 
//     0 Good
//    1~ NG
int CCamDlg::Inspect(CSCResult *pResult)
{
	int iStartTime = GetTickCount();
	CPtrList *pList = GetCamList();
	if (!pList)
	{
		CString strMsg;
		strMsg.Format(_T("Not Teached Cam No:%d"), m_iCamBuildNo + 1);
		CTabDetailLogDlg::Instance()->SetTextLog(strMsg.GetBuffer(), RGB(50, 50, 50), FALSE);
		return -1;
	}
	USES_CONVERSION;
	int nCount = (int)pList->GetCount();
	if (!nCount)return -1;
	CLineEquation m_LineEquation;

	CdblPoint dblPtOffset(0.0, 0.0);
	if (IsThereFid())
	{
		FindOffset(&dblPtOffset, m_pMilCamProc, m_pMilCamDraw);
	}

	if (IsThereHEDet())
	{
		FindHorizontalEdge(&m_LineEquation, &dblPtOffset, m_pMilCamProc, m_pMilCamDraw);
	}
	POSITION pos;
	pos = pList->GetHeadPosition();
	CCompo *pCompo;
	int rv = NULL;
	CSCResult curResult;
	curResult.CamNo = m_iCamBuildNo;
	for (int i = 0; i < nCount; i++)
	{
		pos = pList->FindIndex(i);
		pCompo = (CCompo *)(pList->GetAt(pos));
		CRect rectReal = pCompo->m_pCompoTracker->m_rectBuffer;
		rectReal.OffsetRect(CPoint((int)dblPtOffset.dblPosX, (int)dblPtOffset.dblPosY));
		pCompo->m_pCompoTracker->m_pAlgo->SetPixelCalData(CStatus::Instance()->GetPixelCalData(m_iCamBuildNo));
		switch (pCompo->m_pCompoTracker->m_pAlgo->m_nInspType)
		{
		case CStatus::INSPHEIGHT:
			((CSCHeightAlgo *)(pCompo->m_pCompoTracker->m_pAlgo))->SetLineEquation(&m_LineEquation);
			curResult.iInspType = CStatus::INSPHEIGHT;
			rv += pCompo->m_pCompoTracker->m_pAlgo->Inspect(m_pMilCamProc, m_pMilCamDraw, &rectReal);
			curResult.iResult = rv;
			((CSCHeightAlgo *)(pCompo->m_pCompoTracker->m_pAlgo))->GetResult(&curResult);
			if (CStatus::Instance()->GetOpMode() == CStatus::OPAUTO)
				//				CRepository::Instance()->WriteInspResult(&curResult);
				break;
		case CStatus::INSPALIGN:
			curResult.iInspType = pCompo->m_pCompoTracker->m_pAlgo->m_nInspType;
			rv += pCompo->m_pCompoTracker->m_pAlgo->Inspect(m_pMilCamProc, m_pMilCamDraw, &rectReal);
			curResult.iResult = rv;
			((CSCAlignAlgo *)(pCompo->m_pCompoTracker->m_pAlgo))->GetResult(&curResult);
			//curResult.dblMean = ((CSCAlignAlgo *)(pCompo->m_pCompoTracker->m_pAlgo))->m_dblMean;
			//curResult.dblMin = ((CSCAlignAlgo *)(pCompo->m_pCompoTracker->m_pAlgo))->m_dblMin;
			//curResult.dblMax = ((CSCAlignAlgo *)(pCompo->m_pCompoTracker->m_pAlgo))->m_dblMax;
			if (CStatus::Instance()->GetOpMode() == CStatus::OPAUTO)
				//				CRepository::Instance()->WriteInspResult(&curResult);
				break;
		case CStatus::INSPGAPGAGE:
			curResult.iInspType = pCompo->m_pCompoTracker->m_pAlgo->m_nInspType;
			rv += pCompo->m_pCompoTracker->m_pAlgo->Inspect(m_pMilCamProc, m_pMilCamDraw, &rectReal);
			curResult.iResult = rv;
			((CSCAlignAlgo *)(pCompo->m_pCompoTracker->m_pAlgo))->GetResult(&curResult);
			if (CStatus::Instance()->GetOpMode() == CStatus::OPAUTO)
				//				CRepository::Instance()->WriteInspResult(&curResult);
				break;
		default:
			break;
		}
	}
	DrawToView();
	int iEndTime = GetTickCount();
	CString strText;
	strText.Format(_T("Inspection Time %d ms"), iEndTime - iStartTime);
	CTabDetailLogDlg::Instance()->SetTextLog(strText, RGB(50, 50, 50), FALSE);
	return rv;
}

void CCamDlg::RefreshView()
{
	m_pMilCamView->Copy(m_pMilCamProc);
}


BOOL CCamDlg::IsThereFid()
{
	CPtrList *pList = GetCamList();
	USES_CONVERSION;
	if (!pList)return FALSE;
	int nCount = (int)pList->GetCount();
	if (!nCount)return FALSE;

	POSITION pos;
	pos = pList->GetHeadPosition();
	CCompo *pCompo;
	int rv = NULL;
	for (int i = 0; i < nCount; i++)
	{
		pos = pList->FindIndex(i);
		pCompo = (CCompo *)(pList->GetAt(pos));
		CSCAlgoBase * pAlgo = pCompo->m_pCompoTracker->m_pAlgo;
		if (!pAlgo)continue;
		if (pAlgo->m_nInspType == CStatus::INSPFID)
			return TRUE;
	}
	return FALSE;
}


BOOL CCamDlg::IsThereHEDet()
{
	CPtrList *pList = GetCamList();
	USES_CONVERSION;
	if (!pList)return FALSE;
	int nCount = (int)pList->GetCount();
	if (!nCount)return FALSE;

	POSITION pos;
	pos = pList->GetHeadPosition();
	CCompo *pCompo;
	int rv = NULL;
	for (int i = 0; i < nCount; i++)
	{
		pos = pList->FindIndex(i);
		pCompo = (CCompo *)(pList->GetAt(pos));
		CSCAlgoBase * pAlgo = pCompo->m_pCompoTracker->m_pAlgo;
		if (!pAlgo)continue;
		if (pAlgo->m_nInspType == CStatus::INSPHEDET)
			return TRUE;
	}
	return FALSE;
}

CCompo * CCamDlg::FindFid()
{
	CPtrList *pList = GetCamList();
	USES_CONVERSION;
	if (!pList)return FALSE;
	int nCount = (int)pList->GetCount();
	if (!nCount)return FALSE;

	POSITION pos;
	pos = pList->GetHeadPosition();
	CCompo *pCompo;
	int rv = NULL;
	for (int i = 0; i < nCount; i++)
	{
		pos = pList->FindIndex(i);
		pCompo = (CCompo *)(pList->GetAt(pos));
		CSCAlgoBase * pAlgo = pCompo->m_pCompoTracker->m_pAlgo;
		if (!pAlgo)continue;
		if (pAlgo->m_nInspType == CStatus::INSPFID)
			return  pCompo;
	}
	return NULL;
}

CCompo * CCamDlg::FindHEdge()
{
	CPtrList *pList = GetCamList();
	USES_CONVERSION;
	if (!pList)return FALSE;
	int nCount = (int)pList->GetCount();
	if (!nCount)return FALSE;

	POSITION pos;
	pos = pList->GetHeadPosition();
	CCompo *pCompo;
	int rv = NULL;
	for (int i = 0; i < nCount; i++)
	{
		pos = pList->FindIndex(i);
		pCompo = (CCompo *)(pList->GetAt(pos));
		CSCAlgoBase * pAlgo = pCompo->m_pCompoTracker->m_pAlgo;
		if (!pAlgo)continue;
		if (pAlgo->m_nInspType == CStatus::INSPHEDET)
			return  pCompo;
	}
	return NULL;
}


int CCamDlg::GetFidROI(CRect *pRectFidROI)
{
	CCompo *pCompo = FindFid();
	if (!pCompo)return -1;
	*pRectFidROI = pCompo->m_pCompoTracker->m_rectBuffer;
	return 0;
}

void CCamDlg::DrawToView()
{
	DrawCross(m_pMilCamDraw);
	m_pMilCamView->Copy(m_pMilCamDraw);
}

UINT CCamDlg::ThreadSendRemote(LPVOID pParam)
{
	m_bRun = TRUE;
	while (m_bRun)
	{


	}

	return 0;
}

//bIsNG true->NG, false->good
int CCamDlg::SaveImage(BOOL bIsNG)
{
	CString strNGSaveFolder = CStatus::Instance()->m_strIMGSaveFolder;
	CString strTestImgSaveFolder;
	CString strFileName;
	static CString strPrevFileName = _T("");
	static int iIndex = 0;
	CTime curTime = CTime::GetCurrentTime();

	CString strState;
	if (bIsNG)
	{
		strState = _T("NG");
	}
	else
	{
		strState = _T("Good");
	}
	do
	{
		strFileName = strState + curTime.Format(_T("%y%m%d%H%M%S"));
		strFileName.Format(_T("%s-%s-%d"), m_strCamName.GetBuffer(), strFileName.GetBuffer(), iIndex);
		iIndex++;
	} while (strFileName == strPrevFileName);

	iIndex = 0;
	strPrevFileName = strFileName;
	CTime tmCurrent = CTime::GetCurrentTime();
	CString strDate = tmCurrent.Format(_T("%y%m%d"));
	CString strSaveFolder = strNGSaveFolder + _T("\\") + strDate;
	CString strRemoteFolder = strDate;
	CFileInfo::CreateFolder(strSaveFolder.GetBuffer());

	strTestImgSaveFolder = strSaveFolder + _T("\\") + strFileName + _T(".bmp");
	strNGSaveFolder = strSaveFolder + _T("\\") + strFileName + _T(".jpg");
	strRemoteFolder = strRemoteFolder + _T("\\") + strFileName + _T(".jpg");
	int iStartTime = GetTickCount();
	m_pMilCamDraw->Export(strNGSaveFolder, M_JPEG_LOSSY);
	if (CStatus::Instance()->m_bSaveTestImage)m_pMilCamProc->Export(strTestImgSaveFolder, M_BMP);
	int iEndTime = GetTickCount();
	TRACE(_T("\nSave Time %d"), iEndTime - iStartTime);
	CStatus::Instance()->m_strRemoteImgName = strRemoteFolder;
	BOOL bErr;
	if (strRemoteFolder.GetLength() > 29)
	{
		bErr = TRUE;
	}
	return 0;
}


int CCamDlg::Grab()
{
	if (CStatus::Instance()->m_bIsOffLine)return -1;
	m_pGrabChannel->Grab();
	return 0;
}

BOOL CCamDlg::IsGrabDone()
{
	return m_pGrabChannel->IsGrabDone();
}

//! 촬영과 전송대기, 밀버퍼로 옮기는 것까지수행
int CCamDlg::DoGrab()
{
	if (CStatus::Instance()->m_bIsOffLine)return -1;

	m_pGrabChannel->Grab();
	while (!IsGrabDone());
	DrawToView();
	return 0;
}


void CCamDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CCamDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_pGrabChannel)m_pGrabChannel->Close();

	CDialogEx::OnClose();
}

int CCamDlg::FindOffset(CdblPoint * pDblOffset, CMilBuffer *pMilBuffProc, CMilBuffer *pMilBuffDraw)
{
	CCompo *pCompo = FindFid();

	if (!pCompo)return FALSE;
	CSCAlgoBase * pAlgo = pCompo->m_pCompoTracker->m_pAlgo;
	if (!((CSCFidAlgo *)pAlgo)->m_pMilBuffer)
	{
		AfxMessageBox(_T("Reg the Fiducial!"));
		return FALSE;
	}
	pAlgo->m_pMilBuffORG = pMilBuffProc;
	pAlgo->m_pMilBuffView = pMilBuffDraw;
	CRect rectReal = pCompo->m_pCompoTracker->m_rectBuffer;
	pAlgo->m_rectInsp = rectReal;
	CMilBuffer *pROIMilBuffer;
	pROIMilBuffer = GetROIChild((CSCFidAlgo *)pAlgo);
	if (!((CSCFidAlgo *)pAlgo)->Find(pROIMilBuffer))
	{
		((CSCFidAlgo *)pAlgo)->Draw();
		pDblOffset->dblPosX = ((CSCFidAlgo *)pAlgo)->m_upOffset.dblPosX;
		pDblOffset->dblPosY = ((CSCFidAlgo *)pAlgo)->m_upOffset.dblPosY;
	}
	else
		((CSCFidAlgo *)pAlgo)->Draw(FALSE);
	pROIMilBuffer->Close();
	return TRUE;
}


int CCamDlg::FindHorizontalEdge(CLineEquation * pLineEquation, CdblPoint * pDblOffset, CMilBuffer *pMilBuffProc, CMilBuffer *pMilBuffDraw)
{
	CCompo *pCompo = FindHEdge();

	if (!pCompo)return FALSE;
	CSCAlgoBase * pAlgo = pCompo->m_pCompoTracker->m_pAlgo;
	pAlgo->m_pMilBuffORG = pMilBuffProc;
	pAlgo->m_pMilBuffView = pMilBuffDraw;
	CRect rectReal = pCompo->m_pCompoTracker->m_rectBuffer;
	pAlgo->m_rectInsp = rectReal;
	CPoint ptOffset((int)pDblOffset->dblPosX, (int)pDblOffset->dblPosY);
	rectReal.OffsetRect(ptOffset);
	((CSCHEdgeDetAlgo *)pAlgo)->m_pLineEquation = pLineEquation;
	if (!((CSCHEdgeDetAlgo *)pAlgo)->Inspect(pMilBuffProc, pMilBuffDraw, &rectReal))
	{
		//		((CSCHEdgeDetAlgo *)pAlgo)->Draw(TRUE);
	}
	else
	{
		//		((CSCHEdgeDetAlgo *)pAlgo)->Draw(FALSE);

	}
	return TRUE;

}

CMilBuffer * CCamDlg::GetROIChild(CSCFidAlgo *pFidAlgo)
{
	CRect rectROI;
	GetFidROI(&rectROI);
	pFidAlgo->m_ptFidROICenterOffset.SetPoint(0, 0);

	double offsetx = 0;
	double offsety = 0;

	rectROI.InflateRect(CSize(
		(int)((rectROI.Width() / 2) * (pFidAlgo->m_SearchRangeFactor.dblCX - 1)),
		(int)((rectROI.Height() / 2) * (pFidAlgo->m_SearchRangeFactor.dblCY - 1))));
	pFidAlgo->m_rectFidSearchRange = m_rectFidSearchRange = rectROI;

	if (rectROI.left < 0)
	{
		pFidAlgo->m_ptFidROICenterOffset.dblPosX = -rectROI.left;
		rectROI.left = 0;
	}
	if (rectROI.right > (CAM_HRES - 1))
	{
		pFidAlgo->m_ptFidROICenterOffset.dblPosX = rectROI.right - (CAM_HRES - 1);
		rectROI.right = CAM_HRES - 1;
	}
	if (rectROI.top < 0)
	{
		pFidAlgo->m_ptFidROICenterOffset.dblPosY = -rectROI.top;
		rectROI.top = 0;
	}
	if (rectROI.bottom > (CAM_VRES - 1))
	{
		pFidAlgo->m_ptFidROICenterOffset.dblPosY = rectROI.bottom - (CAM_VRES - 1);
		rectROI.bottom = CAM_VRES - 1;
	}

	pFidAlgo->m_ptFidROICenterOffset.dblPosX = pFidAlgo->m_ptFidROICenterOffset.dblPosX / 2;
	pFidAlgo->m_ptFidROICenterOffset.dblPosY = pFidAlgo->m_ptFidROICenterOffset.dblPosY / 2;

	return m_pMilCamProc->Child2D(&rectROI);
}


void CCamDlg::ClearView()
{
	m_pMilCamView->Copy(m_pMilCamProc);
	m_pMilCamDraw->Copy(m_pMilCamProc);
	DrawCross(m_pMilCamDraw);
}

void CCamDlg::DrawCross(CMilBuffer *pDrawBuffer)
{
	if (!CStatus::Instance()->m_bDrawCross)return;
	CRect rectScrn;
	GetClientRect(&rectScrn);
	double width = CAM_HRES - 1;
	double height = CAM_VRES - 1;
	pDrawBuffer->SetColor(RGB(100, 100, 100));
	pDrawBuffer->Line(0, 0, CAM_HRES - 1, CAM_VRES - 1);
	pDrawBuffer->Line(0, CAM_VRES - 1, CAM_HRES - 1, 0);
	CRect rectMiddle((int)(width / 4), (int)(height / 4), (int)(width - width / 4), (int)(height - height / 4));
	rectMiddle.DeflateRect(2, 2);
	for (int i = -2; i < 3; i++)
	{
		pDrawBuffer->Rect(rectMiddle);
		pDrawBuffer->Line(0, (height / 2) + i, width, (height / 2) + i);
		pDrawBuffer->Line((width / 2) + i, 0, (width / 2) + i, height);
		rectMiddle.InflateRect(1, 1);
	}
	//		pDrawBuffer->Line(, 1.0, (width / 2) +(double)i, height-1);
}


BOOL CCamDlg::IsPixelCalibrated()
{
	return FALSE;
}


BOOL CCamDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

//! Manual Image저장을 한다.
int CCamDlg::SaveManImg()
{
	CString strImgSaveFolder = CStatus::Instance()->m_strIMGSaveFolder;
	CString strTestImgSaveFolder;
	CString strFileName;
	static CString strPrevFileName = _T("");
	static int iIndex = 0;
	CTime curTime = CTime::GetCurrentTime();


	//strPrevFileName = strFileName;
	//CTime tmCurrent = CTime::GetCurrentTime();
	//CString strDate = tmCurrent.Format(_T("%y%m%d"));
	//CString strSaveFolder = strNGSaveFolder + _T("\\") + strDate;
	//CString strRemoteFolder = strDate;
	//CFileInfo::CreateFolder(strSaveFolder.GetBuffer());

	//strTestImgSaveFolder = strSaveFolder + _T("\\") + strFileName + _T(".bmp");
	//strNGSaveFolder = strSaveFolder + _T("\\") + strFileName + _T(".jpg");
	//strRemoteFolder = strRemoteFolder + _T("\\") + strFileName + _T(".jpg");
	//int iStartTime = GetTickCount();
	//m_pMilCamDraw->Export(strNGSaveFolder, M_JPEG_LOSSY);
	//if (CStatus::Instance()->m_bSaveTestImage)m_pMilCamProc->Export(strTestImgSaveFolder, M_BMP);
	//int iEndTime = GetTickCount();
	//TRACE(_T("\nSave Time %d"), iEndTime - iStartTime);
	//CStatus::Instance()->m_strRemoteImgName = strRemoteFolder;
	//BOOL bErr;
	//if (strRemoteFolder.GetLength() > 29)
	//{
	//	bErr = TRUE;
	//}
	return 0;
}


void CCamDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TIME_LEGENDUPDATE:
		LegendUpdate();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

//오버레이 overlay 상단 정보표시
void CCamDlg::LegendUpdate()
{
	CXNPIView* pView = (CXNPIView *)m_xnpiView;

	BOOL timeDisplay = pView->m_pOpPanelDlg->m_bTimeDisplay;
	BOOL pipeInfoDisplay = pView->m_pOpPanelDlg->m_bPipeInfoDisplay;

	CString strTagPipeInfo_1 = _T("");
	CString strTagPipeInfo_2 = _T("");
	CString strTagTimeInfo = _T("");

	//kiy 181015 - 아래 3개 문항 추가 view에 표시 할것
	CString strLeftSecName = _T("");
	CString strRightSecName = _T("");
	CString strCurPipePos = _T("");

	CString comp = pView->m_pOpPanelDlg->m_strPipeInfoComp;
	CString sepc = pView->m_pOpPanelDlg->m_strPipeInfoSEPC;
	CString size = pView->m_pOpPanelDlg->m_strPipeInfoSize;
	CString pipeNo = pView->m_pOpPanelDlg->m_strPipeInfoPipeNo;
	/*CString pipeSerial = pView->m_pOpPanelDlg->m_strPipeInfoSerial;

	if (pipeSerial.IsEmpty())
		pipeSerial = _T("None");*/

	CTime curTime = CTime::GetCurrentTime();
	CString strTime = curTime.Format(_T("%Y.%m.%d_%H:%M:%S"));

	if (pipeInfoDisplay)
	{
		//1번째 줄
		strTagPipeInfo_1 += CStatus::Instance()->m_strInfoLine1;

		//2번째 줄
		strTagPipeInfo_2 += CStatus::Instance()->m_strInfoLine2;
	}

	if (timeDisplay)
	{
		//3번째 줄
		strTagTimeInfo += _T("Time: ") + strTime;
	}

	MIL_DOUBLE TextPosX = 50.0;
	MIL_DOUBLE TextPosY = 30.0;
	MIL_DOUBLE StepYOffset = 38.0;

	MIL_DOUBLE TextLeftSectPosX = 50.0;
	MIL_DOUBLE TextLeftSectPosY = 1200.0;
	MIL_DOUBLE TextRightSectPosX = 1200.0;
	MIL_DOUBLE TextRightSectPosY = 1200.0;

	MgraFontScale(M_DEFAULT, 1.7, 1.7);

	//기존
	//MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
	//MgraColor(M_DEFAULT, M_COLOR_GREEN);
	//MgraText(M_DEFAULT, m_pMilCamDraw->GetIDBuffer(), TextPosX, TextPosY, strTagPipeInfo_1);
	//MgraText(M_DEFAULT, m_pMilCamDraw->GetIDBuffer(), TextPosX, TextPosY + StepYOffset, strTagPipeInfo_2);

	//Test check - 정보표시
	//투명배경 제거 및 글자 흰색- kiy - 0907
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_OPAQUE);
	MgraColor(M_DEFAULT, M_COLOR_GREEN);

	//m_pMilCamAvgResult_Histo -> m_pMilCamDraw로 바꿀것 아래는 테스트용, 181015
	MgraText(M_DEFAULT, m_pMilCamDraw->GetIDBuffer(), TextPosX, TextPosY, strTagPipeInfo_1);
	MgraText(M_DEFAULT, m_pMilCamDraw->GetIDBuffer(), TextPosX, TextPosY + StepYOffset, strTagPipeInfo_2);
	MgraText(M_DEFAULT, m_pMilCamDraw->GetIDBuffer(), TextPosX, TextPosY + StepYOffset * 2, strTagTimeInfo);
}

/*!
	@fn void CCamDlg::SetUseAverage(BOOL use)
	@brief AVG 사용여부 결정한다.o
*/
void CCamDlg::SetUseAverage(BOOL use)
{
	if (m_pGrabChannel != NULL)
		m_pGrabChannel->m_bUseImgAvg = use;
}

/*!
	@fn void CCamDlg::SetUseAverage(BOOL use)
	@brief AVG시 사용할 이미지 개수 결정
*/
void CCamDlg::SetAverageCount(int cnt)
{
	if (m_pGrabChannel != NULL)
	{
		m_pGrabChannel->m_bAvgIsDone = FALSE; //190328
		m_pGrabChannel->m_bSetAvg = TRUE;

		m_pGrabChannel->m_nAvgInputCnt = cnt;
	}
}

/*!
	@fn void CCamDlg::SetUseAverage(BOOL use)
	@brief AVG 완료여부
*/
BOOL CCamDlg::GetIsAverageDone()
{
	if (m_pGrabChannel != NULL)
		return m_pGrabChannel->m_bAvgIsDone;
	else
		return FALSE; //Test Check 원래 FALSE
}

//kiy - test용
//void CCamDlg::TestSendCallBackGrab(int i)
//{
//	if (m_pGrabChannel != NULL)
//		m_pGrabChannel->CallbackGrabEx(i, NULL, NULL);
//}
//-----