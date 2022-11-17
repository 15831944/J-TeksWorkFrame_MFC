// HistoHandle.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "HistoHandle.h"
#include "afxdialogex.h"

// CHistoHandle 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHistoHandle, CButton)

CHistoHandle::CHistoHandle() :m_centerPos(0)
{

}

CHistoHandle::~CHistoHandle()
{
}


BEGIN_MESSAGE_MAP(CHistoHandle, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CHistoHandle 메시지 처리기입니다.


void CHistoHandle::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	// TODO:  지정된 항목을 그리는 코드를 추가합니다.
	CDC *pDC = this->GetDC();
	CDC *pParentDC=GetParent()->GetDC();
	CPoint tri[3];
	CRect ClientRect;
	GetClientRect(&ClientRect);
	tri[0].SetPoint(ClientRect.CenterPoint().x, ClientRect.top+1);
	tri[1].SetPoint(ClientRect.left+1, ClientRect.bottom-1);
	tri[2].SetPoint(ClientRect.right-1, ClientRect.bottom-1);

	pDC->Polygon(tri, 3);
	ReleaseDC(pDC);
}

//#define GHList m_pSplitLine->m_HandleList
void CHistoHandle::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (GetCapture() == this)
	{
		CRect thisRect;
		CRect rectToErase = m_HistoHDLRect;
		rectToErase.top += 1;
		rectToErase.bottom -= 1;
		GetWindowRect(&thisRect);
		GetParent()->ScreenToClient(&thisRect);
		CRect rectToMove;
		rectToMove = thisRect;
		rectToMove.OffsetRect(point.x, 0);
		GetParent()->InvalidateRect(rectToErase);
		if (rectToMove.CenterPoint().x < m_HistoHDLRect.left)return;
		if (rectToMove.CenterPoint().x > m_HistoHDLRect.right)return;
		this->MoveWindow(rectToMove);
	}
	CButton::OnMouseMove(nFlags, point);
}


void CHistoHandle::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCapture();
	CButton::OnLButtonDown(nFlags, point);
}


void CHistoHandle::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ReleaseCapture();
	GetParent()->SendMessage(WM_HISTO_UPDATE, NULL, NULL);
	CButton::OnLButtonUp(nFlags, point);
}

HBRUSH CHistoHandle::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CButton::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	DWORD dwStyle = this->GetStyle();
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


BOOL CHistoHandle::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return false;
	return CButton::OnEraseBkgnd(pDC);
}


void CHistoHandle::SetHistoHDLRect(CRect * pHistoHDLRect)
{
	m_HistoHDLRect = *pHistoHDLRect;
}


void CHistoHandle::SetPos(int iPos)
{
	CRect rectWnd;
	CRect rectPos;
	CRect rectParent;
	int vGrayStart = 0;
	int vGrayEnd = 65535;

	GetWindowRect(&rectWnd);
	rectPos = rectWnd;
	GetParent()->ScreenToClient(&rectPos);
	GetParent()->GetClientRect(rectParent);

	int iWidth = m_HistoHDLRect.Width();
	int xposStart = m_HistoHDLRect.left;

	int xposCenter;
	xposCenter = xposStart + (int)(iPos / ((vGrayEnd - vGrayStart) / ((double)iWidth)));

	int xposLeft = xposCenter - (rectWnd.Width() / 2);
	int yposTop = m_HistoHDLRect.top;
	int widthToMove = rectWnd.Width();
	int heightToMove = rectWnd.Height();

	MoveWindow(CRect(xposLeft, yposTop, xposLeft+widthToMove, yposTop+heightToMove));
}


int CHistoHandle::GetPos()
{
	CRect rectWnd;
	CRect rectPos;
	CRect rectParent;
	int vGrayStart = 0;
	int vGrayEnd = 65535;

	GetWindowRect(&rectWnd);
	rectPos = rectWnd;
	GetParent()->ScreenToClient(&rectPos);
	int xposCur = rectPos.CenterPoint().x;
	int iWidth = m_HistoHDLRect.Width();
	int xposStart = m_HistoHDLRect.left;

	double rv = (xposCur - xposStart) * ((vGrayEnd - vGrayStart) / ((double)iWidth));

	return (int)rv;
}
