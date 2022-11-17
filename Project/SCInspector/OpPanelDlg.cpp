// OpPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "OpPanelDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "SCInspectorDoc.h"
#include "SCInspectorView.h"
#include "MainFrm.h"
#include "CamDlg.h"
#include "EnvSettingDlg.h"
#include "Repository.h"
#include "TabDetailLogDlg.h"
#include "MessageWnd.h"
#include "OpenModelDataDlg.h"
#include "SaveModelDataDlg.h"
#include "Repository.h"
#include "CamRemoteDlg.h"

// COpPanelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COpPanelDlg, CDialogEx)

COpPanelDlg::COpPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPPANEL, pParent)
	, m_SCInspPacket(m_serial)
	, m_bReceived(FALSE)
{

}

COpPanelDlg::~COpPanelDlg()
{
}

void COpPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELCAM, m_ctrlSelCam);
	DDX_Control(pDX, IDC_RUNSTOP, m_cRunStop);
	DDX_Control(pDX, IDC_BTN_GRAB, m_cGrabImage);
	DDX_Control(pDX, IDC_SAVEIMG, m_cSaveImage);
	DDX_Control(pDX, IDC_BTN_INSPIMAGE, m_cSaveInspImg);
	DDX_Control(pDX, IDC_BTN_LOADIMAGE, m_cLoadImage);
	DDX_Control(pDX, IDC_BTN_LOADJOBFILE, m_cLoadJobFile);
	DDX_Control(pDX, IDC_BTN_SAVEJOBFILE, m_cSaveJobFile);
	DDX_Control(pDX, IDC_BTN_SAVEAS, m_cSaveAs);
	DDX_Control(pDX, IDC_BTN_TEACHING, m_cTeaching);
	DDX_Control(pDX, IDC_BTNTRIAL, m_cTrial);
	DDX_Control(pDX, IDC_SETENV, m_cSetEnv);
}


BEGIN_MESSAGE_MAP(COpPanelDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_SELCAM, &COpPanelDlg::OnCbnSelchangeSelcam)
	ON_BN_CLICKED(IDC_BTN_LOADIMAGE, &COpPanelDlg::OnBnClickedBtnLoadimage)
	ON_BN_CLICKED(IDC_SETENV, &COpPanelDlg::OnBnClickedSetenv)
	ON_BN_CLICKED(IDC_SAVEIMG, &COpPanelDlg::OnBnClickedSaveimg)
	ON_BN_CLICKED(IDC_BTN_INSPIMAGE, &COpPanelDlg::OnBnClickedBtnInspimage)
	ON_BN_CLICKED(IDC_BTN_TEACHING, &COpPanelDlg::OnBnClickedBtnTeaching)
	ON_BN_CLICKED(IDC_BTN_LOADJOBFILE, &COpPanelDlg::OnBnClickedBtnLoadjobfile)
	ON_BN_CLICKED(IDC_BTN_SAVEJOBFILE, &COpPanelDlg::OnBnClickedBtnSavejobfile)
	ON_BN_CLICKED(IDC_BTN_GRAB, &COpPanelDlg::OnBnClickedBtnGrab)
	ON_BN_CLICKED(IDC_CHKACTIVEALLCAM, &COpPanelDlg::OnBnClickedChkactiveallcam)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RUNSTOP, &COpPanelDlg::OnBnClickedRunstop)
	ON_BN_CLICKED(IDC_BTN_SAVEAS, &COpPanelDlg::OnBnClickedBtnSaveas)
	ON_BN_CLICKED(IDC_BTNTRIAL, &COpPanelDlg::OnBnClickedBtntrial)
	ON_BN_CLICKED(IDC_BTN_OPTEST, &COpPanelDlg::OnBnClickedBtnOptest)
	ON_WM_ERASEBKGND()
//	ON_WM_CTLCOLOR()
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// COpPanelDlg 메시지 처리기입니다.


BOOL COpPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;;
	CString strCamNo;
	for (int i = 0; i < m_nCountOfCam; i++)
	{
		strCamNo.Format(_T("%d"), i+1);
		m_ctrlSelCam.AddString(strCamNo);
	}
	m_ctrlSelCam.SetCurSel(0);

	SerialCheckNInit();
	LightOn();
	SetTimer(TIMEREVENT_DELOLDFILE, 3600*1000, NULL);
	SetWindowTheme(GetDlgItem(IDC_CHKACTIVEALLCAM)->m_hWnd, _T(""), _T(""));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void COpPanelDlg::OnCbnSelchangeSelcam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int CurSel = m_ctrlSelCam.GetCurSel();
	CStatus::Instance()->SetSelCam(CurSel);
}


