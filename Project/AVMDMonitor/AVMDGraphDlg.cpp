// AVMDGraphDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "AVMDGraphDlg.h"
#include "afxdialogex.h"
#include "DrawingHelper.h"
#include "Status.h"

// CAVMDGraphDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAVMDGraphDlg, CDialogEx)

CAVMDGraphDlg::CAVMDGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GRAPH, pParent)
	
	, m_bIsReSizing(FALSE)
	, m_bToRepaint(FALSE)
	, m_iMouseStart(0)
	, m_iMouseEnd(0)
	, m_nInputType(0)
{
	
}

CAVMDGraphDlg::CAVMDGraphDlg(CString strType, int nInputType)//, BOOL bIsCompareGraph /*=NULL*/)
	: m_iStandardValue(0)
	, m_strGraphType(_T(""))
	, m_iScrClickNum(0)
	, bReEnter(FALSE)
	, m_bIsDownClick(FALSE)
	, m_bIsReSizing(FALSE)
	, m_bToRepaint(FALSE)
	, m_iMouseStart(0)
	, m_iMouseEnd(0)
	, m_nInputType(0)
{
	m_strGraphType = strType;
	m_nInputType = nInputType;
}
CAVMDGraphDlg::~CAVMDGraphDlg()
{
}

void CAVMDGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAVMDGraphDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CAVMDGraphDlg 메시지 처리기입니다.
BOOL CAVMDGraphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAVMDGraphDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	static int nCount = 0;
//jubatemp	printf("\nOnPaintIn:%S:%d", m_strGraphType, ++nCount);

	if (!m_MemDC.m_hDC || !m_MemDC.m_hAttribDC)
	{
		CStatus::Instance()->UpdateScrn();
		return;
	}
		

//jubatemp	printf("\nOnPaintDrawIn:%S:%d", m_strGraphType, nCount);
		DrawView();
		ReleaseDC(m_pDC);

//jubatemp	printf("\nOnPaintOut:%S:%d", m_strGraphType, nCount);
}

void CAVMDGraphDlg::MakeGraph()
{
	static int nCount = 0;
//jubatemp	printf("\nMakeGraphIn:%S:%d", m_strGraphType, ++nCount);

	if (m_strGraphType == "RMS")
		CStatus::Instance()->m_bRMSReEnter = TRUE;
	if (m_strGraphType == "X")
		CStatus::Instance()->m_bXReEnter = TRUE;
	if (m_strGraphType == "Y")
		CStatus::Instance()->m_bYReEnter = TRUE;
	if (m_strGraphType == "Z")
		CStatus::Instance()->m_bZReEnter = TRUE;

	m_bToRepaint = TRUE;

	m_pDC = GetDC();

	CBitmap bmp;
	CBrush backBrush(GetSysColor(COLOR_3DFACE));

	GetClientRect(m_ClienRect);
	m_OldClientRect = m_ClienRect;
	m_MemDC.CreateCompatibleDC(m_pDC);
	BOOL nIsComplete = bmp.CreateCompatibleBitmap(m_pDC, m_ClienRect.Width(), m_ClienRect.Height());

	pOldBitmap = (CBitmap*)m_MemDC.SelectObject(&bmp);
	CBrush* pOldBrush = m_MemDC.SelectObject(&backBrush);

	m_MemDC.PatBlt(0, 0, m_ClienRect.Width(), m_ClienRect.Height(), PATCOPY);

//LONG lStartTime = ::GetTickCount();
	m_drawHelper.SetDC(&m_MemDC);

	m_drawHelper.InitBackGround(m_ClienRect, m_strGraphType, m_nInputType);
	m_drawHelper.DrawSpecLine(CStatus::Instance()->m_EventMode);

	if(CStatus::Instance()->IsFilterUse())
		m_drawHelper.ViewFilter(m_nInputType);
	else
		m_drawHelper.DrawingGraph(m_nInputType);
	//LONG lEndTime = ::GetTickCount();
	//printf("\nDrawing Graph : %d\n", lEndTime - lStartTime);
	Invalidate(FALSE);
//jubatemp	printf("\nMakeGraphOut:%S:%d", m_strGraphType, nCount);
}

