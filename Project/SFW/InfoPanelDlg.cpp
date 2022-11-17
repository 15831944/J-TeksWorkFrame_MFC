// InfoPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SFW.h"
#include "InfoPanelDlg.h"
#include "afxdialogex.h"


// CInfoPanelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInfoPanelDlg, CDialogEx)

CInfoPanelDlg::CInfoPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfoPanelDlg::IDD, pParent)
{

}

CInfoPanelDlg::~CInfoPanelDlg()
{
}

void CInfoPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TINFOSEL, m_tabInfoSel);
}


BEGIN_MESSAGE_MAP(CInfoPanelDlg, CDialogEx)
END_MESSAGE_MAP()


// CInfoPanelDlg 메시지 처리기입니다.


BOOL CInfoPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_tabInfoSel.InsertItem(0, _T("Log"));
	m_tabInfoSel.InsertItem(1, _T("Detail"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
