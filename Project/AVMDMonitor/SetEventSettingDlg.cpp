// SetGoNoGoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "SetEventSettingDlg.h"
#include "afxdialogex.h"
#include "Status.h"

// CSetGoNoGoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetEventSettingDlg, CDialogEx)

CSetEventSettingDlg::CSetEventSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SET_EVENT_SETTING, pParent)
	, m_strRMS_above(_T(""))
	, m_strX_above(_T(""))
	, m_strY_above(_T(""))
	, m_strZ_above(_T(""))
	, m_strX_below(_T(""))
	, m_strY_below(_T(""))
	, m_strZ_below(_T(""))
	, m_strNoGoqualityTime(_T(""))
	, m_bRMS_above(FALSE)
	, m_bX_above(FALSE)
	, m_bY_above(FALSE)
	, m_bZ_above(FALSE)
	, m_bX_below(FALSE)
	, m_bY_below(FALSE)
	, m_bZ_below(FALSE)
	, m_bIsSave(FALSE)
	, m_iEventMode(0)
	, m_strTimeDomainUnit(_T(""))
	, m_strSecond(_T(""))
{

}

CSetEventSettingDlg::~CSetEventSettingDlg()
{
}

void CSetEventSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RMS_ABOVE, m_strRMS_above);
	DDX_Text(pDX, IDC_EDIT_X_ABOVE, m_strX_above);
	DDX_Text(pDX, IDC_EDIT_Y_ABOVE, m_strY_above);
	DDX_Text(pDX, IDC_EDIT_Z_ABOVE, m_strZ_above);
	DDX_Text(pDX, IDC_EDIT_X_BELOW, m_strX_below);
	DDX_Text(pDX, IDC_EDIT_Y_BELOW, m_strY_below);
	DDX_Text(pDX, IDC_EDIT_Z_BELOW, m_strZ_below);
	DDX_Text(pDX, IDC_EDIT_NOGO_VALUE, m_strNoGoqualityTime);
	DDX_Check(pDX, IDC_CHK_RMS, m_bRMS_above);
	DDX_Check(pDX, IDC_CHK_X_ABOVE, m_bX_above);
	DDX_Check(pDX, IDC_CHK_Y_ABOVE, m_bY_above);
	DDX_Check(pDX, IDC_CHK_Z_ABOVE, m_bZ_above);
	DDX_Check(pDX, IDC_CHK_X_BELOW, m_bX_below);
	DDX_Check(pDX, IDC_CHK_Y_BELOW, m_bY_below);
	DDX_Check(pDX, IDC_CHK_Z_BELOW, m_bZ_below);
	DDX_Check(pDX, IDC_CHECK_SAVE, m_bIsSave);
	DDX_Radio(pDX, IDC_RADIO_NONE, m_iEventMode);
	DDX_Text(pDX, IDC_STATIC_TIME_UNIT, m_strTimeDomainUnit);
	DDX_Control(pDX, IDC_CB_QUALIFICATION, m_CB_NoGoSecond);
	DDX_CBString(pDX, IDC_CB_QUALIFICATION, m_strSecond);
}


