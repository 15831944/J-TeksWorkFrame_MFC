
// ALMDMonitorView.h : interface of the CALMDMonitorView class
//

#pragma once
#include "ALMDStatusDlg.h"
#include "ViewDlg.h"
#include "OpPanelDlg.h"
#include "ReportDlg.h"
#include "ExcelReport.h"

class CALMDMonitorView : public CView
{
protected: // create from serialization only
	CALMDMonitorView();
	DECLARE_DYNCREATE(CALMDMonitorView)

// Attributes
public:
	CALMDMonitorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CALMDMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	int MakeChildWnd();
	CALMDStatusDlg *m_pStatusDlg;
	CViewDlg *m_pViewDlg;
	COpPanelDlg *m_pOpPanelDlg;
	
	virtual void OnInitialUpdate();
	int WndRepositioning();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSettingsSetreferenceplane();
	afx_msg void OnSettingsComportsetting();
	afx_msg void OnEnvset();
	afx_msg void OnDiagnosis();
	CReportDlg *m_pReportDlg;
	afx_msg void OnFileDatareport();
	int MakeExcelReport(CString strFileName);
	CExcelReport m_ExcelReport;
	afx_msg void OnSettingsStabilization();
	afx_msg void OnUpdateDiagnosis(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpenlog(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileDatareport(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLogSave(CCmdUI *pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in ALMDMonitorView.cpp
inline CALMDMonitorDoc* CALMDMonitorView::GetDocument() const
   { return reinterpret_cast<CALMDMonitorDoc*>(m_pDocument); }
#endif

