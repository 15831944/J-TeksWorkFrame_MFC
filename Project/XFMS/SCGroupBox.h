#pragma once


// CSCGroupBox

class CSCGroupBox : public CStatic
{
	DECLARE_DYNAMIC(CSCGroupBox)

public:
	CSCGroupBox();
	virtual ~CSCGroupBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


