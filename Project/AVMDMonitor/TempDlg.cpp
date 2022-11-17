// TempDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "TempDlg.h"
#include "afxdialogex.h"


// CTempDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTempDlg, CDialogEx)

CTempDlg::CTempDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEMPDLG, pParent)
{

}

CTempDlg::~CTempDlg()
{
}

void CTempDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTempDlg, CDialogEx)
END_MESSAGE_MAP()


// CTempDlg 메시지 처리기입니다.
