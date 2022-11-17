// OpPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "OpPanelDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "XNPIDoc.h"
#include "XNPIView.h"
#include "MainFrm.h"
#include "CamDlg.h"
#include "EnvSettingDlg.h"
#include "Repository.h"
#include "TabDetailLogDlg.h"
#include "MessageWnd.h"
#include "OpenModelDataDlg.h"
#include "SaveModelDataDlg.h"
#include "Repository.h"
#include "ModBusClient.h"
#include "HistoLevellingDlg.h"
#include "GraphDlg.h"
#include "ToolPanelDlg.h"
#include "Defines.h"
#include "ViVIX.T_def.h"
#include "ViVIX.T_API.h"
using namespace VWSDK;
using namespace VWSDK::T;
// COpPanelDlg 대화 상자입니다.2

IMPLEMENT_DYNAMIC(COpPanelDlg, CDialogEx)

COpPanelDlg::COpPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPPANEL, pParent)
	, m_XFMSPacket(m_serial)
	, m_bReceived(FALSE)
	, m_strMakCondition(_T(""))
	, m_bWriteCSV(FALSE)
	, m_strFullCSVFileName(_T(""))
	, m_pToolPanelDlg(NULL)
	, m_strPipeInfoComp(_T(""))
	, m_strPipeInfoSEPC(_T(""))
	, m_strPipeInfoSize(_T(""))
	, m_strPipeInfoPipeNo(_T(""))
	, m_strFpsValue(_T(""))
	, m_strKeV(_T(""))
	, m_strMA(_T(""))
	, m_strPipeInfoSerial(_T(""))
	, m_bCamIsActivated(FALSE)
	, m_bTimeDisplay(TRUE)
	, m_bPipeInfoDisplay(TRUE)
	, m_bSectionNumDisplay(TRUE)
	, m_bDistanceDisplay(TRUE)
	, m_strInfoLine1(_T(""))
	, m_strInfoLine2(_T(""))
{

}

COpPanelDlg::~COpPanelDlg()
{
}

void COpPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETENV, m_cSetEnv);
	DDX_Control(pDX, IDC_BTN_XRAYON, m_cBtnXrayON);
	DDX_Control(pDX, IDC_BTN_XRAYOFF, m_cBtnXrayOFF);
	//DDX_Control(pDX, IDC_ADDMARK, m_cMarkAdd);
	DDX_Control(pDX, IDC_EDT_VOL, m_cVol);
	DDX_Control(pDX, IDC_EDT_CUR, m_cCur);
	DDX_Text(pDX, IDC_EDIT_PIPEINFO_COMP, m_strPipeInfoComp);
	DDX_Text(pDX, IDC_EDIT_PIPEINFO_SEPC, m_strPipeInfoSEPC);
	DDX_Text(pDX, IDC_EDIT_PIPEINFO_SIZE, m_strPipeInfoSize);
	DDX_Text(pDX, IDC_EDIT_PIPEINFO_PIPE_NO, m_strPipeInfoPipeNo);
	DDX_Control(pDX, IDC_COMBO_DRIVE_MODE, m_cbDriveMode);
	DDX_Text(pDX, IDC_STATIC_FPS_VAL, m_strFpsValue);
	DDX_Text(pDX, IDC_EDT_VOL, m_strKeV);
	DDX_Text(pDX, IDC_EDT_CUR, m_strMA);
	DDX_Text(pDX, IDC_EDIT_PIPEINFO_PIPE_SERIAL, m_strPipeInfoSerial);
	DDX_Check(pDX, IDC_CHECK_TIME_SHOW, m_bTimeDisplay);
	DDX_Check(pDX, IDC_CHKACTIVEALLCAM, m_bCamIsActivated);
	DDX_Check(pDX, IDC_CHECK_All_INFO_HIDE, m_bPipeInfoDisplay);
	DDX_Check(pDX, IDC_CHECK_SECTION_SHOW, m_bSectionNumDisplay);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_SHOW, m_bDistanceDisplay);
	DDX_Text(pDX, IDC_EDIT_LINE1, m_strInfoLine1);
	DDX_Text(pDX, IDC_EDIT_LINE2, m_strInfoLine2);
}


