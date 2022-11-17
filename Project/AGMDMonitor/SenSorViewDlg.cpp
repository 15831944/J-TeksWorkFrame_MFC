// SenSorViewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "SenSorViewDlg.h"
#include "afxdialogex.h"


// CSenSorViewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSenSorViewDlg, CDialogEx)

CSenSorViewDlg::CSenSorViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENSORVIEWDLG, pParent)
{
	CStatus::Instance()->SetSensorViewDlg(this);
}

CSenSorViewDlg::~CSenSorViewDlg()
{
}

void CSenSorViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSenSorViewDlg, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_UIUPDATE, &CSenSorViewDlg::OnUpdateSensorView)
END_MESSAGE_MAP()


// CSenSorViewDlg 메시지 처리기입니다.


void CSenSorViewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	if (CStatus::Instance()->m_bReEnter)
		return;
	CStatus::Instance()->m_bReEnter = TRUE;
	m_pDC = GetDC();
	//CDC MemDC;
	CRect rectClient;
	GetClientRect(&rectClient);

	//	CBitmap * pOldBitmap;
	CBitmap bmp;
	CBrush backBrush(GetSysColor(COLOR_3DFACE));

	m_MemDC.CreateCompatibleDC(m_pDC);
	bmp.CreateCompatibleBitmap(m_pDC, rectClient.Width(), rectClient.Height());
	m_pOldBitmap = (CBitmap *)m_MemDC.SelectObject(&bmp);
	CBrush* pOldBrush = m_MemDC.SelectObject(&backBrush);
	m_MemDC.PatBlt(0, 0, rectClient.Width(), rectClient.Height(), WHITENESS);
	//
	m_drawHelper.SetDC(&m_MemDC);
	m_drawHelper.DrawingInitialize(rectClient);
	m_drawHelper.DrawingGap();
	//
	m_pDC->BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &m_MemDC, 0, 0, SRCCOPY);

	m_MemDC.SelectObject(m_pOldBitmap);

	m_MemDC.DeleteDC();
	ReleaseDC(m_pDC);

	CStatus::Instance()->m_bReEnter = FALSE;
}
LRESULT CSenSorViewDlg::OnUpdateSensorView(WPARAM wParam, LPARAM lParam)
{
	
	Invalidate(FALSE);
	return 0;
}

BOOL CSenSorViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN))
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
