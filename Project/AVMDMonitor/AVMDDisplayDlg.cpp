// AVMDDisplayDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitorDoc.h"
#include "AVMDMonitorView.h"
#include "AVMDMonitor.h"

#include "AVMDDisplayDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "Repository.h"
#include "AVMDDefine.h"
#include "AVMDViewDlg.h"
#include "MainFrm.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define TEST_TIME 7

// CAVMDDisplayDlg 대화 상자입니다.
#pragma comment(lib, "winmm.lib")
#include "Mmsystem.h"

IMPLEMENT_DYNAMIC(CAVMDDisplayDlg, CDialogEx)

CAVMDDisplayDlg::CAVMDDisplayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DISPLAY, pParent)
	, m_AVMDPacket(m_serial)
	, m_bMinimize_RMS(FALSE)
	, m_bMinimize_X(FALSE)
	, m_bMinimize_Y(FALSE)
	, m_bMinimize_Z(FALSE)
	, m_bFFTShow(FALSE)
	, m_strTemperatureStatus(_T(""))
	, m_strBatteryStatus(_T(""))
	, m_strGraphScale(_T(""))
	, m_strGraphTimeUnit(_T(""))
	, m_iProgramMode(0)
	, m_iEventMode(0)
	, m_strFilterStatus(_T(""))
	, m_strTimeDomain(_T(""))
	, m_strTimeUnit(_T("g/div"))
	, m_bReceived(FALSE)
	, m_bReconnected(FALSE)
{
	CStatus::Instance()->SetAVMDDisPlayView(this);
	SerialClose(); //m_AVMDPacket.SendMeasureCommand();
}

CAVMDDisplayDlg::~CAVMDDisplayDlg()
{
}

void CAVMDDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_TRACK_RMS, m_bMinimize_RMS);
	DDX_Check(pDX, IDC_CHK_TRACK_X, m_bMinimize_X);
	DDX_Check(pDX, IDC_CHK_TRACK_Y, m_bMinimize_Y);
	DDX_Check(pDX, IDC_CHK_TRACK_Z, m_bMinimize_Z);
	DDX_Check(pDX, IDC_CHK_FFT_SHOW, m_bFFTShow);
	DDX_Control(pDX, IDC_CB_TIME_SCALE, m_combo_GraphScale);
	DDX_CBString(pDX, IDC_CB_TIME_SCALE, m_strGraphScale);
	DDX_Control(pDX, IDC_CB_HOFRZON_SCALE, m_combo_GraphTimeUnit);
	DDX_CBString(pDX, IDC_CB_HOFRZON_SCALE, m_strGraphTimeUnit);
	DDX_Radio(pDX, IDC_RADIO_VIEW, m_iProgramMode);
	DDX_Control(pDX, IDC_CB_SAMPLESIZE, m_combo_FFTSampleSize);
	DDX_Control(pDX, IDC_STATIC_FILTER_STATUS, m_ctrlText_FilterStatus);
	DDX_Text(pDX, IDC_STATIC_FILTER_STATUS, m_strFilterStatus);
	DDX_Control(pDX, IDC_STATIC_TIME, m_CtrlEdit_TimeDomain);
	DDX_Text(pDX, IDC_STATIC_TIME, m_strTimeDomain);
	DDX_Control(pDX, IDC_CB_FS_SCALE, m_combo_SpectrumScale);
	DDX_Control(pDX, IDC_CB_FFT_HOFRZON_SCALE, m_combo_FFTGraphUnit);
	//	DDX_Control(pDX, IDC_BTN_HOLD, m_btnHold);
	DDX_Control(pDX, IDC_SLIDER_FFT_SHOW, m_Slider_FFT_Show);
}


BEGIN_MESSAGE_MAP(CAVMDDisplayDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHK_TRACK_RMS, &CAVMDDisplayDlg::OnBnClickedChkTrackRMS)
	ON_BN_CLICKED(IDC_CHK_TRACK_X, &CAVMDDisplayDlg::OnBnClickedChkTrackX)
	ON_BN_CLICKED(IDC_CHK_TRACK_Y, &CAVMDDisplayDlg::OnBnClickedChkTrackY)
	ON_BN_CLICKED(IDC_CHK_TRACK_Z, &CAVMDDisplayDlg::OnBnClickedChkTrackZ)
	ON_BN_CLICKED(IDC_CHK_FFT_SHOW, &CAVMDDisplayDlg::OnBnClickedChkFftShow)
	ON_MESSAGE(WM_DISPLAYUIUPDATE, &CAVMDDisplayDlg::UpdateFilterTextBox)
	ON_MESSAGE(WM_DISPLAYCOMBOBOXUPDATE, &CAVMDDisplayDlg::ComboUpdateUI)
	ON_MESSAGE(WM_TIME_UNIT, &CAVMDDisplayDlg::TimeComboInialize)
	ON_MESSAGE(WM_UIUPDATE, &CAVMDDisplayDlg::UpdateTimeDomainTextBox)
	ON_MESSAGE(WM_PM_CHANGE, &CAVMDDisplayDlg::OnProgramModeChange)
	ON_BN_CLICKED(IDC_BUTTON1, &CAVMDDisplayDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_CB_TIME_SCALE, &CAVMDDisplayDlg::OnCbnSelchangeCbTimeScale)
	ON_CBN_SELCHANGE(IDC_CB_HOFRZON_SCALE, &CAVMDDisplayDlg::OnCbnSelchangeCbHorizonScale)
	ON_BN_CLICKED(IDC_RADIO_VIEW, &CAVMDDisplayDlg::OnBnClickedRadioView)
	ON_BN_CLICKED(IDC_RADIO_REVIEW, &CAVMDDisplayDlg::OnBnClickedRadioReview)
	ON_WM_CLOSE()

