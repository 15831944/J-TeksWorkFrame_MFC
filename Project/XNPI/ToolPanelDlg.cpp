// ToolPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "ToolPanelDlg.h"
#include "afxdialogex.h"
#include "XNPIDoc.h"
#include "XNPIView.h"
#include "Status.h"
#include "Repository.h"
#include "MainFrm.h"
#include "SCButton.h"
#include "CamDlg.h"
#include "OpPanelDlg.h"
#include "TabDetailLogDlg.h"
#include "SnapshotDlg.h"

#define DET_FRAME_RATE 18.7
#define IS_AVG_DONE_TIMER_ID 1000

// CToolPanelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CToolPanelDlg, CDialogEx)

CToolPanelDlg::CToolPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOOLPANEL, pParent)
	, m_bAVIMan(FALSE)
	, testNum(0)
	, m_sAvgProc(_T("Avg Proc."))
	, m_sAvgDone(_T("Avg Done"))
	//, m_sAvgNotUse(_T("Avg Not Use"))
	, m_bIsAvgDone(false)
	, m_strAvgVal(_T(""))
	, m_nSectionNum(-1)
	, m_bTriggered(FALSE)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pView = (CXNPIView*)pFrame->GetActiveView();
}

CToolPanelDlg::~CToolPanelDlg()
{
}

void CToolPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_MANSAVEIMG, m_cManSavImg);
	DDX_Control(pDX, IDC_BTN_MANSAVAVI, m_cManSavAVI);
	DDX_Control(pDX, IDC_BTN_DEVSAVE, m_cTiffSave);
	DDX_Control(pDX, IDC_BTN_AVERAGE_ACCEPT, m_cAvrApply);
	DDX_Control(pDX, IDC_AVG_DONE, m_avgIsDoneStatic);
	DDX_Text(pDX, IDC_MAS_AVERAGE, m_strAvgVal);
	DDX_Control(pDX, IDC_BTN_LOAD_IMG, m_btnLoadimg);
	DDX_Control(pDX, IDC_BUTTON_LOAD_INSP_FOLDER, m_btnLoadFolder);
}


BEGIN_MESSAGE_MAP(CToolPanelDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_IMG_AS_TIME, &CToolPanelDlg::OnImgAutoSaveTime)
	ON_MESSAGE(WM_IMG_AS_TEMP, &CToolPanelDlg::OnImgAutoSaveTemp)
	ON_MESSAGE(WM_AVI_RECSTART, &CToolPanelDlg::OnAVIAutoSaveStart)
	ON_MESSAGE(WM_AVI_RECSTOP, &CToolPanelDlg::OnAVIAutoSaveStop)
	ON_MESSAGE(WM_MAN_RECSTART, &CToolPanelDlg::OnAVIManSaveStart)
	ON_MESSAGE(WM_MAN_RECSTOP, &CToolPanelDlg::OnAVIManSaveStop)
	ON_BN_CLICKED(IDC_BTN_MANSAVEIMG, &CToolPanelDlg::OnBnClickedBtnMansaveimg)
	ON_BN_CLICKED(IDC_BTN_MANSAVAVI, &CToolPanelDlg::OnBnClickedBtnMansavavi)
	ON_BN_CLICKED(IDC_BTN_DEVSAVE, &CToolPanelDlg::OnBnClickedBtnDevsave)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_AVERAGE_ACCEPT, &CToolPanelDlg::OnBnClickedBtnAverageAccept)
	ON_BN_CLICKED(IDC_BTN_LOAD_IMG, &CToolPanelDlg::OnBnClickedBtnLoadImg)
	//	ON_WM_TIMER()
		//	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_INSP_FOLDER, &CToolPanelDlg::OnBnClickedButtonLoadInspFolder)
END_MESSAGE_MAP()


// CToolPanelDlg 메시지 처리기입니다.

