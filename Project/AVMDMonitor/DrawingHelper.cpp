#include "stdafx.h"
#include "DrawingHelper.h"
#include "Status.h"
#include "Repository.h"
#include "stdio.h"
#include "AVMDMonitorDoc.h"
#include "AVMDMonitorView.h"
#define _USE_MATH_DEFINES
#include <math.h>


inline double GetFrequencyIntensity(double re, double im)
{
	return sqrt((re*re) + (im*im));
}
#define Amplitude(re,im,len) (GetFrequencyIntensity(re,im)/(len))
#define AmplitudeScaled(re,im,len,scale) ((int)Amplitude(re,im,len)%scale)

CDrawingHelper::CDrawingHelper()
	:m_GraphType(_T(""))
	, m_bIsFFT(FALSE)
	, m_prevPt(0,0)
	, iTempRePosX(0)
	, m_nStepMove(0)
	, m_bCalcValueReEnter(FALSE)
	//, m_pGraphics(NULL)
	, m_iOverlapToTalCount(0)
	, m_dblOverlapToTalSum(0)
	, m_iDragAmount(0)
	, m_bIsFirst(FALSE)
	, m_iCheckTriggerStartNum(0)
	, m_iCheckTriggerEndNum(0)
	, m_iCheckGoNoGoStartNum(0)
	, m_iCheckGoNoGoEndNum(0)
	, m_iTempTriggerEnd(0)
	, m_iTempGoNoGoEnd(0)
	, m_iGoNoGoCount(0)
	, m_iNoGoIndex(0)
	, m_bIsInNoGoSecond(FALSE)
	, m_strGoNoGoGraphType(_T(""))
	, m_iHoldStart(0)
	, m_iHoldEnd(0)
	, m_iHoldFFTStartNum(0)
	, m_iFFTReviewStartNum(0)
	, m_dblPrevTimeUnit(0)
	, m_iFFTDrawAmount(0)
	, m_bChange(FALSE)
	, m_bPrevTriggerSpecOut(FALSE)
{
	m_pFFTData[0] = NULL;
	m_pFFTData[1] = NULL;
	m_pIFFTData[0] = NULL;
	m_pIFFTData[1] = NULL;
	m_pFFTGraphData[0] = NULL;
	m_pFFTGraphData[1] = NULL;
	m_pMonitorView = NULL;
	//
	m_pdblFilteredData[0] = NULL;
	m_pdblFilteredData[1] = NULL;
	m_pdblFilterCompareData[0] = NULL;
	m_pdblFilterCompareData[1] = NULL;
	m_pdblFilterCompareData[2] = NULL;
	m_pdblFilterCompareData[3] = NULL;
}

CDrawingHelper::~CDrawingHelper()
{
}

void CDrawingHelper::SetDC(CDC *pDC)//, int nInputType)
{
	// CDC를 HDC로 변환
	m_pDC = pDC;
	m_iInterval = 8;
}

void CDrawingHelper::DrawingString(CString strGraphType, int nInputType)
{
	CFont font;
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = FONT_SIZE;
	lf.lfWeight = FW_BOLD;
	wsprintf(lf.lfFaceName, TEXT("%s"), TEXT("Arial"));
	font.CreateFontIndirectW(&lf);
	m_pDC->SelectObject(&font);
	m_pDC->SetBkMode(TRANSPARENT);
	m_pDC->SetTextAlign(TA_LEFT);

	int iScale = _ttoi(CStatus::Instance()->m_strGraphScale);
	int iStandardValue = 0;
	BOOL bIsOverlay = CStatus::Instance()->GetOverlayCondition();
	if (nInputType == 1)
	{
		if (m_GraphType == "RMS")
			iStandardValue = CStatus::Instance()->m_iCompareStandardValue;
		else
			iStandardValue = CStatus::Instance()->m_iCompareStandardValue;
	}
	else
	{
		if (strGraphType == "RMS")
			iStandardValue = CStatus::Instance()->m_iRMSStandardValue;
		else if (strGraphType == 'X')
			iStandardValue = CStatus::Instance()->m_iXStandardValue;
		else if (strGraphType == 'Y')
			iStandardValue = CStatus::Instance()->m_iYStandardValue;
		else if (strGraphType == 'Z')
			iStandardValue = CStatus::Instance()->m_iZStandardValue;
	}
	CString strTimeDomain = CStatus::Instance()->m_strTimeDomainUnit;
	double dblMaxValue, dblMinValue;
	double dblGraphScale = _wtof(CStatus::Instance()->m_strGraphScale);
	if (strTimeDomain == "g-Force")
	{
		if (m_GraphType == "RMS")
		{
			if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE1_MODE)
			{
				double dblGraphViewMax = 2. / (double)iScale;
				double dblMoveAmount = 2. * (double)iStandardValue / SCROLL_RATIO;
				dblMaxValue = (dblGraphViewMax / 2. *3.) + dblMoveAmount;
				dblMinValue = (dblGraphViewMax / 2.) + dblMoveAmount;
			}
			if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE2_MODE)
			{
				double dblGraphViewMax = 3. / (double)iScale;
				double dblMoveAmount = 3. * (double)iStandardValue / SCROLL_RATIO;
				dblMaxValue = (dblGraphViewMax / 2. *3.) + dblMoveAmount;
				dblMinValue = (dblGraphViewMax / 2.) + dblMoveAmount;
			}
		}
		else
		{
			if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE1_MODE)
			{
				double dblGraphViewMax = 2. / (double)iScale;
				double dblMoveAmount = 2. * (double)iStandardValue / SCROLL_RATIO;
				dblMaxValue = (dblGraphViewMax / 2.) + dblMoveAmount;
				dblMinValue = -(dblGraphViewMax / 2.) + dblMoveAmount;
			}
			if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE2_MODE)
			{
				double dblGraphViewMax = 3. / (double)iScale;
				double dblMoveAmount = 3. * (double)iStandardValue / SCROLL_RATIO;
				dblMaxValue = (dblGraphViewMax / 2.) + dblMoveAmount;
				dblMinValue = -(dblGraphViewMax / 2.) + dblMoveAmount;
			}
		}
	}
	if (strTimeDomain == "GAL")
	{
		if (m_GraphType == "RMS")
		{
			if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE1_MODE)
			{
				double dblGraphViewMax = 1962. / (double)iScale;
				double dblMoveAmount = 1962. * (double)iStandardValue / SCROLL_RATIO;
				dblMaxValue = (dblGraphViewMax / 2. *3.) + dblMoveAmount;
				dblMinValue = (dblGraphViewMax / 2.) + dblMoveAmount;
			}
			if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE2_MODE)
			{
				double dblGraphViewMax = 2943. / (double)iScale;
				double dblMoveAmount = 2943. * (double)iStandardValue / SCROLL_RATIO;
				dblMaxValue = (dblGraphViewMax / 2. *3.) + dblMoveAmount;
				dblMinValue = (dblGraphViewMax / 2.) + dblMoveAmount;
			}
		}
		else
		{
			if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE1_MODE)
			{
				double dblGraphViewMax = 1962. / (double)iScale;
				double dblMoveAmount = 1962. * (double)iStandardValue / SCROLL_RATIO;
				dblMaxValue = (dblGraphViewMax / 2.) + dblMoveAmount;
				dblMinValue = -(dblGraphViewMax / 2.) + dblMoveAmount;
			}
			else
			{
				double dblGraphViewMax = 2943. / (double)iScale;
				double dblMoveAmount = 2943. * (double)iStandardValue / SCROLL_RATIO;
				dblMaxValue = (dblGraphViewMax / 2.) + dblMoveAmount;
				dblMinValue = -(dblGraphViewMax / 2.) + dblMoveAmount;
			}
		}
	}
	CString strMaxValue;
	CString strStanardValue;
	CString strMinValue;
	strMaxValue.Format(_T("  %.3f"), dblMaxValue);
	strMinValue.Format(_T("  %.3f"), dblMinValue);
	
	double dblOneRectHeight = m_graphRect.Height() / CStatus::Instance()->m_ihorizontalNum;
	m_pDC->TextOutW((int)m_graphRect.right, (int)(m_graphRect.top + dblOneRectHeight), strMaxValue);
	m_pDC->TextOutW((int)m_graphRect.right, (int)(m_graphRect.bottom - dblOneRectHeight), strMinValue);

	m_pDC->TextOutW(m_graphRect.right + 10, (m_graphRect.bottom / 2) + 10 , strGraphType);
	font.DeleteObject();
}

void CDrawingHelper::InitBackGround(CRect dlgRect, CString strGraphType, int nInputType)
{
	if(nInputType == 0) // Org Graph
		m_GraphType = strGraphType;
	if (nInputType == 1) // Compare Graph
		m_GraphType = CStatus::Instance()->GetCompareType();

	int iVerticalNum = CStatus::Instance()->m_iverticalNum;
	int iHorizonNum = CStatus::Instance()->m_ihorizontalNum;

	if(CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		SetGraphRectangleArea(dlgRect);
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
		SetReviewGraphRectangleArea(dlgRect);

	CPen penBackGround(PS_SOLID, 0, ::GetSysColor(COLOR_BTNSHADOW));
	CPen* oldPen = m_pDC->SelectObject(&penBackGround);

	m_pDC->Rectangle(m_alarmRect.left, m_alarmRect.top, m_alarmRect.right, m_alarmRect.bottom);
	m_pDC->Rectangle(m_graphRect.left, m_graphRect.top, m_graphRect.right, m_graphRect.bottom);

	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
	{
#pragma region 가로 줄 긋기
		nHorizontalInterval = (double)m_graphRect.Height() / (double)iHorizonNum;
		for (int i = 0; i <= iHorizonNum; i++)
		{
			if (i == 0 || i == iHorizonNum)
				continue;

			Point LineStart;
			Point LineEnd;

			LineStart.X =(int)m_graphRect.left;
			LineStart.Y = (int)(m_graphRect.top + (nHorizontalInterval * i));
			LineEnd.X = (int)m_graphRect.right;
			LineEnd.Y = (int)(m_graphRect.top + (nHorizontalInterval * i));

			m_pDC->MoveTo(LineStart.X, LineStart.Y);
			m_pDC->LineTo(LineEnd.X, LineEnd.Y);
		}
#pragma endregion

#pragma region 세로 줄 긋기
		for (int i = 0; i <= iVerticalNum; i++)
		{
			Point LineStart;
			Point LineEnd;

			LineStart.X = (int)(m_graphRect.right - (ONERECT_PIXEL*i));
			LineStart.Y = (int)m_graphRect.top;
			LineEnd.X = (int)(m_graphRect.right - (ONERECT_PIXEL*i));
			LineEnd.Y = (int)m_graphRect.bottom;

			if (LineStart.X < m_graphRect.left || LineStart.X > m_graphRect.right)
				continue;	
			
			m_pDC->MoveTo(LineStart.X, LineStart.Y);
			m_pDC->LineTo(LineEnd.X, LineEnd.Y);
		}
#pragma endregion
	}

	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
#pragma region 가로 줄 긋기
		nHorizontalInterval = (double)m_graphRect.Height() / (double)iHorizonNum;
		for (int i = 0; i <= iHorizonNum; i++)
		{
			if (i == 0 || i == iHorizonNum)
				continue;

			Point LineStart;
			Point LineEnd;

			LineStart.X = (int)(m_graphRect.left);
			LineStart.Y = (int)(m_graphRect.top + (nHorizontalInterval * i));
			LineEnd.X = (int)m_graphRect.right;
			LineEnd.Y = (int)(m_graphRect.top + (nHorizontalInterval * i));

			m_pDC->MoveTo(LineStart.X, LineStart.Y);
			m_pDC->LineTo(LineEnd.X, LineEnd.Y);
		}
#pragma endregion
		int nCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
		if (nCount == 0)
		{
#pragma region 세로 줄 긋기
			for (int i = 0; i <= iVerticalNum; i++)
			{
				Point LineStart;
				Point LineEnd;

				LineStart.X = (m_graphRect.left + (ONERECT_PIXEL*i));
				LineStart.Y = m_graphRect.top;
				LineEnd.X = (m_graphRect.left + (ONERECT_PIXEL*i));
				LineEnd.Y = m_graphRect.bottom;

				if (LineStart.X < m_graphRect.left || LineStart.X > m_graphRect.right)
					continue;
				
				m_pDC->MoveTo(LineStart.X, LineStart.Y);
				m_pDC->LineTo(LineEnd.X, LineEnd.Y);
			}
#pragma endregion
		}
	}
	m_pDC->SelectObject(oldPen);
	penBackGround.DeleteObject();
}

void CDrawingHelper::InitFFTBackGround(CRect dlgRect, CString strGraphType,int nInputType)
{
	int iFftVerticalNum = CStatus::Instance()->m_iFfftVerticalNum;
	int iHorizonNum = CStatus::Instance()->m_ihorizontalNum;
	
	if (nInputType == 0) // Org Graph
		m_GraphType = strGraphType;
	if (nInputType == 1) // Compare Graph
		m_GraphType = CStatus::Instance()->GetCompareType();
	SetFFTGraphRectangleArea(dlgRect);

	CPen penBackGround(PS_SOLID, 0, ::GetSysColor(COLOR_BTNSHADOW));
	CPen* oldPen = m_pDC->SelectObject(&penBackGround);

	m_pDC->Rectangle(m_graphRect.left, m_graphRect.top, m_graphRect.right, m_graphRect.bottom);
	CStatus::Instance()->m_iFFTwidth = m_graphRect.Width();
	
#pragma region 가로 줄 긋기
	nHorizontalInterval = (double)m_graphRect.Height() / (double)iHorizonNum;
	for (int i = 0; i <= iHorizonNum; i++)
	{
		if (i == 0 || i == iHorizonNum)
			continue;

		Point LineStart;
		Point LineEnd;

		LineStart.X = (int)m_graphRect.left;
		LineStart.Y = (int)(m_graphRect.top + (nHorizontalInterval * i));
		LineEnd.X = (int)m_graphRect.right;
		LineEnd.Y = (int)(m_graphRect.top + (nHorizontalInterval * i));

		m_pDC->MoveTo(LineStart.X, LineStart.Y);
		m_pDC->LineTo(LineEnd.X, LineEnd.Y);
	}
#pragma endregion

#pragma region 세로 줄 긋기
	nVerticalInterval = (double)m_graphRect.Width() / ((double)iFftVerticalNum);
	int iFFTShowRatio = CStatus::Instance()->m_iFFTShowRatio;
	int iFFTShowStartPos = CStatus::Instance()->m_iFFTShowStartPos;
	int iHzPixel = FFTONERECT_PIXEL;
	int iFFTSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	int nIndex = (int)((double)iFFTShowStartPos *(double)iFFTSampleSize / (double)FFT_SAMPLING_FREQUENCY);
	int nIntervalIndex = (int)((double)FFT_ONERECT_HZ  *(double)iFFTSampleSize / (double)FFT_SAMPLING_FREQUENCY);
	double dblMaxHz = (int)CStatus::Instance()->m_DeviceFrequency / 2;
	int nMaxIndex = (int)(dblMaxHz *(double)iFFTSampleSize / (double)FFT_SAMPLING_FREQUENCY);
	int iGraphWidthOneInterval = 0;
	if (iFFTSampleSize == 32)	iGraphWidthOneInterval = 128 * iFFTShowRatio;
	if (iFFTSampleSize == 64)	iGraphWidthOneInterval = 64 * iFFTShowRatio;
	if (iFFTSampleSize == 128)	iGraphWidthOneInterval = 32 * iFFTShowRatio;
	if (iFFTSampleSize == 256)	iGraphWidthOneInterval = 16 * iFFTShowRatio;
	if (iFFTSampleSize == 512)	iGraphWidthOneInterval = 8 * iFFTShowRatio;
	if (iFFTSampleSize == 1024)	iGraphWidthOneInterval = 4 * iFFTShowRatio;
	if (iFFTSampleSize == 2048)	iGraphWidthOneInterval = 2 * iFFTShowRatio;
	if (iFFTSampleSize == 4096)	iGraphWidthOneInterval = 1 * iFFTShowRatio;
	for (int i = nIndex; i <= nIndex +m_graphRect.Width(); i++)
	{
		if (nIndex > nMaxIndex)
			return;
		if (i % nIntervalIndex == 0 && i != 0 )
		{
			Point LineStart, LineEnd;

			LineStart.X = (int)(m_graphRect.left) + (iGraphWidthOneInterval*(i - nIndex));
			LineStart.Y = (int)m_graphRect.top;

			LineEnd.X = (int)(m_graphRect.left) + (iGraphWidthOneInterval*(i - nIndex));
			LineEnd.Y = (int)m_graphRect.bottom;
			if (nIndex == (iFFTSampleSize/2) - 1)
			{
				m_pDC->MoveTo(LineStart.X, LineStart.Y);
				m_pDC->LineTo(LineEnd.X, LineEnd.Y);
			}
			else
			{
				if (LineStart.X < m_graphRect.left || LineStart.X > m_graphRect.right || i> iFFTSampleSize / 2)
					continue;
				m_pDC->MoveTo(LineStart.X, LineStart.Y);
				m_pDC->LineTo(LineEnd.X, LineEnd.Y);
			}
		}
	}
	m_pDC->SelectObject(oldPen);
	penBackGround.DeleteObject();
#pragma endregion
}

void CDrawingHelper::UnitofMeasure(BOOL bIsFFT,  CRect dlgRect, int nInputType)
{
	CFont font;
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = FONT_SIZE;
	lf.lfWeight = FW_BOLD;
	wsprintf(lf.lfFaceName, TEXT("%s"), TEXT("Arial"));
	font.CreateFontIndirectW(&lf);
	
	m_pDC->SelectObject(&font);
	m_pDC->SetBkMode(TRANSPARENT);

	
	double dblDegree = 0, dblDegreeInterval = 0, dblReviewDegree =0;
	int iFftDegree = 0, iFftdegreeInterval = 0;
	CString strTemp, strDegree, strUnit;
	int iMaxHz = (int)CStatus::Instance()->m_DeviceFrequency / 2;
	if (bIsFFT)
	{//m_textPt는 왼쪽부터 출력됨
		m_pDC->SetTextAlign(TA_LEFT);

		int iFftVerticalNum = CStatus::Instance()->m_iFfftVerticalNum;

		strDegree.Format(_T("%s"), CStatus::Instance()->m_strFFTDegree);
		SetFFTGraphRectangleArea(dlgRect);

		iFftdegreeInterval = CStatus::Instance()->m_iFFTShowRatio * FFT_ONERECT_HZ;
		
		nFftVerticalInterval = FFTONERECT_PIXEL* FFT_ONERECT_HZ * CStatus::Instance()->m_iFFTShowRatio;
		int iFFTStartPos = CStatus::Instance()->m_iFFTShowStartPos;
		int iFFTSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
		int iFFTShowRatio = CStatus::Instance()->m_iFFTShowRatio;
		int nIndex = (int)((double)iFFTStartPos *(double)iFFTSampleSize / (double)FFT_SAMPLING_FREQUENCY);
		int nIntervalIndex = (int)((double)FFT_ONERECT_HZ  *(double)iFFTSampleSize / (double)FFT_SAMPLING_FREQUENCY);

		int iGraphWidthOneInterval = 0;
		if (iFFTSampleSize == 32)	iGraphWidthOneInterval = 128 * iFFTShowRatio;
		if (iFFTSampleSize == 64)	iGraphWidthOneInterval = 64 * iFFTShowRatio;
		if (iFFTSampleSize == 128)	iGraphWidthOneInterval = 32 * iFFTShowRatio;
		if (iFFTSampleSize == 256)	iGraphWidthOneInterval = 16 * iFFTShowRatio;
		if (iFFTSampleSize == 512)	iGraphWidthOneInterval = 8 * iFFTShowRatio;
		if (iFFTSampleSize == 1024)	iGraphWidthOneInterval = 4 * iFFTShowRatio;
		if (iFFTSampleSize == 2048)	iGraphWidthOneInterval = 2 * iFFTShowRatio;
		if (iFFTSampleSize == 4096)	iGraphWidthOneInterval = 1 * iFFTShowRatio;

		for (int i = nIndex; i <= nIndex + m_graphRect.Width(); i++)
		{
			m_textPt.Y = (REAL)(m_timeRect.bottom - ((double)m_timeRect.Height() *0.7));
			if (i == 0 )
			{
				m_pDC->SetTextAlign(TA_LEFT);
				double dblHz = (double)FFT_SAMPLING_FREQUENCY / (double)iFFTSampleSize * (double)i;
				strTemp.Format(_T("%.2f %s"), dblHz, strDegree);
				m_pDC->TextOutW((int)m_textPt.X, (int)m_textPt.Y, strTemp);
			}
			else
			{
				if (i % nIntervalIndex == 0 && i != 0)
				{
					m_pDC->SetTextAlign(TA_CENTER);
					m_textPt.X = (REAL)(m_graphRect.left) + iGraphWidthOneInterval*(i - nIndex);
					m_textPt.X = (REAL)(m_graphRect.left) + iGraphWidthOneInterval*(i - nIndex);
					if(m_textPt.X + 10 <= m_graphRect.left)
						m_pDC->SetTextAlign(TA_LEFT);
					else
						m_pDC->SetTextAlign(TA_CENTER);

					if (nIndex == (iFFTSampleSize / 2) - 1)
					{
						double dblHz = (double)FFT_SAMPLING_FREQUENCY / (double)iFFTSampleSize * (double)i;
						if (dblHz > iMaxHz)
							continue;
						strTemp.Format(_T("%.2f %s"), dblHz, strDegree);
						m_pDC->TextOutW((int)m_textPt.X, (int)m_textPt.Y, strTemp);
					}
					else
					{
						double dblHz = (double)FFT_SAMPLING_FREQUENCY / (double)iFFTSampleSize * (double)i;
						if (dblHz > iMaxHz)
							continue;
						if (m_textPt.X > m_graphRect.right)
							continue;
						strTemp.Format(_T("%.2f %s"), dblHz, strDegree);
						m_pDC->TextOutW((int)m_textPt.X, (int)m_textPt.Y, strTemp);
					}
				}
			}
		}
	}
	else
	{//m_textPt는 오른쪽부터 출력됨
		m_pDC->SetTextAlign(TA_CENTER);

		int iVerticalNum = CStatus::Instance()->m_iverticalNum;
		int iHorizonNum = CStatus::Instance()->m_ihorizontalNum;

		strDegree.Format(_T("%s"), CStatus::Instance()->m_strDegree);
		strUnit.Format(_T("%f"), CStatus::Instance()->m_dblUnit);
		if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
			SetGraphRectangleArea(dlgRect);
		if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
			SetReviewGraphRectangleArea(dlgRect);

		dblDegreeInterval = _wtof(strUnit);
		nVerticalInterval = ONERECT_PIXEL;

		if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
		{
			for (int i = 0; i <= iVerticalNum; i++)
			{
				if (i == 0)
				{
					m_textPt.Y = (REAL)(m_timeRect.bottom - ((double)m_timeRect.Height() *0.7));
					strTemp.Format(_T("%.2f %s"), dblDegree, strDegree);
					m_pDC->TextOutW((int)m_textPt.X, (int)m_textPt.Y, strTemp);
				}
				else
				{
					m_textPt.X -= (REAL)nVerticalInterval;
					if (m_textPt.X <= m_graphRect.left)
						continue;
					dblDegree += dblDegreeInterval;
					strTemp.Format(_T("%.2f"), dblDegree);
					m_pDC->TextOutW((int)m_textPt.X, (int)m_textPt.Y, strTemp);
				}
			}
		}
		if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
		{
			m_textPt.Y = (REAL)(m_timeRect.bottom - ((double)m_timeRect.Height() *0.7));
			int iReviewCount = CStatus::Instance()->GetUnitTextPtXCount(nInputType);
			CString strUnit;
			CStatus::tUnitTextInfo tTextInfo;
			for (int i = 0; i < iReviewCount; i++)
			{
				tTextInfo = CStatus::Instance()->GetUnitTextPtX(nInputType);

				strUnit = tTextInfo.strUnit;
				strTemp.Format(_T("%s"), strUnit);
					
				Point textPt;
				textPt.X = tTextInfo.iTextPt;
				textPt.Y = (int)m_textPt.Y;

				if (textPt.X <= m_graphRect.left)
					continue;
				m_pDC->TextOutW(textPt.X, textPt.Y, strTemp);
			}
			Point DegreePt;
			DegreePt.X = m_graphRect.right;
			DegreePt.Y = (int)m_textPt.Y;

			m_pDC->SetTextAlign(TA_LEFT);
			strTemp.Format(_T("%s"), strDegree);
			m_pDC->TextOutW(DegreePt.X, DegreePt.Y, strTemp);
		}
	}
	font.DeleteObject();
}