BEGIN_MESSAGE_MAP(CSetEventSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetEventSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_NONE, &CSetEventSettingDlg::OnBnClickedRadioNone)
	ON_BN_CLICKED(IDC_RADIO_GONOGO, &CSetEventSettingDlg::OnBnClickedRadioGonogo)
	ON_BN_CLICKED(IDC_RADIO_TRIGGER, &CSetEventSettingDlg::OnBnClickedRadioTrigger)
	ON_BN_CLICKED(IDC_CHECK_SAVE, &CSetEventSettingDlg::OnBnClickedCheckSave)
	ON_BN_CLICKED(IDCANCEL, &CSetEventSettingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSetGoNoGoDlg 메시지 처리기입니다.


BOOL CSetEventSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CStatus::Instance()->GetDefault();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int iEventMode = CStatus::Instance()->GetEventMode();
	BOOL bFilterUse = CStatus::Instance()->IsFilterUse();

	/*if (bFilterUse)
	{
		AfxMessageBox(_T("It can not be used while the filter is in use."));
		CDialogEx::OnCancel();
		return FALSE;
	}*/
	if (iEventMode == CStatus::Evt_NONE)
	{
		m_iEventMode = 0;
		GoNoGoGroupBoxShow(FALSE);
		EvtNoneReset();
	}
	if (iEventMode == CStatus::Evt_GONOGO)
	{
		m_iEventMode = 1;
		GoNoGoGroupBoxShow(TRUE);
		GetGoNoGoInfo(CStatus::Instance()->m_strTimeDomainUnit);
	}
	if (iEventMode == CStatus::Evt_TRIGGER)
	{
		m_iEventMode = 2;
		GoNoGoGroupBoxShow(FALSE);
		GetTriggerInfo(CStatus::Instance()->m_strTimeDomainUnit);
	}
	if(CStatus::Instance()->m_strTimeDomainUnit == "g-Force")
		m_strTimeDomainUnit = _T("Units are in g-Force");
	if (CStatus::Instance()->m_strTimeDomainUnit == "GAL")
		m_strTimeDomainUnit = _T("Units are in GAL");

	for (int i = 1; i <= 10; i++)
	{
		CString strTemp;
		strTemp.Format(_T("%d"), i);
		m_CB_NoGoSecond.AddString(strTemp);
	}

	int nIndex = CStatus::Instance()->m_iNoGoIndicator-1;
	if (nIndex <= 0)
		nIndex = 0;
	m_CB_NoGoSecond.SetCurSel(nIndex);
	if (CStatus::Instance()->GetIsSepcOutSave())
		m_bIsSave = TRUE;
	else
		m_bIsSave = FALSE;
	UpdateData(FALSE);

	ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetEventSettingDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (!RangeCheck(CStatus::Instance()->m_strTimeDomainUnit))
		return;
	CStatus::Instance()->SetIsSepcOutSave(m_bIsSave);

	if(m_iEventMode == 0)
		CStatus::Instance()->SetEventMode(CStatus::tEventMode::Evt_NONE);
	if (m_iEventMode == 1)
		CStatus::Instance()->SetEventMode(CStatus::tEventMode::Evt_GONOGO);
	if (m_iEventMode == 2)
		CStatus::Instance()->SetEventMode(CStatus::tEventMode::Evt_TRIGGER);

	int iEventMode = CStatus::Instance()->GetEventMode();

	if (iEventMode == CStatus::Evt_GONOGO)
	{
		int iNogoqualityTime = _ttoi(m_strNoGoqualityTime);
		if (iNogoqualityTime <= 0 || iNogoqualityTime > 50)
		{
			AfxMessageBox(_T("Please Check NoGo qualification Time(From 1ms to 50ms)"));
				return;
		}
		SetGoNoGoInfo(CStatus::Instance()->m_strTimeDomainUnit);
		CStatus::Instance()->TriggerReset();
	}
	if (iEventMode == CStatus::Evt_TRIGGER)
	{
		SetTriggerInfo(CStatus::Instance()->m_strTimeDomainUnit);
		CStatus::Instance()->GoNoGoReset();
	}
	if (iEventMode == CStatus::Evt_NONE)
	{
		CStatus::Instance()->TriggerReset();
		CStatus::Instance()->GoNoGoReset();
	}


	CStatus::Instance()->SetDefault();

	CDialogEx::OnOK();
}

