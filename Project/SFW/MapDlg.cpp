// MapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SFW.h"
#include "MapDlg.h"
#include "afxdialogex.h"


// CMapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapDlg, CDialogEx)

CMapDlg::CMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMapDlg::IDD, pParent)
{

}

CMapDlg::~CMapDlg()
{
}

void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TMAPSEL, m_tabMapSel);
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialogEx)
END_MESSAGE_MAP()


// CMapDlg 메시지 처리기입니다.


BOOL CMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_tabMapSel.InsertItem(0, _T("X-Ray"));
	m_tabMapSel.InsertItem(1, _T("ViewCamera"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
