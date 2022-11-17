#pragma once
#include "atltypes.h"
class CPlotGage
{
public:
	enum tPlotMode{PLOT_SINGLE, PLOT_DUAL};
	CPlotGage();
	~CPlotGage();
	void SetDC(CDC *pDC);
	CDC *m_pDC;
	void SetRect(CRect rectClient);
	CRect m_ClientRect;
	void SetMode(int iDispMode);
	int m_iDispMode;
	void Draw();
	int Calc();
	CRect m_DrawRect;
	int m_HeightMargin;
	CRect m_rectNormal;
	CRect m_rectZoom;
	void DrawBG();
	void DrawBubble();
	CRect m_rectSafeZone;
	CRect m_rectSafeZoneZoom;
	CRect m_rectZoomArea;
	double m_xPixelForDegree;
	double m_yPixelForDegree;
	double m_xPixelForDegreeZoom;
	double m_yPixelForDegreeZoom;
	CRect m_rectZoomHighLimit;
	BOOL m_bIsOutBound;
	BOOL m_bIsOverTolerance;
	CPoint m_ptBubbleCenter;
	CRect m_NormalPlotBubble;
	double m_dblCurXPos;
	double m_dblCurYPos;
	CRect m_rectZoomLow;
	CRect m_rectRelOrgZoomArea;
	CPoint m_ptZoomBubbleCenter;
	CRect m_ZoomPlotBubble;
	BOOL m_bIsOutBoundZoom;
	int m_dblDispThreshold;
	// //Normal Plot에 확대영상을 그릴것인지 아닌지
	BOOL m_bDispOverThreshold;
	double m_dblZoomAreaDegree;
	double m_dblHiZoomAreaDegree;
	void DrawNotch(CDC * pDC);
};

