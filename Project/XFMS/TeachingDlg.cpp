// TeachingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XFMS.h"
#include "XFMSDoc.h"
#include "XFMSView.h"
#include "MainFrm.h"
#include "TeachingDlg.h"
#include "CamDlg.h"
#include "afxdialogex.h"
#include "Status.h"


// CTeachingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTeachingDlg, CDialogEx)

CTeachingDlg::CTeachingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEACHING, pParent)
	, m_iTeachingMode(0)
{

}

CTeachingDlg::~CTeachingDlg()
{
}

void CTeachingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_MODESETFID, m_iTeachingMode);
	DDX_Control(pDX, IDC_RETOPMODE, m_cOPMode);
	DDX_Control(pDX, IDC_BTN_TRIAL, m_cTrial);
}


BEGIN_MESSAGE_MAP(CTeachingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RETOPMODE, &CTeachingDlg::OnBnClickedRetopmode)
	ON_BN_CLICKED(IDC_MODESETFID, &CTeachingDlg::OnBnClickedModes)
	ON_BN_CLICKED(IDC_MODESETHT, &CTeachingDlg::OnBnClickedModes)
	ON_BN_CLICKED(IDC_MODEALIGN, &CTeachingDlg::OnBnClickedModes)
	ON_BN_CLICKED(IDC_MODEHEIGHT, &CTeachingDlg::OnBnClickedModes)
	ON_BN_CLICKED(IDC_MODEGAPGAGE, &CTeachingDlg::OnBnClickedModes)
	ON_BN_CLICKED(IDC_BTN_TRIAL, &CTeachingDlg::OnBnClickedBtnTrial)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTeachingDlg 메시지 처리기입니다.


void CTeachingDlg::OnBnClickedRetopmode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	pView->SetOPMode(CStatus::OPSTOP);


}

BOOL CTeachingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_iTeachingMode = 0;
	SetWindowTheme(GetDlgItem(IDC_MODESETFID)->m_hWnd, _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_MODESETHT)->m_hWnd, _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_MODEALIGN)->m_hWnd, _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_MODEHEIGHT)->m_hWnd, _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_MODEGAPGAGE)->m_hWnd, _T(""), _T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTeachingDlg::OnBnClickedModes()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CStatus::Instance()->SetCurAddAlgoIndex(m_iTeachingMode);
}


void CTeachingDlg::OnBnClickedBtnTrial()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	pView->DoTrial();

}



BOOL CTeachingDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH CTeachingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (nCtlColor)
	{
	case CTLCOLOR_BTN:    // 버튼의 배경색을 투명으로...
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(90, 107, 135));  // static text 글자색 변경
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
