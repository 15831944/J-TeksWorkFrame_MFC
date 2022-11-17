#pragma once
#include "ExcelReport.h"

// CReportDlg 대화 상자입니다.

class CReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReportDlg)

public:
	CReportDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CReportDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnload();
	afx_msg void OnBnClickedBtnDataapply();
	CExcelReport m_ExcelReport;
	void Initialize();
	virtual BOOL OnInitDialog();
	int m_nReportData;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