void CSetEventSettingDlg::GetGoNoGoInfo(CString strTimeDomainUnit)
{
	if (strTimeDomainUnit == "g-Force")
	{
		m_strRMS_above.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoAbove_RMS_Value / 981.);
		m_strX_above.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoAbove_X_Value / 981.);
		m_strY_above.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoAbove_Y_Value / 981.);
		m_strZ_above.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoAbove_Z_Value / 981.);
		m_strX_below.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoBelow_X_Value / 981.);
		m_strY_below.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoBelow_Y_Value / 981.);
		m_strZ_below.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoBelow_Z_Value / 981.);
	}
	if (strTimeDomainUnit == "GAL")
	{
		m_strRMS_above.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoAbove_RMS_Value);
		m_strX_above.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoAbove_X_Value);
		m_strY_above.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoAbove_Y_Value);
		m_strZ_above.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoAbove_Z_Value);
		m_strX_below.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoBelow_X_Value);
		m_strY_below.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoBelow_Y_Value);
		m_strZ_below.Format(_T("%.2f"), CStatus::Instance()->m_dblGoNoGoBelow_Z_Value);
	}

	CtrlValueEnableWindow(TRUE);

	if (CStatus::Instance()->m_iGoNoGo_QualityTime == 0)
		m_strNoGoqualityTime.Format(_T("10"));
	else
		m_strNoGoqualityTime.Format(_T("%d"), CStatus::Instance()->m_iGoNoGo_QualityTime);

	m_bRMS_above = CStatus::Instance()->m_bIsGoNoGoAbove_RMS;
	m_bX_above = CStatus::Instance()->m_bIsGoNoGoAbove_X;
	m_bY_above = CStatus::Instance()->m_bIsGoNoGoAbove_Y;
	m_bZ_above = CStatus::Instance()->m_bIsGoNoGoAbove_Z;

	m_bX_below = CStatus::Instance()->m_bIsGoNoGoBelow_X;
	m_bY_below = CStatus::Instance()->m_bIsGoNoGoBelow_Y;
	m_bZ_below = CStatus::Instance()->m_bIsGoNoGoBelow_Z;

	m_bIsSave = CStatus::Instance()->m_bSaveActivation;

	int iTempNum = CStatus::Instance()->m_iNoGoIndicator;
	int iNum;

	if ((iTempNum - 1) <= 0)
		iNum = 0;
	else
		iNum = iTempNum-1;
	m_CB_NoGoSecond.SetCurSel(iNum);
	

	UpdateData(FALSE);
}

void CSetEventSettingDlg::SetGoNoGoInfo(CString strTimeDomainUnit)
{
	UpdateData(TRUE);
	CStatus::Instance()->m_bIsGoNoGoAbove_RMS = m_bRMS_above;
	CStatus::Instance()->m_bIsGoNoGoAbove_X = m_bX_above;
	CStatus::Instance()->m_bIsGoNoGoAbove_Y = m_bY_above;
	CStatus::Instance()->m_bIsGoNoGoAbove_Z = m_bZ_above;

	CStatus::Instance()->m_bIsGoNoGoBelow_X = m_bX_below;
	CStatus::Instance()->m_bIsGoNoGoBelow_Y = m_bY_below;
	CStatus::Instance()->m_bIsGoNoGoBelow_Z = m_bZ_below;

	CStatus::Instance()->m_bSaveActivation = m_bIsSave;

	CString strNum;
	m_CB_NoGoSecond.GetLBText(m_CB_NoGoSecond.GetCurSel(), strNum);
	int iNum = _ttoi(strNum);
	CStatus::Instance()->m_iNoGoIndicator = iNum;

	if (strTimeDomainUnit == "g-Force")
	{
		CStatus::Instance()->m_dblGoNoGoAbove_RMS_Value = _wtof(m_strRMS_above) * 981.;
		CStatus::Instance()->m_dblGoNoGoAbove_X_Value = _wtof(m_strX_above) * 981.;
		CStatus::Instance()->m_dblGoNoGoAbove_Y_Value = _wtof(m_strY_above) * 981.;
		CStatus::Instance()->m_dblGoNoGoAbove_Z_Value = _wtof(m_strZ_above) * 981.;

		CStatus::Instance()->m_dblGoNoGoBelow_X_Value = _wtof(m_strX_below) * 981.;
		CStatus::Instance()->m_dblGoNoGoBelow_Y_Value = _wtof(m_strY_below) * 981.;
		CStatus::Instance()->m_dblGoNoGoBelow_Z_Value = _wtof(m_strZ_below) * 981.;
	}
	if (strTimeDomainUnit == "GAL")
	{
		CStatus::Instance()->m_dblGoNoGoAbove_RMS_Value = _wtof(m_strRMS_above);
		CStatus::Instance()->m_dblGoNoGoAbove_X_Value = _wtof(m_strX_above);
		CStatus::Instance()->m_dblGoNoGoAbove_Y_Value = _wtof(m_strY_above);
		CStatus::Instance()->m_dblGoNoGoAbove_Z_Value = _wtof(m_strZ_above);

		CStatus::Instance()->m_dblGoNoGoBelow_X_Value = _wtof(m_strX_below);
		CStatus::Instance()->m_dblGoNoGoBelow_Y_Value = _wtof(m_strY_below);
		CStatus::Instance()->m_dblGoNoGoBelow_Z_Value = _wtof(m_strZ_below);
	}
	CStatus::Instance()->m_iGoNoGo_QualityTime = _ttoi(m_strNoGoqualityTime);

}

