// GraphDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XFMS.h"
#include "GraphDlg.h"
#include "afxdialogex.h"


// CGraphDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGraphDlg, CDialogEx)

CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GRAPH, pParent)
{

}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGraphDlg 메시지 처리기입니다.

void CGraphDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
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

	BOOL bIsStable = FALSE;
	if (!bIsStable)
	{
		myBrush.CreateSolidBrush(RGB(228, 228, 228));
		pOldBrush = MemDC.SelectObject(&myBrush);
	}

	m_LiveChart.SetDC(&MemDC);
	m_LiveChart.SetRect(rectClient);
	m_LiveChart.Calc();
	m_LiveChart.DrawBG();
	m_LiveChart.Draw();
	pDC->BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &MemDC, 0, 0, SRCCOPY);

	if (!bIsStable)
	{
		MemDC.SelectObject(pOldBrush);
		myBrush.DeleteObject();
	}
	MemDC.DeleteDC();
	ReleaseDC(pDC);
				   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}


int CGraphDlg::AddTempDataToDraw(double * pDblTemp)
{

	return 0;
}
