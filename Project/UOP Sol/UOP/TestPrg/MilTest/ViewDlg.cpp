// ViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MilTest.h"
#include "ViewDlg.h"


// CViewDlg dialog

IMPLEMENT_DYNAMIC(CViewDlg, CDialog)

CViewDlg::CViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewDlg::IDD, pParent)
{

}

CViewDlg::~CViewDlg()
{
}

void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialog)
END_MESSAGE_MAP()


// CViewDlg message handlers