BEGIN_MESSAGE_MAP(COpPanelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SETENV, &COpPanelDlg::OnBnClickedSetenv)
	ON_BN_CLICKED(IDC_CHKACTIVEALLCAM, &COpPanelDlg::OnBnClickedChkactiveallcam)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_XRAYON, &COpPanelDlg::OnBnClickedBtnXrayon)
	ON_BN_CLICKED(IDC_BTN_XRAYOFF, &COpPanelDlg::OnBnClickedBtnXrayoff)
	ON_CBN_SELCHANGE(IDC_COMBO_DRIVE_MODE, &COpPanelDlg::OnCbnSelchangeComboDriveMode)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &COpPanelDlg::OnBnClickedButtonApply)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// COpPanelDlg 메시지 처리기입니다.

BOOL COpPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetTimer(TIMEREVENT_DELOLDFILE, 3600 * 1000, NULL);
	SetTimer(TIMEEVENT_READTEMP, 1000, NULL);
	SetWindowTheme(GetDlgItem(IDC_CHKACTIVEALLCAM)->m_hWnd, _T(""), _T(""));

	m_cbDriveMode.EnableWindow(TRUE);

	UpdateDriveModeInfo();
	float fpsVal = GetFpsValueFromDeviceSetting();
	SetFpsValue(fpsVal);

	m_strPipeInfoComp = CStatus::Instance()->m_strComp;
	m_strPipeInfoSEPC = CStatus::Instance()->m_strSEPC;
	m_strPipeInfoSize = CStatus::Instance()->m_strSize;
	m_strPipeInfoPipeNo = CStatus::Instance()->m_strPipeNo;
	m_strPipeInfoSerial = CStatus::Instance()->m_strSerial;
	m_strInfoLine1 = CStatus::Instance()->m_strInfoLine1;
	m_strInfoLine2 = CStatus::Instance()->m_strInfoLine2;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void COpPanelDlg::OnBnClickedSetenv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEnvSettingDlg dlg;
	dlg.DoModal();
}

int COpPanelDlg::InitLight()
{
	return 0;
}

//int COpPanelDlg::SerialCheckNInit()
//{
//	if (m_serial.IsOpen())
//	{
//		m_serial.Close();
//	}
//
//	m_serial.RegCallback(std::bind(&COpPanelDlg::ProcessCMD, this, std::placeholders::_1));
//	m_serial.RegAnalizerCallback(std::bind(&COpPanelDlg::CheckoutPacket, this));
//
//	CString  strSerialNo = CStatus::Instance()->GetComportNo();
//	int iBaudRate = CStatus::Instance()->GetBaudRate();
//	CSerial::EBaudrate BaudRate;
//	switch (iBaudRate)
//	{
//	case 9600://9600
//		BaudRate = CSerial::EBaud9600;
//		break;
//	case 19200://19200
//		BaudRate = CSerial::EBaud19200;
//		break;
//	case 38400://38400
//		BaudRate = CSerial::EBaud38400;
//		break;
//	case 115200://115200
//		BaudRate = CSerial::EBaud115200;
//		break;
//	default:
//		BaudRate = CSerial::EBaud9600;
//	}
//
//	strSerialNo = _T("\\\\.\\") + strSerialNo;
//
//	// 필수 -
//	if (m_serial.Open(strSerialNo.GetBuffer(), this, 0, 0, 16, 16) == ERROR_SUCCESS)
//	{
//		//		VERIFY(m_serial.SetupHandshaking(CSerial::EHandshake::EHandshakeOff) == ERROR_SUCCESS);
//		VERIFY(m_serial.Setup(BaudRate, CSerial::EData8, CSerial::EParNone, CSerial::EStop1) == ERROR_SUCCESS);
//		VERIFY(m_serial.SetMask(CSerial::EEventBreak | CSerial::EEventError | CSerial::EEventRecv) == ERROR_SUCCESS);
//	}
//	// - 필수
//
//	if (!m_serial.IsOpen())
//	{
//		AfxMessageBox(_T("Unable to open COM-port"), MB_ICONSTOP | MB_OK);
//	}
//
//	CStatus::Instance()->SetComm(&m_XFMSPacket);
//	return 0;
//}

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
	CXFMSPacket::EPacketType eType = CXFMSPacket::EPacketType::None;

	if (eEvent == CSerial::EEvent::EEventRecv)
		eType = m_XFMSPacket.CheckoutPacket();
	else
		TRACE(_T("Serial Event: %d\n"), eEvent);

	if (eType == CXFMSPacket::None) return -1;

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
	return m_XFMSPacket.CheckoutPacket();
}


