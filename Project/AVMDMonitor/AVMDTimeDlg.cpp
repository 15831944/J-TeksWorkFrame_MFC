// AVMDTimeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "AVMDTimeDlg.h"
#include "afxdialogex.h"
#include "DrawingHelper.h"
#include "Status.h"

// CAVMDTimeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAVMDTimeDlg, CDialogEx)

CAVMDTimeDlg::CAVMDTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TIME, pParent)
	, m_nInputType(0)
{
}
CAVMDTimeDlg::CAVMDTimeDlg(CString Degree, double unit, BOOL bIsFFTDlg, int nInputType /*=NULL*/)
{
	strDegree = Degree;
	dblUnit = unit;
	bIsFFT = bIsFFTDlg;
	CStatus::Instance()->SetUnit(strDegree, unit, bIsFFT);
	m_nInputType = nInputType;
}

CAVMDTimeDlg::~CAVMDTimeDlg()
{
}

void CAVMDTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAVMDTimeDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAVMDTimeDlg 메시지 처리기입니다.


void CAVMDTimeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	if (!m_MemDC.m_hDC || !m_MemDC.m_hAttribDC)
	{
		//CStatus::Instance()->UpdateScrn();
		return;
	}
	DrawView();
	ReleaseDC(m_pDC);
	/*CDC* pDC = GetDC();
	CDC MemDC;
	CBitmap* pOldBitmap;
	CBitmap bmp;
	CBrush backBrush(GetSysColor(COLOR_3DFACE));
	CRect parent;
	CRect clientRect;

	this->GetClientRect(clientRect);

	GetClientRect(parent);
	MemDC.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, parent.Width(), parent.Height());

	pOldBitmap = (CBitmap*)MemDC.SelectObject(&bmp);
	CBrush* pOldBrush = MemDC.SelectObject(&backBrush);

	MemDC.PatBlt(0, 0, parent.Width(), parent.Height(), PATCOPY);

	m_drawHelper.SetDC(&MemDC);

	m_drawHelper.SetDeegreeDlgRect(clientRect);
	m_drawHelper.UnitofMeasure(bIsFFT, parent, m_nInputType);
	pDC->BitBlt(0, 0, parent.Width(), parent.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBrush);
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();*/
}


void CAVMDTimeDlg::MakeTimeUnit()
{
	if (bIsFFT == FALSE)
		CStatus::Instance()->m_bTimeDlgReEnter = TRUE;
	if (bIsFFT == TRUE)
		CStatus::Instance()->m_bFFTTimeDlgReEnter = TRUE;

	m_bToRepaint = TRUE;

	m_pDC = GetDC();

	CBitmap bmp;
	CBrush backBrush(GetSysColor(COLOR_3DFACE));

	CRect parent;
	CRect clientRect;

	GetClientRect(m_ClienRect);
	m_OldClientRect = m_ClienRect;
	m_MemDC.CreateCompatibleDC(m_pDC);
	BOOL nIsComplete = bmp.CreateCompatibleBitmap(m_pDC, m_ClienRect.Width(), m_ClienRect.Height());

	pOldBitmap = (CBitmap*)m_MemDC.SelectObject(&bmp);
	CBrush* pOldBrush = m_MemDC.SelectObject(&backBrush);

	m_MemDC.PatBlt(0, 0, m_ClienRect.Width(), m_ClienRect.Height(), PATCOPY);
	
	m_drawHelper.SetDC(&m_MemDC);

	m_drawHelper.SetDeegreeDlgRect(m_ClienRect);
	m_drawHelper.UnitofMeasure(bIsFFT, m_ClienRect, m_nInputType);

	Invalidate(FALSE);
	/*m_pDC->BitBlt(0, 0, parent.Width(), parent.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBrush);
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();*/
}


void CAVMDTimeDlg::DrawView()
{
	if (m_bToRepaint)
	{
		m_pDC->BitBlt(0, 0, m_ClienRect.Width(), m_ClienRect.Height(), &m_MemDC, 0, 0, SRCCOPY);
		if (bIsFFT == FALSE)
			CStatus::Instance()->m_bTimeDlgReEnter = FALSE;
		if (bIsFFT == TRUE)
			CStatus::Instance()->m_bFFTTimeDlgReEnter = FALSE;
	}
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
		//if (m_drawHelper.m_pGraphics != NULL)
		//delete m_drawHelper.m_pGraphics;
		m_MemDC.m_hDC = NULL;
		m_bToRepaint = FALSE;
	}

	CStatus::Instance()->GetReEnter();
}