void COpPanelDlg::OnBnClickedBtnLoadimage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog LoadFile(TRUE, _T(""), _TEXT("*.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _TEXT("BMP|*.BMP|TIF|*.TIF|"));
	if (LoadFile.DoModal()==IDOK)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CSCInspectorView *pView = (CSCInspectorView *)pFrame->GetActiveView();
		int nCurSel = m_ctrlSelCam.GetCurSel();

		CString strExt;
		strExt = CStatus::Instance()->GetModelData()->strClip(LoadFile.GetPathName(), 2);
		strExt.MakeUpper();
		MIL_INT typeImage;
		if (strExt == _T(".BMP"))
		{
			typeImage = M_BMP;
		}else
		if (strExt == _T(".TIF"))
		{ 
			typeImage = M_TIFF;
		}
		CMilBuffer *pImage = CRepository::Instance()->m_MilLib.NewImage();
		pImage->Restore(LoadFile.GetPathName());
		pView->m_pCamDlg[nCurSel]->LoadBufImage(pImage);
//		pView->m_pCamDlg[nCurSel]->m_pMilCamView->BufImport(LoadFile.GetPathName(), typeImage);
		pView->m_pCamDlg[nCurSel]->m_pMilDisp->FilWindow();
		pImage->Close();
	}

}


void COpPanelDlg::OnBnClickedSetenv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEnvSettingDlg dlg;
	dlg.DoModal();
}


void COpPanelDlg::OnBnClickedSaveimg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog SaveFile(FALSE, _T(""), _T("*.bmp"), OFN_OVERWRITEPROMPT, _TEXT("BMP|*.bmp|JPG|*.jpg|"));
	if (SaveFile.DoModal() == IDOK)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CSCInspectorView *pView = (CSCInspectorView *)pFrame->GetActiveView();
		int nCurSel = m_ctrlSelCam.GetCurSel();
		pView->m_pCamDlg[nCurSel]->m_pMilCamProc->Export(SaveFile.GetPathName(), M_BMP);
	}
}

void COpPanelDlg::OnBnClickedBtnInspimage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog SaveFile(FALSE, _T(""), _T("*.jpg"), OFN_OVERWRITEPROMPT, _TEXT("|JPG|*.jpg|"));
	if (SaveFile.DoModal() == IDOK)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CSCInspectorView *pView = (CSCInspectorView *)pFrame->GetActiveView();
		int nCurSel = m_ctrlSelCam.GetCurSel();

		pView->m_pCamDlg[nCurSel]->m_pMilCamView->Export(SaveFile.GetPathName(), M_JPEG_LOSSY);
	}
}


int COpPanelDlg::InitLight()
{
	return 0;
}


int COpPanelDlg::SerialCheckNInit()
{
	if (m_serial.IsOpen())
	{
		m_serial.Close();
	}

	m_serial.RegCallback(std::bind(&COpPanelDlg::ProcessCMD, this, std::placeholders::_1));
	m_serial.RegAnalizerCallback(std::bind(&COpPanelDlg::CheckoutPacket, this));

	CString  strSerialNo = CStatus::Instance()->GetComportNo();
	int iBaudRate = CStatus::Instance()->GetBaudRate();
	CSerial::EBaudrate BaudRate;
	switch (iBaudRate)
	{
	case 9600://9600
		BaudRate = CSerial::EBaud9600;
		break;
	case 19200://19200
		BaudRate = CSerial::EBaud19200;
		break;
	case 38400://38400
		BaudRate = CSerial::EBaud38400;
		break;
	case 115200://115200
		BaudRate = CSerial::EBaud115200;
		break;
	default:
		BaudRate = CSerial::EBaud9600;
	}

	strSerialNo = _T("\\\\.\\") + strSerialNo;

	// 필수 -
	if (m_serial.Open(strSerialNo.GetBuffer(), this, 0, 0, 16, 16) == ERROR_SUCCESS)
	{
		//		VERIFY(m_serial.SetupHandshaking(CSerial::EHandshake::EHandshakeOff) == ERROR_SUCCESS);
		VERIFY(m_serial.Setup(BaudRate, CSerial::EData8, CSerial::EParNone, CSerial::EStop1) == ERROR_SUCCESS);
		VERIFY(m_serial.SetMask(CSerial::EEventBreak | CSerial::EEventError | CSerial::EEventRecv) == ERROR_SUCCESS);
	}
	// - 필수

	if (!m_serial.IsOpen())
	{
		AfxMessageBox(_T("Unable to open COM-port"), MB_ICONSTOP | MB_OK);
	}

	CStatus::Instance()->SetComm(&m_SCInspPacket);
	return 0;
}


void COpPanelDlg::ProcessCMD(int nEvent)
{
	RcvSerialMsg((WPARAM)nEvent);
	SetFlagReceived();
}

