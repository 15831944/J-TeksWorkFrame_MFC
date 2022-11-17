// ModeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "ModeDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "Repository.h"
#include "GraphDlg.h"
#include "ViewOptionDlg.h"
#include "APMDMessages.h"

// CModeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CModeDlg, CDialogEx)

CModeDlg::CModeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MODE, pParent)
	, m_iProgramMode(0)
	, m_pWThd(NULL)
	, m_APMDPacket(m_serial)
	, m_pStatusDlg(NULL)
	, m_pRepository(NULL)
	, m_pGraphDlg(NULL)
	, m_strAPMDStatus(_T(""))
	, m_iPurgeSec(0)
	, m_iMeasureSec(0)
	, m_pViewOptionDlg(NULL)
	, m_bStart(false)
	, m_nPurgeStep(0)
{
	if (CStatus::GetProgramMode() == CStatus::PM_MONITORING)m_iProgramMode = 1;
}

CModeDlg::~CModeDlg()
{
}

void CModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNSS, m_cProgramMode);
	DDX_Radio(pDX, IDC_RMODE, m_iProgramMode);
	DDX_Control(pDX, IDC_CBPT, m_cPurgeUnit);
	DDX_Control(pDX, IDC_SPPURGE, m_cSpinPurgeTime);
	DDX_Control(pDX, IDC_PURGE_PROG, m_progPurge);
	DDX_Control(pDX, IDC_SPGT, m_SpinGatheringTime);
	DDX_Control(pDX, IDC_CBDGTU, m_cGTUnit);
	DDX_Text(pDX, IDC_APMD_STATUS, m_strAPMDStatus);
	DDX_Control(pDX, IDC_MEAS_PROG, m_progMeasure);
}


BEGIN_MESSAGE_MAP(CModeDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_CBDGTU, &CModeDlg::OnCbnSelchangeCbdgtu)
	ON_EN_CHANGE(IDC_EDTTIMELABEL, &CModeDlg::OnEnChangeEdttimelabel)
	ON_CBN_SELCHANGE(IDC_CBPT, &CModeDlg::OnCbnSelchangeCbpt)
	ON_BN_CLICKED(IDC_BTNSS, &CModeDlg::OnBnClickedBtnss)
	ON_MESSAGE(WM_SEND_TESTRECORD, SendTestRecord)
	ON_WM_SERIAL(OnSerialMsg)		// 필수
	ON_MESSAGE(WM_UPDATE_STATINFO, OnUpdateStatusMsg)
END_MESSAGE_MAP()


// CModeDlg 메시지 처리기입니다.


BOOL CModeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cPurgeUnit.AddString(_T("seconds"));
	m_cPurgeUnit.AddString(_T("minutes"));
	m_cPurgeUnit.AddString(_T("hours"));
	m_cPurgeUnit.SetCurSel(0);//! 분으로 설정

	if (CStatus::Instance()->GetProgramMode() != CStatus::PM_MONITORING)
	{
		GetDlgItem(IDC_BTNSS)->EnableWindow(FALSE);
	}

	SetDlgItemText(IDC_EDTPURGETIME, _T("10"));
	SetDlgItemText(IDC_EDTDGT, _T("1"));

	m_cProgramMode.SetFlat(false);

	m_cGTUnit.AddString(_T("seconds"));
	m_cGTUnit.AddString(_T("minutes"));
	m_cGTUnit.AddString(_T("hours"));
	m_cGTUnit.SetCurSel(1);//! 분으로 설정
	m_cSpinPurgeTime.SetRange32(1, 60);
	m_SpinGatheringTime.SetRange32(1, 60);
	m_progPurge.SetRange32(0, 100);
	m_progPurge.SetPos(0);
	m_progMeasure.SetRange32(0, 100);
	m_progMeasure.SetPos(0);
	m_nPurgeStep = 0;

	if(CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING) SerialCheckNInit();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CModeDlg::OnCbnSelchangeCbdgtu()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_cGTUnit.GetCurSel() == 2) m_SpinGatheringTime.SetRange32(1, 3);
	else m_SpinGatheringTime.SetRange32(1, 60);
	SetDlgItemInt(IDC_EDTDGT, 1);
}


void CModeDlg::OnCbnSelchangeCbpt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_cPurgeUnit.GetCurSel() == 2) m_cSpinPurgeTime.SetRange32(1, 3);
	else m_cSpinPurgeTime.SetRange32(1, 60);
	SetDlgItemInt(IDC_EDTPURGETIME, 1);
}