ON_CBN_SELCHANGE(IDC_CB_SAMPLESIZE, &CAVMDDisplayDlg::OnCbnSelchangeCbSamplesize)
ON_CBN_SELCHANGE(IDC_CB_FS_SCALE, &CAVMDDisplayDlg::OnCbnSelchangeCbFsScale)
//ON_BN_CLICKED(IDC_BTN_HOLD, &CAVMDDisplayDlg::OnBnClickedBtnHold)

ON_WM_TIMER()
ON_CBN_SELCHANGE(IDC_CB_FFT_HOFRZON_SCALE, &CAVMDDisplayDlg::OnCbnSelchangeCbFftHorizonScale)
ON_WM_CONTEXTMENU()
ON_COMMAND(ID_TEST_TESTFILEWRITE, &CAVMDDisplayDlg::OnTestTestfilewrite)
ON_WM_HSCROLL()
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FFT_SHOW, &CAVMDDisplayDlg::OnNMReleasedcaptureSliderFftShow)
END_MESSAGE_MAP()


// CAVMDDisplayDlg 메시지 처리기입니다.

#pragma region 체크 박스
void CAVMDDisplayDlg::OnBnClickedChkTrackRMS()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_bMinimize_RMS && m_bMinimize_X && m_bMinimize_Y && m_bMinimize_Z)
	{
		AfxMessageBox(_T("All Graphs can't minimized."));
		m_bMinimize_RMS = !m_bMinimize_RMS;
		UpdateData(FALSE);
	}

	CStatus::SetMinimize(m_bMinimize_RMS, m_bMinimize_X, m_bMinimize_Y, m_bMinimize_Z);
	UpdateData(FALSE);
}

void CAVMDDisplayDlg::OnBnClickedChkTrackX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_bMinimize_RMS && m_bMinimize_X && m_bMinimize_Y && m_bMinimize_Z)
	{
		AfxMessageBox(_T("All Graphs can't minimized."));
		m_bMinimize_X = !m_bMinimize_X;
	}

	CStatus::SetMinimize(m_bMinimize_RMS, m_bMinimize_X, m_bMinimize_Y, m_bMinimize_Z);
	UpdateData(FALSE);
}

void CAVMDDisplayDlg::OnBnClickedChkTrackY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_bMinimize_RMS && m_bMinimize_X && m_bMinimize_Y && m_bMinimize_Z)
	{
		AfxMessageBox(_T("All Graphs can't minimized."));
		m_bMinimize_Y = !m_bMinimize_Y;
	}

	CStatus::SetMinimize(m_bMinimize_RMS, m_bMinimize_X, m_bMinimize_Y, m_bMinimize_Z);
	UpdateData(FALSE);
}

void CAVMDDisplayDlg::OnBnClickedChkTrackZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_bMinimize_RMS && m_bMinimize_X && m_bMinimize_Y && m_bMinimize_Z)
	{
		AfxMessageBox(_T("All Graphs can't minimized."));
		m_bMinimize_Z = !m_bMinimize_Z;
	}

	CStatus::SetMinimize(m_bMinimize_RMS, m_bMinimize_X, m_bMinimize_Y, m_bMinimize_Z);
	UpdateData(FALSE);
}

void CAVMDDisplayDlg::OnBnClickedChkFftShow()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	UpdateData(TRUE);
	m_pView->SetDrawPointNum();
	CStatus::SetShowFFTGraph(m_bFFTShow);

	if (m_bFFTShow)
	{
		GetDlgItem(IDC_CB_FS_SCALE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CB_SAMPLESIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CB_FFT_HOFRZON_SCALE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_FFT_SHOW)->EnableWindow(TRUE);
		CString strTemp;
		GetDlgItemText(IDC_CB_FFT_HOFRZON_SCALE, strTemp);
		int nFFTRatio = _ttoi(strTemp);
		if (strTemp == "Full")
		{
			GetDlgItem(IDC_SLIDER_FFT_SHOW)->EnableWindow(FALSE);
			CStatus::Instance()->m_iFFTShowTempStartPos = 0;
			m_Slider_FFT_Show.SetPos(0);
			nFFTRatio = 1;
		}
		else
			GetDlgItem(IDC_SLIDER_FFT_SHOW)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CB_FS_SCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CB_SAMPLESIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CB_FFT_HOFRZON_SCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_FFT_SHOW)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}
