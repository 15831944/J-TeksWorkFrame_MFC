#include "stdafx.h"
#include "PlotGage.h"
#include "Status.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define PLOT_SPACE		20
#define HEIGHT_MARGIN	10
#define BOUND_MARGIN	20

CPlotGage::CPlotGage()
	: m_pDC(NULL)
	, m_iDispMode(0)
	, m_HeightMargin(2)
	, m_rectNormal(0)
	, m_xPixelForDegree(0)
	, m_yPixelForDegree(0)
	, m_xPixelForDegreeZoom(0)
	, m_yPixelForDegreeZoom(0)
	, m_bIsOutBound(FALSE)
	, m_bIsOverTolerance(FALSE)
	, m_ptBubbleCenter(0)
	, m_dblCurXPos(0)
	, m_dblCurYPos(0)
	, m_ptZoomBubbleCenter(0)
	, m_bIsOutBoundZoom(FALSE)
	, m_dblDispThreshold(5)
	, m_bDispOverThreshold(FALSE)
	, m_dblZoomAreaDegree(7)
	, m_dblHiZoomAreaDegree(5)
{
}


CPlotGage::~CPlotGage()
{
}


void CPlotGage::SetDC(CDC *pDC)
{
	m_pDC = pDC;
}


void CPlotGage::SetRect(CRect ClientRect)
{
	m_ClientRect = ClientRect;
}


void CPlotGage::SetMode(int iDispMode)
{
	m_iDispMode = iDispMode;
}

