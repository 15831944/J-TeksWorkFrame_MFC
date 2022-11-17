// StatusDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "StatusDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "StatusCompareDlg.h"


// CStatusDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStatusDlg, CDialogEx)

CStatusDlg::CStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STATUS, pParent)
	, m_uiP01Counter(0)
	, m_uiP03Counter(0)
	, m_uiP05Counter(0)
	, m_strAPMDTemp(_T(""))
	, m_strPressure(_T(""))
	, m_pRepository(NULL)
	, m_strHumidity(_T(""))
	, m_pStatusCompareDlg(NULL)
{

}

CStatusDlg::~CStatusDlg()
{
}

void CStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAT_LASER, m_LaserIndicator);
	DDX_Control(pDX, IDC_STAT_FAN, m_FanIndicator);
	DDX_Control(pDX, IDC_STAT_BATTERY, m_BatteryState);
	DDX_Control(pDX, IDC_STAT_HUMI, m_HumidityState);
	DDX_Text(pDX, IDC_P01COUNT, m_uiP01Counter);
	DDX_Text(pDX, IDC_P03COUNT, m_uiP03Counter);
	DDX_Text(pDX, IDC_P05COUNT, m_uiP05Counter);
	DDX_Text(pDX, IDC_APMDTEMP, m_strAPMDTemp);
	DDX_Text(pDX, IDC_PRESSURE, m_strPressure);
	DDX_Control(pDX, IDC_LST_STAT, m_lstStatChart);
	DDX_Text(pDX, IDC_EDT_WARN, m_strWarnDisp);
	DDX_Control(pDX, IDC_EDT_WARN, m_cWarnDisplay);
	DDX_Text(pDX, IDC_HUMI_TEXT, m_strHumidity);
	DDX_Control(pDX, IDC_P01COUNT, m_cP01Counter);
	DDX_Control(pDX, IDC_P03COUNT, m_cP03Counter);
	DDX_Control(pDX, IDC_P05COUNT, m_cP05Counter);
	DDX_Control(pDX, IDC_STT_LABELP01, m_cLabelP01);
	DDX_Control(pDX, IDC_STT_LABELP03, m_cLabelP03);
	DDX_Control(pDX, IDC_STT_LABELP05, m_cLabelP05);
}


BEGIN_MESSAGE_MAP(CStatusDlg, CDialogEx)
//	ON_WM_CTLCOLOR()
ON_WM_TIMER()
END_MESSAGE_MAP()


// CStatusDlg 메시지 처리기입니다.


BOOL CStatusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_LaserIndicator.Set(false);
	m_FanIndicator.Set(false);
	m_BatteryState.SetRange32(0, 100);
	m_BatteryState.SetPos(20);
	m_HumidityState.SetRange32(0, 100);
	m_HumidityState.SetPos(50);

	LOGFONT log;
	m_cP01Counter.GetFont()->GetLogFont(&log);
	log.lfHeight = 25;
	m_BigFont.CreateFontIndirect(&log);
	m_cP01Counter.SetFont(&m_BigFont);
	m_cP03Counter.SetFont(&m_BigFont);
	m_cP05Counter.SetFont(&m_BigFont);
	m_cLabelP01.SetFont(&m_BigFont);
	m_cLabelP03.SetFont(&m_BigFont);
	m_cLabelP05.SetFont(&m_BigFont);

	m_lstStatChart.SetExtendedStyle(m_lstStatChart.GetExStyle() || LVS_EX_GRIDLINES);
	m_lstStatChart.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 50);
	m_lstStatChart.InsertColumn(1, _T("Accumulated"), LVCFMT_RIGHT, 100);
	m_lstStatChart.InsertItem(0, _T("0.1um"));
	m_lstStatChart.InsertItem(1, _T("0.3um"));
	m_lstStatChart.InsertItem(2, _T("0.5um"));
	m_lstStatChart.InsertItem(3, _T("Sum"));

	UpdateStatisticChart(0, 0, 0);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CStatusDlg::SetRespository(CRepository * pRepository)
{
	m_pRepository = pRepository;
}


int CStatusDlg::UpdateCompareChart(tAPMDDispData *pAcqData)
{
	return 0;
}

