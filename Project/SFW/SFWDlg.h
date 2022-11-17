
// SFWDlg.h : 헤더 파일
//

#pragma once
#include "CamViewDlg.h"
#include "TabContainerDlg.h"
#include "MapDlg.h"
#include "InfoPanelDlg.h"
#include "XrayPanelDlg.h"


// CSFWDlg 대화 상자
class CSFWDlg : public CDialogEx
{
// 생성입니다.
public:
	CSFWDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SFW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int MakeChildWnd();
	CCamViewDlg *m_pCamViewDlg;
	CTabContainerDlg *m_pTabContainerDlg;
	CMapDlg *m_pMapDlg;
	CInfoPanelDlg *m_pInfoPanelDlg;
	CXrayPanelDlg *m_pXrayPanelDlg;
};
