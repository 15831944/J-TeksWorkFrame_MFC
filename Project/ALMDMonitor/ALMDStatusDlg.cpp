// ALMDStatusDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "ALMDStatusDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "ViewDlg.h"
#include "Repository.h"
#include "ALMDMessages.h"
#include "UnitConversion.h"
#include "Stabilization.h"

class CViewDlg;

// CALMDStatusDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CALMDStatusDlg, CDialogEx)

CALMDStatusDlg::CALMDStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STATUS, pParent)
	, m_ALMDPacket(m_serial)
	, m_iALMDTemp(0)
	, m_strRollMag(_T(""))
	, m_strPitchDir(_T(""))
	, m_strLabelRollMag(_T(""))
	, m_strLabelPitchDir(_T(""))
	, m_strRollUnit(_T("°"))
	, m_strPitchUnit(_T("°"))
	, m_iProgramMode(0)
	, m_pViewDlg(NULL)
	, m_strBatStat(_T(""))
	, m_pRepository(NULL)
	, m_bUseAutoCmd(FALSE)
	, m_bConnected(FALSE)
	, m_dblDispRollValue(0)
	, m_dblDispPitchValue(0)
	, m_strDispTolerance(_T(""))
{

}

CALMDStatusDlg::~CALMDStatusDlg()
{
}

void CALMDStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_GINT, m_ctrlIntervalUnit);
	DDX_Control(pDX, IDC_CB_INTNO, m_ctrlIntNo);
	DDX_Control(pDX, IDC_EDT_ROLL, m_edtRoll);
	DDX_Control(pDX, IDC_EDT_PITCH, m_edtPitch);
	DDX_Control(pDX, IDC_LABEL_ROLL, m_labelRoll);
	DDX_Control(pDX, IDC_LABEL_PITCH, m_labelPitch);
	DDX_Control(pDX, IDC_STAT_BATTERY, m_ctrlBattery);
	DDX_Text(pDX, IDC_ALMDTEMP, m_iALMDTemp);
	DDX_Text(pDX, IDC_EDT_ROLL, m_strRollMag);
	DDX_Text(pDX, IDC_EDT_PITCH, m_strPitchDir);
	DDX_Text(pDX, IDC_LABEL_ROLL, m_strLabelRollMag);
	DDX_Text(pDX, IDC_LABEL_PITCH, m_strLabelPitchDir);
	DDX_Control(pDX, IDC_STAT_CONNECT, m_cStatConnect);
	DDX_Text(pDX, IDC_RUNIT, m_strRollUnit);
	DDX_Text(pDX, IDC_PUNIT, m_strPitchUnit);
	DDX_Radio(pDX, IDC_RPROMODE, m_iProgramMode);
	DDX_Text(pDX, IDC_BATSTAT, m_strBatStat);
	DDX_Check(pDX, IDC_CHK_USEAC, m_bUseAutoCmd);
	DDX_Control(pDX, IDC_BTN_READ, m_cBtnRead);
	DDX_Control(pDX, IDC_CHK_USEAC, m_cBtnUseAC);
	DDX_Text(pDX, IDC_DISPTOLERANCE, m_strDispTolerance);
}


BEGIN_MESSAGE_MAP(CALMDStatusDlg, CDialogEx)
	ON_WM_SERIAL(OnSerialMsg)		// 필수
	ON_BN_CLICKED(IDC_BTN_READ, &CALMDStatusDlg::OnBnClickedBtnRead)
	ON_MESSAGE(WM_PM_CHANGE, &CALMDStatusDlg::OnPMChange)
	ON_MESSAGE(WM_UPDATE_STATINFO, &CALMDStatusDlg::OnUpdateStatusInfo)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RPROMODEV, &CALMDStatusDlg::OnBnClickedRpromodev)
	ON_BN_CLICKED(IDC_RPROMODE, &CALMDStatusDlg::OnBnClickedRpromode)
	ON_BN_CLICKED(IDC_CHK_USEAC, &CALMDStatusDlg::OnBnClickedChkUseac)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CALMDStatusDlg 메시지 처리기입니다.


