// StabilizationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "StabilizationDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "Stabilization.h"

#define STABLE_LOW 0.002
#define STABLE_HIGH 1.000
#define STR_STABLE_RANGE _T("Enter the value from 0.002 to 1.000")

// CStabilizationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStabilizationDlg, CDialogEx)

CStabilizationDlg::CStabilizationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STABILIZATION, pParent)
	, m_dblStblzSensitivity(0)
	, m_dblSensitivity(0)
{

}

CStabilizationDlg::~CStabilizationDlg()
{
}

void CStabilizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_SESITIVITY, m_cStblzSensitivity);
	DDX_Text(pDX, IDC_EDT_SESITIVITY, m_dblSensitivity);
}


BEGIN_MESSAGE_MAP(CStabilizationDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStabilizationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_STBLZDEFAULT, &CStabilizationDlg::OnBnClickedBtnStblzdefault)
END_MESSAGE_MAP()


// CStabilizationDlg 메시지 처리기입니다.


BOOL CStabilizationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_dblSensitivity = CStatus::Instance()->GetStblzSensitivity();
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CStabilizationDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!IsInTheRange())return;
	UpdateData(true);
	CStatus::Instance()->SetStblzSensitivity(m_dblSensitivity);
	CDialogEx::OnOK();
}


void CStabilizationDlg::OnBnClickedBtnStblzdefault()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_dblSensitivity = (CStabilization::Instance()->GetDefaultSensitivity()/1000.);
	UpdateData(false);
}


BOOL CStabilizationDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			if (pMsg->hwnd == GetDlgItem(IDC_EDT_SESITIVITY)->GetSafeHwnd())
			{
				StableEdtChange();
			}
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CStabilizationDlg::IsInTheRange()
{
	CString strNum;
	GetDlgItemText(IDC_EDT_TOL, strNum);
	double dblNum = _wtof(strNum.GetBuffer());
	if ((dblNum<STABLE_LOW) || (dblNum>STABLE_HIGH))
	{
		AfxMessageBox(STR_STABLE_RANGE);
		return FALSE;
	}
	return TRUE;
}


BOOL CStabilizationDlg::StableEdtChange()
{
	if (!IsInTheRange()) return FALSE;
	UpdateData(true);
	UpdateData(false);
	return TRUE;
}