#pragma endregion

BOOL CAVMDDisplayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_combo_GraphScale.AddString(_T("1"));
	m_combo_GraphScale.AddString(_T("2"));
	m_combo_GraphScale.AddString(_T("4"));
	m_combo_GraphScale.AddString(_T("8"));
	m_combo_GraphScale.AddString(_T("16"));
	m_combo_GraphScale.AddString(_T("32"));
	m_combo_GraphScale.AddString(_T("64"));
	m_combo_GraphScale.AddString(_T("128"));

	m_combo_GraphTimeUnit.AddString(_T("0.1"));
	m_combo_GraphTimeUnit.AddString(_T("0.2"));
	m_combo_GraphTimeUnit.AddString(_T("0.4"));
	m_combo_GraphTimeUnit.AddString(_T("0.8"));
	m_combo_GraphTimeUnit.AddString(_T("1.0"));
	m_combo_GraphTimeUnit.AddString(_T("2.0"));
	m_combo_GraphTimeUnit.AddString(_T("5.0"));
	m_combo_GraphTimeUnit.AddString(_T("10.0"));
	m_combo_GraphTimeUnit.AddString(_T("20.0"));
	//m_combo_GraphTimeUnit.AddString(_T("50.0"));
	//m_combo_GraphTimeUnit.AddString(_T("100.0"));
//jubadel	
	int iMaxHz = (int)CStatus::Instance()->m_DeviceFrequency / 2;
	m_Slider_FFT_Show.SetRange(0, iMaxHz);
	m_Slider_FFT_Show.SetPos(0);

	//
	m_combo_FFTSampleSize.Clear();
	m_combo_FFTSampleSize.AddString(_T("64"));
	m_combo_FFTSampleSize.AddString(_T("128"));
	m_combo_FFTSampleSize.AddString(_T("256"));
	m_combo_FFTSampleSize.AddString(_T("512"));
	m_combo_FFTSampleSize.AddString(_T("1024"));
	m_combo_FFTSampleSize.AddString(_T("2048"));
	m_combo_FFTSampleSize.AddString(_T("4096"));

	CString strTimeUnit = _T("g/div");
	m_combo_SpectrumScale.Clear();
	m_combo_SpectrumScale.AddString(_T("0.005") + strTimeUnit);
	m_combo_SpectrumScale.AddString(_T("0.010") + strTimeUnit);
	m_combo_SpectrumScale.AddString(_T("0.025") + strTimeUnit);
	m_combo_SpectrumScale.AddString(_T("0.050") + strTimeUnit);
	m_combo_SpectrumScale.AddString(_T("0.100") + strTimeUnit);
	m_combo_SpectrumScale.AddString(_T("0.250") + strTimeUnit);
	m_combo_SpectrumScale.AddString(_T("0.500") + strTimeUnit);

	m_combo_FFTGraphUnit.AddString(_T("Full"));
	m_combo_FFTGraphUnit.AddString(_T("1x"));
	m_combo_FFTGraphUnit.AddString(_T("2x"));
	m_combo_FFTGraphUnit.AddString(_T("3x"));
	//m_combo_FFTGraphUnit.AddString(_T("500"));

	m_combo_GraphScale.SetCurSel(0);
	m_combo_GraphTimeUnit.SetCurSel(0);
	m_combo_FFTSampleSize.SetCurSel(6);
	m_combo_SpectrumScale.SetCurSel(6);
	m_combo_FFTGraphUnit.SetCurSel(0);

	CString strFilterStatus = CStatus::Instance()->m_strFilterStatus;
	SetDlgItemText(IDC_STATIC_FILTER_STATUS, strFilterStatus);

	CString strScale, strFftSampleSize, strFFTScale;

	m_combo_GraphScale.GetLBText(this->m_combo_GraphScale.GetCurSel(), strScale);
	m_combo_GraphTimeUnit.GetLBText(this->m_combo_GraphTimeUnit.GetCurSel(), strTimeUnit);
	m_combo_FFTSampleSize.GetLBText(this->m_combo_FFTSampleSize.GetCurSel(), strFftSampleSize);
	m_combo_SpectrumScale.GetLBText(this->m_combo_SpectrumScale.GetCurSel(), strFFTScale);

	GetDlgItem(IDC_CB_FS_SCALE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CB_SAMPLESIZE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CB_FFT_HOFRZON_SCALE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER_FFT_SHOW)->EnableWindow(FALSE);
	m_strTimeDomain.Format(_T("%s"), CStatus::Instance()->m_strTimeDomainUnit);
	m_strFilterStatus.Format(_T("%s"), CStatus::Instance()->m_strFilterStatus);

	CStatus::Instance()->m_strGraphScale = strScale;
	int iRealSampleSize = _ttoi(strFftSampleSize);
	CStatus::Instance()->m_FFT_SampleSize.Format(_T("%d"), iRealSampleSize);
	CStatus::Instance()->m_strFFTScale = strFFTScale;

	m_iProgramMode = CStatus::Instance()->GetProgramMode();
	m_iEventMode = CStatus::Instance()->GetEventMode();
	SetTimer(TEST_TIME, 40, NULL);

	CStatus::Instance()->SetReviewDragReset(0);
	CStatus::Instance()->SetReviewDragReset(1);

	//UpdateData(TRUE);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


