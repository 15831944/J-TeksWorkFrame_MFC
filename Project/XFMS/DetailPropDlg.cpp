// DetailPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XFMS.h"
#include "DetailPropDlg.h"


// CDetailPropDlg

IMPLEMENT_DYNAMIC(CDetailPropDlg, CPropertySheet)

CDetailPropDlg::CDetailPropDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_pTabDetailLogDlg = CTabDetailLogDlg::Instance();
	AddPage(m_pTabDetailLogDlg);
	AddPage(&m_TabDetailResultDlg);
}

CDetailPropDlg::CDetailPropDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pTabDetailLogDlg = CTabDetailLogDlg::Instance();
	AddPage(m_pTabDetailLogDlg);
	AddPage(&m_TabDetailResultDlg);
}

CDetailPropDlg::~CDetailPropDlg()
{
}


BEGIN_MESSAGE_MAP(CDetailPropDlg, CPropertySheet)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDetailPropDlg message handlers

BOOL CDetailPropDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CRect winRC;
	//GetClientRect(winRC);

	//CBrush bkBrush(RGB(0, 0, 0));
	//CBrush* pOldBrush = pDC->SelectObject(&bkBrush);
	//pDC->Rectangle(winRC);
	//pDC->SelectObject(pOldBrush);
	//return TRUE;
	return CPropertySheet::OnEraseBkgnd(pDC);
}
