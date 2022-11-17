#pragma once

#include "ParticlePlot.h"
#include "afxwin.h"
#include "SliderCtrlExt.h"
#include "NumEdit.h"

// CGraphDlg 대화 상자입니다.
class CViewOptionDlg;
class CGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	CGraphDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGraphDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_cPosText;
	void PosTextOut(CString strText);
	afx_msg void OnNMReleasedcaptureSldPos(NMHDR *pNMHDR, LRESULT *pResult);
	bool m_OnNavi;
	CSliderCtrlExt m_cNavi;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	void PosTextOut();
	CParticlePlot m_cParticlePlot;
	CParticlePlot m_cComparePlot;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void SetViewOptionDlg(CViewOptionDlg * pViewOptionDlg);
	CViewOptionDlg *m_pViewOptionDlg;
	CStatic m_cMaxPosText;
	void PosTextOut(int CurPos, int MaxPos);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int CheckPosLimit();
	int CheckMinPosLimit();
	CMarkToolTip m_cMarkToolTip;
	CEdit m_cMinPosText;
	CStatic m_cMaxMinPosText;
	void CreateComponents();
	void SetDualMode(bool bDualMode);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// Main과 Compare간의 시간차를 초로
	CNumEdit m_cDiffTime;
	void DiffTimeTextOut();
	void DiffTimeTextOut(int nDiffTime);
	int CheckDiffTimeLimit();
	int GetDiffTime();
	afx_msg void OnTRBNThumbPosChangingSldPos(NMHDR *pNMHDR, LRESULT *pResult);
};
