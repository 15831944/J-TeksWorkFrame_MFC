
// AVMDMonitorView.h : CAVMDMonitorView 클래스의 인터페이스
//

#pragma once
#include "AVMDDisplayDlg.h"
#include "AVMDGraphDlg.h"
#include "AVMDFFTGraphDlg.h"
#include "AVMDTimeDlg.h"
#include "AVMDScrollDlg.h"
#include "AVMDViewDlg.h"
#include "AVMDReview.h"
#include "TempDlg.h"
#include "PartialSaveDlg.h"
#include "ProgressBarDlg.h"

class CAVMDMonitorView : public CView
{
protected: // serialization에서만 만들어집니다.
	CAVMDMonitorView();
	DECLARE_DYNCREATE(CAVMDMonitorView)

// 특성입니다.
public:
	CAVMDMonitorDoc* GetDocument() const;

// 작업입니다.
public:
	struct ReportCalcResult
	{
		double dblMax_X;
		double dblMin_X;
		double dblMax_Y;
		double dblMin_Y;
		double dblMax_Z;
		double dblMin_Z;
	};
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
	virtual ~CAVMDMonitorView();
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

	CAVMDDisplayDlg *m_pDisplayDlg;
	CAVMDViewDlg *m_pViewDlg;
	CAVMDReview *m_pReviewDlg;

	CAVMDGraphDlg *m_pGraphRMSDlg;
	CAVMDGraphDlg *m_pGraphXDlg;
	CAVMDGraphDlg *m_pGraphYDlg;
	CAVMDGraphDlg *m_pGraphZDlg;
	//
	CAVMDGraphDlg *m_pCompareGraphDlg;

	CAVMDFFTGraphDlg *m_pFFTGraphRMSDlg;
	CAVMDFFTGraphDlg *m_pFFTGraphXDlg;
	CAVMDFFTGraphDlg *m_pFFTGraphYDlg;
	CAVMDFFTGraphDlg *m_pFFTGraphZDlg;
	//
	CAVMDFFTGraphDlg *m_pCompareFFTGraphDlg;

	CAVMDTimeDlg *m_pTimeDlg;
	CAVMDTimeDlg *m_pFFTTimeDlg;

	CAVMDTimeDlg *m_pCompareTimeDlg;
	CAVMDTimeDlg *m_pCompareFFTTimeDlg;

	CAVMDScrollDlg *m_pGraphRMSSrcollDlg;
	CAVMDScrollDlg *m_pGraphXSrcollDlg;
	CAVMDScrollDlg *m_pGraphYSrcollDlg;
	CAVMDScrollDlg *m_pGraphZSrcollDlg;
	//
	CAVMDScrollDlg *m_pCompareGraphSrcollDlg;
	//
	CTempDlg *m_pTempDlg1;
	CTempDlg *m_pTempDlg2;
	CPartialSaveDlg *m_pPartialSaveDlg;

	int MakeChildWnd();
	int WndRepositioning(int nMinimizeNum);

	bool bReEnter = FALSE;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT afx_msg OnReceiveDlgMinimizeUpdate(WPARAM wParam, LPARAM lParam);
	LRESULT afx_msg OnReceiveGraphUpdate(WPARAM wParam, LPARAM lParam);
	LRESULT afx_msg OnUpdateReviewMenuReset(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSettingsComportsetting();
	void DlgDeleteDC(CAVMDGraphDlg* pGraphDlg);
	void FftDlgDeleteDC(CAVMDFFTGraphDlg* pGraphDlg);

	afx_msg void OnSettingsSetstationinformation();
	afx_msg void OnSettingsSetfilter();
	afx_msg void OnSettingsSettimedomainformat();
//	afx_msg void OnOptions();
	afx_msg void OnSettingsAnnotation();
	afx_msg void OnSelectcolorsRms();
	afx_msg void OnSelectcolorsX();
	afx_msg void OnSelectcolorsY();
	afx_msg void OnSelectcolorsZ();
	afx_msg void OnSettingsEnvironmentsetting();
	afx_msg void OnSettingsMarkeditor();
	afx_msg void OnEventSettings();
	afx_msg void OnSelectcolorsTrigger();
	afx_msg void OnSelectcolorsGonogo();
	afx_msg void OnSelectcolorsSpecIn();
	afx_msg void OnSelectcolorsSpecOut();

	static UINT ProcessThread(LPVOID lParam);
private:
	int WndViewRepositioning(int nMinimizeNum);
	int WndReViewRepositioning(int nMinimizeNum);
public:
	afx_msg void OnViewComparedata();
	afx_msg void OnViewOverlay();
	afx_msg void OnSelectcolorsOverlay();
private:
	static int MakeExcelReport(CString strFileName);
public:
	afx_msg void OnSelectcolorsMarks();
private:
    static ReportCalcResult CalcMaxMin(int iStartNum, int iEndNum, int nInputType =0);
	

public:
	afx_msg void OnUpdateSettingsMarkeditor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEventSettings(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSettingsSetstationinformation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewComparedata(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewOverlay(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSaveSettingsAs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileLoadcomparedata(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
private:
    static	void KillProcess();
public:
	afx_msg void OnUpdateFileUpdate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilePartialsave(CCmdUI *pCmdUI);
	afx_msg void OnFilePartialsave();
	afx_msg void OnSelectcolorsIndicator();
	afx_msg void OnUpdateFileSaveas(CCmdUI *pCmdUI);
	void SetDrawPointNum();
	int MessagePumping();
	afx_msg void OnUpdateFileSavecurrentfilteredcsv(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // AVMDMonitorView.cpp의 디버그 버전
inline CAVMDMonitorDoc* CAVMDMonitorView::GetDocument() const
   { return reinterpret_cast<CAVMDMonitorDoc*>(m_pDocument); }
#endif