BOOL CToolPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemInt(IDC_MAS_DURATION, 3);
	SetDlgItemInt(IDC_MAS_AVERAGE, 1);

	SetTimer(IS_AVG_DONE_TIMER_ID, 50, NULL);
	//SetTimer(1394, 50, NULL);	//kiy - grab test용
	//testNum = 0;	//kiy
	//testFlag = FALSE;	//kiy

	CString strAvg;
	strAvg.Format(_T("%d"), CStatus::Instance()->m_avg);
	m_strAvgVal = strAvg;

	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//m_pView = (CXNPIView*)pFrame->GetActiveView();
	//m_pView->m_pCamDlg[0]->m_pGrabChannel->SetOnAvgStatusCallback()

	CXNPIView *pView = (CXNPIView *)m_pView;

	//Test Check 190401
	//1. 기존 190329 두원 최종소스, 오프라인에서 쓰면 에러
	/*CXNPIView *pView = (CXNPIView *)m_pView;
	pView->m_pCamDlg[0]->m_pGrabChannel->SetOnAvgStatusCallback(
		std::bind(&CToolPanelDlg::OnReceiveAvgStatus, this,std::placeholders::_1));*/
	//2. 수정한 버전, 오프라인에서도 사용가능하나 실제 디텍터로 테스트 안해봄
	CVWChannel *pGrabCh = pView->m_pCamDlg[0]->m_pGrabChannel;
	if (pGrabCh != NULL)
	{
		pGrabCh->SetOnAvgStatusCallback(std::bind(&CToolPanelDlg::OnReceiveAvgStatus, this, std::placeholders::_1));
	}
	else 
	{
		//log
		{
			GetLocalTime(&m_tCur_time);

			CString strTime;
			strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
				m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

			CString str;
			str.Format(_T("[ERR] Grab Channel is NULL - SetOnAvgStatusCallback Faild!"));
			str = strTime + str;
			CTabDetailLogDlg::Instance()->SetTextLog(str, RGB(50, 50, 50), FALSE);
		}
		
		MessageBox(_T("ERROR: Grab Channel is NULL - SetOnAvgStatusCallback Faild!"), _T("In ToolPanelDlg OnInitDialog()"),
			MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND | MB_TOPMOST);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CToolPanelDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CToolPanelDlg::OnBnClickedBtnMansaveimg()
{
	CString strImgSaveFolder = CStatus::Instance()->m_strIMGSaveFolder;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	CString strFileName = pView->m_pOpPanelDlg->GetManualFileName();

	strImgSaveFolder += _T("\\Manual");

	CTime tmCurrent = CTime::GetCurrentTime();
	CString strDate = tmCurrent.Format(_T("%y%m%d"));
	CString strSaveFolder = strImgSaveFolder + _T("\\") + strDate;
	CFileInfo::CreateFolder(strSaveFolder.GetBuffer());

	CString strFullFileName = strSaveFolder + _T("\\") + strFileName + _T(".JPG");
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Manual Image saving"), RGB(50, 50, 50), FALSE);

	SaveImage(strFullFileName, pView->m_pCamDlg[0]->m_pMilCamView, M_JPEG_LOSSY);
}


void CToolPanelDlg::OnBnClickedBtnMansavavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static CString strFullFileName;
	int iDuration = GetDlgItemInt(IDC_MAS_DURATION);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();

	CString strRecBtn;
	GetDlgItemText(IDC_BTN_MANSAVAVI, strRecBtn);
	if (strRecBtn == _T("AVI"))
	{
		if (CStatus::Instance()->m_bIsAVIRecording)
		{
			AfxMessageBox(_T("Already, In recording !"));
			return;
		}
		AVIManSaveStarter();

		m_cManSavAVI.SetBlink(TRUE);
		SetDlgItemText(IDC_BTN_MANSAVAVI, _T("AVI-Stop"));

	}
	else
	{
		m_cManSavAVI.SetBlink(FALSE);
		m_bAVIMan = FALSE;
		CStatus::Instance()->m_bIsAVIRecording = FALSE;
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Manual AVI saving is stoped"), RGB(50, 50, 50), FALSE);
		SetDlgItemText(IDC_BTN_MANSAVAVI, _T("AVI"));
	}
}


