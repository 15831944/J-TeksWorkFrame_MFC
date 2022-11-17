// GraphDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "GraphDlg.h"
#include "afxdialogex.h"
#include "APMDMonitorDoc.h"
#include "MainFrm.h"
#include "Status.h"
#include "ViewOptionDlg.h"
#include "ModeDlg.h"


// CGraphDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGraphDlg, CDialogEx)

CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GRAPH, pParent)
	, m_OnNavi(false)
	, m_pViewOptionDlg(NULL)
{

}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_POSTXT, m_cPosText);
	DDX_Control(pDX, IDC_SLD_POS, m_cNavi);
	DDX_Control(pDX, IDC_STT_MAXPOS, m_cMaxPosText);
	DDX_Control(pDX, IDC_EDT_POSMINTXT, m_cMinPosText);
	DDX_Control(pDX, IDC_STT_MAXMINPOS, m_cMaxMinPosText);
	DDX_Control(pDX, IDC_EDT_DIFF, m_cDiffTime);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialogEx)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_POS, &CGraphDlg::OnNMReleasedcaptureSldPos)
ON_WM_HSCROLL()
ON_WM_PAINT()
ON_WM_MOUSEMOVE()
ON_WM_SETCURSOR()
ON_WM_MOUSEWHEEL()
ON_WM_SIZE()
//ON_EN_UPDATE(IDC_EDT_DIFF, &CGraphDlg::OnEnUpdateEdtDiff)
//ON_EN_CHANGE(IDC_EDT_DIFF, &CGraphDlg::OnEnChangeEdtDiff)
ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLD_POS, &CGraphDlg::OnTRBNThumbPosChangingSldPos)
END_MESSAGE_MAP()


// CGraphDlg 메시지 처리기입니다.


BOOL CGraphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CAPMDMonitorDoc *pDoc = (CAPMDMonitorDoc *)pFrame->GetActiveDocument();
	CreateComponents();
	m_cParticlePlot.SetRepository(&(pDoc->m_Repository), 0);
	m_cComparePlot.SetRepository(&(pDoc->m_Repository), 1);
	m_cNavi.SetRangeMin(1);
	CStatus::Instance()->m_pGraphDlg = this;
	DiffTimeTextOut(0);
	m_cMarkToolTip.Create(NULL, _T("MarkToolTip"), WS_CHILD, CRect(0, 0, 0, 0), this, NULL);
	m_cMarkToolTip.Init(&m_cParticlePlot);
	CRect rectMarkToolTipRect;
	GetClientRect(rectMarkToolTipRect);

	m_cMarkToolTip.m_rectScanArea = rectMarkToolTipRect;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CGraphDlg::OnNMReleasedcaptureSldPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStatus::Instance()->SetNaviStatus(false);
	*pResult = 0;
}



void CGraphDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CStatus::Instance()->SetNaviStatus(true);
	PosTextOut();
	int MainIndexToGo = m_cNavi.GetPos() - 1;
	int CompareIndexToGo = MainIndexToGo + GetDiffTime();
	if (CompareIndexToGo < 0)
	{
		DiffTimeTextOut(GetDiffTime() + CompareIndexToGo);
		CompareIndexToGo = 0;
	}
	m_cParticlePlot.SetCurIndexNo((m_cNavi.GetPos()-1));
	m_cComparePlot.SetCurIndexNo(CompareIndexToGo);
	Invalidate();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CGraphDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DiffTimeTextOut();
	
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}


void CGraphDlg::PosTextOut()
{
	int curPos = m_cNavi.GetPos();
	int maxPos = m_cNavi.GetRangeMax();
	PosTextOut(curPos, maxPos);
}


void CGraphDlg::PosTextOut(int CurPos, int MaxPos)
{
	CString strCurPos, strMaxPos;
	CString strCurMinPos, strMaxMinPos;
	strCurPos.Format(_T("%d"), CurPos);
	strMaxPos.Format(_T("of %ds"), MaxPos);

	strCurMinPos.Format(_T("%d"), CurPos/60);
	strMaxMinPos.Format(_T("of %dm"), MaxPos/60);
	SetDlgItemText(m_cPosText.GetDlgCtrlID(), strCurPos);
	SetDlgItemText(m_cMaxPosText.GetDlgCtrlID(), strMaxPos);

	SetDlgItemText(m_cMinPosText.GetDlgCtrlID(), strCurMinPos);
	SetDlgItemText(m_cMaxMinPosText.GetDlgCtrlID(), strMaxMinPos);
}

