#include "stdafx.h"
#include "PlotLiveChart.h"
#include "Status.h"
#include "Repository.h"

#define P1LINECOLOR	RGB(255, 0, 0)
#define P2LINECOLOR	RGB(0, 0, 255)
#define P3LINECOLOR	RGB(128, 0, 128)
#define P4LINECOLOR	RGB(255, 255, 128)
#define P5LINECOLOR	RGB(255, 155, 10)
#define P6LINECOLOR	RGB(155, 255, 100)

#define X_GRID_COUNT 32
#define Y_GRID_COUNT 4
CPlotLiveChart::CPlotLiveChart()
	: m_pDC(NULL)
	, m_dblXGridSpace(0)
	, m_dblYGridSpace(0)
	, m_iGraphXRange(0)
	, m_iGraphYMax(0)
	, m_iGraphYMin(0)
	, m_pHistoGraph(NULL)
	, m_nCountOfData(0)
	, m_dblHRate(0)
	, m_iDiffYRange(0)
	, m_nCountOfSensors(0)
	, m_iValidHisto(0)
{
}


CPlotLiveChart::~CPlotLiveChart()
{
}


void CPlotLiveChart::SetDC(CDC *pDC)
{
	m_pDC = pDC;
}


void CPlotLiveChart::SetRect(CRect rectClient)
{
	m_rectClient = rectClient;
}


int CPlotLiveChart::Calc()
{
	m_rectPlot = m_rectClient;
	m_rectPlot.DeflateRect(0, 20);
	
	m_dblYGridSpace = m_rectPlot.Height() / (double)Y_GRID_COUNT;
	m_dblXGridSpace = m_dblYGridSpace;

	double dblPlotWidth = m_rectPlot.Width();
	double dblGraphMargin = ((dblPlotWidth - m_dblXGridSpace * X_GRID_COUNT) / 2);
	m_rectPlot.DeflateRect(dblGraphMargin, 0);
	m_iGraphXRange = CStatus::Instance()->m_ulGraphXRange;
	m_iGraphYMax = CStatus::Instance()->m_ulGraphYMax;
	m_iGraphYMin = CStatus::Instance()->m_ulGraphYMin;
	double dblDataCountForPixel = m_iGraphXRange / dblPlotWidth;
	m_nCountOfSensors = CStatus::Instance()->m_nCountOfSensors;

	int widthGraph = (int)dblPlotWidth;
	if (m_pHistoGraph)
	{
		for (int index = 0; index < m_nCountOfSensors; index++)
			delete [] m_pHistoGraph[index];
		delete [] m_pHistoGraph;
	}
	m_pHistoGraph = new int*[6]();
	for (int index = 0; index < m_nCountOfSensors; index++)
		m_pHistoGraph[index] = new int[widthGraph]();


	POSITION pos;
	tXFMSRecord *pXMFMSRecord;
	CPtrList *pList = &(CStatus::Instance()->GetRepository()->m_DataRecords.m_XFMSRecords);

	int i = 0, iHistoIndex = 0;
	double dblIdxCount = 0;
	double sumTemp[6][2];
	memset(sumTemp, 0, sizeof(sumTemp));
	double dblDataCount = dblDataCountForPixel;
	m_nCountOfData = pList->GetCount();
	m_iValidHisto = (int)(m_nCountOfData / dblDataCountForPixel);

	for (pos = pList->GetHeadPosition(); pos != NULL;)
	{		
		pXMFMSRecord = (tXFMSRecord *)pList->GetNext(pos);
		
		if (dblDataCount > i)
		{
		}
		else
		{
			for (int index = 0; index < m_nCountOfSensors; index++)
				m_pHistoGraph[index][iHistoIndex] = sumTemp[index][0] / sumTemp[index][1];
			iHistoIndex++;
			memset(sumTemp, 0, sizeof(sumTemp));
			dblDataCount += dblDataCountForPixel;
		}

		for (int index = 0; index < m_nCountOfSensors; index++)
		{
			sumTemp[index][0] += pXMFMSRecord->Temperature[index];
			sumTemp[index][1] += 1;
		}
		i++;
	}

	if (dblDataCount > i)
	{
		for (int index = 0; index < m_nCountOfSensors; index++)
		{
			if(sumTemp[index][0] != 0)
				m_pHistoGraph[index][iHistoIndex] = sumTemp[index][0] / sumTemp[index][1];
			else
				m_pHistoGraph[index][iHistoIndex] = 0;
		}
	}

	m_iDiffYRange = m_iGraphYMax - m_iGraphYMin;
	m_dblHRate = m_rectPlot.Height()/ (double)m_iDiffYRange;
	return 0;
}


