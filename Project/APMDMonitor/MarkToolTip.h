#pragma once
#include "atltypes.h"


// CMarkToolTip

class CMarkToolTip : public CWnd
{
	DECLARE_DYNAMIC(CMarkToolTip)

public:
	CMarkToolTip();
	virtual ~CMarkToolTip();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL RelayEvent(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CRect m_rectScanArea;
	int m_nTimeForTip;
	bool m_IsInRect;
	int Init(CWnd *pWnd, int nTimeForTip=400);
	CWnd *m_pWndReg;
	bool m_IsToolTip;
	CPoint m_curPoint;
	long m_poslParam;
};


