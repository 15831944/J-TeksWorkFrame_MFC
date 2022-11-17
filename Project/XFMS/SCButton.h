#pragma once


// CSCButton

class CSCButton : public CButton
{
	DECLARE_DYNAMIC(CSCButton)

public:
	CSCButton();
	virtual ~CSCButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	BOOL m_bStatHover;
//	afx_msg void OnBnHotItemChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	TRACKMOUSEEVENT m_stTrackMouse;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SetBlink(bool bBlink);
	BOOL m_bBlink;
	BOOL m_iBlinkColor;
	void BlinkText(int iBlinkColor);
	BOOL IsBlink();
};


