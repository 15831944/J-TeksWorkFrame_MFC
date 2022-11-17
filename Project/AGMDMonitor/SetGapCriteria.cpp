// SetGapCriteria.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "SetGapCriteria.h"
#include "afxdialogex.h"
#include "Status.h"

// CSetGapCriteria 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetGapCriteria, CDialogEx)

CSetGapCriteria::CSetGapCriteria(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETGAPCRITERIADLG, pParent)
	, m_iGapUnit(FALSE)
	, m_bAllGapSameSettings(FALSE)
	, m_strAllTarget(_T(""))
	, m_strGap1Target(_T(""))
	, m_strGap2Target(_T(""))
	, m_strGap3Target(_T(""))
	, m_strALLTolerance(_T(""))
	, m_strGap1Tolerance(_T(""))
	, m_strGap2Tolerance(_T(""))
	, m_strGap3Tolerance(_T(""))
	, m_strViewRange(_T(""))
{

}

CSetGapCriteria::~CSetGapCriteria()
{
}

void CSetGapCriteria::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_INCHES, m_iGapUnit);
	DDX_Check(pDX, IDC_CHECK_ALL_GAP_SAME, m_bAllGapSameSettings);
	DDX_Text(pDX, IDC_EDIT_ALL_TARGET, m_strAllTarget);
	DDX_Text(pDX, IDC_EDIT_GAP1_TARGET, m_strGap1Target);
	DDX_Text(pDX, IDC_EDIT_GAP2_TARGET, m_strGap2Target);
	DDX_Text(pDX, IDC_EDIT_GAP3_TARGET, m_strGap3Target);
	DDX_Text(pDX, IDC_EDIT_ALL_TOLERANCE, m_strALLTolerance);
	DDX_Text(pDX, IDC_EDIT_GAP1_TOLERANCE, m_strGap1Tolerance);
	DDX_Text(pDX, IDC_EDIT_GAP2_TOLERANCE, m_strGap2Tolerance);
	DDX_Text(pDX, IDC_EDIT_GAP3_TOLERANCE, m_strGap3Tolerance);
	DDX_Text(pDX, IDC_EDIT_VIEW_RANGE, m_strViewRange);
}


BEGIN_MESSAGE_MAP(CSetGapCriteria, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_INCHES, &CSetGapCriteria::OnBnClickedRadioInches)
	ON_BN_CLICKED(IDC_RADIO_MM, &CSetGapCriteria::OnBnClickedRadioMm)
	ON_BN_CLICKED(IDC_CHECK_ALL_GAP_SAME, &CSetGapCriteria::OnBnClickedCheckAllGapSame)
	ON_BN_CLICKED(IDOK, &CSetGapCriteria::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetGapCriteria 메시지 처리기입니다.


BOOL CSetGapCriteria::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_iGapUnit = CStatus::Instance()->GetUnitMode();

	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
	{
		CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_INCHES);
		pCheck->SetCheck(1);
	}
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
	{
		CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_INCHES);
		pCheck->SetCheck(0);

		CButton* pCheck2 = (CButton*)GetDlgItem(IDC_RADIO_MM);
		pCheck2->SetCheck(1);
	}
	double dblViewRange;
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
	{
		m_bAllGapSameSettings = CStatus::Instance()->GetIsGapSameSettings();
		CheckDlgButton(IDC_CHECK_ALL_GAP_SAME, m_bAllGapSameSettings);
		dblViewRange = CStatus::Instance()->GetGapRangeView();
	}
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		
		tAGMDRecord *pMeaInfo = CStatus::Instance()->GetRepository()->GetMeasureInfo(CStatus::Instance()->GetRepository()->m_iDispIndex);
		m_bAllGapSameSettings = pMeaInfo->bIsAllGapSame;
		CheckDlgButton(IDC_CHECK_ALL_GAP_SAME, m_bAllGapSameSettings);
		dblViewRange = pMeaInfo->dblGapViewRange;
	}

	UpdateData(TRUE);
	m_strViewRange.Format(_T("%.3f"), dblViewRange);
	UpdateData(FALSE);
	ChangeDlgUnit();
	GetControlValue();
	UpdateData(FALSE);

	DisableControl(m_bAllGapSameSettings);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetGapCriteria::DisableControl(BOOL bIsGapSameSetting)
{
	if (bIsGapSameSetting)
	{
		GetDlgItem(IDC_EDIT_ALL_TARGET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ALL_TOLERANCE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAP1_TARGET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAP1_TOLERANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAP2_TARGET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAP2_TOLERANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAP3_TARGET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAP3_TOLERANCE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_ALL_TARGET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ALL_TOLERANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAP1_TARGET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAP1_TOLERANCE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAP2_TARGET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAP2_TOLERANCE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAP3_TARGET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAP3_TOLERANCE)->EnableWindow(TRUE);
	}
}