void CModeDlg::OnEnChangeEdttimelabel()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CModeDlg::OnBnClickedBtnss()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_bStart)
	{
		int iPurgeTime, iGatheringTime;
		m_cProgramMode.SetWindowTextW(_T("STOP"));
		m_pRepository->AddEndMark();
		m_pRepository->EraseAllRecords();
		UpdateData(TRUE);

		CString strMark;
		if (m_pViewOptionDlg->m_lstMark.GetCount())
		{
			strMark = m_pViewOptionDlg->m_lstMark.GetAt(0);
			m_pViewOptionDlg->m_strNextMarkName = strMark;
			m_pViewOptionDlg->UpdateData(FALSE);
		}

		CStatus::Instance()->SetMarkPos(0);
		CStatus::Instance()->SetAutoMarkPos(1);
		iPurgeTime = m_cSpinPurgeTime.GetPos();
		int wPurgeTime = (int)(iPurgeTime*pow(60, m_cPurgeUnit.GetCurSel()));
		m_iPurgeSec = wPurgeTime;
		m_progPurge.SetRange32(0, m_iPurgeSec);
		m_progPurge.SetBarColor(RGB(6, 176, 37));
		m_progPurge.SetPos(0);
		m_nPurgeStep = 0;
		iGatheringTime = m_SpinGatheringTime.GetPos();
		int wMeasureTime = (int)(iGatheringTime*pow(60, m_cGTUnit.GetCurSel()));
		m_iMeasureSec = wMeasureTime;
		m_progMeasure.SetRange32(0, m_iMeasureSec);
		m_progMeasure.SetBarColor(RGB(6, 176, 37));
		m_progMeasure.SetPos(0);
		m_APMDPacket.SendStartCommand(wPurgeTime, wMeasureTime);
		m_pWThd = AfxBeginThread(ThreadTrigger, (LPVOID)this);
		m_bStart = true;
	}
	else
	{
		m_cProgramMode.SetWindowTextW(_T("Start"));
		UpdateData(TRUE);
		m_APMDPacket.SendStopCommand();
		m_bStart = false;
	}
}


UINT  CModeDlg::ThreadTrigger(LPVOID pParam)
{
	((CModeDlg *)pParam)->ThreadDataGathering();
	return 0;
}


int CModeDlg::ThreadDataGathering()
{
	//Purge Start

	//Purge End

	//Data acquire start
	SendMessage(WM_SEND_TESTRECORD);
	//Data acquire end
	return 0;
}

LRESULT CModeDlg::SendTestRecord(WPARAM wparam, LPARAM lparam)
{
	
	return 0;
}


void CModeDlg::SerialCheckNInit()
{
	if (CStatus::Instance()->GetProgramMode() != CStatus::PM_MONITORING)return;
	if (m_serial.IsOpen())
	{
		m_serial.Close();
	}

	CString  strSerialNo=CStatus::Instance()->GetComportNo();
	strSerialNo = _T("\\\\.\\") + strSerialNo;

	// 필수 -
	if (m_serial.Open(strSerialNo.GetBuffer(), this) == ERROR_SUCCESS)
	{
		VERIFY(m_serial.Setup(CSerial::EBaud9600, CSerial::EData8, CSerial::EParNone, CSerial::EStop1) == ERROR_SUCCESS);
		VERIFY(m_serial.SetMask(CSerial::EEventBreak | CSerial::EEventError | CSerial::EEventRecv) == ERROR_SUCCESS);
	}
	// - 필수

	if (!m_serial.IsOpen())
	{
		AfxMessageBox(_T("Unable to open COM-port"), MB_ICONSTOP | MB_OK);
	}

}

LRESULT CModeDlg::OnUpdateStatusMsg(WPARAM wParam, LPARAM lParam)	// 필수
{
	tAPMDDispData dispMain, dispCompare;
	memset(&dispMain, 0, sizeof(dispMain));
	memset(&dispCompare, 0, sizeof(dispCompare));
	DispDataUpdate(dispMain, 0);
	DispDataUpdate(dispCompare, 1);


	CStatusDlg *pStatusDlg = m_pStatusDlg;
	m_pStatusDlg->SetDispData(dispMain, dispCompare);

//	pStatusDlg->UpdateData(FALSE);
	return 0;
}

