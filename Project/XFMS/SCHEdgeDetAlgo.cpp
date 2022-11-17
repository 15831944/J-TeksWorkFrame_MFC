#include "stdafx.h"
#include "SCAlgoBase.h"
#include "SCHEdgeDetAlgo.h"
#include "LineEquation.h"


CSCHEdgeDetAlgo::CSCHEdgeDetAlgo()
	: m_iHEdgeScanDir(0),
	m_coeA(M_NULL),
	m_coeB(M_NULL),
	m_coeC(M_NULL), 
	m_pLineEquation(NULL)
	, m_iDir(0)
	, m_bUseCenterLine(FALSE)
{
}


CSCHEdgeDetAlgo::~CSCHEdgeDetAlgo()
{
}

// return 0:Good, not 0: NG
int CSCHEdgeDetAlgo::Inspect(CMilBuffer * pMilORG, CMilBuffer * pMilView, CRect * pRectInsp)
{
	CSCAlgoBase::Inspect(pMilORG, pMilView, pRectInsp);

	TRACE(_T("\nRect L:%d, T:%d, R:%d, B:%d"), pRectInsp->left, pRectInsp->top, pRectInsp->right, pRectInsp->bottom);

	pMilView->SetColor(M_COLOR_YELLOW);
	Init();
	if (m_bUseCenterLine)
	{
		m_nFound = 1;
		m_coeA = 0, m_coeB = 1, m_coeC = m_rectInsp.CenterPoint().y;
		if (m_pLineEquation)m_pLineEquation->SetCoe(m_coeA, m_coeB, m_coeC, 1);
		DrawROI(m_pMilBuffView, 0);
		DrawHLine(m_pMilBuffView);
	}
	else
	{
		Measure();
	}
	return 0;
}



void CSCHEdgeDetAlgo::Measure()
{
	// Allocate edge or stripe marker and setup search region and polarity.
	MIL_ID MilSystem = m_pMilBuffORG->m_milIDSystem;
	MIL_ID MilImage = m_pMilBuffORG->GetIDBuffer();

	int rv = 0;
	rv = FindHEdge(m_rectInsp);
	DrawResult(rv);
	
	MmeasFree(m_milMarker);
}

//***************************************************************************
// Draw the example legend.
//***************************************************************************
void CSCHEdgeDetAlgo::DrawLegend(MIL_ID MilOverlayImage, bool DrawSubPositions)
{
	MIL_DOUBLE TextPosX = 10.0;
	MIL_DOUBLE TextPosY = 10.0;
	MIL_DOUBLE StepY = 15.0;

	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
	MgraColor(M_DEFAULT, M_COLOR_BLUE);
	MgraText(M_DEFAULT, MilOverlayImage, TextPosX, TextPosY, MIL_TEXT("Measurement Search Region - Blue"));
	TextPosY += StepY;

	if (DrawSubPositions)
	{
		MgraColor(M_DEFAULT, M_COLOR_GREEN);
		MgraText(M_DEFAULT, MilOverlayImage, TextPosX, TextPosY, MIL_TEXT("Subedges Positions        - Green"));
		TextPosY += StepY;
	}
}

void CSCHEdgeDetAlgo::DrawResult(int rv)
{
	// Draw the contour of the measurement region.
	bool DrawSubPositions = true;
	MgraColor(M_DEFAULT, M_COLOR_BLUE);

	CRect DoubleRect = m_rectInsp;

	// Draw the search region orientation
	DrawROI(m_pMilBuffView, rv);
	//	MmeasDraw(M_DEFAULT, m_milMarker, m_milBufDisp, M_DRAW_SEARCH_REGION, M_DEFAULT, M_MARKER);
	MmeasDraw(M_DEFAULT, m_milMarker, m_milBufDisp, M_DRAW_SEARCH_DIRECTION, M_DEFAULT, M_MARKER);
	MgraColor(M_DEFAULT, M_COLOR_GREEN);
	DrawHLine(m_pMilBuffView);
	//MmeasDraw(M_DEFAULT, m_milMarker, m_milBufDisp, M_DRAW_EDGES, M_DEFAULT, M_RESULT);

	// Draw a cross at the found position.
	MgraColor(M_DEFAULT, M_COLOR_MAGENTA);
	MmeasDraw(M_DEFAULT, m_milMarker, m_milBufDisp, M_DRAW_POSITION, M_DEFAULT, M_RESULT);

	DrawSubPositions = FALSE;

	if (m_nFound >= 1)
	{
//		DrawLegend(m_milBufDisp, DrawSubPositions);
	}
	else
	{
		MgraColor(M_DEFAULT, M_COLOR_RED);
		MgraText(M_DEFAULT, m_milBufDisp, 10, 10, MIL_TEXT("Horizontal Edge was not found."));
	}

}


int CSCHEdgeDetAlgo::Init()
{
	m_nFound = 0;
	m_milMarker = M_NULL;
	m_milBufDisp = M_NULL;
	m_coeA = 0, m_coeB = 0, m_coeC = 0;
	m_dblRectFound.Set(0, 0, 0, 0);

	m_milBufDisp = m_pMilBuffView->GetIDBuffer();

	return 0;
}