void CGraphDlg::PosTextOut(CString strText)
{
	SetDlgItemText(m_cPosText.GetDlgCtrlID(), strText);
}


void CGraphDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TRACKMOUSEEVENT        mouseEvent;
	mouseEvent.cbSize = sizeof(mouseEvent);
	mouseEvent.dwFlags = TME_LEAVE | TME_HOVER;
	mouseEvent.hwndTrack = m_hWnd;
	TrackMouseEvent(&mouseEvent);

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CGraphDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, IDC_HAND));
	return true;
}


BOOL CGraphDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	WPARAM wParam;
	LPARAM lParam=NULL;
	wParam = MAKEWPARAM(0, zDelta);
	if (nFlags & MK_CONTROL)
		m_pViewOptionDlg->m_cHScaleValue.SendMessage(WM_MOUSEWHEEL, wParam, lParam);
	else
		m_pViewOptionDlg->m_cVScaleValue.SendMessage(WM_MOUSEWHEEL, wParam, lParam);
	return true;
//	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CGraphDlg::SetViewOptionDlg(CViewOptionDlg * pViewOptionDlg)
{
	m_pViewOptionDlg = pViewOptionDlg;
}



int CGraphDlg::CheckPosLimit()
{
	int rv = GetDlgItemInt(IDC_EDT_POSTXT);
	if (rv <= 0) {
		rv = 1;
		SetDlgItemInt(IDC_EDT_POSTXT, 1);
	}
	else
	{
		if (rv > m_cNavi.GetRangeMax())
		{
			rv = m_cNavi.GetRangeMax();
			SetDlgItemInt(IDC_EDT_POSTXT, rv);
		}
	}
	return rv;
}

int CGraphDlg::CheckMinPosLimit()
{
	int rv = GetDlgItemInt(IDC_EDT_POSMINTXT);
	if (rv < 0) {
		rv = 0;
		SetDlgItemInt(IDC_EDT_POSMINTXT, 0);
	}
	else
	{
		if ( rv > (m_cNavi.GetRangeMax()/60))
		{
			rv = m_cNavi.GetRangeMax()/60;
			SetDlgItemInt(IDC_EDT_POSMINTXT, rv);
		}
	}
	return rv;
}

int CGraphDlg::CheckDiffTimeLimit()
{
	int rv = GetDiffTime();
	if (rv < -1000) {
		rv = 0;
		SetDlgItemInt(IDC_EDT_DIFF, 0);
	}
	else
	{
		if (rv > 1000)
		{
			SetDlgItemInt(IDC_EDT_DIFF, rv);
		}
	}
	int IndexToMove = m_cParticlePlot.GetCurIndexNo() + rv;
	if (IndexToMove < 0) rv = rv - IndexToMove;
	return rv;
}

BOOL CGraphDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//6월 10일 MarkToolTip기능제거	m_cMarkToolTip.RelayEvent(pMsg);
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}

		if (pMsg->wParam == VK_RETURN)
		{
			if ((pMsg->hwnd) == m_cPosText.GetSafeHwnd())
			{
				int iTemp = CheckPosLimit();
				m_cNavi.SetPos(iTemp);
				m_cParticlePlot.SetCurIndexNo((iTemp - 1));
				Invalidate();
			}
			if ((pMsg->hwnd) == m_cMinPosText.GetSafeHwnd())
			{
				int iTemp = CheckMinPosLimit();
				if (!iTemp)iTemp = 1;
				else iTemp *= 60;
				m_cNavi.SetPos(iTemp);
				m_cParticlePlot.SetCurIndexNo((iTemp - 1));
				Invalidate();
			}
			if ((pMsg->hwnd) == m_cDiffTime.GetSafeHwnd())
			{
				int iTemp = CheckDiffTimeLimit();
				int iIndexTime = m_cParticlePlot.GetCurIndexNo();
				m_cComparePlot.SetCurIndexNo(iIndexTime + iTemp);
				Invalidate();
			}
			return TRUE;
		}
	}

	if (pMsg->message == WM_MOUSEMOVE)
	{

	}


	return CDialogEx::PreTranslateMessage(pMsg);
}