void CSetGapCriteria::ChangeDlgUnit()
{
	UpdateData(TRUE);
	CString strUnit;
	if (m_iGapUnit == 0)
		strUnit = _T("inches");
	if(m_iGapUnit == 1)
		strUnit = _T("mm");
	
	SetDlgItemText(IDC_STATIC_ALL_TARGET_UNIT, strUnit);
	SetDlgItemText(IDC_STATIC_GAP1_TARGET_UNIT, strUnit);
	SetDlgItemText(IDC_STATIC_GAP2_TARGET_UNIT, strUnit);
	SetDlgItemText(IDC_STATIC_GAP3_TARGET_UNIT, strUnit);
	SetDlgItemText(IDC_STATIC_ALL_TOLERANCE_UNIT, strUnit);
	SetDlgItemText(IDC_STATIC_GAP1_TOLERANCE_UNIT, strUnit);
	SetDlgItemText(IDC_STATIC_GAP2_TOLERANCE_UNIT, strUnit);
	SetDlgItemText(IDC_STATIC_GAP3_TOLERANCE_UNIT, strUnit);

	UpdateData(FALSE);
}

void CSetGapCriteria::OnBnClickedRadioInches()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_iGapUnit == 0)
		return;
	m_iGapUnit = 0;
	CalcControlValue();
	ChangeDlgUnit();
}

void CSetGapCriteria::OnBnClickedRadioMm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_iGapUnit == 1)
		return;
	m_iGapUnit = 1;
	CalcControlValue();
	ChangeDlgUnit();
}

void CSetGapCriteria::OnBnClickedCheckAllGapSame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	DisableControl(m_bAllGapSameSettings);
	UpdateData(FALSE);
}

void CSetGapCriteria::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	tCheckRange range = CheckRange();
	if (range.bRangeOver)
	{
		AfxMessageBox(range.strMessage);
		return;
	}
	if (m_iGapUnit == 0)
		CStatus::Instance()->SetUnitMode(CStatus::tUnitMode::UNIT_INCH);
	if (m_iGapUnit == 1)
		CStatus::Instance()->SetUnitMode(CStatus::tUnitMode::UNIT_MM);

	CStatus::Instance()->SetIsGapSameSettings(m_bAllGapSameSettings);

	

	SetControlValue();
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetDefault();
	CStatus::Instance()->MonitorUpdate();
	CStatus::Instance()->OPPanelUnitUpdate();
	CStatus::Instance()->OPPanelOffsetUpdate();
	CStatus::Instance()->UpdateScrn();
	UpdateData(FALSE);
	
	CDialogEx::OnOK();
}

void CSetGapCriteria::GetControlValue()
{
	m_strGap1Target.Format(_T("%.3f"), CStatus::Instance()->GetTarget(0));
	m_strGap2Target.Format(_T("%.3f"), CStatus::Instance()->GetTarget(1));
	m_strGap3Target.Format(_T("%.3f"), CStatus::Instance()->GetTarget(2));
	m_strAllTarget.Format(_T("%.3f"), CStatus::Instance()->GetTarget(3));

	m_strGap1Tolerance.Format(_T("%.3f"), CStatus::Instance()->GetTolerance(0));
	m_strGap2Tolerance.Format(_T("%.3f"), CStatus::Instance()->GetTolerance(1));
	m_strGap3Tolerance.Format(_T("%.3f"), CStatus::Instance()->GetTolerance(2));
	m_strALLTolerance.Format(_T("%.3f"), CStatus::Instance()->GetTolerance(3));
}

