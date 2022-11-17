#pragma once
#include "afxcmn.h"


// CInfoPanelDlg 대화 상자입니다.

class CInfoPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoPanelDlg)

public:
	CInfoPanelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInfoPanelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INFOPANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_tabInfoSel;
};