BOOL CALMDStatusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CUnitConversion * pUnitConversion = CUnitConversion::Instance();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LOGFONT log;
	m_iProgramMode = CStatus::Instance()->GetProgramMode();
	m_labelRoll.GetFont()->GetLogFont(&log);
	log.lfHeight = 25;
	m_BigFont.CreateFontIndirect(&log);
	m_edtRoll.SetFont(&m_BigFont);
	m_edtPitch.SetFont(&m_BigFont);
	ResetStatConnect();

	CWnd *p_combo = GetDlgItem(IDC_CB_INTNO);
	HWND h_wnd = ::FindWindowEx(p_combo->m_hWnd, NULL, _T("Edit"), NULL);
	if (NULL != h_wnd) ((CEdit *)CWnd::FromHandle(h_wnd))->SetReadOnly(TRUE);
	p_combo = GetDlgItem(IDC_CB_GINT);
	h_wnd = ::FindWindowEx(p_combo->m_hWnd, NULL, _T("Edit"), NULL);
	if (NULL != h_wnd) ((CEdit *)CWnd::FromHandle(h_wnd))->SetReadOnly(TRUE);

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
	m_iALMDTemp = 0;
	m_ctrlBattery.SetRange(0, 100);
	m_ctrlBattery.SetPos(50);
	m_strBatStat.Format(_T("50%%"));
	m_strLabelRollMag.Format(_T("Roll(x)"));
	m_strLabelPitchDir.Format(_T("Pitch(y)"));
	m_cStatConnect.Set(false);
	CStatus::Instance()->SetProgramMode(CStatus::PM_REVIEW);
//Disable Time Limit	SetTimer(4, 5000, NULL);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


LRESULT CALMDStatusDlg::OnSerialMsg(WPARAM wParam, LPARAM lParam)
{
	m_cStatConnect.Set(true);
	ResetStatConnect();
	USES_CONVERSION;

	CALMDPacket::EPacketType eType = m_ALMDPacket.CheckoutPacket();

	switch (eType)
	{
	case CALMDPacket::SensorInfo:
	{
		DVC_PACKET_SENSORINFO* pPacket = (DVC_PACKET_SENSORINFO *)m_ALMDPacket.GetPacket();
		switch (pPacket->byRadius)
		{
		case CALMDPacket::W150MM:
			CStatus::Instance()->SetWaferSize(150);
			break;
		case CALMDPacket::W200MM:
			CStatus::Instance()->SetWaferSize(200);
			break;
		case CALMDPacket::W300MM:
			CStatus::Instance()->SetWaferSize(300);
			break;
		case CALMDPacket::W450MM:
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
	case CALMDPacket::Measure:
	{
		DVC_PACKET_MEAINFO* pPacket = (DVC_PACKET_MEAINFO *)m_ALMDPacket.GetPacket();
		SetStateInfo(pPacket);
	}
		break;

	}
	return 0;
}


void CALMDStatusDlg::SerialCheckNInit()
{
	if (CStatus::Instance()->GetProgramMode() != CStatus::PM_MONITORING)return;
	if (m_serial.IsOpen())
	{
		m_serial.Close();
	}

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
	if (m_serial.Open(strSerialNo.GetBuffer(), this) == ERROR_SUCCESS)
	{
		VERIFY(m_serial.Setup(BaudRate, CSerial::EData8, CSerial::EParNone, CSerial::EStop1) == ERROR_SUCCESS);
		VERIFY(m_serial.SetMask(CSerial::EEventBreak | CSerial::EEventError | CSerial::EEventRecv) == ERROR_SUCCESS);
	}
	// - 필수

	if (!m_serial.IsOpen())
	{
		AfxMessageBox(_T("Unable to open COM-port"), MB_ICONSTOP | MB_OK);
	}
}


void CALMDStatusDlg::OnBnClickedBtnRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if(m_bConnected)m_pRepository->WriteCurStatus();
}


void CALMDStatusDlg::SetStateInfo(DVC_PACKET_MEAINFO * pPacketInfo)
{
	tALMDRecord newRecord;
	MakeWriteInfo(&newRecord, pPacketInfo);
	CStatus::Instance()->SetMeasureData(&newRecord);
}


int CALMDStatusDlg::SetDispValue(double iRoll, double iPitch)
{
	if (CStatus::GetCoordinateSystem() == CStatus::CS_CARTESIAN)
	{
			
	}
	else
	{

	}
	return 0;
}

BOOL CALMDStatusDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CALMDStatusDlg::LabelUpdate()
{
	OnUpdateStatusInfo(0, 0);

	if (CStatus::Instance()->GetCoordinateSystem() == CStatus::CS_CARTESIAN)
	{
		m_strLabelRollMag.Format(_T("Roll(x)"));
		m_strLabelPitchDir.Format(_T("Pitch(y)"));
	}
	else
	{
		m_strLabelRollMag.Format(_T("Magnitude"));
		m_strLabelPitchDir.Format(_T("Direction"));
	}

	CUnitConversion::Instance()->GetUnitString(m_strRollUnit, m_strPitchUnit);

	UpdateData(false);
}


void CALMDStatusDlg::OnTimer(UINT_PTR nIDEvent)
{
// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
switch (nIDEvent)
{
case 0:
	m_cStatConnect.Set(false);
	m_bConnected = false;
	break;
case 2:
	if (m_bConnected)m_pRepository->WriteCurStatus();
	break;
case 4:
//Disable Time License	CheckDateLicense();
	break;
}
CDialogEx::OnTimer(nIDEvent);
}


void CALMDStatusDlg::ResetStatConnect()
{
	m_cStatConnect.Set(true);
	m_bConnected = TRUE;
	SetTimer(0, 1000, NULL);
}


void CALMDStatusDlg::OnBnClickedRpromodev()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iProgramMode = 1;
	m_pRepository->ClearData();
	CStabilization::Instance()->Init();
	CStatus::Instance()->SetProgramMode(CStatus::PM_MONITORING);
}


