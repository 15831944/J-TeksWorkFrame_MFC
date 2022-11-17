
// XFMSView.h : CXFMSView 클래스의 인터페이스
//

#pragma once
#include "atltypes.h"

class CCamDlg;
class COpPanelDlg;
class CTeachingDlg;
class CStatusPanelDlg;
class CGraphDlg;
class CHistoLevellingDlg;
class CToolPanelDlg;

class CXFMSView : public CView
{
protected: // serialization에서만 만들어집니다.
	CXFMSView();
	DECLARE_DYNCREATE(CXFMSView)

// 특성입니다.
public:
	CXFMSDoc* GetDocument() const;

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
	virtual ~CXFMSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	int MakeChildWnd();
	CCamDlg *m_pCamDlg[4];
	COpPanelDlg *m_pOpPanelDlg;
	CTeachingDlg *m_pTeachingDlg;
	int WndRepositioning();
	CRect CameraDlgPos(int nPos, CRect rectMain, int widthCam, int heightCam, int xOffset = 0);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetOPMode(int OPMode);
	BOOL IsTeachingMode();
	CRect m_rectCamFull;
	int m_nCountOfCam;
	CStatusPanelDlg *m_pStatusPanelDlg;
	void DoTrial();
	int LoadDefaultJob();
	int CloseAll();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CGraphDlg *m_pGraphDlg;
	CHistoLevellingDlg *m_pHistoLevellingDlg;
	afx_msg void OnToolHistowindowlevel();
	afx_msg void OnUpdateToolHistowindowlevel(CCmdUI *pCmdUI);
	CToolPanelDlg *m_pToolPanelDlg;
};

#ifndef _DEBUG  // SCInspectorView.cpp의 디버그 버전
inline CXFMSDoc* CXFMSView::GetDocument() const
   { return reinterpret_cast<CXFMSDoc*>(m_pDocument); }
#endif

