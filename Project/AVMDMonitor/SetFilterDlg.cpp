// SetFilterDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "SetFilterDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "ProgressBarDlg.h"

// CSetFilterDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetFilterDlg, CDialogEx)

CSetFilterDlg::CSetFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SET_FILTER, pParent)
	, m_bIsFilterEnable(FALSE)
{

}

CSetFilterDlg::~CSetFilterDlg()
{
}

void CSetFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_FILTERNAME, m_combo_FilterType);
	DDX_Control(pDX, IDC_EDIT_LOW_VALUE, m_edit_lowValue);
	DDX_Control(pDX, IDC_EDIT_HIGH_VALUE, m_edit_highValue);
	DDX_Check(pDX, IDC_CHK_ENABLE_FILTER, m_bIsFilterEnable);
}


BEGIN_MESSAGE_MAP(CSetFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetFilterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHK_ENABLE_FILTER, &CSetFilterDlg::OnBnClickedChkEnableFilter)
	ON_CBN_SELCHANGE(IDC_CB_FILTERNAME, &CSetFilterDlg::OnCbnSelchangeCbFiltername)
	ON_BN_CLICKED(IDCANCEL, &CSetFilterDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSetFilterDlg 메시지 처리기입니다.


BOOL CSetFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_combo_FilterType.AddString(_T("disable"));
	m_combo_FilterType.AddString(_T("LowPass"));
	m_combo_FilterType.AddString(_T("HighPass"));
	m_combo_FilterType.AddString(_T("BandPass"));

	int iLowvalue = CStatus::Instance()->m_iFilterLowValue;
	int iHighvalue = CStatus::Instance()->m_iFilterHighValue;
	CString strTemp;
	if (CStatus::Instance()->m_bIsLowPassFilter == TRUE)
	{
		m_combo_FilterType.SetCurSel(1);
		GetDlgItem(IDC_EDIT_LOW_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HIGH_VALUE)->EnableWindow(TRUE);
		m_edit_lowValue.SetWindowTextW(_T("0"));
		strTemp.Format(_T("%d"), iHighvalue);
		m_edit_highValue.SetWindowTextW(strTemp);
	}
	else if (CStatus::Instance()->m_bIsHighPassFilter == TRUE)
	{
		m_combo_FilterType.SetCurSel(2);
		GetDlgItem(IDC_EDIT_LOW_VALUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_HIGH_VALUE)->EnableWindow(FALSE);
		strTemp.Format(_T("%d"), iLowvalue);
		m_edit_lowValue.SetWindowTextW(strTemp);
		m_edit_highValue.SetWindowTextW(_T("500"));
	}
	else if (CStatus::Instance()->m_bIsBandPassFilter == TRUE)
	{
		m_combo_FilterType.SetCurSel(3);
		GetDlgItem(IDC_EDIT_LOW_VALUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_HIGH_VALUE)->EnableWindow(TRUE);
		strTemp.Format(_T("%d"), iLowvalue);
		m_edit_lowValue.SetWindowTextW(strTemp);
		strTemp.Format(_T("%d"), iHighvalue);
		m_edit_highValue.SetWindowTextW(strTemp);
	}
	else //disable
	{
		m_combo_FilterType.SetCurSel(0);
		GetDlgItem(IDC_EDIT_LOW_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HIGH_VALUE)->EnableWindow(FALSE);
		m_edit_lowValue.SetWindowTextW(_T("0"));
		m_edit_highValue.SetWindowTextW(_T("500"));
	}

	m_bIsFilterEnable = CStatus::Instance()->m_bIsFilterEnable;
	ShowWindow(SW_SHOW);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetFilterDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iReturn = 0;
	if (m_bIsFilterEnable) // 체크 박스가 선택됬을 경우에만 적용 시키기
	{
		CString strFilterType;
		m_combo_FilterType.GetLBText(this->m_combo_FilterType.GetCurSel(), strFilterType);
		iReturn = SetFilterSetting(strFilterType);
		if (iReturn == 0)
		{
			if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
			{
				if (strFilterType == "disable")
				{

				}
				else
				{
					CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::FILTER);
					CStatus::Instance()->m_nProgressBarCount = 0;
					CProgressBarDlg *pProgressDlg = new CProgressBarDlg;

					CStatus::Instance()->ReviewDataClear(TRUE, TRUE);
					CStatus::Instance()->ReviewDataFiltering();

					pProgressDlg->DoModal();
					CStatus::Instance()->DisplayDlgUpdateUI();
				}
			}
		}
	}
	else
	{
		iReturn = 0;
		CStatus::Instance()->ReviewDataClear(TRUE, TRUE);
		CStatus::Instance()->SetFilterType(FALSE, FALSE, FALSE);
		CStatus::Instance()->m_strFilterStatus = _T("disable");
		CStatus::Instance()->DisplayDlgUpdateUI();
	}
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
	{
		CStatus::Instance()->UpdateScrn();
		CStatus::Instance()->GraphUpdate();
	}

	if(iReturn ==0)
		CDialogEx::OnOK();
}

