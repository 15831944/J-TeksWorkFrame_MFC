// ToolPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XFMS.h"
#include "ToolPanelDlg.h"
#include "afxdialogex.h"
#include "XFMSDoc.h"
#include "XFMSView.h"
#include "Status.h"
#include "Repository.h"
#include "MainFrm.h"
#include "SCButton.h"
#include "CamDlg.h"
#include "OpPanelDlg.h"
#include "TabDetailLogDlg.h"

// CToolPanelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CToolPanelDlg, CDialogEx)

CToolPanelDlg::CToolPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOOLPANEL, pParent)
	, m_bAVIMan(FALSE)
{

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
}


BEGIN_MESSAGE_MAP(CToolPanelDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_IMG_AS_TIME,	&CToolPanelDlg::OnImgAutoSaveTime)
	ON_MESSAGE(WM_IMG_AS_TEMP,	&CToolPanelDlg::OnImgAutoSaveTemp)
	ON_MESSAGE(WM_AVI_RECSTART,		&CToolPanelDlg::OnAVIAutoSaveStart)
	ON_MESSAGE(WM_AVI_RECSTOP,	&CToolPanelDlg::OnAVIAutoSaveStop)
	ON_MESSAGE(WM_AVI_RECRESTART,	&CToolPanelDlg::OnAVIAutoSaveReStart)
	ON_MESSAGE(WM_MAN_RECSTART, &CToolPanelDlg::OnAVIManSaveStart)
	ON_MESSAGE(WM_MAN_RECSTOP,	&CToolPanelDlg::OnAVIManSaveStop)
	ON_BN_CLICKED(IDC_BTN_MANSAVEIMG, &CToolPanelDlg::OnBnClickedBtnMansaveimg)
	ON_BN_CLICKED(IDC_BTN_MANSAVAVI, &CToolPanelDlg::OnBnClickedBtnMansavavi)
	ON_BN_CLICKED(IDC_BTN_DEVSAVE, &CToolPanelDlg::OnBnClickedBtnDevsave)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CToolPanelDlg 메시지 처리기입니다.


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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strImgSaveFolder = CStatus::Instance()->m_strIMGSaveFolder;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	CString strFileName = pView->m_pOpPanelDlg->GetManualFileName();

	strImgSaveFolder += _T("\\Manual");
	
	CTime tmCurrent = CTime::GetCurrentTime();
	CString strDate = tmCurrent.Format(_T("%y%m%d"));
	CString strSaveFolder = strImgSaveFolder + _T("\\") + strDate;
	CFileInfo::CreateFolder(strSaveFolder.GetBuffer());

	CString strFullFileName = strSaveFolder + _T("\\") + strFileName + _T(".JPG");
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Manual Image saving"), RGB(50, 50, 50), FALSE);

	pView->m_pCamDlg[0]->m_pMilCamView->Export(strFullFileName, M_JPEG_LOSSY);

}


void CToolPanelDlg::OnBnClickedBtnMansavavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static CString strFullFileName;
	int iDuration = GetDlgItemInt(IDC_MAS_DURATION);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog SaveFile(FALSE, _T(""), _T("*.TIF"), OFN_OVERWRITEPROMPT, _TEXT("TIF|*.TIF|BMP|*.BMP|JPG|*.JPG|"));
	if (SaveFile.DoModal() == IDOK)
	{
		CString strExt;
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
		strExt = PathFindExtension(SaveFile.GetPathName().GetBuffer());
		strExt.MakeUpper();

		int iFileType = M_TIFF;
		if (strExt.MakeLower() == ".BMP") iFileType = M_BMP;
		if (strExt.MakeLower() == ".JPG") iFileType = M_JPEG_LOSSY;
		if (strExt.MakeLower() == ".TIF") iFileType = M_TIFF;
		pView->m_pCamDlg[0]->m_pMilCamProc->Export(SaveFile.GetPathName(), iFileType);
	}

}


HBRUSH CToolPanelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(90, 107, 135));  // static text 글자색 변경
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


LRESULT CToolPanelDlg::OnImgAutoSaveTime(WPARAM wparam, LPARAM lparam)
{
	CString strImgSaveFolder = CStatus::Instance()->m_strIMGSaveFolder;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
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
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
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
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();

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

	pView->m_pCamDlg[0]->m_pGrabChannel->SetRecMilBuffer(pView->m_pCamDlg[0]->m_pMilCamView);
	pView->m_pCamDlg[0]->m_pMilCamView->ExportAVIOpen(&m_ExportAutoAVITag);

	return LRESULT();
}

LRESULT CToolPanelDlg::OnAVIAutoSaveStop(WPARAM wparam, LPARAM lparam)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();

	pView->m_pCamDlg[0]->m_pMilCamView->ExportAVIClose(&m_ExportAutoAVITag);
//	pView->m_pCamDlg[0]->m_pGrabChannel->SetRecMilBuffer(NULL);
	return LRESULT();
}

LRESULT CToolPanelDlg::OnAVIAutoSaveReStart(WPARAM wparam, LPARAM lparam)
{
	OnAVIAutoSaveStop(wparam, lparam);
	OnAVIAutoSaveStart(wparam, lparam);
	return LRESULT();
}

LRESULT CToolPanelDlg::OnAVIManSaveStart(WPARAM wparam, LPARAM lparam)
{
	static CString strFullFileName;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();

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

	pView->m_pCamDlg[0]->m_pMilCamView->m_bAVIClosed = TRUE;
	pView->m_pCamDlg[0]->m_pGrabChannel->SetRecMilBuffer(pView->m_pCamDlg[0]->m_pMilCamView);
	pView->m_pCamDlg[0]->m_pMilCamView->ExportAVIOpen(&m_ExportAVITag);

	return LRESULT();
}

LRESULT CToolPanelDlg::OnAVIManSaveStop(WPARAM wparam, LPARAM lparam)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	pView->m_pCamDlg[0]->m_pMilCamView->ExportAVIClose(&m_ExportAVITag);
	pView->m_pCamDlg[0]->m_pGrabChannel->SetRecMilBuffer(NULL);
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
		if (iDurationNext <= tmDiff.GetTotalMinutes())
		{
			pToolPanelDlg->SendMessage(WM_MAN_RECSTOP);
			bRecording = FALSE;
			Sleep(AVI_RESTART_DELAY);
			iDurationNext += iDuration;
			pToolPanelDlg->SendMessage(WM_MAN_RECSTART);
			bRecording = TRUE;
		}
		Sleep(AVISAVE_POLLING_TIME);
	}

	CStatus::Instance()->m_bIsAVIRecording = FALSE;
	if(bRecording)
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

BOOL CToolPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemInt(IDC_MAS_DURATION, 3);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
