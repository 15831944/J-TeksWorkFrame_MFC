#pragma once
#include "stdafx.h"
#include "Status.h"

class CDrawingHelper
{
public:
	CDrawingHelper();
	~CDrawingHelper();
	CDC *m_pDC;
	HDC m_hDC;
	Graphics *m_pGraphics;
	void SetDC(CDC *pDC);
private:
	int m_iRadius;
	Point m_CenterPt;
	Point m_GapCenterPt[3];
	CRect m_circleRect;
	Rect m_rect;

	struct tNotch
	{
		double dblStart;
		double dblEnd;
	};
public:
	void DrawingInitialize(CRect circleRect);
	void DrawingGap();
private:
	void DrawingNotch();
	void SettingThreeGapCenterPt();
	tNotch m_tNotch;
	int m_iGapRadius;
	int m_iInterval;
	PointF m_MaxStringPt[3], m_MinStringPt[3], m_TargetStringPt[3];	// 0 : gap1, 1: gap2, 2: gap3
	void DrawGapEllipse(int iGapNum);
	int m_iRectHalfHeight;
	int m_iRectHalfWidth;
	void DrawString(int iGapNum);
	void DrawToleranceRect(int iGapNum, double dblHighTolerance);
	int CalcPointY(int iGapNum, double dblTarget, double dblMaxValue, double dblMinValue);
	void DrawLine(int iGapNum);
public:
	BOOL InspectGap(int iGapNum, double dblGapValue);
private:
	void CalcPoint(int iGapNum);
	double m_dblMaxValue[3];	//  0 : gap1, 1: gap2, 2: gap3
	double m_dblMinValue[3];
	double m_dblTarget[3];
	double m_dblGapValueByUnit[3];
	double m_dblCalcHighTolerance[3];
	double m_dblCalcLowTolerance[3];
	void CalcVariableByUnit(int iGapNum);
};