LRESULT CModeDlg::OnSerialMsg(WPARAM wParam, LPARAM lParam)	// 필수
{
	USES_CONVERSION;

	CAPMDPacket::EPacketType eType = m_APMDPacket.CheckoutPacket();
	static int nPrevStep = 0; //0 Stadby 1:// 
//	static int nPurgeStep = 0;
	if (m_pStatusDlg == NULL) return 0;
	CStatusDlg *pStatusDlg = m_pStatusDlg;

	switch (eType)
	{
	case CAPMDPacket::Measure:
	{
		DVC_PACKET_MEASURE* pPacket = (DVC_PACKET_MEASURE*)m_APMDPacket.GetPacket();

		float fTemperature = (float)pPacket->sTemperature / 100.F;
		float fPressure = (float)pPacket->wPressure / 10.F;

		pStatusDlg->m_uiP01Counter = pPacket->wCount01;
		pStatusDlg->m_uiP03Counter = pPacket->wCount03;
		pStatusDlg->m_uiP05Counter = pPacket->wCount05;
		pStatusDlg->m_strAPMDTemp.Format(_T("%.2f℃"), fTemperature);
		pStatusDlg->m_strPressure.Format(_T("%.1f mbar"), fPressure);
		pStatusDlg->m_BatteryState.SetPos(pPacket->byBattery);
		pStatusDlg->m_strHumidity.Format(_T("%d%%"), (int)(pPacket->byHumidity));
		if (pPacket->byHumidity <= 100)
		{
			pStatusDlg->m_HumidityState.SetPos(pPacket->byHumidity);
		}
		else
		{
			pStatusDlg->m_HumidityState.SetBarColor(RGB(200, 200, 200));
		}

		switch (m_APMDPacket.CheckDeviceState(pPacket))
		{
		case CAPMDPacket::StateStandby:
			if (nPrevStep != 0)SetReady();
			pStatusDlg->m_FanIndicator.Set(false);
			pStatusDlg->m_LaserIndicator.Set(false);
			m_strAPMDStatus = _T("STANDBY");
			m_nPurgeStep = 0;
			nPrevStep = 0;
			break;

		case CAPMDPacket::StatePurge:
			m_nPurgeStep++;
			pStatusDlg->m_FanIndicator.Set(true);
			pStatusDlg->m_LaserIndicator.Set(true);
			m_strAPMDStatus = _T("PURGE");
	
			m_progPurge.SetPos(m_nPurgeStep);
			SetDlgItemText(IDC_TIME_TEXT, GetPurgeTimeMsg(m_nPurgeStep));
			nPrevStep = 1; //PurgeStep
			break;

		case CAPMDPacket::StateMeasure:
			if (nPrevStep == 1)m_pRepository->NewMeasure();//!다시 파일생성함(즉 처음부터함)
			if(nPrevStep==1)
				NewMeasureDataAdd(pPacket, true);
			else
				NewMeasureDataAdd(pPacket, false);

			nPrevStep = 2; //measure step
			break;
		default:
			//m_strAPMDStatus = _T("UNKNOWN");
			break;
		}

		UpdateData(FALSE);
		m_pStatusDlg->UpdateData(FALSE);
	}
	break;

	case CAPMDPacket::State:
	{
		DVC_PACKET_STATE* pPacket = (DVC_PACKET_STATE *)m_APMDPacket.GetPacket();
		char szCalDate[MAX_CAL_DATE + 1] = { 0 };
		char szSerialNo[MAX_SERIAL_NO + 1] = { 0 };
		::CopyMemory(szCalDate, pPacket->chCalDate, MAX_CAL_DATE);
		::CopyMemory(szSerialNo, pPacket->chSerialNo, MAX_SERIAL_NO);
		CStatus::Instance()->m_strCalDate = A2CT(szCalDate);
//나중 버젼에 추가하기로 함		CheckCalDate(CStatus::Instance()->m_strCalDate);
		CStatus::Instance()->m_strSerialNo = A2CT(szSerialNo);
		UpdateData(FALSE);
	}
	break;
	default:
		break;
	}

	return 0;
}


void CModeDlg::SetStatusDlg(CStatusDlg * pStatusDlg)
{
	m_pStatusDlg = pStatusDlg;
}


void CModeDlg::SetRepository(CRepository * pRepository)
{
	m_pRepository = pRepository;
}


void CModeDlg::SetGraphDlg(CGraphDlg * pGraphDlg)
{
	m_pGraphDlg = pGraphDlg;
}


void CModeDlg::SetViewOptionDlg(CViewOptionDlg * pViewOption)
{
	m_pViewOptionDlg = pViewOption;
}


void CModeDlg::CheckWarning(bool & bUseWarning, CString & strErrMsg)
{
	bUseWarning = 0 != CStatus::Instance()->m_iUseWarning;

}


