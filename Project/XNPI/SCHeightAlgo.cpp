#include "stdafx.h"
#include "SCAlgoBase.h"
#include "SCHeightAlgo.h"
#include "LineEquation.h"
#include "SCResult.h"


CSCHeightAlgo::CSCHeightAlgo()
	: m_pLineEquation(NULL)
	, m_dblMax(0)
	, m_dblMin(0)
	, m_dblMean(0)
	, m_iCriteriaLow(10)
	, m_iCriteriaHi(100)
	, m_iScanDir(0)
	, m_umMax(0)
	, m_umMin(0)
	, m_umMean(0)
{
}


CSCHeightAlgo::~CSCHeightAlgo()
{
}

// return 0:Good, not 0: NG
int CSCHeightAlgo::Inspect(CMilBuffer * pMilORG, CMilBuffer * pMilView, CRect * pRectInsp)
{
	CSCAlgoBase::Inspect(pMilORG, pMilView, pRectInsp);

	TRACE(_T("\nRect L:%d, T:%d, R:%d, B:%d"), pRectInsp->left, pRectInsp->top, pRectInsp->right, pRectInsp->bottom);

	pMilView->SetColor(M_COLOR_YELLOW);
	Init();

	int rv = Measure();

	MmeasFree(m_milMarker);
	return rv;
}



int CSCHeightAlgo::Measure()
{
	// Allocate edge or stripe marker and setup search region and polarity.
	MIL_ID MilSystem = m_pMilBuffORG->m_milIDSystem;
	MIL_ID MilImage = m_pMilBuffORG->GetIDBuffer();
	int rv = 0;
	CPoint ptCenter = m_rectInsp.CenterPoint();

	FindEdge(m_rectInsp);
	rv = CheckCriteria();
	DrawResult(rv);
	return rv;
}

//***************************************************************************
// Draw the example legend.
//***************************************************************************
void CSCHeightAlgo::DrawLegend(MIL_ID MilOverlayImage, bool DrawSubPositions)
{
	MIL_DOUBLE TextPosX = 10.0;
	MIL_DOUBLE TextPosY = 10.0;
	MIL_DOUBLE StepY = 20.0;
	MgraFontScale(M_DEFAULT, 3, 3);

	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
	MgraColor(M_DEFAULT, M_COLOR_YELLOW);
	MgraText(M_DEFAULT, MilOverlayImage, TextPosX, TextPosY, MIL_TEXT("Result GOOD - YELLOW"));
	TextPosY += StepY;
	MgraColor(M_DEFAULT, M_COLOR_RED);
	MgraText(M_DEFAULT, MilOverlayImage, TextPosX, TextPosY + TextPosY, MIL_TEXT("Result NG - RED"));

	MgraFontScale(M_DEFAULT, 2, 2);
	MgraColor(M_DEFAULT, M_COLOR_YELLOW);
	double resultX = m_rectInsp.left;
	double resultY = m_rectInsp.bottom + StepY;
	CString strMsg;
	if (!m_pLineEquation->IsSetCoe())
	{
		strMsg.Format(_T("H.Edge is not Founded"));
		MgraText(M_DEFAULT, MilOverlayImage, resultX, resultY, strMsg.GetBuffer());
		return;
	}

	strMsg.Format(_T("Mean:%5.2fp"), m_dblMean);
	MgraText(M_DEFAULT, MilOverlayImage, resultX, resultY, strMsg.GetBuffer());
	resultY += StepY;
	strMsg.Format(_T("Max:%5.2fp"), m_dblMax);
	MgraText(M_DEFAULT, MilOverlayImage, resultX, resultY, strMsg.GetBuffer());
	resultY += StepY;
	strMsg.Format(_T("Min:%5.2fp"), m_dblMin);
	MgraText(M_DEFAULT, MilOverlayImage, resultX, resultY, strMsg.GetBuffer());

	if (m_bIsPixelCal)
	{
		resultY += StepY;
		resultY += StepY;
		strMsg.Format(_T("Mean:%5.2fmm"), m_umMean);
		MgraText(M_DEFAULT, MilOverlayImage, resultX, resultY, strMsg.GetBuffer());
		resultY += StepY;
		strMsg.Format(_T("Max:%5.2fmm"), m_umMax);
		MgraText(M_DEFAULT, MilOverlayImage, resultX, resultY, strMsg.GetBuffer());
		resultY += StepY;
		strMsg.Format(_T("Min:%5.2fmm"), m_umMin);
		MgraText(M_DEFAULT, MilOverlayImage, resultX, resultY, strMsg.GetBuffer());
	}
}