int CAVMDDisplayDlg::RcvSerialMsg(WPARAM wParam)
{
	CSerial::EEvent eEvent = (CSerial::EEvent)wParam;

	static int prevTime = 0;
	static int prevNum = 0;
	static int Packet0Time = 0;
	static int Packet255Time = 0;
	USES_CONVERSION;
	CAVMDPacket::EPacketType eType = CAVMDPacket::EPacketType::None;
	
	if (eEvent == CSerial::EEvent::EEventRecv)
		eType = m_AVMDPacket.CheckoutPacket();
	else
		TRACE(_T("Serial Event: %d\n"), eEvent);

	if (eType == CAVMDPacket::None) return -1;

	//if (prevTime == 0)prevTime = timeGetTime();
	//int curTime = timeGetTime();
	//printf("\nInterval Packet %dms",curTime-prevTime);
	//prevTime = curTime;

	switch (eType)
	{
	case CAVMDPacket::SensorInfo:
	{
		DVC_PACKET_SENSORINFO* pPacket = new DVC_PACKET_SENSORINFO;
		m_AVMDPacket.GetQueuePacket((BYTE*)pPacket, CAVMDPacket::SensorInfo);
		switch (pPacket->byRadius)
		{
		case CAVMDPacket::W150MM:
			CStatus::Instance()->SetWaferSize(150);
			break;
		case CAVMDPacket::W200MM:
			CStatus::Instance()->SetWaferSize(200);
			break;
		case CAVMDPacket::W300MM:
			CStatus::Instance()->SetWaferSize(300);
			break;
		case CAVMDPacket::W450MM:
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
		CStatus::Instance()->m_strSerialNo[0] = A2CT(szSerialNo);
	}
	break;
	case CAVMDPacket::Measure:
	{
		DVC_PACKET_MEAINFO* pPacket = new DVC_PACKET_MEAINFO;
		m_AVMDPacket.GetQueuePacket((BYTE*)pPacket, CAVMDPacket::Measure);
		if (pPacket->byNum == 0)
			Packet0Time = timeGetTime();
		if (pPacket->byNum == 255)
		{
			Packet255Time = timeGetTime();
			if (Packet0Time)printf("\n0~255:Time %d one packet time:%f", Packet255Time - Packet0Time, (Packet255Time - Packet0Time)/256.);
		} //! check the elapse time, every 256 packets 

//		if( (nStep%10) == 0 )
		
//		int ax, ay, az;
//		CAVMDPacket::GetXYZ(ax, ay, az, (BYTE *)(pPacket->accInfo));
//		printf("\nACCX:%5d", ax);
		if (((int)(pPacket->byNum) - prevNum) != 1)
		{
			if( (prevNum!=255) || (pPacket->byNum != 0) )
			{
				printf("\nPrevNum:%d Num:%3d:%3d", prevNum, (int)(pPacket->byNum), nStep++);
				if (CStatus::Instance()->GetRepository()->m_iCSVDispIndexNo)
					if (CStatus::Instance()->GetRepository()->GetFileSaveStatus() == CRepository::SS_WRITE)
					{
						m_bReconnected = TRUE;
					}
					else
					{
						m_bReconnected = FALSE;
					}
			}
		}
		prevNum = pPacket->byNum; //! Check the timming 

		SetStateInfo(pPacket);
	}
	break;

	}
	return 0;
}

void CAVMDDisplayDlg::SetStateInfo(DVC_PACKET_MEAINFO * pPacketInfo)
{
	MakeWriteInfo(pPacketInfo, 0);
}

int CAVMDDisplayDlg::MakeWriteInfo(DVC_PACKET_MEAINFO *pPacketInfo, int nInputType)
{
	tAVMDData AVMDLastData;

//	MakeSimulationData(pPacketInfo);

	AppendMarkInfo(nInputType);
	CStatus::Instance()->GetRepository()->AppendData(pPacketInfo);
	CStatus::Instance()->GetRepository()->GetLastData(&AVMDLastData);
	
	CStatus::Instance()->SetMeasureData(&AVMDLastData);
	return 0;
}

void CAVMDDisplayDlg::SerialCheckNInit()
{
	if (m_serial.IsOpen())
	{
		m_serial.Close();
	}

	m_serial.RegCallback(std::bind(&CAVMDDisplayDlg::ProcessCMD, this, std::placeholders::_1));
	m_serial.RegAnalizerCallback(std::bind(&CAVMDDisplayDlg::CheckoutPacket, this));

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

void CAVMDDisplayDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	

	SerialCheckNInit();
	Sleep(1000);
	m_AVMDPacket.SendMeasureCommand();
}

void CAVMDDisplayDlg::OnCbnSelchangeCbTimeScale()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strScale;
	m_combo_GraphScale.GetLBText(this->m_combo_GraphScale.GetCurSel(), strScale);

	CStatus::Instance()->m_strGraphScale = strScale;
//	CStatus::Instance()->PlayerStart();
	//UpdateData(FALSE);
	//CStatus::Instance()->DisplayDlgUpdateUI();
	CStatus::Instance()->GraphUpdate();
}