int CSCHEdgeDetAlgo::FindHEdge(CRect rectUpper)
{
	MIL_ID MilSystem = m_pMilBuffORG->m_milIDSystem;
	MIL_ID MilImage = m_pMilBuffORG->GetIDBuffer();
	CPoint ptCenter = rectUpper.CenterPoint();
	m_milMarker = MmeasAllocMarker(MilSystem, M_EDGE, M_DEFAULT, M_NULL);

	MmeasSetMarker(m_milMarker, M_POLARITY, M_ANY, M_DEFAULT);
//	MmeasSetMarker(m_milMarker, M_POLARITY, M_NEGATIVE, M_DEFAULT);
	MmeasSetMarker(m_milMarker, M_BOX_CENTER, ptCenter.x, ptCenter.y);
	MmeasSetMarker(m_milMarker, M_BOX_SIZE, rectUpper.Width(), rectUpper.Height());
	MmeasSetMarker(m_milMarker, M_SUB_REGIONS_NUMBER, 1, M_NULL);
	MmeasSetMarker(m_milMarker, M_NUMBER, M_DEFAULT, M_NULL);
	MmeasSetMarker(m_milMarker, M_EDGEVALUE_MIN, M_DEFAULT, M_NULL);
	MmeasSetMarker(m_milMarker, M_FILTER_TYPE, M_PREWITT, M_NULL);
	if (m_iHEdgeScanDir == 0)
		MmeasSetMarker(m_milMarker, M_BOX_ANGLE, 0, M_NULL);
	else
	{
		MmeasSetMarker(m_milMarker, M_BOX_ANGLE, 180, M_NULL);
		MmeasSetMarker(m_milMarker, M_BOX_ANGLE_DELTA_NEG, 180, M_NULL);
	}

	MmeasSetMarker(m_milMarker, M_ORIENTATION, M_HORIZONTAL, M_NULL);

	// Find the marker and compute all applicable measurements.
	MmeasFindMarker(M_DEFAULT, MilImage, m_milMarker, M_DEFAULT);

	// If occurrence is found, show the results.
	MmeasGetResult(m_milMarker, M_NUMBER + M_TYPE_MIL_INT, &m_nFound, M_NULL);

	MmeasGetResult(m_milMarker, M_LINE_A, &m_coeA, M_NULL);
	MmeasGetResult(m_milMarker, M_LINE_B, &m_coeB, M_NULL);
	MmeasGetResult(m_milMarker, M_LINE_C, &m_coeC, M_NULL);
	MIL_DOUBLE dblLeft, dblRight, dblTop, dblBottom;
	MmeasGetResult(m_milMarker, M_BOX_CORNER_TOP_LEFT, &dblLeft, &dblTop);
	MmeasGetResult(m_milMarker, M_BOX_CORNER_BOTTOM_RIGHT, &dblRight, &dblBottom);

	m_dblRectFound.Set(dblLeft, dblTop, dblRight, dblBottom);

	if (m_nFound >= 1)
	{
		if (m_pLineEquation)m_pLineEquation->SetCoe(m_coeA, m_coeB, m_coeC, m_iHEdgeScanDir);
		return 0;
	}
	else return -1;
}

int CSCHEdgeDetAlgo::DrawROI(CMilBuffer *pMilView, int iResult)
{
	CRect DoubleRect = m_rectInsp;
	if (!iResult)
		pMilView->SetColor(M_COLOR_YELLOW);
	else
		pMilView->SetColor(M_COLOR_RED);

	for (int i = 0; i < 5; i++)
	{
		DoubleRect.InflateRect(1, 1);
		pMilView->Rect(&DoubleRect);
	}
	return 0;
}

void CSCHEdgeDetAlgo::DrawHLine(CMilBuffer *pMilView)
{
	//double dblStartY = CalcLine(m_dblRectFound.dblLeft);
	//double dblEndY = CalcLine(m_dblRectFound.dblRight);
	//pMilView->Line(m_dblRectFound.dblLeft, dblStartY - 1, m_dblRectFound.dblRight, dblEndY - 1);
	//pMilView->Line(m_dblRectFound.dblLeft, dblStartY, m_dblRectFound.dblRight, dblEndY);
	//pMilView->Line(m_dblRectFound.dblLeft, dblStartY + 1, m_dblRectFound.dblRight, dblEndY + 1);
	double dblStartY = CalcLine(0);
	double dblEndY = CalcLine(CAM_HRES-1);

	pMilView->Line(0, dblStartY - 1, CAM_HRES - 1, dblEndY - 1);
	pMilView->Line(0, dblStartY, CAM_HRES - 1, dblEndY);
	pMilView->Line(0, dblStartY + 1, CAM_HRES - 1, dblEndY + 1);
}

double CSCHEdgeDetAlgo::CalcLine(double dblPosX)
{
	if (!(m_pLineEquation->IsSetCoe())) return -1;
	return  m_pLineEquation->GetY(dblPosX);
}


int CSCHEdgeDetAlgo::Draw(BOOL bIsFinded)
{
	return 0;
}