void CModeDlg::SetReady()
{
	m_cProgramMode.SetWindowTextW(_T("Start"));
	m_pRepository->AddEndMark();
	m_pGraphDlg->Invalidate();
	m_bStart = false;
}


CString CModeDlg::GetPurgeTimeMsg(int ElapsedSecs)
{
	CString Msg;

	CString strLeftTime;
	CString strHours, strMinutes, strSeconds;

	int leftTime = m_iPurgeSec - ElapsedSecs;
	int iHours = leftTime / 3600;
	int iMinutes = (leftTime % 3600) / 60;
	int iSeconds = leftTime % 60;

	if (iHours)strHours.Format(_T("%dh:"), iHours);
	if (iMinutes)strMinutes.Format(_T("%dm:"), iMinutes);
	if (iSeconds)strSeconds.Format(_T("%ds"), iSeconds);

	strLeftTime = _T("Time Left ");
	if(iHours+iMinutes+ iSeconds)
		strLeftTime = strLeftTime + strHours + strMinutes + strSeconds;
	else strLeftTime = strLeftTime + _T("0s");

	CString strTotalTime, strUnit;
	m_cPurgeUnit.GetLBText(m_cPurgeUnit.GetCurSel(), strUnit);
	strTotalTime.Format(_T(" of %d "), m_cSpinPurgeTime.GetPos());
	strTotalTime = strTotalTime + strUnit;

	Msg = strLeftTime + strTotalTime;
	return Msg;
}


CString CModeDlg::GetMeasureTimeMsg(int ElapsedSecs)
{
	CString Msg;

	CString strLeftTime;
	CString strHours, strMinutes, strSeconds;

	int leftTime = m_iMeasureSec - ElapsedSecs;
	int iHours = leftTime / 3600;
	int iMinutes = (leftTime % 3600) / 60;
	int iSeconds = leftTime % 60;

	if (iHours)strHours.Format(_T("%dh:"), iHours);
	if (iMinutes)strMinutes.Format(_T("%dm:"), iMinutes);
	if (iSeconds)strSeconds.Format(_T("%ds"), iSeconds);

	strLeftTime = _T("Time Left ");
	if (iHours + iMinutes + iSeconds)
		strLeftTime = strLeftTime + strHours + strMinutes + strSeconds;
	else strLeftTime = strLeftTime + _T("0s");

	CString strTotalTime, strUnit;
	m_cGTUnit.GetLBText(m_cGTUnit.GetCurSel(), strUnit);
	strTotalTime.Format(_T(" of %d "), m_SpinGatheringTime.GetPos());
	strTotalTime = strTotalTime + strUnit;

	Msg = strLeftTime + strTotalTime;
	return Msg;
}


BOOL CModeDlg::PreTranslateMessage(MSG* pMsg)
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


int CModeDlg::NewMeasureDataAdd(DVC_PACKET_MEASURE* pPacket, bool IsFirst)
{
	static int prevSec=0;
	int iAccum01, iAccum03, iAccum05;
	CString strAddMark;
	CString ErrMsg;
	tAPMDRecord *pNewRecord;
	CStatusDlg *pStatusDlg = m_pStatusDlg;

	pNewRecord = new tAPMDRecord;
	pNewRecord->Particle01 = pPacket->wCount01;
	pNewRecord->Particle03 = pPacket->wCount03;
	pNewRecord->Particle05 = pPacket->wCount05;
	pNewRecord->Battery = pPacket->byBattery;
	pNewRecord->Humidity = pPacket->byHumidity;
	pNewRecord->Temperature = pPacket->sTemperature;
	pNewRecord->TimeInfo = pPacket->wSec;
	pNewRecord->Pressure = pPacket->wPressure;

	strAddMark = m_pViewOptionDlg->GetMark();
	if (!strAddMark.IsEmpty())
	{
		pNewRecord->strMark = strAddMark;
		strAddMark = _T("");
	}

	if (IsFirst)prevSec = 0;
	if ((prevSec + 1) != pPacket->wSec)
	{
		MakeDisconnectedData(prevSec, pPacket->wSec);
		pNewRecord->strMark = _T("Connected");
	}else
	{

	}
	if (prevSec == 0)
	{
		if(pNewRecord->strMark.IsEmpty())
			pNewRecord->strMark = _T("Start");
		else
			pNewRecord->strMark += _T("-Start");
	}

	m_pRepository->AppendData(pNewRecord);
	m_pGraphDlg->m_cNavi.SetRangeMax(m_pRepository->m_DataRecords[0].nCountOfRecords);
	m_pGraphDlg->m_cNavi.SetPos(m_pRepository->m_DataRecords[0].nCountOfRecords);
	m_pGraphDlg->m_cParticlePlot.SetCurIndexNo(-1);
	m_pRepository->CalculateSum(iAccum01, iAccum03, iAccum05);
	bool bUseCheckWarning, bIsError;
	bUseCheckWarning = false; bIsError = false;
	m_pRepository->CheckWarning(bUseCheckWarning, bIsError, ErrMsg);
	m_pStatusDlg->CheckWarnDisp(bUseCheckWarning, bIsError, ErrMsg);
	pStatusDlg->UpdateStatisticChart(iAccum01, iAccum03, iAccum05);
	pStatusDlg->m_FanIndicator.Set(true);
	pStatusDlg->m_LaserIndicator.Set(true);
	if (!CStatus::Instance()->IsOnNavi())
		m_pGraphDlg->Invalidate();
	m_strAPMDStatus = _T("MEASURE");
	
	m_progMeasure.SetPos(pPacket->wSec);
	SetDlgItemText(IDC_TIME_TEXT, GetMeasureTimeMsg(pPacket->wSec));
	prevSec = pPacket->wSec;
	return 0;
}