void CSetEventSettingDlg::GetTriggerInfo(CString strTimeDomainUnit)
{
	if (strTimeDomainUnit == "g-Force")
	{
		m_strRMS_above.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerAbove_RMS_Value / 981.);
		m_strX_above.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerAbove_X_Value / 981.);
		m_strY_above.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerAbove_Y_Value / 981.);
		m_strZ_above.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerAbove_Z_Value / 981.);
		m_strX_below.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerBelow_X_Value / 981.);
		m_strY_below.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerBelow_Y_Value / 981.);
		m_strZ_below.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerBelow_Z_Value / 981.);
	}
	if (strTimeDomainUnit == "GAL")
	{
		m_strRMS_above.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerAbove_RMS_Value);
		m_strX_above.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerAbove_X_Value);
		m_strY_above.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerAbove_Y_Value);
		m_strZ_above.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerAbove_Z_Value);
		m_strX_below.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerBelow_X_Value);
		m_strY_below.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerBelow_Y_Value);
		m_strZ_below.Format(_T("%.2f"), CStatus::Instance()->m_dblTriggerBelow_Z_Value);
	}

	CtrlValueEnableWindow(TRUE);

	m_bRMS_above = CStatus::Instance()->m_bIsTriggerAbove_RMS;
	m_bX_above = CStatus::Instance()->m_bIsTriggerAbove_X;
	m_bY_above = CStatus::Instance()->m_bIsTriggerAbove_Y;
	m_bZ_above = CStatus::Instance()->m_bIsTriggerAbove_Z;

	m_bX_below = CStatus::Instance()->m_bIsTriggerBelow_X;
	m_bY_below = CStatus::Instance()->m_bIsTriggerBelow_Y;
	m_bZ_below = CStatus::Instance()->m_bIsTriggerBelow_Z;

	UpdateData(FALSE);
}

void CSetEventSettingDlg::SetTriggerInfo(CString strTimeDomainUnit)
{
	CStatus::Instance()->m_bIsTriggerAbove_RMS = m_bRMS_above;
	CStatus::Instance()->m_bIsTriggerAbove_X = m_bX_above;
	CStatus::Instance()->m_bIsTriggerAbove_Y = m_bY_above;
	CStatus::Instance()->m_bIsTriggerAbove_Z = m_bZ_above;

	CStatus::Instance()->m_bIsTriggerBelow_X = m_bX_below;
	CStatus::Instance()->m_bIsTriggerBelow_Y = m_bY_below;
	CStatus::Instance()->m_bIsTriggerBelow_Z = m_bZ_below;

	if (strTimeDomainUnit == "g-Force")
	{
		CStatus::Instance()->m_dblTriggerAbove_RMS_Value = _wtof(m_strRMS_above) * 981.;
		CStatus::Instance()->m_dblTriggerAbove_X_Value = _wtof(m_strX_above) * 981.;
		CStatus::Instance()->m_dblTriggerAbove_Y_Value = _wtof(m_strY_above) * 981.;
		CStatus::Instance()->m_dblTriggerAbove_Z_Value = _wtof(m_strZ_above) * 981.;

		CStatus::Instance()->m_dblTriggerBelow_X_Value = _wtof(m_strX_below) * 981.;
		CStatus::Instance()->m_dblTriggerBelow_Y_Value = _wtof(m_strY_below) * 981.;
		CStatus::Instance()->m_dblTriggerBelow_Z_Value = _wtof(m_strZ_below) * 981.;
	}
	if (strTimeDomainUnit == "GAL")
	{
		CStatus::Instance()->m_dblTriggerAbove_RMS_Value = _wtof(m_strRMS_above);
		CStatus::Instance()->m_dblTriggerAbove_X_Value = _wtof(m_strX_above);
		CStatus::Instance()->m_dblTriggerAbove_Y_Value = _wtof(m_strY_above);
		CStatus::Instance()->m_dblTriggerAbove_Z_Value = _wtof(m_strZ_above);

		CStatus::Instance()->m_dblTriggerBelow_X_Value = _wtof(m_strX_below);
		CStatus::Instance()->m_dblTriggerBelow_Y_Value = _wtof(m_strY_below);
		CStatus::Instance()->m_dblTriggerBelow_Z_Value = _wtof(m_strZ_below);
	}
}

