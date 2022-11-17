#include "stdafx.h"
#include "DrawingHelper.h"
#include "Status.h"
#include <gdiplus.h>
#define _USE_MATH_DEFINES
#include <math.h>

CDrawingHelper::CDrawingHelper()
	: m_pGraphics(NULL)
	, m_iRadius(0)
	, m_iGapRadius(0)
	, m_iInterval(0)
	, m_iRectHalfHeight(0)
	, m_iRectHalfWidth(0)
{

}

CDrawingHelper::~CDrawingHelper()
{
}

void CDrawingHelper::SetDC(CDC *pDC)
{
	// CDC를 HDC로 변환
	m_pDC = pDC;
	m_hDC = pDC->m_hDC;
	m_hDC = pDC->GetSafeHdc();
	//
	if (m_pGraphics != NULL)
	{
		delete m_pGraphics;
		m_pGraphics = new Graphics(m_hDC);
		
	}
	else
		m_pGraphics = new Graphics(m_hDC);
}

void CDrawingHelper::DrawingInitialize(CRect circleRect)
{
	m_circleRect = circleRect;
	m_pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
	Pen circlePen(Color::Black, 1.0f);
	Pen rectPen(Color::Black, 1.0f);
	
	m_CenterPt.X = circleRect.Width() / 2;
	m_CenterPt.Y = circleRect.Height() / 2;
	m_iRadius = circleRect.Width() / 2;
	m_iGapRadius = (int)((double)m_iRadius / 2.4);
	m_iInterval = (int)((double)m_iRadius / 2.);
	DrawingNotch();
	SettingThreeGapCenterPt();
	//아래의 -1은 Drawing 할 경우 픽셀을 넘어가기 때문에 해주었다.
	m_pGraphics->DrawRectangle(&rectPen, m_circleRect.left, m_circleRect.top, m_circleRect.Width() - 1, m_circleRect.Height() -1);
	m_pGraphics->DrawArc(&circlePen ,m_circleRect.left, m_circleRect.top, m_circleRect.right - 1, m_circleRect.bottom - 1, (float)m_tNotch.dblStart, (float)m_tNotch.dblEnd);
}

void CDrawingHelper::DrawingNotch()
{
	Pen circlePen(Color::Black, 1.0f);

	int iAngle = 0;

	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::TWO_CLOCK)
		iAngle = -30;
	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::FOUR_CLOCK)
		iAngle = -330;
	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::SIX_CLOCK)
		iAngle = -270;
	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::EIGHT_CLOCK)
		iAngle = -210;
	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::TEN_CLOCK)
		iAngle = -150;
	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::TWELVE_CLOCK)
		iAngle = -90;

	m_tNotch.dblStart = iAngle + 2;
	m_tNotch.dblEnd = 356;
	
	// Notch 부분 3점을 계산 하여 직선을 긋는다.
	int height1 = (int)(m_iRadius *sin(M_PI * (abs(iAngle) - 2) / 180.0));
	int width1 = (int)(m_iRadius * cos(M_PI * (abs(iAngle) - 2) / 180.0));
	Point notchPt1;
	notchPt1.X = m_CenterPt.X + width1;
	notchPt1.Y = m_CenterPt.Y - height1;

	int height2 = (int)(m_iRadius *sin(M_PI * (abs(iAngle) + 2) / 180.0));
	int width2 = (int)(m_iRadius * cos(M_PI * (abs(iAngle) + 2) / 180.0));
	Point notchPt2;
	notchPt2.X = m_CenterPt.X + width2;
	notchPt2.Y = m_CenterPt.Y - height2;

	int height3 = (int)((m_iRadius - 15) *sin(M_PI * abs(iAngle) / 180.0));
	int width3 = (int)((m_iRadius - 15) * cos(M_PI * abs(iAngle) / 180.0));
	
	Point notchPt3;
	notchPt3.X = m_CenterPt.X + width3;
	notchPt3.Y = m_CenterPt.Y - height3;

	m_pGraphics->DrawLine(&circlePen, notchPt1, notchPt3);
	m_pGraphics->DrawLine(&circlePen, notchPt2, notchPt3);
}

