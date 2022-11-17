#pragma once
#include <UniUtility.h>
#include "SCAlgoBase.h"
class CLineEquation;
#define COUNT_SUBREGION 20
class CSCHEdgeDetAlgo:public CSCAlgoBase
{
public:
	CSCHEdgeDetAlgo();
	~CSCHEdgeDetAlgo();
	int Inspect(CMilBuffer * pMilORG, CMilBuffer * pMilView, CRect * pRectInsp);
	void Measure();
	void DrawLegend(MIL_ID MilOverlayImage, bool DrawSubPositions);
	int m_iStartTime;
	int m_iEndTime;
	void DrawResult(int rv);
	MIL_ID m_milMarker;
	MIL_INT m_nFound;
	int Init();
	MIL_ID m_milBufDisp;
	MIL_DOUBLE m_coeA, m_coeB, m_coeC;
	int FindHEdge(CRect rectUpper);
	int DrawROI(CMilBuffer *pMilView, int iResult);
	long m_iHEdgeScanDir;
	void DrawHLine(CMilBuffer *pMilView);
	CdblRect m_dblRectFound;
	double CalcLine(double dblPosX);
	int Draw(BOOL bIsFinded);
	CLineEquation *m_pLineEquation;
	int m_iDir;
	BOOL m_bUseCenterLine;
};

