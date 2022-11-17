#pragma once
#include "afxwin.h"


// CSetTimeDomainFormatDlg 대화 상자입니다.

class CSetTimeDomainFormatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetTimeDomainFormatDlg)

public:
	CSetTimeDomainFormatDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetTimeDomainFormatDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SET_TIME_DOMAIN_FORMAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_ctrl_combo_TimeDomain;
	afx_msg void OnBnClickedOk();
};