void CDrawingHelper::SettingThreeGapCenterPt()
{
	//테스트 하면서 최적의 위치 선정
	int iPosXDivide = m_circleRect.Width() / 20;
	int iPosYDivide = m_circleRect.Height() / 15;
	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::TWO_CLOCK)
	{
		m_GapCenterPt[2].X = m_CenterPt.X - (int)(iPosXDivide * 4.5);
		m_GapCenterPt[2].Y = m_CenterPt.Y - (int)(iPosYDivide * 2);
		m_GapCenterPt[1].X = m_CenterPt.X;
		m_GapCenterPt[1].Y = m_CenterPt.Y + (int)(iPosYDivide * 4);
		m_GapCenterPt[0].X = m_CenterPt.X + (int)(iPosXDivide * 4.5);
		m_GapCenterPt[0].Y = m_CenterPt.Y - (int)(iPosYDivide * 2);
	}
	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::FOUR_CLOCK)
	{
		m_GapCenterPt[2].X = m_CenterPt.X;
		m_GapCenterPt[2].Y = m_CenterPt.Y - (int)(iPosYDivide * 4);
		m_GapCenterPt[1].X = m_CenterPt.X - (int)(iPosXDivide * 4.5);
		m_GapCenterPt[1].Y = m_CenterPt.Y + (int)(iPosYDivide * 2);
		m_GapCenterPt[0].X = m_CenterPt.X + (int)(iPosXDivide * 4.5);
		m_GapCenterPt[0].Y = m_CenterPt.Y + (int)(iPosYDivide * 2);
	}
	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::SIX_CLOCK)
	{
		m_GapCenterPt[2].X = m_CenterPt.X + (int)(iPosXDivide * 4.5);
		m_GapCenterPt[2].Y = m_CenterPt.Y - (int)(iPosYDivide * 2);
		m_GapCenterPt[1].X = m_CenterPt.X - (int)(iPosXDivide * 4.5);
		m_GapCenterPt[1].Y = m_CenterPt.Y - (int)(iPosYDivide * 2);
		m_GapCenterPt[0].X = m_CenterPt.X;
		m_GapCenterPt[0].Y = m_CenterPt.Y + (int)(iPosYDivide * 4);
	}
	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::EIGHT_CLOCK)
	{
		m_GapCenterPt[2].X = m_CenterPt.X + (int)(iPosXDivide * 4.5);
		m_GapCenterPt[2].Y = m_CenterPt.Y + (int)(iPosYDivide * 2);
		m_GapCenterPt[1].X = m_CenterPt.X;
		m_GapCenterPt[1].Y = m_CenterPt.Y - (int)(iPosYDivide * 4);
		m_GapCenterPt[0].X = m_CenterPt.X - (int)(iPosXDivide * 4.5);
		m_GapCenterPt[0].Y = m_CenterPt.Y + (int)(iPosYDivide * 2);
	}
	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::TEN_CLOCK)
	{
		m_GapCenterPt[2].X = m_CenterPt.X;
		m_GapCenterPt[2].Y = m_CenterPt.Y + (int)(iPosYDivide * 4);
		m_GapCenterPt[1].X = m_CenterPt.X + (int)(iPosXDivide * 4.5);
		m_GapCenterPt[1].Y = m_CenterPt.Y - (int)(iPosYDivide * 2);
		m_GapCenterPt[0].X = m_CenterPt.X - (int)(iPosXDivide * 4.5);
		m_GapCenterPt[0].Y = m_CenterPt.Y - (int)(iPosYDivide * 2);
	}
	if (CStatus::Instance()->GetNotchMode() == CStatus::tNotchMode::TWELVE_CLOCK)
	{
		m_GapCenterPt[2].X = m_CenterPt.X - (int)(iPosXDivide * 4.5);
		m_GapCenterPt[2].Y = m_CenterPt.Y + (int)(iPosYDivide * 2);
		m_GapCenterPt[1].X = m_CenterPt.X + (int)(iPosXDivide * 4.5);
		m_GapCenterPt[1].Y = m_CenterPt.Y + (int)(iPosYDivide * 2);
		m_GapCenterPt[0].X = m_CenterPt.X;
		m_GapCenterPt[0].Y = m_CenterPt.Y - (int)(iPosYDivide * 4);
	}
}

void CDrawingHelper::DrawingGap()
{
	for (int iGapNum = 0; iGapNum < 3; iGapNum++)
	{
		DrawGapEllipse(iGapNum);

		// Target을 기준으로 Max, Min 값 구한 후 값 적기
		CalcVariableByUnit(iGapNum);

		CalcPoint(iGapNum);
		DrawString(iGapNum);	
		DrawToleranceRect(iGapNum, m_dblCalcHighTolerance[iGapNum]);
		DrawLine(iGapNum);
		InspectGap(iGapNum, m_dblGapValueByUnit[iGapNum]);
	}
}

