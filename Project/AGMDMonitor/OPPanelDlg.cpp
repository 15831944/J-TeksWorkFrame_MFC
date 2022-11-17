// OPPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "OPPanelDlg.h"
#include "afxdialogex.h"
#include "Status.h"

// COPPanelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COPPanelDlg, CDialogEx)

COPPanelDlg::COPPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPPANELDLG, pParent)
	, m_strGap1Offset(_T(""))
	, m_strGap2Offset(_T(""))
	, m_strGap3Offset(_T(""))
	, m_strOpGapUnit1(_T(""))
	, m_strOpGapUnit2(_T(""))
	, m_strOpGapUnit3(_T(""))
	, m_iLastUnitMode(0)
{
	CStatus::Instance()->SetOPPanelDlg(this);
	m_iLastUnitMode = CStatus::Instance()->GetUnitMode();
}

COPPanelDlg::~COPPanelDlg()
{
}

void COPPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OFFSET_GAP1, m_strGap1Offset);
	DDX_Text(pDX, IDC_EDIT_OFFSET_GAP2, m_strGap2Offset);
	DDX_Text(pDX, IDC_EDIT_OFFSET_GAP3, m_strGap3Offset);
	DDX_Text(pDX, IDC_STATIC_1, m_strOpGapUnit1);
	DDX_Text(pDX, IDC_STATIC_2, m_strOpGapUnit2);
	DDX_Text(pDX, IDC_STATIC_3, m_strOpGapUnit3);
}


BEGIN_MESSAGE_MAP(COPPanelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_NONE, &COPPanelDlg::OnBnClickedRadioNone)
	ON_BN_CLICKED(IDC_RADIO_OFFSET, &COPPanelDlg::OnBnClickedRadioOffset)
	ON_BN_CLICKED(IDC_BTN_APPLY, &COPPanelDlg::OnBnClickedBtnApply)
	ON_MESSAGE(WM_UNITUPDATE, &COPPanelDlg::OnDlgUnitUpdate)
	ON_MESSAGE(WM_UIUPDATE, &COPPanelDlg::OnOffsetChangeUpdate)
	ON_MESSAGE(WM_INITIALIZE, &COPPanelDlg::OnInitOffset)
	ON_MESSAGE(WM_DLG_INITIALIZE, &COPPanelDlg::OnDlgInit)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// COPPanelDlg 메시지 처리기입니다.


BOOL COPPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	

	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_NONE);
		pCheck->SetCheck(1);

		CButton* pCheck2 = (CButton*)GetDlgItem(IDC_RADIO_OFFSET);
		pCheck2->SetCheck(0);

		ControlDisable(TRUE);

		CStatus::Instance()->m_curMeasureData.usGap1 = 0;
		CStatus::Instance()->m_curMeasureData.usGap2 = 0;
		CStatus::Instance()->m_curMeasureData.usGap3 = 0;
		m_strGap1Offset.Format(_T("0.000"));
		m_strGap2Offset.Format(_T("0.000"));
		m_strGap3Offset.Format(_T("0.000"));
	
	
	}
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void COPPanelDlg::OnBnClickedRadioNone()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		if (CStatus::Instance()->GetIsOffsetMode())
		{
			CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_NONE);
			pCheck->SetCheck(0);
			CButton* pCheck2 = (CButton*)GetDlgItem(IDC_RADIO_OFFSET);
			pCheck2->SetCheck(1);
			ControlDisable(TRUE);
		}
		else
		{
			CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_NONE);
			pCheck->SetCheck(1);
			CButton* pCheck2 = (CButton*)GetDlgItem(IDC_RADIO_OFFSET);
			pCheck2->SetCheck(0);
			ControlDisable(TRUE);
		}
		return;
	}
		
		
	CStatus::Instance()->SetOffsetMode(FALSE);
	CStatus::Instance()->SetDefault();
	ControlDisable(TRUE);
}

void COPPanelDlg::OnBnClickedRadioOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		if (CStatus::Instance()->GetIsOffsetMode())
		{
			CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_NONE);
			pCheck->SetCheck(0);
			CButton* pCheck2 = (CButton*)GetDlgItem(IDC_RADIO_OFFSET);
			pCheck2->SetCheck(1);
			ControlDisable(TRUE);
		}
		else
		{
			CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_NONE);
			pCheck->SetCheck(1);
			CButton* pCheck2 = (CButton*)GetDlgItem(IDC_RADIO_OFFSET);
			pCheck2->SetCheck(0);
			ControlDisable(TRUE);
		}
		return;
	}
		
	CStatus::Instance()->SetOffsetMode(TRUE);
	CStatus::Instance()->SetDefault();
	ControlDisable(FALSE);
}

void COPPanelDlg::ControlDisable(BOOL bIsDisable)
{
	if (bIsDisable)
	{
		GetDlgItem(IDC_EDIT_OFFSET_GAP1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_GAP2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_GAP3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_OFFSET_GAP1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OFFSET_GAP2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OFFSET_GAP3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);
	}
}

void COPPanelDlg::OnBnClickedBtnApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	double dblOffsetNum = 0;
	double dblOffset1, dblOffset2, dblOffset3;
	dblOffset1 = _wtof(m_strGap1Offset);
	dblOffset2 = _wtof(m_strGap2Offset);
	dblOffset3 = _wtof(m_strGap3Offset);

	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
	{
		if (dblOffset1 > 1.1811 || dblOffset2 > 1.1811 || dblOffset3 > 1.1811)
		{
			AfxMessageBox(_T("The offset value must be less than 1.1811 inches."));
			return;
		}
	}
	else
	{
		if (dblOffset1 > 30. || dblOffset2 > 30. || dblOffset3 > 30.)
		{
			AfxMessageBox(_T("The offset value must be less than 30 mm."));
			return;
		}
	}
	dblOffsetNum = _wtof(m_strGap1Offset);
	CStatus::Instance()->SetOffset(0, dblOffsetNum);
	dblOffsetNum = _wtof(m_strGap2Offset);
	CStatus::Instance()->SetOffset(1, dblOffsetNum);
	dblOffsetNum = _wtof(m_strGap3Offset);
	CStatus::Instance()->SetOffset(2, dblOffsetNum);

	CStatus::Instance()->SetDefault();
	UpdateData(FALSE);
}

void COPPanelDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}

LRESULT COPPanelDlg::OnDlgUnitUpdate(WPARAM wParam, LPARAM lParam)
{
	UnitUpdate();
	return 0;
}

LRESULT COPPanelDlg::OnOffsetChangeUpdate(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	ControlDisable(FALSE);
	int iCurUnitMode = CStatus::Instance()->GetUnitMode();
	CString strTemp;
	double dblCalcNum = 0;
	
	GetDlgItemText(IDC_EDIT_OFFSET_GAP1, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iLastUnitMode);
	m_strGap1Offset.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, iCurUnitMode));
	GetDlgItemText(IDC_EDIT_OFFSET_GAP2, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iLastUnitMode);
	m_strGap2Offset.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, iCurUnitMode));

	GetDlgItemText(IDC_EDIT_OFFSET_GAP3, strTemp);
	dblCalcNum = CalcValueToOriginalValue(strTemp, m_iLastUnitMode);
	m_strGap3Offset.Format(_T("%.3f"), OriginalValueToCalcValue(dblCalcNum, iCurUnitMode));

	m_iLastUnitMode = iCurUnitMode;

	UpdateData(FALSE);
	return 0;
}

LRESULT COPPanelDlg::OnInitOffset(WPARAM wParam, LPARAM lParam)
{
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
	{
		if (CStatus::Instance()->GetIsOffsetMode())
		{
			CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_NONE);
			pCheck->SetCheck(0);
			CButton* pCheck2 = (CButton*)GetDlgItem(IDC_RADIO_OFFSET);
			pCheck2->SetCheck(1);
			ControlDisable(FALSE);
		}
		else
		{
			CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_NONE);
			pCheck->SetCheck(1);
			CButton* pCheck2 = (CButton*)GetDlgItem(IDC_RADIO_OFFSET);
			pCheck2->SetCheck(0);
			ControlDisable(TRUE);
		}
	}
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		if (CStatus::Instance()->GetIsOffsetMode())
		{
			CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_NONE);
			pCheck->SetCheck(0);
			CButton* pCheck2 = (CButton*)GetDlgItem(IDC_RADIO_OFFSET);
			pCheck2->SetCheck(1);
			ControlDisable(TRUE);
		}
		else
		{
			CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_NONE);
			pCheck->SetCheck(1);
			CButton* pCheck2 = (CButton*)GetDlgItem(IDC_RADIO_OFFSET);
			pCheck2->SetCheck(0);
			ControlDisable(TRUE);
		}
	}
	double dblGap1Offset, dblGap2Offset, dblGap3Offset;
	dblGap1Offset = (double)CStatus::Instance()->GetOffset(0);
	dblGap2Offset = (double)CStatus::Instance()->GetOffset(1);
	dblGap3Offset = (double)CStatus::Instance()->GetOffset(2);
	m_strGap1Offset.Format(_T("%.3f"), dblGap1Offset);
	m_strGap2Offset.Format(_T("%.3f"), dblGap2Offset);
	m_strGap3Offset.Format(_T("%.3f"), dblGap3Offset);
	UpdateData(FALSE);
	return 0;
}

double COPPanelDlg::CalcValueToOriginalValue(CString strOffsetValue, int iGapUnit)
{
	double dblOffsetValue;
	double dblOrgOffsetValue;

	dblOffsetValue = _wtof(strOffsetValue);
	if (iGapUnit == CStatus::tUnitMode::UNIT_MM)
		dblOrgOffsetValue = dblOffsetValue * UNIT_MM_VALUE;
	if (iGapUnit == CStatus::tUnitMode::UNIT_INCH)
		dblOrgOffsetValue = dblOffsetValue * UNIT_MM_VALUE / UNIT_INCH_VALUE;

	return dblOrgOffsetValue;
}

double COPPanelDlg::OriginalValueToCalcValue(double dblGapValue, int iGapUnit)
{
	double dbCalcValue;
	if (iGapUnit == CStatus::tUnitMode::UNIT_MM)
		dbCalcValue = dblGapValue / UNIT_MM_VALUE;
	if (iGapUnit == CStatus::tUnitMode::UNIT_INCH)
		dbCalcValue = dblGapValue / UNIT_MM_VALUE * UNIT_INCH_VALUE; //  0.03937 : mm -> inche
	return dbCalcValue;
}

void COPPanelDlg::UnitUpdate()
{
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
	{
		m_strOpGapUnit1 = _T("mm");
		m_strOpGapUnit2 = _T("mm");
		m_strOpGapUnit3 = _T("mm");
	}
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
	{
		m_strOpGapUnit1 = _T("inches");
		m_strOpGapUnit2 = _T("inches");
		m_strOpGapUnit3 = _T("inches");
	}
	UpdateData(FALSE);
}


BOOL COPPanelDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN))
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT COPPanelDlg::OnDlgInit(WPARAM wParam, LPARAM lParam)
{
	CheckDlgButton(IDC_RADIO_NONE, TRUE);
	CheckDlgButton(IDC_RADIO_OFFSET, FALSE);
	return 0;
}