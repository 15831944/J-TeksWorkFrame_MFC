#pragma once
#include "afxwin.h"
#include "Status.h"
//#include <UniUtility.h>
#include "..\..\Library\UniUtility\UniUtility.h"


// COpPanelDlg 대화 상자입니다.

class COpPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpPanelDlg)

public:
	COpPanelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COpPanelDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPPANEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_iRefPlan;
	virtual BOOL OnInitDialog();
	CString m_strMOP;
	CString m_strMTool;
	CString m_strMStation;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnSetplan();
	CString m_strRelRoll;
	CString m_strRelPitch;
	afx_msg void OnBnClickedBtnMarkapply();
	CString m_strMComment;
	afx_msg void OnBnClickedBtnMarkclear();
	void ClearMark();
	CEdit m_cMOperator;
	afx_msg void OnBnClickedRrefplan();
//	afx_msg void OnBnClickedRrel();
	afx_msg void OnBnClickedRrel();
	void SetProgramMode(CStatus::tProgramMode ProgramMode);
	CButton m_cBtnMPrev;
	CButton m_cBtnMNext;
	CEdit m_cEdtMPos;
	CStatic m_cLNOf;
	CStatic m_cLTOT;
	LRESULT OnPMChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnMprev();
	afx_msg void OnBnClickedBtnMnext();
	LRESULT OnUpdateStatus(WPARAM wParam, LPARAM lParam);
	CRepository *m_pRepository;
	CEdit m_cMTool;
	CEdit m_cMStation;
	CEdit m_cMComment;
	int RefDispUpdate();
	CdblPoint m_dptRefDegree;
	CString m_strXValueUnit;
	CString m_strYValueUnit;
	CdblPoint m_DispRefValue;
	void GoMarkPos(int iPos);
};