int CPlotGage::Calc()
{
#pragma region 전체화면 계산(Dual)	
	int rectHeight = m_ClientRect.Height();
	int rectWidth = m_ClientRect.Width();

	//Single Value Init
	int squreSize = 0;
	m_DrawRect = m_ClientRect;
	m_DrawRect.InflateRect(-BOUND_MARGIN*2, -HEIGHT_MARGIN*2);

	squreSize = (rectHeight > rectWidth) ? rectWidth : rectHeight;
	CPoint ptCenter = m_ClientRect.CenterPoint();
	CPoint ptLeftTop;
	if (m_iDispMode == PLOT_SINGLE)
	{
		ptLeftTop = ptCenter - CPoint(squreSize / 2, squreSize / 2);
		CRect rectNormal(ptLeftTop, SIZE{ squreSize, squreSize });
		m_rectNormal = rectNormal;
	}
	else
	{
		//! Dual Mode
		int DrawRectWidth = m_DrawRect.Width();
		int DualPlotWidth = DrawRectWidth - PLOT_SPACE;
		int PlotWidth = DualPlotWidth / 2;
		if (PlotWidth > m_DrawRect.Height())
		{
			PlotWidth = m_DrawRect.Height();
		}
		SIZE sizePlot{ PlotWidth, PlotWidth };
		
		CPoint ptCenter = m_DrawRect.CenterPoint();
		int iNormalPlotRight = ptCenter.x - PLOT_SPACE / 2;
		int iNormalPlotLeft = iNormalPlotRight - PlotWidth;
		int iZoomPlotLeft = ptCenter.x + PLOT_SPACE / 2;
		int iZoomPlotRight = iZoomPlotLeft + PlotWidth;

		int iPlotTop = ptCenter.y - PlotWidth / 2;
		int iPlotBottom = ptCenter.y + PlotWidth / 2;

		m_rectNormal.SetRect(iNormalPlotLeft, iPlotTop, iNormalPlotRight, iPlotBottom);
		m_rectZoom = m_rectNormal;
		m_rectZoom.OffsetRect(POINT{ PLOT_SPACE + PlotWidth, 0 });
		CRect rectSafeZoneZoom = m_rectZoom;
		rectSafeZoneZoom.InflateRect(-rectSafeZoneZoom.Width() / 4, -rectSafeZoneZoom.Width() / 4);
		m_rectSafeZoneZoom = rectSafeZoneZoom;
	}
#pragma endregion m_rectNormal, m_rectSafeZone,과 m_rectZoom, m_rectSafeZoneZoom를 계산하기 위한 부분

#pragma region m_rectNormal의 내부의 변수값들 초기화, 경계값구하기
	m_bIsOutBound = FALSE;
	m_bIsOutBoundZoom = FALSE;
	m_bDispOverThreshold = FALSE;
	CRect rectZoomLow = m_rectNormal;
	rectZoomLow.DeflateRect(m_rectNormal.Width() / 4, m_rectNormal.Height() / 4);
	m_rectZoomLow = rectZoomLow;
	tALMDRecord * pCurData = CStatus::Instance()->GetCurMeasureData();
	double dblTolerance = CStatus::Instance()->GetTolerance();

	//! Check Tolerance
	m_bIsOverTolerance = FALSE;

	short sRawRoll = pCurData->sRawRoll;
	short sRawPitch = pCurData->sRawPitch;
	if (CStatus::Instance()->IsRelative())
	{
		sRawRoll -= (1000 * CStatus::Instance()->m_dptRelPlan.dblPosX);
		sRawPitch -= (1000 * CStatus::Instance()->m_dptRelPlan.dblPosY);
	}
	m_xPixelForDegreeZoom = (double)(m_rectSafeZoneZoom.Width()) / (dblTolerance * 2);
	m_yPixelForDegreeZoom = (double)(m_rectSafeZoneZoom.Height()) / (dblTolerance * 2);

	double dblDistFromCenter = sqrt(pow(((sRawRoll) / 1000.), 2) + pow((sRawPitch) / 1000., 2));
	if (fabs(dblDistFromCenter) > dblTolerance)m_bIsOverTolerance = TRUE;
	if (fabs(dblDistFromCenter) > m_dblDispThreshold)m_bDispOverThreshold = TRUE;

	if (m_bDispOverThreshold)
	{
		m_xPixelForDegree = (double)(m_rectZoomLow.Width()) / (m_dblZoomAreaDegree * 2);
		m_yPixelForDegree = (double)(m_rectZoomLow.Height()) / (m_dblZoomAreaDegree * 2);
	}
	else
	{
		m_xPixelForDegree = (double)((0.5*m_rectNormal.Width())) / (m_dblHiZoomAreaDegree * 2);
		m_yPixelForDegree = (double)((0.5*m_rectNormal.Height())) / (m_dblHiZoomAreaDegree * 2);
	}


	int iWidth = (int)(m_rectNormal.Width()*cos(M_PI / 4.0));
	m_rectZoomHighLimit.SetRect(0, 0, iWidth, iWidth);
	m_rectZoomHighLimit.MoveToXY(m_rectNormal.CenterPoint());
	m_rectZoomHighLimit.OffsetRect(-iWidth / 2, -iWidth / 2);

	if (m_bDispOverThreshold)
	{
		int ToleranceWidth = (int)((m_xPixelForDegree * CStatus::Instance()->GetTolerance()) * 2);
		int ToleranceHeight = (int)((m_yPixelForDegree * CStatus::Instance()->GetTolerance()) * 2);
		m_rectSafeZone.SetRect(0, 0, ToleranceWidth, ToleranceHeight);
		m_rectSafeZone.MoveToXY(m_rectNormal.CenterPoint());
		m_rectSafeZone.OffsetRect(CPoint(-ToleranceWidth / 2, -ToleranceHeight / 2));
		//SafeZone의 2배로 ZoomArea설정
		CRect rectZoomArea = m_rectSafeZone;
		rectZoomArea.InflateRect(rectZoomArea.Width() / 2, rectZoomArea.Width() / 2);
		m_rectZoomArea = rectZoomArea;
	}
	else
	{
		int ToleranceWidth = (int)((m_xPixelForDegree * CStatus::Instance()->GetTolerance()) * 2);
		int ToleranceHeight = (int)((m_yPixelForDegree * CStatus::Instance()->GetTolerance()) * 2);
		m_rectSafeZone.SetRect(0, 0, ToleranceWidth, ToleranceHeight);
		m_rectSafeZone.MoveToXY(m_rectNormal.CenterPoint());
		m_rectSafeZone.OffsetRect(CPoint(-ToleranceWidth / 2, -ToleranceHeight / 2));
		//SafeZone의 2배로 ZoomArea설정
		CRect rectZoomArea = m_rectSafeZone;
		rectZoomArea.InflateRect(rectZoomArea.Width() / 2, rectZoomArea.Width() / 2);
		m_rectZoomArea = rectZoomArea;
	}
#pragma endregion 각종 지역변수들 초기화 m_rectZoomHighLimit, m_rectZoomLow, m_rectSafeZone, m_rectZoomArea

#pragma region 버블 위치구하기
	m_dblCurXPos = m_xPixelForDegree * ((sRawRoll) / 1000.);
	m_dblCurYPos = m_yPixelForDegree * ((sRawPitch) / 1000.);
	m_ptBubbleCenter.SetPoint(m_rectNormal.CenterPoint().x, m_rectNormal.CenterPoint().y);
	m_ptBubbleCenter.x += (LONG)m_dblCurXPos;
	m_ptBubbleCenter.y -= (LONG)m_dblCurYPos;
	if (m_ptBubbleCenter.x > m_rectNormal.right) { m_ptBubbleCenter.x = m_rectNormal.right; m_bIsOutBound = TRUE; }
	if (m_ptBubbleCenter.x < m_rectNormal.left) { m_ptBubbleCenter.x = m_rectNormal.left;  m_bIsOutBound = TRUE; }
	if (m_ptBubbleCenter.y > m_rectNormal.bottom) { m_ptBubbleCenter.y = m_rectNormal.bottom;  m_bIsOutBound = TRUE; }
	if (m_ptBubbleCenter.y < m_rectNormal.top) { m_ptBubbleCenter.y = m_rectNormal.top;  m_bIsOutBound = TRUE; }

	m_NormalPlotBubble.SetRect(0, 0, 30, 30);
	m_NormalPlotBubble.MoveToXY(m_ptBubbleCenter);
	m_NormalPlotBubble.OffsetRect(-15, -15);

	if (m_iDispMode == PLOT_DUAL)
	{
		double dblCurXPos = m_xPixelForDegreeZoom * ((sRawRoll) / 1000.);
		double dblCurYPos = m_yPixelForDegreeZoom * ((sRawPitch) / 1000.);
		CPoint ptZoomBubbleCenter;
		ptZoomBubbleCenter = m_rectZoom.CenterPoint();
		ptZoomBubbleCenter.x += (LONG)dblCurXPos;
		ptZoomBubbleCenter.y -= (LONG)dblCurYPos;

		if (ptZoomBubbleCenter.x > m_rectZoom.right) { ptZoomBubbleCenter.x = m_rectZoom.right; m_bIsOutBoundZoom = TRUE; }
		if (ptZoomBubbleCenter.x < m_rectZoom.left) { ptZoomBubbleCenter.x = m_rectZoom.left; m_bIsOutBoundZoom = TRUE; }
		if (ptZoomBubbleCenter.y > m_rectZoom.bottom) { ptZoomBubbleCenter.y = m_rectZoom.bottom; m_bIsOutBoundZoom = TRUE; }
		if (ptZoomBubbleCenter.y < m_rectZoom.top) { ptZoomBubbleCenter.y = m_rectZoom.top; m_bIsOutBoundZoom = TRUE; }

		m_ptZoomBubbleCenter = ptZoomBubbleCenter;

		CRect ZoomPlotBubble;
		ZoomPlotBubble.SetRect(0, 0, 60, 60);
		ZoomPlotBubble.MoveToXY(m_ptZoomBubbleCenter);
		ZoomPlotBubble.OffsetRect(-30, -30);
		m_ZoomPlotBubble = ZoomPlotBubble;
	}

/*! 내부사각형이 버블을 따라 커짐
	m_rectZoomArea = rectZoomArea;  //! ZoomLow내에 버블이 있으면 더이상 ZoomArea를 줄이지 않고 LowLimit으로 그리도록함
	if (rectZoomArea.PtInRect(m_NormalPlotBubble.CenterPoint()))
	{
		m_rectZoomArea = rectZoomArea;  //! ZoomLow내에 버블이 있으면 더이상 ZoomArea를 줄이지 않고 LowLimit으로 그리도록함
	}else
	{
		if (m_rectZoomHighLimit.PtInRect(m_NormalPlotBubble.CenterPoint()))
		{
			CPoint ptCenterToBubble = m_ptBubbleCenter;
			ptCenterToBubble.Offset(-m_rectNormal.CenterPoint());
			int iZoomAreaSize = 2*max(abs(ptCenterToBubble.x), abs(ptCenterToBubble.y) );
			m_rectZoomArea.SetRect(0, 0, iZoomAreaSize, iZoomAreaSize);
			m_rectZoomArea.MoveToXY(m_rectNormal.CenterPoint());
			m_rectZoomArea.OffsetRect(-m_rectZoomArea.Width() / 2, -m_rectZoomArea.Width() / 2);
		}
		else m_rectZoomArea = m_rectZoomHighLimit;  //! 최대크기를 벗어나면 더이사 Tracking하지 않고 최대크기인 HightLimit으로 그리도록함
	}
*/
#pragma endregion 버블의 위치 계산

#pragma region Relative 사각형
	m_rectRelOrgZoomArea =m_rectZoomArea;
	CPoint ptRelOffset;
	double OffsetX = CStatus::Instance()->m_dptRelPlan.dblPosX;
	double OffsetY = CStatus::Instance()->m_dptRelPlan.dblPosY;
	ptRelOffset.x = m_xPixelForDegree * OffsetX;
	ptRelOffset.y = m_yPixelForDegree * OffsetY;
	m_rectRelOrgZoomArea.OffsetRect(POINT{ -ptRelOffset.x, ptRelOffset.y });
#pragma endregion Relative모드에서 기존 사각형을 그려줌

	return 0;
}