void CDrawingHelper::SetGraphRectangleArea(CRect dlgRect)
{
	int nAlarmRectWidth = 30;
	m_clientRect.SetRect(m_iInterval, m_iInterval, dlgRect.Width() - (m_iInterval * 7), dlgRect.Height() - (m_iInterval * 2));
	m_alarmRect.SetRect(m_iInterval, m_iInterval, m_iInterval + nAlarmRectWidth, dlgRect.Height() - (m_iInterval * 2));
	m_graphRect.SetRect(m_alarmRect.right, m_iInterval, m_clientRect.right, m_clientRect.bottom);
	//TimeDlg 객체때 사용하기 위해 값을 저장(오른쪽 부터 출력되도록)
	m_textPt.X = (REAL)m_graphRect.right;
}

void CDrawingHelper::SetReviewGraphRectangleArea(CRect dlgRect)
{
	int nAlarmRectWidth = 30;
	m_clientRect.SetRect(m_iInterval + nAlarmRectWidth, m_iInterval, dlgRect.Width() - (m_iInterval * 7), dlgRect.Height() - (m_iInterval * 2));
	m_alarmRect.SetRect(m_iInterval, m_iInterval, m_iInterval + nAlarmRectWidth, dlgRect.Height() - (m_iInterval * 2));
	m_graphRect.SetRect(m_alarmRect.right, m_iInterval, m_clientRect.right, m_clientRect.bottom);
	
	//TimeDlg 객체때 사용하기 위해 값을 저장(왼쪽 부터 출력되도록)
	m_textPt.X = (REAL)m_graphRect.left;
	CStatus::Instance()->m_iAlaramRectWidth = m_alarmRect.Width();
}

void CDrawingHelper::SetFFTGraphRectangleArea(CRect dlgRect)
{
	//m_clientRect.SetRect(m_iInterval, m_iInterval, dlgRect.Width() - (m_iInterval ), dlgRect.Height() - (m_iInterval * 2));
	int iWidth;
	if (dlgRect.Width() >= 800)
		iWidth = 800 + m_iInterval;
	else
		iWidth = dlgRect.Width();
	
	m_clientRect.SetRect(m_iInterval, m_iInterval, iWidth, dlgRect.Height() - (m_iInterval * 2));
	m_graphRect.SetRect(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_clientRect.bottom);

	//TimeDlg 객체때 사용하기 위해 값을 저장(왼쪽 부터 출력되도록)
	m_textPt.X = (REAL)m_graphRect.left;
}

void CDrawingHelper::SetDeegreeDlgRect(CRect dlgRect)
{
	m_timeRect.SetRect(dlgRect.left,dlgRect.top,dlgRect.right,dlgRect.bottom);
}

void CDrawingHelper::DrawingGraph(int nInputType)
{
	//LONG lStartTime = ::GetTickCount();
	//X , Y 좌표를 계산하여 CptrList 에 저장후 CptrList를 리턴해준다.
	tCalcValue calcValue;
	Point newPt(0, 0);
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;
	double dblMaxPos = m_graphRect.Height() / 2;
	int nTempIndex = 0;
	int iScale = _ttoi(CStatus::Instance()->m_strGraphScale);
	int nIndex = CStatus::Instance()->m_iDataCount;
	int nCount = nIndex;

	if (nCount == 0)
		return;
#pragma region X좌표 구하기(iRePosX)
	double dblTimeUnit = CStatus::Instance()->m_dblUnit;		//ex 0.1	

	int nFrequeny = (int)(CStatus::Instance()->GetDeviceFrequency());
	

	double dblOneRectDrawNum = (double)nFrequeny * dblTimeUnit; // 가변 되야함 한칸당 몇개씩 그릴거냐
	m_dblOneRectInterval = m_graphRect.Height() / CStatus::Instance()->m_ihorizontalNum;// 한칸당 높이
	CStatus::Instance()->m_nDrawPointNum = (int)(dblOneRectDrawNum * ((double)m_graphRect.Width() / (double)ONERECT_PIXEL));
	double dblGraphPosWidth = (double)ONERECT_PIXEL / dblOneRectDrawNum;
#pragma endregion

	int iDrawDataCount = CStatus::Instance()->m_nDrawPointNum;
	int iDrawStartPos;
	int nTempOverlapFirstValue = 0;
	int iScrollMovePixel = 0;
	double dblMaxValue = 0, dblMinValue = 0;
	
	iDrawStartPos = nCount - iDrawDataCount; 
	nIndex = iDrawDataCount;
	int iOverlapCount = (int)(dblOneRectDrawNum / (double)ONERECT_PIXEL);
	if (iOverlapCount != 0)
	{
		while (iDrawStartPos % iOverlapCount != 0)
			iDrawStartPos -= 1;
	}
	m_iOverlapToTalCount = 0;
	m_dblOverlapToTalSum = 0;
	m_iTempTriggerEnd = CStatus::Instance()->m_iDataCount;

	InspectTrigger(m_iTempTriggerEnd, nInputType);
	InspectGoNoGo(m_iTempTriggerEnd, nInputType);

	DrawingString(m_GraphType, nInputType);
	DrawIndicator();

	double dblValue = 0;
	tAVMDData NewData;

	CPen penDrawLine;
	if (m_GraphType == "RMS")
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_RMSPenColor);
	if (m_GraphType == "X")
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_XPenColor);
	if (m_GraphType == "Y")
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_YPenColor);
	if (m_GraphType == "Z")
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_ZPenColor);
	
	CPen* oldPen = m_pDC->SelectObject(&penDrawLine);

	if (!CStatus::Instance()->GetIsHoldCondition())
	{
		m_iHoldStart = iDrawStartPos;
		m_iHoldEnd = nCount;
		CStatus::Instance()->m_iHoldIndex = m_iHoldEnd;
	}
	else
	{
		iDrawStartPos = m_iHoldStart;
		nCount = m_iHoldEnd;
		nIndex = m_iHoldEnd - iDrawStartPos;
	}
	
	double* pdblPosRepository = new double[m_graphRect.bottom];
	memset(pdblPosRepository, 0, m_graphRect.bottom * sizeof(double));
	if (iOverlapCount > 1)
		m_pOverlayTempRepository = new double[m_graphRect.bottom];
	for (int i= iDrawStartPos; i< nCount; i++)
	{
		CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);

#pragma region Y좌표 구하기(X,Y,Z,RMS)
		if (m_GraphType == 'X')
		{
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iXValue, strTimeDomainUnit);

			double dblRePosWidth = abs(calcValue.AdjustedValue);

			if (i == nCount - 1)
				DrawingMonitoringValue(i, calcValue.RealValue);

			if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)// &&  (i == nCount-1))
			{	
				if (i == (nCount - 1))
				{
					BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, FALSE);
					AlarmRectColor(bIsSpecOut, CStatus::Evt_TRIGGER);
				}
			}
			if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
			{
				if (i == (nCount - 1))
				{
					if (m_strGoNoGoGraphType == "X")
					{
						AlarmRectColor(TRUE, CStatus::Evt_GONOGO);
					}
					else
					{
						BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, FALSE);
						AlarmRectColor(bIsSpecOut, CStatus::Evt_GONOGO);
					}
				}
			}
			m_nStepMove = CStatus::Instance()->m_iXStandardValue;
			if (NewData.iXValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale)+ m_iInterval;
			
			dblValue += Scroll_CalcValue(strTimeDomainUnit);
		}
		else if (m_GraphType == 'Y')
		{
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iYValue, strTimeDomainUnit);

			double dblRePosWidth = abs(calcValue.AdjustedValue);

			if (i == nCount - 1)
				DrawingMonitoringValue(i, calcValue.RealValue);

			if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)// &&  (i == nCount-1))
			{
				if (i == (nCount - 1))
				{
					BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, FALSE);
					AlarmRectColor(bIsSpecOut, CStatus::Evt_TRIGGER);
				}
			}
			if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
			{
				if (i == (nCount - 1))
				{
					if (m_strGoNoGoGraphType == "Y")
					{
						AlarmRectColor(TRUE, CStatus::Evt_GONOGO);
					}
					else
					{
						BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, FALSE);
						AlarmRectColor(bIsSpecOut, CStatus::Evt_GONOGO);
					}
				}
			}
			m_nStepMove = CStatus::Instance()->m_iYStandardValue;

			if (NewData.iYValue >= 0)
				dblValue = dblMaxPos - ((double)dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + ((double)dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += Scroll_CalcValue(strTimeDomainUnit);
		}
		else if (m_GraphType == 'Z')
		{
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iZValue, strTimeDomainUnit);

			double dblRePosWidth = abs(calcValue.AdjustedValue);

			if (i == nCount - 1)
				DrawingMonitoringValue(i, calcValue.RealValue);

			if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)// &&  (i == nCount-1))
			{
				if (i == (nCount - 1))
				{
					BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, FALSE);
					AlarmRectColor(bIsSpecOut, CStatus::Evt_TRIGGER);
				}
			}
			if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
			{
				if (i == (nCount - 1))
				{
					if (m_strGoNoGoGraphType == "Z")
					{
						AlarmRectColor(TRUE, CStatus::Evt_GONOGO);
					}
					else
					{
						BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, FALSE);
						AlarmRectColor(bIsSpecOut, CStatus::Evt_GONOGO);
					}
				}
			}
			m_nStepMove = CStatus::Instance()->m_iZStandardValue;

			if (NewData.iZValue >= 0)
				dblValue = dblMaxPos - ((double)dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + ((double)dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += Scroll_CalcValue(strTimeDomainUnit);
		}
		else if (m_GraphType == "RMS")
			{
				CalcRMS(&NewData);

				calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iRMSValue, strTimeDomainUnit);

				double dblRePosWidth = abs(calcValue.AdjustedValue);

				if (i == nCount - 1)
					DrawingMonitoringValue(i, calcValue.RealValue);

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)// &&  (i == nCount-1))
				{
					if (i == (nCount - 1))
					{
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, FALSE);
						AlarmRectColor(bIsSpecOut, CStatus::Evt_TRIGGER);
					}
				}
				if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
				{
					if (i == (nCount - 1))
					{
						if (m_strGoNoGoGraphType == "RMS")
						{
							AlarmRectColor(TRUE, CStatus::Evt_GONOGO);
						}
						else
						{
							BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, FALSE);
							AlarmRectColor(bIsSpecOut, CStatus::Evt_GONOGO);
						}
					}
				}
				m_nStepMove = CStatus::Instance()->m_iRMSStandardValue;

				if (NewData.iRMSValue >= 0)
					dblValue = (dblMaxPos*2.) - ((double)dblRePosWidth * (double)iScale) + m_iInterval;
				else
					dblValue = (dblMaxPos * 2) + ((double)dblRePosWidth * (double)iScale) + m_iInterval;

				dblValue += Scroll_CalcValue(strTimeDomainUnit);
			}
#pragma endregion
		if (iOverlapCount == 1 || iOverlapCount == 0)	// overlap이 없는 경우
		{												// overlap = 0 경우 => 주기가 500인 경우 TimeDomain : 0.1 
														// Overlap = 1 경우 => 주기가 1000인 경우 TimeDomain : 0.1
			m_iOverlapToTalCount = 0;
			if (nTempIndex == 0)		// for문에 총 1번들어옴
			{
				int iRePosX = m_graphRect.right - (int)((double)(nIndex)* dblGraphPosWidth);
				if (dblValue > m_graphRect.bottom)
					dblValue = m_graphRect.bottom;
				if (dblValue < m_graphRect.top)
					dblValue = m_graphRect.top;
				m_prevPt.X = iRePosX;
				m_prevPt.Y = (int)dblValue;
				nTempIndex++;
				nIndex--;
				continue;
			}
			else //  주기가 1000인 경우 TimeDomain >= 0.2
			{
				int iRePosX = m_graphRect.right - (int)((double)(nIndex)* dblGraphPosWidth);
				newPt.X = iRePosX;
				newPt.Y = (int)dblValue;
				if (newPt.Y <= m_iInterval)
					newPt.Y = m_iInterval;
				if (newPt.Y >= m_iInterval + m_graphRect.Height())
					newPt.Y = m_iInterval + m_graphRect.Height();
				m_pDC->MoveTo(m_prevPt.X, m_prevPt.Y);
				m_pDC->LineTo(newPt.X, newPt.Y);

				m_prevPt = newPt;
				nIndex--;
			}
		}
		else
		{
			if (nTempOverlapFirstValue == 0)
			{
				if (dblValue > m_graphRect.bottom)
					dblValue = m_graphRect.bottom;
				if (dblValue < m_graphRect.top)
					dblValue = m_graphRect.top;

				dblMaxValue = dblValue;
				dblMinValue = dblValue;
				nTempOverlapFirstValue++;
			}
			m_iOverlapToTalCount++;
			int iRePosX = m_graphRect.left + (int)((double)(nIndex)* dblGraphPosWidth);

			if (dblValue > m_graphRect.bottom)
				dblValue = m_graphRect.bottom;
			if (dblValue < m_graphRect.top)
				dblValue = m_graphRect.top;

			int nRepositoryIndex = (int)dblValue;
			int nStandardIndex = 0;
			if (m_GraphType == "RMS")
				nStandardIndex = (int)(dblMaxPos * 2. + (double)iScrollMovePixel);
			else
				nStandardIndex = (int)(dblMaxPos + (double)iScrollMovePixel);
			if (nStandardIndex < 0)
				nStandardIndex = 0;

			if (nStandardIndex > nRepositoryIndex)
			{
				for (int i = nRepositoryIndex; i < nStandardIndex; i++)
				{
					if (i < m_graphRect.bottom)
						pdblPosRepository[i]++;
				}
			}
			else
			{
				for (int i = nStandardIndex; i <= nRepositoryIndex; i++)
				{
					if (i< m_graphRect.bottom)
						pdblPosRepository[i]++;
				}
			}
			if (dblMaxValue < dblValue)
				dblMaxValue = dblValue;
			if (dblMinValue > dblValue)
				dblMinValue = dblValue;

			if (m_iOverlapToTalCount == iOverlapCount)
			{
				//걸리내기
				//printf("Max : %.f, Min : %.f \n", dblMaxValue, dblMinValue);
				int nMinIndex = (int)dblMinValue;
				int nMaxIndex = (int)dblMaxValue + 1;
				
				//printf("Min : %d, Max : %d\n", nMinIndex, nMaxIndex);
				for (int i = 0; i < nMinIndex; i++)
					pdblPosRepository[i] = 0;

				for (int i = nMaxIndex; i < m_graphRect.bottom; i++)
					pdblPosRepository[i] = 0;

				double dblUnit = CStatus::Instance()->m_dblUnit;
				int iCutOffValue = CalcCutOffValue(dblUnit);

				tCalcInfo tMaxMinInfo = CalcOverlayTendencyDelete(pdblPosRepository, iCutOffValue, nMinIndex, nMaxIndex);
				//deque에 넣기

				memcpy(m_pOverlayTempRepository, pdblPosRepository, m_graphRect.bottom * sizeof(double));

				tOverlayInfo tInfo;
				if (tMaxMinInfo.nMinValue > m_graphRect.Height() + m_iInterval)
					tMaxMinInfo.nMinValue = m_graphRect.Height() + m_iInterval;
				if (tMaxMinInfo.nMaxValue < m_iInterval)
					tMaxMinInfo.nMaxValue = m_iInterval;

				tInfo.nMaxIndex = tMaxMinInfo.nMaxValue;
				tInfo.nMinIndex = tMaxMinInfo.nMinValue;
				tInfo.pOverlayRepository = m_pOverlayTempRepository;

				m_pOverlayRepository.push_back(tInfo);
				int size = m_pOverlayRepository.size();
				m_iOverlapToTalCount = 0;
				dblMaxValue = 0; dblMinValue = 0;
				nTempOverlapFirstValue = 0;
				nIndex--;
			}
			else
			{
				nIndex--;
			}
		}
	}
	m_pDC->SelectObject(oldPen);
	penDrawLine.DeleteObject();
	if (iOverlapCount > 1)
	{
		DrawOverlayGraph(FALSE);
	}
	
	delete[] pdblPosRepository;
	m_TempPt = newPt;

	//LONG lEndTime = ::GetTickCount();
	//printf("%d\n", lEndTime - lStartTime);
	return;
}

