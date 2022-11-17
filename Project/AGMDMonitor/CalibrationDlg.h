#pragma once
#include "afxwin.h"

class CCal1Dlg;
class CCal2Dlg;

// CCalibrationDlg 대화 상자입니다.

class CCalibrationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCalibrationDlg)

public:
	CCalibrationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCalibrationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIBRATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int MakeChildWnd();
	CCal1Dlg *m_pCal1Dlg;
	CCal2Dlg *m_pCal2Dlg;
	int WndRepositioning();
	CButton m_cNext;
	CButton m_cPrev;
	int m_nStep;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnNext();
	void SetStep(int nStep);
	afx_msg void OnBnClickedBtnPrev();
	CButton m_cBtnCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};