void CAVMDDisplayDlg::OnCbnSelchangeCbHorizonScale()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTimeScale;
	m_combo_GraphTimeUnit.GetLBText(this->m_combo_GraphTimeUnit.GetCurSel(), strTimeScale);
	double dblTimeScale = _wtof(strTimeScale);

	CStatus::Instance()->m_dblUnit = dblTimeScale;
	
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		m_pView->SetDrawPointNum();
		CStatus::Instance()->m_iDataCount = CStatus::Instance()->GetRepository()->GetDataCount(0);
		CStatus::Instance()->m_iDataCompareCount = CStatus::Instance()->GetRepository()->GetDataCount(1);
	
		CStatus::Instance()->m_bIsTimeScaleChange = TRUE;

		if(CStatus::Instance()->GetCompareCondition())
			CStatus::Instance()->m_bIsCompareTimeScaleChange = TRUE;
	}
	
	CStatus::Instance()->GraphUpdate();
	CStatus::Instance()->UpdateScrn();
}

void CAVMDDisplayDlg::OnBnClickedRadioView()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
		return;
	m_iProgramMode = 0;
	CStatus::Instance()->ReviewDataClear(TRUE, TRUE);
	CStatus::Instance()->m_bCompareReEnter = FALSE;
	//CStatus::Instance()->m_bCompareReEnter = FALSE;
	CStatus::Instance()->GetRepository()->ClearData(0);
	CStatus::Instance()->GetRepository()->SetDummyData();
	CStatus::Instance()->GetRepository()->ClearData(1);
	CStatus::Instance()->SetCompareCondition(FALSE);
	CStatus::Instance()->SetOverlayCondition(FALSE);
	CStatus::Instance()->ReviewMenuReset();
	CStatus::Instance()->SetProgramMode(CStatus::PM_MONITORING);
	CStatus::Instance()->m_strFilterStatus = _T("disable");
	CStatus::Instance()->SetFilterType(FALSE, FALSE, FALSE);
	m_combo_GraphTimeUnit.ResetContent();
	m_combo_GraphTimeUnit.AddString(_T("0.1"));
	m_combo_GraphTimeUnit.AddString(_T("0.2"));
	m_combo_GraphTimeUnit.AddString(_T("0.4"));
	m_combo_GraphTimeUnit.AddString(_T("1.0"));
	
	m_combo_GraphTimeUnit.SetCurSel(0);
	CStatus::Instance()->m_dblUnit = 0.1;

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->SetTitleCaptionReset();
	CStatus::Instance()->UpdateScrn();
}

void CAVMDDisplayDlg::OnBnClickedRadioReview()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
		return;
	CStatus::Instance()->SetProgramMode(CStatus::PM_REVIEW);
	m_iProgramMode = 1;

	CStatus::Instance()->UpdateViewHoldButton();
	CStatus::Instance()->GetRepository()->ClearData(0);
	CStatus::Instance()->ReviewComboClear();
	CStatus::Instance()->SetEventMode(CStatus::tEventMode::Evt_NONE);
	CStatus::Instance()->SetIsHoldCondition(FALSE);
	CStatus::Instance()->m_strFilterStatus = _T("disable");
	CStatus::Instance()->SetFilterType(FALSE, FALSE, FALSE);

	m_combo_GraphTimeUnit.ResetContent();

	m_combo_GraphTimeUnit.AddString(_T("0.1"));
	m_combo_GraphTimeUnit.AddString(_T("0.2"));
	m_combo_GraphTimeUnit.AddString(_T("0.4"));
	m_combo_GraphTimeUnit.AddString(_T("0.8"));
	m_combo_GraphTimeUnit.AddString(_T("1.0"));
	m_combo_GraphTimeUnit.AddString(_T("2.0"));
	m_combo_GraphTimeUnit.AddString(_T("5.0"));
	m_combo_GraphTimeUnit.AddString(_T("10.0"));
	m_combo_GraphTimeUnit.AddString(_T("20.0"));
	//m_combo_GraphTimeUnit.AddString(_T("50.0"));
	//m_combo_GraphTimeUnit.AddString(_T("100.0"));