//Active Cam
void COpPanelDlg::OnBnClickedChkactiveallcam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;

	//pView->m_pCamDlg[0]->LegendUpdate();

	if (m_bCamIsActivated)
	{
		m_bCamIsActivated = TRUE;
		pView->m_pCamDlg[0]->m_pGrabChannel->GrabContinuous();

		//kiy device mode
		m_cbDriveMode.EnableWindow(FALSE);
		UpdateDriveModeInfo();

		//Test Check - Active Cam On상태일때 Average버튼 눌리도록 변경
		pView->m_pToolPanelDlg->OnBnClickedBtnAverageAccept();
	}
	else
	{
		m_bCamIsActivated = FALSE;
		pView->m_pCamDlg[0]->m_pGrabChannel->Stop();

		//kiy device mode
		m_cbDriveMode.EnableWindow(TRUE);
		m_cbDriveMode.ResetContent();
		UpdateDriveModeInfo();
	}
}


void COpPanelDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;

	switch (nIDEvent)
	{
	case TIMEREVENT_GRAPCONTINUOUS:
		for (int i = 0; i < m_nCountOfCam; i++)
			pView->m_pCamDlg[i]->DoGrab();
		break;
	case TIMEREVENT_DELOLDFILE:
		if (CStatus::Instance()->m_bUseDelOldImg)DeleteOldFile();
		break;
	case TIMEEVENT_READTEMP:
		//GetTempData();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void COpPanelDlg::DeleteOldFile()
{
	CString strNGSaveFolder = CStatus::Instance()->m_strIMGSaveFolder;

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
		if (('0' > strName.GetAt(i)) || ('9' < strName.GetAt(i)))
			return FALSE;
	}

	CString strYear = strName.Left(2);
	CString strMon = strName.Mid(2, 2);
	CString strDay = strName.Right(2);

	int iYear = _wtoi(strYear) + 2000;
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

HBRUSH COpPanelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	//switch (nCtlColor)
	//{
	//case CTLCOLOR_STATIC:
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(RGB(90, 107, 135));  // static text 글자색 변경
	//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	//}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

UINT COpPanelDlg::ThreadTest(LPVOID pParam)
{
	CXNPIView *pView = (CXNPIView *)pParam;
	BOOL m_bRun = TRUE;
	for (int i = 0; i < 1000; i++)
	{
		pView->m_pCamDlg[0]->SaveImage();
		Sleep(200);
	}

	return 0;	return 0;
}

void COpPanelDlg::OnBnClickedBtnXrayon()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Xray turn On!"), RGB(50, 50, 50), FALSE);
	m_XFMSPacket.SendSetXrayOnOff(TRUE);
}

void COpPanelDlg::OnBnClickedBtnXrayoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Xray turn Off!"), RGB(50, 50, 50), FALSE);
	m_XFMSPacket.SendSetXrayOnOff(FALSE);
}

CString COpPanelDlg::GetManualFileName()
{
	CString strFileName;
	CString strSampleName;
	//CString strMakCdt;
	//GetDlgItemText(IDC_EDT_MAKCONDITION, strMakCdt);
	GetDlgItemText(IDC_EDT_SMPNAME, strSampleName);
	strFileName = strSampleName;
	CTime curTime = CTime::GetCurrentTime();
	strFileName = strFileName + curTime.Format(_T("%y%m%d%H%M%S"));

	return strFileName;
}

