#pragma once
#include "afxcmn.h"


// CTabContainerDlg 대화 상자입니다.

class CTabContainerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTabContainerDlg)

public:
	CTabContainerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabContainerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TABCONTAINER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabPanelContainer;
	virtual BOOL OnInitDialog();
};
