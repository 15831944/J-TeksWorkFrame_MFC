#pragma once

extern const UINT RANGE_CHANGED;

enum _RangeMessages {
	HS_LEFTCHANGED = 1,
	HS_RIGHTCHANGED,
	HS_BOTHCHANGED,
};

// CHistoSlider

class CHistoSlider : public CWnd
{
	DECLARE_DYNAMIC(CHistoSlider)

public:
	CHistoSlider();
	virtual ~CHistoSlider();

	void Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext = NULL);

	// Variables
protected:
	double m_dMin;
	double m_dMax;
	double m_dLeft;
	double m_dRight;
	int m_nArrowWidth;

	// Tracking the Mouse
	BOOL m_bTracking;
	enum _TrackMode {
		TRACK_LEFT,
		TRACK_RIGHT,
		TRACK_MIDDLE,
	} m_TrackMode; 

	CRect m_rcRectLeft;
	CRect m_rcRectRight;

	int m_dx;
	CPoint m_ptClickOffset;

	// Operations
protected:
	BOOL RegisterWindowClass(void);

	// Displaying
	void Normalize(void);

public:
	void SetMinMax(int nMin, int nMax);
	void SetRange(int nLeft, int nRight);

	double GetMin(void) { return (int)m_dMin; };    
	double GetMax(void) { return (int)m_dMax; };
	double GetLeft(void) { return (int)m_dLeft; };
	double GetRight(void) { return (int)m_dRight; };

	void GetMinMax(int &nMin, int &nMax) { 
		nMin = (int)m_dMin; 
		nMax = (int)m_dMax; 
	};
	void GetRange(int &nLeft, int &nRight) { 
		nLeft = (int)m_dLeft; 
		nRight = (int)m_dRight; 
	};

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