void CPlotGage::DrawBG()
{
	CPen myPen;
	myPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = m_pDC->SelectObject(&myPen);
	m_pDC->Rectangle(&m_ClientRect);
	m_pDC->SelectObject(pOldPen);
	myPen.DeleteObject();

	myPen.CreatePen(PS_DASH, 1, RGB(210, 210, 210));
	pOldPen = m_pDC->SelectObject(&myPen);
	CBrush myBrush;
	myBrush.CreateStockObject(NULL_BRUSH);
	CBrush *pOldBrush = m_pDC->SelectObject(&myBrush);

	m_pDC->Rectangle(&m_rectNormal);

	if (m_bDispOverThreshold)
	{
		m_pDC->SelectObject(pOldPen);
		myPen.DeleteObject();
		myPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		CPen* pOldPen = m_pDC->SelectObject(&myPen);
		DrawNotch(m_pDC);
		m_pDC->Ellipse(&m_rectNormal);
	}
	m_pDC->SelectObject(pOldPen);
	myPen.DeleteObject();

	if (m_iDispMode == PLOT_DUAL)
	{
		//Dule
		m_pDC->Rectangle(&m_rectZoom);

	}

	//원의 십자라인
	CPoint XLineStart(m_rectNormal.left, m_rectNormal.CenterPoint().y);
	CPoint XLineEnd(m_rectNormal.right, m_rectNormal.CenterPoint().y);
	CPoint YLineStart(m_rectNormal.CenterPoint().x, m_rectNormal.top);
	CPoint YLineEnd(m_rectNormal.CenterPoint().x, m_rectNormal.bottom);

	m_pDC->MoveTo(XLineStart);
	m_pDC->LineTo(XLineEnd);
	m_pDC->MoveTo(YLineStart);
	m_pDC->LineTo(YLineEnd);

	if (m_iDispMode == PLOT_DUAL)
	{
		//줌윈도의 십자라인
		XLineStart.SetPoint(m_rectZoom.left, m_rectZoom.CenterPoint().y);
		CPoint XLineEnd(m_rectZoom.right, m_rectZoom.CenterPoint().y);
		CPoint YLineStart(m_rectZoom.CenterPoint().x, m_rectZoom.top);
		CPoint YLineEnd(m_rectZoom.CenterPoint().x, m_rectZoom.bottom);
		m_pDC->MoveTo(XLineStart);
		m_pDC->LineTo(XLineEnd);
		m_pDC->MoveTo(YLineStart);
		m_pDC->LineTo(YLineEnd);	
	}

	m_pDC->SetBkMode(TRANSPARENT);
	CString strXUnitValue;
	m_pDC->SetTextAlign(TA_CENTER);
	if (CStatus::Instance()->GetCoordinateSystem() == CStatus::CS_CARTESIAN)
	{
		m_pDC->TextOutW(m_rectNormal.left-8, m_rectNormal.CenterPoint().y, _T("X-"));
		m_pDC->TextOutW(m_rectNormal.right+9, m_rectNormal.CenterPoint().y, _T("X+"));
		m_pDC->TextOutW(m_rectNormal.CenterPoint().x, m_rectNormal.top-15, _T("Y+"));
		m_pDC->TextOutW(m_rectNormal.CenterPoint().x, m_rectNormal.bottom, _T("Y-"));
	}
	else
	{
		m_pDC->TextOutW(m_rectNormal.left-10, m_rectNormal.CenterPoint().y, _T("180°"));
		m_pDC->TextOutW(m_rectNormal.right+15, m_rectNormal.CenterPoint().y, _T("0°"));
		m_pDC->TextOutW(m_rectNormal.CenterPoint().x+9, m_rectNormal.top-15, _T("90°"));
		m_pDC->TextOutW(m_rectNormal.CenterPoint().x+9, m_rectNormal.bottom, _T("270°"));
	}

	//NormalPlot의 Zoom Area 
	m_pDC->SelectObject(pOldPen);
	myPen.DeleteObject();
	myPen.CreatePen(PS_DOT, 1, RGB(210, 210, 210));
	pOldPen = m_pDC->SelectObject(&myPen);

	m_pDC->Rectangle(m_rectZoomArea);

	//! m_rectZoomArea에서 m_rectZoom으로의 가이드라인
	m_pDC->MoveTo(m_rectZoomArea.TopLeft());
	m_pDC->LineTo(m_rectZoom.TopLeft());
	m_pDC->MoveTo(POINT{ m_rectZoomArea.left, m_rectZoomArea.bottom});
	m_pDC->LineTo(POINT{ m_rectZoom.left, m_rectZoom.bottom });

	m_pDC->SelectObject(pOldPen);
	m_pDC->SelectObject(pOldBrush);
	myPen.DeleteObject();
}