void CSetGapCriteria::SetControlValue()
{
	CString strTarget, strTolerance, strViewRange;
	double dblTarget, dblTolerance;

	GetDlgItemText(IDC_EDIT_VIEW_RANGE, strViewRange);
	CStatus::Instance()->SetGapRangeView(_wtof(strViewRange));

	GetDlgItemTextW(IDC_EDIT_ALL_TARGET, strTarget);
	GetDlgItemTextW(IDC_EDIT_ALL_TOLERANCE, strTolerance);
	dblTarget = _wtof(strTarget);
	dblTolerance = _wtof(strTolerance);

	CStatus::Instance()->SetTarget(3, dblTarget);
	CStatus::Instance()->SetTolerance(3, dblTolerance);

	GetDlgItemTextW(IDC_EDIT_GAP1_TARGET, strTarget);
	GetDlgItemTextW(IDC_EDIT_GAP1_TOLERANCE, strTolerance);
	dblTarget = _wtof(strTarget);
	dblTolerance = _wtof(strTolerance);
	CStatus::Instance()->SetTarget(0, dblTarget);
	CStatus::Instance()->SetTolerance(0, dblTolerance);

	GetDlgItemTextW(IDC_EDIT_GAP2_TARGET, strTarget);
	GetDlgItemTextW(IDC_EDIT_GAP2_TOLERANCE, strTolerance);
	dblTarget = _wtof(strTarget);
	dblTolerance = _wtof(strTolerance);
	CStatus::Instance()->SetTarget(1, dblTarget);
	CStatus::Instance()->SetTolerance(1, dblTolerance);

	GetDlgItemTextW(IDC_EDIT_GAP3_TARGET, strTarget);
	GetDlgItemTextW(IDC_EDIT_GAP3_TOLERANCE, strTolerance);
	dblTarget = _wtof(strTarget);
	dblTolerance = _wtof(strTolerance);
	CStatus::Instance()->SetTarget(2, dblTarget);
	CStatus::Instance()->SetTolerance(2, dblTolerance);
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		tAGMDRecord *pMeaInfo = CStatus::Instance()->GetRepository()->GetMeasureInfo(CStatus::Instance()->GetRepository()->m_iDispIndex);
		if (pMeaInfo == NULL)
			return;
		pMeaInfo->iUnitMode = m_iGapUnit;
		CString strViewRange;
		GetDlgItemTextW(IDC_EDIT_VIEW_RANGE, strViewRange);
		pMeaInfo->dblGapViewRange = _wtof(strViewRange);

		pMeaInfo->bIsAllGapSame = m_bAllGapSameSettings;

		GetDlgItemTextW(IDC_EDIT_ALL_TARGET, strTarget);
		GetDlgItemTextW(IDC_EDIT_ALL_TOLERANCE, strTolerance);
		dblTarget = _wtof(strTarget);
		dblTolerance = _wtof(strTolerance);
		CStatus::Instance()->SetPointTarget(3, dblTarget);
		CStatus::Instance()->SetPointTolerance(3, dblTolerance);

		GetDlgItemTextW(IDC_EDIT_GAP1_TARGET, strTarget);
		GetDlgItemTextW(IDC_EDIT_GAP1_TOLERANCE, strTolerance);
		dblTarget = _wtof(strTarget);
		dblTolerance = _wtof(strTolerance);
		CStatus::Instance()->SetPointTarget(0, dblTarget);
		CStatus::Instance()->SetPointTolerance(0, dblTolerance);

		GetDlgItemTextW(IDC_EDIT_GAP2_TARGET, strTarget);
		GetDlgItemTextW(IDC_EDIT_GAP2_TOLERANCE, strTolerance);
		dblTarget = _wtof(strTarget);
		dblTolerance = _wtof(strTolerance);
		CStatus::Instance()->SetPointTarget(1, dblTarget);
		CStatus::Instance()->SetPointTolerance(1, dblTolerance);

		GetDlgItemTextW(IDC_EDIT_GAP3_TARGET, strTarget);
		GetDlgItemTextW(IDC_EDIT_GAP3_TOLERANCE, strTolerance);
		dblTarget = _wtof(strTarget);
		dblTolerance = _wtof(strTolerance);
		CStatus::Instance()->SetPointTarget(2, dblTarget);
		CStatus::Instance()->SetPointTolerance(2, dblTolerance);
	}
}

double CSetGapCriteria::OriginalValueToCalcValue(double dblGapValue, int iGapUnit)
{
	double dbCalcValue;
	if (iGapUnit == CStatus::tUnitMode::UNIT_MM)
		dbCalcValue = dblGapValue / UNIT_MM_VALUE;
	if (iGapUnit == CStatus::tUnitMode::UNIT_INCH)
		dbCalcValue = dblGapValue / UNIT_MM_VALUE * UNIT_INCH_VALUE; //  0.03937 : mm -> inche
	return dbCalcValue;
}

double CSetGapCriteria::CalcValueToOriginalValue(CString strGapValue, int iGapUnit)
{
	double dblGapValue;
	double dblOrgGapValue;

	dblGapValue = _wtof(strGapValue);
	if (iGapUnit == CStatus::tUnitMode::UNIT_MM)
		dblOrgGapValue = dblGapValue * UNIT_MM_VALUE / UNIT_INCH_VALUE;
	if (iGapUnit == CStatus::tUnitMode::UNIT_INCH)
		dblOrgGapValue = dblGapValue * UNIT_MM_VALUE;
	
	return dblOrgGapValue;
}