int COpPanelDlg::RcvSerialMsg(WPARAM wParam)
{
	CSerial::EEvent eEvent = (CSerial::EEvent)wParam;

	static int prevTime = 0;
	static int prevNum = 0;
	static int Packet0Time = 0;
	static int Packet255Time = 0;
	USES_CONVERSION;
	CSCInspPacket::EPacketType eType = CSCInspPacket::EPacketType::None;

	if (eEvent == CSerial::EEvent::EEventRecv)
		eType = m_SCInspPacket.CheckoutPacket();
	else
		TRACE(_T("Serial Event: %d\n"), eEvent);

	if (eType == CSCInspPacket::None) return -1;

	//if (prevTime == 0)prevTime = timeGetTime();
	//int curTime = timeGetTime();
	//printf("\nInterval Packet %dms",curTime-prevTime);
	//prevTime = curTime;

	switch (eType)
	{
	case 0:
		break;
	default:
		break;
	}
	return 0;
}

void COpPanelDlg::SetFlagReceived()
{
	m_bReceived = TRUE;
}

BOOL COpPanelDlg::IsCMDReceived()
{
	BOOL bReturn;
	bReturn = m_bReceived;
	m_bReceived = FALSE;
	return bReturn;
}

int COpPanelDlg::CheckoutPacket()
{
	return m_SCInspPacket.CheckoutPacket();
}

void COpPanelDlg::LightOn()
{
	CStatus::Instance()->GetComm()->LightOnOff(CSCInspPacket::LIGHT_ON, CStatus::Instance()->m_LightBrightness);
}


void COpPanelDlg::OnBnClickedBtnTeaching()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (CStatus::Instance()->GetOpMode() == CStatus::OPAUTO)
		OnBnClickedRunstop();
	CSCInspectorView *pView = (CSCInspectorView *)pFrame->GetActiveView();
	pView->SetOPMode(CStatus::OPTEACHING);
}


void COpPanelDlg::OnBnClickedBtnLoadjobfile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COpenModelDataDlg dlg;
//	dlg.FindModelData();
	dlg.DoModal();
}


void COpPanelDlg::OnBnClickedBtnSavejobfile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		AfxMessageBox(_T("COM ERROR\r\nCoInitialize Failed!"));
		return;
	}
	CStatus::Instance()->GetModelData()->SetModelPath(CStatus::Instance()->GetModelRepository().GetBuffer());
	CStatus::Instance()->GetModelData()->SetMDName(CStatus::Instance()->GetModelName().GetBuffer());
	CStatus::Instance()->GetModelData()->SaveModelData();
}

void COpPanelDlg::OnBnClickedBtnGrab()
{
	// TODO: 여기에 컨트롤 알림 처리기 
	UpdateData(TRUE);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CSCInspectorView *pView = (CSCInspectorView *)pFrame->GetActiveView();
	if(CStatus::Instance()->GetSelCamNo() <= (CStatus::Instance()->m_nCountOfCam-1))
		pView->m_pCamDlg[CStatus::Instance()->GetSelCamNo()]->DoGrab();

}


void COpPanelDlg::OnBnClickedChkactiveallcam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CSCInspectorView *pView = (CSCInspectorView *)pFrame->GetActiveView();
	int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	if (IsDlgButtonChecked(IDC_CHKACTIVEALLCAM))
	{
		SetTimer(TIMEREVENT_GRAPCONTINUOUS, 50, NULL);
		//for (int i = 0; i<m_nCountOfCam; i++)
		//	pView->m_pCamDlg[i]->m_pEuresysChannel->GrabContinuous();
	}
	else
	{
		KillTimer(TIMEREVENT_GRAPCONTINUOUS);
		//for (int i = 0; i < m_nCountOfCam; i++)
		//	pView->m_pCamDlg[i]->m_pEuresysChannel->GrabStop();
	}
}


void COpPanelDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CSCInspectorView *pView = (CSCInspectorView *)pFrame->GetActiveView();
	int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;

	switch (nIDEvent)
	{
	case TIMEREVENT_GRAPCONTINUOUS:
		for (int i = 0; i < m_nCountOfCam; i++)
			pView->m_pCamDlg[i]->DoGrab();
		break;
	case TIMEREVENT_DELOLDFILE:
		if(CStatus::Instance()->m_bUseDelOldImg)DeleteOldFile();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void COpPanelDlg::OnBnClickedRunstop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strRunStopBtn;
	GetDlgItemText(IDC_RUNSTOP, strRunStopBtn);
	if (strRunStopBtn == _T("Run"))
	{
		CStatus::Instance()->SetOPMode(CStatus::OPAUTO);
		SetDlgItemText(IDC_RUNSTOP, _T("Stop"));
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Online Mode Start"), RGB(50, 50, 50), FALSE);
	}
	else
	{
		CStatus::Instance()->SetOPMode(CStatus::OPSTOP);
		SetDlgItemText(IDC_RUNSTOP, _T("Run"));
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Offline Mode Start"), RGB(50, 50, 50), FALSE);
	}
}


