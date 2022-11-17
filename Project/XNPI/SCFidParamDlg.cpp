// SCFidParamDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "SCFidAlgo.h"
#include "SCFidParamDlg.h"
#include "SCAlgoDialog.h"
#include "Repository.h"
#include "Status.h"
#include "XNPIDoc.h"
#include "XNPIView.h"
#include "MainFrm.h"
#include "CamDlg.h"

// CSCFidParamDlg 대화 상자입니다.
CSCFidParamDlg * CSCFidParamDlg::m_pInstance = NULL;

IMPLEMENT_DYNAMIC(CSCFidParamDlg, CSCAlgoDialog)

CSCFidParamDlg::CSCFidParamDlg(CWnd* pParent /*=NULL*/)
	: CSCAlgoDialog(CSCFidParamDlg::IDD, pParent)
	, m_iScore(60)
	, m_InspXOfs(_T(""))
	, m_InspYOfs(_T(""))
	, m_ROIFactorW(0)
	, m_ROIFactorH(0)
{

}

CSCFidParamDlg::~CSCFidParamDlg()
{
	delete m_pSCAlgo;
}

void CSCFidParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE, m_FidImgView);
	DDX_Text(pDX, IDC_EDT_SCORE, m_iScore);
	DDX_Text(pDX, IDC_INSP_XOFS, m_InspXOfs);
	DDX_Text(pDX, IDC_INSP_YOFS, m_InspYOfs);
	DDX_Control(pDX, IDC_SP_FROIW, m_spFidROIW);
	DDX_Control(pDX, IDC_SP_FROIH, m_spFidROIH);
	DDX_Text(pDX, IDC_EDT_FROIW, m_ROIFactorW);
	DDX_Text(pDX, IDC_EDT_FROIH, m_ROIFactorH);
	DDX_Control(pDX, IDC_BTN_FIDREG, m_cReg);
	DDX_Control(pDX, IDC_BTN_TEST, m_cTest);
	DDX_Control(pDX, IDC_FIDTRIAL, m_cTrial);
}


BEGIN_MESSAGE_MAP(CSCFidParamDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_FIDREG, &CSCFidParamDlg::OnBnClickedBtnFidreg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_TEST, &CSCFidParamDlg::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDOK, &CSCFidParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_FIDTRIAL, &CSCFidParamDlg::OnBnClickedFidtrial)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSCFidParamDlg 메시지 처리기입니다.
/*! @fn void CFidSetDlg::GetFidParam(int FidNum)
@brief 보정마크에서 파라메터를 가져옴
*/
void CSCFidParamDlg::GetFidParam()
{
	CSCFidAlgo *pFidAlgo = (CSCFidAlgo *)m_pSCAlgo;
	m_iScore = pFidAlgo->m_iMatRate;
	m_spFidROIW.SetPos(pFidAlgo->m_SearchRangeFactor.dblCX);
	m_spFidROIH.SetPos(pFidAlgo->m_SearchRangeFactor.dblCY);
	m_ROIFactorW = pFidAlgo->m_SearchRangeFactor.dblCX;
	m_ROIFactorH = pFidAlgo->m_SearchRangeFactor.dblCY;
	//m_iScaleFactor = m_pRegFid->m_iScaleRate;
	//m_UseScaleFactor = m_pRegFid->m_iUseScale;
	//m_bUseAngleFactor = m_pRegFid->m_iUseAngle;
	//m_uiFidSizeWidth = m_pRegFid->m_upFid[FidNum].cx;
	//m_uiFidSizeHeight = m_pRegFid->m_upFid[FidNum].cy;
	//m_uiROISizeWidth = m_pRegFid->m_upROI[FidNum].cx;
	//m_uiROISizeHeight = m_pRegFid->m_upROI[FidNum].cy;
	UpdateData(FALSE);
}