void CDrawingHelper::DrawingReviewGraph(int nInputType, int nPriority, BOOL bIsOverColor, BOOL bIsDrawString)
{
	//LONG lStartTime = ::GetTickCount();
	//X , Y 좌표를 계산하여 CptrList 에 저장후 CptrList를 리턴해준다.
	tCalcValue calcValue;
	Point newPt(0, 0);
	int nTempIndex = 0;
	double dblMaxPos = m_graphRect.Height() / 2;
	double dblRePosWidth = 0;
	double dblValue = 0;
	int iScale = _ttoi(CStatus::Instance()->m_strGraphScale);

	if (bIsDrawString)
	{
		DrawingString(m_GraphType, nInputType);
		DrawIndicator();
	}
	tAVMDData NewData;

	int nIndex = 0;
	int nCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
	if (nCount == 0)
		return;

#pragma region X좌표 구하기
	double dblTimeUnit = CStatus::Instance()->m_dblUnit;		//ex 0.1	
	int nFrequeny = (int)(CStatus::Instance()->GetDeviceFrequency());
	double dblOneRectDrawNum = (double)nFrequeny * dblTimeUnit; // 가변 되야함 한칸당 몇개씩 그릴거냐
	m_dblOneRectInterval = m_graphRect.Height() / CStatus::Instance()->m_ihorizontalNum;// 한칸당 높이
	CStatus::Instance()->m_nDrawPointNum = (int)(dblOneRectDrawNum * ((double)m_graphRect.Width() / (double)ONERECT_PIXEL));
	double dblTempGraphPosWidth = (double)ONERECT_PIXEL / dblOneRectDrawNum;
	CString strTemp;
	strTemp.Format(_T("%.5f"), dblTempGraphPosWidth);
	double dblGraphPosWidth = _wtof(strTemp);
	double dblMoveAmountofOnePixel = (double)CStatus::Instance()->m_nDrawPointNum / (double)m_graphRect.Width();
	CStatus::Instance()->SetOnePixelDrawCount(dblOneRectDrawNum / ONERECT_PIXEL);
	int test = CStatus::Instance()->m_nDrawPointNum;
#pragma endregion

	int iDrawStartNum = 0;
	int iDrawEndNum = 0;
	if (nInputType == 0)
	{
		if (CStatus::Instance()->m_bIsTimeScaleChange == TRUE)// && m_GraphType == _T("X"))
		{
			if (!CStatus::Instance()->m_bIsPlayBack)
			{
				double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

				int iNewPos = CStatus::Instance()->m_iStartPosText;
				CStatus::Instance()->SetMarkEditStart(iNewPos, 0);
				CStatus::Instance()->SetMarkClickEvent(TRUE, 0);
				double dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;
				CStatus::Instance()->SetDragAmount(dblNewDragNum, 0);

				CStatus::Instance()->m_bIsTimeScaleChange = FALSE;
			}
			else
			{
				if (m_dblPrevTimeUnit != dblTimeUnit)
				{
					double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

					int iNewPos = CStatus::Instance()->m_iStartPosText;
					CStatus::Instance()->SetMarkEditStart(iNewPos, 0);
					CStatus::Instance()->SetMarkClickEvent(TRUE, 0);
					double dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;
					CStatus::Instance()->SetDragAmount(dblNewDragNum, 0);

					CStatus::Instance()->m_bIsTimeScaleChange = FALSE;
				}
			}
		}
	}
	else
	{
		if (CStatus::Instance()->m_bIsCompareTimeScaleChange == TRUE)// && m_GraphType == _T("X"))
		{
			if (!CStatus::Instance()->m_bIsPlayBack)
			{
				double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

				int iNewPos = CStatus::Instance()->m_iCompareStartPosText;
				CStatus::Instance()->SetMarkEditStart(iNewPos, 1);
				CStatus::Instance()->SetMarkClickEvent(TRUE, 1);
				double dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;
				CStatus::Instance()->SetDragAmount(dblNewDragNum, 1);

				CStatus::Instance()->m_bIsCompareTimeScaleChange = FALSE;
			}
			else
			{
				double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

				int iNewPos = CStatus::Instance()->m_iCompareStartPosText;
				CStatus::Instance()->SetMarkEditStart(iNewPos, 1);
				CStatus::Instance()->SetMarkClickEvent(TRUE, 1);
				double dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;
				CStatus::Instance()->SetDragAmount(dblNewDragNum, 1);

				CStatus::Instance()->m_bIsCompareTimeScaleChange = FALSE;
			}
		}
	}
	if(CStatus::Instance()->GetMarkClickEvent(nInputType) == TRUE)
	{
		iDrawStartNum = CStatus::Instance()->GetMarkEditStart(nInputType);
		iDrawEndNum = iDrawStartNum + CStatus::Instance()->m_nDrawPointNum;
	}
	else
	{
		iDrawStartNum = CStatus::Instance()->GetReviewDrawStart(nInputType);
		iDrawEndNum = CStatus::Instance()->GetReviewDrawEnd(nInputType);
	}
	
	//mark String 쓰기
	//Gdiplus::Font font(_T("Arial"), 12, FontStyleBold, UnitPixel);
	//SolidBrush sbrush(Color::Black);
	//StringFormat strFormat;
	//strFormat.SetAlignment(StringAlignmentCenter);
	//strFormat.SetLineAlignment(StringAlignmentCenter);
	
	CPen penDrawLine;
	if (bIsOverColor)
	{
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_OverlayColor);
	}
	else
	{
		if (m_GraphType == "RMS")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_RMSPenColor);
		if (m_GraphType == "X")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_XPenColor);
		if (m_GraphType == "Y")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_YPenColor);
		if (m_GraphType == "Z")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_ZPenColor);
	}	

	CPen* oldPen = m_pDC->SelectObject(&penDrawLine);

	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;
	BOOL bIsMarkIndex = FALSE;
	tMarkCheckInfo tMarkCheck;
	int iOverlapCount = (int)(dblOneRectDrawNum /(double)ONERECT_PIXEL);
	if (iOverlapCount != 0)
	{
		/*if (iDrawEndNum % iOverlapCount != 0)
		{
			while (iDrawEndNum % iOverlapCount != 0)
			{
				iDrawEndNum -= 1;
			}
		}*/
		if (iDrawStartNum % iOverlapCount != 0)
		{
			while (iDrawStartNum % iOverlapCount != 0)
			{
				iDrawStartNum -= 1;
			}
		}
	}
	m_iOverlapToTalCount = 0;
	m_dblOverlapToTalSum = 0;
	////
	double dblMaxValue = 0, dblMinValue = 0;
	int nPlusCount = 0, nMinusCount = 0, nTempOverlapFirstValue = 0;
	double* pdblPosRepository = new double[m_graphRect.bottom];
	int testValue = m_graphRect.bottom;
	memset(pdblPosRepository, 0, m_graphRect.bottom * sizeof(double));
	int iScrollMovePixel = 0;
	int iTestDrawCount = 0;
	int nTestCount = 0;
	if(iOverlapCount > 1)
		m_pOverlayTempRepository = new double[m_graphRect.bottom];

	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
	if (iDrawEndNum > nDataCount)
		iDrawEndNum = nDataCount;

	CalcUnitPt(dblGraphPosWidth, iDrawStartNum, iDrawEndNum, nInputType, nPriority);
	CalcMarkPt(dblGraphPosWidth, iDrawStartNum, iDrawEndNum, nInputType, nPriority);

	
		
	//CStatus::Instance()->m_nOverlayDrawPtPixel = (iDrawEndNum - iDrawStartNum) / iOverlapCount;
	for (int i = iDrawStartNum; i < iDrawEndNum; i++)
	{
		if (CStatus::Instance()->GetRepository()->GetDataCount(nInputType) < i)
			return;

		if (iDrawStartNum == CStatus::Instance()->GetRepository()->GetDataCount(nInputType))
		{
			CStatus::Instance()->GetRepository()->GetAtData(&NewData, CStatus::Instance()->GetRepository()->GetDataCount(nInputType) - 1, nInputType);
		}
		else
		{
			CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
		}
#pragma region Y좌표 구하기(X, Y, Z, RMS)
		if (m_GraphType == 'X')
		{
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iXValue, strTimeDomainUnit);
			dblRePosWidth = abs(calcValue.AdjustedValue);
			if (nInputType == 1 && nPriority == 1)
				m_nStepMove = CStatus::Instance()->m_iCompareStandardValue;
			else
				m_nStepMove = CStatus::Instance()->m_iXStandardValue;

			if (NewData.iXValue >= 0)
				dblValue = dblMaxPos - ((double)dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + ((double)dblRePosWidth * (double)iScale) + m_iInterval;

			iScrollMovePixel = (int)Scroll_CalcValue(strTimeDomainUnit);
			dblValue += Scroll_CalcValue(strTimeDomainUnit);
			if (bIsDrawString)
			{
				if (i == iDrawStartNum)
					DrawingMonitoringValue(i, calcValue.RealValue);
			}
		}
		else if (m_GraphType == 'Y')
		{
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iYValue, strTimeDomainUnit);
			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (nInputType == 1 && nPriority == 1)
				m_nStepMove = CStatus::Instance()->m_iCompareStandardValue;
			else
				m_nStepMove = CStatus::Instance()->m_iYStandardValue;

			if (NewData.iYValue >= 0)
				dblValue = dblMaxPos - ((double)dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + ((double)dblRePosWidth * (double)iScale) + m_iInterval;

			iScrollMovePixel = (int)Scroll_CalcValue(strTimeDomainUnit);
			dblValue += Scroll_CalcValue(strTimeDomainUnit);
			if (bIsDrawString)
			{
				if (i == iDrawStartNum)
					DrawingMonitoringValue(i, calcValue.RealValue);
			}
		}
		else if (m_GraphType == 'Z')
		{
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iZValue, strTimeDomainUnit);
			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (nInputType == 1 && nPriority == 1)
				m_nStepMove = CStatus::Instance()->m_iCompareStandardValue;
			else
				m_nStepMove = CStatus::Instance()->m_iZStandardValue;

			if (NewData.iZValue >= 0)
				dblValue = dblMaxPos - ((double)dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + ((double)dblRePosWidth * (double)iScale) + m_iInterval;

			iScrollMovePixel = (int)Scroll_CalcValue(strTimeDomainUnit);
			dblValue += Scroll_CalcValue(strTimeDomainUnit);
			if (bIsDrawString)
			{
				if (i == iDrawStartNum)
					DrawingMonitoringValue(i, calcValue.RealValue);
			}
		}
		else if (m_GraphType == "RMS")
		{
			CalcRMS(&NewData);

			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iRMSValue, strTimeDomainUnit);
			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (nInputType == 1 && nPriority == 1)
				m_nStepMove = CStatus::Instance()->m_iCompareStandardValue;
			else
				m_nStepMove = CStatus::Instance()->m_iRMSStandardValue;

			if (NewData.iRMSValue >= 0)
				dblValue = (dblMaxPos * 2.) - ((double)dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = (dblMaxPos * 2.) + ((double)dblRePosWidth * (double)iScale) + m_iInterval;

			iScrollMovePixel = (int)Scroll_CalcValue(strTimeDomainUnit);
			dblValue += Scroll_CalcValue(strTimeDomainUnit);
			if (bIsDrawString)
			{
				if (i == iDrawStartNum)
					DrawingMonitoringValue(i, calcValue.RealValue);
			}
		}
#pragma endregion

		
		if (iOverlapCount == 1 || iOverlapCount == 0) // overlap이 없는 경우
		{												// overlap = 0 경우 => 주기가 500인 경우 TimeDomain : 0.1 
														// Overlap = 1 경우 => 주기가 1000인 경우 TimeDomain : 0.1
			m_iOverlapToTalCount = 0;
			if (nTempIndex == 0)		// for문에 총 1번들어옴
			{
				int iRePosX = m_graphRect.left + (int)((double)(nIndex)* dblGraphPosWidth);
				if (dblValue > m_graphRect.bottom)
					dblValue = m_graphRect.bottom;
				if (dblValue < m_graphRect.top)
					dblValue = m_graphRect.top;
				m_prevPt.X = iRePosX;
				m_prevPt.Y = (int)dblValue;
				nTempIndex++;
				nIndex++;
				continue;
			}
			else
			{
				int iRePosX = m_graphRect.left + (int)((double)(nIndex)* dblGraphPosWidth);
				newPt.X = iRePosX;
				newPt.Y = (int)dblValue;
				if (newPt.Y <= m_iInterval)
					newPt.Y = m_iInterval;
				if (newPt.Y >= m_iInterval + m_graphRect.Height())
					newPt.Y = m_iInterval + m_graphRect.Height();
				m_pDC->MoveTo(m_prevPt.X, m_prevPt.Y);
				m_pDC->LineTo(newPt.X, newPt.Y);

				m_prevPt = newPt;
				nIndex++;
			}
		}
		else
		{
			if (nTempOverlapFirstValue == 0)
			{
				if (dblValue > m_graphRect.bottom)
					dblValue = m_graphRect.bottom;
				if (dblValue < m_graphRect.top)
					dblValue = m_graphRect.top;

				dblMaxValue = dblValue;
				dblMinValue = dblValue;
				nTempOverlapFirstValue++;
			}
			m_iOverlapToTalCount++;

			int iRePosX = m_graphRect.left + (int)((double)(nIndex)* dblGraphPosWidth);
			
			if (dblValue > m_graphRect.bottom)
				dblValue = m_graphRect.bottom;
			if (dblValue < m_graphRect.top)
				dblValue = m_graphRect.top;

			int nRepositoryIndex = (int)dblValue;
			int nStandardIndex = 0;
			if (m_GraphType == "RMS")
				nStandardIndex = (int)(dblMaxPos * 2. + (double)iScrollMovePixel);
			else
				nStandardIndex = (int)(dblMaxPos + (double)iScrollMovePixel);
			if (nStandardIndex < 0)
				nStandardIndex = 0;

			if (nStandardIndex > nRepositoryIndex)
			{
				for (int i = nRepositoryIndex; i < nStandardIndex; i++)
				{
					if(i< m_graphRect.bottom)
						pdblPosRepository[i]++;
				}
			}
			else
			{
				for (int i = nStandardIndex; i <= nRepositoryIndex; i++)
				{
					if (i< m_graphRect.bottom)
						pdblPosRepository[i]++;
				}
			}
			if (dblMaxValue < dblValue)		
				dblMaxValue = dblValue;
			if (dblMinValue > dblValue)		
				dblMinValue = dblValue;
		
			if (m_iOverlapToTalCount == iOverlapCount)
			{
				//걸리내기
				//printf("Max : %.f, Min : %.f \n", dblMaxValue, dblMinValue);
				int nMinIndex  = (int)dblMinValue;
				int nMaxIndex = (int)dblMaxValue + 1;

				for (int i = 0; i < nMinIndex; i++)
					pdblPosRepository[i] = 0;

				for (int i = nMaxIndex; i < m_graphRect.bottom; i++)
					pdblPosRepository[i] = 0;
				
				double dblUnit = CStatus::Instance()->m_dblUnit;
				int iCutOffValue = CalcCutOffValue(dblUnit);

				tCalcInfo tMaxMinInfo =	CalcOverlayTendencyDelete(pdblPosRepository, iCutOffValue, nMinIndex, nMaxIndex);
				//deque에 넣기
				
				memcpy(m_pOverlayTempRepository, pdblPosRepository, m_graphRect.bottom *sizeof(double));

				tOverlayInfo tInfo;
				if (tMaxMinInfo.nMinValue > m_graphRect.Height() + m_iInterval)
					tMaxMinInfo.nMinValue = m_graphRect.Height() + m_iInterval;
				if (tMaxMinInfo.nMaxValue < m_iInterval)
					tMaxMinInfo.nMaxValue = m_iInterval;

				tInfo.nMaxIndex = tMaxMinInfo.nMaxValue;
				tInfo.nMinIndex = tMaxMinInfo.nMinValue;
				tInfo.pOverlayRepository = m_pOverlayTempRepository;
			
				m_pOverlayRepository.push_back(tInfo);
				int size = m_pOverlayRepository.size();
				m_iOverlapToTalCount = 0;
				dblMaxValue = 0; dblMinValue = 0;
				nTempOverlapFirstValue = 0;
				nIndex++;
				//그리기
			}
			else
			{
				nIndex++;
			}
		}
	}

	m_pDC->SelectObject(oldPen);
	penDrawLine.DeleteObject();
	m_dblPrevTimeUnit = dblTimeUnit;
	m_TempPt = newPt;
	if (iOverlapCount > 1)
	{
		DrawOverlayGraph(bIsOverColor);
	}

	delete[] pdblPosRepository;
	return;
}
int CDrawingHelper::CalcCutOffValue(double dblTimeUnit)
{
	int iCutOffValue = 1;
	//if (dblTimeUnit == 20.0)
	//	iCutOffValue = 5;
	//else if (dblTimeUnit == 10.0)
	//	iCutOffValue = 3;
	//else if (dblTimeUnit == 5.0)
	//	iCutOffValue = 2;
	//else
	//	iCutOffValue = 1;
	return iCutOffValue;
}

CDrawingHelper::tCalcInfo CDrawingHelper::CalcOverlayTendencyDelete(double *dblSrcArray, int iValue, int nMinIndex, int nMaxIndex) // iValue 이상 통과
{
	int nMax = nMaxIndex, nMin = m_graphRect.bottom;
	BOOL bFirstEntery= TRUE;
	tCalcInfo tCalc;
	int nNewValue = iValue;
	
	for (int i = 0; i < m_graphRect.bottom; i++)
	{
		double dblArrayValue = dblSrcArray[i];
		if (dblArrayValue != 0 && dblArrayValue < nNewValue)
			dblSrcArray[i] = 0;
	}
	for (int i = 0; i < m_graphRect.bottom; i++)
	{
		double dblArrayValue = dblSrcArray[i];
		if (dblArrayValue != 0)
		{
			if (bFirstEntery)
			{
				nMin = i;
				bFirstEntery = FALSE;
			}
			if (i > nMax)
			{
				nMax = i;
			}
			else
			{
				nMax = nMaxIndex;
			}
		}
	}
	tCalc.nMaxValue = nMax;
	tCalc.nMinValue = nMin;
	
	return tCalc;
}
void CDrawingHelper::DrawViewFilterOverlayGraph(BOOL bIsOverColor)
{
	CPen penDrawLine;
	if (bIsOverColor)
	{
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_OverlayColor);
	}
	else
	{
		if (m_GraphType == "RMS")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_RMSPenColor);
		if (m_GraphType == "X")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_XPenColor);
		if (m_GraphType == "Y")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_YPenColor);
		if (m_GraphType == "Z")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_ZPenColor);
	}

	CPen* oldPen = m_pDC->SelectObject(&penDrawLine);

	int nStartLocationX = m_graphRect.left;
	int nEndLocationX = m_graphRect.right;
	int nTempFirst = 0;
	Point ArrayMinPt, ArrayMaxPt;
	BOOL bIsFirst = TRUE;
	Point PrevPt;
	int test = CStatus::Instance()->m_nDrawPointNum;
	//int nOverlayDrawPixcel = CStatus::Instance()->m_nOverlayDrawPtPixel;
	int nSize = m_pOverlayRepository.size();
	int nIndex = nSize-1;
	printf("nIndex : %d\n", nIndex);
	bool isFirstEnter = true;
	for (int i = 0; i < nSize; i++)
	{
		if (m_pOverlayRepository.size() == 0)
		{
			delete[] m_pOverlayTempRepository;
			if (m_pOverlayRepository.size() != 0)
				m_pOverlayRepository.clear();
			return;
		}

	
		//printf("%d 배열 Max : %d, Min : %d\n", i, ArrayInfo.nMaxIndex, ArrayInfo.nMinIndex);
		int iXPos = m_graphRect.left + nIndex;
		nIndex--;
		if (iXPos < m_graphRect.right)
		{
			tOverlayInfo ArrayInfo = m_pOverlayRepository.at(i);
			if (i == 0 || isFirstEnter == true)
			{
				isFirstEnter = false;
				ArrayMinPt.X = iXPos;
				ArrayMinPt.Y = ArrayInfo.nMinIndex;

				ArrayMaxPt.X = iXPos;
				ArrayMaxPt.Y = ArrayInfo.nMaxIndex;

				if (ArrayMinPt.Y < m_iInterval)		ArrayMinPt.Y = m_iInterval;
				if (ArrayMinPt.Y > m_graphRect.Height() + m_iInterval)	ArrayMinPt.Y = m_graphRect.Height() + m_iInterval;

				if (ArrayMaxPt.Y < m_iInterval)		ArrayMaxPt.Y = m_iInterval;
				if (ArrayMaxPt.Y > m_graphRect.Height() + m_iInterval)	ArrayMaxPt.Y = m_graphRect.Height() + m_iInterval;

				if (ArrayMinPt.X <= 0 || ArrayMaxPt.X <=0)
				{
					int gg = 10;
				}
				m_pDC->MoveTo(ArrayMinPt.X, ArrayMinPt.Y);
				m_pDC->LineTo(ArrayMaxPt.X, ArrayMaxPt.Y);
				PrevPt = ArrayMaxPt;
			}
			else
			{
				ArrayMinPt.X = iXPos;
				ArrayMinPt.Y = ArrayInfo.nMinIndex;

				ArrayMaxPt.X = iXPos;
				ArrayMaxPt.Y = ArrayInfo.nMaxIndex;

				if (ArrayMinPt.Y < m_iInterval)		ArrayMinPt.Y = m_iInterval;
				if (ArrayMinPt.Y > m_graphRect.Height() + m_iInterval)	ArrayMinPt.Y = m_graphRect.Height() + m_iInterval;

				if (ArrayMaxPt.Y < m_iInterval)		ArrayMaxPt.Y = m_iInterval;
				if (ArrayMaxPt.Y > m_graphRect.Height() + m_iInterval)	ArrayMaxPt.Y = m_graphRect.Height() + m_iInterval;

				if (ArrayMinPt.X <= 0 || ArrayMaxPt.X <= 0 || PrevPt.X <=0)
				{
					int gg = 10;
				}

				m_pDC->MoveTo(PrevPt.X, PrevPt.Y);
				m_pDC->LineTo(ArrayMinPt.X, ArrayMinPt.Y);
				m_pDC->MoveTo(ArrayMinPt.X, ArrayMinPt.Y);
				m_pDC->LineTo(ArrayMaxPt.X, ArrayMaxPt.Y);
				PrevPt = ArrayMaxPt;
			}
		}
	}
	if (m_pOverlayRepository.size() != 0)
		m_pOverlayRepository.clear();

	m_pDC->SelectObject(oldPen);
	penDrawLine.DeleteObject();
	delete[] m_pOverlayTempRepository;
}
void CDrawingHelper::DrawOverlayGraph(BOOL bIsOverColor)
{
	CPen penDrawLine;
	if (bIsOverColor)
	{
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_OverlayColor);
	}
	else
	{
		if (m_GraphType == "RMS")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_RMSPenColor);
		if (m_GraphType == "X")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_XPenColor);
		if (m_GraphType == "Y")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_YPenColor);
		if (m_GraphType == "Z")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_ZPenColor);
	}

	CPen* oldPen = m_pDC->SelectObject(&penDrawLine);

	int nStartLocationX = m_graphRect.left;
	int nEndLocationX = m_graphRect.right;
	int nTempFirst = 0;
	Point ArrayMinPt, ArrayMaxPt;
	BOOL bIsFirst = TRUE;
	Point PrevPt;
	//int nOverlayDrawPixcel = CStatus::Instance()->m_nOverlayDrawPtPixel;
	int nSize = m_pOverlayRepository.size();
	for (int i = 0; i < nSize; i++)
	{
		if (m_pOverlayRepository.size() == 0)
		{
			delete[] m_pOverlayTempRepository;
			if (m_pOverlayRepository.size() != 0)
				m_pOverlayRepository.clear();
			return;
		}

		tOverlayInfo ArrayInfo = m_pOverlayRepository.at(i);
		//printf("%d 배열 Max : %d, Min : %d\n", i, ArrayInfo.nMaxIndex, ArrayInfo.nMinIndex);
		int iXPos = m_graphRect.left + i;

		if (iXPos < m_graphRect.right)
		{
			if (i == 0)
			{
				ArrayMinPt.X = iXPos;
				ArrayMinPt.Y = ArrayInfo.nMinIndex;

				ArrayMaxPt.X = iXPos;
				ArrayMaxPt.Y = ArrayInfo.nMaxIndex;

				if (ArrayMinPt.Y < m_iInterval)		ArrayMinPt.Y = m_iInterval;
				if (ArrayMinPt.Y > m_graphRect.Height() + m_iInterval)	ArrayMinPt.Y = m_graphRect.Height() + m_iInterval;

				if (ArrayMaxPt.Y < m_iInterval)		ArrayMaxPt.Y = m_iInterval;
				if (ArrayMaxPt.Y > m_graphRect.Height() + m_iInterval)	ArrayMaxPt.Y = m_graphRect.Height() + m_iInterval;

				m_pDC->MoveTo(ArrayMinPt.X, ArrayMinPt.Y);
				m_pDC->LineTo(ArrayMaxPt.X, ArrayMaxPt.Y);
				PrevPt = ArrayMaxPt;
			}
			else
			{
				ArrayMinPt.X = iXPos;
				ArrayMinPt.Y = ArrayInfo.nMinIndex;

				ArrayMaxPt.X = iXPos;
				ArrayMaxPt.Y = ArrayInfo.nMaxIndex;

				if (ArrayMinPt.Y < m_iInterval)		ArrayMinPt.Y = m_iInterval;
				if (ArrayMinPt.Y > m_graphRect.Height() + m_iInterval)	ArrayMinPt.Y = m_graphRect.Height() + m_iInterval;

				if (ArrayMaxPt.Y < m_iInterval)		ArrayMaxPt.Y = m_iInterval;
				if (ArrayMaxPt.Y > m_graphRect.Height() + m_iInterval)	ArrayMaxPt.Y = m_graphRect.Height() + m_iInterval;

				m_pDC->MoveTo(PrevPt.X, PrevPt.Y);
				m_pDC->LineTo(ArrayMinPt.X, ArrayMinPt.Y);
				m_pDC->MoveTo(ArrayMinPt.X, ArrayMinPt.Y);
				m_pDC->LineTo(ArrayMaxPt.X, ArrayMaxPt.Y);
				PrevPt = ArrayMaxPt;
			}
		}
	}
	if (m_pOverlayRepository.size() != 0)
		m_pOverlayRepository.clear();

	m_pDC->SelectObject(oldPen);
	penDrawLine.DeleteObject();
	delete[] m_pOverlayTempRepository;
}

double CDrawingHelper::CalcRMS(tAVMDData *pAVMDData)
{
	double tempRMS = 0;
	tempRMS = ((pAVMDData->iXValue * pAVMDData->iXValue)
		+ (pAVMDData->iYValue * pAVMDData->iYValue)
		+ (pAVMDData->iZValue * pAVMDData->iZValue)) / (double)3;

	pAVMDData->iRMSValue = (int)(sqrt(tempRMS));
	return pAVMDData->iRMSValue;
}