void CSetFilterDlg::OnBnClickedChkEnableFilter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CSetFilterDlg::OnCbnSelchangeCbFiltername()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strFilterType;
	m_combo_FilterType.GetLBText(this->m_combo_FilterType.GetCurSel(), strFilterType);

	if (strFilterType == "LowPass")
	{
		GetDlgItem(IDC_EDIT_LOW_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HIGH_VALUE)->EnableWindow(TRUE);
	}
	if (strFilterType == "HighPass")
	{
		GetDlgItem(IDC_EDIT_LOW_VALUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_HIGH_VALUE)->EnableWindow(FALSE);
	}
	if (strFilterType == "BandPass")
	{
		GetDlgItem(IDC_EDIT_LOW_VALUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_HIGH_VALUE)->EnableWindow(TRUE);
	}
	if (strFilterType == "disable")
	{
		GetDlgItem(IDC_EDIT_LOW_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HIGH_VALUE)->EnableWindow(FALSE);
	}

	SetDlgItemText(IDC_EDIT_LOW_VALUE, _T("0"));
	SetDlgItemText(IDC_EDIT_HIGH_VALUE, _T("500"));
}

int CSetFilterDlg::SetFilterSetting(CString strFilterType)
{
	
	int iFftSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount();
	CStatus::Instance()->m_bIsFilterEnable = m_bIsFilterEnable;
	CString strLowValue, strHighValue;

	GetDlgItemText(IDC_EDIT_LOW_VALUE, strLowValue);
	GetDlgItemText(IDC_EDIT_HIGH_VALUE, strHighValue);

	int iLowValue = _ttoi(strLowValue);
	int iHighValue = _ttoi(strHighValue);

	CStatus::Instance()->m_iFilterLowValue = _wtoi(strLowValue);
	CStatus::Instance()->m_iFilterHighValue = _wtoi(strHighValue);
	
	if (strFilterType == "LowPass")
	{
		if (iHighValue < 1 || iHighValue > 499)
		{
			AfxMessageBox(_T("Please check the frequency. The frequency range is from 1Hz to 499Hz."));
			return -1;
		}
		CStatus::Instance()->SetFilterType(TRUE, FALSE, FALSE);
		CStatus::Instance()->m_strFilterStatus = strFilterType + _T(" at ") + strHighValue + _T("Hz");
		//CStatus::Instance()->DisplayDlgUpdateUI();
	}
	else if (strFilterType == "HighPass")
	{
		if (iLowValue < 1 || iLowValue > 499)
		{
			AfxMessageBox(_T("Please check the frequency. The frequency range is from 1Hz to 499Hz."));
			return -1;
		}
		CStatus::Instance()->SetFilterType(FALSE, TRUE, FALSE);
		CStatus::Instance()->m_strFilterStatus = strFilterType + _T(" at ") + strLowValue + _T("Hz");
		//CStatus::Instance()->DisplayDlgUpdateUI();
	}
	else if (strFilterType == "BandPass")
	{
		if (iLowValue < 1 || iLowValue > iHighValue || iHighValue < iLowValue || iHighValue > 499)
		{
			AfxMessageBox(_T("Please check the frequency. The frequency range is from 1Hz to 499Hz."));
			return -1;
		}
		CStatus::Instance()->SetFilterType(FALSE, FALSE, TRUE);
		CStatus::Instance()->m_strFilterStatus = strFilterType + _T(" From ") + strLowValue + _T("Hz to ") + strHighValue + _T("Hz");
		//CStatus::Instance()->DisplayDlgUpdateUI();
	}
	else
	{
		CStatus::Instance()->SetFilterType(FALSE, FALSE, FALSE);
		CStatus::Instance()->m_strFilterStatus = _T("disable");
		//CStatus::Instance()->DisplayDlgUpdateUI();
	}
	CStatus::Instance()->TriggerReset();
	CStatus::Instance()->GoNoGoReset();
	return 0;
}

void CSetFilterDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		
	}
	CDialogEx::OnCancel();
}
