#include "stdafx.h"
#include "resource.h"
#include "SCAlgoBase.h"
#include "SCAlignAlgo.h"
#include "TabDetailLogDlg.h"


CSCAlignAlgo::CSCAlignAlgo()
	: m_iScanDir(0)
	, m_iStartTime(0)
	, m_iEndTime(0)
	, m_iCriteria(30)
	, m_nFound(0)
	, m_dblMin(0)
	, m_dblMax(0)
	, m_dblMean(0)
	, m_umMin(0)
	, m_umMax(0)
	, m_umMean(0)
{
}


CSCAlignAlgo::~CSCAlignAlgo()
{
}

// return 0:Good, not 0: NG
int CSCAlignAlgo::Inspect(CMilBuffer * pMilORG, CMilBuffer * pMilView, CRect * pRectInsp)
{
	CSCAlgoBase::Inspect(pMilORG, pMilView, pRectInsp);
	int rv = 0;

	TRACE(_T("\nRect L:%d, T:%d, R:%d, B:%d"), pRectInsp->left, pRectInsp->top, pRectInsp->right, pRectInsp->bottom);
	Init();
	Measure();
	rv = CheckCriteria();
	DrawResult(rv);
	MmeasFree(m_milMarker);
	return rv;
}



void CSCAlignAlgo::Measure()
{
	// Allocate edge or stripe marker and setup search region and polarity.
	MIL_ID MilSystem = m_pMilBuffORG->m_milIDSystem;
	MIL_ID MilImage = m_pMilBuffORG->GetIDBuffer();
	CPoint ptCenter = m_rectInsp.CenterPoint();
	m_milMarker = MmeasAllocMarker(MilSystem, M_STRIPE, M_DEFAULT, M_NULL);

	MmeasSetMarker(m_milMarker, M_POLARITY, M_ANY, M_DEFAULT);
	MmeasSetMarker(m_milMarker, M_BOX_CENTER, ptCenter.x, ptCenter.y);
	MmeasSetMarker(m_milMarker, M_BOX_SIZE, m_rectInsp.Width(), m_rectInsp.Height());
	MmeasSetMarker(m_milMarker, M_SUB_REGIONS_NUMBER, COUNT_SUBREGION, M_NULL);
	MmeasSetMarker(m_milMarker, M_NUMBER, M_DEFAULT, M_NULL);
	MmeasSetMarker(m_milMarker, M_EDGEVALUE_MIN, M_DEFAULT, M_NULL);
	MmeasSetMarker(m_milMarker, M_FILTER_TYPE, M_PREWITT, M_NULL);
	if (m_iScanDir == 0)
	{
		MmeasSetMarker(m_milMarker, M_BOX_ANGLE, 0, M_NULL);
		MmeasSetMarker(m_milMarker, M_ORIENTATION, M_HORIZONTAL, M_NULL);
	}
	else
	{
		MmeasSetMarker(m_milMarker, M_ORIENTATION, M_VERTICAL, M_NULL);
	}

	// Find the marker and compute all applicable measurements.
	MmeasFindMarker(M_DEFAULT, MilImage, m_milMarker, M_DEFAULT);

	// If occurrence is found, show the results.
	MmeasGetResult(m_milMarker, M_NUMBER + M_TYPE_MIL_INT, &m_nFound, M_NULL);

	memset(m_posFirstX, 0, sizeof(m_posFirstX));
	memset(m_posFirstY, 0, sizeof(m_posFirstY));
	memset(m_posSecondX, 0, sizeof(m_posSecondX));
	memset(m_posSecondY, 0, sizeof(m_posSecondY));

	if (m_nFound >= 1)
	{
		MmeasGetResult(m_milMarker, M_SUB_EDGES_POSITION + M_EDGE_FIRST + M_TYPE_MIL_DOUBLE, m_posFirstX, m_posFirstY);
		MmeasGetResult(m_milMarker, M_SUB_EDGES_POSITION + M_EDGE_SECOND + M_TYPE_MIL_DOUBLE, m_posSecondX, m_posSecondY);

	}
}

//***************************************************************************
// Draw the example legend.
//***************************************************************************
void CSCAlignAlgo::DrawLegend(MIL_ID MilOverlayImage, bool DrawSubPositions)
{
	MIL_DOUBLE TextPosX = 10.0;
	MIL_DOUBLE TextPosY = 10.0;
	MIL_DOUBLE StepY = 19.0;
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
	strMsg.Format(_T("Mean:%5.2f, Max:%5.2f, Min:%5.2f"), m_dblMean, m_dblMax, m_dblMin);
	MgraText(M_DEFAULT, MilOverlayImage, resultX, resultY, strMsg.GetBuffer());
	if (m_bIsPixelCal)
	{
		resultY += StepY;
		strMsg.Format(_T("Mean:%5.2fum, Max:%5.2fmm, Min:%5.2fmm"), m_umMean, m_umMax, m_umMin);
		MgraText(M_DEFAULT, MilOverlayImage, resultX, resultY, strMsg.GetBuffer());
	}
}