void CAVMDGraphDlg::DrawView()
{
	if (m_bToRepaint)
	{
		m_pDC->BitBlt(0, 0, m_ClienRect.Width(), m_ClienRect.Height(), &m_MemDC, 0, 0, SRCCOPY);
		if (m_strGraphType == "RMS")
		{
			CStatus::Instance()->m_bRMSReEnter = FALSE;
			if(m_nInputType == 1)
				CStatus::Instance()->m_bCompareReEnter = FALSE;
		}
		if (m_strGraphType == "X")
		{
			CStatus::Instance()->m_bXReEnter = FALSE;
			if (m_nInputType == 1)
				CStatus::Instance()->m_bCompareReEnter = FALSE;
		}
		if (m_strGraphType == "Y")
		{
			CStatus::Instance()->m_bYReEnter = FALSE;
			if (m_nInputType == 1)
				CStatus::Instance()->m_bCompareReEnter = FALSE;
		}			
		if (m_strGraphType == "Z")
		{
			CStatus::Instance()->m_bZReEnter = FALSE;
			if (m_nInputType == 1)
				CStatus::Instance()->m_bCompareReEnter = FALSE;
		}
	}
	//MemDC.SelectObject(pOldBrush);		//애때매 죽음
	if (m_bToRepaint)
	{
		if (!m_MemDC.m_hDC)
		{
			CStatus::Instance()->GetReEnter();
			m_bToRepaint = FALSE;
			return;
		}
		m_MemDC.SelectObject(pOldBitmap);
		
		m_MemDC.DeleteDC();
		m_MemDC.m_hDC = NULL;
		m_bToRepaint = FALSE;
	}

	CStatus::Instance()->GetReEnter();
}

void CAVMDGraphDlg::MakeReviewGraph()
{
	if (m_strGraphType == "RMS")
	{
		CStatus::Instance()->m_bRMSReEnter = TRUE;
		if (m_nInputType == 1)
			CStatus::Instance()->m_bCompareReEnter = TRUE;
	}
	if (m_strGraphType == "X")
	{
		CStatus::Instance()->m_bXReEnter = TRUE;
		if (m_nInputType == 1)
			CStatus::Instance()->m_bCompareReEnter = TRUE;
	}
	if (m_strGraphType == "Y")
	{
		CStatus::Instance()->m_bYReEnter = TRUE;
		if (m_nInputType == 1)
			CStatus::Instance()->m_bCompareReEnter = TRUE;
	}
	if (m_strGraphType == "Z")
	{
		CStatus::Instance()->m_bZReEnter = TRUE;
		if (m_nInputType == 1)
			CStatus::Instance()->m_bCompareReEnter = TRUE;
	}
		
	
	m_bToRepaint = TRUE;

	m_pDC = GetDC();

	CBitmap bmp;
	CBrush backBrush(GetSysColor(COLOR_3DFACE));
	GetClientRect(m_ClienRect);
	m_OldClientRect = m_ClienRect;
	m_MemDC.CreateCompatibleDC(m_pDC);
	BOOL nIsComplete = bmp.CreateCompatibleBitmap(m_pDC, m_ClienRect.Width(), m_ClienRect.Height());
	pOldBitmap = (CBitmap*)m_MemDC.SelectObject(&bmp);
	CBrush* pOldBrush = m_MemDC.SelectObject(&backBrush);
	BOOL bIsOverlay = CStatus::Instance()->GetOverlayCondition();
	m_MemDC.PatBlt(0, 0, m_ClienRect.Width(), m_ClienRect.Height(), PATCOPY);
	m_drawHelper.SetDC(&m_MemDC);
	m_drawHelper.InitBackGround(m_ClienRect, m_strGraphType, m_nInputType);

	if (CStatus::Instance()->IsFilterUse())
	{
		if (m_nInputType == 0)
		{
			if (bIsOverlay)
			{
				m_drawHelper.DrawReviewFilteredGraph(1, 0, TRUE, FALSE);
				m_drawHelper.DrawReviewFilteredGraph(0, 0, FALSE, TRUE);
			}
			else
				m_drawHelper.DrawReviewFilteredGraph(0, 0, FALSE, TRUE);
		}
		if (m_nInputType == 1)
			m_drawHelper.DrawReviewFilteredGraph(1, 1, FALSE, TRUE);
	}
	else
	{
		if (m_nInputType == 0)
		{
			if (bIsOverlay)
			{
				m_drawHelper.DrawingReviewGraph(1, 0, TRUE, FALSE);
				m_drawHelper.DrawingReviewGraph(0, 0, FALSE, TRUE);
			}
			else
				m_drawHelper.DrawingReviewGraph(0, 0, FALSE, TRUE);
		}
		if (m_nInputType == 1)
			m_drawHelper.DrawingReviewGraph(1, 1, FALSE, TRUE);
	}
	Invalidate(FALSE);
}

void CAVMDGraphDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		m_bIsDownClick = TRUE;
		CStatus::Instance()->SetMarkClickEvent(FALSE, m_nInputType);
		m_iMouseStart = point.x;
		SetCursor(LoadCursor(NULL, IDC_HAND));
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CAVMDGraphDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		m_iMouseEnd = point.x;
		if (m_bIsDownClick)
		{
			m_bIsDownClick = FALSE;
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			CStatus::Instance()->SetAmountofMouseMovement(m_iMouseStart - m_iMouseEnd, m_nInputType);
			CStatus::Instance()->UpdateScrn();
		}
	}
	
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CAVMDGraphDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		if(m_bIsDownClick)
			SetCursor(LoadCursor(NULL, IDC_HAND));
	}
	CDialogEx::OnMouseMove(nFlags, point);
}