void CALMDStatusDlg::OnBnClickedRpromode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iProgramMode = 0;
	CStatus::Instance()->SetProgramMode(CStatus::PM_REVIEW);
}


void CALMDStatusDlg::SerialClose()
{
	if (m_serial.IsOpen())
	{
		m_ALMDPacket.SendSensorInfoCommand();
		Sleep(300);
		m_serial.Close();
	}
}


void CALMDStatusDlg::OnBnClickedChkUseac()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if (m_bUseAutoCmd)
	{
		if (m_bConnected)m_pRepository->WriteCurStatus();
		CString strIntNo;
		m_ctrlIntNo.GetLBText(m_ctrlIntNo.GetCurSel(), strIntNo);
		int iIntNo = _wtoi(strIntNo.GetBuffer());
		int iIntSec = (int)(iIntNo * (pow(60, m_ctrlIntervalUnit.GetCurSel())));
		SetTimer(2, iIntSec * 1000, NULL);
	}
	else
	{
		KillTimer(2);
	}

}


void CALMDStatusDlg::SetProgramMode(CStatus::tProgramMode ProgramMode)
{
	if (ProgramMode == CStatus::PM_REVIEW)
	{
		SerialClose();
		m_cBtnRead.EnableWindow(FALSE);
		m_cBtnUseAC.EnableWindow(FALSE);
		m_ctrlIntervalUnit.EnableWindow(FALSE);
		m_ctrlIntNo.EnableWindow(FALSE);
	}
	else
	{
		m_cBtnRead.EnableWindow(TRUE);
		m_cBtnUseAC.EnableWindow(TRUE);
		m_ctrlIntervalUnit.EnableWindow(TRUE);
		m_ctrlIntNo.EnableWindow(TRUE);
		m_iProgramMode = 1;
		SerialCheckNInit();
		Sleep(100);
		m_ALMDPacket.SendSensorInfoCommand();
		Sleep(100);
		m_ALMDPacket.SendMeasureCommand();
	}
}


LRESULT CALMDStatusDlg::OnPMChange(WPARAM wParam, LPARAM lParam)
{
	SetProgramMode(CStatus::Instance()->GetProgramMode());
	return 0;
}


LRESULT CALMDStatusDlg::OnUpdateStatusInfo(WPARAM wParam, LPARAM lParam)
{

	tALMDRecord * pMeaInfo;
	pMeaInfo = &(CStatus::Instance()->m_curMeasureData);
	int battery = (pMeaInfo->byBattery);
	int temperature = (int)((signed char)(pMeaInfo->byTemperature));
	m_ctrlBattery.SetPos(battery);
	m_strBatStat.Format(_T("%d%%"), battery);
	m_iALMDTemp = temperature;

	int pitch = (int)(pMeaInfo->sRawPitch);
	int roll = (int)(pMeaInfo->sRawRoll);
	if (CStatus::Instance()->IsRelative())
	{
		roll -= (CStatus::Instance()->m_dptRelPlan.dblPosX * 1000);
		pitch -= (CStatus::Instance()->m_dptRelPlan.dblPosY * 1000);
	}

	CUnitConversion::Instance()->SetWaferSize(CStatus::Instance()->GetWaferSize());
	CUnitConversion::Instance()->SetRollAndPitch(roll / 1000., pitch / 1000.);
	double dblRollValue = 0;
	double dblPitchValue = 0;
	CUnitConversion::Instance()->GetStateValue(dblRollValue, dblPitchValue);
	m_dblDispRollValue = dblRollValue;
	m_dblDispPitchValue = dblPitchValue;
	if (abs(pitch) == 30000)
		m_strPitchDir.Format(_T("####"));
	else
	{
		if(CStatus::Instance()->IsDigit2Mode())
			m_strPitchDir.Format(_T("%2.2f"), dblPitchValue);
		else
			m_strPitchDir.Format(_T("%2.3f"), dblPitchValue);

	}
	if (abs(roll) == 30000)
		m_strRollMag.Format(_T("####"));
	else
	{
		if (CStatus::Instance()->IsDigit2Mode())
			m_strRollMag.Format(_T("%2.2f"), dblRollValue);
		else
			m_strRollMag.Format(_T("%2.3f"), dblRollValue);
	}
	GetGoNoGoDispInitData();
	UpdateData(false);
	return 0;
}