void CDrawingHelper::DrawFFTGraph(CPoint fftMeasurePos, int nInputType, int nPriority, BOOL bIsOverColor)
{
	int nDataCount = 0;

	int iFftSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	double *dblFFT;
	double test[10000];
	dblFFT = new double[iFftSampleSize / 2];
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
	{
		nDataCount = CStatus::Instance()->m_iDataCount;
		if (nDataCount - iFftSampleSize < 0)
			return;
	}

	for (int j = 0; j < iFftSampleSize / 2; j++)
	{
		dblFFT[j] = AmplitudeScaled(m_pFFTGraphData[nInputType][j].Re, m_pFFTGraphData[nInputType][j].Im, iFftSampleSize/2 , iFftSampleSize);
		test[j] = dblFFT[j];
	}
	Point newPt,nextPt;
	int nData = 0;
	double dblMaxPos = m_graphRect.Height();
	double dblRePosWidth = 0;
	double DblCalPointX = 0;
	
	double dblFFTScale = _wtof(CStatus::Instance()->m_strFFTScale);
	int iFFtUnit = CStatus::Instance()->m_iFFTUnit;
	int iverticalNum = CStatus::Instance()->m_iFfftVerticalNum;

	double dblGraphWidthOneInterval = (double)m_graphRect.Width() / (double)(iFFtUnit *iverticalNum); // 최대 그리는 주파수 정해지는 부분
	
	CString strFftInfo;

	//Pen graphPen(Color(255, 255, 0, 0), 1.0f);
	
	CPen penDrawLine;
	if (bIsOverColor)
	{
		penDrawLine.CreatePen(PS_SOLID | PS_ENDCAP_FLAT, 0, CStatus::Instance()->m_OverlayColor);
	}
	else
	{
		if (m_GraphType == "RMS")
			penDrawLine.CreatePen(PS_SOLID | PS_ENDCAP_FLAT, 0, CStatus::Instance()->m_RMSPenColor);
		if (m_GraphType == "X")
			penDrawLine.CreatePen(PS_SOLID | PS_ENDCAP_FLAT, 0, CStatus::Instance()->m_XPenColor);
		if (m_GraphType == "Y")
			penDrawLine.CreatePen(PS_SOLID | PS_ENDCAP_FLAT, 0, CStatus::Instance()->m_YPenColor);
		if (m_GraphType == "Z")
			penDrawLine.CreatePen(PS_SOLID | PS_ENDCAP_FLAT, 0, CStatus::Instance()->m_ZPenColor);
	}

	CPen* oldPen = m_pDC->SelectObject(&penDrawLine);

	int iNextPt_X = 0;
	int iMaxHz = (int)CStatus::Instance()->m_DeviceFrequency / 2;
	int* iFFTHz = new int[iMaxHz]; // MaxHz : 500

	int iFFTStartPos = CStatus::Instance()->m_iFFTShowStartPos;
	int nIndex = (int)((double)iFFTStartPos *(double)iFftSampleSize / (double)FFT_SAMPLING_FREQUENCY);

	int iFFTEndPos = nIndex + m_graphRect.Width();
	if(iFftSampleSize/2 < iFFTEndPos)
		iFFTEndPos = iFftSampleSize/2;
	int nInterval = 0;

	int iGraphWidthOneInterval = 0;
	int FFTShowRatio = CStatus::Instance()->m_iFFTShowRatio;
	if (iFftSampleSize == 32)	iGraphWidthOneInterval = 128 * FFTShowRatio;
	if (iFftSampleSize == 64)	iGraphWidthOneInterval = 64 * FFTShowRatio;
	if (iFftSampleSize == 128)	iGraphWidthOneInterval = 32 * FFTShowRatio;
	if (iFftSampleSize == 256)	iGraphWidthOneInterval = 16 * FFTShowRatio;
	if (iFftSampleSize == 512)	iGraphWidthOneInterval = 8 * FFTShowRatio;
	if (iFftSampleSize == 1024)	iGraphWidthOneInterval = 4 * FFTShowRatio;
	if (iFftSampleSize == 2048)	iGraphWidthOneInterval = 2 * FFTShowRatio;
	if (iFftSampleSize == 4096)	iGraphWidthOneInterval = 1 * FFTShowRatio;

	CFont font;
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = FONT_SIZE;
	lf.lfWeight = FW_BOLD;
	wsprintf(lf.lfFaceName, TEXT("%s"), TEXT("Arial"));
	font.CreateFontIndirectW(&lf);
	m_pDC->SelectObject(&font);
	m_pDC->SetBkMode(TRANSPARENT);
	m_pDC->SetTextAlign(TA_LEFT);

	//for (int i = nIndex; i < iF7FTEndPos; i++)
	for (int i = nIndex; i < iFFTEndPos; i++)
	{
		newPt.X = (int)(m_graphRect.left + ((double)nInterval*iGraphWidthOneInterval));
		iNextPt_X = (int)(m_graphRect.left + ((double)(nInterval + 1)*iGraphWidthOneInterval));
		nInterval++;
		if (i >= iFftSampleSize / 2)
			return;
	
		if (strTimeDomainUnit == "g-Force")
			dblRePosWidth = (dblMaxPos * (dblFFT[i] / 981.)) / (4.0 * dblFFTScale);
		
		if (strTimeDomainUnit == "GAL")
			dblRePosWidth = (dblMaxPos * dblFFT[i]) / (4.0 * dblFFTScale);

		double dblFftInfoPosX = fftMeasurePos.x;
		if (dblFftInfoPosX > m_graphRect.right)
			dblFftInfoPosX = (double)m_graphRect.right;
		double dblFftInfoPosY = fftMeasurePos.y;

		if (dblFftInfoPosX >= newPt.X&& dblFftInfoPosX < iNextPt_X)
		{
			CString strHzValue;
			int iLineLength = (int)(m_graphRect.Height()*3. / (double)CStatus::Instance()->m_iverticalNum);
			BOOL bIsOverlay = CStatus::Instance()->GetOverlayCondition();
			tCalcValue calcValue;
			Pen graphLinePen(Color(125, 125, 125, 0), 1.0f);
			int iLinePos = 0;
			double dblCalcHz = 0;
			if (newPt.X == (int)dblFftInfoPosX)
			{
				calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblFFT[i], strTimeDomainUnit);
				dblCalcHz = (double)FFT_SAMPLING_FREQUENCY / (double)iFftSampleSize * (double)i;
				iLinePos = newPt.X;
			}
			else
			{
				////Point1
				double dblCurFftValue = dblFFT[i];
				double dblCurPos = newPt.X;
				////Point2
				double dblNextFftValue = dblFFT[i + 1];
				double dblNextPos = iNextPt_X;// *Ratio;

				double abs1, abs2;
				abs1 = abs(dblFftInfoPosX - dblCurPos);
				abs2 = abs(dblFftInfoPosX - dblNextPos);

				if (abs1 > abs2)
				{
					calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblFFT[i+1], strTimeDomainUnit);
					dblCalcHz = (double)FFT_SAMPLING_FREQUENCY / (double)iFftSampleSize * (double)(i +1);
					iLinePos = (int)dblNextPos;
				}
				else
				{
					calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblFFT[i], strTimeDomainUnit);
					dblCalcHz = (double)FFT_SAMPLING_FREQUENCY / (double)iFftSampleSize * (double)i;
					iLinePos = (int)dblCurPos;
				}
			}
			if (!bIsOverlay)
			{
				double dblOneRectWidth = (double)m_graphRect.Width() / (double)(iFFtUnit * iverticalNum);
				if (strTimeDomainUnit == "g-Force")
					strHzValue.Format(_T("(%.2fHz, %.3fg)"), dblCalcHz, calcValue.RealValue);
				if (strTimeDomainUnit == "GAL")
					strHzValue.Format(_T("(%.2fHz, %.3fGAL)"), dblCalcHz, calcValue.RealValue);

				if (dblFftInfoPosX + 80 > m_graphRect.right)
					m_pDC->SetTextAlign(TA_RIGHT);

				m_pDC->TextOutW(iLinePos, iLineLength, strHzValue);
				m_pDC->MoveTo(iLinePos, iLineLength);
				m_pDC->LineTo(iLinePos, m_graphRect.bottom);
			}
			else
			{
				double dblOneRectWidth = (double)m_graphRect.Width() / (double)(iFFtUnit * iverticalNum);
				if (nInputType == 1 && nPriority == 0)
				{
					if (strTimeDomainUnit == "g-Force")
						strHzValue.Format(_T("(Overlay : %.2fHz, %.3fg)"), dblCalcHz, calcValue.RealValue);
					if (strTimeDomainUnit == "GAL")
						strHzValue.Format(_T("(Overlay : %.2fHz, %.3fGAL)"), dblCalcHz, calcValue.RealValue);

					if (dblFftInfoPosX + 80 > m_graphRect.right)
						m_pDC->SetTextAlign(TA_RIGHT);
					m_pDC->TextOutW(iLinePos, iLineLength + 20, strHzValue);
				}
				if (nInputType == 0 && nPriority == 0)
				{
					if (strTimeDomainUnit == "g-Force")
						strHzValue.Format(_T("%.2fHz %.3fg"), dblCalcHz, calcValue.RealValue);
					if (strTimeDomainUnit == "GAL")
						strHzValue.Format(_T("(%.2fHz, %.3fGAL)"), dblCalcHz, calcValue.RealValue);

					if (dblFftInfoPosX + 80 > m_graphRect.right)
						m_pDC->SetTextAlign(TA_RIGHT);
					m_pDC->TextOutW(iLinePos, iLineLength, strHzValue);
					m_pDC->MoveTo(iLinePos, iLineLength);
					m_pDC->LineTo(iLinePos, m_graphRect.bottom);
				}
				if (nInputType == 1 && nPriority == 1)
				{
					if (strTimeDomainUnit == "g-Force")
						strHzValue.Format(_T("(%.2fHz, %.3fg)"), dblCalcHz, calcValue.RealValue);
					if (strTimeDomainUnit == "GAL")
						strHzValue.Format(_T("(%.2fHz, %.3fGAL)"), dblCalcHz, calcValue.RealValue);
			
					if (dblFftInfoPosX + 80 > m_graphRect.right)
						m_pDC->SetTextAlign(TA_RIGHT);
					m_pDC->TextOutW(iLinePos, iLineLength, strHzValue);
					m_pDC->MoveTo(iLinePos, iLineLength);
					m_pDC->LineTo(iLinePos, m_graphRect.bottom);
				}
			}
		}

		if (dblFFT[i] >= 0)
			newPt.Y = (int)(dblMaxPos - (double)dblRePosWidth + (double)m_iInterval);
		else
			newPt.Y = (int)(dblMaxPos + (double)dblRePosWidth + (double)m_iInterval);

		if (newPt.Y <= m_iInterval)
			newPt.Y = m_iInterval;

		if (i == nIndex)
		{
			newPt.Y = (int)(dblMaxPos + (double)m_iInterval);
		}
		else
		{
			if (newPt.X > m_graphRect.right)
			{
				newPt.X = m_graphRect.right;
				newPt.Y = m_graphRect.bottom;
				continue;
			}
			double dblHzValue = (double)FFT_SAMPLING_FREQUENCY / (double)iFftSampleSize  * (double)i;
			if (CStatus::Instance()->m_bIsFFTFullMode)
			{
				if (dblHzValue > 200)
				{
					delete[] dblFFT, iFFTHz;	
					m_pDC->SelectObject(oldPen);
					penDrawLine.DeleteObject();
					font.DeleteObject();
					return;
				}
			}
			if (dblHzValue > (int)CStatus::Instance()->m_DeviceFrequency / 2)
			{
				delete[] dblFFT, iFFTHz;	
				m_pDC->SelectObject(oldPen);
				penDrawLine.DeleteObject();
				font.DeleteObject();
				return;
			}
			m_pDC->MoveTo(m_prevPt.X, m_prevPt.Y - 1);//계산상 1pixel식 차이가 생김..
			m_pDC->LineTo(newPt.X, newPt.Y - 1);//계산상 1pixel식 차이가 생김..
		}
		m_prevPt = newPt;
	}

	m_pDC->SelectObject(oldPen);
	penDrawLine.DeleteObject();
	font.DeleteObject();
	delete[] dblFFT , iFFTHz;// , pDblCalPointX;
}

int CDrawingHelper::OnFft2d(int nInputType)
{
	int iNum, iLog2N, iFftSampleSize;
	Complex *Data;
	int nDataCount = CStatus::Instance()->m_iDataCount;
	tAVMDData NewData;
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;

	iNum = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	iFftSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	iLog2N = 0;
	if (nDataCount == 0)
		return 0;
	if (iFftSampleSize <= 0)
		return 0;
	
	int iDrawStartNum = nDataCount - iFftSampleSize;
	int iDrawEndNum = nDataCount;
	if (CStatus::Instance()->GetIsHoldCondition())
	{
		iDrawStartNum = CStatus::Instance()->m_iHoldIndex - iFftSampleSize;
		if (iDrawStartNum <= 0)
		{
			return -1;
		}
		iDrawEndNum = CStatus::Instance()->m_iHoldIndex;
	}

	if (iDrawStartNum <= 0)
		return -1;

	while (iNum >= 2)
	{
		iNum >>= 1;
		iLog2N++;
	}

	Data = new Complex[iFftSampleSize];

	if (m_pFFTGraphData[nInputType] != NULL)
	{
		delete[] m_pFFTGraphData[nInputType];
		m_pFFTGraphData[nInputType] = new Complex[iFftSampleSize];
	}
	else
		m_pFFTGraphData[nInputType] = new Complex[iFftSampleSize];

	int nIndex = 0;
	for (int i = iDrawStartNum; i < iDrawEndNum; i++)
	{
		CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
		if (m_GraphType == "X")
			Data[nIndex].Re = (double)NewData.iXValue;
		if (m_GraphType == "Y")
			Data[nIndex].Re = (double)NewData.iYValue;
		if (m_GraphType == "Z")
			Data[nIndex].Re = (double)NewData.iZValue;
		if (m_GraphType == "RMS")
			Data[nIndex].Re = CalcRMS(&NewData);
		
		Data[nIndex].Im = 0;

		nIndex++;
	}

	OnFft1d(Data, iFftSampleSize, iLog2N); // 1차원FFT

	for (int i = 0; i < iFftSampleSize; i++)
	{
		m_pFFTGraphData[nInputType][i] = Data[i];
	}		
	delete[] Data;
	return 0;
}

int CDrawingHelper::OnFilterFft2d(int nInputType)
{
	int iNum, iLog2N, iFftSampleSize;
	Complex *Data;
	int nDataCount = CStatus::Instance()->m_iDataCount;
	int nDrawCountNUm = CStatus::Instance()->m_nDrawPointNum;
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;

	iNum = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	iFftSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	iLog2N = 0;

	if (nDataCount - iFftSampleSize  <= 0)
		return -1;

	while (iNum >= 2)
	{
		iNum >>= 1;
		iLog2N++;
	}

	Data = new Complex[iFftSampleSize];
	
	if (m_pFFTGraphData[nInputType] != NULL)
	{
		delete[] m_pFFTGraphData[nInputType];
		m_pFFTGraphData[nInputType] = new Complex[iFftSampleSize];
	}
	else
		m_pFFTGraphData[nInputType] = new Complex[iFftSampleSize];
	
	for (int i = 0; i < iFftSampleSize; i++)
	{
		Data[i].Re = CStatus::Instance()->GetViewFilteredValue(m_GraphType, nInputType, i);
		Data[i].Im = 0;
	}
	GetFilterDataSize(m_GraphType);
	OnFft1d(Data, iFftSampleSize, iLog2N); // 1차원FFT
	//double test[10000];
	for (int i = 0; i < iFftSampleSize; i++)
	{
		m_pFFTGraphData[nInputType][i] = Data[i];
		//test[i] = Data[i].Re;
	}
	delete[] Data;
	return 0;
}

int CDrawingHelper::OnReviewFilterFft2d(int nInputType)
{
	int iNum, iLog2N, iFftSampleSize;
	Complex *Data;
	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
	int nDrawCountNUm = CStatus::Instance()->m_nDrawPointNum;
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;

	iNum = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	iFftSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	iLog2N = 0;

	if (nDataCount - iFftSampleSize <= 0)
		return -1;

	while (iNum >= 2)
	{
		iNum >>= 1;
		iLog2N++;
	}

	Data = new Complex[iFftSampleSize];

	if (m_pFFTGraphData[nInputType] != NULL)
	{
		delete[] m_pFFTGraphData[nInputType];
		m_pFFTGraphData[nInputType] = new Complex[iFftSampleSize];
	}
	else
		m_pFFTGraphData[nInputType] = new Complex[iFftSampleSize];

	int iDrawStartNum = 0;

	iDrawStartNum = CStatus::Instance()->GetReviewDrawStart(nInputType);

	if ((iDrawStartNum + iFftSampleSize) > nDataCount)
		return -1;

	int nIndex = 0;
	for (int i = iDrawStartNum; i < iDrawStartNum + iFftSampleSize; i++)
	{
		Data[nIndex].Re = CStatus::Instance()->GetViewFilteredValue(m_GraphType, nInputType, i);
		Data[nIndex].Im = 0;
		nIndex++;
	}

	OnFft1d(Data, iFftSampleSize, iLog2N); // 1차원FFT

	for (int i = 0; i < iFftSampleSize; i++)
	{
		m_pFFTGraphData[nInputType][i] = Data[i];
	}
	delete[] Data;
	return 0;
}

int CDrawingHelper::OnReviewFft2d(int nInputType)
{
	int iNum, iLog2N, iFftSampleSize;
	Complex *Data;
	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
	tAVMDData NewData;
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;

	iNum = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	iFftSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	iLog2N = 0;

	if (nDataCount == 0)
		return -1;
	if (iFftSampleSize <= 0)
		return -1;

	while (iNum >= 2)
	{
		iNum >>= 1;
		iLog2N++;
	}

	Data = new Complex[iFftSampleSize];
	if (m_pFFTGraphData[nInputType] != NULL)
	{
		delete[] m_pFFTGraphData[nInputType];
		m_pFFTGraphData[nInputType] = new Complex[iFftSampleSize];
	}
	else
		m_pFFTGraphData[nInputType] = new Complex[iFftSampleSize];

	int nIndex = 0;

	int iDrawStartNum = 0;
	
	iDrawStartNum = CStatus::Instance()->GetReviewDrawStart(nInputType);

	if ((iDrawStartNum + iFftSampleSize) > nDataCount)
		return -1;

	for (int i = iDrawStartNum; i < iDrawStartNum + iFftSampleSize; i++)
	{
		int iResult = CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
		if (iResult == -1)
			return -1;
		if (m_GraphType == "X")
			Data[nIndex].Re = (double)NewData.iXValue;
		if (m_GraphType == "Y")
			Data[nIndex].Re = (double)NewData.iYValue;
		if (m_GraphType == "Z")
			Data[nIndex].Re = (double)NewData.iZValue;
		if (m_GraphType == "RMS")
			Data[nIndex].Re = CalcRMS(&NewData);

		Data[nIndex].Im = 0;

		nIndex++;
	}

	OnFft1d(Data, iFftSampleSize, iLog2N); // 1차원FFT

	for (int i = 0; i < iFftSampleSize; i++)
		m_pFFTGraphData[nInputType][i] = Data[i];

	delete[] Data;
	return 0;
}

void CDrawingHelper::ViewFilter(int nInputType)
{
	int iLowValue = CStatus::Instance()->m_iFilterLowValue;
	int iHighValue = CStatus::Instance()->m_iFilterHighValue;
	SetDrawPointNum();

	double dblTimeUnit = CStatus::Instance()->m_dblUnit;		//ex 0.1	
	int nFrequeny = (int)(CStatus::Instance()->GetDeviceFrequency());
	double dblOneRectDrawNum = (double)nFrequeny * dblTimeUnit; // 가변 되야함 한칸당 몇개씩 그릴거냐
	int iDrawStartPos = 0, iDrawEndPos = 0;
	int iFftSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	int iDrawDataCount = CStatus::Instance()->m_nDrawPointNum;
	int nDataCount = CStatus::Instance()->m_iDataCount;
	int iOverlapCount = (int)(dblOneRectDrawNum / (double)ONERECT_PIXEL);

	m_iOverlapToTalCount = 0;
	m_dblOverlapToTalSum = 0;
	// 오른쪽부터 넣는다.
	if ((iDrawDataCount + iFftSampleSize)> nDataCount)
	{
		iDrawStartPos = 0;
		iDrawEndPos = nDataCount;
	}
	else
	{
		int nIndex = 0;
		if(iDrawDataCount < iFftSampleSize)
			iDrawStartPos = nDataCount - iFftSampleSize;
		else
			iDrawStartPos = nDataCount - (iDrawDataCount);// 기존
		
		if (iOverlapCount != 0)
		{
			if (iDrawStartPos % iOverlapCount != 0)
			{
				while (iDrawStartPos % iOverlapCount != 0)
				{
					iDrawStartPos -= 1;
					nIndex++;
				}
			}
		}
		iDrawEndPos = nDataCount - nIndex;
	}
	if (!CStatus::Instance()->GetIsHoldCondition())
	{
		m_iHoldStart = iDrawStartPos;
		m_iHoldEnd = iDrawEndPos;
		CStatus::Instance()->m_iHoldIndex = m_iHoldEnd;
	}
	else
	{
		iDrawStartPos = m_iHoldStart;
		iDrawEndPos = CStatus::Instance()->m_iHoldIndex;
	}

	CFilterIIR iiR;
	tAVMDData NewData;
	if (m_pdblFilteredData[nInputType] != NULL)
		delete[] m_pdblFilteredData[nInputType];

	int nArraySize = iDrawEndPos - iDrawStartPos;



	//int nArraySize = iDrawEndPos - iFftSampleSize;
	m_pdblFilteredData[nInputType] = new double[nArraySize];
	double* dblOrgData = new double[nArraySize];
	printf("ArrayCount : %d\n", nArraySize);
	CStatus::Instance()->m_nFilterArrayEndIndex = nArraySize;
	int nIndex = nArraySize - 1;
	if (m_GraphType == "RMS")
	{
		for (int i = iDrawStartPos; i <iDrawEndPos; i++)
		{
			CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
			CalcRMS(&NewData);
			dblOrgData[nIndex] = (double)NewData.iRMSValue;
			nIndex--;
		}
	}
	if (m_GraphType == "X")
	{
		for (int i = iDrawStartPos; i <iDrawEndPos; i++)
		{
			CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
			dblOrgData[nIndex] = (double)NewData.iXValue;
			nIndex--;
		}
	}
	if (m_GraphType == "Y")
	{
		for (int i = iDrawStartPos; i <iDrawEndPos; i++)
		{
			CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
			dblOrgData[nIndex] = (double)NewData.iYValue;
			nIndex--;
		}
	}
	if (m_GraphType == "Z")
	{
		for (int i = iDrawStartPos; i <iDrawEndPos; i++)
		{
			CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
			dblOrgData[nIndex] = (double)NewData.iZValue;
			nIndex--;
		}
	}

	if (CStatus::Instance()->m_bIsLowPassFilter)
	{
		if (iiR.Init(nFrequeny, iHighValue, CFilterIIR::FT_LOWPASS, 6))AfxMessageBox(_T("Err"));
		iiR.DoFilter(dblOrgData, m_pdblFilteredData[nInputType], nArraySize);
		//printf("ArraySize : %d\n", nArraySize);
	}
	if (CStatus::Instance()->m_bIsHighPassFilter)
	{
		if (iiR.Init(nFrequeny, iLowValue, CFilterIIR::FT_HIGHPASS, 6))AfxMessageBox(_T("Err"));
		iiR.DoFilter(dblOrgData, m_pdblFilteredData[nInputType], nArraySize);
	}
	if (CStatus::Instance()->m_bIsBandPassFilter)
	{
		if (iiR.Init(nFrequeny, iLowValue, CFilterIIR::FT_HIGHPASS, 6))AfxMessageBox(_T("Err"));
		iiR.DoFilter(dblOrgData, m_pdblFilteredData[nInputType], nArraySize);
		iiR.Free();
		if (iiR.Init(nFrequeny, iHighValue, CFilterIIR::FT_LOWPASS, 6))AfxMessageBox(_T("Err"));
		iiR.DoFilterInThePlace(m_pdblFilteredData[nInputType], nArraySize);
	}
	iiR.Free();
	if (iDrawDataCount < iFftSampleSize)
		nArraySize = iDrawDataCount;
	DrawFilteredGraph(nInputType, iDrawEndPos, nArraySize);
	delete[] dblOrgData;
}

#pragma region FFT 관련 <Private>
void CDrawingHelper::OnShuffle(Complex *X, int N, int Log2N)
{
	Complex *temp;
	temp = new Complex[N];

	for (int i = 0; i < N; i++)
	{
		temp[i].Re = X[OnReverseBitOrder(i, Log2N)].Re;
		temp[i].Im = X[OnReverseBitOrder(i, Log2N)].Im;
	}
	for (int i = 0; i<N; i++)
	{
		X[i].Re = temp[i].Re;
		X[i].Im = temp[i].Im;
	}
	delete[] temp;
}

void CDrawingHelper::OnButterfly(Complex *X, int N, int Log2N, int mode)
{
	// 나비(Butterfly) 구조를위한함수
	int start;
	double Value;
	int m;

	Complex *Y, temp;

	Y = new Complex[N / 2];

	for (int i = 0; i<Log2N; i++)
	{
		Value = pow(2, i + 1);
		if (mode == 1)
		{
			for (int j = 0; j<(int)(Value / 2); j++)
			{
				Y[j].Re = cos(j*2.0*M_PI / Value);
				Y[j].Im = -sin(j*2.0*M_PI / Value);
			}
		}
		if (mode == 2)
		{
			for (int j = 0; j<(int)(Value / 2); j++)
			{
				Y[j].Re = cos(j*2.0*M_PI / Value);
				Y[j].Im = sin(j*2.0*M_PI / Value);
			}
		}
		start = 0;
		for (int k = 0; k<N / (int)Value; k++)
		{
			for (int j = start; j<start + (int)(Value / 2); j++)
			{
				m = j + (int)(Value / 2);
				temp.Re = Y[j - start].Re * X[m].Re
					- Y[j - start].Im * X[m].Im;
				temp.Im = Y[j - start].Im * X[m].Re
					+ Y[j - start].Re * X[m].Im;
				X[m].Re = X[j].Re - temp.Re;
				X[m].Im = X[j].Im - temp.Im;
				X[j].Re = X[j].Re + temp.Re;
				X[j].Im = X[j].Im + temp.Im;
			}
			start = start + (int)Value;
		}
	}
	if (mode == 2)
	{
		for (int i = 0; i<N; i++)
		{
			X[i].Re = X[i].Re / N;
			X[i].Im = X[i].Im / N;
		}
	}
	delete[] Y;
}