void CStatusDlg::UpdateMainChart(tAPMDDispData * pDispData)
{
	UpdateStatisticChart(pDispData->nAcc01, pDispData->nAcc03, pDispData->nAcc05);
}

void CStatusDlg::UpdateStatisticChart(int iAcc01, int iAcc03, int iAcc05)
{
	CString strAcc01, strAcc03, strAcc05, strAccSum;

	strAcc01.Format(_T("%d"), iAcc01);
	strAcc03.Format(_T("%d"), iAcc03);
	strAcc05.Format(_T("%d"), iAcc05);
	strAccSum.Format(_T("%d"), iAcc01+iAcc03+iAcc05);

	m_lstStatChart.SetItem(0, 1, LVIF_TEXT,	strAcc01, NULL, NULL, NULL, NULL);
	m_lstStatChart.SetItem(1, 1, LVIF_TEXT,	strAcc03, NULL, NULL, NULL, NULL);
	m_lstStatChart.SetItem(2, 1, LVIF_TEXT, strAcc05, NULL, NULL, NULL, NULL);
	m_lstStatChart.SetItem(3, 1, LVIF_TEXT,	strAccSum, NULL, NULL, NULL, NULL);
}


int CStatusDlg::CheckWarnDisp(bool bUseWarning, bool bIsError, CString strMsg)
{
	m_strWarnDisp = strMsg;
	if (!bUseWarning)
	{
		m_cWarnDisplay.SetTextBackGround(RGB(128, 128, 128));
		m_cWarnDisplay.SetBackGround(RGB(128, 128, 128));
		m_cWarnDisplay.SetForeGround(RGB(0, 0, 0));
	}
	else
	{
		if(!bIsError)
		{ 
			m_cWarnDisplay.SetTextBackGround(RGB(0, 255, 0));
			m_cWarnDisplay.SetBackGround(RGB(0, 255, 0));
			m_cWarnDisplay.SetForeGround(RGB(0, 0, 0));
		}else
		{ 
			m_cWarnDisplay.SetTextBackGround(RGB(255, 0, 0));
			m_cWarnDisplay.SetBackGround(RGB(255, 0, 0));
			m_cWarnDisplay.SetForeGround(RGB(0, 255, 0));
			SetTimer(1000, 3000, NULL);
		}
	}
	UpdateData(FALSE);
	return 0;
}



void CStatusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
	switch (nIDEvent)
	{
	case 1000:
		if (CStatus::Instance()->m_iUseWarning)
		{

		}
		else
		{
			m_cWarnDisplay.SetTextBackGround(RGB(128, 128, 128));
			m_cWarnDisplay.SetBackGround(RGB(128, 128, 128));
			m_cWarnDisplay.SetForeGround(RGB(0, 0, 0));
			KillTimer(1000);
		}
		break;
	}
}


int CStatusDlg::NormalDisplay(CString strMsg)
{
	return 0;
}


BOOL CStatusDlg::PreTranslateMessage(MSG* pMsg)
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



int CStatusDlg::SetDispData(tAPMDDispData & dispMain, tAPMDDispData & dispCompare)
{
	if (!m_pStatusCompareDlg)return -1;
	if (CStatus::Instance()->IsCompareMode())
	{
		m_pStatusCompareDlg->SetDispData(dispMain, dispCompare);
		return -1;
	}
	UpdateMainChart(&dispMain);

	m_uiP01Counter = dispMain.iParticle01;
	m_uiP03Counter = dispMain.iParticle03;
	m_uiP05Counter = dispMain.iParticle05;
	m_strAPMDTemp.Format(_T("%.2f℃"), dispMain.dblTemp);
	m_strPressure.Format(_T("%.1f mbar"), dispMain.dblPressure);
	m_BatteryState.SetPos(dispMain.iBattery);
	m_strHumidity.Format(_T("%d%%"), (int)(dispMain.iHumi));
	if (dispMain.iHumi <= 100)
	{
		m_HumidityState.SetPos(dispMain.iHumi);
	}
	else
	{
		m_HumidityState.SetBarColor(RGB(200, 200, 200));
	}

	UpdateData(FALSE);
	return 0;
}