int CModeDlg::MakeDisconnectedData(int prevSec, int curSec)
{
	tAPMDRecord *pNewRecord;
	
	for (int i = prevSec+1; i < curSec; i++)
	{
		pNewRecord = new tAPMDRecord;
		pNewRecord->TimeInfo = i;
		pNewRecord->Particle01 = 0;
		pNewRecord->Particle03 = 0;
		pNewRecord->Particle05 = 0;
		pNewRecord->Battery = 0;
		pNewRecord->Humidity = 0;
		pNewRecord->Temperature = 0;
		pNewRecord->Pressure = 0;

		if (i == (prevSec+1))
		{
			pNewRecord->strMark = _T("Disconnected");
		}
		m_pRepository->AppendData(pNewRecord);
	}
	
	return 0;
}


int CModeDlg::CheckCalDate(CString strCalDate)
{
	static bool IsFirst = true;
	if (!IsFirst)return -1;

	CString strTemp=strCalDate;
	CTime timeCurrent = CTime::GetCurrentTime();
	CString strYear, strMon, strDay;
	strYear = strTemp.Left(2);
	strYear = _T("20") + strYear;
	strTemp = strTemp.Mid(2);
	strMon = strTemp.Left(2);
	strTemp = strTemp.Mid(2);
	strDay = strTemp.Left(2);
	int nYear, nMon, nDay;
	nYear = _wtoi(strYear);
	nMon = _wtoi(strMon);
	nDay = _wtoi(strDay);
	CTime timeCalibrated(nYear, nMon, nDay, 0, 0, 0);
	CTimeSpan tDiff = timeCurrent - timeCalibrated;
	if (tDiff.GetDays() > 365)
	{
		IsFirst = false;
		AfxMessageBox(_T("Calibration time is expired. You need to calibrate APMD."), MB_OK);
	}

	return 0;
}


int CModeDlg::DispDataUpdate(tAPMDDispData &DispData, int KindOfData)
{
	m_pRepository;
	CStatusDlg *pStatusDlg = m_pStatusDlg;
	int curIndexNo = -1;
	if(KindOfData==0)
		curIndexNo = m_pGraphDlg->m_cParticlePlot.GetCurIndexNo();
	else
		curIndexNo = m_pGraphDlg->m_cComparePlot.GetCurIndexNo();

	POSITION pos = m_pRepository->m_DataRecords[KindOfData].m_APMDRecords.FindIndex(curIndexNo);
	if (!pos)return -1;
	tAPMDRecord *pNewRecord = (tAPMDRecord *)(m_pRepository->m_DataRecords[KindOfData].m_APMDRecords.GetAt(pos));
	int nAcc01=0, nAcc03=0, nAcc05=0;
	m_pRepository->CalculateSum(curIndexNo, nAcc01, nAcc03, nAcc05, KindOfData);

	float fTemperature = (float)pNewRecord->Temperature / 100.F;
	float fPressure = (float)pNewRecord->Pressure / 10.F;

	DispData.iParticle01 = pNewRecord->Particle01;
	DispData.iParticle03 = pNewRecord->Particle03;
	DispData.iParticle05 = pNewRecord->Particle05;
	DispData.nAcc01 = nAcc01;
	DispData.nAcc03 = nAcc03;
	DispData.nAcc05 = nAcc05;
	DispData.dblTemp = fTemperature;
	DispData.dblPressure = fPressure;
	DispData.iHumi = pNewRecord->Humidity;

	return 0;
}
