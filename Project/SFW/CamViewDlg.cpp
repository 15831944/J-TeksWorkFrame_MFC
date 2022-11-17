// CamViewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SFW.h"
#include "CamViewDlg.h"
#include "afxdialogex.h"


// CCamViewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCamViewDlg, CDialogEx)

CCamViewDlg::CCamViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCamViewDlg::IDD, pParent)
{

}

CCamViewDlg::~CCamViewDlg()
{
}

void CCamViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCAMSEL, m_tabCamSel);
}


BEGIN_MESSAGE_MAP(CCamViewDlg, CDialogEx)
END_MESSAGE_MAP()


// CCamViewDlg 메시지 처리기입니다.


BOOL CCamViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_tabCamSel.InsertItem(1, _T("Xray"));
	m_tabCamSel.InsertItem(2, _T("AOI"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
