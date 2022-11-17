#pragma once
#include "afxwin.h"

// CDispUnitsSetDlg 대화 상자입니다.
#include "Status.h"
#include "NumEdit.h"

class CDispUnitsSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDispUnitsSetDlg)

public:
	CDispUnitsSetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDispUnitsSetDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISPUNITSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_iCoordinateSystem;
	int m_iInclinationType;
	virtual BOOL OnInitDialog();
	CComboBox m_cKofAngle;
	CComboBox m_cKofROR;
	afx_msg void OnBnClickedOk();
	double m_dblGoNoGoDegree;
	CString m_strDispTolerance;
	afx_msg void OnCbnSelchangeCbKofangle();
	afx_msg void OnCbnSelchangeCbKofror();
	CString m_strGoNoGoUnit;
	afx_msg void OnBnClickedRdisptype();
	afx_msg void OnBnClickedRdispunit();
	afx_msg void OnBnClickedRror();
	afx_msg void OnBnClickedRmagdir();
	void GoNoGoDispUpdate();
	double m_dblDispTolerance;
	void GetDispData();
	void GetGoNoGoDispInitData();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL GoNoGoEdtChange();
	void GetDispGoNoGo();
	void SetGoNoGoLabel();
	CString m_strGoNoGoRange;
	double m_dblToleranceLow;
	double m_dblToleranceHigh;
	BOOL IsInTheTolRange();
	afx_msg void OnEnUpdateEdtTol();
	CNumEdit m_cTolValue;
};
