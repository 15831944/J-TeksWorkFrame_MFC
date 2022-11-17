// EnvSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "EnvSettingDlg.h"
#include "afxdialogex.h"
#include "Status.h"
// CEnvSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEnvSettingDlg, CDialogEx)

CEnvSettingDlg::CEnvSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ENVSETTING, pParent)
	, m_strSaveFolder(_T(""))
	, m_strDataFileName(_T(""))
	, m_strReportGraphWidth(_T(""))
	, m_strReportGraphHeight(_T(""))
{

}

CEnvSettingDlg::~CEnvSettingDlg()
{
}

void CEnvSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_SAVEFOLDER, m_strSaveFolder);
	DDX_Text(pDX, IDC_EDT_DFNAME, m_strDataFileName);
	DDX_Control(pDX, IDC_CB_UNIT, m_cCBUnit);
	DDX_Control(pDX, IDC_CB_TIME, m_cCBTimeValue);
	DDX_Text(pDX, IDC_EDIT_REPORT_WIDTH, m_strReportGraphWidth);
	DDX_Text(pDX, IDC_EDIT_REPORT_HEIGHT, m_strReportGraphHeight);
	DDX_Control(pDX, IDC_CB_VIEWRANGE, m_cCBViewRange);
	DDX_Control(pDX, IDC_CB_DATARANGE, m_cCBDataFrequency);
}


BEGIN_MESSAGE_MAP(CEnvSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SETFOLDER, &CEnvSettingDlg::OnBnClickedBtnSetfolder)
	ON_BN_CLICKED(IDOK, &CEnvSettingDlg::OnBnClickedOk)
//	ON_WM_PAINT()
ON_CBN_SELCHANGE(IDC_CB_UNIT, &CEnvSettingDlg::OnCbnSelchangeCbUnit)
END_MESSAGE_MAP()


// CEnvSettingDlg 메시지 처리기입니다.


void CEnvSettingDlg::OnBnClickedBtnSetfolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFolderDialog FolderDlg(_T("Select the log data save folder."), CStatus::Instance()->GetDataSaveFolder().GetBuffer());
	CString SaveFolder;
	switch(FolderDlg.DoModal())
	{
	case IDOK:
		SaveFolder = FolderDlg.GetFolderPath();
		m_strSaveFolder = SaveFolder;
		UpdateData(FALSE);
		break;
	case IDCANCEL:
		break;
	}


}


BOOL CEnvSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString strTemp;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_strSaveFolder = CStatus::Instance()->GetDataSaveFolder();
	m_strDataFileName = CStatus::Instance()->GetDataFileName();
	
	m_cCBDataFrequency.AddString(_T("500"));
	m_cCBDataFrequency.AddString(_T("1000"));
	if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::Five_HUNDRED)
		m_cCBDataFrequency.SetCurSel(0);
	if ((int)CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::ONE_THOUSAND)
		m_cCBDataFrequency.SetCurSel(1);

	m_cCBViewRange.AddString(_T("-2G~2G(GAL) / -1962~1962(g-Force)"));
	m_cCBViewRange.AddString(_T("-3G~3G(GAL) / -2943~2943(g-Force)"));
	if (CStatus::Instance()->m_viewRangeMode == CStatus::tViewRangeMode::RANGE_TYPE1_MODE)
		m_cCBViewRange.SetCurSel(0);
	if (CStatus::Instance()->m_viewRangeMode == CStatus::tViewRangeMode::RANGE_TYPE2_MODE)
		m_cCBViewRange.SetCurSel(1);

	strTemp.Format(_T("%d"), CStatus::Instance()->m_iReportGraphWidth);
	m_strReportGraphWidth = strTemp;

	strTemp.Format(_T("%d"), CStatus::Instance()->m_iReportGraphHeight);
	m_strReportGraphHeight = strTemp;

	m_cCBUnit.AddString(_T("seconds"));
	m_cCBUnit.AddString(_T("minutes"));
	for (int i = 0; i <= 59; i++)
	{
		strTemp.Format(_T("%d"), i);
		m_cCBTimeValue.AddString(strTemp);
	}

	int iUnitState = 0;
	int iTimeValueSate = 0;
	CStatus::Instance()->GetPreRecordStatus(iTimeValueSate, iUnitState);
	m_cCBUnit.SetCurSel(iUnitState);
	m_cCBTimeValue.SetCurSel(iTimeValueSate);

	UpdateData(FALSE);
	ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEnvSettingDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CStatus::Instance()->SetDataSaveFolder(m_strSaveFolder.GetBuffer());
	CStatus::Instance()->SetDataFileName(m_strDataFileName.GetBuffer());
	CStatus::Instance()->m_iReportGraphWidth = _ttoi(m_strReportGraphWidth);
	CStatus::Instance()->m_iReportGraphHeight = _ttoi(m_strReportGraphHeight);
	CString strTemp;
	m_cCBDataFrequency.GetLBText(m_cCBDataFrequency.GetCurSel(), strTemp);
	if (strTemp == "500")
		CStatus::Instance()->m_DeviceFrequency = CStatus::tDevice_Frequency::Five_HUNDRED;
	if (strTemp == "1000")
		CStatus::Instance()->m_DeviceFrequency = CStatus::tDevice_Frequency::ONE_THOUSAND;

	m_cCBViewRange.GetLBText(m_cCBViewRange.GetCurSel(), strTemp);
	if (strTemp == "-2G~2G(GAL) / -1962~1962(g-Force)")
		CStatus::Instance()->m_viewRangeMode = CStatus::tViewRangeMode::RANGE_TYPE1_MODE;
	if (strTemp == "-3G~3G(GAL) / -2943~2943(g-Force)")
		CStatus::Instance()->m_viewRangeMode = CStatus::tViewRangeMode::RANGE_TYPE2_MODE;

	CStatus::Instance()->SetDefault();
	CStatus::Instance()->SetPreRecordStatus(m_cCBTimeValue.GetCurSel(), m_cCBUnit.GetCurSel());
	CDialogEx::OnOK();
}



void CEnvSettingDlg::OnCbnSelchangeCbUnit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;

	m_cCBTimeValue.ResetContent();
	switch (m_cCBUnit.GetCurSel())
	{
	case 0:
		for (int i = 0; i <= 59; i++)
		{
			strTemp.Format(_T("%d"), i);
			m_cCBTimeValue.AddString(strTemp);
		}
		break;
	case 1:
		for (int i = 1; i <= 5; i++)
		{
			strTemp.Format(_T("%d"), i);
			m_cCBTimeValue.AddString(strTemp);
		}
		m_cCBTimeValue.SetCurSel(0);
		break;
	}
}

void CEnvSettingDlg::GetPreRecordStatus(int iStateTime, int iStateUnit)
{
}