/*! @fn void CFidSetDlg::ApplyFidParam(int FidNum)
@brief UI의 파라메터를 보정마크로 적용함
*/
void CSCFidParamDlg::ApplyFidParam()
{
	UpdateData(TRUE);
	CSCFidAlgo *pFidAlgo = (CSCFidAlgo *)m_pSCAlgo;
	pFidAlgo->m_iMatRate = m_iScore;
	pFidAlgo->m_SearchRangeFactor.dblCX = m_ROIFactorW;
	pFidAlgo->m_SearchRangeFactor.dblCY = m_ROIFactorH;
	//m_pRegFid->m_iScaleRate = m_iScaleFactor;
	//m_pRegFid->m_iUseScale = m_UseScaleFactor;
	//m_pRegFid->m_iUseAngle = m_bUseAngleFactor;

	//m_pRegFid->m_upFid[FidNum].cx = m_uiFidSizeWidth;
	//m_pRegFid->m_upFid[FidNum].cy = m_uiFidSizeHeight;
	//m_pRegFid->m_upROI[FidNum].cx = m_uiROISizeWidth;
	//m_pRegFid->m_upROI[FidNum].cy = m_uiROISizeHeight;
}


void CSCFidParamDlg::ShowInspResult(void)
{
	CSCFidAlgo *pFidAlgo = (CSCFidAlgo *)m_pSCAlgo;
//	m_InspAngle.Format(_T("%3.1f"), pFidAlgo->m_theta);
	m_InspXOfs.Format(_T("%5.1f"), pFidAlgo->m_upOffset.dblPosX);
	m_InspYOfs.Format(_T("%5.1f"), pFidAlgo->m_upOffset.dblPosY);
}


CSCFidParamDlg * CSCFidParamDlg::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSCFidParamDlg;
		m_pInstance->Create(IDD_FIDSET, NULL);
	}
	return m_pInstance;
}


void CSCFidParamDlg::OnBnClickedBtnFidreg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// rec위치의 Child를 만들고
	// 해당 위치의 
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	CRect FidROI;
	pView->m_pCamDlg[CStatus::Instance()->GetSelCamNo()]->GetFidROI(&FidROI);
	CSCFidAlgo *pSCAlgo = ((CSCFidAlgo *)m_pSCAlgo);
	if(!pSCAlgo->m_pMilBuffer)
		pSCAlgo->m_pMilBuffer = CRepository::Instance()->m_MilLib.NewImage();
	pSCAlgo->m_pMilBuffer->Close();
	pSCAlgo->m_pMilBuffer->SetDepth(8);
	pSCAlgo->m_pMilBuffer->SetAttribute(M_IMAGE + M_PROC + M_DISP);
	pSCAlgo->m_pMilBuffer->Alloc2D(CSize(FidROI.Width(), FidROI.Height()));
	pSCAlgo->m_pMilBuffer->Clear();
	pSCAlgo->m_pMilBuffer->SetDisplay(m_pMilFidDisp);
	pSCAlgo->m_pMilBuffer->SelectWindow(m_FidImgView);


	pView->m_pCamDlg[CStatus::Instance()->GetSelCamNo()]->m_pMilCamProc->CopyColor2D(pSCAlgo->m_pMilBuffer, &FidROI);
	pSCAlgo->RemoveAllModels();
	pSCAlgo->Reg(pSCAlgo->m_pMilBuffer, FidROI);
	ApplyFidParam();

}



