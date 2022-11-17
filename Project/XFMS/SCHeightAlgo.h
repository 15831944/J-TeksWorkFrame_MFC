#pragma once
#define COUNT_HEIGHTSUBREGION 20
#include "SCAlgoBase.h"
#include "SCResult.h"

class CLineEquation;

class CSCHeightAlgo:public CSCAlgoBase
{
public:
	CSCHeightAlgo();
	~CSCHeightAlgo();
	int Inspect(CMilBuffer * pMilORG, CMilBuffer * pMilView, CRect * pRectInsp);
	int Measure();
	int DrawSubPos(double *pDblXPos, double *pDblYPos);
	void DrawLegend(MIL_ID MilOverlayImage, bool DrawSubPositions);
	int m_iStartTime;
	int m_iEndTime;
	int CheckCriteria();
	void DrawResult(int rv);
	MIL_ID m_milMarker;
	MIL_INT m_nFound;
	MIL_DOUBLE m_posEdgeX[COUNT_HEIGHTSUBREGION];
	MIL_DOUBLE m_posEdgeY[COUNT_HEIGHTSUBREGION];
	int m_posResult[COUNT_HEIGHTSUBREGION];
	int Init();
	MIL_ID m_milBufDisp;
	int FindEdge(CRect rectUpper);
	int DrawROI(CMilBuffer *pMilView, int iResult);
	void SetLineEquation(CLineEquation * pLineEquation);
	CLineEquation *m_pLineEquation;
	double m_dblMax;
	double m_dblMin;
	double m_dblMean;
	int m_iCriteriaLow;
	int m_iCriteriaHi;
	int m_iScanDir;
	double m_umMax;
	double m_umMin;
	double m_umMean;
	void GetResult(CSCResult * pResult);
};

