// AnnotateMarkDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "AnnotateMarkDlg.h"
#include "afxdialogex.h"


// CAnnotateMarkDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnnotateMarkDlg, CDialogEx)

CAnnotateMarkDlg::CAnnotateMarkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANNOTATE_MARK, pParent)
{

}

CAnnotateMarkDlg::~CAnnotateMarkDlg()
{
}

void CAnnotateMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnnotateMarkDlg, CDialogEx)
END_MESSAGE_MAP()


// CAnnotateMarkDlg 메시지 처리기입니다.


BOOL CAnnotateMarkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
