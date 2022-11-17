#pragma once
#include "stdafx.h"
#include "Repository.h"
#include "Status.h"
#include <deque>
#include "FilterIIR.h"

class CDrawingHelper
{
private:
	struct Complex {
		double Re;	//실수
		double Im;	//허수
	};
	struct tCalcValue
	{
		double RealValue;	// 단위에 따른 계산 결과
		double AdjustedValue;	//그래프 크기에 맞게 계산된 결과
	};
	struct tMarkCheckInfo
	{
		BOOL bIsMark;
		CString strMark;
	};
	struct tOverlayInfo
	{
		double* pOverlayRepository;
		int nMinIndex;
		int nMaxIndex;
	};
	struct tCalcInfo
	{
		int nMaxValue;
		int nMinValue;
	};
	deque<tOverlayInfo> m_pOverlayRepository;
	double* m_pOverlayTempRepository;
	CAVMDMonitorView *m_pMonitorView;
	CDC *m_pDC;
	//HDC m_hDC;
	//Graphics *m_pGraphics;
	double nVerticalInterval;
	double nFftVerticalInterval;
	double nHorizontalInterval;
	double m_dblOneRectInterval;

	int m_iInterval;
	int iTempRePosX;
	int m_nStepMove;
	
	CString m_GraphType;

	CRect m_alarmRect;
	CRect m_clientRect;
	CRect m_timeRect;

	PointF m_textPt;
	Point m_prevPt;
	Point m_TempPt;
	bool m_bIsFFT;
	bool m_bIsFirst;
public:
	CRect m_graphRect;
	struct tReportFFTResult
	{
		BOOL bIsFFTFail;
		int iMaxHz;
		double dblMaxValue;
	};

	CDrawingHelper();
	~CDrawingHelper();
	
	CRepository *m_pRepository;

	void SetDC(CDC *pDC);
	void InitBackGround(CRect dlgRect, CString strGraphType, int nInputType);
	void InitFFTBackGround(CRect dlgRect, CString strGraphType, int nInputType);
	void SetDeegreeDlgRect(CRect dlgRect);
	void UnitofMeasure(BOOL bIsFFT, CRect dlgRect, int nInputType);
	void DrawingGraph(int nInputType =0);
	void SetGraphRectangleArea(CRect dlgRect);
	void SetReviewGraphRectangleArea(CRect dlgRect);
private:
	void SetFFTGraphRectangleArea(CRect dlgRect);
	
