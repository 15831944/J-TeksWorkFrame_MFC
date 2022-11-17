// MarkToolTip.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "MarkToolTip.h"
#include "APMDMessages.h"


// CMarkToolTip

IMPLEMENT_DYNAMIC(CMarkToolTip, CWnd)

CMarkToolTip::CMarkToolTip()
	: m_nTimeForTip(400)
	, m_IsInRect(false)
	, m_pWndReg(NULL)
	, m_IsToolTip(false)
	, m_curPoint(0,0)
	, m_poslParam(0)
{

}

CMarkToolTip::~CMarkToolTip()
{
}


BEGIN_MESSAGE_MAP(CMarkToolTip, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CMarkToolTip 메시지 처리기입니다.




BOOL CMarkToolTip::RelayEvent(MSG* pMsg)
{
	static CPoint PrevPoint(0, 0);
	switch (pMsg->message)
	{
	case WM_MOUSELEAVE:
		KillTimer(710720);
		if (m_IsToolTip)GetParent()->Invalidate();
		OutputDebugString(_T("Leave\n"));
		m_IsToolTip = false;
		break;
	case WM_MOUSEMOVE:
		if (m_IsToolTip)GetParent()->Invalidate();
		m_IsToolTip = false;
		CPoint curPoint(GET_X_LPARAM(pMsg->lParam),	GET_Y_LPARAM(pMsg->lParam));
		m_poslParam = pMsg->lParam;
		m_curPoint = curPoint;
		if(m_rectScanArea.PtInRect(curPoint))
		{
			static int incCount = 0;
			CString strTest;
			strTest.Format(_T("SetTimer%d\n"), incCount++);
			OutputDebugString(strTest.GetBuffer());
			SetTimer(710720, m_nTimeForTip, NULL);
			m_IsInRect = true;
		}
		else
		{
			m_IsInRect = false;
		}
		PrevPoint = curPoint;
		break;
	}
	return 0;
}


void CMarkToolTip::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 710720:
		if (m_pWndReg)m_pWndReg->SendMessage(WM_MARK_TOOLTIPTIME, NULL, m_poslParam);
		m_IsToolTip = true;
		break;
	}
	CWnd::OnTimer(nIDEvent);
}


int CMarkToolTip::Init(CWnd *pWnd, int nTimeForTip)
{
	m_nTimeForTip = nTimeForTip;
	m_pWndReg = pWnd;
	m_IsToolTip = false;
	return 0;
}
