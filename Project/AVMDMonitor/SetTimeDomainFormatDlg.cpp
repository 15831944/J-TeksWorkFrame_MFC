// SetTimeDomainFormatDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "SetTimeDomainFormatDlg.h"
#include "afxdialogex.h"
#include "Status.h"

// CSetTimeDomainFormatDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetTimeDomainFormatDlg, CDialogEx)

CSetTimeDomainFormatDlg::CSetTimeDomainFormatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SET_TIME_DOMAIN_FORMAT, pParent)
{

}

CSetTimeDomainFormatDlg::~CSetTimeDomainFormatDlg()
{
}

void CSetTimeDomainFormatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TIME, m_ctrl_combo_TimeDomain);
}


BEGIN_MESSAGE_MAP(CSetTimeDomainFormatDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetTimeDomainFormatDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetTimeDomainFormatDlg 메시지 처리기입니다.


BOOL CSetTimeDomainFormatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrl_combo_TimeDomain.AddString(_T("GAL"));
	m_ctrl_combo_TimeDomain.AddString(_T("g-Force"));
	
	if(CStatus::Instance()->m_strTimeDomainUnit == "GAL")
		m_ctrl_combo_TimeDomain.SetCurSel(0);
	if (CStatus::Instance()->m_strTimeDomainUnit == "g-Force")
		m_ctrl_combo_TimeDomain.SetCurSel(1);
	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetTimeDomainFormatDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTimedoMain;
	m_ctrl_combo_TimeDomain.GetLBText(this->m_ctrl_combo_TimeDomain.GetCurSel(), strTimedoMain);

	CStatus::Instance()->m_strTimeDomainUnit = strTimedoMain;
	//CStatus::Instance()->m_iRMSStandardValue = 0;
	//CStatus::Instance()->m_iXStandardValue = 0;
	//CStatus::Instance()->m_iYStandardValue = 0;
	//CStatus::Instance()->m_iZStandardValue = 0;
	//CStatus::Instance()->m_iCompareStandardValue = 0;
	CStatus::Instance()->DisplayDlgUpdateUI();
	CStatus::Instance()->SpectrumComboBoxUpdate();
	CStatus::Instance()->UpdateUITimeDomain();
	CStatus::Instance()->SetEventMode(CStatus::tEventMode::Evt_NONE);
	CStatus::Instance()->UpdateScrn();

	UpdateData(FALSE);
	CDialogEx::OnOK();
}
