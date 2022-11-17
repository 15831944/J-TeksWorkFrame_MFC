// XMLTestDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "UniUntilityTest.h"
#include "XMLTestDlg.h"
#include "afxdialogex.h"
#include <UniUtility.h>

// CXMLTestDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CXMLTestDlg, CDialog)

CXMLTestDlg::CXMLTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXMLTestDlg::IDD, pParent)
{

}

CXMLTestDlg::~CXMLTestDlg()
{
}

void CXMLTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CXMLTestDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_XML_READ, &CXMLTestDlg::OnBnClickedBtnXmlRead)
	ON_BN_CLICKED(IDC_BTN_XML_WRITE, &CXMLTestDlg::OnBnClickedBtnXmlWrite)
END_MESSAGE_MAP()


// CXMLTestDlg 메시지 처리기입니다.


void CXMLTestDlg::OnBnClickedBtnXmlRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCfgFile MyXMLCfg;
	MyXMLCfg.SetPath(CAppInfo::GetAppDir() + _T("\\My.Cfg"));
	MyXMLCfg.SetDepth(2);
	MyXMLCfg.Open();

	long iValue;
	double dblValue;
	CString strValue;
	dblValue	= MyXMLCfg.Get(10.5, _T("Setup"), _T("FirstDouble"));
	iValue = MyXMLCfg.Get(15L, _T("Setup"), _T("FirstLong"));
	strValue = MyXMLCfg.Get(_T("TESTstring"), _T("Setup"), _T("FirstString"));

}


void CXMLTestDlg::OnBnClickedBtnXmlWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCfgFile MyXMLCfg;
	MyXMLCfg.SetPath(CAppInfo::GetAppDir() + _T("\\My.Cfg"));
	MyXMLCfg.SetDepth(2);
	MyXMLCfg.Open();

	MyXMLCfg.Set(55.7, _T("Setup"), _T("FirstDouble"));
	MyXMLCfg.Set(15L, _T("Setup"), _T("FirstLong"));
	MyXMLCfg.Set(_T("testString"), _T("Setup"), _T("FirstString"));

	MyXMLCfg.Flush();
}