int CPlotLiveChart::DrawBG()
{
	DrawGrid();
	DrawLegend();
	return 0;
}


int CPlotLiveChart::Draw()
{
	CDC *pDC;
	pDC = m_pDC;
	int StartIndex = 0;
	double dblHeight;

	CPen myPen[6];
	COLORREF clColor[6];
	clColor[0] = P1LINECOLOR;
	clColor[1] = P2LINECOLOR;
	clColor[2] = P3LINECOLOR;
	clColor[3] = P4LINECOLOR;
	clColor[4] = P5LINECOLOR;
	clColor[5] = P6LINECOLOR;

	myPen[0].CreatePen(PS_SOLID, 3, P1LINECOLOR);
	myPen[1].CreatePen(PS_SOLID, 3, P2LINECOLOR);
	myPen[2].CreatePen(PS_SOLID, 3, P3LINECOLOR);
	myPen[3].CreatePen(PS_SOLID, 3, P4LINECOLOR);
	myPen[4].CreatePen(PS_SOLID, 3, P5LINECOLOR);
	myPen[5].CreatePen(PS_SOLID, 3, P6LINECOLOR);
	CPen* pOldPen = pDC->SelectObject(&myPen[0]);

	for (int nCh = 0; nCh < m_nCountOfSensors; nCh++)
	{
		pDC->SelectObject(&myPen[nCh]);
		pDC->MoveTo(CPoint(m_rectPlot.left, m_rectPlot.bottom));
		for (int index = 0; index < m_iValidHisto-1; index++)
		{
			dblHeight = m_dblHRate * (m_pHistoGraph[nCh][index] / 10.);
			pDC->LineTo(CPoint(m_rectPlot.left + index, m_rectPlot.bottom - dblHeight));
		}
	}

	pDC->SelectObject(pOldPen);
	for(int index=0;index<6;index++)
		myPen[index].DeleteObject();

	return 0;
}

void CPlotLiveChart::DrawLegend()
{
	int iyLineTag = 20;
	CPoint posText(m_rectClient.left+10, iyLineTag);
	CRect rectColor(CPoint(m_rectClient.left +50, iyLineTag + 5), CPoint(m_rectClient.left+60, iyLineTag + 5 + 5));
	int spaceY = 15;

	if(m_nCountOfSensors >= 1)m_pDC->TextOutW(posText.x, posText.y, _T("P1"));
	posText.y += spaceY;
	if (m_nCountOfSensors >= 2)m_pDC->TextOutW(posText.x, posText.y, _T("P2"));
	posText.y += spaceY;
	if (m_nCountOfSensors >= 3)m_pDC->TextOutW(posText.x, posText.y, _T("P3"));
	posText.y += spaceY;
	if (m_nCountOfSensors >= 4)m_pDC->TextOutW(posText.x, posText.y, _T("P4"));
	posText.y += spaceY;
	if (m_nCountOfSensors >= 5)m_pDC->TextOutW(posText.x, posText.y, _T("P5"));
	posText.y += spaceY;
	if (m_nCountOfSensors >= 6)m_pDC->TextOutW(posText.x, posText.y, _T("P6"));

	CPen myPenP1, myPenP2, myPenP3, myPenP4, myPenP5, myPenP6;
	myPenP1.CreatePen(PS_SOLID, 1, P1LINECOLOR);
	myPenP2.CreatePen(PS_SOLID, 1, P2LINECOLOR);
	myPenP3.CreatePen(PS_SOLID, 1, P3LINECOLOR);
	myPenP4.CreatePen(PS_SOLID, 1, P4LINECOLOR);
	myPenP5.CreatePen(PS_SOLID, 1, P5LINECOLOR);
	myPenP6.CreatePen(PS_SOLID, 1, P6LINECOLOR);

	CPen* pOldPen = m_pDC->SelectObject(&myPenP1);
	if (m_nCountOfSensors >= 1)m_pDC->Rectangle(rectColor);
	rectColor.OffsetRect(0, spaceY);
	m_pDC->SelectObject(&myPenP2);
	if (m_nCountOfSensors >= 2)m_pDC->Rectangle(rectColor);
	rectColor.OffsetRect(0, spaceY);
	m_pDC->SelectObject(&myPenP3);
	if (m_nCountOfSensors >= 3)m_pDC->Rectangle(rectColor);
	rectColor.OffsetRect(0, spaceY);
	m_pDC->SelectObject(&myPenP4);
	if (m_nCountOfSensors >= 4)m_pDC->Rectangle(rectColor);
	rectColor.OffsetRect(0, spaceY);
	m_pDC->SelectObject(&myPenP5);
	if (m_nCountOfSensors >= 5)m_pDC->Rectangle(rectColor);
	rectColor.OffsetRect(0, spaceY);
	m_pDC->SelectObject(&myPenP6);
	if (m_nCountOfSensors >= 6)m_pDC->Rectangle(rectColor);
	rectColor.OffsetRect(0, spaceY);

	m_pDC->SelectObject(pOldPen);
	myPenP1.DeleteObject();
	myPenP2.DeleteObject();
	myPenP3.DeleteObject();
	myPenP4.DeleteObject();
	myPenP5.DeleteObject();
	myPenP6.DeleteObject();
}


