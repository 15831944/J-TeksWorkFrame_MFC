#include "StdAfx.h"
#include "MilGraph.h"

CMilGraph::CMilGraph(void)
: m_milIDSystem(0)
, m_milIDGraph(0)
, m_milIDBuffer(0)
{
}

CMilGraph::~CMilGraph(void)
{
	Free();
}

void CMilGraph::SetColor(MIL_DOUBLE color)
{
	MgraColor(m_milIDGraph, color);
}

void CMilGraph::Clear(void)
{
	MgraClear(m_milIDGraph, m_milIDBuffer);
}

int CMilGraph::EnableGraph(void)
{
	return MgraAlloc(m_milIDSystem, (MIL_ID *)&m_milIDGraph);
}

void CMilGraph::Free(void)
{
	if (m_milIDGraph)
	{
		MgraFree(m_milIDGraph);
		m_milIDGraph = NULL;
	}
}

void CMilGraph::Rect(double dblLeft, double dblTop, double dblRight, double dblBottom)
{
	MgraRect(m_milIDGraph, m_milIDBuffer,dblLeft, dblTop, dblRight, dblBottom);
}

void CMilGraph::Line(double dblLeft, double dblTop, double dblRight, double dblBottom)
{
	MgraLine(m_milIDGraph, m_milIDBuffer,dblLeft, dblTop, dblRight, dblBottom);
}

int CMilGraph::GetIDGraph(void)
{
	return m_milIDGraph;
}

void CMilGraph::Dot(double dblX, double dblY)
{
	MgraDot(m_milIDGraph, m_milIDBuffer, dblX, dblY);
}

void CMilGraph::OutputText(double dblX, double dblY, TCHAR * szText)
{
	MgraText(m_milIDGraph, m_milIDBuffer, dblX, dblY, szText);
}

void CMilGraph::FontScale(double dblXScale, double dblYScale)
{
	MgraFontScale(m_milIDGraph, dblXScale, dblYScale);
}

void CMilGraph::FontSize(int sizeFont)
{
	MgraFont(m_milIDGraph, sizeFont);
}

void CMilGraph::Arc(double dblXCenter, double dblYCenter, double dblXRad, double dblYRad, double dblStartAngle, double dblEndAngle)
{
	MgraArc(m_milIDGraph, m_milIDBuffer, dblXCenter, dblYCenter, dblXRad, dblYRad, dblStartAngle, dblEndAngle);
}

void CMilGraph::Ellipse(double dblXCenter, double dblYCenter, double dblXRad, double dblYRad)
{
	Arc(dblXCenter, dblYCenter, dblXRad, dblYRad, 0, 360);
}

CDC *CMilGraph::GetDC(void)
{
	HDC hDC;
	MbufControl(m_milIDBuffer, M_DC_ALLOC, M_DEFAULT);
	hDC = (HDC)MbufInquire(m_milIDBuffer, M_DC_HANDLE, M_NULL);
	if(hDC)
	{
		CDC *pDC = new(CDC);
		pDC->Attach(hDC);
		return pDC;
	}
	return NULL;
}

void CMilGraph::FreeDC(void)
{
     MbufControl(m_milIDBuffer, M_DC_FREE, M_DEFAULT);
}

int CMilGraph::Modified(void)
{
	MbufControl(m_milIDBuffer, M_MODIFIED, M_DEFAULT);
	return 0;
}