void COpPanelDlg::OnBnClickedBtnSaveas()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSaveModelDataDlg dlg;
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		AfxMessageBox(_T("COM ERROR\r\nCoInitialize Failed!"));
		return;
	}

	dlg.DoModal();

//	CStatus::Instance()->GetModelData()->SaveModelData(SaveFile.GetPathName());

	//CFileDialog SaveFile(FALSE, _T(""), _T("*.INI"), OFN_OVERWRITEPROMPT, _TEXT("INI|*.INI|"));
	//if (SaveFile.DoModal()==IDOK)
	//{
	//}
}


void COpPanelDlg::OnBnClickedBtntrial()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CSCInspectorView *pView = (CSCInspectorView *)pFrame->GetActiveView();
	int nCurSel = m_ctrlSelCam.GetCurSel();
	pView->DoTrial();
}


void COpPanelDlg::OnBnClickedBtnOptest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CSCInspectorView *pView = (CSCInspectorView *)pFrame->GetActiveView();
	int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;

	AfxBeginThread(ThreadTest, pView);
	//CString strFileName = _T("d:\\test.jpg");
	//pView->m_pCamRemoteDlg[0]->LoadFile(strFileName);
//	pView->m_pCamDlg[0]->SaveImage();
	//	DeleteOldFile();
//	pView->m_pCamDlg[0]->SaveNGImage();
	CSCResult test;
	//test.bIsPixelCal = 0;
	//test.CamNo = 1;
	//test.dblMax = 15;
	//test.dblMean = 10;
	//test.dblMin = 5;
	//test.umMax = 15.11;
	//test.umMean = 10.11;
	//test.umMin = 5.11;

	//CRepository::Instance()->WriteInspResult(&test);
}


void COpPanelDlg::DeleteOldFile()
{
	CString strNGSaveFolder = CStatus::Instance()->m_strNGSaveFolder;

	CFileFind NGFolderFinder;
	BOOL bFind = NGFolderFinder.FindFile(strNGSaveFolder + _T("\\*.*"));
	while (bFind)
	{
		bFind = NGFolderFinder.FindNextFileW();
		if (NGFolderFinder.IsDirectory())
		{
			CString strName = NGFolderFinder.GetFileName();
			if (strName.GetLength() == 6)
			{
				CTime tmDate;
				if (IsDate(strName, &tmDate))
				{
					if (DiffDate(&tmDate) > CStatus::Instance()->m_nDaysDel)
					{
						DelFolder(NGFolderFinder.GetFilePath());
					}
				}
			}
		}
	}
	

}

BOOL COpPanelDlg::IsDate(CString strName, CTime *pTime)
{
	int nLength = strName.GetLength();
	if (nLength != 6)return FALSE;
	for (int i = 0; i < nLength; i++)
	{
		if ( ('0' > strName.GetAt(i)) || ('9' < strName.GetAt(i)) )
			return FALSE;
	}
	
	CString strYear = strName.Left(2);
	CString strMon = strName.Mid(2, 2);
	CString strDay = strName.Right(2);
	
	int iYear = _wtoi(strYear)+2000;
	int iMon = _wtoi(strMon);
	int iDay = _wtoi(strDay);
	CTime tmDate(iYear, iMon, iDay, 0, 0, 0);

	*pTime = tmDate;

	return TRUE;
}


int COpPanelDlg::DiffDate(CTime * pDate)
{
	CTime tmCurrent = CTime::GetCurrentTime();
	CTimeSpan tmDiff = tmCurrent - *pDate;
	return tmDiff.GetDays();
}


void COpPanelDlg::DelFolder(CString strFolder)
{
	CFileInfo::SHDeleteFolder(strFolder);
}


BOOL COpPanelDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


//HBRUSH COpPanelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  여기서 DC의 특성을 변경합니다.
//	switch (nCtlColor)
//	{
//	case CTLCOLOR_STATIC:
//		hbr = CreateSolidBrush(RGB(255, 255, 0));
//		pDC->SetBkColor(RGB(255, 0, 0));
//		break;
//	}
//
//	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
//	return hbr;
//}


HBRUSH COpPanelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


UINT COpPanelDlg::ThreadTest(LPVOID pParam)
{
	CSCInspectorView *pView = (CSCInspectorView *)pParam;
	BOOL m_bRun = TRUE;
	for(int i=0;i<1000;i++)
	{
		pView->m_pCamDlg[0]->SaveImage();
		Sleep(200);
	}

	return 0;	return 0;
}
