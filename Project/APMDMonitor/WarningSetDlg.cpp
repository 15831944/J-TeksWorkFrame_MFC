// WarningSetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "WarningSetDlg.h"
#include "afxdialogex.h"
#include "Status.h"


// CWarningSetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWarningSetDlg, CDialogEx)

CWarningSetDlg::CWarningSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WARN_SET, pParent)
	, m_dblTemp(0)
	, m_bUseTemp(FALSE)
	, m_bUseP01(FALSE)
	, m_bUseP03(FALSE)
	, m_bUseP05(FALSE)
	, m_bUseWarning(FALSE)
	, m_iThresP01(0)
	, m_iThresP03(0)
	, m_iThresP05(0)
{

}

CWarningSetDlg::~CWarningSetDlg()
{
}

void CWarningSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_SAMPLETIME, m_cSampleTimeSelect);
	DDX_Text(pDX, IDC_EDTWARNTEMP, m_dblTemp);
	DDX_Check(pDX, IDC_CHKUSETEMP, m_bUseTemp);
	DDX_Check(pDX, IDC_CHK_PAT01, m_bUseP01);
	DDX_Check(pDX, IDC_USE_PAT03, m_bUseP03);
	DDX_Check(pDX, IDC_USE_PAT05, m_bUseP05);
	DDX_Check(pDX, IDC_CHK_WARN, m_bUseWarning);
	DDX_Text(pDX, IDC_EDTWARNP01, m_iThresP01);
	DDX_Text(pDX, IDC_EDTWARNP03, m_iThresP03);
	DDX_Text(pDX, IDC_EDTWARNP05, m_iThresP05);
}

BEGIN_MESSAGE_MAP(CWarningSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWarningSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CWarningSetDlg 메시지 처리기입니다.


void CWarningSetDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	CStatus::Instance()->m_iUseP01 = m_bUseP01;
	CStatus::Instance()->m_iUseP03 = m_bUseP03;
	CStatus::Instance()->m_iUseP05 = m_bUseP05;
	CStatus::Instance()->m_iUseTemp = m_bUseTemp;
	CStatus::Instance()->m_iUseWarning = m_bUseWarning;
	CStatus::Instance()->m_dblThresTemp = m_dblTemp;
	CStatus::Instance()->m_iThresP01 = m_iThresP01;
	CStatus::Instance()->m_iThresP03 = m_iThresP03;
	CStatus::Instance()->m_iThresP05 = m_iThresP05;
	CStatus::Instance()->m_iTimeWarningSel = m_cSampleTimeSelect.GetCurSel();
	CStatus::Instance()->SetDefault();
	CDialogEx::OnOK();
}


BOOL CWarningSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cSampleTimeSelect.AddString(_T("Second"));
	m_cSampleTimeSelect.AddString(_T("Minute"));

	m_bUseP01 = CStatus::Instance()->m_iUseP01;
	m_bUseP03 = CStatus::Instance()->m_iUseP03;
	m_bUseP05 = CStatus::Instance()->m_iUseP05;
	m_bUseTemp = CStatus::Instance()->m_iUseTemp;
	m_bUseWarning = CStatus::Instance()->m_iUseWarning;
	m_dblTemp = CStatus::Instance()->m_dblThresTemp;
	m_iThresP01 = CStatus::Instance()->m_iThresP01;
	m_iThresP03 = CStatus::Instance()->m_iThresP03;
	m_iThresP05 = CStatus::Instance()->m_iThresP05;
	m_cSampleTimeSelect.SetCurSel(CStatus::Instance()->m_iTimeWarningSel);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
