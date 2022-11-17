// TabContainerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SFW.h"
#include "TabContainerDlg.h"
#include "afxdialogex.h"


// CTabContainerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabContainerDlg, CDialogEx)

CTabContainerDlg::CTabContainerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabContainerDlg::IDD, pParent)
{

}

CTabContainerDlg::~CTabContainerDlg()
{
}

void CTabContainerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TPANELCONTAINER, m_tabPanelContainer);
}


BEGIN_MESSAGE_MAP(CTabContainerDlg, CDialogEx)
END_MESSAGE_MAP()


// CTabContainerDlg 메시지 처리기입니다.


BOOL CTabContainerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_tabPanelContainer.InsertItem(0, _T("Parameter"));
	m_tabPanelContainer.InsertItem(1, _T("Auto"));
	m_tabPanelContainer.InsertItem(1, _T("Result"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
