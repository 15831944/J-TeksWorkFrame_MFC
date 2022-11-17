
// APMDMonitorView.h : CAPMDMonitorView 클래스의 인터페이스
//

#pragma once

#include "ViewOptionDlg.h"
#include "GraphDlg.h"
#include "ModeDlg.h"
#include "StatusDlg.h"
#include "ComSetDlg.h"
#include "MarkEditDlg.h"
#include "WarningSetDlg.h"
#include "EnvSettingDlg.h"
#include "afxcoll.h"
#include "StatusCompareDlg.h"

class CAPMDMonitorView : public CView
{
protected: // serialization에서만 만들어집니다.
	CAPMDMonitorView();
	DECLARE_DYNCREATE(CAPMDMonitorView)

// 특성입니다.
public:
	CAPMDMonitorDoc* GetDocument() const;

// 작업입니다.
public:

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
	virtual ~CAPMDMonitorView();
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
	int MakeChildWnd();
	CViewOptionDlg *m_pViewOptionDlg;
	CGraphDlg *m_pGraphDlg;
	CModeDlg *m_pModeDlg;
	CStatusDlg *m_pStatusDlg;
	CComSetDlg *m_pComsetDlg;

	afx_msg void OnComset();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void WndRepositioning();
	afx_msg void OnWarnset();
	afx_msg void OnMarkeditor();
	afx_msg void OnEnvset();
	CPtrList m_lstMarks;
	int MakeExcelReport(CString strFileName);
	afx_msg void OnViewComparedata();
	afx_msg void OnOverlay();
	void DemendComparedataView();
	void ApplyCompareMode();
	CStatusCompareDlg *m_pStatusCompareDlg;
	afx_msg void OnUpdateLoadComparedata(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewComparedata(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOverlay(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // APMDMonitorView.cpp의 디버그 버전
inline CAPMDMonitorDoc* CAPMDMonitorView::GetDocument() const
   { return reinterpret_cast<CAPMDMonitorDoc*>(m_pDocument); }
#endif

