// Diagnosis3Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "Diagnosis3Dlg.h"
#include "afxdialogex.h"


// CDiagnosis3Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDiagnosis3Dlg, CDialogEx)

CDiagnosis3Dlg::CDiagnosis3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIAG3, pParent)
{

}

CDiagnosis3Dlg::~CDiagnosis3Dlg()
{
}

void CDiagnosis3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDiagnosis3Dlg, CDialogEx)
END_MESSAGE_MAP()


// CDiagnosis3Dlg 메시지 처리기입니다.
