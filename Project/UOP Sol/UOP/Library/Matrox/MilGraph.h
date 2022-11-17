#pragma once
#include <mil.h>
#include "MilBase.h"

/*!
\brief 
* Mil의 Graph관련  함수군을 Class화 하였다
@author 전병근
*/
class MIL_DLLDECL CMilGraph:public CMilBase
{
public:
	enum tGRACOLOR {GRA_RED=M_COLOR_RED, GRA_GREEN=M_COLOR_GREEN, GRA_BLACK=M_COLOR_BLACK};
	enum tFONTSIZE {GRA_LARGE=M_FONT_DEFAULT_LARGE, GRA_MEDIUM=M_FONT_DEFAULT_MEDIUM, GRA_SMALL=M_FONT_DEFAULT_SMALL};
	CMilGraph(void);
	~CMilGraph(void);
	void SetColor(MIL_DOUBLE color);
	void Clear(void);
	int EnableGraph(void);
	MIL_ID m_milIDSystem, m_milIDGraph;
	///m_milIDBuffer는 상속받는 대상에 따라 Buffer ID로 혹은OverlayID로 사용된다. 
	void Free(void);
	void Rect(double dblLeft, double dblTop, double dblRight, double dblBottom);
	void Line(double dblLeft, double dblTop, double dblRight, double dblBottom);
	int GetIDGraph(void);
	void Dot(double dblX, double dblY);
	void OutputText(double dblX, double dblY, TCHAR * szText);
	void FontScale(double dblXScale, double dblYScale);
	void FontSize(int sizeFont);
	void Arc(double dblXCenter, double dblYCenter, double dblXRad, double dblYRad, double dblStartAngle, double dblEndAngle);
	void Ellipse(double dblXCenter, double dblYCenter, double dblXRad, double dblYRad);
	CDC *GetDC(void);
	void FreeDC(void);
	int Modified(void);

protected:
	MIL_ID m_milIDBuffer;
};
