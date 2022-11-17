// Diagnosis4Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "Diagnosis4Dlg.h"
#include "afxdialogex.h"
#include "Status.h"


// CDiagnosis4Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDiagnosis4Dlg, CDialogEx)

CDiagnosis4Dlg::CDiagnosis4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIAG4, pParent)
	, m_strRPResult(_T(""))
	, m_strRRResult(_T(""))
	, m_strPPResult(_T(""))
{

}

CDiagnosis4Dlg::~CDiagnosis4Dlg()
{
}

void CDiagnosis4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRESULT, m_cGResult);
	DDX_Control(pDX, IDC_RPRESULT, m_cRPResult);
	DDX_Control(pDX, IDC_RRRESULT, m_cRRResult);
	DDX_Control(pDX, IDC_PPRESULT, m_cPPResult);
}


BEGIN_MESSAGE_MAP(CDiagnosis4Dlg, CDialogEx)
END_MESSAGE_MAP()


// CDiagnosis4Dlg 메시지 처리기입니다.


void CDiagnosis4Dlg::HideResult()
{
	m_cGResult.ShowWindow(SW_HIDE);
	m_cPPResult.ShowWindow(SW_HIDE);
	m_cRPResult.ShowWindow(SW_HIDE);
	m_cRRResult.ShowWindow(SW_HIDE);
}


void CDiagnosis4Dlg::ShowResult()
{
	m_cRPResult.SetWindowTextW(m_strRPResult);
	m_cRRResult.SetWindowTextW(m_strRRResult);
	m_cPPResult.SetWindowTextW(m_strPPResult);

	m_cGResult.ShowWindow(SW_SHOW);
	m_cPPResult.ShowWindow(SW_SHOW);
	m_cRPResult.ShowWindow(SW_SHOW);
	m_cRRResult.ShowWindow(SW_SHOW);
}


void CDiagnosis4Dlg::SetResult(double dblRPResult, double dblRRResult, double dblPPResult)
{
	if (CStatus::Instance()->IsDigit2Mode())
	{
		m_strRPResult.Format(_T("Between Roll and Pitch : %2.2f"), dblRPResult);
		m_strRRResult.Format(_T("Roll to Roll : %2.2f"), dblRRResult);
		m_strPPResult.Format(_T("Pitch to Pitch : %2.2f"), dblPPResult);
	}
	else
	{
		m_strRPResult.Format(_T("Between Roll and Pitch : %2.3f"), dblRPResult);
		m_strRRResult.Format(_T("Roll to Roll : %2.3f"), dblRRResult);
		m_strPPResult.Format(_T("Pitch to Pitch : %2.3f"), dblPPResult);
	}
}
