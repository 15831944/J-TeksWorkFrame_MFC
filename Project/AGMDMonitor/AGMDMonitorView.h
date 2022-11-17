
// AGMDMonitorView.h : CAGMDMonitorView 클래스의 인터페이스
//

#pragma once
#include "MonitorDlg.h"
#include "OPPanelDlg.h"
#include "SenSorViewDlg.h"
#include "ViewDlg.h"
#include "SenSorStatusDlg.h"
#include "BrowserDlg.h"
#include "ExcelReport.h"
#include "ReportDlg.h"
#include "StationInfomationDlg.h"

class CAGMDMonitorView : public CView
{
protected: // serialization에서만 만들어집니다.
	CAGMDMonitorView();
	DECLARE_DYNCREATE(CAGMDMonitorView)

// 특성입니다.
public:
	CAGMDMonitorDoc* GetDocument() const;

// 작업입니다.
public:
	CMonitorDlg *m_pMonitorDlg;
	COPPanelDlg *m_pOppanelDlg;
	CSenSorViewDlg *m_pSenSorViewDlg;
	CSenSorStatusDlg *m_pSenSorStatusDlg;
	CBrowserDlg *m_pBrowserDlg;
	CViewDlg *m_pViewDlg;
	CViewDlg *m_pTempDlg;
	CReportDlg *m_pReportDlg;
	CStationInfomationDlg *m_pStationInfoDlg;
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CAGMDMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT afx_msg OnDlgUpdate(WPARAM wParam, LPARAM lParam);
private:
	int WndRepositioning();
	int MakeChildWnd();
public:
	afx_msg void OnSettingsComportsetting();
	afx_msg void OnSetNotch2();
	afx_msg void OnSetNotch4();
	afx_msg void OnSetNotch6();
	afx_msg void OnSetNotch8();
	afx_msg void OnSetNotch10();
	afx_msg void OnSetNotch12();
	afx_msg void OnSettingsSetgapcriteria();
	afx_msg void OnSelectcolorsTolerance();
	afx_msg void OnSelectcolorsSpecin();
	afx_msg void OnSpecoutGapistoosmall();
	afx_msg void OnSpecoutGapistoobig();
	afx_msg void OnEnvSet();
	afx_msg void OnSettingsSetstationinformation();
	afx_msg void OnSettingsCalibration();
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSettingsSetgapcriteria(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSettingsSetgapoffsets(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSettingsCalibration(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetNotch2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetNotch4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetNotch6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetNotch8(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetNotch10(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetNotch12(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSaveaslogfile(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // AGMDMonitorView.cpp의 디버그 버전
inline CAGMDMonitorDoc* CAGMDMonitorView::GetDocument() const
   { return reinterpret_cast<CAGMDMonitorDoc*>(m_pDocument); }
#endif

