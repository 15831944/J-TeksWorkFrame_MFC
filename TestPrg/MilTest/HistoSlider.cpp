// HistoSlider.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "HistoSlider.h"
#include <math.h>


#define HISTOSLIDER_CLASSNAME	_T("HistoSlider")

const UINT RANGE_CHANGED = 
::RegisterWindowMessage(_T("RANGE_CHANGED{2E7439A4-12DB-42a2-BB95-3462B3EC39CE}CHistoSlider"));

// CHistoSlider

IMPLEMENT_DYNAMIC(CHistoSlider, CWnd)

CHistoSlider::CHistoSlider()
{
	RegisterWindowClass();
	m_dMin = 0.0;
	m_dMax = 1.0;
	m_dLeft = 0.0;
	m_dRight = 1.0;
	m_nArrowWidth = 10;
	m_bTracking = FALSE;
}

CHistoSlider::~CHistoSlider()
{
}

void CHistoSlider::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext)
{
	CWnd::Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CHistoSlider::RegisterWindowClass(void)
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if(!(::GetClassInfo(hInst, HISTOSLIDER_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = NULL; // No Background brush (gives flicker)
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = HISTOSLIDER_CLASSNAME;

		if(!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

void CHistoSlider::SetMinMax(int nMin, int nMax)
{
	if(nMin > nMax)
	{
		int nTemp = nMax;
		nMax = nMin;
		nMin = nTemp;
	}

	m_dMin = nMin;
	m_dMax = nMax;
	Normalize();
	Invalidate();
}

void CHistoSlider::SetRange(int nLeft, int nRight)
{
	if(nLeft > nRight)
	{
		int nTemp = nRight;
		nRight = nLeft;
		nLeft = nTemp;
	}

	m_dLeft = nLeft;
	m_dRight = nRight;
	Normalize();
	Invalidate();
}

void CHistoSlider::Normalize(void)
{
	if(m_dLeft < m_dMin)
	{
		m_dLeft = m_dMin;
		::SendMessage(GetParent()->GetSafeHwnd(), RANGE_CHANGED, HS_LEFTCHANGED, 0);
		if (m_dRight < m_dLeft)
		{
			m_dRight = m_dLeft;
			::SendMessage(GetParent()->GetSafeHwnd(), RANGE_CHANGED, HS_RIGHTCHANGED, 0);
		}
	}
	if(m_dRight > m_dMax)
	{
		m_dRight = m_dMax;
		::SendMessage(GetParent()->GetSafeHwnd(), RANGE_CHANGED, HS_RIGHTCHANGED, 0);
		if (m_dLeft > m_dRight)
		{
			m_dLeft = m_dRight;
			::SendMessage(GetParent()->GetSafeHwnd(), RANGE_CHANGED, HS_LEFTCHANGED, 0);
		}
	}
}


BEGIN_MESSAGE_MAP(CHistoSlider, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CHistoSlider 메시지 처리기입니다.


void CHistoSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TRACE("Down Point %d, %d\n", point.x, point.y);
	SetFocus();
	Invalidate();

	if(!m_bTracking)
	{
		// Hit Testing into Rects.
		// Left, Middle or Right?
		CRect rect;
		GetClientRect(&rect);
		CRect rcMiddleRect (0,0,0,0);
		rcMiddleRect = CRect(m_rcRectLeft.right + 1, 0, m_rcRectRight.left - 1, rect.bottom);

		if(m_rcRectLeft.PtInRect(point))
		{
			m_bTracking = TRUE;
			m_TrackMode = TRACK_LEFT;
			m_ptClickOffset = point - m_rcRectLeft.CenterPoint();
		}
		else if(m_rcRectRight.PtInRect(point))
		{
			m_bTracking = TRUE;
			m_TrackMode = TRACK_RIGHT;
			m_ptClickOffset = point - m_rcRectRight.CenterPoint();
		}
		else if(rcMiddleRect.PtInRect(point))
		{
			m_bTracking = TRUE;
			m_TrackMode = TRACK_MIDDLE;
			m_ptClickOffset = point - rcMiddleRect.CenterPoint();
		}
		if (m_bTracking)
			SetCapture();
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CHistoSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_bTracking)
	{
		m_bTracking = FALSE;
		::ReleaseCapture();
		Invalidate();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CHistoSlider::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_bTracking)
	{
		int nx = 0;
		nx = point.x - m_ptClickOffset.x;
		double dOldLeft = m_dLeft;
		double dOldRight = m_dRight;

		//CRect rect;
		WPARAM dwChanged = 0;
		switch (m_TrackMode)
		{
		case TRACK_LEFT:
			{
				m_dLeft = (double)(nx - m_nArrowWidth / 2) / m_dx * (m_dMax - m_dMin) + m_dMin; 

				if(m_dLeft >= m_dRight) m_dLeft = m_dRight;
				if(m_dLeft <= m_dMin) m_dLeft = m_dMin;
				if(dOldLeft != m_dLeft) dwChanged = HS_LEFTCHANGED;

				//rect = m_rcRectLeft;
			}
			break;

		case TRACK_RIGHT:
			{
				m_dRight = (double)(nx - m_nArrowWidth * 3 / 2) / m_dx * (m_dMax - m_dMin) + m_dMin;

				if(m_dRight <= m_dLeft) m_dRight = m_dLeft;
				if(m_dRight >= m_dMax) m_dRight = m_dMax;
				if(dOldRight != m_dRight) dwChanged = HS_RIGHTCHANGED;

				//rect = m_rcRectRight;
			}
			break;

		case TRACK_MIDDLE:
			{
				double dDelta = m_dRight - m_dLeft;
				ASSERT(dDelta >= 0.0);
				m_dLeft  = (double)(nx - m_nArrowWidth) / m_dx * (m_dMax - m_dMin) + m_dMin - dDelta/2.0;
				m_dRight = (double)(nx - m_nArrowWidth) / m_dx * (m_dMax - m_dMin) + m_dMin + dDelta/2.0; 

				if(m_dLeft <= m_dMin)
				{
					m_dLeft = m_dMin;
					m_dRight = m_dLeft + dDelta;
				}
				if(m_dRight >= m_dMax)
				{
					m_dRight = m_dMax;
					m_dLeft = m_dRight - dDelta;
				}

				dwChanged = HS_BOTHCHANGED;
			}
			break;

		default:
			{
				TRACE("Unknown Track Mode\n");
				ASSERT(FALSE);
			}
			break;
		}
		Invalidate();

		if(dOldLeft != m_dLeft || dOldRight != m_dRight)
			::PostMessage(GetParent()->GetSafeHwnd(), RANGE_CHANGED, dwChanged, (LPARAM)(0));
	}

	CWnd::OnMouseMove(nFlags, point);
}

UINT CHistoSlider::OnGetDlgCode()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return DLGC_WANTARROWS;
	//return CWnd::OnGetDlgCode();
}

void CHistoSlider::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UINT nKey1, nKey2;
	nKey1 = VK_RIGHT; nKey2 = VK_LEFT;
	
	if((nChar == nKey1 || nChar == nKey2))
	{
		double dx = (m_dMax - m_dMin) / m_dx;
		if(dx != 0.0 )
		{
			int nLeft = (int) ((m_dLeft - m_dMin) / dx + 0.5);
			int nRight = (int) ((m_dRight - m_dMin) / dx + 0.5);

			BOOL bShift = ::GetKeyState(VK_SHIFT) < 0;
			BOOL bCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
			if(nChar == nKey2)
			{
				if(bCtrl)
				{
					nRight--;
				}
				else if(bShift)
				{
					nLeft--;
				}
				else
				{
					nRight--;
					nLeft--;
				}
			}
			else
			{
				if(bCtrl)
				{
					nRight++;
				}
				else if(bShift)
				{
					nLeft++;
				}
				else
				{
					nRight++;
					nLeft++;
				}
			}
			if(nLeft >= 0 && nRight <= m_dx)
			{
				m_dLeft = m_dMin + nLeft * dx;
				m_dRight = m_dMin + nRight * dx;

				if(m_dLeft > m_dRight) m_dLeft = m_dRight;

				if(bCtrl)
					::SendMessage(GetParent()->GetSafeHwnd(), RANGE_CHANGED, HS_RIGHTCHANGED, 0);
				else if(bShift)
					::SendMessage(GetParent()->GetSafeHwnd(), RANGE_CHANGED, HS_LEFTCHANGED, 0);
				else 
					::SendMessage(GetParent()->GetSafeHwnd(), RANGE_CHANGED, HS_BOTHCHANGED, 0);
			}

			Invalidate();
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CHistoSlider::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Invalidate();
}

void CHistoSlider::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Invalidate();
}

void CHistoSlider::OnPaint()
{
	CPaintDC RawDC(this);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	// Calculate Arrow width and x-axis width.
	CRect rcClientRect;
	GetClientRect(&rcClientRect);

	int ny = rcClientRect.Height();
	m_nArrowWidth = ny - 4;

	CDC dc;
	CBitmap bmPanel, *pbmOldPanel = NULL;
	if(!dc.CreateCompatibleDC(&RawDC)) return;
	if(!bmPanel.CreateCompatibleBitmap(&RawDC, rcClientRect.Width(), rcClientRect.Height()))
	{
		dc.DeleteDC();
		return;
	}

	pbmOldPanel = dc.SelectObject(&bmPanel);
	if(pbmOldPanel == NULL)
	{
		bmPanel.DeleteObject();
		dc.DeleteDC();
		return;
	}

	m_dx = rcClientRect.Width() - 2 * m_nArrowWidth;

	// Erase Background.
	CBrush brBack;
	brBack.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	CBrush* pOldBrush = dc.SelectObject(&brBack);
	CGdiObject *pOldPen = dc.SelectStockObject(NULL_PEN);
	dc.FillRect(&rcClientRect, &brBack);
	brBack.DeleteObject();

	CRect rcClipBox;
	dc.GetClipBox(&rcClipBox);

	// Slide Drawing
	CPen penOut;
	penOut.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	dc.SelectObject(&penOut);
	dc.MoveTo(rcClipBox.left + 1, rcClipBox.top);
	dc.LineTo(rcClipBox.right, rcClipBox.top+1);
	penOut.DeleteObject();

	CPen penIn;
	penIn.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DDKSHADOW));
	dc.SelectObject(&penIn);
	dc.MoveTo(rcClipBox.left, rcClipBox.top);
	dc.LineTo(rcClipBox.right - 1, rcClipBox.top);
	penIn.DeleteObject();

	// Check if [Left, Right] is contained in [Min, Max].
	ASSERT(m_dMin <= m_dLeft);
	ASSERT(m_dLeft <= m_dRight);
	ASSERT(m_dRight <= m_dMax);

	// Position of Left and Right
	int x1 = (int)((m_dLeft - m_dMin)/(m_dMax - m_dMin) * m_dx + 0.5);
	int x2 = (int)((m_dRight - m_dMin)/(m_dMax - m_dMin) * m_dx + 0.5);

	dc.SelectStockObject(BLACK_PEN);
	CBrush brButtonFace;
	brButtonFace.CreateSolidBrush(RGB(255, 255, 255));
	dc.SelectObject(brButtonFace);
	dc.SelectStockObject(BLACK_PEN);

	//CFont fontTahoma;
	//fontTahoma.CreateFont(-11,

	// Draw Left Arrow
	CRect rcFocusRectLeft;
	m_rcRectLeft = CRect(CPoint(x1, 3), CSize(m_nArrowWidth, ny - 4));
	dc.Rectangle(&m_rcRectLeft);
	rcFocusRectLeft = m_rcRectLeft;
	dc.MoveTo(m_rcRectLeft.left + m_rcRectLeft.Width() / 2, m_rcRectLeft.top - 3);
	dc.LineTo(m_rcRectLeft.left + m_rcRectLeft.Width() / 2, m_rcRectLeft.top);

	// Draw Right Arrow
	CRect rcFocusRectRight;
	m_rcRectRight = CRect(CPoint(m_nArrowWidth + x2, 3), CSize(m_nArrowWidth, ny - 4));
	dc.Rectangle(&m_rcRectRight);
	rcFocusRectRight = m_rcRectRight;
	dc.MoveTo(m_rcRectRight.left + m_rcRectRight.Width() / 2, m_rcRectRight.top - 3);
	dc.LineTo(m_rcRectRight.left + m_rcRectRight.Width() / 2, m_rcRectRight.top);

	// Draw Inside range
	CRect rcRange = CRect(CPoint(m_nArrowWidth + x1 + 1, 4), CPoint (m_nArrowWidth + x2, ny - 1));
	dc.SelectStockObject(NULL_PEN);
	dc.Rectangle(&rcRange);

	brButtonFace.DeleteObject();

	// Draw Focus
	if(GetFocus() == this)
	{
		CBrush brFocus;
		brFocus.CreateSolidBrush(RGB(255,255,230));
		dc.SelectObject(&brFocus);

		rcFocusRectLeft.DeflateRect(4,4);
		dc.FillRect(&rcFocusRectLeft, &brFocus);
		rcFocusRectRight.DeflateRect(4,4);
		dc.FillRect(&rcFocusRectRight, &brFocus);

		brFocus.DeleteObject();
	}

	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);

	// Ole'
	RawDC.BitBlt(0, 0, rcClientRect.Width(), rcClientRect.Height(), &dc, 0, 0, SRCCOPY);
}