void CToolPanelDlg::OnBnClickedBtnDevsave()
{
	CFileDialog SaveFile(FALSE, _T(""), _T("*.TIF"), OFN_OVERWRITEPROMPT, _TEXT("TIF|*.TIF|BMP|*.BMP|JPG|*.JPG|"));
	if (SaveFile.DoModal() == IDOK)
	{
		CString strExt;
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
		strExt = PathFindExtension(SaveFile.GetPathName().GetBuffer());
		strExt.MakeUpper();

		int iFileType = M_TIFF;
		if (strExt.MakeLower() == ".BMP") iFileType = M_BMP;
		if (strExt.MakeLower() == ".JPG") iFileType = M_JPEG_LOSSY;
		if (strExt.MakeLower() == ".TIF") iFileType = M_TIFF;

		//pView->m_pCamDlg[0]->m_pMilCamView->Export(SaveFile.GetPathName(), iFileType);
		//pView->m_pCamDlg[0]->m_pMilCamProc->Export(SaveFile.GetPathName(), iFileType); //오버레이 overlay 없음

		//SaveImage(SaveFile.GetPathName(), pView->m_pCamDlg[0]->m_pMilCamView, iFileType); //오버레이 overlay 있음, 24비트


		//pView->m_pCamDlg[0]->m_pMilCamProc->WndLvlTranform(pView->m_pCamDlg[0]->m_pMilCamProc, 12000, 13000, 0, 65535); //0919
		SaveImage(SaveFile.GetPathName(), pView->m_pCamDlg[0]->m_pMilCamProc, iFileType); //! 오버레이 overlay 없음, 16비트 저장
	}
}


HBRUSH CToolPanelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(90, 107, 135));  // static text 글자색 변경

		//! avg 완료 여부 표시
		if (pWnd->GetDlgCtrlID() == IDC_AVG_DONE)
		{
			pDC->SetBkMode(OPAQUE);

			if (m_bIsAvgDone)
				pDC->SetTextColor(RGB(255, 0, 0));
			else
				pDC->SetTextColor(RGB(0, 0, 0));
		}

		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	return hbr;
}


LRESULT CToolPanelDlg::OnImgAutoSaveTime(WPARAM wparam, LPARAM lparam)
{
	CString strImgSaveFolder = CStatus::Instance()->m_strIMGSaveFolder;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	CString strFileName = pView->m_pOpPanelDlg->GetManualFileName();

	strImgSaveFolder += _T("\\Auto");

	CTime tmCurrent = CTime::GetCurrentTime();
	CString strDate = tmCurrent.Format(_T("%y%m%d"));
	CString strSaveFolder = strImgSaveFolder + _T("\\") + strDate;
	CFileInfo::CreateFolder(strSaveFolder.GetBuffer());

	CString strFullFileName = strSaveFolder + _T("\\") + strFileName + _T(".JPG");

	pView->m_pCamDlg[0]->m_pMilCamView->Export(strFullFileName, M_JPEG_LOSSY);

	return LRESULT();
}


LRESULT CToolPanelDlg::OnImgAutoSaveTemp(WPARAM wparam, LPARAM lparam)
{
	CString strImgSaveFolder = CStatus::Instance()->m_strIMGSaveFolder;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	CString strFileName = pView->m_pOpPanelDlg->GetManualFileName();

	strImgSaveFolder += _T("\\Auto");

	CTime tmCurrent = CTime::GetCurrentTime();
	CString strDate = tmCurrent.Format(_T("%y%m%d"));
	CString strSaveFolder = strImgSaveFolder + _T("\\") + strDate;
	CFileInfo::CreateFolder(strSaveFolder.GetBuffer());

	CString strFullFileName = strSaveFolder + _T("\\") + strFileName + _T(".JPG");

	pView->m_pCamDlg[0]->m_pMilCamView->Export(strFullFileName, M_JPEG_LOSSY);

	return LRESULT();
}