//jubadel	m_combo_GraphTimeUnit.AddString(_T("50.0"));
//	m_combo_GraphTimeUnit.AddString(_T("100.0"));
	m_combo_GraphTimeUnit.SetCurSel(0);
	CStatus::Instance()->m_dblUnit = 0.1;
	CStatus::Instance()->SetReviewDragReset(0);
	CStatus::Instance()->SetReviewDragReset(1); 

	CStatus::Instance()->SetMarkClickEvent(FALSE, 0);
	CStatus::Instance()->SetMarkClickEvent(FALSE, 1);
	CStatus::Instance()->UpdateScrn();
}

void CAVMDDisplayDlg::SerialClose()
{
	if (m_serial.IsOpen())
	{
		m_AVMDPacket.SendSensorInfoCommand();
		Sleep(300);
		m_serial.Close();
	}
}

void CAVMDDisplayDlg::SetProgramMode(CStatus::tProgramMode ProgramMode)
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
		m_AVMDPacket.SendSensorInfoCommand();
		Sleep(200);
		m_AVMDPacket.SendMeasureCommand();

		//CStatus::Instance()->ledstatus
	}
}

LRESULT CAVMDDisplayDlg::OnProgramModeChange(WPARAM wParam, LPARAM lParam)
{
	SetProgramMode(CStatus::Instance()->GetProgramMode());
	SetTimer(0, 200, NULL);
	return 0;
}

void CAVMDDisplayDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
	{
		SerialClose();
	}
	CDialogEx::OnClose();
}

void CAVMDDisplayDlg::ProcessCMD(int nEvent)
{
	RcvSerialMsg((WPARAM)nEvent);
	SetFlagReceived();
}

void CAVMDDisplayDlg::OnCbnSelchangeCbSamplesize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetRepository()->GetFileSaveStatus() == CRepository::SS_WRITE)
	{
		AfxMessageBox(_T("You can not change the filter in the save mode."));
		int iFFTSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
		if (iFFTSampleSize == 32)		m_combo_FFTSampleSize.SetCurSel(0);
		if (iFFTSampleSize == 64)		m_combo_FFTSampleSize.SetCurSel(1);
		if (iFFTSampleSize == 128)		m_combo_FFTSampleSize.SetCurSel(2);
		if (iFFTSampleSize == 256)		m_combo_FFTSampleSize.SetCurSel(3);
		if (iFFTSampleSize == 512)		m_combo_FFTSampleSize.SetCurSel(4);
		if (iFFTSampleSize == 1024)		m_combo_FFTSampleSize.SetCurSel(5);
		if (iFFTSampleSize == 2048)		m_combo_FFTSampleSize.SetCurSel(6);
		if (iFFTSampleSize == 4096)		m_combo_FFTSampleSize.SetCurSel(7);
		return;
	}
	CString strScale;
	m_combo_FFTSampleSize.GetLBText(this->m_combo_FFTSampleSize.GetCurSel(), strScale);
	int iRealSampleSize = 0;
	int nFrequeny = (int)(CStatus::Instance()->GetDeviceFrequency());
	if(nFrequeny == 500)
		iRealSampleSize = _ttoi(strScale) / 2 ;
	if(nFrequeny == 1000)
		iRealSampleSize = _ttoi(strScale);
	CStatus::Instance()->m_FFT_SampleSize.Format(_T("%d"), iRealSampleSize);
	UpdateData(FALSE);
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
		CStatus::Instance()->DisplayDlgUpdateUI();	
}

