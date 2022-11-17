// LogIn.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SFW.h"
#include "LogIn.h"
#include "afxdialogex.h"


// CLogIn 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogIn, CDialogEx)

CLogIn::CLogIn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogIn::IDD, pParent)
{

}

CLogIn::~CLogIn()
{
}

void CLogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogIn, CDialogEx)
END_MESSAGE_MAP()


// CLogIn 메시지 처리기입니다.