LRESULT CToolPanelDlg::OnAVIAutoSaveStart(WPARAM wparam, LPARAM lparam)
{
	static CString strFullFileName;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();

	CString strAVISaveFolder = CStatus::Instance()->m_strAVISaveFolder;
	CString strFileName = pView->m_pOpPanelDlg->GetManualFileName();
	strAVISaveFolder += _T("\\Auto");

	CTime tmCurrent = CTime::GetCurrentTime();
	CString strDate = tmCurrent.Format(_T("%y%m%d"));
	CString strSaveFolder = strAVISaveFolder + _T("\\") + strDate;

	strFullFileName = strSaveFolder + _T("\\") + strFileName + _T(".AVI");
	CFileInfo::CreateFolder(strSaveFolder.GetBuffer());

	m_ExportAutoAVITag.strFileName = strFullFileName;
	m_ExportAutoAVITag.FileFormat = M_AVI_MJPG;
	m_ExportAutoAVITag.dblFrameRate = DET_FRAME_RATE;

	pView->m_pCamDlg[0]->m_pMilCamView->ExportAVIOpen(&m_ExportAutoAVITag);
	pView->m_pCamDlg[0]->m_pGrabChannel->SetRecMilBuffer(pView->m_pCamDlg[0]->m_pMilCamView);

	return LRESULT();
}

LRESULT CToolPanelDlg::OnAVIAutoSaveStop(WPARAM wparam, LPARAM lparam)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();

	pView->m_pCamDlg[0]->m_pMilCamView->ExportAVIClose(&m_ExportAutoAVITag);
	pView->m_pCamDlg[0]->m_pGrabChannel->SetRecMilBuffer(NULL);
	CStatus::Instance()->m_bIsAVIRecording = FALSE;
	return LRESULT();
}

LRESULT CToolPanelDlg::OnAVIManSaveStart(WPARAM wparam, LPARAM lparam)
{
	static CString strFullFileName;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();

	CString strAVISaveFolder = CStatus::Instance()->m_strAVISaveFolder;
	CString strFileName = pView->m_pOpPanelDlg->GetManualFileName();
	strAVISaveFolder += _T("\\Manual");

	CTime tmCurrent = CTime::GetCurrentTime();
	CString strDate = tmCurrent.Format(_T("%y%m%d"));
	CString strSaveFolder = strAVISaveFolder + _T("\\") + strDate;

	strFullFileName = strSaveFolder + _T("\\") + strFileName + _T(".AVI");
	CFileInfo::CreateFolder(strSaveFolder.GetBuffer());

	m_ExportAVITag.strFileName = strFullFileName;
	m_ExportAVITag.FileFormat = M_AVI_MJPG;
	m_ExportAVITag.dblFrameRate = DET_FRAME_RATE;

	pView->m_pCamDlg[0]->m_pMilCamView->ExportAVIOpen(&m_ExportAVITag);
	pView->m_pCamDlg[0]->m_pGrabChannel->SetRecMilBuffer(pView->m_pCamDlg[0]->m_pMilCamView);

	return LRESULT();
}

LRESULT CToolPanelDlg::OnAVIManSaveStop(WPARAM wparam, LPARAM lparam)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();

	pView->m_pCamDlg[0]->m_pMilCamView->ExportAVIClose(&m_ExportAVITag);
	pView->m_pCamDlg[0]->m_pGrabChannel->SetRecMilBuffer(NULL);
	CStatus::Instance()->m_bIsAVIRecording = FALSE;
	return LRESULT();
}

