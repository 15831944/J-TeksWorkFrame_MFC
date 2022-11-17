
// DemoDeviceDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DemoDevice.h"
#include "DemoDeviceDlg.h"
#include "afxdialogex.h"

#include <atlbase.h>
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDT_ONE_SEC		0

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDemoDeviceDlg 대화 상자



CDemoDeviceDlg::CDemoDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMODEVICE_DIALOG, pParent)
	, m_APMDPacket(m_serial)
	, m_dwCount01(1)
	, m_dwCount03(3)
	, m_dwCount05(5)
	, m_sTemperature(0)
	, m_dwPressure(0)
	, m_byBattery(0)
	, m_byHumidity(0)
	, m_strCalDate(_T("160404"))
	, m_strSerialNo(_T("3P70016"))
	, m_bStart(FALSE)
	, m_bTimerOn(FALSE)
	, m_wSecond(0)
	, m_wPurgeTime(0)
	, m_wMeasureTime(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoDeviceDlg::UpdateAvailableComPort()
{
	m_wndComPorts.ResetContent();

	HKEY hKey;

	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szName[100];
	TCHAR szData[20];

	DWORD dwIndex = 0;
	DWORD dwSize = 100;
	DWORD dwSize2 = 20;
	DWORD dwType = REG_SZ;

	while (ERROR_SUCCESS == RegEnumValue(hKey, dwIndex, szName, &dwSize, NULL, NULL, NULL, NULL))
	{
		if (RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2) != ERROR_SUCCESS)
		{
			break;
		}

		TCHAR* pszComPort = new TCHAR[sizeof(szData) + 4];

		_stprintf_s(pszComPort, sizeof(szData) + 4, _T("\\\\.\\%s"), szData);
		if (CSerial::CheckPort(pszComPort) == CSerial::EPortAvailable)
		{
			CString strItem;
			strItem.Format(_T("%s (%s)"), szData, szName);

			int nIndex = m_wndComPorts.AddString(strItem);
			m_wndComPorts.SetItemData(nIndex, (DWORD_PTR)pszComPort);
		}
		else
		{
			delete[] pszComPort;
		}

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 100;
		dwSize2 = 20;

		dwIndex++;
	}

	RegCloseKey(hKey);

	if (m_wndComPorts.GetCount() > 0)
	{
		m_wndComPorts.SetCurSel(0);
	}
}

void CDemoDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COUNT_01, m_dwCount01);
	DDX_Text(pDX, IDC_COUNT_03, m_dwCount03);
	DDX_Text(pDX, IDC_COUNT_05, m_dwCount05);
	DDX_Text(pDX, IDC_TEMPERATURE, m_sTemperature);
	DDX_Text(pDX, IDC_PRESSURE, m_dwPressure);
	DDX_Text(pDX, IDC_BATTERY, m_byBattery);
	DDX_Text(pDX, IDC_HUMIDITY, m_byHumidity);
	DDX_Control(pDX, IDC_SEC, m_wndSecond);
	DDX_Control(pDX, IDC_STATE, m_wndState);
	DDX_Text(pDX, IDC_CAL_DATE, m_strCalDate);
	DDV_MaxChars(pDX, m_strCalDate, 6);
	DDX_Text(pDX, IDC_SERIAL_NO, m_strSerialNo);
	DDV_MaxChars(pDX, m_strCalDate, 8);
	DDX_Control(pDX, IDC_COMPORTS, m_wndComPorts);
	DDX_Control(pDX, IDC_START, m_wndStart);
}

BEGIN_MESSAGE_MAP(CDemoDeviceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_CBN_SELENDOK(IDC_COMPORTS, &CDemoDeviceDlg::OnCbnSelendokComports)
	ON_WM_SERIAL(OnSerialMsg)
	ON_BN_CLICKED(IDC_START, &CDemoDeviceDlg::OnBnClickedStart)
END_MESSAGE_MAP()


// CDemoDeviceDlg 메시지 처리기

BOOL CDemoDeviceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_COUNT_01))->SetRange32(0, 65535);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_COUNT_03))->SetRange32(0, 65535);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_COUNT_05))->SetRange32(0, 65535);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TEMPERATURE))->SetRange32(SHRT_MIN, SHRT_MAX);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PRESSURE))->SetRange32(0, 65535);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_BATTERY))->SetRange32(0, 255);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_HUMIDITY))->SetRange32(0, 255);

	UpdateAvailableComPort();

	OnCbnSelendokComports();		// Initial COM port open.

	//int nComPort = CSerial::FindFirstAvailablePort();
	//if (nComPort > -1)
	//{
	//	CString strComPort;
	//	strComPort.Format(_T("\\\\.\\COM%d"), nComPort + 1);
	//	if (m_serial.Open(strComPort, this) == ERROR_SUCCESS)
	//	{
	//		VERIFY(m_serial.Setup(CSerial::EBaud9600, CSerial::EData8, CSerial::EParNone, CSerial::EStop1) == ERROR_SUCCESS);
	//		VERIFY(m_serial.SetMask(CSerial::EEventBreak | CSerial::EEventError | CSerial::EEventRecv) == ERROR_SUCCESS);
	//	}
	//}

	//if (!m_serial.IsOpen())
	//{
	//	AfxMessageBox(_T("Unable to open COM-port"), MB_ICONSTOP | MB_OK);

	//	PostMessage(WM_CLOSE);

	//	return TRUE;
	//}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDemoDeviceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDemoDeviceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDemoDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDemoDeviceDlg::OnDestroy()
{
	if (m_serial.IsOpen())
		m_serial.Close();

	for (int i = 0; i < m_wndComPorts.GetCount(); i++)
	{
		delete[](TCHAR*)m_wndComPorts.GetItemData(i);
	}

	CDialogEx::OnDestroy();
}

