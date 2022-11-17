// XrayPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SFW.h"
#include "XrayPanelDlg.h"
#include "afxdialogex.h"


// CXrayPanelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CXrayPanelDlg, CDialogEx)

CXrayPanelDlg::CXrayPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CXrayPanelDlg::IDD, pParent)
{

}

CXrayPanelDlg::~CXrayPanelDlg()
{
}

void CXrayPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CXrayPanelDlg, CDialogEx)
END_MESSAGE_MAP()


// CXrayPanelDlg 메시지 처리기입니다.
