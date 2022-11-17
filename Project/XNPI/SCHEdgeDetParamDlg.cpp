// SCHeightParamDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "SCHEdgeDetParamDlg.h"
#include "SCAlgoDialog.h"
#include "SCHEdgeDetAlgo.h"


// CSCHEdgeDetParamDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSCHEdgeDetParamDlg, CSCAlgoDialog)

CSCHEdgeDetParamDlg * CSCHEdgeDetParamDlg::m_pInstance = NULL;

CSCHEdgeDetParamDlg::CSCHEdgeDetParamDlg(CWnd* pParent /*=NULL*/)
	: CSCAlgoDialog(IDD_SCHEIGHTPARAM, pParent)
{

}

CSCHEdgeDetParamDlg::~CSCHEdgeDetParamDlg()
{
}

void CSCHEdgeDetParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBHEIGHTDIR, m_cHEDetDir);
	DDX_Control(pDX, IDOK, m_cApply);
	DDX_Control(pDX, IDCANCEL2, m_cCancel);
}


BEGIN_MESSAGE_MAP(CSCHEdgeDetParamDlg, CSCAlgoDialog)
	ON_BN_CLICKED(IDOK, &CSCHEdgeDetParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSCHEdgeDetParamDlg 메시지 처리기입니다.


CSCHEdgeDetParamDlg * CSCHEdgeDetParamDlg::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSCHEdgeDetParamDlg;
		m_pInstance->Create(IDD_SCHEDGEDETPARAM, NULL);
	}
	return m_pInstance;
}


BOOL CSCHEdgeDetParamDlg::OnInitDialog()
{
	CSCAlgoDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cHEDetDir.AddString(_T("Top->Down"));
	m_cHEDetDir.AddString(_T("Down->Top"));
	CheckDlgButton(IDC_CHK_USE_CNTRLINE, 0);
	m_cHEDetDir.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSCHEdgeDetParamDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CSCHEdgeDetAlgo *)m_pSCAlgo)->m_bUseCenterLine = IsDlgButtonChecked(IDC_CHK_USE_CNTRLINE);
	((CSCHEdgeDetAlgo *)m_pSCAlgo)->m_iHEdgeScanDir = m_cHEDetDir.GetCurSel();
	OnOK();
}

void CSCHEdgeDetParamDlg::LoadParam(CSCAlgoBase * pAlgo)
{
	m_cHEDetDir.SetCurSel(((CSCHEdgeDetAlgo *)pAlgo)->m_iHEdgeScanDir);
	CheckDlgButton(IDC_CHK_USE_CNTRLINE, ((CSCHEdgeDetAlgo *)pAlgo)->m_bUseCenterLine);
}