void CDemoDeviceDlg::OnTimer(UINT_PTR nIDEvent)
{
	static DWORD dwCount = 0;
	static int nMTCount = 0;

	if (nIDEvent == IDT_ONE_SEC)
	{
		dwCount++;

		CString strSec;
		strSec.Format(_T("%lu"), m_wSecond);

		m_wndSecond.SetWindowText(strSec);

		if (m_serial.IsOpen())
		{
			UpdateData(TRUE);

			if (m_bStart)
			{
				m_wSecond++;

				if (m_wPurgeTime > 0)
				{
					m_wPurgeTime--;
					nMTCount = 0;

					m_APMDPacket.SendMeasureData(0,
						(WORD)m_dwCount01, (WORD)m_dwCount03, (WORD)m_dwCount05,
						m_sTemperature, (WORD)m_dwPressure, m_byBattery, m_byHumidity);
				}
				else if (m_wMeasureTime > 0)
				{
					m_wMeasureTime--;
					nMTCount++;

					m_APMDPacket.SendMeasureData(nMTCount,
						(WORD)m_dwCount01, (WORD)m_dwCount03, (WORD)m_dwCount05,
						m_sTemperature, (WORD)m_dwPressure, m_byBattery, m_byHumidity);
				}
			}
			else
			{
				USES_CONVERSION;

				m_wndSecond.SetWindowText(_T("0"));

				if (!(dwCount % 4))				// Measure
				{
					m_APMDPacket.SendMeasureData(0, 0, 0, 0,
						m_sTemperature, (WORD)m_dwPressure, m_byBattery, m_byHumidity);
				}
				else if (!((dwCount + 2) % 4))	// State
				{
					m_APMDPacket.SendStateData(CT2A(m_strCalDate), CT2A(m_strSerialNo));
				}
			}

			CString strState;

			if (m_wPurgeTime > 0)
			{
				strState.Format(_T("PURGE %u sec"), m_wPurgeTime);
			}
			else if (m_wMeasureTime > 0)
			{
				strState.Format(_T("MEASURE %u sec"), m_wMeasureTime);
			}
			else
			{
				strState.Format(_T("STOP"), m_wPurgeTime);
				m_bStart = FALSE;
			}

			m_wndState.SetWindowText(strState);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDemoDeviceDlg::OnCbnSelendokComports()
{
	int nIndex = m_wndComPorts.GetCurSel();
	if (nIndex == CB_ERR)
	{
		return;
	}

	if (m_serial.IsOpen())
	{
		m_serial.Close();
	}

	TCHAR* pszComPort = (TCHAR*)m_wndComPorts.GetItemData(nIndex);

	if (m_serial.Open(pszComPort, this) == ERROR_SUCCESS)
	{
		VERIFY(m_serial.Setup(CSerial::EBaud9600, CSerial::EData8, CSerial::EParNone, CSerial::EStop1) == ERROR_SUCCESS);
		VERIFY(m_serial.SetMask(CSerial::EEventBreak | CSerial::EEventError | CSerial::EEventRecv) == ERROR_SUCCESS);
	}

	if (!m_serial.IsOpen())
	{
		AfxMessageBox(_T("Unable to open COM-port"), MB_ICONSTOP | MB_OK);
	}
}

void CDemoDeviceDlg::OnBnClickedStart()
{
	m_bTimerOn = !m_bTimerOn;

	if (m_bTimerOn)
	{
		SetTimer(IDT_ONE_SEC, 1000, NULL);
		m_wndStart.SetWindowText(_T("중지"));
	}
	else
	{
		KillTimer(IDT_ONE_SEC);
		m_wndStart.SetWindowText(_T("시작"));
	}
}

LRESULT CDemoDeviceDlg::OnSerialMsg(WPARAM wParam, LPARAM lParam)
{
	CAPMDPacket::EPacketType eType = m_APMDPacket.CheckoutPacket();

	switch (eType)
	{
	case CAPMDPacket::Start:
		{
			PC_PACKET_CTRL* pPacket = (PC_PACKET_CTRL*)m_APMDPacket.GetPacket();

			m_wSecond = 0;
			m_bStart = TRUE;

			m_wPurgeTime = pPacket->wPurgeTime;
			m_wMeasureTime = pPacket->wMeasureTime;
		}
		break;

	case CAPMDPacket::Stop:
		{
			PC_PACKET_CTRL* pPacket = (PC_PACKET_CTRL*)m_APMDPacket.GetPacket();

			m_bStart = FALSE;

			m_wPurgeTime = 0;
			m_wMeasureTime = 0;
	}
		break;

	default:
		break;
	}

	return 0;
}
