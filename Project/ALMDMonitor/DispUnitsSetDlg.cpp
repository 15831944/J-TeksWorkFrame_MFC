// DispUnitsSetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "DispUnitsSetDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "UnitConversion.h"

#define ROUNDING(x, dig)    ( floor((x) * pow(float(10), dig) + 0.5f) / pow(float(10), dig) )

// CDispUnitsSetDlg 대화 상자입니다.
#define TOLERANCE_LOW 0.03
#define TOLERANCE_HIGH 5.0
IMPLEMENT_DYNAMIC(CDispUnitsSetDlg, CDialogEx)

CDispUnitsSetDlg::CDispUnitsSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DISPUNITSET, pParent)
	, m_iCoordinateSystem(0)
	, m_iInclinationType(CStatus::IT_ANGLE)
	, m_dblGoNoGoDegree(0)
	, m_strGoNoGoUnit(_T(""))
	, m_strDispTolerance(_T(""))
	, m_dblDispTolerance(0)
	, m_strGoNoGoRange(_T(""))
	, m_dblToleranceLow(0)
	, m_dblToleranceHigh(0)
{

}

CDispUnitsSetDlg::~CDispUnitsSetDlg()
{
}

void CDispUnitsSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RDISPTYPE, m_iCoordinateSystem);
	DDX_Radio(pDX, IDC_RDISPUNIT, m_iInclinationType);
	DDX_Control(pDX, IDC_CB_KOFANGLE, m_cKofAngle);
	DDX_Control(pDX, IDC_CB_KOFROR, m_cKofROR);
	DDX_Text(pDX, IDC_EDT_TOL, m_strDispTolerance);
	DDX_Text(pDX, IDC_LGONOGOUNIT, m_strGoNoGoUnit);
	DDX_Text(pDX, IDC_LGONOGO, m_strGoNoGoRange);
	DDX_Control(pDX, IDC_EDT_TOL, m_cTolValue);
}


BEGIN_MESSAGE_MAP(CDispUnitsSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDispUnitsSetDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CB_KOFANGLE, &CDispUnitsSetDlg::OnCbnSelchangeCbKofangle)
	ON_CBN_SELCHANGE(IDC_CB_KOFROR, &CDispUnitsSetDlg::OnCbnSelchangeCbKofror)
	ON_BN_CLICKED(IDC_RDISPTYPE, &CDispUnitsSetDlg::OnBnClickedRdisptype)
	ON_BN_CLICKED(IDC_RDISPUNIT, &CDispUnitsSetDlg::OnBnClickedRdispunit)
	ON_BN_CLICKED(IDC_RROR, &CDispUnitsSetDlg::OnBnClickedRror)
	ON_BN_CLICKED(IDC_RMAGDIR, &CDispUnitsSetDlg::OnBnClickedRmagdir)
	ON_EN_UPDATE(IDC_EDT_TOL, &CDispUnitsSetDlg::OnEnUpdateEdtTol)
END_MESSAGE_MAP()


// CDispUnitsSetDlg 메시지 처리기입니다.


BOOL CDispUnitsSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_iCoordinateSystem = CStatus::Instance()->GetCoordinateSystem();
	m_iInclinationType = CStatus::Instance()->GetInclinationType();
	m_cKofAngle.AddString(_T("Degrees"));
	m_cKofAngle.AddString(_T("MilRad"));
	m_cKofROR.AddString(_T("mm(diameter)"));
	m_cKofROR.AddString(_T("mm(radius)"));
	m_cKofAngle.SetCurSel(CStatus::Instance()->GetAngleType());
	m_cKofROR.SetCurSel(CStatus::Instance()->GetRORType());

	m_dblGoNoGoDegree = CStatus::Instance()->GetTolerance();
	GetGoNoGoDispInitData();
	SetGoNoGoLabel();
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDispUnitsSetDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if (GoNoGoEdtChange() == FALSE)return;
	CStatus::Instance()->SetCoordinateSystem( (CStatus::tCoordinateSystem) m_iCoordinateSystem);
	CStatus::Instance()->SetInclinationType( (CStatus::tIncType) m_iInclinationType);

	switch (m_cKofAngle.GetCurSel())
	{
	case 0:
		CStatus::Instance()->SetAngleType(CStatus::AT_DEGREE);
		break;
	case 1:
		CStatus::Instance()->SetAngleType(CStatus::AT_RADIAN);
		break;
	}
	switch (m_cKofROR.GetCurSel())
	{
	case 0:
		CStatus::Instance()->SetRORType(CStatus::RT_MMD);
		break;
	case 1:
		CStatus::Instance()->SetRORType(CStatus::RT_MMR);
		break;
	}
	UpdateData(true);
	CStatus::Instance()->SetTolerance(m_dblGoNoGoDegree);

	CDialogEx::OnOK();
}


