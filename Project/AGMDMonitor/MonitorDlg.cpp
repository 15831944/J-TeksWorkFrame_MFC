// MonitorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "MonitorDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "AGMDDefine.h"
#include "Repository.h"
#include "MainFrm.h"

// CMonitorDlg 대화 상자입니다.
#pragma comment(lib, "winmm.lib")
#include "Mmsystem.h"

#define INT_TIME 3
IMPLEMENT_DYNAMIC(CMonitorDlg, CDialogEx)

CMonitorDlg::CMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MONITORDLG, pParent)
	, m_AGMDPacket(m_serial)
	, m_iProgramMode(FALSE)
	, m_bReceived(FALSE)
	, m_strGap1(_T(""))
	, m_strGap2(_T(""))
	, m_strGap3(_T(""))
	, m_strGap1Unit(_T(""))
	, m_strGap2Unit(_T(""))
	, m_strGap3Unit(_T(""))
	, m_strTest(_T(""))
	, m_strGap1Tol(_T(""))
	, m_strGap2Tol(_T(""))
	, m_strGap3Tol(_T(""))
	, m_bUseAutoCmd(FALSE)
{
	CStatus::Instance()->SetMonitorDlg(this);
}

CMonitorDlg::~CMonitorDlg()
{
}

void CMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_VIEW, m_iProgramMode);
	DDX_Text(pDX, IDC_LGAP1, m_strGap1);
	DDX_Text(pDX, IDC_LGAP2, m_strGap2);
	DDX_Text(pDX, IDC_LGAP3, m_strGap3);
	DDX_Text(pDX, IDC_STATIC_GAP1_UNIT, m_strGap1Unit);
	DDX_Text(pDX, IDC_STATIC_GAP2_UNIT, m_strGap2Unit);
	DDX_Text(pDX, IDC_STATIC_GAP3_UNIT, m_strGap3Unit);
	DDX_Text(pDX, IDC_STATIC_GAP1_TOL, m_strGap1Tol);
	DDX_Text(pDX, IDC_STATIC_GAP2_TOL, m_strGap2Tol);
	DDX_Text(pDX, IDC_STATIC_GAP3_TOL, m_strGap3Tol);
	DDX_Control(pDX, IDC_CB_INTNO, m_ctrlIntNo);
	DDX_Control(pDX, IDC_CB_GINT, m_ctrlIntervalUnit);
	DDX_Check(pDX, IDC_CHK_AUTO_CMD, m_bUseAutoCmd);
	DDX_Control(pDX, IDC_LGAP1, m_ctrl_Gap1);
	DDX_Control(pDX, IDC_LGAP2, m_ctrl_Gap2);
	DDX_Control(pDX, IDC_LGAP3, m_ctrl_Gap3);
}


BEGIN_MESSAGE_MAP(CMonitorDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_VIEW, &CMonitorDlg::OnBnClickedRadioView)
	ON_MESSAGE(WM_PM_CHANGE, &CMonitorDlg::OnProgramModeChange)
	ON_MESSAGE(WM_UIUPDATE, &CMonitorDlg::OnReceiveUpdateUI)
	ON_MESSAGE(WM_DLG_INITIALIZE, &CMonitorDlg::OnDlgInit)
	ON_BN_CLICKED(IDC_RADIO_REVIEW, &CMonitorDlg::OnBnClickedRadioReview)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_READ, &CMonitorDlg::OnBnClickedBtnRead)
	ON_BN_CLICKED(IDC_CHK_AUTO_CMD, &CMonitorDlg::OnBnClickedChkAutoCmd)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMonitorDlg 메시지 처리기입니다.


void CMonitorDlg::OnBnClickedRadioView()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		return;
	m_iProgramMode = 0;
	EnableControl(TRUE);

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->SetTitleCaptionReset();
	CStatus::Instance()->GetDefault();
	CStatus::Instance()->SetProgramMode(CStatus::PM_MONITORING);
	CStatus::Instance()->OpPanelOffsetInitialize();
}

void CMonitorDlg::OnBnClickedRadioReview()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
		return;
	KillTimer(2);
	m_bUseAutoCmd = FALSE;
	m_iProgramMode = 1;
	CStatus::Instance()->SetProgramMode(CStatus::PM_REVIEW);
	CStatus::Instance()->GetRepository()->ClearData();
	CStatus::Instance()->AllDlgClear();
	EnableControl(FALSE);

}

