#pragma once
#define COUNT_SUBREGION 20
#include "SCResult.h"

class CSCAlignAlgo:public CSCAlgoBase
{
public:
	CSCAlignAlgo();
	~CSCAlignAlgo();
	int Inspect(CMilBuffer * pMilORG, CMilBuffer * pMilView, CRect * pRectInsp);
	int m_iScanDir;
	void Measure();
	void DrawLegend(MIL_ID MilOverlayImage, bool DrawSubPositions);
	int DrawSubPos(double *pDblXPos, double *pDblYPos, double *pDbl2XPos, double *pDbl2YPos);
	int m_iStartTime;
	int m_iEndTime;
	int m_iCriteria;
	int CheckCriteria();
	void DrawResult(int rv);
	MIL_ID m_milMarker;
	MIL_INT m_nFound;
	MIL_DOUBLE m_posFirstX[COUNT_SUBREGION];
	MIL_DOUBLE m_posFirstY[COUNT_SUBREGION];
	MIL_DOUBLE m_posSecondX[COUNT_SUBREGION];
	MIL_DOUBLE m_posSecondY[COUNT_SUBREGION];
	int m_posResult[COUNT_SUBREGION];
	int Init();
	MIL_ID m_milIDBufDisp;
	int DrawROI(CMilBuffer *pMilView, int iResult);
	double m_dblMin;
	double m_dblMax;
	double m_dblMean;
	double m_umMin;
	double m_umMax;
	double m_umMean;
	void GetResult(CSCResult * pResult);
};

