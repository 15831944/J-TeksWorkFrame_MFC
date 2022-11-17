// SCButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "SCButton.h"


// CSCButton

IMPLEMENT_DYNAMIC(CSCButton, CButton)

CSCButton::CSCButton()
	: m_bStatHover(FALSE)
{

}

CSCButton::~CSCButton()
{
}


BEGIN_MESSAGE_MAP(CSCButton, CButton)
	ON_WM_ERASEBKGND()
//	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
//	ON_NOTIFY_REFLECT(BCN_HOTITEMCHANGE, &CSCButton::OnBnHotItemChange)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CSCButton 메시지 처리기입니다.




BOOL CSCButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rectClient;
	GetClientRect(&rectClient);
	pDC->FillSolidRect(&rectClient, RGB(0, 255, 255));
	return CButton::OnEraseBkgnd(pDC);
//	return TRUE;
}

void CSCButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  지정된 항목을 그리는 코드를 추가합니다.
	CDC DC;
	RECT rect;
	DC.Attach(lpDrawItemStruct->hDC);   //  버튼의 DC 구하기
	GetClientRect(&rect);     // 버튼 영역 구하기    
	DC.SetBkMode(TRANSPARENT); //배경 투명
	DC.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0)); // 버튼의 외각선 그리기

	UINT state = lpDrawItemStruct->itemState;  //  버튼 상태 구하기
	if ((state & ODS_SELECTED))
	{
		DC.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
	}
	else
	{
		DC.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
	}
	if (state & ODS_DISABLED)
	{
		DC.FillSolidRect(&rect, RGB(230, 230, 230));
		//		DC.SetBkColor(RGB(92, 126, 173)); // TEXT 의 백그라운드 색상
		DC.SetTextColor(RGB(255, 255, 255));  // TEXT 색상
	}
	else
	{
		if (m_bStatHover)
		{
			DC.FillSolidRect(&rect, RGB(92, 126, 173));
			//		DC.SetBkColor(RGB(92, 126, 173)); // TEXT 의 백그라운드 색상
			DC.SetTextColor(RGB(255, 0, 0));  // TEXT 색상
		}
		else
		{
			DC.FillSolidRect(&rect, RGB(90, 107, 135));
			//		DC.SetBkColor(RGB(0, 255, 0)); // TEXT 의 백그라운드 색상
			DC.SetTextColor(RGB(255, 255, 255));  // TEXT 색상
		}
	}

	CString strText;
	GetWindowText(strText);
	DC.DrawText(strText, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);// 버튼에 TEXT 넣기
	DC.Detach();  // Button DC 풀어 주기
}



void CSCButton::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bStatHover = FALSE;
	Invalidate();
	CButton::OnMouseLeave();
}


void CSCButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bStatHover = TRUE;
	DRAWITEMSTRUCT drawItem;
	DrawItem(&drawItem);
	CButton::OnMouseHover(nFlags, point);
}

void CSCButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_bStatHover)
	{ 
	}else
	{
		m_bStatHover = TRUE;
		m_stTrackMouse.cbSize = sizeof(TRACKMOUSEEVENT);
		m_stTrackMouse.dwFlags = TME_LEAVE;
		m_stTrackMouse.dwHoverTime = HOVER_DEFAULT;
		m_stTrackMouse.hwndTrack = GetSafeHwnd();
		TrackMouseEvent(&m_stTrackMouse);
		Invalidate();
		//DRAWITEMSTRUCT drawItem;
		//DrawItem(&drawItem);
	}
	CButton::OnMouseMove(nFlags, point);
}


int CSCButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