void CSetGapCriteria::CalcControlValue()
{
	UpdateData(TRUE);
	CString strTemp;
	double dblCalcNum = 0;
	
	GetDlgItemText(IDC_EDIT_ALL_TARGET, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iGapUnit);
	m_strAllTarget.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, m_iGapUnit));

	GetDlgItemText(IDC_EDIT_GAP1_TARGET, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iGapUnit);
	m_strGap1Target.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, m_iGapUnit));

	GetDlgItemText(IDC_EDIT_GAP2_TARGET, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iGapUnit);
	m_strGap2Target.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, m_iGapUnit));

	GetDlgItemText(IDC_EDIT_GAP3_TARGET, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iGapUnit);
	m_strGap3Target.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, m_iGapUnit));

	GetDlgItemText(IDC_EDIT_ALL_TOLERANCE, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iGapUnit);
	m_strALLTolerance.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, m_iGapUnit));

	GetDlgItemText(IDC_EDIT_GAP1_TOLERANCE, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iGapUnit);
	m_strGap1Tolerance.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, m_iGapUnit));

	GetDlgItemText(IDC_EDIT_GAP2_TOLERANCE, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iGapUnit);
	m_strGap2Tolerance.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, m_iGapUnit));

	GetDlgItemText(IDC_EDIT_GAP3_TOLERANCE, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iGapUnit);
	m_strGap3Tolerance.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, m_iGapUnit));

	GetDlgItemText(IDC_EDIT_VIEW_RANGE, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iGapUnit);
	m_strViewRange.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, m_iGapUnit));
	
	UpdateData(FALSE);
}