void CPlotGage::Draw()
{
	CPen myPen;
	myPen.CreatePen(PS_DASHDOTDOT, 1, RGB(255, 230, 222));
	CPen* pOldPen = m_pDC->SelectObject(&myPen);
	CBrush myBrush;
	myBrush.CreateStockObject(NULL_BRUSH);
	CBrush *pOldBrush = m_pDC->SelectObject(&myBrush);

	if (CStatus::Instance()->IsRelative())
	{
		m_pDC->Rectangle(m_rectRelOrgZoomArea);
	}

	m_pDC->SelectObject(pOldPen);
	myPen.DeleteObject();
	myPen.CreatePen(PS_SOLID, 1, RGB(0, 100, 195));
	pOldPen = m_pDC->SelectObject(&myPen);
	m_pDC->Ellipse(&m_rectSafeZone);
	m_pDC->SelectObject(pOldPen);
	myPen.DeleteObject();

	myPen.CreatePen(PS_SOLID, 2, RGB(0, 100, 195));
	pOldPen = m_pDC->SelectObject(&myPen);
	m_pDC->Ellipse(&m_rectSafeZoneZoom);
	if (m_iDispMode == PLOT_DUAL)
	{
		//Dule
		if (m_bDispOverThreshold)
		{
			m_pDC->SelectObject(pOldPen);
			myPen.DeleteObject();
			myPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			CPen* pOldPen = m_pDC->SelectObject(&myPen);
			DrawNotch(m_pDC);
			m_pDC->Ellipse(&m_rectNormal);
			m_pDC->SelectObject(pOldPen);
			myPen.DeleteObject();
		}
	}
	else
	{
		//Single 
		m_pDC->Ellipse(&m_DrawRect);
	}
	m_pDC->SelectObject(pOldPen);
	m_pDC->SelectObject(pOldBrush);
	myPen.DeleteObject();
}