BOOL CSetEventSettingDlg::RangeCheck(CString strTimeDomainUnit)
{
	double dblRMS_Above = _wtof(m_strRMS_above);
	double dblX_Above = _wtof(m_strX_above);
	double dblY_Above = _wtof(m_strY_above);
	double dblZ_Above = _wtof(m_strZ_above);
	double dblX_Below = _wtof(m_strX_below);
	double dblY_Below = _wtof(m_strY_below);
	double dblZ_Below = _wtof(m_strZ_below);
	if (strTimeDomainUnit == "g-Force")
	{
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE1_MODE)
		{
			if (!(dblRMS_Above >= 0 && dblRMS_Above <= 2))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the RMS rise above is from 0 to 2."));
				return FALSE;
			}
			if (!(dblX_Above >= 0 && dblX_Above <= 2))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the X rise above is from 0 to 2."));
				return FALSE;
			}
			if (!(dblY_Above >= 0 && dblY_Above <= 2))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Y rise above is from 0 to 2."));
				return FALSE;
			}
			if (!(dblZ_Above >= 0 && dblZ_Above <= 2))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Z rise above is from 0 to 2."));
				return FALSE;
			}
			if (!(dblX_Below >= -2 && dblX_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the X rise below is from -2 to 0."));
				return FALSE;
			}
			if (!(dblY_Below >= -2 && dblY_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Y rise below is from -2 to 0."));
				return FALSE;
			}
			if (!(dblZ_Below >= -2 && dblZ_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Z rise below is from -2 to 0."));
				return FALSE;
			}
		}
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE2_MODE)
		{
			if (!(dblRMS_Above >= 0 && dblRMS_Above <= 3))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the RMS rise above is from 0 to 9."));
				return FALSE;
			}
			if (!(dblX_Above >= 0 && dblX_Above <= 3))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the X rise above is from 0 to 3."));
				return FALSE;
			}
			if (!(dblY_Above >= 0 && dblY_Above <= 3))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Y rise above is from 0 to 3."));
				return FALSE;
			}
			if (!(dblZ_Above >= 0 && dblZ_Above <= 3))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Z rise above is from 0 to 3."));
				return FALSE;
			}
			if (!(dblX_Below >= -3 && dblX_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the X rise below is from -3 to 0."));
				return FALSE;
			}
			if (!(dblY_Below >= -3 && dblY_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Y rise below is from -3 to 0."));
				return FALSE;
			}
			if (!(dblZ_Below >= -3 && dblZ_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Z rise below is from -3 to 0."));
				return FALSE;
			}
		}
		return TRUE;
	}
	if (strTimeDomainUnit == "GAL")
	{
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE1_MODE)
		{
			if (!(dblRMS_Above >= 0 && dblRMS_Above <= 1962))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the RMS rise above is from 0 to 1962."));
				return FALSE;
			}
			if (!(dblX_Above >= 0 && dblX_Above <= 1962))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the X rise above is from 0 to 1962."));
				return FALSE;
			}
			if (!(dblY_Above >= 0 && dblY_Above <= 1962))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Y rise above is from 0 to 1962."));
				return FALSE;
			}
			if (!(dblZ_Above >= 0 && dblZ_Above <= 1962))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Z rise above is from 0 to 1962."));
				return FALSE;
			}
			if (!(dblX_Below >= -1962 && dblX_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the X rise below is from -1962 to 0."));
				return FALSE;
			}
			if (!(dblY_Below >= -1962 && dblY_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Y rise below is from -1962 to 0."));
				return FALSE;
			}
			if (!(dblZ_Below >= -1962 && dblZ_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Z rise below is from -1962 to 0."));
				return FALSE;
			}
		}
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE2_MODE)
		{
			if (!(dblRMS_Above >= 0 && dblRMS_Above <= 2943))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the RMS rise above is from 0 to 2943."));
				return FALSE;
			}
			if (!(dblX_Above >= 0 && dblX_Above <= 2943))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the X rise above is from 0 to 2943."));
				return FALSE;
			}
			if (!(dblY_Above >= 0 && dblY_Above <= 2943))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Y rise above is from 0 to 2943."));
				return FALSE;
			}
			if (!(dblZ_Above >= 0 && dblZ_Above <= 2943))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Z rise above is from 0 to 2943."));
				return FALSE;
			}
			if (!(dblX_Below >= -2943 && dblX_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the X rise below is from -2943 to 0."));
				return FALSE;
			}
			if (!(dblY_Below >= -2943 && dblY_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Y rise below is from -2943 to 0."));
				return FALSE;
			}
			if (!(dblZ_Below >= -2943 && dblZ_Below <= 0))
			{
				AfxMessageBox(_T("The range is wrong. Please check again.\nThe range of the Z rise below is from -2943 to 0."));
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}

void CSetEventSettingDlg::OnBnClickedRadioNone()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iEventMode = 0;
	EvtNoneReset();
	GoNoGoGroupBoxShow(FALSE);
}

void CSetEventSettingDlg::OnBnClickedRadioGonogo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iEventMode = 1;
	GetGoNoGoInfo(CStatus::Instance()->m_strTimeDomainUnit);
	GoNoGoGroupBoxShow(TRUE);
}

