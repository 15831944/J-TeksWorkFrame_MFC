#pragma once
#include "NumEdit.h"


// CStabilizationDlg 대화 상자입니다.

class CStabilizationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStabilizationDlg)

public:
	CStabilizationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStabilizationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STABILIZATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	double m_dblStblzSensitivity;
	CNumEdit m_cStblzSensitivity;
	double m_dblSensitivity;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnStblzdefault();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsInTheRange();
	BOOL StableEdtChange();
};