int CDrawingHelper::OnReverseBitOrder(int index, int Log2N)
{
	int i, X, Y;
	Y = 0;
	for (i = 0; i<Log2N; i++)
	{
		X = (index & (1 << i)) >> i;
		Y = (Y << 1) | X;
	}
	return Y;
}

void CDrawingHelper::OnFft1d(Complex *X, int N, int Log2N)
{
	// 1차원fft를위한함수
	OnShuffle(X, N, Log2N); // 함수호출
	OnButterfly(X, N, Log2N, 1); // 함수호출
}

void CDrawingHelper::OnIfft2d(int nRepeatNum, int nInputType)
{
	int iNum = 0, iFftSampleSize=0;
	iNum = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	iFftSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	int Log2N=0;
	Complex *Data;
	
	while (iNum >= 2) // 주파수변환된영상의너비계산
	{
		iNum >>= 1;
		Log2N++;
	}

	Data = new Complex[iFftSampleSize];
	if (m_pIFFTData[nInputType] != NULL)
	{
		delete[] m_pIFFTData[nInputType];
		m_pIFFTData[nInputType] = new Complex[FFT_MAXCOUNT];// 역변환된영상을위한배열
	}
	else
		m_pIFFTData[nInputType] = new Complex[FFT_MAXCOUNT];

	for (int j = 0; j<iFftSampleSize; j++)
	{
		Data[j].Re = m_pFFTData[nInputType][j].Re;
		Data[j].Im = m_pFFTData[nInputType][j].Im;
	}

	OnIfft1d(Data, iFftSampleSize, Log2N); // 1차원IFFT

	for (int i = 0; i < iFftSampleSize; i++)
	{
		m_pIFFTData[nInputType][i].Re = Data[i].Re;
	}
	delete[] Data;
}

void CDrawingHelper::OnIfft1d(Complex *X, int N, int Log2N)
{
	OnShuffle(X, N, Log2N);
	OnButterfly(X, N, Log2N, 2);
}
#pragma endregion

void CDrawingHelper::OnFilterFft2d(int iTotalLog2N, int nDataCount, int iStartPos, int nInputType)
{
	int iNum = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	//int iFftSampleSize =_ttoi(CStatus::Instance()->m_FFT_SampleSize);
	int iFftSampleSize = nDataCount - iStartPos;
	int iDrawNum = CStatus::Instance()->m_nDrawPointNum;
	int iLog2N = 0;
	Complex *Data;
	tAVMDData NewData;
	int iFFTRepeatCount = (iDrawNum / iFftSampleSize);
	int iFFTStartNum = 0;
	int iIntervalLength = iFftSampleSize / 2;
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
	{
		//iFFTStartNum = iStartPos + ((iFftSampleSize / 2) * iRepeatNum);
	}

	if (nDataCount == 0 || iFftSampleSize >= nDataCount)
		return;

	while (iNum >= 2)
	{
		iNum >>= 1;
		iLog2N++;
	}

	Data = new Complex[iFftSampleSize];

	if (m_pFFTData[nInputType] != NULL)
	{
		delete[] m_pFFTData[nInputType];
		m_pFFTData[nInputType] = new Complex[iFftSampleSize];
	}
	else
	{
		m_pFFTData[nInputType] = new Complex[iFftSampleSize];
	}

	
	int nIndex = 0;

	
	for (int i = iFFTStartNum; i <iFFTStartNum + iFftSampleSize ; i++)
	{
		CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
		if (m_GraphType == "X")
			Data[nIndex].Re = (double)NewData.iXValue;
		if (m_GraphType == "Y")
			Data[nIndex].Re = (double)NewData.iYValue;
		if (m_GraphType == "Z")
			Data[nIndex].Re = (double)NewData.iZValue;
		if (m_GraphType == "RMS")
			Data[nIndex].Re = CalcRMS(&NewData);

		Data[nIndex].Im = 0;

		nIndex++;
	}

	OnFft1d(Data, iFftSampleSize, iLog2N); // 1차원FFT
	for (int i = 0; i < iFftSampleSize; i++)
		m_pFFTData[nInputType][i] = Data[i];
	
	delete[] Data;
}

void CDrawingHelper::DrawFilteredGraph(int nInputType, int nDataEnd, int nFilterSize)	// View 일때
{
	tCalcValue calcValue;
	Point newPt(0, 0);
	int nTempIndex = 0;
	double dblMaxPos = m_graphRect.Height() / 2;
	double dblRePosWidth = 0;
	double dblValue = 0;

	if (m_pdblFilteredData == NULL)
		return;
#pragma region X좌표 구하기(iRePosX)
	double dblTimeUnit = CStatus::Instance()->m_dblUnit;		//ex 0.1	
	int nFrequeny = (int)(CStatus::Instance()->GetDeviceFrequency());
	double dblOneRectDrawNum = (double)nFrequeny * dblTimeUnit; // 가변 되야함 한칸당 몇개씩 그릴거냐
	m_dblOneRectInterval = m_graphRect.Height() / CStatus::Instance()->m_ihorizontalNum;// 한칸당 높이
	CStatus::Instance()->m_nDrawPointNum = (int)(dblOneRectDrawNum * ((double)m_graphRect.Width() / (double)ONERECT_PIXEL));
	double dblGraphPosWidth = (double)ONERECT_PIXEL / dblOneRectDrawNum;
#pragma endregion
	CPen penDrawLine;
	if (m_GraphType == "RMS")
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_RMSPenColor);
	if (m_GraphType == "X")
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_XPenColor);
	if (m_GraphType == "Y")
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_YPenColor);
	if (m_GraphType == "Z")
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_ZPenColor);

	CPen* oldPen = m_pDC->SelectObject(&penDrawLine);

	DrawingString(m_GraphType, nInputType);
	DrawIndicator();
	m_iTempTriggerEnd = nDataEnd;
	InspectFilterTrigger(m_iTempTriggerEnd, nInputType);
	InspectFilterGoNoGo(m_iTempTriggerEnd, nInputType);
	int nDrawDataCount = CStatus::Instance()->m_nDrawPointNum;

	double dblMaxValue = dblMaxPos, dblMinValue = dblMaxPos;
	int nPlusCount = 0, nMinusCount = 0;
	int nIndex = 0, nTempOverlapFirstValue = 0;
	int iScrollMovePixel = 0;
	int iOverlapCount = (int)(dblOneRectDrawNum / (double)ONERECT_PIXEL);
	double* pdblPosRepository = new double[m_graphRect.bottom];
	memset(pdblPosRepository, 0, m_graphRect.bottom * sizeof(double));
	//nIndex = nFilterSize;
	nIndex = nDrawDataCount;
	
	m_pOverlayTempRepository = new double[m_graphRect.bottom];
	
	for(int i = 0; i< nFilterSize; i++)
	{
		double	dblFilteredValue = m_pdblFilteredData[nInputType][i];

		CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;
#pragma region Y좌표 구하기(iValue)
		int iScale = _ttoi(CStatus::Instance()->m_strGraphScale);

		calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblFilteredValue, strTimeDomainUnit);
		dblRePosWidth = abs(calcValue.AdjustedValue);

		if (nInputType == 1)
		{
			m_nStepMove = CStatus::Instance()->m_iCompareStandardValue;
		}
		if (nInputType == 0)
		{
			if (m_GraphType == "RMS")
			{
				m_nStepMove = CStatus::Instance()->m_iRMSStandardValue;
				if (i == 0)
				{
					DrawingMonitoringValue(i, calcValue.RealValue);
					if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
					{
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, FALSE);
						AlarmRectColor(bIsSpecOut, CStatus::Evt_TRIGGER);
					}
					if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
					{
						if (m_strGoNoGoGraphType == "RMS")
						{
							AlarmRectColor(TRUE, CStatus::Evt_GONOGO);
						}
						else
						{
							BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, FALSE);
							AlarmRectColor(bIsSpecOut, CStatus::Evt_GONOGO);
						}
					}
				}
			}
			if (m_GraphType == 'X')
			{
				m_nStepMove = CStatus::Instance()->m_iXStandardValue;
				if (i == 0)
				{
					DrawingMonitoringValue(i, calcValue.RealValue);
					if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
					{
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, FALSE);
						AlarmRectColor(bIsSpecOut, CStatus::Evt_TRIGGER);
					}
					if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
					{
						if (m_strGoNoGoGraphType == "X")
						{
							AlarmRectColor(TRUE, CStatus::Evt_GONOGO);
						}
						else
						{
							BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, FALSE);
							AlarmRectColor(bIsSpecOut, CStatus::Evt_GONOGO);
						}
					}
				}
			}
			if (m_GraphType == 'Y')
			{
				m_nStepMove = CStatus::Instance()->m_iYStandardValue;
				if (i == 0)
				{
					DrawingMonitoringValue(i, calcValue.RealValue);
					if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
					{
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, FALSE);
						AlarmRectColor(bIsSpecOut, CStatus::Evt_TRIGGER);
					}
					if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
					{
						if (m_strGoNoGoGraphType == "Y")
						{
							AlarmRectColor(TRUE, CStatus::Evt_GONOGO);
						}
						else
						{
							BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, FALSE);
							AlarmRectColor(bIsSpecOut, CStatus::Evt_GONOGO);
						}
					}
				}
			}
			if (m_GraphType == 'Z')
			{
				m_nStepMove = CStatus::Instance()->m_iZStandardValue;
				if (i == 0)
				{
					DrawingMonitoringValue(i, calcValue.RealValue);
					if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
					{
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, FALSE);
						AlarmRectColor(bIsSpecOut, CStatus::Evt_TRIGGER);
					}
					if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
					{
						if (m_strGoNoGoGraphType == "Z")
						{
							AlarmRectColor(TRUE, CStatus::Evt_GONOGO);
						}
						else
						{
							BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, FALSE);
							AlarmRectColor(bIsSpecOut, CStatus::Evt_GONOGO);
						}
					}
				}
			}
				
		}
		if (m_GraphType == "RMS")
		{
			if (dblFilteredValue >= 0)
				dblValue = (dblMaxPos*2. - (dblRePosWidth * (double)iScale) + m_iInterval);
			else
				dblValue = (dblMaxPos*2. + (dblRePosWidth * (double)iScale) + m_iInterval);
		}
		else
		{
			if (dblFilteredValue >= 0)
				dblValue = (dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval);
			else
				dblValue = (dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval);
		}

		iScrollMovePixel = (int)Scroll_CalcValue(strTimeDomainUnit);
		dblValue += Scroll_CalcValue(strTimeDomainUnit);


#pragma endregion
		if (iOverlapCount == 1 || iOverlapCount == 0) // overlap이 없는 경우
		{												// overlap = 0 경우 => 주기가 500인 경우 TimeDomain : 0.1 
														// Overlap = 1 경우 => 주기가 1000인 경우 TimeDomain : 0.1
			m_iOverlapToTalCount = 0;
			if (nTempIndex == 0)		// for문에 총 1번들어옴
			{
				int iRePosX = m_graphRect.left + (int)((double)(nIndex)* dblGraphPosWidth);
				if (iRePosX > m_graphRect.right || iRePosX < m_graphRect.left)
				{
					return;
				}
				if (dblValue > m_graphRect.bottom)
					dblValue = m_graphRect.bottom;
				if (dblValue < m_graphRect.top)
					dblValue = m_graphRect.top;
				m_prevPt.X = iRePosX;
				m_prevPt.Y = (int)dblValue;
				nTempIndex++;
				nIndex--;
				continue;
			}
			else
			{
				int iRePosX = m_graphRect.left + (int)((double)nIndex* dblGraphPosWidth);
				//printf("%d, nIndex :%d\n", iRePosX, nIndex);
				if (iRePosX > m_graphRect.right || iRePosX < m_graphRect.left)
				{
					return;
				}
				newPt.X = iRePosX;
				newPt.Y = (int)dblValue;
				if (newPt.Y <= m_iInterval)
					newPt.Y = m_iInterval;
				if (newPt.Y >= m_iInterval + m_graphRect.Height())
					newPt.Y = m_iInterval + m_graphRect.Height();
				m_pDC->MoveTo(m_prevPt.X, m_prevPt.Y);
				m_pDC->LineTo(newPt.X, newPt.Y);

				m_prevPt = newPt;
				nIndex--;
			}
		}
		else
		{
			if (nTempOverlapFirstValue == 0)
			{
				if (dblValue > m_graphRect.bottom)
					dblValue = m_graphRect.bottom;
				if (dblValue < m_graphRect.top)
					dblValue = m_graphRect.top;

				dblMaxValue = dblValue;
				dblMinValue = dblValue;
				nTempOverlapFirstValue++;
			}
			m_iOverlapToTalCount++;
			int iRePosX = m_graphRect.left + (int)((double)(nIndex)* dblGraphPosWidth);

			if (dblValue > m_graphRect.bottom)
				dblValue = m_graphRect.bottom;
			if (dblValue < m_graphRect.top)
				dblValue = m_graphRect.top;

			int nRepositoryIndex = (int)dblValue;
			int nStandardIndex = 0;
			if (m_GraphType == "RMS")
				nStandardIndex = (int)(dblMaxPos * 2. + (double)iScrollMovePixel);
			else
				nStandardIndex = (int)(dblMaxPos + (double)iScrollMovePixel);
			if (nStandardIndex < 0)
				nStandardIndex = 0;

			if (nStandardIndex > nRepositoryIndex)
			{
				for (int i = nRepositoryIndex; i < nStandardIndex; i++)
				{
					if (i< m_graphRect.bottom)
						pdblPosRepository[i]++;
				}
			}
			else
			{
				for (int i = nStandardIndex; i <= nRepositoryIndex; i++)
				{
					if (i< m_graphRect.bottom)
						pdblPosRepository[i]++;
				}
			}
			if (dblMaxValue < dblValue)
				dblMaxValue = dblValue;
			if (dblMinValue > dblValue)
				dblMinValue = dblValue;

			if (m_iOverlapToTalCount == iOverlapCount)
			{
				//걸리내기
				//printf("Max : %.f, Min : %.f \n", dblMaxValue, dblMinValue);
				int nMinIndex = (int)dblMinValue;
				int nMaxIndex = (int)dblMaxValue + 1;

				for (int i = 0; i < nMinIndex; i++)
					pdblPosRepository[i] = 0;

				for (int i = nMaxIndex; i < m_graphRect.bottom; i++)
					pdblPosRepository[i] = 0;

				double dblUnit = CStatus::Instance()->m_dblUnit;
				int iCutOffValue = CalcCutOffValue(dblUnit);

				tCalcInfo tMaxMinInfo = CalcOverlayTendencyDelete(pdblPosRepository, iCutOffValue, nMinIndex, nMaxIndex);
				//deque에 넣기

				memcpy(m_pOverlayTempRepository, pdblPosRepository, m_graphRect.bottom * sizeof(double));

				tOverlayInfo tInfo;
				if (tMaxMinInfo.nMinValue > m_graphRect.Height() + m_iInterval)
					tMaxMinInfo.nMinValue = m_graphRect.Height() + m_iInterval;
				if (tMaxMinInfo.nMaxValue < m_iInterval)
					tMaxMinInfo.nMaxValue = m_iInterval;

				tInfo.nMaxIndex = tMaxMinInfo.nMaxValue;
				tInfo.nMinIndex = tMaxMinInfo.nMinValue;
				tInfo.pOverlayRepository = m_pOverlayTempRepository;

				m_pOverlayRepository.push_back(tInfo);
				int size = m_pOverlayRepository.size();
				m_iOverlapToTalCount = 0;
				dblMaxValue = 0; dblMinValue = 0;
				nTempOverlapFirstValue = 0;
				nIndex--;
			}
			else
			{
				nIndex--;
			}
		}
	}
	//printf("OverlayCount : %d\n", iOverlapCount);
	m_pDC->SelectObject(oldPen);
	penDrawLine.DeleteObject();
	if (iOverlapCount > 1)
	{
		DrawViewFilterOverlayGraph(FALSE);
	}
	delete[] pdblPosRepository;
	m_TempPt = newPt;
	return;
}

void CDrawingHelper::DrawReviewFilteredGraph(int nInputType, int nPriority, BOOL bIsOverColor, BOOL bIsDrawString)
{
	//LONG lStartTime = ::GetTickCount();
	tCalcValue calcValue;
	int iSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	int nIndex = 0;

	Point newPt(0, 0);
	int nTempIndex = 0;
	double dblMaxPos = m_graphRect.Height() / 2;
	double dblRePosWidth = 0;
	double dblValue = 0;
	int nCount = 0;
	

	nCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);

	if (nCount == 0)
		return;
	if (bIsDrawString)
	{
		DrawingString(m_GraphType, nInputType);
		DrawIndicator();
	}
	Pen graphPen(Color(255, 255, 0, 0), 1.0f);
	CPen penDrawLine;
	if (bIsOverColor)
	{
		penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_OverlayColor);
	}
	else
	{
		if (m_GraphType == "RMS")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_RMSPenColor);
		if (m_GraphType == "X")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_XPenColor);
		if (m_GraphType == "Y")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_YPenColor);
		if (m_GraphType == "Z")
			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_ZPenColor);
	}

	CPen* oldPen = m_pDC->SelectObject(&penDrawLine);

#pragma region X좌표 구하기(iRePosX)
	double dblTimeUnit = CStatus::Instance()->m_dblUnit;		//ex 0.1	
	int nFrequeny = (int)(CStatus::Instance()->GetDeviceFrequency());
	double dblOneRectDrawNum = (double)nFrequeny * dblTimeUnit; // 가변 되야함 한칸당 몇개씩 그릴거냐
	m_dblOneRectInterval = m_graphRect.Height() / CStatus::Instance()->m_ihorizontalNum;// 한칸당 높이
	CStatus::Instance()->m_nDrawPointNum = (int)(dblOneRectDrawNum * ((double)m_graphRect.Width() / (double)ONERECT_PIXEL));
	double dblGraphPosWidth = (double)ONERECT_PIXEL / dblOneRectDrawNum;
	double dblMoveAmountofOnePixel = (double)CStatus::Instance()->m_nDrawPointNum / (double)m_graphRect.Width();
	CStatus::Instance()->SetOnePixelDrawCount(dblOneRectDrawNum / ONERECT_PIXEL);
#pragma endregion
	int iDrawStartNum = 0;
	int iDrawEndNum = 0;

	if (nInputType == 0)
	{
		if (CStatus::Instance()->m_bIsTimeScaleChange == TRUE)// && m_GraphType == _T("X"))
		{
			if (!CStatus::Instance()->m_bIsPlayBack)
			{
				double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

				int iNewPos = CStatus::Instance()->m_iStartPosText;
				CStatus::Instance()->SetMarkEditStart(iNewPos, 0);
				CStatus::Instance()->SetMarkClickEvent(TRUE, 0);
				double dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;
				CStatus::Instance()->SetDragAmount(dblNewDragNum, 0);

				CStatus::Instance()->m_bIsTimeScaleChange = FALSE;
			}
			else
			{
				if (m_dblPrevTimeUnit != dblTimeUnit)
				{
					double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

					int iNewPos = CStatus::Instance()->m_iStartPosText;
					CStatus::Instance()->SetMarkEditStart(iNewPos, 0);
					CStatus::Instance()->SetMarkClickEvent(TRUE, 0);
					double dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;
					CStatus::Instance()->SetDragAmount(dblNewDragNum, 0);

					CStatus::Instance()->m_bIsTimeScaleChange = FALSE;
				}
			}
		}
	}
	else
	{
		if (CStatus::Instance()->m_bIsCompareTimeScaleChange == TRUE)// && m_GraphType == _T("X"))
		{
			if (!CStatus::Instance()->m_bIsPlayBack)
			{
				double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

				int iNewPos = CStatus::Instance()->m_iCompareStartPosText;
				CStatus::Instance()->SetMarkEditStart(iNewPos, 1);
				CStatus::Instance()->SetMarkClickEvent(TRUE, 1);
				double dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;
				CStatus::Instance()->SetDragAmount(dblNewDragNum, 1);

				CStatus::Instance()->m_bIsCompareTimeScaleChange = FALSE;
			}
			else
			{
				double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

				int iNewPos = CStatus::Instance()->m_iCompareStartPosText;
				CStatus::Instance()->SetMarkEditStart(iNewPos, 1);
				CStatus::Instance()->SetMarkClickEvent(TRUE, 1);
				double dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;
				CStatus::Instance()->SetDragAmount(dblNewDragNum, 1);

				CStatus::Instance()->m_bIsCompareTimeScaleChange = FALSE;
			}
		}
	}
	if (CStatus::Instance()->GetMarkClickEvent(nInputType) == TRUE)
	{
		iDrawStartNum = CStatus::Instance()->GetMarkEditStart(nInputType);
		iDrawEndNum = iDrawStartNum + CStatus::Instance()->m_nDrawPointNum;
	}
	else
	{
		iDrawStartNum = CStatus::Instance()->GetReviewDrawStart(nInputType);
		iDrawEndNum = CStatus::Instance()->GetReviewDrawEnd(nInputType);
	}

	

	if (nInputType == 0)
	{
		if (iDrawEndNum >= CStatus::Instance()->GetRepository()->GetDataCount(0))
		{
			iDrawEndNum = CStatus::Instance()->GetRepository()->GetDataCount(0);
		}
	}
	if (nInputType == 1)
	{
		if (iDrawEndNum >= CStatus::Instance()->GetRepository()->GetDataCount(1))
		{
			iDrawEndNum = CStatus::Instance()->GetRepository()->GetDataCount(1);
		}
	}
	int iOverlapCount = (int)(dblOneRectDrawNum / (double)ONERECT_PIXEL);
	if (iOverlapCount != 0)
	{
		/*if (iDrawEndNum % iOverlapCount != 0)
		{
			while (iDrawEndNum % iOverlapCount != 0)
			{
				iDrawEndNum -= 1;
			}
		}*/
		if (iDrawStartNum % iOverlapCount != 0)
		{
			while (iDrawStartNum % iOverlapCount != 0)
			{
				iDrawStartNum -= 1;
			}
		}
	}
	
	//
	CalcUnitPt(dblGraphPosWidth, iDrawStartNum, iDrawEndNum, nInputType, nPriority);
	CalcMarkPt(dblGraphPosWidth, iDrawStartNum, iDrawEndNum, nInputType, nPriority);

	m_iOverlapToTalCount = 0;
	m_dblOverlapToTalSum = 0;
	double dblMaxValue = 0, dblMinValue = 0;
	int nPlusCount = 0, nMinusCount = 0 , nTempOverlapFirstValue =0;
	int iScrollMovePixel = 0;

	double* pdblPosRepository = new double[m_graphRect.bottom];
	memset(pdblPosRepository, 0, m_graphRect.bottom * sizeof(double));
	if (iOverlapCount > 1)
		m_pOverlayTempRepository = new double[m_graphRect.bottom];

	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
	if (iDrawEndNum > nDataCount)
		iDrawEndNum = nDataCount;
	
	for (int i = iDrawStartNum; i< iDrawEndNum; i++)
	{
		double dblFiltedValue = 0;
		if(nInputType == 0)
			dblFiltedValue = m_pdblFilteredData[nInputType][i];
		if (nInputType == 1)
		{
			if (m_GraphType == "RMS")
			{
				dblFiltedValue = m_pdblFilterCompareData[0][i];
			}
			if (m_GraphType == "X")
			{
				dblFiltedValue = m_pdblFilterCompareData[1][i];
			}
				
			if (m_GraphType == "Y")
			{
				dblFiltedValue = m_pdblFilterCompareData[2][i];
			}
			if (m_GraphType == "Z")
			{
				dblFiltedValue = m_pdblFilterCompareData[3][i];
			}
		}
		CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;
#pragma region Y좌표 구하기(iValue)
		int iScale = _ttoi(CStatus::Instance()->m_strGraphScale);

		calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblFiltedValue, strTimeDomainUnit);

		if (bIsDrawString)
		{
			if (i == iDrawStartNum)
				DrawingMonitoringValue(i, calcValue.RealValue);
		}
		dblRePosWidth = abs(calcValue.AdjustedValue);

		if (nInputType == 1 && nPriority == 1)
			m_nStepMove = CStatus::Instance()->m_iCompareStandardValue;
		else
		{
			if(m_GraphType =="RMS")
				m_nStepMove = CStatus::Instance()->m_iRMSStandardValue;
			if (m_GraphType == 'X')
				m_nStepMove = CStatus::Instance()->m_iXStandardValue;
			if (m_GraphType == 'Y')
				m_nStepMove = CStatus::Instance()->m_iYStandardValue;
			if (m_GraphType == 'Z')
				m_nStepMove = CStatus::Instance()->m_iZStandardValue;
		}
			
		if (m_GraphType == "RMS")
		{
			if (dblFiltedValue >= 0)
				dblValue = (dblMaxPos*2. - (dblRePosWidth * (double)iScale) + m_iInterval);
			else
				dblValue = (dblMaxPos*2. + (dblRePosWidth * (double)iScale) + m_iInterval);
		}
		else
		{
			if (dblFiltedValue >= 0)
				dblValue = (dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval);
			else
				dblValue = (dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval);
		}

		iScrollMovePixel = (int)Scroll_CalcValue(strTimeDomainUnit);
		dblValue += Scroll_CalcValue(strTimeDomainUnit);