void CPlotLiveChart::DrawGrid()
{
	int XOAxis = 0;
	CPen myPenP1, myPenP2, myPenP3, myPenP4;
	myPenP1.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	myPenP2.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	myPenP3.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	myPenP4.CreatePen(PS_SOLID, 1, RGB(255, 0, 255));

	CPen* pOldPen = m_pDC->SelectObject(&myPenP1);
	m_pDC->Rectangle(m_rectPlot);


	m_pDC->SelectObject(&myPenP3);
	double dblXPosYLine2 = m_rectPlot.left;
	double dblSubXGridSpace = m_dblXGridSpace / 5;
	for (int i = 1; dblXPosYLine2 < m_rectPlot.right-1; i++)
	{
		dblXPosYLine2 += dblSubXGridSpace;
		m_pDC->MoveTo(dblXPosYLine2, m_rectPlot.top);
		m_pDC->LineTo(dblXPosYLine2, m_rectPlot.bottom);
	}

	double dblYPosXLine2 = m_rectPlot.top;
	double dblSubYGridSpace = m_dblYGridSpace / 5;
	for (int i = 1; dblYPosXLine2 < m_rectPlot.bottom; i++)
	{
		dblYPosXLine2 += dblSubYGridSpace;
		m_pDC->MoveTo(m_rectPlot.left, dblYPosXLine2);
		m_pDC->LineTo(m_rectPlot.right, dblYPosXLine2);
	}

	m_pDC->SelectObject(&myPenP2);
	double dblXPosYLine = m_rectPlot.left;
	CString strSec;
	int iGraphXRange = m_iGraphXRange;
	int iGraphYStep = iGraphXRange / X_GRID_COUNT;
	int iXValue = iGraphYStep;
	for(int i=1; dblXPosYLine < m_rectPlot.right; i++, iXValue+=iGraphYStep)
	{
		dblXPosYLine += m_dblXGridSpace;
		m_pDC->MoveTo(dblXPosYLine, m_rectPlot.top);
		m_pDC->LineTo(dblXPosYLine, m_rectPlot.bottom);
		strSec.Format(_T("%d"), iXValue);
		m_pDC->TextOut(dblXPosYLine, m_rectPlot.bottom + 5, strSec);
	}

	double dblYPosXLine = m_rectPlot.bottom;
	int iGraphYMax = m_iGraphYMax;
	int iGraphYMin = m_iGraphYMin;
	int iStepYValue = (iGraphYMax - iGraphYMin) / Y_GRID_COUNT;
	int iYValue = iGraphYMin + iStepYValue;
	for (int i = 1; (dblYPosXLine > m_rectPlot.top); i++, iYValue += iStepYValue)
	{
		dblYPosXLine -= m_dblYGridSpace;
		m_pDC->MoveTo(m_rectPlot.left, dblYPosXLine);
		m_pDC->LineTo(m_rectPlot.right, dblYPosXLine);
		strSec.Format(_T("%4d"), iYValue);
		m_pDC->TextOut(m_rectPlot.left-40, dblYPosXLine, strSec);
	}
	m_pDC->TextOutW(m_rectPlot.left - 15, m_rectPlot.top - 15, _T("กษ"));
	m_pDC->TextOutW(m_rectPlot.right + 10, m_rectPlot.bottom - 10, _T("SEC"));

	m_pDC->SelectObject(pOldPen);

	myPenP1.DeleteObject();
	myPenP2.DeleteObject();
	myPenP3.DeleteObject();
	myPenP4.DeleteObject();

}