CSetGapCriteria::tCheckRange CSetGapCriteria::CheckRange()
{
	tCheckRange checkRenge;
	checkRenge.bRangeOver = FALSE;
	CString strViewRange;
	GetDlgItemText(IDC_EDIT_VIEW_RANGE, strViewRange);

	double dblViewRange = _wtof(strViewRange);

	double dblTarget1, dblTarget2, dblTarget3, dblTargetAll;
	double dblTolerance1, dblTolerance2, dblTolerance3, dblToleranceAll;

	dblTarget1 = _wtof(m_strGap1Target);
	dblTarget2 = _wtof(m_strGap2Target);
	dblTarget3 = _wtof(m_strGap3Target);
	dblTargetAll = _wtof(m_strAllTarget);
	dblTolerance1 = _wtof(m_strGap1Tolerance);
	dblTolerance2 = _wtof(m_strGap2Tolerance);
	dblTolerance3 = _wtof(m_strGap3Tolerance);
	dblToleranceAll = _wtof(m_strALLTolerance);

	if (m_iGapUnit == 0) // inch
	{
		if (dblViewRange < 0.0001 || dblViewRange > 1.1811)
		{
			checkRenge.strMessage = _T("Gap View Range is from 0.0001inces to 1.1811inces.");
			checkRenge.bRangeOver = TRUE;
			return checkRenge;
		}

		if (m_bAllGapSameSettings) // Same Target, Tolerance
		{
			if (dblTargetAll<0.1968 || dblTargetAll > 1.1811)
			{
				checkRenge.strMessage = _T("Target Range is from 0.1968inces to 1.1811inces.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}
			if (dblToleranceAll<0.003 || dblToleranceAll > 11.000)
			{
				checkRenge.strMessage = _T("Tolerance Range is from 0.003inces to 11.000inces.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}
		}
		else
		{
			if (dblTarget1<0.1968 || dblTarget1 > 1.1811)
			{
				checkRenge.strMessage = _T("Gap1 Target Range is from 0.1968inces to 1.1811inces.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}
			if (dblTarget2<0.1968 || dblTarget2 > 1.1811)
			{
				checkRenge.strMessage = _T("Gap2 Target Range is from 0.1968inces to 1.1811inces.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}
			if (dblTarget3<0.1968 || dblTarget3 > 1.1811)
			{
				checkRenge.strMessage = _T("Gap3 Target Range is from 0.1968inces to 1.1811inces.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}

			if (dblTolerance1<0.0001 || dblTolerance1 > 0.4330)
			{
				checkRenge.strMessage = _T("Gap1 Tolerance Range is from 0.0001inces to 0.4330inces.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}
			if (dblTolerance2<0.0001 || dblTolerance2 > 11.000)
			{
				checkRenge.strMessage = _T("Gap2 Tolerance Range is from 0.0001inces to 0.4330inces.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}
			if (dblTolerance3<0.0001 || dblTolerance3 > 11.000)
			{
				checkRenge.strMessage = _T("Gap3 Tolerance Range is from 0.0001inces to 0.4330inces.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}
		}
		

	}
	if (m_iGapUnit == 1) // mm
	{
		if (dblViewRange < 0.003 || dblViewRange > 30.000)
		{
			checkRenge.strMessage = _T("Gap View Range is from 0.003mm to 30.000mm.");
			checkRenge.bRangeOver = TRUE;
			return checkRenge;
		}

		if (m_bAllGapSameSettings) // Same Target, Tolerance
		{
			if (dblTargetAll<5.000 || dblTargetAll > 30.000)
			{
				checkRenge.strMessage = _T("Target Range is from 5.000mm to 30.000mm.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}

			if (dblToleranceAll<0.003 || dblToleranceAll > 11.000)
			{
				checkRenge.strMessage = _T("Tolerance Range is from 0.003mm to 11.000mm.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}
		}
		else
		{
			if (dblTarget1<5.000 || dblTarget1 > 30.000)
			{
				checkRenge.strMessage = _T("Gap1 Target Range is from 5.000mm to 30.000mm.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}
			if (dblTarget2<5.000 || dblTarget2 > 30.000)
			{
				checkRenge.strMessage = _T("Gap2 Target Range is from 5.000mm to 30.000mm.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}

			if (dblTarget3<5.000 || dblTarget3 > 30.000)
			{
				checkRenge.strMessage = _T("Gap3 Target Range is from 5.000mm to 30.000mm.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}


			if (dblTolerance1<0.003 || dblTolerance1 > 11.000)
			{
				checkRenge.strMessage = _T("Gap1 Tolerance Range is from 0.003mm to 11.000mm.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}

			if (dblTolerance2<0.003 || dblTolerance2 > 11.000)
			{
				checkRenge.strMessage = _T("Gap2 Tolerance Range is from 0.003mm to 11.000mm.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}

			if (dblTolerance3<0.003 || dblTolerance3 > 11.000)
			{
				checkRenge.strMessage = _T("Gap3 Tolerance Range is from 0.003mm to 11.000mm.");
				checkRenge.bRangeOver = TRUE;
				return checkRenge;
			}

		}
	}

	if (m_bAllGapSameSettings) // Same Target, Tolerance
	{
		if ((dblTargetAll + dblToleranceAll) > (dblTargetAll + dblViewRange))
		{
			checkRenge.strMessage = _T("The range of the View Range is incorrect.");
			checkRenge.bRangeOver = TRUE;
			return checkRenge;
		}
	}
	else
	{
		if ((dblTarget1 + dblTolerance1) > (dblTarget1 + dblViewRange))
		{
			checkRenge.strMessage = _T("The range of the View Range is incorrect.");
			checkRenge.bRangeOver = TRUE;
			return checkRenge;
		}
		else if ((dblTarget2 + dblTolerance2) > (dblTarget2 + dblViewRange))
		{
			checkRenge.strMessage = _T("The range of the View Range is incorrect.");
			checkRenge.bRangeOver = TRUE;
			return checkRenge;
		}
			
		else if ((dblTarget3 + dblTolerance3) > (dblTarget3 + dblViewRange))
		{
			checkRenge.strMessage = _T("The range of the View Range is incorrect.");
			checkRenge.bRangeOver = TRUE;
			return checkRenge;
		}
		
	}
	return checkRenge;
}


CString CSetGapCriteria::checkEachRange()
{
	CString strViewRange;
	GetDlgItemText(IDC_EDIT_VIEW_RANGE, strViewRange);

	double dblViewRange = _wtof(strViewRange);

	double dblTarget1, dblTarget2, dblTarget3, dblTargetAll;
	double dblTolerance1, dblTolerance2, dblTolerance3, dblToleranceAll;

	dblTarget1 = _wtof(m_strGap1Target);
	dblTarget2 = _wtof(m_strGap2Target);
	dblTarget3 = _wtof(m_strGap3Target);
	dblTargetAll = _wtof(m_strAllTarget);
	dblTolerance1 = _wtof(m_strGap1Tolerance);
	dblTolerance2 = _wtof(m_strGap2Tolerance);
	dblTolerance3 = _wtof(m_strGap3Tolerance);
	dblToleranceAll = _wtof(m_strALLTolerance);


	
	return CString();
}