BOOL CSCFidParamDlg::OnInitDialog()
{
	CSCAlgoDialog::OnInitDialog();
	m_pMilFidDisp = CRepository::Instance()->m_MilLib.NewDisplay();
	m_InspXOfs = _T("0.0");
	m_InspYOfs = _T("0.0");
	//m_spFidROIW.SetRange(20, 30);
	//m_spFidROIH.SetRange(20, 30);

	m_spFidROIW.SetDecimalPlaces(2);
	m_spFidROIW.SetTrimTrailingZeros(FALSE);
	m_spFidROIW.SetRangeAndDelta(2.0, 3.0, 0.1);
	m_spFidROIW.SetPos(2.0);
	m_spFidROIH.SetDecimalPlaces(2);
	m_spFidROIH.SetTrimTrailingZeros(FALSE);
	m_spFidROIH.SetRangeAndDelta(2.0, 3.0, 0.1);
	m_spFidROIH.SetPos(2.0);

	MakeDefaultFid();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSCFidParamDlg::MakeDefaultFid()
{
//	CSCFidAlgo *pFidAlgo = (CSCFidAlgo *)m_pSCAlgo;
	CSCFidAlgo *pFidAlgo = new CSCFidAlgo;
	m_pSCAlgo = pFidAlgo;
	pFidAlgo->m_pMilBuffer = CRepository::Instance()->m_MilLib.NewImage();
	pFidAlgo->m_pMilBuffer->SetDepth(8);
	pFidAlgo->m_pMilBuffer->SetAttribute(M_IMAGE + M_PROC + M_DISP);
	pFidAlgo->m_pMilBuffer->Alloc2D(CSize(30, 30));
	pFidAlgo->m_pMilBuffer->Clear();
	pFidAlgo->m_pMilBuffer->SetDisplay(m_pMilFidDisp);
	pFidAlgo->m_pMilBuffer->GetDisplay()->Control(M_FILL_DISPLAY, M_ENABLE);
	pFidAlgo->m_pMilBuffer->GetDisplay()->SetViewAutoScale();

	pFidAlgo->m_pMilBuffer->SelectWindow(m_FidImgView);
	///m_pRegFid의 ROI정보를 동기화시키기위해 사용됨
}


void CSCFidParamDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CSCAlgoDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSCFidAlgo *pSCAlgo = ((CSCFidAlgo *)m_pSCAlgo);
	if (!pSCAlgo->m_pMilBuffer)return;
	pSCAlgo->m_pMilBuffer->SetDisplay(m_pMilFidDisp);
	pSCAlgo->m_pMilBuffer->SelectWindow(m_FidImgView);
	GetFidParam();
	//UpdateData(FALSE);
}

void CSCFidParamDlg::LoadParam(CSCAlgoBase * pAlgo)
{
}

void CSCFidParamDlg::OnBnClickedBtnTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSCFidAlgo *pFidAlgo = (CSCFidAlgo *)m_pSCAlgo;
	if (pFidAlgo->IsEmpty())
	{
		AfxMessageBox(_T("Fid is not teached!"), MB_OK);
		return;
	}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	pView->m_pCamDlg[CStatus::Instance()->GetSelCamNo()]->RefreshView();
	pView->m_pCamDlg[CStatus::Instance()->GetSelCamNo()]->DrawAllTrack();

	CMilBuffer *m_pMilROIArea = pView->m_pCamDlg[CStatus::Instance()->GetSelCamNo()]->GetROIChild(pFidAlgo);
	if (!pFidAlgo->Find(m_pMilROIArea))
	{
		CRect ROIRect = GetROIRect();
		pFidAlgo->m_rectInsp = ROIRect;
//		pFidAlgo->SetDrawOffset(CSize(ROIRect.left, ROIRect.top));
		pFidAlgo->Draw();
		pFidAlgo->SetDrawOffset();
	}
	else
	{
		pFidAlgo->Draw(FALSE);
	}
	pView->m_pCamDlg[CStatus::Instance()->GetSelCamNo()]->DrawToView();

	m_pMilROIArea->Close();
	ShowInspResult();
	UpdateData(FALSE);
}


void CSCFidParamDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ApplyFidParam();
	OnOK();
}


void CSCFidParamDlg::OnBnClickedFidtrial()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	CRect FidROI = GetROIRect();
	return pView->DoTrial();
}

CRect CSCFidParamDlg::GetROIRect()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	CSize ImageSize;
	CSCFidAlgo *pFidAlgo = (CSCFidAlgo *)m_pSCAlgo;
	CRect FidROI;
	pView->m_pCamDlg[CStatus::Instance()->GetSelCamNo()]->GetFidROI(&FidROI);
	return FidROI;
}



void CSCFidParamDlg::CloseMod()
{
	((CSCFidAlgo *)m_pSCAlgo)->CloseMod();
}