void CPlotGage::DrawBubble()
{
	CBrush myBrush;
	CBrush *pOldBrush = NULL;

//	if (m_bIsOutBound)
	if (false)
		{
		myBrush.CreateSolidBrush(RGB(228, 127, 39));
		pOldBrush = m_pDC->SelectObject(&myBrush);

	}else
		if (m_bIsOverTolerance)
		{
			myBrush.CreateSolidBrush(RGB(228, 0, 0));
			pOldBrush = m_pDC->SelectObject(&myBrush);
		}
		else
		{
			myBrush.CreateSolidBrush(RGB(0, 228, 0));
			pOldBrush = m_pDC->SelectObject(&myBrush);
		}


	m_pDC->Ellipse(m_NormalPlotBubble);
	m_pDC->SelectObject(pOldBrush);
	myBrush.DeleteObject();


//	if (m_bIsOutBoundZoom)
	if (false)
		{
		myBrush.CreateSolidBrush(RGB(228, 127, 39));
		pOldBrush = m_pDC->SelectObject(&myBrush);

	}
	else
		if (m_bIsOverTolerance)
		{
			myBrush.CreateSolidBrush(RGB(228, 0, 0));
			pOldBrush = m_pDC->SelectObject(&myBrush);
		}
		else
		{
			myBrush.CreateSolidBrush(RGB(0, 228, 0));
			pOldBrush = m_pDC->SelectObject(&myBrush);
		}

	if (m_iDispMode == PLOT_DUAL)
	{
		m_pDC->Ellipse(m_ZoomPlotBubble);
	}
	m_pDC->SelectObject(pOldBrush);
	myBrush.DeleteObject();
}


void CPlotGage::DrawNotch(CDC * pDC)
{
	CPoint triAngle[3];
	CPoint ptApex(m_rectNormal.CenterPoint());
	ptApex.y = m_rectNormal.bottom;
	triAngle[0].SetPoint(ptApex.x, ptApex.y - 6);
	triAngle[1].SetPoint(ptApex.x - 4, ptApex.y - 1);
	triAngle[2].SetPoint(ptApex.x + 4, ptApex.y - 1);

	pDC->Polygon(triAngle, 3);
}
