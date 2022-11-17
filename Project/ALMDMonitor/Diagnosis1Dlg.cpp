// Diagnosis1Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "Diagnosis1Dlg.h"
#include "afxdialogex.h"


// CDiagnosis1Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDiagnosis1Dlg, CDialogEx)

CDiagnosis1Dlg::CDiagnosis1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIAG1, pParent)
{

}

CDiagnosis1Dlg::~CDiagnosis1Dlg()
{
}

void CDiagnosis1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDiagnosis1Dlg, CDialogEx)
END_MESSAGE_MAP()


// CDiagnosis1Dlg 메시지 처리기입니다.
