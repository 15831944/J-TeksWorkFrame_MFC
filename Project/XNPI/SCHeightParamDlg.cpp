// SCHeightParamDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "SCHeightParamDlg.h"
#include "SCAlgoDialog.h"
#include "SCHeightAlgo.h"


// CSCHeightParamDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSCHeightParamDlg, CSCAlgoDialog)

CSCHeightParamDlg * CSCHeightParamDlg::m_pInstance = NULL;

CSCHeightParamDlg::CSCHeightParamDlg(CWnd* pParent /*=NULL*/)
	: CSCAlgoDialog(IDD_SCHEIGHTPARAM, pParent)
{

}

CSCHeightParamDlg::~CSCHeightParamDlg()
{
}

void CSCHeightParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBHEIGHTDIR, m_cScanDir);
	DDX_Control(pDX, IDOK, m_cApply);
}


BEGIN_MESSAGE_MAP(CSCHeightParamDlg, CSCAlgoDialog)
	ON_BN_CLICKED(IDOK, &CSCHeightParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSCHeightParamDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSCHeightParamDlg 메시지 처리기입니다.


CSCHeightParamDlg * CSCHeightParamDlg::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSCHeightParamDlg;
		m_pInstance->Create(IDD_SCHEIGHTPARAM, NULL);
	}
	return m_pInstance;
}


BOOL CSCHeightParamDlg::OnInitDialog()
{
	CSCAlgoDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemInt(IDC_INSPHEIGHTLOW, 0);
	SetDlgItemInt(IDC_INSPHEIGHTHI, 0);
	m_cScanDir.AddString(_T("Top->Down"));
	m_cScanDir.AddString(_T("Down->Top"));
	m_cScanDir.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSCHeightParamDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CSCHeightAlgo *)m_pSCAlgo)->m_iCriteriaLow = GetDlgItemInt(IDC_INSPHEIGHTLOW);
	((CSCHeightAlgo *)m_pSCAlgo)->m_iCriteriaHi = GetDlgItemInt(IDC_INSPHEIGHTHI);
	((CSCHeightAlgo *)m_pSCAlgo)->m_iScanDir = m_cScanDir.GetCurSel();
	OnOK();
}


void CSCHeightParamDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void CSCHeightParamDlg::LoadParam(CSCAlgoBase * pAlgo)
{
	SetDlgItemInt(IDC_INSPHEIGHTLOW, ((CSCHeightAlgo *)pAlgo)->m_iCriteriaLow);
	SetDlgItemInt(IDC_INSPHEIGHTHI, ((CSCHeightAlgo *)pAlgo)->m_iCriteriaHi);
}