#pragma endregion
		int iRePosX = 0;
#pragma region 시작 부분과 끝부분 이어 주기
		if (iOverlapCount == 1 || iOverlapCount == 0) // overlap이 없는 경우
		{												// overlap = 0 경우 => 주기가 500인 경우 TimeDomain : 0.1 
														// Overlap = 1 경우 => 주기가 1000인 경우 TimeDomain : 0.1
			m_iOverlapToTalCount = 0;
			if (nTempIndex == 0)		// for문에 총 1번들어옴
			{
				int iRePosX = m_graphRect.left + (int)((double)(nIndex)* dblGraphPosWidth);
				if (dblValue > m_graphRect.bottom)
					dblValue = m_graphRect.bottom;
				if (dblValue < m_graphRect.top)
					dblValue = m_graphRect.top;
				m_prevPt.X = iRePosX;
				m_prevPt.Y = (int)dblValue;
				nTempIndex++;
				nIndex++;
				continue;
			}
			else
			{
				int iRePosX = m_graphRect.left + (int)((double)(nIndex)* dblGraphPosWidth);
				newPt.X = iRePosX;
				newPt.Y = (int)dblValue;
				if (newPt.Y <= m_iInterval)
					newPt.Y = m_iInterval;
				if (newPt.Y >= m_iInterval + m_graphRect.Height())
					newPt.Y = m_iInterval + m_graphRect.Height();

				m_pDC->MoveTo(m_prevPt.X, m_prevPt.Y);
				m_pDC->LineTo(newPt.X, newPt.Y);

				m_prevPt = newPt;
				nIndex++;
			}
		}
		else
		{
			if (nTempOverlapFirstValue == 0)
			{
				if (dblValue > m_graphRect.bottom)
					dblValue = m_graphRect.bottom;
				if (dblValue < m_graphRect.top)
					dblValue = m_graphRect.top;

				dblMaxValue = dblValue;
				dblMinValue = dblValue;

				nTempOverlapFirstValue++;
			}
			m_iOverlapToTalCount++;

			int iRePosX = m_graphRect.left + (int)((double)(nIndex)* dblGraphPosWidth);

			if (dblValue > m_graphRect.bottom)
				dblValue = m_graphRect.bottom;
			if (dblValue < m_graphRect.top)
				dblValue = m_graphRect.top;

			int nRepositoryIndex = (int)dblValue;
			int nStandardIndex = 0;
			if (m_GraphType == "RMS")
				nStandardIndex = (int)(dblMaxPos * 2. + (double)iScrollMovePixel);
			else
				nStandardIndex = (int)(dblMaxPos + (double)iScrollMovePixel);
			if (nStandardIndex < 0)
				nStandardIndex = 0;

			if (nStandardIndex > nRepositoryIndex)
			{
				for (int i = nRepositoryIndex; i < nStandardIndex; i++)
					pdblPosRepository[i]++;
			}
			else
			{
				for (int i = nStandardIndex; i <= nRepositoryIndex; i++)
					pdblPosRepository[i]++;
			}
			if (dblMaxValue < dblValue)
				dblMaxValue = dblValue;
			if (dblMinValue > dblValue)
				dblMinValue = dblValue;

			if (m_iOverlapToTalCount == iOverlapCount)
			{
				//걸리내기
				int nMinIndex = (int)dblMinValue;
				int nMaxIndex = (int)dblMaxValue + 1;

				for (int i = 0; i < nMinIndex; i++)
					pdblPosRepository[i] = 0;

				for (int i = nMaxIndex; i < m_graphRect.bottom; i++)
					pdblPosRepository[i] = 0;

				double dblUnit = CStatus::Instance()->m_dblUnit;
				int iCutOffValue = CalcCutOffValue(dblUnit);

				tCalcInfo tMaxMinInfo = CalcOverlayTendencyDelete(pdblPosRepository, iCutOffValue, nMinIndex, nMaxIndex);
				//deque에 넣기

				memcpy(m_pOverlayTempRepository, pdblPosRepository, m_graphRect.bottom * sizeof(double));

				tOverlayInfo tInfo;
				tInfo.nMaxIndex = tMaxMinInfo.nMaxValue;
				tInfo.nMinIndex = tMaxMinInfo.nMinValue;
				tInfo.pOverlayRepository = m_pOverlayTempRepository;

				m_pOverlayRepository.push_back(tInfo);
				int size = m_pOverlayRepository.size();
				m_iOverlapToTalCount = 0;
				dblMaxValue = 0; dblMinValue = 0;
				nTempOverlapFirstValue = 0;
				nIndex++;
				//그리기
			}
			else
			{
				nIndex++;
			}
		}
		m_dblPrevTimeUnit = dblTimeUnit;
	}

	m_pDC->SelectObject(oldPen);
	penDrawLine.DeleteObject();

	m_TempPt = newPt;
	if (iOverlapCount > 1)
	{
		DrawOverlayGraph(bIsOverColor);
	}


	//LONG lEndTime = ::GetTickCount();
	//printf("%d\n", lEndTime - lStartTime);
	return;
}