void CDispUnitsSetDlg::OnCbnSelchangeCbKofangle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GoNoGoDispUpdate();
}


void CDispUnitsSetDlg::OnCbnSelchangeCbKofror()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GoNoGoDispUpdate();
}


void CDispUnitsSetDlg::OnBnClickedRdisptype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GoNoGoDispUpdate();

}

void CDispUnitsSetDlg::OnBnClickedRdispunit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GoNoGoDispUpdate();
}

void CDispUnitsSetDlg::OnBnClickedRror()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GoNoGoDispUpdate();
}

void CDispUnitsSetDlg::OnBnClickedRmagdir()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GoNoGoDispUpdate();
}


void CDispUnitsSetDlg::GoNoGoDispUpdate()
{
	UpdateData(TRUE);
	GetDispData();
	SetGoNoGoLabel();
	UpdateData(false);
}


void CDispUnitsSetDlg::GetDispData()
{
	CStatus::tAngleType AngleType = CStatus::AT_DEGREE;
	switch (m_cKofAngle.GetCurSel())
	{
	case 0:
		AngleType = CStatus::AT_DEGREE;
		break;
	case 1:
		AngleType = CStatus::AT_RADIAN;
		break;
	}

	CStatus::tRORType RORType = CStatus::RT_MMD;
	switch (m_cKofROR.GetCurSel())
	{
	case 0:
		RORType = CStatus::RT_MMD;
		break;
	case 1:
		RORType = CStatus::RT_MMR;
		break;
	}

	double dispGoNoGoTolerance = _wtof(m_strDispTolerance.GetBuffer());
	double dblGoNoGoDegree;
	if (dispGoNoGoTolerance != m_dblDispTolerance)
	{
		CUnitConversion::Instance()->ConvertToDegree((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &dblGoNoGoDegree, dispGoNoGoTolerance);
		if (dblGoNoGoDegree != m_dblGoNoGoDegree)
		{
		}
	}

	CUnitConversion::Instance()->ConvertToDest((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &m_strGoNoGoUnit, &m_dblDispTolerance, m_dblGoNoGoDegree);
	if(CStatus::Instance()->IsDigit2Mode())
		m_strDispTolerance.Format(_T("%5.2f"), m_dblDispTolerance);
	else
		m_strDispTolerance.Format(_T("%5.3f"), m_dblDispTolerance);
}


void CDispUnitsSetDlg::GetGoNoGoDispInitData()
{
	CStatus::tAngleType AngleType = CStatus::AT_DEGREE;
	switch (m_cKofAngle.GetCurSel())
	{
	case 0:
		AngleType = CStatus::AT_DEGREE;
		break;
	case 1:
		AngleType = CStatus::AT_RADIAN;
		break;
	}

	CStatus::tRORType RORType = CStatus::RT_MMD;
	switch (m_cKofROR.GetCurSel())
	{
	case 0:
		RORType = CStatus::RT_MMD;
		break;
	case 1:
		RORType = CStatus::RT_MMR;
		break;
	}

	CUnitConversion::Instance()->ConvertToDest((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &m_strGoNoGoUnit, &m_dblDispTolerance, m_dblGoNoGoDegree);
	if(CStatus::Instance()->IsDigit2Mode())
		m_strDispTolerance.Format(_T("%5.2f"), m_dblDispTolerance);
	else
		m_strDispTolerance.Format(_T("%5.3f"), m_dblDispTolerance);
}


BOOL CDispUnitsSetDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			if (pMsg->hwnd == GetDlgItem(IDC_EDT_TOL)->GetSafeHwnd())
			{
				GoNoGoEdtChange();
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDispUnitsSetDlg::GoNoGoEdtChange()
{
	if (!IsInTheTolRange()) return FALSE;
	UpdateData(true);
	GetDispGoNoGo();
	UpdateData(false);
	return TRUE;
}


void CDispUnitsSetDlg::GetDispGoNoGo()
{
	CStatus::tAngleType AngleType = CStatus::AT_DEGREE;
	switch (m_cKofAngle.GetCurSel())
	{
	case 0:
		AngleType = CStatus::AT_DEGREE;
		break;
	case 1:
		AngleType = CStatus::AT_RADIAN;
		break;
	}

	CStatus::tRORType RORType = CStatus::RT_MMD;
	switch (m_cKofROR.GetCurSel())
	{
	case 0:
		RORType = CStatus::RT_MMD;
		break;
	case 1:
		RORType = CStatus::RT_MMR;
		break;
	}

	double dispGoNoGoTolerance = _wtof(m_strDispTolerance.GetBuffer());
	double dblGoNoGoDegree;
	CUnitConversion::Instance()->ConvertToDegree((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &dblGoNoGoDegree, dispGoNoGoTolerance);
	if (dblGoNoGoDegree != m_dblGoNoGoDegree)
	{
			m_dblGoNoGoDegree = dblGoNoGoDegree;
	}
}


void CDispUnitsSetDlg::SetGoNoGoLabel()
{
	CStatus::tAngleType AngleType = CStatus::AT_DEGREE;
	switch (m_cKofAngle.GetCurSel())
	{
	case 0:
		AngleType = CStatus::AT_DEGREE;
		break;
	case 1:
		AngleType = CStatus::AT_RADIAN;
		break;
	}

	CStatus::tRORType RORType = CStatus::RT_MMD;
	switch (m_cKofROR.GetCurSel())
	{
	case 0:
		RORType = CStatus::RT_MMD;
		break;
	case 1:
		RORType = CStatus::RT_MMR;
		break;
	}


	CUnitConversion::Instance()->ConvertToDest((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &m_strGoNoGoUnit, &m_dblToleranceLow, TOLERANCE_LOW);
	CUnitConversion::Instance()->ConvertToDest((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &m_strGoNoGoUnit, &m_dblToleranceHigh, TOLERANCE_HIGH);

	if (CStatus::Instance()->IsDigit2Mode())
	{
		m_strDispTolerance.Format(_T("%5.2f"), m_dblDispTolerance);
		m_strGoNoGoRange.Format(_T("Enter the value from %5.2f to %5.2f"), m_dblToleranceLow, m_dblToleranceHigh);
	}
	else
	{
		m_strDispTolerance.Format(_T("%5.3f"), m_dblDispTolerance);
		m_strGoNoGoRange.Format(_T("Enter the value from %5.3f to %5.3f"), m_dblToleranceLow, m_dblToleranceHigh);
	}
}


BOOL CDispUnitsSetDlg::IsInTheTolRange()
{
	CString strNum;
	GetDlgItemText(IDC_EDT_TOL, strNum);
	double dblNum = _wtof(strNum.GetBuffer());
	if ((dblNum<(ROUNDING(m_dblToleranceLow, 4))) || (dblNum>(ROUNDING(m_dblToleranceHigh, 4))))
	{
		AfxMessageBox(m_strGoNoGoRange);
		return FALSE;
	}
	return TRUE;
}


void CDispUnitsSetDlg::OnEnUpdateEdtTol()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
