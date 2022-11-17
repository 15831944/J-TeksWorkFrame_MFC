#pragma once
#include "HistoSlider.h"
#include "histoGraph.h"
#include <matrox.h>

// CScrnHistoDlg 대화 상자입니다.



class CScrnHistoDlg : public CDialog
{
	DECLARE_DYNAMIC(CScrnHistoDlg)

public:
	CScrnHistoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CScrnHistoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SCRN_HISTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	CHistoSlider m_HistoSlider;
	CHistoGraph m_HistoOrg;
	CHistoGraph m_HistoModified;
	virtual BOOL OnInitDialog();
	CMilBuffer *m_pImage;
	double m_dXValPro[65536];

//protected:
	double *m_pHistoValueOrg;
	double *m_pHistoValueModified;
public:
	long m_SliderLeft;
	long m_SliderRight;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	BOOL m_IsManual;
	LRESULT OnAdjModeUpdated(UINT wParam, LONG lParam);
	LRESULT OnAdjRangeUpdated(UINT wParam, LONG lParam);
	LRESULT OnAdjRangeChanged(UINT wParam, LONG lParam);
};