	void DrawingString(CString strGraphType, int nInputType);
public:
	double CalcRMS(tAVMDData *pAPMDData);
	void DrawFFTGraph(CPoint fftMeasurePos, int nInputType, int nPriority, BOOL bIsOverColor);

private:
	void OnFft1d(Complex *X, int N, int Log2N);
	void OnIfft2d(int nRepeatNum, int nInputType);
	void OnIfft1d(Complex *X, int N, int Log2N);
	void OnShuffle(Complex *X, int N, int Log2N);
	void OnButterfly(Complex *X, int N, int Log2N, int mode);
	int OnReverseBitOrder(int index, int Log2N);
	Complex *m_pFFTData[2]; // 0 : Org Gragh, 1: Compare Graph
	Complex *m_pIFFTData[2];
	Complex *m_pFFTGraphData[2]; // FFT 그래프 쪽
public:
	int OnFft2d(int nInputType);
	int OnReviewFft2d(int nInputType);
	void OnFilterFft2d(int iTotalLog2N, int nDataCount, int iStartPos=0, int nInputType=0);
private:
	void DrawFilteredGraph(int nInputType, int nDataEnd, int nFilterSize);
	BOOL m_bCalcValueReEnter;
	BOOL CheckTrigger(double dblData, BOOL bIsMarkAdd);
	void AlarmRectColor(BOOL bIsSpecOut, CStatus::tEventMode eventMode);
public:
	
private:
	tCalcValue CalcDataByTimeDomainUnit(double dblMaxPos, double dblData, CString strTimeDomainUnit);
public:
	void DrawSpecLine(CStatus::tEventMode eventMode);
private:
	void DrawTriggerLine();
	void DrawGoNoGoLine();
	BOOL CheckGoNoGo(double dblData, BOOL bIsMarkAdd);
private:
	int m_iOverlapToTalCount;
	double m_dblOverlapToTalSum;
	int m_iDragAmount;
public:
	void DrawingReviewGraph(int nInputType, int nPriority, BOOL bIsOverColor, BOOL bIsDrawString); // nPriority 0: Org Graph가 우선, 1: Compare Graph가 우선
	void DrawReviewFilteredGraph(int nInputType, int nPriority, BOOL bIsOverColor, BOOL bIsDrawString);
private:
	void DrawReviewUnit(int iNum, int newPt_X, int nInputType);
	
private:
	int m_iCheckTriggerStartNum;
	int m_iCheckGoNoGoStartNum;
	int m_iCheckTriggerEndNum;
	int m_iCheckGoNoGoEndNum;
	int m_iTempTriggerEnd;
	int m_iTempGoNoGoEnd;
	void InspectTrigger(int iTempDataCount, int nInputType);
	void TriggerSaveAvailability(BOOL bIsTrigger, int nIndex);
	void GoNoGoSaveAvailability(BOOL bIsGoNoGo, int nIndex);
	void InspectGoNoGo(int iTempDataCount, int nInputType);
	int m_iGoNoGoCount;
public:
	void CalcUnitPt(double dblTempGraphPosWidth, int iDrawStartNum, int iDrawEndNum, int nInputType, int nPriority);
private:
	tMarkCheckInfo MarkCheckFromIndex(int nIndex, int nInputType, int nPriority);
	void CalcMarkPt(double dblTempGraphPosWidth, int iDrawStartNum, int iDrawEndNum, int nInputType, int nPriority);
	int m_iNoGoIndex;
public:
	tReportFFTResult ReportFFT(CString strGraphType,int iDrawStartNum);
private:
	BOOL m_bIsInNoGoSecond;
	CString m_strGoNoGoGraphType;
public:
	void DrawingMonitoringValue(int nIndex, double dblValue);
	void DrawIndicator();
	void SetDrawPointNum();
	void InspectFilterTrigger(int iDataEndNum, int nInputType);
	void InspectFilterGoNoGo(int iDataEndNum, int nInputType);
	int m_iHoldStart;
	int m_iHoldEnd;
	int m_iHoldFFTStartNum;
	int m_iFFTReviewStartNum;
	double m_dblPrevTimeUnit;
	int m_iFFTDrawAmount;
	BOOL m_bChange;
	double Scroll_CalcValue(CString strTimeUnit);
	BOOL m_bPrevTriggerSpecOut;

public:
	void AllFilterDataClear(int nInputType);
	double GetFilterData(int nIndex);
	void ViewFilter(int nInputType=0);
	double* m_pdblFilteredData[2];	// 0 : Org, 1 : Compare
	double* m_pdblFilterCompareData[4]; // 0 : RMS , 1 : X, 2: Y, 3: Z
	int OnFilterFft2d(int nInputType);
	void ReviewAllDataFilter(int nInputType);
	int OnReviewFilterFft2d(int nInputType);
	BYTE ColorTruncate(BYTE in, int iValue);
	Color BrightnessChange(Color curInput, int iValue);
	int GetFilterDataSize(CString strGraphType);
	void DrawOverlayGraph(BOOL bIsOverColor);
	void DrawViewFilterOverlayGraph(BOOL bIsOverColor);
	tCalcInfo CalcOverlayTendencyDelete(double *dblSrcArray, int iValue, int nMinIndex, int nMaxIndex);
	int CalcCutOffValue(double dblTimeUnit);
};