void CDrawingHelper::DrawGapEllipse(int iGapNum)
{
	// 원그리기
	Pen circlePen(Color::Black, 1.0f);
	Rect circlRect;
	circlRect.X = m_GapCenterPt[iGapNum].X - m_iGapRadius;
	circlRect.Y = m_GapCenterPt[iGapNum].Y - m_iGapRadius;
	circlRect.Width = m_iGapRadius * 2;
	circlRect.Height = m_iGapRadius * 2;

	m_pGraphics->DrawEllipse(&circlePen, circlRect);

	// 사각형 및 직선그리기
	m_iRectHalfHeight = (int)((double)m_iGapRadius * 0.8);
	m_iRectHalfWidth = (int)((double)m_iGapRadius * 0.1);
	m_rect.X = m_GapCenterPt[iGapNum].X - m_iRectHalfWidth;
	m_rect.Y = m_GapCenterPt[iGapNum].Y - m_iRectHalfHeight;
	m_rect.Width = m_iRectHalfWidth * 2;
	m_rect.Height = m_iRectHalfHeight * 2;

	m_pGraphics->DrawRectangle(&circlePen, m_rect);

	// Gap 번호 적기
	int iFontSize = m_iGapRadius / 2;
	Gdiplus::Font font(_T("Arial"), (REAL)iFontSize, FontStyleRegular, UnitPixel);

	SolidBrush sbrush(Color::Black);
	StringFormat format;
	CString strGapNum;
	strGapNum.Format(_T("%d"), iGapNum + 1);
	PointF gapStringPt;
	gapStringPt.X = (REAL)(m_GapCenterPt[iGapNum].X + m_rect.Width);
	gapStringPt.Y = (REAL)(m_GapCenterPt[iGapNum].Y - m_iRectHalfHeight);
	m_pGraphics->DrawString(strGapNum, -1, &font, gapStringPt, &format, &sbrush);
}

void CDrawingHelper::DrawString(int iGapNum)
{
	CString strMaxValue, strMinValue, strTargetValue;

	strMaxValue.Format(_T("%.3f"), m_dblMaxValue[iGapNum]);
	strMinValue.Format(_T("%.3f"), m_dblMinValue[iGapNum]);
	strTargetValue.Format(_T("%.3f"), m_dblTarget[iGapNum]);

	int iFontSize = m_iGapRadius / 12;
	Gdiplus::Font rangefont(_T("Arial"), (REAL)iFontSize, FontStyleRegular, UnitPixel);
	SolidBrush sbrush(Color::Black);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);

	m_pGraphics->DrawString(strMaxValue, -1, &rangefont, m_MaxStringPt[iGapNum], &format, &sbrush);
	m_pGraphics->DrawString(strMinValue, -1, &rangefont, m_MinStringPt[iGapNum], &format, &sbrush);
	m_pGraphics->DrawString(strTargetValue, -1, &rangefont, m_TargetStringPt[iGapNum], &format, &sbrush);
}

void CDrawingHelper::DrawToleranceRect(int iGapNum, double dblHighTolerance)
{
	double dblFromMinToMax = abs(abs(m_dblMaxValue[iGapNum]) - abs(m_dblMinValue[iGapNum]));
	double dblFromMinToHighTolerance = abs(abs(dblHighTolerance) - abs(m_dblMinValue[iGapNum]));
	double dblFromMaxPtToMinPt = (double)(m_MinStringPt[iGapNum].Y - m_MaxStringPt[iGapNum].Y);
	int iCalcPt_Y = 0, iTemp = 0;
	// dblFromMinToMax : dblFromMaxPtToMinPt = dblFromMinToHighTolerance : iCalcPt_Y -> 비례식
	iCalcPt_Y = (int)(dblFromMaxPtToMinPt * dblFromMinToHighTolerance / dblFromMinToMax);

 	Rect ToleranceRect;
	ToleranceRect.X = m_rect.X;
	ToleranceRect.Y = (int)m_MinStringPt[iGapNum].Y - iCalcPt_Y;
	ToleranceRect.Width = m_rect.Width;
	ToleranceRect.Height = (int)(abs(abs(ToleranceRect.Y) - abs(m_TargetStringPt[iGapNum].Y)) * 2.);
	
	Color color = CStatus::Instance()->m_ToleranceColor;

	SolidBrush solidbrush(color);
	
	int iMaxRectPointY = m_rect.Y + m_rect.Height;

	if (ToleranceRect.Y + ToleranceRect.Height >= iMaxRectPointY)
		ToleranceRect.Height = m_rect.Height - (ToleranceRect.Y - m_rect.Y);

	//m_pGraphics->FillRectangle(&solidbrush, ToleranceRect.X + 1, ToleranceRect.Y + 1, ToleranceRect.Width - 1, ToleranceRect.Height - 1 );
	m_pGraphics->FillRectangle(&solidbrush, ToleranceRect.X, ToleranceRect.Y, ToleranceRect.Width, ToleranceRect.Height);
}

