// DispView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "DispView.h"
#include "afxdialogex.h"


// CDispView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDispView, CDialogEx)

CDispView::CDispView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIEW, pParent)
{

}

CDispView::~CDispView()
{
}

void CDispView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDispView, CDialogEx)
END_MESSAGE_MAP()


// CDispView 메시지 처리기입니다.