int CSCAlignAlgo::CheckCriteria()
{
	double dblLength[COUNT_SUBREGION];
	BOOL bFounded[COUNT_SUBREGION];
	if (m_nFound == 0)return -1;

	memset(bFounded, 0, sizeof(bFounded));
	for (int i = 0; i < COUNT_SUBREGION; i++)
	{
		bFounded[i] = TRUE;
		if (m_posFirstX[i] == 0.0) bFounded[i] = FALSE;
		if (m_posFirstY[i] == 0.0) bFounded[i] = FALSE;
		if (m_posSecondX[i] == 0.0) bFounded[i] = FALSE;
		if (m_posSecondY[i] == 0.0) bFounded[i] = FALSE;
	}

	for (int i = 0; i < COUNT_SUBREGION; i++)
	{
		dblLength[i] = sqrt(pow(m_posFirstX[i] - m_posSecondX[i], 2) + pow(m_posFirstY[i] - m_posSecondY[i], 2));
		if ((int)dblLength[i] > m_iCriteria)
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
	for (int i = 0; i < COUNT_SUBREGION; i++)
	{
		if (m_dblMax < dblLength[i])m_dblMax = dblLength[i];
		if (m_dblMin > dblLength[i])m_dblMin = dblLength[i];
		sum += dblLength[i];
	}
	m_dblMean = sum / COUNT_SUBREGION;

	if (m_bIsPixelCal)
	{
		if (m_iScanDir == 0)
		{
			m_umMax = m_dblMax * m_dblPixelCalData.dblCY;
			m_umMin = m_dblMin * m_dblPixelCalData.dblCY;
			m_umMean = m_dblMean * m_dblPixelCalData.dblCY;
		}
		else
		{
			m_umMax = m_dblMax * m_dblPixelCalData.dblCX;
			m_umMin = m_dblMin * m_dblPixelCalData.dblCX;
			m_umMean = m_dblMean * m_dblPixelCalData.dblCX;
		}
	}
	else
	{
		m_umMax = 0.0;
		m_umMin = 0.0;
		m_umMean = 0.0;
	}

	for (int i = 0; i < COUNT_SUBREGION; i++)
	{
		if (m_posResult[i] == -1) return -1;
	}

	return 0;
}


void CSCAlignAlgo::DrawResult(int rv)
{
	// Draw the contour of the measurement region.
	bool DrawSubPositions = true;

	CRect DoubleRect = m_rectInsp;

	DrawROI(m_pMilBuffView, rv);

//	MmeasDraw(M_DEFAULT, m_milMarker, m_milIDBufDisp, M_DRAW_SEARCH_REGION, M_DEFAULT, M_MARKER);

	// Draw the search region orientation
	MmeasDraw(M_DEFAULT, m_milMarker, m_milIDBufDisp, M_DRAW_SEARCH_DIRECTION, M_DEFAULT, M_MARKER);

	if (m_nFound >= 1)
	{
		MmeasGetResult(m_milMarker, M_SUB_EDGES_POSITION + M_EDGE_FIRST + M_TYPE_MIL_DOUBLE, m_posFirstX, m_posFirstY);
		MmeasGetResult(m_milMarker, M_SUB_EDGES_POSITION + M_EDGE_SECOND + M_TYPE_MIL_DOUBLE, m_posSecondX, m_posSecondY);

		if (DrawSubPositions)
		{
			// 화면에 보이게 하기 위해 여러번그림
			DrawSubPos(m_posFirstX, m_posFirstY, m_posSecondX, m_posSecondY);
			//결과를 그리기
			//MgraColor(M_DEFAULT, M_COLOR_YELLOW);
			//MmeasDraw(M_DEFAULT, m_milMarker, m_milIDBufDisp, M_DRAW_SUB_POSITIONS, M_DEFAULT, M_RESULT);
		}
		DrawLegend(m_milIDBufDisp, DrawSubPositions);
	}
	else
	{
		MgraColor(M_DEFAULT, M_COLOR_RED);
		//MgraText(M_DEFAULT, MilOverlayImage, ERROR_TEXT_POS_X, ERROR_TEXT_POS_Y, MIL_TEXT("The marker was not found."));
	}

}


int CSCAlignAlgo::Init()
{
	m_nFound = 0;
	m_milMarker = M_NULL;
	m_milIDBufDisp = M_NULL;

	m_milIDBufDisp = m_pMilBuffView->GetIDBuffer();

	return 0;
}

int CSCAlignAlgo::DrawSubPos(double *pDblXPos, double *pDblYPos, double *pDbl2XPos, double *pDbl2YPos)
{
	for (int i = 0; i < COUNT_SUBREGION; i++)
	{
		if (m_posResult[i]!=-1)
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
			m_pMilBuffView->Line(pDbl2XPos[i] - 20, pDbl2YPos[i] - 1 + j, pDbl2XPos[i] + 20, pDbl2YPos[i] - 1 + j);
			m_pMilBuffView->Line(pDbl2XPos[i] - 1 + j, pDbl2YPos[i] - 20, pDbl2XPos[i] - 1 + j, pDbl2YPos[i] + 20);
		}
	}
	return 0;
}

int CSCAlignAlgo::DrawROI(CMilBuffer *pMilView, int iResult)
{
	CRect DoubleRect = m_rectInsp;
	if(!iResult)
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


void CSCAlignAlgo::GetResult(CSCResult * pResult)
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