LRESULT CAVMDDisplayDlg::ComboUpdateUI(WPARAM wParam, LPARAM lParam)
{
	CString strTimedomain = CStatus::Instance()->m_strTimeDomainUnit;

	int iTextCount, iTotalCount;
	CString strScaleValue;

	if (strTimedomain == "g-Force")
	{
		m_strTimeUnit = _T("g/div");
		iTextCount = m_strTimeUnit.GetLength();
		m_combo_SpectrumScale.ResetContent();
		m_combo_SpectrumScale.AddString(_T("0.005") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("0.010") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("0.025") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("0.050") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("0.100") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("0.250") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("0.500") + m_strTimeUnit);
		m_combo_SpectrumScale.SetCurSel(6);
	}
	if (strTimedomain == "GAL")
	{
		m_strTimeUnit = _T("GAL/div");
		iTextCount = m_strTimeUnit.GetLength();
		m_combo_SpectrumScale.ResetContent();
		m_combo_SpectrumScale.AddString(_T("5") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("10") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("25") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("50") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("100") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("250") + m_strTimeUnit);
		m_combo_SpectrumScale.AddString(_T("500") + m_strTimeUnit);
		m_combo_SpectrumScale.SetCurSel(6);
	}

	m_combo_SpectrumScale.GetLBText(m_combo_SpectrumScale.GetCurSel(), strScaleValue);
	iTotalCount = strScaleValue.GetLength();

	CStatus::Instance()->m_strFFTScale = strScaleValue.Left(iTotalCount - iTextCount);

	return 0;
}

LRESULT CAVMDDisplayDlg::TimeComboInialize(WPARAM wParam, LPARAM lParam)
{
	m_combo_GraphTimeUnit.SetCurSel(0);
	return 0;
}

void CAVMDDisplayDlg::OnCbnSelchangeCbFsScale()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iTextCount, nTotalCount;
	CString strScaleValue;
	
	m_combo_SpectrumScale.GetLBText(m_combo_SpectrumScale.GetCurSel(), strScaleValue);
	iTextCount = m_strTimeUnit.GetLength();
	nTotalCount = strScaleValue.GetLength();

	CStatus::Instance()->m_strFFTScale = strScaleValue.Left(nTotalCount - iTextCount);
	CStatus::Instance()->UpdateScrn();
}

static int nIndex = 0;
static int nTest = 0;
static double xx[1024];
static int j = 0;
void CAVMDDisplayDlg::MakeSimulationData(DVC_PACKET_MEAINFO * pPacketInfo)
{
	//double dbl = 2.*10.*M_PI / SAMPLEING_RATE;
	//for (int i = 0; i < GRAPH_MAX_DATA_NUM; i++)
	//{
	int i = nTest % 25;
	for (int i = 0; i < 25; i++)
	{
		//pPacketInfo->accInfo[i].x = (double)900 * sin(2.*1.*M_PI*(double)nIndex / 180.);// +10.*cos(2.*M_PI*nIndex / 180.));
		int accX = (int)((double)981. * (sin(2.*1.*M_PI*(double)nIndex / 180.) + sin(2.*10.*M_PI*(double)nIndex / 180.)));
		int accY = (int)((double)981. * (sin(2.*10.*M_PI*(double)nIndex / 180.)));
		int accZ = (int)((double)981. * sin(2.*1.*M_PI*(double)nIndex / 180.));

		CAVMDPacket::SetXYZ((BYTE *)(&(pPacketInfo->accInfo[i])), accX, accY, accZ);
		/*	xx[j] = pPacketInfo->accInfo[i].z;
		j++;
		if(j>1024)
		j %= 1024;*/
		nIndex++;
		nIndex %= 360;
	}

	//	m_iData[i] = (double)AmplitudeValue *sin(dbl*i);
	nTest++;
	//}
}

void CAVMDDisplayDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TEST_TIME:
	{
		if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
		{
			if(IsCMDReceived())
				CStatus::Instance()->UpdateScrn();
			MessagePumping();
		}
	}
		break;
	case 0:
	{
		//if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
		//{
			if (CStatus::Instance()->GetReEnter())
				return;
			CStatus::Instance()->UpdateScrn();
			KillTimer(0);
		//}
	}
	break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CAVMDDisplayDlg::OnCbnSelchangeCbFftHorizonScale()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp, strFFTViewRatio;
	GetDlgItemText(IDC_CB_FFT_HOFRZON_SCALE, strTemp);
	int nFFTRatio = _ttoi(strTemp);
	if (strTemp == "Full")
	{
		GetDlgItem(IDC_SLIDER_FFT_SHOW)->EnableWindow(FALSE);
		CStatus::Instance()->m_bIsFFTFullMode = TRUE;
		CStatus::Instance()->m_iFFTShowTempStartPos = 0;
		m_Slider_FFT_Show.SetPos(0);
		nFFTRatio = 1;
	}
	else
	{
		CStatus::Instance()->m_iFFTShowRatio = nFFTRatio;
		int nPos = m_Slider_FFT_Show.GetPos();
		if (!CStatus::Instance()->CheckFFTDrawingRange(nPos))
		{
			int nNewPos = CStatus::Instance()->FFTDrawingMaxPos();
			nPos = nNewPos;
			m_Slider_FFT_Show.SetPos(nPos);
			CStatus::Instance()->m_iFFTShowTempStartPos = nPos;
		}
		CStatus::Instance()->m_bIsFFTFullMode = FALSE;
		GetDlgItem(IDC_SLIDER_FFT_SHOW)->EnableWindow(TRUE);
	}
	
	CStatus::Instance()->m_iFFTShowRatio = nFFTRatio;
	
	CStatus::Instance()->GraphUpdate();
}