int CALMDStatusDlg::MakeWriteInfo(tALMDRecord  * pNewRecord, DVC_PACKET_MEAINFO *pPacketInfo)
{
	memset(pNewRecord, 0, sizeof(tALMDRecord));
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
	pNewRecord->sRawRoll = pPacketInfo->sRoll;
	pNewRecord->sRawPitch = pPacketInfo->sPitch;
	CStabilization::Instance()->Add(CPoint(pPacketInfo->sRoll, pPacketInfo->sPitch));
	pNewRecord->bStableRoll		= CStabilization::Instance()->m_bIsStableX;
	pNewRecord->bStablePitch	= CStabilization::Instance()->m_bIsStableY;
	pNewRecord->bStablization	= CStabilization::Instance()->m_bIsStable;
	pNewRecord->bRel = CStatus::Instance()->IsRelative();
	pNewRecord->dblRefRoll = CStatus::Instance()->m_dptRelPlan.dblPosX;
	pNewRecord->dblRefPitch = CStatus::Instance()->m_dptRelPlan.dblPosY;
	pNewRecord->byBattery = pPacketInfo->byBattery;
	pNewRecord->byTemperature = pPacketInfo->byTemperature;
	GetDispXYValue(pNewRecord);
	pNewRecord->bStablization = CStabilization::Instance()->IsStable();
	pNewRecord->dateCalibration = timeCalTime;
	pNewRecord->CurTime = timeNow;
	pNewRecord->dblGoNoGoValue = CStatus::Instance()->GetTolerance();
	return 0;
}


double CALMDStatusDlg::GetDispXValue()
{
	return 0;
}


double CALMDStatusDlg::GetDispYValue()
{
	return 0;
}


void CALMDStatusDlg::GetGoNoGoDispInitData()
{
	CString strGoNoGoUnit;
	double dblGoNoGoDegree = CStatus::Instance()->GetTolerance();
	double dblDispTolerance;
	CUnitConversion::Instance()->ConvertToDest(CStatus::Instance()->GetCoordinateSystem(), CStatus::Instance()->GetAngleType(), (CStatus::tIncType)(CStatus::Instance()->GetInclinationType()), CStatus::Instance()->GetRORType(), &strGoNoGoUnit, &dblDispTolerance, dblGoNoGoDegree);
	if(CStatus::Instance()->IsDigit2Mode())
		m_strDispTolerance.Format(_T("%5.2f %s"), dblDispTolerance, strGoNoGoUnit.GetBuffer());
	else
		m_strDispTolerance.Format(_T("%5.3f %s"), dblDispTolerance, strGoNoGoUnit.GetBuffer());
}

#pragma warning(disable:4996)
int CALMDStatusDlg::CheckDateLicense()
{
	struct tm tm_ptr;
	time_t timeFirstDay;
	int iYear = 2016;
	int iMon = 12;
	int iDay = 12;

	tm_ptr.tm_year = iYear - 1900;
	tm_ptr.tm_mon = iMon - 1;
	tm_ptr.tm_mday = iDay;
	tm_ptr.tm_hour = 1;
	tm_ptr.tm_min = 0;
	tm_ptr.tm_sec = 0;
	tm_ptr.tm_isdst = 0;
	timeFirstDay = mktime(&tm_ptr);

	time_t timeNow;
	time(&timeNow);

	int iCalElapseTime = (int)(difftime(timeNow, timeFirstDay) / (60 * 60 * 24));

	if (iCalElapseTime > 150)
		exit(0);
	return 0;
}

void CALMDStatusDlg::GetDispXYValue(tALMDRecord * pMeaInfo)
{
	int pitch = (int)(pMeaInfo->sRawPitch);
	int roll = (int)(pMeaInfo->sRawRoll);
	if (CStatus::Instance()->IsRelative())
	{
		roll -= (CStatus::Instance()->m_dptRelPlan.dblPosX * 1000);
		pitch -= (CStatus::Instance()->m_dptRelPlan.dblPosY * 1000);
	}

	CUnitConversion::Instance()->SetWaferSize(CStatus::Instance()->GetWaferSize());
	CUnitConversion::Instance()->SetRollAndPitch(roll / 1000., pitch / 1000.);
	double dblRollValue = 0;
	double dblPitchValue = 0;
	CUnitConversion::Instance()->GetStateValue(dblRollValue, dblPitchValue);
	m_dblDispRollValue = dblRollValue;
	m_dblDispPitchValue = dblPitchValue;
	pMeaInfo->dblDispRoll = dblRollValue;
	pMeaInfo->dblDispPitch = dblPitchValue;
}


void CALMDStatusDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
	{
		SerialClose();
	}
	CDialogEx::OnClose();
}
