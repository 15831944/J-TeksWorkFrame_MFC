// StationInformationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "StationInformationDlg.h"
#include "afxdialogex.h"
#include "Repository.h"
#include "Status.h"

// CStationInformationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStationInformationDlg, CDialogEx)

CStationInformationDlg::CStationInformationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STATION_INFORMATION, pParent)
	, m_strOperator(_T(""))
	, m_strTool(_T(""))
	, m_strStation(_T(""))
	, m_strComment(_T(""))
{

}

CStationInformationDlg::~CStationInformationDlg()
{
}

void CStationInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_OP, m_strOperator);
	DDX_Text(pDX, IDC_EDT_TOOL, m_strTool);
	DDX_Text(pDX, IDC_EDT_STATION, m_strStation);
	DDX_Text(pDX, IDC_EDT_COMMENT, m_strComment);
}


BEGIN_MESSAGE_MAP(CStationInformationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CStationInformationDlg::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CStationInformationDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CStationInformationDlg 메시지 처리기입니다.


BOOL CStationInformationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ShowWindow(SW_SHOW);
	GetWorkEnvData();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CStationInformationDlg::OnBnClickedBtnApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// View 에서만 사용하기 때문에 m_WorkEnvInfo[0]
	UpdateData();
	tWorkEnvInfo *pWorkEnvInfo = new tWorkEnvInfo;
	if (CStatus::Instance()->GetRepository()->m_WorkEnvInfo[0].size())
	{
		pWorkEnvInfo = CStatus::Instance()->GetRepository()->m_WorkEnvInfo[0].at(0);
	}else
	{
		pWorkEnvInfo = new tWorkEnvInfo;
		CStatus::Instance()->GetRepository()->m_WorkEnvInfo[0].push_back(pWorkEnvInfo);
	}

	pWorkEnvInfo->iIndex = -1;
	wsprintf(pWorkEnvInfo->szOperator, m_strOperator.GetBuffer());
	wsprintf(pWorkEnvInfo->szTool, m_strTool.GetBuffer());
	wsprintf(pWorkEnvInfo->szStation, m_strStation.GetBuffer());
	wsprintf(pWorkEnvInfo->szComment, m_strComment.GetBuffer());
	CDialogEx::OnOK();
}


void CStationInformationDlg::OnBnClickedBtnClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_strOperator.Empty();
	m_strTool.Empty();
	m_strStation.Empty();
	m_strComment.Empty();
	UpdateData(FALSE);
}


void CStationInformationDlg::GetWorkEnvData()
{
	tWorkEnvInfo *pWorkEnvInfo = new tWorkEnvInfo;
	if (CStatus::Instance()->GetRepository()->m_WorkEnvInfo[0].size())
	{
		pWorkEnvInfo = CStatus::Instance()->GetRepository()->m_WorkEnvInfo[0].at(0);
	}
	else
	{
		return;
	}
	m_strOperator = pWorkEnvInfo->szOperator;
	m_strTool = pWorkEnvInfo->szTool;
	m_strStation = pWorkEnvInfo->szStation;
	m_strComment = pWorkEnvInfo->szComment;
	UpdateData(FALSE);
}


BOOL CStationInformationDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