LRESULT CAVMDDisplayDlg::UpdateFilterTextBox(WPARAM wParam, LPARAM lParam)
{
	if(CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
		GetDlgItem(IDC_STATIC_FILTER_STATUS)->SetWindowTextW(CStatus::Instance()->m_strFilterStatus);
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
		GetDlgItem(IDC_STATIC_FILTER_STATUS)->SetWindowTextW(CStatus::Instance()->m_strFilterStatus);
	return 0;
}

int CAVMDDisplayDlg::AppendMarkInfo(int nInputType)
{
	if (CStatus::Instance()->GetRepository()->GetFileSaveStatus() != CRepository::SS_WRITE)return -1;

	CString strAddMark = CStatus::Instance()->m_pAVMDViewDlg->GetMark();

	if (m_bReconnected)
	{
		strAddMark = _T("Reconnected");
		m_bReconnected = FALSE;
	}
	if (!strAddMark.IsEmpty())
	{
		tMarkInfo *pMarkInfo = new tMarkInfo;
		pMarkInfo->iIndex = (CStatus::Instance()->GetRepository()->m_iCSVDispIndexNo);
		pMarkInfo->strMark = strAddMark;
		CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].push_back(pMarkInfo);
	}


	return 0;
}

void CAVMDDisplayDlg::SetFlagReceived()
{
	m_bReceived = TRUE;
}

BOOL CAVMDDisplayDlg::IsCMDReceived()
{
	BOOL bReturn;
	bReturn = m_bReceived;
	m_bReceived = FALSE;
	return bReturn;
}

BOOL CAVMDDisplayDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

int CAVMDDisplayDlg::MessagePumping()
{
	static MSG msg; 
	if (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE) == TRUE) {
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}

	return 0;
}

LRESULT CAVMDDisplayDlg::UpdateTimeDomainTextBox(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_STATIC_TIME)->SetWindowTextW(CStatus::Instance()->m_strTimeDomainUnit);
	return LRESULT();
}

void CAVMDDisplayDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
#ifdef _DEBUG
	CMenu mnuPopupSubmit;
	mnuPopupSubmit.LoadMenu(IDR_TESTMENU);
	CMenu *mnuPopupMenu = mnuPopupSubmit.GetSubMenu(0);

	mnuPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
#endif
	
}

void CAVMDDisplayDlg::OnTestTestfilewrite()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStatus::Instance()->GetRepository()->TestFileWrite();
	AfxMessageBox(_T("Finish"));
}

void CAVMDDisplayDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar)
	{
		// 어떤 슬라이더인지 검사
		if (pScrollBar == (CScrollBar*)&m_Slider_FFT_Show)
		{
			// 슬라이더의 위치를 검사한다.
			if (CStatus::Instance()->m_bIsFFTFullMode)
			{
				m_Slider_FFT_Show.SetPos(0);
				return;
			}
			if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
			{
				if (CStatus::Instance()->GetReEnter())
					return;
			}
			int nPosition = m_Slider_FFT_Show.GetPos();
			if (!CStatus::Instance()->CheckFFTDrawingRange(nPosition))
			{
				int nNewPos = CStatus::Instance()->FFTDrawingMaxPos();
				nPosition = nNewPos;
				m_Slider_FFT_Show.SetPos(nPosition);
			}

			int nPos = m_Slider_FFT_Show.GetPos();
			CStatus::Instance()->m_iFFTShowTempStartPos = nPos;
			CStatus::Instance()->UpdateScrn();

			CStatus::Instance()->GraphUpdate();
		}
	}
	else
	{
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CAVMDDisplayDlg::CheckoutPacket()
{
	return m_AVMDPacket.CheckoutPacket();
}

void CAVMDDisplayDlg::OnNMReleasedcaptureSliderFftShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	//{
	if (CStatus::Instance()->m_bIsFFTFullMode)
	{
		m_Slider_FFT_Show.SetPos(0);
		return;
	}
	int nPos = m_Slider_FFT_Show.GetPos();
	if (!CStatus::Instance()->CheckFFTDrawingRange(nPos))
	{
		int nNewPos = CStatus::Instance()->FFTDrawingMaxPos();
		nPos = nNewPos;
		m_Slider_FFT_Show.SetPos(nPos);
	}
			

	CStatus::Instance()->m_iFFTShowTempStartPos = nPos;
	if (CStatus::Instance()->GetReEnter())
	{
		SetTimer(0, 200, NULL);
		return;
	}
	else
		CStatus::Instance()->UpdateScrn();
//	}
	*pResult = 0;
}

