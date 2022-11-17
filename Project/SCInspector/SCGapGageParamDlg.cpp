// SCAlignParamDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "SCGapGageParamDlg.h"
#include "SCAlgoDialog.h"
#include "SCGapGageAlgo.h"

// CSCGapGageParamDlg 대화 상자입니다.

CSCGapGageParamDlg * CSCGapGageParamDlg::m_pInstance = NULL;

IMPLEMENT_DYNAMIC(CSCGapGageParamDlg, CSCAlgoDialog)

CSCGapGageParamDlg::CSCGapGageParamDlg(CWnd* pParent /*=NULL*/)
	: CSCAlgoDialog(CSCGapGageParamDlg::IDD, pParent)
{
}

CSCGapGageParamDlg::~CSCGapGageParamDlg()
{
}

void CSCGapGageParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELSCANDIR, m_cSelScanDir);
	DDX_Control(pDX, IDOK, m_cApply);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
}


BEGIN_MESSAGE_MAP(CSCGapGageParamDlg, CSCAlgoDialog)
	ON_BN_CLICKED(IDOK, &CSCGapGageParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSCGapGageParamDlg 메시지 처리기입니다.


CSCGapGageParamDlg * CSCGapGageParamDlg::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSCGapGageParamDlg;
		m_pInstance->Create(IDD_SCALIGNPARAM, NULL);
	}
	return m_pInstance;
}


BOOL CSCGapGageParamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cSelScanDir.AddString(_T("Horizontal"));
	m_cSelScanDir.AddString(_T("Vertical"));
	SetDlgItemInt(IDC_INSPALIGNVAL, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSCGapGageParamDlg::LoadParam(CSCAlgoBase * pAlgo)
{
	m_cSelScanDir.SetCurSel(((CSCGapGageAlgo *)pAlgo)->m_iScanDir);
	SetDlgItemInt(IDC_INSPALIGNVAL, ((CSCGapGageAlgo *)pAlgo)->m_iCriteria);
}


void CSCGapGageParamDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CSCGapGageAlgo *)m_pSCAlgo)->m_iScanDir = m_cSelScanDir.GetCurSel();
	((CSCGapGageAlgo *)m_pSCAlgo)->m_iCriteria = GetDlgItemInt(IDC_INSPALIGNVAL);
	OnOK();
}
