// Diagnosis2Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "Diagnosis2Dlg.h"
#include "afxdialogex.h"


// CDiagnosis2Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDiagnosis2Dlg, CDialogEx)

CDiagnosis2Dlg::CDiagnosis2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIAG2, pParent)
{

}

CDiagnosis2Dlg::~CDiagnosis2Dlg()
{
}

void CDiagnosis2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDiagnosis2Dlg, CDialogEx)
END_MESSAGE_MAP()


// CDiagnosis2Dlg 메시지 처리기입니다.