UINT CToolPanelDlg::ThreadAVIManSave(LPVOID pParam)
{
	CToolPanelDlg * pToolPanelDlg = (CToolPanelDlg *)pParam;
	tManSaveDuration AVIManSaveDuration = pToolPanelDlg->m_AVIManSaveDuration;
	CTime tmStart = AVIManSaveDuration.tmStart;
	CTimeSpan tmDiff;
	int iDuration = AVIManSaveDuration.iMinTimeDuration;
	int iDurationNext = iDuration;
	CTime curTime;
	CTime dtStartTime;
	dtStartTime = CTime::GetCurrentTime();
	CTimeSpan diffTime;
	BOOL bRecording = FALSE;

	pToolPanelDlg->SendMessage(WM_MAN_RECSTART);
	bRecording = TRUE;
	while (pToolPanelDlg->m_bAVIMan)
	{
		curTime = CTime::GetCurrentTime();
		tmDiff = curTime - tmStart;
		if (iDurationNext < tmDiff.GetTotalMinutes())
		{
			pToolPanelDlg->SendMessage(WM_MAN_RECSTOP);
			bRecording = FALSE;
			Sleep(15);
			iDurationNext += iDuration;
			pToolPanelDlg->SendMessage(WM_MAN_RECSTART);
			bRecording = TRUE;
		}
		Sleep(400);
	}

	if (bRecording)
		pToolPanelDlg->SendMessage(WM_MAN_RECSTOP);
	pToolPanelDlg->SendMessage(WM_XRAY_OFF);
	return 0;
}

void CToolPanelDlg::AVIManSaveStarter()
{
	CTime tmStart;

	int iDuration = GetDlgItemInt(IDC_MAS_DURATION);
	if (iDuration <= 0)
	{
		iDuration = 3;
		SetDlgItemInt(IDC_MAS_DURATION, 3);
	}

	tmStart = CTime::GetCurrentTime();

	CTabDetailLogDlg::Instance()->SetTextLog(_T("Manual AVI saving is started"), RGB(50, 50, 50), FALSE);

	m_AVIManSaveDuration.tmStart = tmStart;
	m_AVIManSaveDuration.iMinTimeDuration = iDuration;

	m_bAVIMan = TRUE;
	CStatus::Instance()->m_bIsAVIRecording = TRUE;

	AfxBeginThread(ThreadAVIManSave, this);
}

//kiy--------------------------------------------------
void CToolPanelDlg::OnBnClickedBtnAverageAccept()
{
	m_bIsAvgDone = false;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();

	int avgMaxLimit = 1000; //kiy 0906 - avg 최대값(따로 말은 없었으나 임의로 넣음)

	long avgMinLimit = CStatus::Instance()->m_avgMinLimit; //kiy 0906 - avg 최소값

	int avgInputCnt = GetDlgItemInt(IDC_MAS_AVERAGE);

	if (avgInputCnt < avgMinLimit)
		avgInputCnt = avgMinLimit;
	if (avgInputCnt > avgMaxLimit)
		avgInputCnt = avgMaxLimit;

	//log
	{
		GetLocalTime(&m_tCur_time);

		CString strTime;
		strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
			m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

		CString str;
		str.Format(_T("[SYS] Apply Average : %d"), avgInputCnt);
		str = strTime + str;
		CTabDetailLogDlg::Instance()->SetTextLog(str, RGB(50, 50, 50), FALSE);
	}

	pView->m_pCamDlg[0]->SetAverageCount(avgInputCnt);

	CStatus::Instance()->m_avg = avgInputCnt;

	SetDlgItemInt(IDC_MAS_AVERAGE, CStatus::Instance()->m_avg);


}

void CToolPanelDlg::OnBnClickedBtnLoadImg()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();

	BOOL isCamActivated = pView->m_pOpPanelDlg->m_bCamIsActivated;
	if (!isCamActivated)
	{
		CFileDialog LoadFile(TRUE, _TEXT("*.tif;*.bmp;*.jpg"));
		if (LoadFile.DoModal())
		{
			CString path = (WCHAR *)(LoadFile.GetPathName().GetString());
			pView->m_pCamDlg[0]->m_pMilCamView->BufLoad(path); //TEST CHECK - 원본
		}
	}
	else
	{
		AfxMessageBox(_T("Cam Activated!"), MB_OK);
	}
}

