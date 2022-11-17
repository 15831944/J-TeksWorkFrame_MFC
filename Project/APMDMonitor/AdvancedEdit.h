#ifndef __ADVANCEDEDIT_H__
#define __ADVANCEDEDIT_H__

/////////////////////////////////////////////////////////////////////////////
// AdvancedEdit.h : header file for the CAdvancedButton class
//
// Written by Loic Brayat (dev@pileouface.org)
//
/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdvancedComponent.h"

class CAdvancedEdit : public CEdit, public CAdvancedComponent
{
DECLARE_DYNAMIC(CAdvancedEdit)
public:

    CAdvancedEdit();
	virtual ~CAdvancedEdit();
    
    void SetTextBackGround(const COLORREF color);
    COLORREF GetTextBackGround() const { return textBGColor; };
    void ResetTextBackGround();
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedEdit)
	//}}AFX_VIRTUAL

protected:

    //{{AFX_MSG(CAdvancedEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

    void RedrawComponent() {
        RedrawWindow();
    }

    COLORREF textBGColor;

    CBrush cBrush;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif 
