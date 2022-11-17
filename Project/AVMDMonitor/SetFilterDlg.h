#pragma once
#include "afxwin.h"


// CSetFilterDlg 대화 상자입니다.

class CSetFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetFilterDlg)

public:
	CSetFilterDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetFilterDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SET_FILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_combo_FilterType;
	CEdit m_edit_lowValue;
	CEdit m_edit_highValue;
	afx_msg void OnBnClickedOk();
	BOOL m_bIsFilterEnable;
	afx_msg void OnBnClickedChkEnableFilter();
	afx_msg void OnCbnSelchangeCbFiltername();
	int SetFilterSetting(CString strFilterType);
	afx_msg void OnBnClickedCancel();
};