BOOL CToolPanelDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CWnd* hWnd = GetFocus();
			CEdit* edit = (CEdit*)(hWnd);

			if (IDC_MAS_AVERAGE == edit->GetDlgCtrlID())
			{
				OnBnClickedBtnAverageAccept();
			}

			return TRUE;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CToolPanelDlg::OnTimer(UINT_PTR nIDEvent)
{
	static BOOL m_bEntered = FALSE;
	//! Avg 완료 여부 표시m_bIsAvgDone
	if (m_bEntered)
	{
		CDialogEx::OnTimer(nIDEvent);
		return;
	}

	switch (nIDEvent)
	{
	case IS_AVG_DONE_TIMER_ID:
	{
		if (m_bTriggered && m_bIsAvgDone)
		{
			m_bEntered = TRUE;
			//log
			{
				GetLocalTime(&m_tCur_time);

				CString strTime;
				strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
					m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

				CString log = strTime + _T("[SYS] Average DONE - TRIGGER_RECV");
				CTabDetailLogDlg::Instance()->SetTextLog(log, RGB(50, 50, 50), FALSE);
			}

			m_bTriggered = FALSE;

			//! Avg 완료후 SnapshotDlg의 TakeImage 메서드 동작
			/*CMilBuffer *tempMilBuf = m_pView->m_pCamDlg[0]->m_pGrabChannel->m_pMilTempBuf;
			tempMilBuf->Copy(m_pView->m_pCamDlg[0]->m_pMilCamAvgResult_Histo);*/

			OutputDebugString(_T("\nToolPanel TakeImage call"));
			CXNPIView *pView = (CXNPIView *)m_pView;
			pView->m_pSnapshotDlg->TakeImage(m_nSectionNum);

			m_nSectionNum = -1;
		}
	}
	break;
	}
	m_bEntered = FALSE;
	CDialogEx::OnTimer(nIDEvent);
}

void CToolPanelDlg::SaveImage(CString path, CMilBuffer* milBuf, MIL_INT fileType)
{
	CString strExt = path;
	strExt.MakeUpper();

	milBuf->Export(strExt, fileType);
}

void CToolPanelDlg::Triggered(int sectionNum)
{
	m_bTriggered = TRUE;
	m_nSectionNum = sectionNum;

	OnBnClickedBtnAverageAccept(); //Test Check - avg 맞는 타이밍에 되는가?
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
		// 폴더선택 다이얼로그의 초기화가 끝난 경우
	case BFFM_INITIALIZED:
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	}
	return 0;
}

void CToolPanelDlg::OnBnClickedButtonLoadInspFolder()
{
	ITEMIDLIST  *pidlBrowse;
	WCHAR       pszPathname[MAX_PATH];

	BROWSEINFO  BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0x00, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select folder");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	// 이벤트에 대한 사용자정의 함수
	BrInfo.lpfn = BrowseCallbackProc;

	CString strInitPath = CStatus::Instance()->m_strIMGSaveFolder;
	strInitPath += _T("\\Pipe Inspection");
	BrInfo.lParam = (LPARAM)strInitPath.GetBuffer();

	pidlBrowse = ::SHBrowseForFolder(&BrInfo);
	if (pidlBrowse != NULL)
	{
		// 선택된 폴더 경로얻음
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// 경로(pszPathname)를 이용하여 이후작업 추가
	}
}

void CToolPanelDlg::OnReceiveAvgStatus(bool isAvgDone) //190327
{
	m_bIsAvgDone = isAvgDone;

	if (m_bIsAvgDone)
		m_avgIsDoneStatic.SetWindowText(m_sAvgDone);
	else
		m_avgIsDoneStatic.SetWindowText(m_sAvgProc);

	/*	if (m_bTriggered && m_bIsAvgDone)
		{
			//log
			{
				GetLocalTime(&m_tCur_time);

				CString strTime;
				strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
					m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

				CString log = strTime + _T("[SYS] Average DONE - TRIGGER_RECV");
				CTabDetailLogDlg::Instance()->SetTextLog(log, RGB(50, 50, 50), FALSE);
			}

			m_bTriggered = FALSE;

			//! Avg 완료후 SnapshotDlg의 TakeImage 메서드 동작
			//CXNPIView *pView = (CXNPIView *)m_pView;
			//pView->m_pSnapshotDlg->TakeImage(m_nSectionNum);

			m_nSectionNum = -1;
		}*/
}
//---------------------------------------------------------------------------