int CSCHeightAlgo::CheckCriteria()
{
	double dblLength[COUNT_HEIGHTSUBREGION];
	BOOL bFounded[COUNT_HEIGHTSUBREGION];
	if (m_nFound == 0)return -1;
	for (int i = 0; i < COUNT_HEIGHTSUBREGION; i++)
	{
		bFounded[i] = TRUE;
		if (m_posEdgeX[i] == 0.0) bFounded[i] = FALSE;
		if (m_posEdgeY[i] == 0.0) bFounded[i] = FALSE;
	}


	for (int i = 0; i < COUNT_HEIGHTSUBREGION; i++)
	{
		dblLength[i] = sqrt(pow(m_posEdgeY[i] - m_pLineEquation->GetY(m_posEdgeX[i]), 2));
		if ((m_iCriteriaLow < (int)dblLength[i]) && ((int)dblLength[i] < m_iCriteriaHi))
		{
			if (bFounded[i])
				m_posResult[i] = 0;
			else
				m_posResult[i] = -1;

		}else
			m_posResult[i] = -1;
	}

	m_dblMax = m_dblMin = dblLength[0];
	double sum = 0;
	for (int i = 0; i < COUNT_HEIGHTSUBREGION; i++)
	{
		if (m_dblMax < dblLength[i])m_dblMax = dblLength[i];
		if (m_dblMin > dblLength[i])m_dblMin = dblLength[i];
		sum += dblLength[i];
	}
	m_dblMean = sum / COUNT_HEIGHTSUBREGION;

	for (int i = 0; i < COUNT_HEIGHTSUBREGION; i++)
	{
		if (m_posResult[i] == -1) return -1;
	}

	if (m_bIsPixelCal)
	{
		m_umMin = m_dblMin * m_dblPixelCalData.dblCY;
		m_umMax = m_dblMax * m_dblPixelCalData.dblCY;
		m_umMean =m_dblMean * m_dblPixelCalData.dblCY;
	}
	else
	{
		m_umMin = 0.0;
		m_umMax = 0.0;
		m_umMean = 0.0;
	}
	return 0;
}

int CSCHeightAlgo::DrawSubPos(double *pDblXPos, double *pDblYPos)
{
	for (int i = 0; i < COUNT_HEIGHTSUBREGION; i++)
	{
		if (m_posResult[i] != -1)
		{
			//Good
			m_pMilBuffView->SetColor(M_COLOR_YELLOW);

		}
		else
		{
			//NG
			m_pMilBuffView->SetColor(M_COLOR_RED);

		}
		for (int j = 0; j < 3; j++)
		{
			m_pMilBuffView->Line(pDblXPos[i] - 20, pDblYPos[i] - 1 + j, pDblXPos[i] + 20, pDblYPos[i] - 1 + j);
			m_pMilBuffView->Line(pDblXPos[i] - 1 + j, pDblYPos[i] - 20, pDblXPos[i] - 1 + j, pDblYPos[i] + 20);
		}
	}

	return 0;
}