BOOL CMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_iProgramMode =CStatus::Instance()->GetProgramMode();
	OnInitText();

	CString strNum;
	for (int i = 1; i < 61; i++)
	{
		strNum.Format(_T("%d"), i);
		m_ctrlIntNo.AddString(strNum);
	}
	m_ctrlIntNo.SetCurSel(0);
	m_ctrlIntervalUnit.AddString(_T("seconds"));
	m_ctrlIntervalUnit.AddString(_T("minutes"));
	m_ctrlIntervalUnit.SetCurSel(0);
	
	LOGFONT log;
	m_ctrl_Gap1.GetFont()->GetLogFont(&log);
	log.lfHeight = 25;
	m_BigFont.CreateFontIndirect(&log);
	m_ctrl_Gap1.SetFont(&m_BigFont);
	m_ctrl_Gap2.SetFont(&m_BigFont);
	m_ctrl_Gap3.SetFont(&m_BigFont);

	EnableControl(FALSE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

LRESULT CMonitorDlg::OnProgramModeChange(WPARAM wParam, LPARAM lParam)
{
	SetProgramMode(CStatus::Instance()->GetProgramMode());
	return 0;
}

void CMonitorDlg::SetProgramMode(CStatus::tProgramMode ProgramMode)
{
	if (ProgramMode == CStatus::PM_REVIEW)
	{
		SerialClose();
	}
	else
	{
		m_iProgramMode = 0;
		SerialCheckNInit();
		Sleep(100);
		m_AGMDPacket.SendSensorInfoCommand();
		Sleep(100);
		m_AGMDPacket.SendMeasureCommand();
	}
}

void CMonitorDlg::SerialCheckNInit()
{
	if (m_serial.IsOpen())
	{
		m_serial.Close();
	}

	m_serial.RegCallback(std::bind(&CMonitorDlg::ProcessCMD, this, std::placeholders::_1));

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
}

void CMonitorDlg::SerialClose()
{
	if (m_serial.IsOpen())
	{
		m_AGMDPacket.SendSensorInfoCommand();
		Sleep(400);
		m_serial.Close();
	}
}

void CMonitorDlg::ProcessCMD(int nEvent)
{
	RcvSerialMsg((WPARAM)nEvent);
	SetFlagReceived();
}

int CMonitorDlg::RcvSerialMsg(WPARAM wParam)
{
	CSerial::EEvent eEvent = (CSerial::EEvent)wParam;

	static int prevTime = 0;
	static int prevNum = 0;
	static int Packet0Time = 0;
	static int Packet255Time = 0;
	USES_CONVERSION;
	CAGMDPacket::EPacketType eType = CAGMDPacket::EPacketType::None;

	if (eEvent == CSerial::EEvent::EEventRecv)
		eType = m_AGMDPacket.CheckoutPacket();
	else
		TRACE(_T("Serial Event: %d\n"), eEvent);

	if (prevTime == 0)prevTime = timeGetTime();
	int curTime = timeGetTime();
	prevTime = curTime;

	switch (eType)
	{
	case CAGMDPacket::SensorInfo:
	{
		DVC_PACKET_SENSORINFO* pPacket = new DVC_PACKET_SENSORINFO;
		m_AGMDPacket.GetQueuePacket((BYTE*)pPacket, CAGMDPacket::SensorInfo);
		switch (pPacket->byRadius)
		{
		case CAGMDPacket::W150MM:
			CStatus::Instance()->SetWaferSize(150);
			break;
		case CAGMDPacket::W200MM:
			CStatus::Instance()->SetWaferSize(200);
			break;
		case CAGMDPacket::W300MM:
			CStatus::Instance()->SetWaferSize(300);
			break;
		case CAGMDPacket::W450MM:
			CStatus::Instance()->SetWaferSize(450);
			break;
		default:

			break;
		}
		char szCalDate[MAX_CAL_DATE + 1] = { 0 };
		char szSerialNo[MAX_SERIAL_NO + 1] = { 0 };
		::CopyMemory(szCalDate, pPacket->chCalDate, MAX_CAL_DATE);
		::CopyMemory(szSerialNo, pPacket->chSerialNo, MAX_SERIAL_NO);
		CStatus::Instance()->m_strCalDate = A2CT(szCalDate);
		CStatus::Instance()->m_strSerialNo = A2CT(szSerialNo);
	}
	break;
	case CAGMDPacket::GapMeasure:
	{
		DVC_PACKET_GAPINFO* pPacket = new DVC_PACKET_GAPINFO;
		m_AGMDPacket.GetQueuePacket((BYTE*)pPacket, CAGMDPacket::GapMeasure);
		if(CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
			SetStateInfo(pPacket);
	}
	break;
	case CAGMDPacket::CalInfo:
	{
		DVC_PACKET_CALINFO* pPacket = new DVC_PACKET_CALINFO;
		m_AGMDPacket.GetQueuePacket((BYTE*)pPacket, CAGMDPacket::CalInfo);
		SetCalInfo(pPacket);
	}
	break;
	case CAGMDPacket::SetCalInfo:
	{
		DVC_PACKET_CALINFO* pPacket = new DVC_PACKET_CALINFO;
		m_AGMDPacket.GetQueuePacket((BYTE*)pPacket, CAGMDPacket::SetCalInfo);
	}
	break;
	}
	return 0;
}

void CMonitorDlg::SetStateInfo(DVC_PACKET_GAPINFO * pPacketInfo)
{
	tAGMDRecord newRecord;
	MakeWriteInfo(&newRecord, pPacketInfo, 0);
	CStatus::Instance()->SetMeasureData(&newRecord);
	CStatus::Instance()->UpdateScrn();
}

void CMonitorDlg::SetCalInfo(DVC_PACKET_CALINFO * pPacketInfo)
{
	CStatus::Instance()->m_strCalDate = pPacketInfo->chCalDate;
	CStatus::Instance()->m_CalData.uJigGap = pPacketInfo->usCalTargetDist;
	CStatus::Instance()->m_CalData.iGap1 = pPacketInfo->sGap1;
	CStatus::Instance()->m_CalData.iGap2 = pPacketInfo->sGap2;
	CStatus::Instance()->m_CalData.iGap3 = pPacketInfo->sGap3;
	CStatus::Instance()->m_CalData.bRcvCalInfo = TRUE;
	CStatus::Instance()->UpdateScrn();
}


int CMonitorDlg::MakeWriteInfo(tAGMDRecord  * pNewRecord, DVC_PACKET_GAPINFO *pPacketInfo, int nInputType)
{
	memset(pNewRecord, 0, sizeof(tAGMDRecord));

	struct tm tm_ptr;
	time_t timeCalTime;
	CString strCalDate = CStatus::Instance()->m_strCalDate;
	CString strYear = strCalDate.Left(2);
	strCalDate = strCalDate.Mid(2);
	CString strMon = strCalDate.Left(2);
	CString strDay = strCalDate.Right(2);
	int iYear = _wtoi(strYear);
	int iMon = _wtoi(strMon);
	int iDay = _wtoi(strDay);
	tm_ptr.tm_year = 2000 + iYear - 1900;
	tm_ptr.tm_mon = iMon - 1;
	tm_ptr.tm_mday = iDay;
	tm_ptr.tm_hour = 1;
	tm_ptr.tm_min = 0;
	tm_ptr.tm_sec = 0;
	tm_ptr.tm_isdst = 0;
	timeCalTime = mktime(&tm_ptr);
	time_t timeNow;
	time(&timeNow);
	
	pNewRecord->byBattery = pPacketInfo->byBattery;
	pNewRecord->byTemperature = pPacketInfo->byTemperatue;
	pNewRecord->usGap1 = pPacketInfo->usGap1;
	pNewRecord->usGap2 = pPacketInfo->usGap2;
	pNewRecord->usGap3 = pPacketInfo->usGap3;
	pNewRecord->bIsOffsetMode = CStatus::Instance()->GetIsOffsetMode();
	pNewRecord->iUnitMode = (int)CStatus::Instance()->GetUnitMode();
	pNewRecord->iNotchMode = (int)CStatus::Instance()->GetNotchMode();
	pNewRecord->dateCalibration = timeCalTime;
	pNewRecord->CurTime = timeNow;

	if (CStatus::Instance()->GetIsOffsetMode())
	{
		pNewRecord->dblOffset_Gap1 = CStatus::Instance()->GetOffset(0);
		pNewRecord->dblOffset_Gap2 = CStatus::Instance()->GetOffset(1);
		pNewRecord->dblOffset_Gap3 = CStatus::Instance()->GetOffset(2);
	}
	else
	{
		pNewRecord->dblOffset_Gap1 = 0.;
		pNewRecord->dblOffset_Gap2 = 0.;
		pNewRecord->dblOffset_Gap3 = 0.;
	}
	
	pNewRecord->dblTarget_Gap1 = CStatus::Instance()->GetTarget(0);
	pNewRecord->dblTarget_Gap2 = CStatus::Instance()->GetTarget(1);
	pNewRecord->dblTarget_Gap3 = CStatus::Instance()->GetTarget(2);
	pNewRecord->dblTolerance_Gap1 = CStatus::Instance()->GetTolerance(0);
	pNewRecord->dblTolerance_Gap2 = CStatus::Instance()->GetTolerance(1);
	pNewRecord->dblTolerance_Gap3 = CStatus::Instance()->GetTolerance(2);
	pNewRecord->dblTarget_AllGap = CStatus::Instance()->GetTarget(3);
	pNewRecord->dblTolerance_AllGap = CStatus::Instance()->GetTolerance(3);
	pNewRecord->bIsAllGapSame = CStatus::Instance()->GetIsGapSameSettings();
	pNewRecord->dblGapViewRange = CStatus::Instance()->GetGapRangeView();
	wsprintf(pNewRecord->szMark, CStatus::Instance()->m_strMark.GetBuffer());

	return 0;
}

void CMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 1:
		break;
	case 2:
	{
		BOOL bConnected = CStatus::Instance()->GetLedStatus();
		if (bConnected)CStatus::Instance()->GetRepository()->WriteCurStatus();
		break;
	}
	case INT_TIME:
		//m_cRecPrgrs.SetPos(CStatus::Instance()->GetRepository()->m_iLastWriteIndex);
		//if (CStatus::Instance()->GetRepository()->IsSaveTargetIndex())
		//{
		//	OnBnClickedBtnRecordFinish();
		//}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CMonitorDlg::OnReceiveUpdateUI(WPARAM wParam, LPARAM lParam)
{
	double dblGap1, dblGap2, dblGap3 ,dblGap1_Offset, dblGap2_Offset, dblGap3_Offset;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
	{
		dblGap1 = (double)CStatus::Instance()->m_curMeasureData.usGap1 / UNIT_MM_VALUE;
		dblGap2 = (double)CStatus::Instance()->m_curMeasureData.usGap2 / UNIT_MM_VALUE;
		dblGap3 = (double)CStatus::Instance()->m_curMeasureData.usGap3 / UNIT_MM_VALUE;
		if (CStatus::Instance()->GetIsOffsetMode())
		{
			dblGap1_Offset = CStatus::Instance()->GetOffset(0);
			dblGap2_Offset = CStatus::Instance()->GetOffset(1);
			dblGap3_Offset = CStatus::Instance()->GetOffset(2);
			m_strGap1.Format(_T("%.3f"), dblGap1 + dblGap1_Offset);
			m_strGap2.Format(_T("%.3f"), dblGap2 + dblGap2_Offset);
			m_strGap3.Format(_T("%.3f"), dblGap3 + dblGap3_Offset);
		}
		else
		{
			m_strGap1.Format(_T("%.3f"), dblGap1);
			m_strGap2.Format(_T("%.3f"), dblGap2);
			m_strGap3.Format(_T("%.3f"), dblGap3);
		}
	}
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
	{
		dblGap1 = (double)CStatus::Instance()->m_curMeasureData.usGap1 / UNIT_MM_VALUE * UNIT_INCH_VALUE;
		dblGap2 = (double)CStatus::Instance()->m_curMeasureData.usGap2 / UNIT_MM_VALUE * UNIT_INCH_VALUE;
		dblGap3 = (double)CStatus::Instance()->m_curMeasureData.usGap3 / UNIT_MM_VALUE * UNIT_INCH_VALUE;
		if (CStatus::Instance()->GetIsOffsetMode())
		{
			dblGap1_Offset = CStatus::Instance()->GetOffset(0);
			dblGap2_Offset = CStatus::Instance()->GetOffset(1);
			dblGap3_Offset = CStatus::Instance()->GetOffset(2);
			m_strGap1.Format(_T("%.3f"), dblGap1 + dblGap1_Offset);
			m_strGap2.Format(_T("%.3f"), dblGap2 + dblGap2_Offset);
			m_strGap3.Format(_T("%.3f"), dblGap3 + dblGap3_Offset);
		}
		else
		{
			m_strGap1.Format(_T("%.3f"), dblGap1);
			m_strGap2.Format(_T("%.3f"), dblGap2);
			m_strGap3.Format(_T("%.3f"), dblGap3);
		}
	}
	
	WriteToleranceValue();
	UpdateData(FALSE);
	return LRESULT();
}

void CMonitorDlg::SetFlagReceived()
{
	m_bReceived = TRUE;
}

BOOL CMonitorDlg::IsCMDReceived()
{
	BOOL bReturn;
	bReturn = m_bReceived;
	m_bReceived = FALSE;
	return bReturn;
}

void CMonitorDlg::WriteToleranceValue()
{
	//UpdateData(TRUE);
	if (CStatus::Instance()->GetIsGapSameSettings())
	{
		double dblTolerance = CStatus::Instance()->GetTolerance(3);

		if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
		{
			m_strGap1Unit.Format(_T("mm"));
			m_strGap2Unit.Format(_T("mm"));
			m_strGap3Unit.Format(_T("mm"));
		}
		if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
		{
			m_strGap1Unit.Format(_T("inches"));
			m_strGap2Unit.Format(_T("inches"));
			m_strGap3Unit.Format(_T("inches"));
		}
	
		m_strGap1Tol.Format(_T("Tolerance : %.3f"), dblTolerance);
		m_strGap2Tol.Format(_T("Tolerance : %.3f"), dblTolerance);
		m_strGap3Tol.Format(_T("Tolerance : %.3f"), dblTolerance);
	
	}
	else
	{
		double dblGap1Tolerance = CStatus::Instance()->GetTolerance(0);
		double dblGap2Tolerance = CStatus::Instance()->GetTolerance(1);
		double dblGap3Tolerance = CStatus::Instance()->GetTolerance(2);
		if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
		{
			m_strGap1Unit = _T("mm");
			m_strGap2Unit = _T("mm");
			m_strGap3Unit = _T("mm");
		}
			
		if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
		{
			m_strGap1Unit = _T("inches");
			m_strGap2Unit = _T("inches");
			m_strGap3Unit = _T("inches");
		}
		
			m_strGap1Tol.Format(_T("Tolerance : %.3f"), dblGap1Tolerance);
			m_strGap2Tol.Format(_T("Tolerance : %.3f"), dblGap2Tolerance);
			m_strGap3Tol.Format(_T("Tolerance : %.3f"), dblGap3Tolerance);
	}
}

void CMonitorDlg::OnInitText()
{
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
	{		
		m_strGap1Unit.Format(_T("mm"));
		m_strGap2Unit.Format(_T("mm"));
		m_strGap3Unit.Format(_T("mm"));
	}
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
	{
		m_strGap1Unit.Format(_T("inches"));
		m_strGap2Unit.Format(_T("inches"));
		m_strGap3Unit.Format(_T("inches"));
	}
	m_strGap1.Format(_T("%.3f"), 0.);
	m_strGap2.Format(_T("%.3f"), 0.);
	m_strGap3.Format(_T("%.3f"), 0.);
	m_strGap1Tol.Format(_T("Tolerance : %.3f"), 0.);
	m_strGap2Tol.Format(_T("Tolerance : %.3f"), 0.);
	m_strGap3Tol.Format(_T("Tolerance : %.3f"), 0.);
}

void CMonitorDlg::OnBnClickedBtnRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	BOOL bConnected = CStatus::Instance()->GetLedStatus();
	if (!bConnected)
		return;
	CStatus::Instance()->GetRepository()->WriteCurStatus();
}

void CMonitorDlg::EnableControl(bool bIsDisable)
{
	GetDlgItem(IDC_BTN_READ)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_CHK_AUTO_CMD)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_CB_INTNO)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_CB_GINT)->EnableWindow(bIsDisable);
}

void CMonitorDlg::OnBnClickedChkAutoCmd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if (m_bUseAutoCmd)
	{
		GetDlgItem(IDC_BTN_READ)->EnableWindow(FALSE);
		BOOL bConnected = CStatus::Instance()->GetLedStatus();
		if (bConnected)
			CStatus::Instance()->GetRepository()->WriteCurStatus();
			//m_pRepository->WriteCurStatus();

		CString strIntNo;
		m_ctrlIntNo.GetLBText(m_ctrlIntNo.GetCurSel(), strIntNo);
		int iIntNo = _wtoi(strIntNo.GetBuffer());
		int iIntSec = (int)(iIntNo * (pow(60, m_ctrlIntervalUnit.GetCurSel())));
		SetTimer(2, iIntSec * 1000, NULL);
	}
	else
	{
		GetDlgItem(IDC_BTN_READ)->EnableWindow(TRUE);
		KillTimer(2);
	}
}

BOOL CMonitorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN))
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMonitorDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnClose();
}

LRESULT CMonitorDlg::OnDlgInit(WPARAM wParam, LPARAM lParam)
{
	CheckDlgButton(IDC_CHK_AUTO_CMD, FALSE);
	return 0;
}