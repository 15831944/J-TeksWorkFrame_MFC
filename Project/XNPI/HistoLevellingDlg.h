#pragma once
#include "afxwin.h"
#include "SCButton.h"
#include "HistoHandle.h"

// CHistoLevellingDlg 대화 상자입니다.

class CXNPIView;

class CHistoLevellingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoLevellingDlg)

public:
	CHistoLevellingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHistoLevellingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOLEVEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void HistoUpdate();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CXNPIView *m_pView;
	CMilBuffer *m_pOrgMilBuffer;
	CStatic m_cHisotoArea;
	void DrawHistogram();
	MIL_INT m_Histodata[65536];

	afx_msg void OnBnClickedBtnApply();
	CHistoHandle m_cMinHandle;
	CHistoHandle m_cMaxHandle;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	CStatic m_cHistoHandleRect;
	LRESULT OnHistoHDLUpdate(WPARAM wparam, LPARAM lparam);
	afx_msg void OnBnClickedBtLoadimg();
	// 히스토그램중에 가장 큰 값
	afx_msg void OnBnClickedBtnSavwlv();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	CComboBox m_cbxWLPreset;
	afx_msg void OnCbnSelchangeComboPreset();
};
