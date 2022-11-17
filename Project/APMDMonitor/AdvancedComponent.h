#ifndef __ADVANCEDCOMPONENT_H__
#define __ADVANCEDCOMPONENT_H__

/////////////////////////////////////////////////////////////////////////////
// AdvancedComponent.h : header file for the virtual CAdvancedComponent class
//
// Written by Loic Brayat (dev@pileouface.org)
//
/////////////////////////////////////////////////////////////////////////////

const COLORREF CLOUDBLUE = RGB(128, 184, 223);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF BLACK = RGB(1, 1, 1);
const COLORREF DKGRAY = RGB(128, 128, 128);
const COLORREF LTGRAY = RGB(192, 192, 192);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF DKYELLOW = RGB(128, 128, 0);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF DKRED = RGB(128, 0, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF DKBLUE = RGB(0, 0, 128);
const COLORREF CYAN = RGB(0, 255, 255);
const COLORREF DKCYAN = RGB(0, 128, 128);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF DKGREEN = RGB(0, 128, 0);
const COLORREF MAGENTA = RGB(255, 0, 255);
const COLORREF DKMAGENTA = RGB(128, 0, 128);

/////////////////////////////////////////////////////////////////////////////

class CAdvancedComponent
{

public:

    virtual bool SetForeGround(const COLORREF FGColor) {
        m_FGColor = FGColor; 
        this->RedrawComponent();
    	return true;
    }

    COLORREF GetForeGroundColor() const { return m_FGColor; };

    virtual bool SetBackGround(const COLORREF BGColor) {
        m_BGColor = BGColor; 
        this->RedrawComponent();
    	return true;
    }

    COLORREF GetBackGroundColor() const { return m_BGColor; };

protected:

    COLORREF m_BGColor;
    COLORREF m_FGColor;

    virtual void RedrawComponent() = 0;

};

#endif