int CDrawingHelper::CalcPointY(int iGapNum, double dblTarget, double dblMaxValue, double dblMinValue)
{
	// Max, Min에 따라 Target Y 좌표 계산
	double dblFromMinToMax = abs(abs(dblMaxValue) - abs(dblMinValue));
	double dblFromMinToTarget = abs(abs(dblTarget) - abs(dblMinValue));
	double dblFromMaxPtToMinPt = (double)(m_MinStringPt[iGapNum].Y - m_MaxStringPt[iGapNum].Y);
	int iCalcPt_Y = 0, iTemp = 0;
	// dblFromMinToMax : dblFromMaxPtToMinPt = dblFromMinToTarget : iCalcPt_Y -> 비례식
	iTemp = (int)(dblFromMaxPtToMinPt * dblFromMinToTarget / dblFromMinToMax);
	iCalcPt_Y = (int)m_MinStringPt[iGapNum].Y - iTemp;
	return iCalcPt_Y;
}

void CDrawingHelper::DrawLine(int iGapNum)
{
	Pen linePen(Color::Black, 1.0f);
	m_pGraphics->DrawLine(&linePen, (REAL)(m_MaxStringPt[iGapNum].X + (m_iRectHalfWidth*2)),
		(REAL)(m_MaxStringPt[iGapNum].Y), (REAL)(m_rect.X + m_rect.Width), (REAL)(m_MaxStringPt[iGapNum].Y));
	m_pGraphics->DrawLine(&linePen, (REAL)(m_MinStringPt[iGapNum].X + (m_iRectHalfWidth * 2)),
		(REAL)(m_MinStringPt[iGapNum].Y), (REAL)(m_rect.X + m_rect.Width), (REAL)(m_MinStringPt[iGapNum].Y));
	m_pGraphics->DrawLine(&linePen, (REAL)(m_TargetStringPt[iGapNum].X + (m_iRectHalfWidth * 2)),
		(REAL)(m_TargetStringPt[iGapNum].Y), (REAL)(m_rect.X + m_rect.Width), (REAL)(m_TargetStringPt[iGapNum].Y));
}

BOOL CDrawingHelper::InspectGap(int iGapNum, double dblGapValue)
{
	int iCalcPt_Y = 0;

	if (dblGapValue >= m_dblMaxValue[iGapNum])
		dblGapValue = m_dblMaxValue[iGapNum];
	if (dblGapValue <= m_dblMinValue[iGapNum])
		dblGapValue = m_dblMinValue[iGapNum];

	iCalcPt_Y = CalcPointY(iGapNum, dblGapValue, m_dblMaxValue[iGapNum], m_dblMinValue[iGapNum]);
	
	Color color = CStatus::Instance()->m_SpecInColor;
	SolidBrush solidBrush(color);

	if (dblGapValue > m_dblCalcHighTolerance[iGapNum])
		color = CStatus::Instance()->m_SpecBigOutColor;
	else if(dblGapValue < m_dblCalcLowTolerance[iGapNum])
		color = CStatus::Instance()->m_SpecSmallOutColor;
	else
		color = CStatus::Instance()->m_SpecInColor;
	
	solidBrush.SetColor(color);

	Point TrianglePt[3];
	TrianglePt[0].X = m_rect.X + m_rect.Width ;
	TrianglePt[0].Y = iCalcPt_Y;

	TrianglePt[1].X = m_rect.X + (m_rect.Width * 2);
	TrianglePt[1].Y = iCalcPt_Y - (m_rect.Width / 2);

	TrianglePt[2].X = m_rect.X + (m_rect.Width * 2);
	TrianglePt[2].Y = iCalcPt_Y + (m_rect.Width / 2);

	m_pGraphics->FillPolygon(&solidBrush, TrianglePt, 3);
	
	return 0;
}

void CDrawingHelper::CalcPoint(int iGapNum)
{
	m_MaxStringPt[iGapNum].X = (REAL)(m_rect.X - (m_iRectHalfWidth * 3));
	m_MaxStringPt[iGapNum].Y = (REAL)(m_rect.Y);

	m_MinStringPt[iGapNum].X = (REAL)(m_rect.X - (m_iRectHalfWidth * 3));
	m_MinStringPt[iGapNum].Y = (REAL)(m_rect.Y + m_rect.Height);

	int iCalcPt_Y = 0;
	iCalcPt_Y = CalcPointY(iGapNum, m_dblTarget[iGapNum], m_dblMaxValue[iGapNum], m_dblMinValue[iGapNum]);

	m_TargetStringPt[iGapNum].X = (REAL)(m_rect.X - (m_iRectHalfWidth * 3));
	m_TargetStringPt[iGapNum].Y = (REAL)iCalcPt_Y;
}

