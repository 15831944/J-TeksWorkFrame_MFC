// Cal1Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "Cal1Dlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "MonitorDlg.h"


// CCal1Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCal1Dlg, CDialogEx)

CCal1Dlg::CCal1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAL1, pParent)
	, m_strCalInfo(_T(""))
	, m_strJigGap(_T(""))
	, m_bAbleToNext(FALSE)
{

}

CCal1Dlg::~CCal1Dlg()
{
}

void CCal1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CALINFO, m_strCalInfo);
	DDX_Text(pDX, IDC_EDT_JIGGAP, m_strJigGap);
}


BEGIN_MESSAGE_MAP(CCal1Dlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCal1Dlg 메시지 처리기입니다.

#define REF_TIME 2

BOOL CCal1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCal1Dlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CStatus::Instance()->m_CalData.bRcvCalInfo = FALSE;
	m_bAbleToNext = FALSE;
	CStatus::Instance()->m_pMonitorDlg->m_AGMDPacket.SendCalInfoCmd();
	SetTimer(REF_TIME, 500, NULL);
}


void CCal1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case REF_TIME:
		if (IsGetRcvCalData())
		{
			DispValue();
			m_bAbleToNext = TRUE;
		}
		UpdateData(FALSE);
		KillTimer(REF_TIME);
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CCal1Dlg::IsGetRcvCalData()
{
	return CStatus::Instance()->m_CalData.bRcvCalInfo;
}


void CCal1Dlg::DispValue()
{
	int Gap1, Gap2, Gap3;
	int JigGap = CStatus::Instance()->m_CalData.uJigGap;
	Gap1 = CStatus::Instance()->m_CalData.iGap1;
	Gap2 = CStatus::Instance()->m_CalData.iGap2;
	Gap3 = CStatus::Instance()->m_CalData.iGap3;
	m_strCalInfo.Format(_T("Gap1:%dum, Gap2:%dum, Gap3:%dum"), Gap1, Gap2, Gap3);
	m_strJigGap.Format(_T("%d"), JigGap);
}


int CCal1Dlg::GetJigGap()
{
	return GetDlgItemInt(IDC_EDT_JIGGAP);
}


BOOL CCal1Dlg::PreTranslateMessage(MSG* pMsg)
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
