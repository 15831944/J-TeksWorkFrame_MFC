#pragma once
#include "atltypes.h"
class CPlotLiveChart
{
public:
	CPlotLiveChart();
	~CPlotLiveChart();
	void SetDC(CDC *pDC);
	CDC *m_pDC;
	void SetRect(CRect rectClient);
	CRect m_rectClient;
	CRect m_rectPlot;
	int Calc();
	int DrawBG();
	int Draw();
	void DrawLegend();
	void DrawGrid();
	double m_dblXGridSpace;
	double m_dblYGridSpace;
	int m_iGraphXRange;
	int m_iGraphYMax;
	int m_iGraphYMin;
	int **m_pHistoGraph;
	int m_nCountOfData;
	double m_dblHRate;
	int m_iDiffYRange;
	int m_nCountOfSensors;
	int m_iValidHisto;
};

