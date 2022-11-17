
// DemoPCDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DemoPC.h"
#include "DemoPCDlg.h"
#include "afxdialogex.h"
#include <atlbase.h>
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CDemoPCDlg 대화 상자



CDemoPCDlg::CDemoPCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMOPC_DIALOG, pParent)
	, m_AVMDPacket(m_serial)
	, m_editXValue(_T(""))
	, m_editYValue(_T(""))
	, m_editZValue(_T(""))
	, m_strTemperature(_T(""))
	, m_strBattery(_T(""))
	, m_strNum(_T(""))
	, m_strCalDate(_T(""))
	, m_strSerialNo(_T(""))
	, m_strState(_T(""))
	, m_strTime(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CDemoPCDlg::UpdateAvailableComPort()
{
	//m_wndComPorts.ResetContent();

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

void CDemoPCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_editXValue);
	DDX_Text(pDX, IDC_EDIT_Y, m_editYValue);
	DDX_Text(pDX, IDC_EDIT_Z, m_editZValue);
	DDX_Control(pDX, IDC_COMPORTS, m_wndComPorts);
	DDX_Text(pDX, IDC_TEMPERATURE, m_strTemperature);
	DDX_Text(pDX, IDC_BATTERY, m_strBattery);
	DDX_Text(pDX, IDC_NUM, m_strNum);
	DDX_Text(pDX, IDC_CAL_DATE, m_strCalDate);
	DDX_Text(pDX, IDC_SERIAL_NO, m_strSerialNo);
	DDX_Text(pDX, IDC_STATE, m_strState);
	DDX_Text(pDX, IDC_TIME, m_strTime);
}

BEGIN_MESSAGE_MAP(CDemoPCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_CBN_SELENDOK(IDC_COMPORTS, &CDemoPCDlg::OnCbnSelendokComports)
	ON_BN_CLICKED(IDC_START, &CDemoPCDlg::OnBnClickedStart)
	ON_WM_SERIAL(OnSerialMsg)		// 필수
END_MESSAGE_MAP()


// CDemoPCDlg 메시지 처리기

BOOL CDemoPCDlg::OnInitDialog()
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

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	UpdateAvailableComPort();

	OnCbnSelendokComports();		// 필수 Initial COM port open.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDemoPCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDemoPCDlg::OnPaint()
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
HCURSOR CDemoPCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDemoPCDlg::OnCbnSelendokComports()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	// 필수 -
	if (m_serial.Open(pszComPort, this) == ERROR_SUCCESS)
	{
		VERIFY(m_serial.Setup(CSerial::EBaud115200, CSerial::EData8, CSerial::EParNone, CSerial::EStop1) == ERROR_SUCCESS);
		VERIFY(m_serial.SetMask(CSerial::EEventBreak | CSerial::EEventError | CSerial::EEventRecv) == ERROR_SUCCESS);
	}
	// - 필수

	if (!m_serial.IsOpen())
	{
		AfxMessageBox(_T("Unable to open COM-port"), MB_ICONSTOP | MB_OK);
	}
}

LRESULT CDemoPCDlg::OnSerialMsg(WPARAM wParam, LPARAM lParam)	// 필수
{
	USES_CONVERSION;

	CAGMDPacket::EPacketType eType = m_AVMDPacket.CheckoutPacket();
	
	switch (eType)
	{
	case CAGMDPacket::Measure:
	{
		DVC_PACKET_MEAINFO* pPacket = (DVC_PACKET_MEAINFO*)m_AVMDPacket.GetPacket();

		float fTemperature = (float)pPacket->byTemperatue / 100.F;
	
		m_editXValue = _T("");
		m_editYValue = _T("");
		m_editZValue = _T("");
		for (int i = 0; i < 16; i++)
		{
			CString strX, strY, strZ;

			//int iXvalue = pPacket->accInfo[i].x;
			//int iYvalue = pPacket->accInfo[i].y;
			//int iZvalue = pPacket->accInfo[i].z;

			//strX.Format(_T("%d"), iXvalue);
			//strY.Format(_T("%d"), iYvalue);
			//strZ.Format(_T("%d"), iZvalue);
			
			m_editXValue += strX;
			m_editXValue += _T("\r\n");
		
			m_editYValue += strY;
			m_editYValue += _T("\r\n");

			m_editZValue += strZ;
			m_editZValue += _T("\r\n");
			
		}
		
		m_strTemperature.Format(_T("%.2f degree"), fTemperature);
		m_strBattery.Format(_T("%u %%"), pPacket->byBattery);
		UpdateData(FALSE); 
		break;
	}
	

	default:
		break;
	}

	return 0;
}


void CDemoPCDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_serial.IsOpen())
		m_serial.Close();

	for (int i = 0; i < m_wndComPorts.GetCount(); i++)
	{
		delete[](TCHAR*)m_wndComPorts.GetItemData(i);
	}

}


void CDemoPCDlg::OnBnClickedStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	//m_AVMDPacket.SendSensorInfoCommand();
	//Sleep(100);
	m_AVMDPacket.SendMeasureCommand();
}
