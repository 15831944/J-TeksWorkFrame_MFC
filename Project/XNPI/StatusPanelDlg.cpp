// StatusPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "StatusPanelDlg.h"
#include "afxdialogex.h"
#include "DetailInfoDlg.h"
#include "Status.h"


// CStatusPanelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStatusPanelDlg, CDialogEx)

CStatusPanelDlg::CStatusPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STSPANEL, pParent)
	, m_pDetailInfoDlg(NULL)
{

}

CStatusPanelDlg::~CStatusPanelDlg()
{
}

void CStatusPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStatusPanelDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CStatusPanelDlg 메시지 처리기입니다.


BOOL CStatusPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect DetailInfoRect;
	GetClientRect(&DetailInfoRect);
	m_pDetailInfoDlg = new CDetailInfoDlg(this);
	m_pDetailInfoDlg->Create(CDetailInfoDlg::IDD, this);
	m_pDetailInfoDlg->GetClientRect(&DetailInfoRect);
	m_pDetailInfoDlg->ShowWindow(SW_SHOW);
	SetTimer(TIMEREVENT_DISPMOUSEPOSINFO, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CStatusPanelDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TIMEREVENT_DISPMOUSEPOSINFO:
		CStatus::Instance()->m_infoCamMousePos;
		SetDlgItemInt(IDC_DISPCAMNO, CStatus::Instance()->m_infoCamMousePos.nCamNo);
		SetDlgItemInt(IDC_DISPXPOS, (int)(CStatus::Instance()->m_infoCamMousePos.posImage.dblPosX));
		SetDlgItemInt(IDC_DISPYPOS, (int)(CStatus::Instance()->m_infoCamMousePos.posImage.dblPosY));
		SetDlgItemInt(IDC_DISPVALUE, CStatus::Instance()->m_infoCamMousePos.GrayValue);
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CStatusPanelDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}