void CSCHeightAlgo::DrawResult(int rv)
{
	// Draw the contour of the measurement region.
	bool DrawSubPositions = true;
	MgraColor(M_DEFAULT, M_COLOR_BLUE);

	CRect DoubleRect = m_rectInsp;

	DrawROI(m_pMilBuffView, rv);

//	MmeasDraw(M_DEFAULT, m_milMarker, m_milBufDisp, M_DRAW_SEARCH_REGION, M_DEFAULT, M_MARKER);

	// Draw the search region orientation
//	MmeasDraw(M_DEFAULT, m_milMarker, m_milBufDisp, M_DRAW_SEARCH_DIRECTION, M_DEFAULT, M_MARKER);

	if (m_nFound >= 1)
	{
		if (DrawSubPositions)
		{
			// Draw the sub edges position used to fit the marker.
			DrawSubPos(m_posEdgeX, m_posEdgeY);
			//MgraColor(M_DEFAULT, M_COLOR_YELLOW);
			//MmeasDraw(M_DEFAULT, m_milMarker, m_milBufDisp, M_DRAW_SUB_POSITIONS, M_DEFAULT, M_RESULT);
		}
		DrawLegend(m_milBufDisp, DrawSubPositions);
	}
	else
	{
		MgraColor(M_DEFAULT, M_COLOR_RED);
		//juba		MgraText(M_DEFAULT, MilOverlayImage, ERROR_TEXT_POS_X, ERROR_TEXT_POS_Y, MIL_TEXT("The marker was not found."));
	}

}


int CSCHeightAlgo::Init()
{
	m_nFound = 0;
	m_milMarker = M_NULL;
	m_milBufDisp = M_NULL;

	m_milBufDisp = m_pMilBuffView->GetIDBuffer();

	return 0;
}


int CSCHeightAlgo::FindEdge(CRect rectUpper)
{
	MIL_ID MilSystem = m_pMilBuffORG->m_milIDSystem;
	MIL_ID MilImage = m_pMilBuffORG->GetIDBuffer();
	CPoint ptCenter = rectUpper.CenterPoint();
	m_milMarker = MmeasAllocMarker(MilSystem, M_EDGE, M_DEFAULT, M_NULL);


	MmeasSetMarker(m_milMarker, M_POLARITY, M_ANY, M_DEFAULT);
	MmeasSetMarker(m_milMarker, M_BOX_CENTER, ptCenter.x, ptCenter.y);
	MmeasSetMarker(m_milMarker, M_BOX_SIZE, rectUpper.Width(), rectUpper.Height());
	MmeasSetMarker(m_milMarker, M_SUB_REGIONS_NUMBER, COUNT_HEIGHTSUBREGION, M_NULL);
	MmeasSetMarker(m_milMarker, M_NUMBER, M_DEFAULT, M_NULL);
	MmeasSetMarker(m_milMarker, M_EDGEVALUE_MIN, M_DEFAULT, M_NULL);
	if(m_iScanDir==0)
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

	if (m_nFound >= 1)
	{
		MmeasGetResult(m_milMarker, M_SUB_EDGES_POSITION + M_EDGE_FIRST + M_TYPE_MIL_DOUBLE, m_posEdgeX, m_posEdgeY);

	}

	return 0;
}

int CSCHeightAlgo::DrawROI(CMilBuffer *pMilView, int iResult)
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


void CSCHeightAlgo::SetLineEquation(CLineEquation * pLineEquation)
{
	m_pLineEquation = pLineEquation;
}


void CSCHeightAlgo::GetResult(CSCResult * pResult)
{
	pResult->dblMean = m_dblMean;
	pResult->dblMin = m_dblMin;
	pResult->dblMax = m_dblMax;
	pResult->bIsPixelCal = m_bIsPixelCal;
	if (m_bIsPixelCal)
	{
		pResult->umMax = m_umMax;
		pResult->umMean = m_umMean;
		pResult->umMin = m_umMin;
	}
	else
	{
		pResult->umMax = 0.0;
		pResult->umMean = 0.0;
		pResult->umMin = 0.0;
	}
}
