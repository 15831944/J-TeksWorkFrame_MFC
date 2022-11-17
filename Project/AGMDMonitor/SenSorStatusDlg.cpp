// SenSorStatusDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "SenSorStatusDlg.h"
#include "afxdialogex.h"
#include "Status.h"


// CSenSorStatusDlg 대화 상자입니다.
#define TID_CNTCHECK 1
#define TIME_CNTCHECK 1500
IMPLEMENT_DYNAMIC(CSenSorStatusDlg, CDialogEx)

CSenSorStatusDlg::CSenSorStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENSORSTATUSDLG, pParent)
{
	CStatus::Instance()->SetSensorStatusDlg(this);
}

CSenSorStatusDlg::~CSenSorStatusDlg()
{
}

void CSenSorStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAT_CONNECT, m_cCntStat);
	DDX_Control(pDX, IDC_STATIC_TEMPERATURE, m_cTempStat);
	DDX_Control(pDX, IDC_PROGRESS_BATTERY, m_cBatt);
	DDX_Control(pDX, IDC_BATTNUM, m_cBattNUM);
}


BEGIN_MESSAGE_MAP(CSenSorStatusDlg, CDialogEx)
	ON_MESSAGE(WM_UIUPDATE, &CSenSorStatusDlg::OnReceiveUpdateUI)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSenSorStatusDlg 메시지 처리기입니다.


BOOL CSenSorStatusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cCntStat.Set(FALSE);
	SetTimer(TID_CNTCHECK, TIME_CNTCHECK, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

LRESULT CSenSorStatusDlg::OnReceiveUpdateUI(WPARAM wParam, LPARAM lParam)
{
	SetSensorState((int)((signed char)CStatus::Instance()->m_curMeasureData.byTemperature), CStatus::Instance()->m_curMeasureData.byBattery);
	return LRESULT();
}
int CSenSorStatusDlg::SetSensorState(int iTemp, int nBatt)
{
	CString strTemp;
	m_cCntStat.Set(TRUE);
	SetTimer(TID_CNTCHECK, TIME_CNTCHECK, NULL);
	strTemp.Format(_T("%d"), iTemp);
	m_cTempStat.SetWindowTextW(strTemp.GetBuffer());
	m_cBatt.SetPos(nBatt);
	CString strBattNum;
	strBattNum.Format(_T("%d%%"), nBatt);
	m_cBattNUM.SetWindowTextW(strBattNum.GetBuffer());
	return 0;
}


void CSenSorStatusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case TID_CNTCHECK:
			m_cCntStat.Set(FALSE);
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CSenSorStatusDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN))
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
