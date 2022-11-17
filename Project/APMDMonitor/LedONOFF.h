#pragma once


// CLedONOFF

class CLedONOFF : public CProgressCtrl
{
	DECLARE_DYNAMIC(CLedONOFF)

public:
	CLedONOFF();
	virtual ~CLedONOFF();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void Set(bool bLedStatus);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};