void CSetEventSettingDlg::OnBnClickedRadioTrigger()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iEventMode = 2;
	GetTriggerInfo(CStatus::Instance()->m_strTimeDomainUnit);
	GoNoGoGroupBoxShow(FALSE);
}

void CSetEventSettingDlg::EvtNoneReset()
{
	GetDlgItem(IDC_EDIT_RMS_ABOVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_X_ABOVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Y_ABOVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Z_ABOVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_X_BELOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Y_BELOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Z_BELOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(FALSE);
}

void CSetEventSettingDlg::OnBnClickedCheckSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	//CStatus::Instance()->SetIsSepcOutSave(m_bIsSave);
	UpdateData(FALSE);
}

void CSetEventSettingDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CSetEventSettingDlg::CtrlValueEnableWindow(bool bIsEnable)
{
	GetDlgItem(IDC_EDIT_RMS_ABOVE)->EnableWindow(bIsEnable);
	GetDlgItem(IDC_EDIT_X_ABOVE)->EnableWindow(bIsEnable);
	GetDlgItem(IDC_EDIT_Y_ABOVE)->EnableWindow(bIsEnable);
	GetDlgItem(IDC_EDIT_Z_ABOVE)->EnableWindow(bIsEnable);
	GetDlgItem(IDC_EDIT_X_BELOW)->EnableWindow(bIsEnable);
	GetDlgItem(IDC_EDIT_Y_BELOW)->EnableWindow(bIsEnable);
	GetDlgItem(IDC_EDIT_Z_BELOW)->EnableWindow(bIsEnable);
	GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(bIsEnable);
}

void CSetEventSettingDlg::GoNoGoGroupBoxShow(bool bIsShow)
{
	GetDlgItem(IDC_STATIC_TEXT2)->ShowWindow(bIsShow);
	GetDlgItem(IDC_STATIC_GROUP_GONOGO)->ShowWindow(bIsShow);
	GetDlgItem(IDC_EDIT_NOGO_VALUE)->ShowWindow(bIsShow);
	GetDlgItem(IDC_STATIC_NOGONO_QUALIFI_UNIT)->ShowWindow(bIsShow);
	GetDlgItem(IDC_STATIC_GONOGO_TEXT1)->ShowWindow(bIsShow);
	GetDlgItem(IDC_CB_QUALIFICATION)->ShowWindow(bIsShow);
}