void CGraphDlg::CreateComponents()
{
	CRect rectPlot;
	GetClientRect(&rectPlot);
	rectPlot.bottom %= 2;
	rectPlot.top = 10;
	m_cParticlePlot.Create(NULL, WS_CHILD | WS_VISIBLE | SS_WHITEFRAME | SS_NOTIFY, rectPlot, this, IDC_PLOT);

	m_cComparePlot.Create(NULL, WS_CHILD | WS_VISIBLE | SS_WHITEFRAME | SS_NOTIFY, rectPlot, this, IDC_COMPARE_PLOT);

	SetDualMode(false);
}


void CGraphDlg::SetDualMode(bool bDualMode)
{
	CRect rectPlotArea;
	if (!m_cParticlePlot)return;
	if (!m_cComparePlot) return;

	GetClientRect(&rectPlotArea);
	rectPlotArea.bottom -= 70;
	rectPlotArea.top = 10;
	CRect rectMainArea, rectCompareArea;
	rectMainArea = rectPlotArea; 
	rectCompareArea = rectPlotArea; 

	if(CStatus::Instance()->IsCompareMode())m_cDiffTime.ShowWindow(true);
	else m_cDiffTime.ShowWindow(false);
	if (bDualMode)
	{
		if(CStatus::Instance()->IsOverlayMode())
		{
			m_cComparePlot.ShowWindow(SW_HIDE);
			m_cParticlePlot.MoveWindow(rectPlotArea);
		}
		else
		{
			rectMainArea.bottom /= 2;
			rectCompareArea.top = rectMainArea.bottom;
			m_cParticlePlot.ShowWindow(SW_SHOW);
			m_cComparePlot.ShowWindow(SW_SHOW);
			m_cParticlePlot.MoveWindow(rectMainArea);
			m_cComparePlot.MoveWindow(rectCompareArea);
		}
	}
	else
	{
		m_cParticlePlot.ShowWindow(SW_SHOW);
		m_cComparePlot.ShowWindow(SW_HIDE);
		m_cParticlePlot.MoveWindow(rectPlotArea);
	}

	//auto dynamicLayout = this->GetDynamicLayout();
	//if (dynamicLayout == nullptr)return;

	//auto moveSettings = CMFCDynamicLayout::MoveSettings{};
	////moveSettings.m_nXRatio = 100;
	////moveSettings.m_nYRatio = 100;
	//auto sizeSettings = CMFCDynamicLayout::SizeSettings{};
	//sizeSettings.m_nXRatio = 100;
	//sizeSettings.m_nYRatio = 100;
	//dynamicLayout-
	//dynamicLayout->Create(this);
	//dynamicLayout->AddItem(IDC_PLOT, moveSettings, sizeSettings);
	//dynamicLayout->AddItem(IDC_COMPARE_PLOT, moveSettings, sizeSettings);
	//m_cParticlePlot.EnableDynamicLayout(TRUE);
	Invalidate();
}


void CGraphDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (CStatus::Instance()->IsCompareMode() && !CStatus::Instance()->IsOverlayMode())
	{
		SetDualMode(true);
	}
	else
	{
		SetDualMode(false);
	}
	Invalidate();
}

void CGraphDlg::DiffTimeTextOut()
{
	CString strToOut;
	int nDiffTime = m_cComparePlot.GetCurIndexNo() - m_cParticlePlot.GetCurIndexNo();
	strToOut.Format(_T("%d"), nDiffTime);
	SetDlgItemText(m_cDiffTime.GetDlgCtrlID(), strToOut);
}

void CGraphDlg::DiffTimeTextOut(int nDiffTime)
{
	CString strToOut;
	strToOut.Format(_T("%d"), nDiffTime);
	SetDlgItemText(m_cDiffTime.GetDlgCtrlID(), strToOut);
}

int CGraphDlg::GetDiffTime()
{
	int rv = 0;
	if(CStatus::Instance()->IsCompareMode())
		rv = GetDlgItemInt(IDC_EDT_DIFF);

	return rv;
}


void CGraphDlg::OnTRBNThumbPosChangingSldPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
