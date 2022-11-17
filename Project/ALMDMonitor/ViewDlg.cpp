// ViewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "ViewDlg.h"
#include "afxdialogex.h"
#include "PlotGage.h"
#include "ALMDMessages.h"
#include "Stabilization.h"
#include "Status.h"


// CViewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewDlg, CDialogEx)

CViewDlg::CViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIEW, pParent)
{

}

CViewDlg::~CViewDlg()
{
}

void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_UPDATE_STATINFO, &CViewDlg::OnUpdateStatusInfo)
END_MESSAGE_MAP()


// CViewDlg 메시지 처리기입니다.


BOOL CViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CViewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	CDC* pDC = GetDC();
	CRect rectClient;
	GetClientRect(&rectClient);
	CDC MemDC;
	CBitmap * pOldBitmap;
	CBitmap bmp;
	MemDC.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rectClient.Width(), rectClient.Height());
	pOldBitmap = (CBitmap *)MemDC.SelectObject(&bmp);
	MemDC.PatBlt(0, 0, rectClient.Width(), rectClient.Height(), WHITENESS);

	CBrush myBrush;
	CBrush *pOldBrush = NULL;
	
	BOOL bIsStable = CStatus::Instance()->GetCurMeasureData()->bStablization;
	if (!bIsStable)
	{
		myBrush.CreateSolidBrush(RGB(228, 228, 228));
		pOldBrush = MemDC.SelectObject(&myBrush);
	}

	m_Gage.SetDC(&MemDC);
	m_Gage.SetMode(CPlotGage::PLOT_DUAL);
//	m_Gage.SetMode(CPlotGage::PLOT_SINGLE);
	m_Gage.SetRect(rectClient);
	m_Gage.Calc();
	m_Gage.DrawBG();
	m_Gage.DrawBubble();
	m_Gage.Draw();
	pDC->BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);
	if (!bIsStable)
	{
		MemDC.SelectObject(pOldBrush);
		myBrush.DeleteObject();
	}
	MemDC.DeleteDC();
	ReleaseDC(pDC);
}


BOOL CViewDlg::PreTranslateMessage(MSG* pMsg)
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


LRESULT CViewDlg::OnUpdateStatusInfo(WPARAM wParam, LPARAM lParam)
{
	Invalidate(FALSE);
	return 0;
}
