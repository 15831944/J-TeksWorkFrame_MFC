/////////////////////////////////////////////////////////////////////////////
// AdvancedEdit.cpp : implementation file for the CAdvancedEdit class
//
// Written by Loic Brayat (dev@pileouface.org)
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AdvancedEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CAdvancedEdit, CEdit)

CAdvancedEdit::CAdvancedEdit()
{
    textBGColor = TRANSPARENT;
    m_BGColor = WHITE;	
}

CAdvancedEdit::~CAdvancedEdit()
{
    if (cBrush.GetSafeHandle())
        cBrush.DeleteObject();
}

void CAdvancedEdit::SetTextBackGround(const COLORREF color) 
{ 
    textBGColor = color; 

    this->RedrawWindow();
};

void CAdvancedEdit::ResetTextBackGround() 
{ 
    SetTextBackGround(TRANSPARENT); 
};

BEGIN_MESSAGE_MAP(CAdvancedEdit, CEdit)
	//{{AFX_MSG_MAP(CAdvancedEdit)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvancedEdit message handlers

HBRUSH CAdvancedEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    CRect rect;

    this->GetClientRect(&rect);

    if (textBGColor == TRANSPARENT)
        pDC->SetBkMode(TRANSPARENT);
    else
        pDC->SetBkMode(OPAQUE);

    pDC->SetBkColor(textBGColor);
    pDC->SetTextColor(m_FGColor);

    if (cBrush.GetSafeHandle())
       cBrush.DeleteObject();
    cBrush.CreateSolidBrush(m_BGColor);
    
    return cBrush;
}