void CDrawingHelper::CalcVariableByUnit(int iGapNum)
{
	double dblTarget = 0;
	double dblGapValue, dblTolerance;

	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
	{
		if (CStatus::Instance()->GetIsGapSameSettings())
		{
			dblTarget = CStatus::Instance()->GetTarget(3);
			dblTolerance = CStatus::Instance()->GetTolerance(3);
		}
		else
		{
			dblTarget = CStatus::Instance()->GetTarget(iGapNum);
			dblTolerance = CStatus::Instance()->GetTolerance(iGapNum);
		}
	}
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		if (CStatus::Instance()->GetIsGapSameSettings())//
		{
			dblTarget = CStatus::Instance()->GetTarget(3);
			dblTolerance = CStatus::Instance()->GetTolerance(3);
		}
		else
		{
			dblTarget = CStatus::Instance()->GetTarget(iGapNum);
			dblTolerance = CStatus::Instance()->GetTolerance(iGapNum);
		}
	}

	if (iGapNum == 0)	dblGapValue = (double)CStatus::Instance()->m_curMeasureData.usGap1;
	if (iGapNum == 1)	dblGapValue = (double)CStatus::Instance()->m_curMeasureData.usGap2;
	if (iGapNum == 2)	dblGapValue = (double)CStatus::Instance()->m_curMeasureData.usGap3;

	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
	{
		if (CStatus::Instance()->GetIsOffsetMode())
		{
			double dblOffset = CStatus::Instance()->GetOffset(iGapNum);
			m_dblGapValueByUnit[iGapNum] = (dblGapValue / UNIT_MM_VALUE) + dblOffset;
		}
		else
		{
			m_dblGapValueByUnit[iGapNum] = (dblGapValue / UNIT_MM_VALUE);
		}
		m_dblTarget[iGapNum] = dblTarget;
		
		double dblViewRange = CStatus::Instance()->GetGapRangeView();
		m_dblMaxValue[iGapNum] = m_dblTarget[iGapNum] + dblViewRange;
		m_dblMinValue[iGapNum] = m_dblTarget[iGapNum] - dblViewRange;
		m_dblCalcHighTolerance[iGapNum] = m_dblTarget[iGapNum] + dblTolerance;
		m_dblCalcLowTolerance[iGapNum] = m_dblTarget[iGapNum] - dblTolerance;

		if (m_dblMaxValue[iGapNum] > 60)	m_dblMaxValue[iGapNum] = 60;
		if (m_dblMinValue[iGapNum] < 0)	m_dblMinValue[iGapNum] = 0;
		if (m_dblCalcHighTolerance[iGapNum] > 60)	m_dblCalcHighTolerance[iGapNum] = 60;
		if (m_dblCalcLowTolerance[iGapNum] < 0)	m_dblCalcLowTolerance[iGapNum] = 0;
	}
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
	{
		if (CStatus::Instance()->GetIsOffsetMode())
		{
			double dblOffset = CStatus::Instance()->GetOffset(iGapNum);
			m_dblGapValueByUnit[iGapNum] = (dblGapValue / UNIT_MM_VALUE * UNIT_INCH_VALUE) + dblOffset;
		}
		else
		{
			m_dblGapValueByUnit[iGapNum] = dblGapValue / UNIT_MM_VALUE * UNIT_INCH_VALUE;
		}
		m_dblTarget[iGapNum] = dblTarget;
		double dblViewRange = CStatus::Instance()->GetGapRangeView();
		m_dblMaxValue[iGapNum] = m_dblTarget[iGapNum] + dblViewRange;
		m_dblMinValue[iGapNum] = m_dblTarget[iGapNum] - dblViewRange;
		m_dblCalcHighTolerance[iGapNum] = m_dblTarget[iGapNum] + dblTolerance;
		m_dblCalcLowTolerance[iGapNum] = m_dblTarget[iGapNum] - dblTolerance;

		if (m_dblMaxValue[iGapNum] > 2.362)	m_dblMaxValue[iGapNum] = 2.362;
		if (m_dblMinValue[iGapNum] < 0)		m_dblMinValue[iGapNum] = 0;
		if (m_dblCalcHighTolerance[iGapNum] > 2.362)	m_dblCalcHighTolerance[iGapNum] = 2.362;
		if (m_dblCalcLowTolerance[iGapNum] < 0)		m_dblCalcLowTolerance[iGapNum] = 0;
	}
}
