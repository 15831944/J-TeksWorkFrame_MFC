#pragma once
#include "afxwin.h"


// CWarningSetDlg 대화 상자입니다.

class CWarningSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWarningSetDlg)

public:
	CWarningSetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWarningSetDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WARN_SET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_cSampleTimeSelect;
	double m_dblTemp;
	BOOL m_bUseTemp;
	BOOL m_bUseP01;
	BOOL m_bUseP03;
	BOOL m_bUseP05;
	BOOL m_bUseWarning;
	virtual BOOL OnInitDialog();
	int m_iThresP01;
	int m_iThresP03;
	int m_iThresP05;
};