BOOL COpPanelDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{

			UINT nID = ::GetDlgCtrlID(pMsg->hwnd);
			CString strNum;
			double dblNum;

			switch (nID)
			{
			case IDC_EDT_VOL:
				GetDlgItemText(IDC_EDT_VOL, strNum);
				dblNum = _tstof(strNum);
				m_XFMSPacket.SendSetKvCMD(dblNum);
				break;
			case IDC_EDT_CUR:
				GetDlgItemText(IDC_EDT_CUR, strNum);
				dblNum = _tstof(strNum);
				m_XFMSPacket.SendSetMaCmd(dblNum);
				break;
				//kiy - 180403
			case IDC_EDIT_PIPEINFO_COMP:
			case IDC_EDIT_PIPEINFO_SEPC:
			case IDC_EDIT_PIPEINFO_SIZE:
			case IDC_EDIT_PIPEINFO_PIPE_NO:
			case IDC_EDIT_PIPEINFO_PIPE_SERIAL:
			case IDC_EDIT_LINE1:
			case IDC_EDIT_LINE2:
			case IDC_CHECK_All_INFO_HIDE:
			case IDC_CHECK_TIME_SHOW:
			case IDC_CHECK_SECTION_SHOW:
			case IDC_CHECK_DISTANCE_SHOW:
				OnBnClickedButtonApply();
				break;
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void COpPanelDlg::CloseProcess()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();

	OnBnClickedBtnXrayoff();
	if (IsDlgButtonChecked(IDC_CHKACTIVEALLCAM))
	{
		pView->m_pCamDlg[0]->m_pGrabChannel->GrabStop();
	}
}

//! drive mode 로드, 디텍터 SDK함수 사용
void COpPanelDlg::OnCbnSelchangeComboDriveMode()
{
	CWaitCursor oWaitCursor;
	int nDriveMode = m_cbDriveMode.GetCurSel();

	RESULT res = RESULT_ERROR;
	int nProgress = 0;

	int deviceID = 0;

	//device list 변경
	res = VwSetDriveMode(deviceID, nDriveMode);
	if (res != RESULT_SUCCESS)
	{
		AfxMessageBox(_T("Can't change the 'Detector' Drive mode."));
		return;
	}

	while (TRUE)
	{
		res = VwGetChangeDriveModeProgress(deviceID, &nProgress);

		if (res != RESULT_SUCCESS)
		{
			AfxMessageBox(_T("Fail to change 'Detector' drive mode"));
			break;
		}

		if (nProgress == 100)
		{
			break;
		}
	}

	float fpsVal = GetFpsValueFromDeviceSetting();
	SetFpsValue(fpsVal);
}

//! drive list update
void COpPanelDlg::UpdateDriveModeInfo()
{
	RESULT res;
	int nDriveModeNum;
	DRIVE_MODE_NAME_SET* pDriveModeNameSet;
	CString strDriveModeName;

	int deviceID = 0;
	res = VwGetDriveModeNameSet(deviceID, &nDriveModeNum, &pDriveModeNameSet);
	if (RESULT_SUCCESS != res)
	{
		AfxMessageBox(_T("Fail to get 'Detector' Drive mode info."));
		return;
	}

	m_cbDriveMode.ResetContent();
	for (int i = 0; i < nDriveModeNum; i++)
	{
		strDriveModeName.Format(_T("%d - "), i);
		strDriveModeName += CString(pDriveModeNameSet[i].szName, 20);
		m_cbDriveMode.InsertString(i, strDriveModeName);
	}

	int nDriveMode;
	res = VwGetDriveMode(deviceID, &nDriveMode);
	if (RESULT_SUCCESS != res)
	{
		AfxMessageBox(_T("Fail to get current Drive mode."));
		return;
	}

	m_cbDriveMode.SetCurSel(nDriveMode);

	return;
}

//! fps value 로드
float COpPanelDlg::GetFpsValueFromDeviceSetting()
{
	RESULT res = RESULT_ERROR;
	CString strText;
	int nIndex = 0;
	//LVITEM lvItem;
	int deviceID = 0;

	// Get Current frame rate
	float frame_rate;

	res = VwGetCurrentFrameRate(deviceID, &frame_rate);

	if (res == RESULT_SUCCESS)
		return frame_rate;
	else
		return -1;
}

void COpPanelDlg::SetFpsValue(float fpsVal)
{
	CString strFpsVal = _T("");
	strFpsVal.Format(_T("%.02f"), fpsVal);
	m_strFpsValue = strFpsVal;

	UpdateData(FALSE);
}

//! pipe info update
void COpPanelDlg::OnBnClickedButtonApply()
{
	UpdateData(TRUE);

	CStatus::Instance()->m_strComp = m_strPipeInfoComp;
	CStatus::Instance()->m_strSEPC = m_strPipeInfoSEPC;
	CStatus::Instance()->m_strSize = m_strPipeInfoSize;
	CStatus::Instance()->m_strSerial = m_strPipeInfoSerial;
	CStatus::Instance()->m_strPipeNo = m_strPipeInfoPipeNo;
	CStatus::Instance()->m_strInfoLine1 = m_strInfoLine1;
	CStatus::Instance()->m_strInfoLine2 = m_strInfoLine2;

	//log
	{
		GetLocalTime(&m_tCur_time);

		CString strTime;
		strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
			m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

		CString log = strTime + _T("[SYS] Pipe Info Updated");
		CTabDetailLogDlg::Instance()->SetTextLog(log, RGB(50, 50, 50), FALSE);
	}
}

void COpPanelDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}