BOOL CDrawingHelper::CheckTrigger(double dblData, BOOL bIsMarkAdd)
{
	CString strTimeDomain = CStatus::Instance()->m_strTimeDomainUnit;
	double dblCalcData = 0;

	if (m_GraphType == "RMS")
	{
		BOOL bIsAboveRMS = CStatus::Instance()->m_bIsTriggerAbove_RMS;

		if (dblData >= 0)
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblTriggerAbove_RMS_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblTriggerAbove_RMS_Value;

			if (bIsAboveRMS)
			{
				if (dblData >= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("RMS Value Above %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("RMS Value Above %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}	
			}
		}
		return FALSE;
	}
	if (m_GraphType == "X")
	{
		BOOL bIsAboveX = CStatus::Instance()->m_bIsTriggerAbove_X;
		BOOL bIsBelowX = CStatus::Instance()->m_bIsTriggerBelow_X;

		if (dblData >= 0)
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblTriggerAbove_X_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblTriggerAbove_X_Value;

			if (bIsAboveX)
			{
				if (dblData >= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("X Value Above %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("X Value Above %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		else
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblTriggerBelow_X_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblTriggerBelow_X_Value;

			if (bIsBelowX)
			{
				if (dblData <= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("X Value Below %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("X Value Below %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	if (m_GraphType == "Y")
	{
		BOOL bIsAboveY = CStatus::Instance()->m_bIsTriggerAbove_Y;
		BOOL bIsBelowY = CStatus::Instance()->m_bIsTriggerBelow_Y;

		if (dblData >= 0)
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblTriggerAbove_Y_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblTriggerAbove_Y_Value;

			if (bIsAboveY)
			{
				if (dblData >= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("Y Value Above %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("Y Value Above %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		else
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblTriggerBelow_Y_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblTriggerBelow_Y_Value;

			if (bIsBelowY)
			{
				if (dblData <= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("Y Value Below %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("Y Value Below %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	if (m_GraphType == "Z")
	{
		BOOL bIsAboveZ = CStatus::Instance()->m_bIsTriggerAbove_Z;
		BOOL bIsBelowZ = CStatus::Instance()->m_bIsTriggerBelow_Z;

		if (dblData >= 0)
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblTriggerAbove_Z_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblTriggerAbove_Z_Value;

			if (bIsAboveZ)
			{
				if (dblData >= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("Z Value Above %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("Z Value Above %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		else
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblTriggerBelow_Z_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblTriggerBelow_Z_Value;

			if (bIsBelowZ)
			{
				if (dblData <= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("Z Value Below %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("Z Value Below %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CDrawingHelper::CheckGoNoGo(double dblData, BOOL bIsMarkAdd)
{
	CString strTimeDomain = CStatus::Instance()->m_strTimeDomainUnit;
	double dblCalcData = 0;

	if (m_GraphType == "RMS")
	{
		BOOL bIsAboveRMS = CStatus::Instance()->m_bIsGoNoGoAbove_RMS;

		if (dblData >= 0)
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoAbove_RMS_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoAbove_RMS_Value;

			if (bIsAboveRMS)
			{
				if (dblData >= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("RMS Value Above %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("RMS Value Above %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	if (m_GraphType == "X")
	{
		BOOL bIsAboveX = CStatus::Instance()->m_bIsGoNoGoAbove_X;
		BOOL bIsBelowX = CStatus::Instance()->m_bIsGoNoGoBelow_X;

		if (dblData >= 0)
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoAbove_X_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoAbove_X_Value;

			if (bIsAboveX)
			{
				if (dblData >= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("X Value Above %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("X Value Above %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		else
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoBelow_X_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoBelow_X_Value;

			if (bIsBelowX)
			{
				if (dblData <= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("X Value Below %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("X Value Below %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	if (m_GraphType == "Y")
	{
		BOOL bIsAboveY = CStatus::Instance()->m_bIsGoNoGoAbove_Y;
		BOOL bIsBelowY = CStatus::Instance()->m_bIsGoNoGoBelow_Y;

		if (dblData >= 0)
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoAbove_Y_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoAbove_Y_Value;

			if (bIsAboveY)
			{
				if (dblData >= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("Y Value Above %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("Y Value Above %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		else
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoBelow_Y_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoBelow_Y_Value;

			if (bIsBelowY)
			{
				if (dblData <= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("Y Value Below %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("Y Value Below %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	if (m_GraphType == "Z")
	{
		BOOL bIsAboveZ = CStatus::Instance()->m_bIsGoNoGoAbove_Z;
		BOOL bIsBelowZ = CStatus::Instance()->m_bIsGoNoGoBelow_Z;

		if (dblData >= 0)
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoAbove_Z_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoAbove_Z_Value;

			if (bIsAboveZ)
			{
				if (dblData >= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("Z Value Above %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("Z Value Above %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		else
		{
			if (strTimeDomain == "g-Force")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoBelow_Z_Value / 981.;

			if (strTimeDomain == "GAL")
				dblCalcData = CStatus::Instance()->m_dblGoNoGoBelow_Z_Value;

			if (bIsBelowZ)
			{
				if (dblData <= dblCalcData)
				{
					if (bIsMarkAdd)
					{
						if (strTimeDomain == "g-Force")
							CStatus::Instance()->m_strEventText.Format(_T("Z Value Below %.2fg"), dblCalcData);
						if (strTimeDomain == "GAL")
							CStatus::Instance()->m_strEventText.Format(_T("Z Value Below %.2fGAL"), dblCalcData);
					}
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	return FALSE;
}

void CDrawingHelper::AlarmRectColor(BOOL bIsSpecOut, CStatus::tEventMode eventMode)
{
	if (eventMode == CStatus::Evt_TRIGGER || eventMode == CStatus::Evt_GONOGO)
	{
		if (bIsSpecOut)
		{
			CBrush newBrush(CStatus::Instance()->m_SpectOutColor);
			CBrush* oldBrush = m_pDC->SelectObject(&newBrush);
			CPen penBackGround(PS_SOLID, 0, CStatus::Instance()->m_SpectOutColor);
			CPen* oldPen = m_pDC->SelectObject(&penBackGround);

			m_pDC->Rectangle(m_alarmRect.left, m_alarmRect.top, m_alarmRect.right, m_alarmRect.bottom);

			m_pDC->SelectObject(oldPen);
			m_pDC->SelectObject(oldBrush);
			newBrush.DeleteObject();
			penBackGround.DeleteObject();
		}
		else
		{
			CBrush newBrush(CStatus::Instance()->m_SpectInColor);
			CBrush* oldBrush = m_pDC->SelectObject(&newBrush);
			CPen penBackGround(PS_SOLID, 0, CStatus::Instance()->m_SpectInColor);
			CPen* oldPen = m_pDC->SelectObject(&penBackGround);

			m_pDC->Rectangle(m_alarmRect.left, m_alarmRect.top, m_alarmRect.right, m_alarmRect.bottom);

			m_pDC->SelectObject(oldPen);
			m_pDC->SelectObject(oldBrush);
			newBrush.DeleteObject();
			penBackGround.DeleteObject();
		}
	}
}

CDrawingHelper::tCalcValue CDrawingHelper::CalcDataByTimeDomainUnit(double dblMaxPos, double dblData, CString strTimeDomainUnit)
{
	double	dblCalcValue =0, dblTempNum =0;
	tCalcValue calValue;
	if (strTimeDomainUnit == "g-Force")
	{
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE1_MODE)
		{
			dblCalcValue = dblData / 981.;// -2 ~ +2
			dblTempNum = dblMaxPos * dblCalcValue / 2.;// max 2
		}
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE2_MODE)
		{
			dblCalcValue = dblData / 981.;// -2 ~ +2
			dblTempNum = dblMaxPos * dblCalcValue / 3.;// max 2
		}
	}
		
	if (strTimeDomainUnit == "GAL")
	{
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE1_MODE)
		{
			dblCalcValue = (double)dblData;// -1962 ~ +1962
			dblTempNum = dblMaxPos * dblCalcValue / 1962.;// max 1962
		}
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE2_MODE)
		{
			dblCalcValue = (double)dblData;// -2943 ~ +2943
			dblTempNum = dblMaxPos * dblCalcValue / 2943.;// max 2943
		}
	}
	calValue.RealValue = dblCalcValue;		// 단위별 계산 값
	calValue.AdjustedValue = dblTempNum;			// 그래프 크기 따른 계산 값
	
	return calValue;
}

void CDrawingHelper::DrawSpecLine(CStatus::tEventMode eventMode)
{
	if (eventMode == CStatus::Evt_TRIGGER)
		DrawTriggerLine();
	if (eventMode == CStatus::Evt_GONOGO)
		DrawGoNoGoLine();
}

void CDrawingHelper::DrawTriggerLine()
{
	double dblMaxPos = m_graphRect.Height() / 2;
	int iScale = _ttoi(CStatus::Instance()->m_strGraphScale);
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;
	m_dblOneRectInterval = m_graphRect.Height() / CStatus::Instance()->m_ihorizontalNum;

	double dblValue, dblRePosWidth;
	tCalcValue calcValue;
	Point StartPt, EndPt;
	CPen penDrawLine;
	if (m_GraphType == "RMS")
	{
		BOOL bIsAboveRMS = CStatus::Instance()->m_bIsTriggerAbove_RMS;
		m_nStepMove = CStatus::Instance()->m_iRMSStandardValue;

		if (bIsAboveRMS)
		{
			double dblOrgValue = CStatus::Instance()->m_dblTriggerAbove_RMS_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = (dblMaxPos*2.) - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = (dblMaxPos*2.) + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_TriggerColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (m_GraphType == "X")
	{
		BOOL bIsAboveX = CStatus::Instance()->m_bIsTriggerAbove_X;
		BOOL bIsABelowX = CStatus::Instance()->m_bIsTriggerBelow_X;
		m_nStepMove = CStatus::Instance()->m_iXStandardValue;

		if (bIsAboveX)
		{
			double dblOrgValue = CStatus::Instance()->m_dblTriggerAbove_X_Value;	
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_TriggerColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
		if (bIsABelowX)
		{
			double dblOrgValue = CStatus::Instance()->m_dblTriggerBelow_X_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_TriggerColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (m_GraphType == "Y")
	{
		BOOL bIsAboveY = CStatus::Instance()->m_bIsTriggerAbove_Y;
		BOOL bIsABelowY = CStatus::Instance()->m_bIsTriggerBelow_Y;
		m_nStepMove = CStatus::Instance()->m_iYStandardValue;

		if (bIsAboveY)
		{
			double dblOrgValue = CStatus::Instance()->m_dblTriggerAbove_Y_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_TriggerColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
		if (bIsABelowY)
		{
			double dblOrgValue = CStatus::Instance()->m_dblTriggerBelow_Y_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_TriggerColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (m_GraphType == "Z")
	{
		BOOL bIsAboveZ = CStatus::Instance()->m_bIsTriggerAbove_Z;
		BOOL bIsABelowZ = CStatus::Instance()->m_bIsTriggerBelow_Z;
		m_nStepMove = CStatus::Instance()->m_iZStandardValue;

		if (bIsAboveZ)
		{
			double dblOrgValue = CStatus::Instance()->m_dblTriggerAbove_Z_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_TriggerColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
		if (bIsABelowZ)
		{
			double dblOrgValue = CStatus::Instance()->m_dblTriggerBelow_Z_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_TriggerColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
	}
}

void CDrawingHelper::DrawGoNoGoLine()
{
	double dblMaxPos = m_graphRect.Height() / 2;
	int iScale = _ttoi(CStatus::Instance()->m_strGraphScale);
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;
	m_dblOneRectInterval = m_graphRect.Height() / CStatus::Instance()->m_ihorizontalNum;

	double dblValue, dblRePosWidth;
	tCalcValue calcValue;
	Point StartPt, EndPt;
	CPen penDrawLine;
	if (m_GraphType == "RMS")
	{
		BOOL bIsAboveRMS = CStatus::Instance()->m_bIsGoNoGoAbove_RMS;
		m_nStepMove = CStatus::Instance()->m_iRMSStandardValue;

		if (bIsAboveRMS)
		{
			double dblOrgValue = CStatus::Instance()->m_dblGoNoGoAbove_RMS_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = (dblMaxPos*2.) - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = (dblMaxPos*2.) + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_GoNoGoColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (m_GraphType == "X")
	{
		BOOL bIsAboveX = CStatus::Instance()->m_bIsGoNoGoAbove_X;
		BOOL bIsABelowX = CStatus::Instance()->m_bIsGoNoGoBelow_X;
		m_nStepMove = CStatus::Instance()->m_iXStandardValue;

		if (bIsAboveX)
		{
			double dblOrgValue = CStatus::Instance()->m_dblGoNoGoAbove_X_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_GoNoGoColor);

			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
		if (bIsABelowX)
		{
			double dblOrgValue = CStatus::Instance()->m_dblGoNoGoBelow_X_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_GoNoGoColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (m_GraphType == "Y")
	{
		BOOL bIsAboveY = CStatus::Instance()->m_bIsGoNoGoAbove_Y;
		BOOL bIsABelowY = CStatus::Instance()->m_bIsGoNoGoBelow_Y;
		m_nStepMove = CStatus::Instance()->m_iYStandardValue;

		if (bIsAboveY)
		{
			double dblOrgValue = CStatus::Instance()->m_dblGoNoGoAbove_Y_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_GoNoGoColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
		if (bIsABelowY)
		{
			double dblOrgValue = CStatus::Instance()->m_dblGoNoGoBelow_Y_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_GoNoGoColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (m_GraphType == "Z")
	{
		BOOL bIsAboveZ = CStatus::Instance()->m_bIsGoNoGoAbove_Z;
		BOOL bIsABelowZ = CStatus::Instance()->m_bIsGoNoGoBelow_Z;
		m_nStepMove = CStatus::Instance()->m_iZStandardValue;

		if (bIsAboveZ)
		{
			double dblOrgValue = CStatus::Instance()->m_dblGoNoGoAbove_Z_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_GoNoGoColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
		if (bIsABelowZ)
		{
			double dblOrgValue = CStatus::Instance()->m_dblGoNoGoBelow_Z_Value;
			calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblOrgValue, strTimeDomainUnit);

			dblRePosWidth = abs(calcValue.AdjustedValue);

			if (dblOrgValue >= 0)
				dblValue = dblMaxPos - (dblRePosWidth * (double)iScale) + m_iInterval;
			else
				dblValue = dblMaxPos + (dblRePosWidth * (double)iScale) + m_iInterval;

			dblValue += m_nStepMove * ((double)m_dblOneRectInterval * (double)iScale);

			penDrawLine.CreatePen(PS_SOLID, 0, CStatus::Instance()->m_GoNoGoColor);
			CPen* oldPen = m_pDC->SelectObject(&penDrawLine);
			m_pDC->MoveTo(m_graphRect.left, (int)dblValue);
			m_pDC->LineTo(m_graphRect.right, (int)dblValue);
			m_pDC->SelectObject(oldPen);
			penDrawLine.DeleteObject();
		}
	}
}

void CDrawingHelper::DrawReviewUnit(int iNum, int newPt_X, int nInputType)
{
	CString strReviewUnit;
	double dblReviewUnit = CStatus::Instance()->m_dblUnit;
	double calcNum = 0;
	int iOnceUnitDraw = 0;

	CPen penBackGround(PS_SOLID, 0, ::GetSysColor(COLOR_BTNSHADOW));
	CPen* oldPen = m_pDC->SelectObject(&penBackGround);
	int nFrequeny = (int)(CStatus::Instance()->GetDeviceFrequency());
	iOnceUnitDraw = (int)(dblReviewUnit *(double)nFrequeny);
	if (iNum % iOnceUnitDraw == 0 && iNum != 0)
	{
		if (newPt_X < m_graphRect.left || newPt_X > m_graphRect.right)
			return;
		calcNum = (iNum / iOnceUnitDraw) * dblReviewUnit;
		//ONERECT_PIXEL
			//calcNum = ONERECT_PIXEL * dblReviewUnit;
		strReviewUnit.Format(_T("%.1f"), calcNum);
		Pen backgroundPen(Color(169, 169, 169, 169), 0.7f);

		m_pDC->MoveTo(newPt_X, m_graphRect.top);
		m_pDC->LineTo(newPt_X, m_graphRect.bottom);
		CStatus::Instance()->SetUnitTextPtX(newPt_X, strReviewUnit, nInputType);
	}
	m_pDC->SelectObject(oldPen);
	penBackGround.DeleteObject();
}

void CDrawingHelper::TriggerSaveAvailability(BOOL bIsTrigger, int nIndex)
{
	if (bIsTrigger)
	{

		if (CStatus::Instance()->GetIsSepcOutSave() && CStatus::Instance()->GetRepository()->GetFileSaveStatus() == CRepository::SS_STOP)
		{
			CStatus::Instance()->StartRecording(nIndex);
		}
	}
}

void CDrawingHelper::GoNoGoSaveAvailability(BOOL bIsGoNoGo, int nIndex)
{
	int iGoNoGoTime = CStatus::Instance()->m_iGoNoGo_QualityTime; // 2는 기존 1000에서 500으로 변경되었기 때문
	int nFrequeny = (int)(CStatus::Instance()->GetDeviceFrequency());
	int iNoGoSecond = (int)(CStatus::Instance()->m_iNoGoIndicator * (double)nFrequeny);

	if (CStatus::Instance()->m_iGoNoGoRecordingStartIndex + iNoGoSecond >= nIndex && 
		CStatus::Instance()->GetRepository()->GetFileSaveStatus() == CRepository::SS_WRITE)
	{
	}
	else
	{
		m_strGoNoGoGraphType = _T("NONE");
		if (bIsGoNoGo)	m_iGoNoGoCount++;
		else	m_iGoNoGoCount = 0;

		if (m_iGoNoGoCount >= iGoNoGoTime)
		{
			m_iGoNoGoCount = 0;

			CStatus::Instance()->m_iGoNoGoRecordingStartIndex = nIndex;

			if (CStatus::Instance()->GetIsSepcOutSave())
			{
				if (CStatus::Instance()->GetRepository()->GetFileSaveStatus() == CRepository::SS_STOP)
					CStatus::Instance()->StartRecording(nIndex);
				if (CStatus::Instance()->GetRepository()->GetFileSaveStatus() == CRepository::SS_WRITE)
				{
					CStatus::Instance()->FinishRecording();
					CStatus::Instance()->StartRecording(nIndex);
				}
			}
			m_strGoNoGoGraphType = m_GraphType;
		}
	}
}

void CDrawingHelper::InspectTrigger(int iDataCount, int nInputType)
{
	tCalcValue calcValue;
	tAVMDData NewData;

	double dblMaxPos = m_graphRect.Height() / 2;
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;

	m_iTempTriggerEnd = iDataCount;

	if (m_GraphType == "RMS")
	{
		if (m_iTempTriggerEnd != m_iCheckTriggerEndNum)
		{
			if (!CStatus::Instance()->GetIsRMSTrigger())
			{
				CStatus::Instance()->SetIsRMSTrigger(TRUE);
				m_iCheckTriggerEndNum = m_iTempTriggerEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
				{
					for (int j = m_iCheckTriggerStartNum; j < m_iCheckTriggerEndNum; j++)
					{
						CStatus::Instance()->GetRepository()->GetAtData(&NewData, j, nInputType);
						CalcRMS(&NewData);
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iRMSValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, TRUE);
						m_bPrevTriggerSpecOut = bIsSpecOut;
						TriggerSaveAvailability(bIsSpecOut, j);
					}
				}
				
				m_iCheckTriggerStartNum = m_iCheckTriggerEndNum;
				CStatus::Instance()->SetIsRMSTrigger(FALSE);
			}
		}
	}
	if (m_GraphType == "X")
	{
		if (m_iTempTriggerEnd != m_iCheckTriggerEndNum)
		{
			if (!CStatus::Instance()->GetIsXTrigger())
			{
				CStatus::Instance()->SetIsXTrigger(TRUE);
				m_iCheckTriggerEndNum = m_iTempTriggerEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
				{
					for (int j = m_iCheckTriggerStartNum; j < m_iCheckTriggerEndNum; j++)
					{

						CStatus::Instance()->GetRepository()->GetAtData(&NewData, j, nInputType);
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iXValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, TRUE);
						TriggerSaveAvailability(bIsSpecOut, j);
					}
				}

				m_iCheckTriggerStartNum = m_iCheckTriggerEndNum;
				CStatus::Instance()->SetIsXTrigger(FALSE);
			}
		}
	}
	if (m_GraphType == "Y")
	{
		if (m_iTempTriggerEnd != m_iCheckTriggerEndNum)
		{
			if (!CStatus::Instance()->GetIsYTrigger())
			{
				CStatus::Instance()->SetIsYTrigger(TRUE);
				m_iCheckTriggerEndNum = m_iTempTriggerEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
				{
					for (int j = m_iCheckTriggerStartNum; j < m_iCheckTriggerEndNum; j++)
					{
						CStatus::Instance()->GetRepository()->GetAtData(&NewData, j, nInputType);
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iYValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, TRUE);
						TriggerSaveAvailability(bIsSpecOut, j);
					}
				}

				m_iCheckTriggerStartNum = m_iCheckTriggerEndNum;
				CStatus::Instance()->SetIsYTrigger(FALSE);
			}
		}
	}
	if (m_GraphType == "Z")
	{
		if (m_iTempTriggerEnd != m_iCheckTriggerEndNum)
		{
			if (!CStatus::Instance()->GetIsZTrigger())
			{
				CStatus::Instance()->SetIsZTrigger(TRUE);
				m_iCheckTriggerEndNum = m_iTempTriggerEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
				{
					for (int j = m_iCheckTriggerStartNum; j < m_iCheckTriggerEndNum; j++)
					{
						CStatus::Instance()->GetRepository()->GetAtData(&NewData, j, nInputType);
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iZValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, TRUE);
						TriggerSaveAvailability(bIsSpecOut, j);
					}
				}

				m_iCheckTriggerStartNum = m_iCheckTriggerEndNum;
				CStatus::Instance()->SetIsZTrigger(FALSE);
			}
		}
	}
}

void CDrawingHelper::InspectGoNoGo(int iTempDataCount, int nInputType)
{
	tCalcValue calcValue;
	tAVMDData NewData;

	double dblMaxPos = m_graphRect.Height() / 2;
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;
	m_iTempGoNoGoEnd = iTempDataCount;

	if (m_GraphType == "RMS")
	{
		if (m_iTempGoNoGoEnd != m_iCheckGoNoGoEndNum)
		{
			if (!CStatus::Instance()->GetIsRMSGoNoGo())
			{
				CStatus::Instance()->SetIsRMSGoNoGo(TRUE);
				m_iCheckGoNoGoEndNum = m_iTempGoNoGoEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
				{
					for (int j = m_iCheckGoNoGoStartNum; j < m_iCheckGoNoGoEndNum; j++)
					{
						CStatus::Instance()->GetRepository()->GetAtData(&NewData, j, nInputType);
						CalcRMS(&NewData);
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iRMSValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, TRUE);
						GoNoGoSaveAvailability(bIsSpecOut, j);
					}
				}

				m_iCheckGoNoGoStartNum = m_iCheckGoNoGoEndNum;
				CStatus::Instance()->SetIsRMSGoNoGo(FALSE);
			}
		}
	}

	if (m_GraphType == "X")
	{
		if (m_iTempGoNoGoEnd != m_iCheckGoNoGoEndNum)
		{
			if (!CStatus::Instance()->GetIsXGoNoGo())
			{
				CStatus::Instance()->SetIsXGoNoGo(TRUE);
				m_iCheckGoNoGoEndNum = m_iTempGoNoGoEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
				{
					for (int j = m_iCheckGoNoGoStartNum; j < m_iCheckGoNoGoEndNum; j++)
					{
						CStatus::Instance()->GetRepository()->GetAtData(&NewData, j, nInputType);
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iXValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, TRUE);
						GoNoGoSaveAvailability(bIsSpecOut, j);
					}
				}

				m_iCheckGoNoGoStartNum = m_iCheckGoNoGoEndNum;
				CStatus::Instance()->SetIsXGoNoGo(FALSE);
			}
		}
	}
	if (m_GraphType == "Y")
	{
		if (m_iTempGoNoGoEnd != m_iCheckGoNoGoEndNum)
		{
			if (!CStatus::Instance()->GetIsYGoNoGo())
			{
				CStatus::Instance()->SetIsYGoNoGo(TRUE);
				m_iCheckGoNoGoEndNum = m_iTempGoNoGoEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
				{
					for (int j = m_iCheckGoNoGoStartNum; j < m_iCheckGoNoGoEndNum; j++)
					{
						CStatus::Instance()->GetRepository()->GetAtData(&NewData, j, nInputType);
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iYValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, TRUE);
						GoNoGoSaveAvailability(bIsSpecOut, j);
					}
				}
				m_iCheckGoNoGoStartNum = m_iCheckGoNoGoEndNum;
				CStatus::Instance()->SetIsYGoNoGo(FALSE);
			}
		}
	}
	if (m_GraphType == "Z")
	{
		if (m_iTempGoNoGoEnd != m_iCheckGoNoGoEndNum)
		{
			if (!CStatus::Instance()->GetIsZGoNoGo())
			{
				CStatus::Instance()->SetIsZGoNoGo(TRUE);
				m_iCheckGoNoGoEndNum = m_iTempGoNoGoEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
				{
					for (int j = m_iCheckGoNoGoStartNum; j < m_iCheckGoNoGoEndNum; j++)
					{
						CStatus::Instance()->GetRepository()->GetAtData(&NewData, j, nInputType);
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, (double)NewData.iZValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, TRUE);
						GoNoGoSaveAvailability(bIsSpecOut, j);
					}
				}
				m_iCheckGoNoGoStartNum = m_iCheckGoNoGoEndNum;
				CStatus::Instance()->SetIsZGoNoGo(FALSE);
			}
		}
	}
}

void CDrawingHelper::CalcUnitPt(double dblTempGraphPosWidth, int iDrawStartNum, int iDrawEndNum, int nInputType, int nPriority)
{
	int nStrCount = 0;
	BOOL bIsOverlay = CStatus::Instance()->GetOverlayCondition();
	//double dblOneRectDrawNum = (double)CYCLE * dblTimeUnit; // 가변 되야함 한칸당 몇개씩 그릴거냐
	CStatus::Instance()->ClearUnitTextPtX(nInputType);
	if (!bIsOverlay)
	{
		for (int k = iDrawStartNum; k <= iDrawEndNum; k++)
		{
			int iStringUnit = m_graphRect.left + (int)((double)(nStrCount)* dblTempGraphPosWidth);
			DrawReviewUnit(k, iStringUnit, nInputType);
			nStrCount++;
		}
	}
	else
	{
		if (nInputType == 1 && nPriority == 0)
			return;

		if (nInputType == 0 && nPriority == 0)
		{
			for (int k = iDrawStartNum; k <= iDrawEndNum; k++)
			{
				int iStringUnit = m_graphRect.left + (int)((double)(nStrCount)* dblTempGraphPosWidth);
				DrawReviewUnit(k, iStringUnit, nInputType);
				nStrCount++;
			}
			return;
		}
		if (nInputType == 1 && nPriority ==1)
		{
			for (int k = iDrawStartNum; k <= iDrawEndNum; k++)
			{
				int iStringUnit = m_graphRect.left + (int)((double)(nStrCount)* dblTempGraphPosWidth);
				DrawReviewUnit(k, iStringUnit, nInputType);
				nStrCount++;
			}
			return;
		}
	}
}

CDrawingHelper::tMarkCheckInfo CDrawingHelper::MarkCheckFromIndex(int nIndex, int nInputType, int nPriority)
{
	int nMarkCount = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].size();
	tMarkInfo *pMarkInfo;
	tMarkCheckInfo tCheckInfo;
	BOOL bIsOverlay = CStatus::Instance()->GetOverlayCondition();

	if (!bIsOverlay)
	{
		for (int i = 0; i < nMarkCount; i++)
		{
			pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].at(i);
			if (nIndex == pMarkInfo->iIndex)
			{
				tCheckInfo.bIsMark = TRUE;
				tCheckInfo.strMark = pMarkInfo->strMark;
				return tCheckInfo;
			}
			else
			{
				tCheckInfo.bIsMark = FALSE;
			}
		}
	}
	else
	{
		if (nInputType == 1 && nPriority == 0)
		{
			tCheckInfo.bIsMark = FALSE;
			return tCheckInfo;
		}
		if (nInputType == 0 && nPriority == 0)
		{
			for (int i = 0; i < nMarkCount; i++)
			{
				pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].at(i);
				if (nIndex == pMarkInfo->iIndex)
				{
					tCheckInfo.bIsMark = TRUE;
					tCheckInfo.strMark = pMarkInfo->strMark;
					return tCheckInfo;
				}
				else
				{
					tCheckInfo.bIsMark = FALSE;
				}
			}
		}
		if (nInputType == 1 && nPriority == 1)
		{
			for (int i = 0; i < nMarkCount; i++)
			{
				pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].at(i);
				if (nIndex == pMarkInfo->iIndex)
				{
					tCheckInfo.bIsMark = TRUE;
					tCheckInfo.strMark = pMarkInfo->strMark;
					return tCheckInfo;
				}
				else
				{
					tCheckInfo.bIsMark = FALSE;
				}
			}
		}
	}
	tCheckInfo.bIsMark = FALSE;
	return tCheckInfo;
}

void CDrawingHelper::CalcMarkPt(double dblTempGraphPosWidth, int iDrawStartNum, int iDrawEndNum, int nInputType, int nPriority)
{
	int nStrCount = 0;
	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
	BOOL bIsOverlay = CStatus::Instance()->GetOverlayCondition();
	tMarkCheckInfo tCheckInfo;

	CPen penBackGround(PS_SOLID, 0, CStatus::Instance()->m_MarkColor);
	CPen* oldPen = m_pDC->SelectObject(&penBackGround);

	CFont font;
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = FONT_SIZE;
	lf.lfWeight = FW_BOLD;
	wsprintf(lf.lfFaceName, TEXT("%s"), TEXT("Arial"));
	font.CreateFontIndirectW(&lf);
	m_pDC->SelectObject(&font);
	m_pDC->SetBkMode(TRANSPARENT);
	m_pDC->SetTextAlign(TA_LEFT);

	if (!bIsOverlay)
	{
		
		for (int k = iDrawStartNum; k <= iDrawEndNum; k++)
		{
			int iMarkPtX = m_graphRect.left + (int)((double)(nStrCount)* dblTempGraphPosWidth);
			tCheckInfo = MarkCheckFromIndex(k, nInputType, nPriority);

			if (iMarkPtX < m_graphRect.left || iMarkPtX > m_graphRect.right)
				return;
			if (tCheckInfo.bIsMark)
			{
				m_pDC->MoveTo(iMarkPtX, m_graphRect.top);
				m_pDC->LineTo(iMarkPtX, m_graphRect.bottom);
				//mark String 쓰기

				Point markPt;
				markPt.X = iMarkPtX;
				markPt.Y = m_graphRect.top;
				m_pDC->TextOutW(markPt.X, markPt.Y, tCheckInfo.strMark);
			}
			nStrCount++;
		}
			if (iDrawEndNum == (nDataCount - 1))
			{
				m_pDC->MoveTo(m_graphRect.right, m_graphRect.top);
				m_pDC->LineTo(m_graphRect.right, m_graphRect.bottom);
				Point markPt;
				markPt.X = m_graphRect.right;
				markPt.Y = m_graphRect.top;
				m_pDC->TextOutW(markPt.X, markPt.Y, _T("End"));
			}
	}
	else
	{
		if (nInputType == 1 && nPriority == 0)
			return;

		if (nInputType == 0 && nPriority == 0)
		{
			for (int k = iDrawStartNum; k <= iDrawEndNum; k++)
			{
				int iMarkPtX = m_graphRect.left + (int)((double)(nStrCount)* dblTempGraphPosWidth);
				tCheckInfo = MarkCheckFromIndex(k, nInputType, nPriority);
				if (iMarkPtX < m_graphRect.left || iMarkPtX > m_graphRect.right)
					return;
				if (tCheckInfo.bIsMark)
				{
					m_pDC->MoveTo(iMarkPtX, m_graphRect.top);
					m_pDC->LineTo(iMarkPtX, m_graphRect.bottom);
					//mark String 쓰기

					Point markPt;
					markPt.X = iMarkPtX;
					markPt.Y = m_graphRect.top;
					m_pDC->TextOutW(markPt.X, markPt.Y, tCheckInfo.strMark);
				}
				nStrCount++;
			}
			if (iDrawEndNum == (nDataCount - 1))
			{
				m_pDC->MoveTo(m_graphRect.right, m_graphRect.top);
				m_pDC->LineTo(m_graphRect.right, m_graphRect.bottom);
				Point markPt;
				markPt.X = m_graphRect.right;
				markPt.Y = m_graphRect.top;
				m_pDC->TextOutW(markPt.X, markPt.Y, _T("End"));
			}
		}
		if (nInputType == 1 && nPriority == 1)
		{
			for (int k = iDrawStartNum; k <= iDrawEndNum; k++)
			{
				int iMarkPtX = m_graphRect.left + (int)((double)(nStrCount)* dblTempGraphPosWidth);
				tCheckInfo = MarkCheckFromIndex(k, nInputType, nPriority);
				if (iMarkPtX < m_graphRect.left || iMarkPtX > m_graphRect.right)
					return;
				if (tCheckInfo.bIsMark)
				{
					m_pDC->MoveTo(iMarkPtX, m_graphRect.top);
					m_pDC->LineTo(iMarkPtX, m_graphRect.bottom);
					//mark String 쓰기

					Point markPt;
					markPt.X = iMarkPtX;
					markPt.Y = m_graphRect.top;
					m_pDC->TextOutW(markPt.X, markPt.Y, tCheckInfo.strMark);
				}
				nStrCount++;
			}
			if (iDrawEndNum == (nDataCount - 1))
			{
				m_pDC->MoveTo(m_graphRect.right, m_graphRect.top);
				m_pDC->LineTo(m_graphRect.right, m_graphRect.bottom);
				Point markPt;
				markPt.X = m_graphRect.right;
				markPt.Y = m_graphRect.top;
				m_pDC->TextOutW(markPt.X, markPt.Y, _T("End"));
			}
		}
	}
	m_pDC->SelectObject(oldPen);
	font.DeleteObject();
	penBackGround.DeleteObject();
}

CDrawingHelper::tReportFFTResult CDrawingHelper::ReportFFT(CString strGraphType, int iDrawStartNum)
{
	int iNum, iLog2N, iFftSampleSize;
	Complex *Data;
	tAVMDData NewData;
	Complex *pReportFFTData;
	tReportFFTResult reportFFtResult;
	iNum = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	iFftSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	iLog2N = 0;
	
	while (iNum >= 2)
	{
		iNum >>= 1;
		iLog2N++;
	}
	int nIndex = 0;
	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(0);

	if (nDataCount <= (iDrawStartNum + iFftSampleSize))
	{
		reportFFtResult.bIsFFTFail = TRUE;
		return reportFFtResult;
	}

	Data = new Complex[iFftSampleSize];
	pReportFFTData = new Complex[iFftSampleSize];

	for (int i = iDrawStartNum; i < iDrawStartNum + iFftSampleSize; i++)
	{
		if (CStatus::Instance()->IsFilterUse())
		{
			Data[nIndex].Re = (double)CStatus::Instance()->GetViewFilteredValue(strGraphType, 0, i);
		}
		else
		{
			CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, 0);
			if (strGraphType == "X")
				Data[nIndex].Re = (double)NewData.iXValue;
			if (strGraphType == "Y")
				Data[nIndex].Re = (double)NewData.iYValue;
			if (strGraphType == "Z")
				Data[nIndex].Re = (double)NewData.iZValue;
		}
		
		Data[nIndex].Im = 0;
		nIndex++;
	}

	OnFft1d(Data, iFftSampleSize, iLog2N); // 1차원FFT

	for (int i = 0; i < iFftSampleSize; i++)
		pReportFFTData[i] = Data[i];

	delete[] Data;
	double *dblFFT;

	dblFFT = new double[iFftSampleSize / 2];
	int iMaxHz = 0;
	double dblMaxFFTValue = 0;

	int iHighValue = CStatus::Instance()->m_iFilterHighValue;
	int iLowValue = CStatus::Instance()->m_iFilterLowValue;
	
	for (int j = 0; j < iFftSampleSize / 2; j++)
	{
		dblFFT[j] = AmplitudeScaled(pReportFFTData[j].Re, pReportFFTData[j].Im, iFftSampleSize / 2, iFftSampleSize);

		if (j != 0)
		{
			if (dblFFT[j] >= dblMaxFFTValue)
			{
				iMaxHz = j;
				dblMaxFFTValue = dblFFT[j];
			}
		}
	}
	
	CString strTimeUnit = CStatus::Instance()->m_strTimeDomainUnit;

	reportFFtResult.bIsFFTFail = FALSE;	// Report 성공
	reportFFtResult.iMaxHz = iMaxHz;

	if (strTimeUnit == "g-Force")
		reportFFtResult.dblMaxValue = dblMaxFFTValue/981.;

	if (strTimeUnit == "GAL")
		reportFFtResult.dblMaxValue = dblMaxFFTValue;

	delete[] pReportFFTData, dblFFT, Data;
	return reportFFtResult;
}

void CDrawingHelper::DrawingMonitoringValue(int nIndex, double dblValue) 
{
	CFont font;
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 13;
	wsprintf(lf.lfFaceName, TEXT("%s"), TEXT("Arial"));
	font.CreateFontIndirectW(&lf);
	m_pDC->SelectObject(&font);
	CString strStanardValue, strTimeDomain;
	m_pDC->SetBkMode(TRANSPARENT);
	m_pDC->SetTextAlign(TA_LEFT);
	strStanardValue.Format(_T("%.3f"), dblValue);
	
	m_pDC->TextOutW(m_graphRect.right + 5, m_graphRect.bottom / 2, strStanardValue);
	font.DeleteObject();
}

void CDrawingHelper::DrawIndicator()
{
	CPen penBackGround(PS_SOLID |PS_ENDCAP_FLAT, 3, CStatus::Instance()->m_IndicatorColor);
	CPen * oldPen = m_pDC->SelectObject(&penBackGround);
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
	{
		PointF markStartPoint, markEndPoint;
		markStartPoint.X = (REAL)(m_graphRect.right + 2);
		markStartPoint.Y = (REAL)m_graphRect.top;
		markEndPoint.X = (REAL)(m_graphRect.right + 2);
		markEndPoint.Y = (REAL)m_graphRect.bottom;

		m_pDC->MoveTo((int)markStartPoint.X, (int)markStartPoint.Y);
		m_pDC->LineTo((int)markEndPoint.X, (int)markEndPoint.Y);
	}
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		PointF markStartPoint, markEndPoint;
		markStartPoint.X = (REAL)(m_graphRect.left - 2);
		markStartPoint.Y = (REAL)m_graphRect.top;
		markEndPoint.X = (REAL)(m_graphRect.left - 2);
		markEndPoint.Y = (REAL)m_graphRect.bottom;

		m_pDC->MoveTo((int)markStartPoint.X, (int)markStartPoint.Y);
		m_pDC->LineTo((int)markEndPoint.X, (int)markEndPoint.Y);
	}
	m_pDC->SelectObject(oldPen);
	penBackGround.DeleteObject();
}

void CDrawingHelper::SetDrawPointNum()
{
	double dblTimeUnit = CStatus::Instance()->m_dblUnit;		//ex 0.1	
	int nFrequeny = (int)(CStatus::Instance()->GetDeviceFrequency());
	double dblOneRectDrawNum = (double)nFrequeny * dblTimeUnit; // 가변 되야함 한칸당 몇개씩 그릴거냐
	m_dblOneRectInterval = m_graphRect.Height() / CStatus::Instance()->m_ihorizontalNum;// 한칸당 높이
	CStatus::Instance()->m_nDrawPointNum = (int)(dblOneRectDrawNum * ((double)m_graphRect.Width() / (double)ONERECT_PIXEL));
}

void CDrawingHelper::InspectFilterTrigger(int iDataEndNum, int nInputType)
{
	tCalcValue calcValue;

	double dblMaxPos = m_graphRect.Height() / 2;
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;
	int nArraySize = CStatus::Instance()->m_nDrawPointNum;
	m_iTempTriggerEnd = iDataEndNum;

	if (m_GraphType == "RMS")
	{
		if (m_iTempTriggerEnd != m_iCheckTriggerEndNum)
		{
			if (!CStatus::Instance()->GetIsRMSTrigger())
			{
				CStatus::Instance()->SetIsRMSTrigger(TRUE);
				m_iCheckTriggerEndNum = m_iTempTriggerEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
				{
					int nEndIndex = abs(m_iCheckTriggerEndNum - m_iCheckTriggerStartNum);
					for (int j = 0; j < nEndIndex; j++)
					{
						double	dblValue = m_pdblFilteredData[nInputType][j];
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, TRUE);

						int iSaveStartIndex = m_iCheckTriggerEndNum - j;
						TriggerSaveAvailability(bIsSpecOut, iSaveStartIndex);
					}
				}
				m_iCheckTriggerStartNum = m_iCheckTriggerEndNum;
				CStatus::Instance()->SetIsRMSTrigger(FALSE);
			}
		}
	}
	if (m_GraphType == "X")
	{
		if (m_iTempTriggerEnd != m_iCheckTriggerEndNum)
		{
			if (!CStatus::Instance()->GetIsXTrigger())
			{
				CStatus::Instance()->SetIsXTrigger(TRUE);
				m_iCheckTriggerEndNum = m_iTempTriggerEnd;
				if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
				{
					int nEndIndex = abs(m_iCheckTriggerEndNum - m_iCheckTriggerStartNum);
					for (int j = 0; j < nEndIndex; j++)
					{
						double	dblValue = m_pdblFilteredData[nInputType][j];
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, TRUE);

						int iSaveStartIndex = m_iCheckTriggerEndNum - j;
						TriggerSaveAvailability(bIsSpecOut, iSaveStartIndex);
					}
				}
				m_iCheckTriggerStartNum = m_iCheckTriggerEndNum;
				CStatus::Instance()->SetIsXTrigger(FALSE);
			}
		}
	}
	if (m_GraphType == "Y")
	{
		if (m_iTempTriggerEnd != m_iCheckTriggerEndNum)
		{
			if (!CStatus::Instance()->GetIsYTrigger())
			{
				CStatus::Instance()->SetIsYTrigger(TRUE);
				m_iCheckTriggerEndNum = m_iTempTriggerEnd;
			
				if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
				{
					int nEndIndex = abs(m_iCheckTriggerEndNum - m_iCheckTriggerStartNum);
					for (int j = 0; j < nEndIndex; j++)
					{
						double	dblValue = m_pdblFilteredData[nInputType][j];
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, TRUE);

						int iSaveStartIndex = m_iCheckTriggerEndNum - j;
						TriggerSaveAvailability(bIsSpecOut, iSaveStartIndex);
					}
				}
				m_iCheckTriggerStartNum = m_iCheckTriggerEndNum;
				CStatus::Instance()->SetIsYTrigger(FALSE);
			}
		}
	}
	if (m_GraphType == "Z")
	{
		if (m_iTempTriggerEnd != m_iCheckTriggerEndNum)
		{
			if (!CStatus::Instance()->GetIsZTrigger())
			{
				CStatus::Instance()->SetIsZTrigger(TRUE);
				m_iCheckTriggerEndNum = m_iTempTriggerEnd;
				
				if (CStatus::Instance()->m_EventMode == CStatus::Evt_TRIGGER)
				{
					int nEndIndex = abs(m_iCheckTriggerEndNum - m_iCheckTriggerStartNum);
					for (int j = 0; j < nEndIndex; j++)
					{
						double	dblValue = m_pdblFilteredData[nInputType][j];
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckTrigger(calcValue.RealValue, TRUE);
						int iSaveStartIndex = m_iCheckTriggerEndNum - j;
						TriggerSaveAvailability(bIsSpecOut, iSaveStartIndex);
					}
				}
				m_iCheckTriggerStartNum = m_iCheckTriggerEndNum;
				CStatus::Instance()->SetIsZTrigger(FALSE);
			}
		}
	}
}

void CDrawingHelper::InspectFilterGoNoGo(int iDataEndNum, int nInputType)
{
	tCalcValue calcValue;

	double dblMaxPos = m_graphRect.Height() / 2;
	CString strTimeDomainUnit = CStatus::Instance()->m_strTimeDomainUnit;
	m_iTempGoNoGoEnd = iDataEndNum;

	if (m_GraphType == "RMS")
	{
		if (m_iTempGoNoGoEnd != m_iCheckGoNoGoEndNum)
		{
			if (!CStatus::Instance()->GetIsRMSGoNoGo())
			{
				CStatus::Instance()->SetIsRMSGoNoGo(TRUE);
				m_iCheckGoNoGoEndNum = m_iTempGoNoGoEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
				{
					int nEndIndex = abs(m_iCheckGoNoGoEndNum - m_iCheckGoNoGoStartNum);
					for (int j = 0; j < nEndIndex; j++)
					{
						double	dblValue = m_pdblFilteredData[nInputType][j];
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, TRUE);

						int iSaveStartIndex = m_iCheckGoNoGoEndNum - j;
						GoNoGoSaveAvailability(bIsSpecOut, iSaveStartIndex);
					}
				}

				m_iCheckGoNoGoStartNum = m_iCheckGoNoGoEndNum;
				CStatus::Instance()->SetIsRMSGoNoGo(FALSE);
			}
		}
	}
	if (m_GraphType == "X")
	{
		if (m_iTempGoNoGoEnd != m_iCheckGoNoGoEndNum)
		{
			if (!CStatus::Instance()->GetIsXGoNoGo())
			{
				CStatus::Instance()->SetIsXGoNoGo(TRUE);
				m_iCheckGoNoGoEndNum = m_iTempGoNoGoEnd;
				
				if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
				{
					int nEndIndex = abs(m_iCheckGoNoGoEndNum - m_iCheckGoNoGoStartNum);
					for (int j = 0; j < nEndIndex; j++)
					{
						double	dblValue = m_pdblFilteredData[nInputType][j];
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, TRUE);

						int iSaveStartIndex = m_iCheckGoNoGoEndNum - j;
						GoNoGoSaveAvailability(bIsSpecOut, iSaveStartIndex);
					}
				}

				m_iCheckGoNoGoStartNum = m_iCheckGoNoGoEndNum;
				CStatus::Instance()->SetIsXGoNoGo(FALSE);
			}
		}
	}
	if (m_GraphType == "Y")
	{
		if (m_iTempGoNoGoEnd != m_iCheckGoNoGoEndNum)
		{
			if (!CStatus::Instance()->GetIsYGoNoGo())
			{
				CStatus::Instance()->SetIsYGoNoGo(TRUE);
				m_iCheckGoNoGoEndNum = m_iTempGoNoGoEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
				{
					int nEndIndex = abs(m_iCheckGoNoGoEndNum - m_iCheckGoNoGoStartNum);
					for (int j = 0; j < nEndIndex; j++)
					{
						double	dblValue = m_pdblFilteredData[nInputType][j];
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, TRUE);

						int iSaveStartIndex = m_iCheckGoNoGoEndNum - j;
						GoNoGoSaveAvailability(bIsSpecOut, iSaveStartIndex);
					}
				}

				m_iCheckGoNoGoStartNum = m_iCheckGoNoGoEndNum;
				CStatus::Instance()->SetIsYGoNoGo(FALSE);
			}
		}
	}
	if (m_GraphType == "Z")
	{
		if (m_iTempGoNoGoEnd != m_iCheckGoNoGoEndNum)
		{
			if (!CStatus::Instance()->GetIsZGoNoGo())
			{
				CStatus::Instance()->SetIsZGoNoGo(TRUE);
				m_iCheckGoNoGoEndNum = m_iTempGoNoGoEnd;

				if (CStatus::Instance()->m_EventMode == CStatus::Evt_GONOGO)
				{
					int nEndIndex = abs(m_iCheckGoNoGoEndNum - m_iCheckGoNoGoStartNum);
					for (int j = 0; j < nEndIndex; j++)
					{
						double	dblValue = m_pdblFilteredData[nInputType][j];
						calcValue = CalcDataByTimeDomainUnit(dblMaxPos, dblValue, strTimeDomainUnit);
						BOOL bIsSpecOut = CheckGoNoGo(calcValue.RealValue, TRUE);

						int iSaveStartIndex = m_iCheckGoNoGoEndNum - j;
						GoNoGoSaveAvailability(bIsSpecOut, iSaveStartIndex);
					}
				}

				m_iCheckGoNoGoStartNum = m_iCheckGoNoGoEndNum;
				CStatus::Instance()->SetIsZGoNoGo(FALSE);
			}
		}
	}
}

double CDrawingHelper::Scroll_CalcValue(CString strTimeUnit)
{
	int iScale = _ttoi(CStatus::Instance()->m_strGraphScale);
	double dblScrollMoveAmount = 0;
	if (strTimeUnit == "g-Force")
	{
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE1_MODE)
		{
			double dblMoveAmount = 2. * (double)m_nStepMove / SCROLL_RATIO;
			double dblScale = 2. / (double)iScale;
			dblScrollMoveAmount = (double)m_graphRect.Height() / dblScale * dblMoveAmount / 2.;
		}
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE2_MODE)
		{
			double dblMoveAmount = 3. * (double)m_nStepMove / SCROLL_RATIO;
			double dblScale = 3. / (double)iScale;
			dblScrollMoveAmount = (double)m_graphRect.Height() / dblScale * dblMoveAmount / 2.;
		}
	}
	if (strTimeUnit == "GAL")
	{
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE1_MODE)
		{
			double dblMoveAmount = 1962. * (double)m_nStepMove / SCROLL_RATIO;
			double dblScale = 1962. / (double)iScale;
			dblScrollMoveAmount = (double)m_graphRect.Height() / dblScale* dblMoveAmount / 2.;
		}
		if (CStatus::Instance()->GetViewRangeMode() == CStatus::tViewRangeMode::RANGE_TYPE2_MODE)
		{
			double dblMoveAmount = 2943. * (double)m_nStepMove / SCROLL_RATIO;
			double dblScale = 2943. / (double)iScale;
			dblScrollMoveAmount = (double)m_graphRect.Height() / dblScale* dblMoveAmount / 2.;
		}
	}
	return dblScrollMoveAmount;
}

void CDrawingHelper::AllFilterDataClear(int nInputType)
{
	if (nInputType == 0)
	{
		if (m_pdblFilteredData[nInputType] != NULL)
		{
			delete[] m_pdblFilteredData[nInputType];
			m_pdblFilteredData[nInputType] = NULL;
		}
		if (CStatus::Instance()->GetRepository()->GetDataCount(1) != 0)
		{
			if (m_GraphType == "RMS")
			{
				if (m_pdblFilterCompareData[0] != NULL)
				{
					delete[] m_pdblFilterCompareData[0];
					m_pdblFilterCompareData[0] = NULL;
				}
			}
			if (m_GraphType == "X")
			{
				if (m_pdblFilterCompareData[1] != NULL)
				{
					delete[] m_pdblFilterCompareData[1];
					m_pdblFilterCompareData[1] = NULL;
				}
			}
			if (m_GraphType == "Y")
			{
				if (m_pdblFilterCompareData[2] != NULL)
				{
					delete[] m_pdblFilterCompareData[2];
					m_pdblFilterCompareData[2] = NULL;
				}
			}
			if (m_GraphType == "Z")
			{
				if (m_pdblFilterCompareData[3] != NULL)
				{
					delete[] m_pdblFilterCompareData[3];
					m_pdblFilterCompareData[3] = NULL;
				}
			}
		}
	}
	if (nInputType == 1)
	{
		if (m_pdblFilteredData[nInputType] != NULL)
		{
			delete[] m_pdblFilteredData[nInputType];
			m_pdblFilteredData[nInputType] = NULL;
		}
		for (int i = 0; i < 4; i++)
		{
			if (m_pdblFilterCompareData[i] != NULL)
			{
				if (m_pdblFilterCompareData[i] != NULL)
				{
					delete[] m_pdblFilterCompareData[i];
					m_pdblFilterCompareData[i] = NULL;
				}
			}
		}
	}
}

double CDrawingHelper::GetFilterData(int nIndex)
{
	double dblValue = m_pdblFilteredData[0][nIndex];
	return dblValue;
}

void CDrawingHelper::ReviewAllDataFilter(int nInputType)
{
	//LONG lStartTime = ::GetTickCount();
	int iLowValue = CStatus::Instance()->m_iFilterLowValue;
	int iHighValue = CStatus::Instance()->m_iFilterHighValue;

	double dblTimeUnit = CStatus::Instance()->m_dblUnit;		//ex 0.1	
	int nFrequeny = (int)(CStatus::Instance()->GetDeviceFrequency());
	double dblOneRectDrawNum = (double)nFrequeny * dblTimeUnit; // 가변 되야함 한칸당 몇개씩 그릴거냐
	int iDrawStartPos = 0;
	int iDrawEndPos = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
	int iDrawDataCount = CStatus::Instance()->m_nDrawPointNum;
	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
	int iOverlapCount = (int)(dblOneRectDrawNum / (double)ONERECT_PIXEL);

	CFilterIIR iiR;
	tAVMDData NewData;
	
	if (CStatus::Instance()->m_bIsThreadStop == TRUE)
		return;

	if (nInputType == 0)
	{
		double* dblOrgData = new double[nDataCount];
		m_pdblFilteredData[nInputType] = new double[nDataCount];
		int nCompareDataCount = CStatus::Instance()->GetRepository()->GetDataCount(1);
		if (m_GraphType == "RMS")
		{
			if(CStatus::Instance()->GetRepository()->GetDataCount(1) != 0)
				m_pdblFilterCompareData[0] = new double[nCompareDataCount];

			for (int i = iDrawStartPos; i <iDrawEndPos; i++)
			{
				CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
				CalcRMS(&NewData);
				dblOrgData[i] = (double)NewData.iRMSValue;
			}
		}
		if (m_GraphType == "X")
		{
			if (CStatus::Instance()->GetRepository()->GetDataCount(1) != 0)
				m_pdblFilterCompareData[1] = new double[nCompareDataCount];

			for (int i = iDrawStartPos; i <iDrawEndPos; i++)
			{
				CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
				dblOrgData[i] = (double)NewData.iXValue;
			}
		}
		if (m_GraphType == "Y")
		{
			if (CStatus::Instance()->GetRepository()->GetDataCount(1) != 0)
				m_pdblFilterCompareData[2] = new double[nCompareDataCount];

			for (int i = iDrawStartPos; i <iDrawEndPos; i++)
			{
				CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
				dblOrgData[i] = (double)NewData.iYValue;
			}
		}
		if (m_GraphType == "Z")
		{
			if (CStatus::Instance()->GetRepository()->GetDataCount(1) != 0)
				m_pdblFilterCompareData[3] = new double[nCompareDataCount];

			for (int i = iDrawStartPos; i <iDrawEndPos; i++)
			{
				CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
				dblOrgData[i] = (double)NewData.iZValue;
			}
		}
		if (CStatus::Instance()->m_bIsLowPassFilter)
		{
			if (iiR.Init(nFrequeny, iHighValue, CFilterIIR::FT_LOWPASS, 6))AfxMessageBox(_T("Err"));
			iiR.DoFilter(dblOrgData, m_pdblFilteredData[nInputType], nDataCount);
		}
		if (CStatus::Instance()->m_bIsHighPassFilter)
		{
			if (iiR.Init(nFrequeny, iLowValue, CFilterIIR::FT_HIGHPASS, 6))AfxMessageBox(_T("Err"));
			iiR.DoFilter(dblOrgData, m_pdblFilteredData[nInputType], nDataCount);
		}
		if (CStatus::Instance()->m_bIsBandPassFilter)
		{
			if (iiR.Init(nFrequeny, iLowValue, CFilterIIR::FT_HIGHPASS, 6))AfxMessageBox(_T("Err"));
			iiR.DoFilter(dblOrgData, m_pdblFilteredData[nInputType], nDataCount);
			iiR.Free();
			if (iiR.Init(nFrequeny, iHighValue, CFilterIIR::FT_LOWPASS, 6))AfxMessageBox(_T("Err"));
			iiR.DoFilterInThePlace(m_pdblFilteredData[nInputType], nDataCount);
		}
		delete[] dblOrgData;
	}
	
	if (nInputType == 1)
	{
		double* dblRMSOrgData = new double[nDataCount];
		double* dblXOrgData = new double[nDataCount];
		double* dblYOrgData = new double[nDataCount];
		double* dblZOrgData = new double[nDataCount];
		for (int i = 0; i < 4; i++)
		{
			m_pdblFilterCompareData[i] = new double[nDataCount];
		}
		for (int i = iDrawStartPos; i <iDrawEndPos; i++)
		{
			CStatus::Instance()->GetRepository()->GetAtData(&NewData, i, nInputType);
			CalcRMS(&NewData);
			dblRMSOrgData[i] = (double)NewData.iRMSValue;
			dblXOrgData[i] = (double)NewData.iXValue;
			dblYOrgData[i] = (double)NewData.iYValue;
			dblZOrgData[i] = (double)NewData.iZValue;
		}
		
		if (CStatus::Instance()->m_bIsLowPassFilter)
		{
			if (iiR.Init(nFrequeny, iHighValue, CFilterIIR::FT_LOWPASS, 6))AfxMessageBox(_T("Err"));
			iiR.DoFilter(dblRMSOrgData, m_pdblFilterCompareData[0], nDataCount);
			iiR.Free();
			iiR.DoFilter(dblXOrgData, m_pdblFilterCompareData[1], nDataCount);
			iiR.Free();
			iiR.DoFilter(dblYOrgData, m_pdblFilterCompareData[2], nDataCount);
			iiR.Free();
			iiR.DoFilter(dblZOrgData, m_pdblFilterCompareData[3], nDataCount);
		}

		if (CStatus::Instance()->m_bIsHighPassFilter)
		{
			if (iiR.Init(nFrequeny, iLowValue, CFilterIIR::FT_HIGHPASS, 6))AfxMessageBox(_T("Err"));
			iiR.DoFilter(dblRMSOrgData, m_pdblFilterCompareData[0], nDataCount);
			iiR.Free();
			iiR.DoFilter(dblXOrgData, m_pdblFilterCompareData[1], nDataCount);
			iiR.Free(); 
			iiR.DoFilter(dblYOrgData, m_pdblFilterCompareData[2], nDataCount);
			iiR.Free(); 
			iiR.DoFilter(dblZOrgData, m_pdblFilterCompareData[3], nDataCount);
		}
		if (CStatus::Instance()->m_bIsBandPassFilter)
		{
			if (iiR.Init(nFrequeny, iLowValue, CFilterIIR::FT_HIGHPASS, 6))AfxMessageBox(_T("Err"));
			iiR.DoFilter(dblRMSOrgData, m_pdblFilterCompareData[0], nDataCount);
			iiR.Free();
			iiR.DoFilter(dblXOrgData, m_pdblFilterCompareData[1], nDataCount);
			iiR.Free();
			iiR.Free();iiR.DoFilter(dblYOrgData, m_pdblFilterCompareData[2], nDataCount);
			iiR.Free();
			iiR.DoFilter(dblZOrgData, m_pdblFilterCompareData[3], nDataCount);
			iiR.Free();

			if (iiR.Init(nFrequeny, iHighValue, CFilterIIR::FT_LOWPASS, 6))AfxMessageBox(_T("Err"));
			iiR.DoFilterInThePlace(m_pdblFilterCompareData[0], nDataCount);
			iiR.Free();
			iiR.DoFilterInThePlace(m_pdblFilterCompareData[1], nDataCount);
			iiR.Free();
			iiR.DoFilterInThePlace(m_pdblFilterCompareData[2], nDataCount);
			iiR.Free();
			iiR.DoFilterInThePlace(m_pdblFilterCompareData[3], nDataCount);
		}
		CAVMDMonitorView *view = CStatus::Instance()->GetAVMDView();

		if (m_pdblFilterCompareData[0] != NULL && view->m_pGraphRMSDlg->m_drawHelper.m_pdblFilterCompareData[0] != NULL)
		{
			view->m_pGraphRMSDlg->m_drawHelper.m_pdblFilterCompareData[0] = new double[nDataCount];
			memcpy(view->m_pGraphRMSDlg->m_drawHelper.m_pdblFilterCompareData[0], m_pdblFilterCompareData[0], nDataCount);
		}
		if (m_pdblFilterCompareData[1] != NULL && view->m_pGraphXDlg->m_drawHelper.m_pdblFilterCompareData[1] != NULL)
		{
			view->m_pGraphXDlg->m_drawHelper.m_pdblFilterCompareData[1] = new double[nDataCount];
			memcpy(view->m_pGraphXDlg->m_drawHelper.m_pdblFilterCompareData[1], m_pdblFilterCompareData[1], nDataCount);
		}
		if (m_pdblFilterCompareData[2] != NULL && view->m_pGraphYDlg->m_drawHelper.m_pdblFilterCompareData[2] != NULL)
		{
			view->m_pGraphYDlg->m_drawHelper.m_pdblFilterCompareData[2] = new double[nDataCount];
			memcpy(view->m_pGraphYDlg->m_drawHelper.m_pdblFilterCompareData[2], m_pdblFilterCompareData[2], nDataCount);
		}
		if (m_pdblFilterCompareData[3] != NULL && view->m_pGraphZDlg->m_drawHelper.m_pdblFilterCompareData[3] != NULL)
		{
			view->m_pGraphZDlg->m_drawHelper.m_pdblFilterCompareData[3] = new double[nDataCount];
			memcpy(view->m_pGraphZDlg->m_drawHelper.m_pdblFilterCompareData[3], m_pdblFilterCompareData[3], nDataCount);
		}
	
		delete[] dblRMSOrgData;
		delete[] dblXOrgData;
		delete[] dblYOrgData;
		delete[] dblZOrgData;
	}
	iiR.Free();
	//LONG lEndTime = ::GetTickCount();
	//printf("%d\n", lEndTime - lStartTime);
}

BYTE CDrawingHelper::ColorTruncate(BYTE in, int iValue)
{
	int rv = in + iValue;
	rv = (rv < 0) ? 0 :
		(rv > 255) ? 255 : rv;
	return (BYTE)rv;
}
Color CDrawingHelper::BrightnessChange(Color curInput, int iValue)
{
	BYTE byBlue = curInput.GetB();
	BYTE byRed = curInput.GetR();
	BYTE byGreen = curInput.GetG();

	byRed = ColorTruncate(byRed, iValue);
	byGreen = ColorTruncate(byGreen, iValue);
	byBlue = ColorTruncate(byBlue, iValue);

	Color rv;
	rv.SetFromCOLORREF(RGB(byRed, byGreen, byBlue));
	return rv;
}

int CDrawingHelper::GetFilterDataSize(CString strGraphType)
{
	//m_pdblFilteredData[nInputType][nIndex];
	int size = 0;
	size = sizeof(m_pdblFilteredData[1]);
	//printf("%s : %d\n", strGraphType, size);
	return size;
}