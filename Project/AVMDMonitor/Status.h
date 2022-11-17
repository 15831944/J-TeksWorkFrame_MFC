#pragma once
#include "stdafx.h"
#include "Repository.h"
#include "atltypes.h"

class CAVMDMonitorView;
class CAVMDDisplayDlg;
class CAVMDReview;
class CAVMDViewDlg;

struct tMarkInfo;

class CStatus
{
public:
		enum tProgramMode { PM_MONITORING, PM_REVIEW };
		enum tEventMode {Evt_NONE, Evt_GONOGO, Evt_TRIGGER};
		enum tProgressBarMode { NONE, EXPORT_EXCEL, PRATICAL_SAVE, SAVE_AS, FILTER, REPORT, COMPARE_FILTER };
		enum tViewRangeMode { RANGE_TYPE1_MODE = 2, RANGE_TYPE2_MODE = 3};   //Type1(-1962~1962) 2G , Type2(-2943~2943) 3G
		enum tDevice_Frequency { Five_HUNDRED = 500, ONE_THOUSAND =1000 };
		struct tUnitTextInfo
		{
			int iTextPt;
			CString strUnit;
		};
		
private:

	deque<tUnitTextInfo > m_iTextPt_X[2];
	static BOOL m_bIsMinimizeRMS;
	static BOOL m_bIsMinimizeX;
	static BOOL m_bIsMinimizeY;
	static BOOL m_bIsMinimizeZ;
	static BOOL m_bIsChangeMinimize;

	static BOOL m_bFFTShowStatus;

	int m_iGraphHeight;
	int m_iMinimizeNum;


	BOOL m_bIsReviewReset_RMS;
	BOOL m_bIsReviewReset_X;
	BOOL m_bIsReviewReset_Y;
	BOOL m_bIsReviewReset_Z;

public:
	CStatus();
	~CStatus();


	
	static tProgramMode m_ProgramMode;
	static tProgramMode GetProgramMode();
	static void SetProgramMode(tProgramMode ProgramMode);

	static tViewRangeMode m_viewRangeMode;
	static tViewRangeMode GetViewRangeMode();

	static tDevice_Frequency m_DeviceFrequency;
	static tDevice_Frequency GetDeviceFrequency();

	static tProgressBarMode m_ProgressBarMode;
	static tProgressBarMode GetProgressBarMode();
	static void SetProgressBarMode(tProgressBarMode ProgressBarMode);

	static tEventMode m_EventMode;
	static tEventMode GetEventMode();
	static void SetEventMode(tEventMode EventMode);

	static BOOL m_bRMSReEnter;
	static BOOL m_bXReEnter;
	static BOOL m_bYReEnter;
	static BOOL m_bZReEnter;
	static BOOL m_bCompareReEnter;

	static BOOL m_bTimeDlgReEnter;
	static BOOL m_bFFTTimeDlgReEnter;

	static BOOL m_bFftRMSReEnter;
	static BOOL m_bFftXReEnter;
	static BOOL m_bFftYReEnter;
	static BOOL m_bFftZReEnter;
	static BOOL m_bFFtCompareReEnter;

	static BOOL m_bAllReEnter;

	static BOOL m_bIsLowPassFilter;
	static BOOL m_bIsHighPassFilter;
	static BOOL m_bIsBandPassFilter;

	static CString m_FFT_SampleSize;
	static int m_iverticalNum;
	static int m_iFfftVerticalNum;
	static int m_ihorizontalNum;

	static CString m_strGraphScale;
	static Point prevPt;
	static CStatus *m_pInstance;
	static void SetMinimize(BOOL bChkRMSStatus, BOOL bChkXStatus, BOOL bChkYStatus, BOOL bChkZStatus);
	static void SetShowFFTGraph(BOOL bChkStatus);
	static void SetFilterType(BOOL bLowPass, BOOL bHighPass, BOOL bBandPass);
	static CStatus* Instance();

	static CAVMDMonitorView *m_pAVMDMonitorView;
	static CAVMDDisplayDlg *m_pAVMDDisplayDlg;
	static CAVMDReview *m_pAVMDReviewDlg;
	static CAVMDViewDlg *m_pAVMDViewDlg;

	BOOL IsMinimizeRMS();
	BOOL IsMinimizeX();
	BOOL IsMinimizeY();
	BOOL IsMinimizeZ();
	BOOL IsFFTShow();

	CString m_strDegree;
	CString m_strFFTDegree;
	CString m_strCalDate;
	CString m_strSerialNo[2];
	CString m_strComportNo;
	CString m_strDataSaveFolder;
	CString m_strDataFileName;

	CString m_strFFTScale;

	double m_dblUnit;
	double m_dblThresTemp;
	int m_iFFTUnit;
	int m_iFFTShowRatio;
	int m_iFFTShowStartPos;
	int m_iFFTShowTempStartPos;
	long m_iUseWarning;
	long m_iUseTemp;
	long m_iTimeWarningSel;
	long m_iMaxMarkInExlForm;
	// 폴더를 제외한 이름 확장자는 자동생성
	
	tAVMDRecord m_curMeasureData;

	int		m_iTemperature;	//-40~100
	BYTE	m_byBattery;		//0~100
	BYTE	m_nyNum;			//0~255
	int		m_iXValue;		//-1962~1962
	int		m_iYValue;		//-1962~1962
	int		m_iZValue;		//-1962~1962
	int		m_iRMSValue;
	int m_iWaferSize;
	long m_iBaudRate;

	int m_iRMSStandardValue;
	int m_iXStandardValue;
	int m_iYStandardValue;
	int m_iZStandardValue;

	int m_nDrawPointNum;

	int m_iFilterLowValue;
	int m_iFilterHighValue;
	
	CString m_strFilterStatus;
	CString m_strTimeDomainUnit;

	COLORREF m_RMSPenColor;
	COLORREF m_XPenColor;
	COLORREF m_YPenColor;
	COLORREF m_ZPenColor;

	COLORREF m_SpectInColor;
	COLORREF m_SpectOutColor;
	COLORREF m_TriggerColor;
	COLORREF m_GoNoGoColor;
	COLORREF m_OverlayColor;

	COLORREF m_MarkColor;
	COLORREF m_IndicatorColor;

	double m_dblGoNoGoAbove_RMS_Value;
	double m_dblGoNoGoAbove_X_Value;
	double m_dblGoNoGoAbove_Y_Value;
	double m_dblGoNoGoAbove_Z_Value;

	double m_dblGoNoGoBelow_X_Value;
	double m_dblGoNoGoBelow_Y_Value;
	double m_dblGoNoGoBelow_Z_Value;
	long m_iGoNoGo_QualityTime;

	double m_dblTriggerAbove_RMS_Value;
	double m_dblTriggerAbove_X_Value;
	double m_dblTriggerAbove_Y_Value;
	double m_dblTriggerAbove_Z_Value;

	double m_dblTriggerBelow_X_Value;
	double m_dblTriggerBelow_Y_Value;
	double m_dblTriggerBelow_Z_Value;

	BOOL m_bIsGoNoGoAbove_RMS;
	BOOL m_bIsGoNoGoAbove_X;
	BOOL m_bIsGoNoGoAbove_Y;
	BOOL m_bIsGoNoGoAbove_Z;
	BOOL m_bIsGoNoGoBelow_X;
	BOOL m_bIsGoNoGoBelow_Y;
	BOOL m_bIsGoNoGoBelow_Z;

	BOOL m_bIsTriggerAbove_RMS;
	BOOL m_bIsTriggerAbove_X;
	BOOL m_bIsTriggerAbove_Y;
	BOOL m_bIsTriggerAbove_Z;
	BOOL m_bIsTriggerBelow_X;
	BOOL m_bIsTriggerBelow_Y;
	BOOL m_bIsTriggerBelow_Z;


	void SetAVMDView(CAVMDMonitorView * pAVMDMonitorView);
	CAVMDMonitorView* GetAVMDView();
	void SetAVMDDisPlayView(CAVMDDisplayDlg * pAVMDDisplayDlg);
	int MinimizeNum();
	void SetWaferSize(int sizeWafer);
	CString GetComportNo();
	void SetBaudRate(int iBaudRate);
	int GetBaudRate();
	void SetComportNo(TCHAR* szComportNo);
	void SetMeasureData(tAVMDData *pCurData);
	void SetUnit(CString Degree, double unit, BOOL bIsFFT);
	int GetDefault();
	void SetDefault();
	BOOL GetReEnter();
	void SpectrumComboBoxUpdate();
private:
	int m_iAmountofMousemovement[2];
	Color RGBToColor(long iRValue, long iGValue, long iBValue);
		static CRepository *m_pRepository;

public:
	CString GetDataSaveFolder();
	void SetDataSaveFolder(TCHAR * szDataSaveFolder);
	CString GetDataFileName();
	void SetDataFileName(TCHAR * szDataFileName);
	void SetWorkEnvInfoToAdd(tWorkEnvInfo * pMarkToAdd);
	tWorkEnvInfo m_WorkEnvInfoToAdd;
	CRepository * GetRepository();
	void SetRepository(CRepository * pRepository);
	void DisplayDlgUpdateUI();
	void UpdateScrn();
	void GraphUpdate();
	void SetAmountofMouseMovement(int mouseMovement, int nInputType);
	int GetAmountofMouseMovement(int nInputType);
	void SetUnitTextPtX(int iPointX, CString strUnit, int nInputType);
	tUnitTextInfo GetUnitTextPtX(int nInputType);
	int GetUnitTextPtXCount(int nInputType);
private:
	int m_iReviewDrawStart[2];
public:
	void SetReviewDrawStart(int iDrawStartNum, int nInputType);
	int GetReviewDrawStart(int nInputType);
private:
	int m_iReviewDrawEnd[2];
public:
	void SetReviewDrawEnd(int iDrawEndNum, int nInputType);
	int GetReviewDrawEnd(int nInputType);
	void TimeUnitInitialize();
	void ReviewDlgUpdate();
	void SetAVMDReView(CAVMDReview * pAVMDReviewDlg);
	void SetAVMDViewDlg(CAVMDViewDlg * pAVMDViewDlg);
private:
	double m_dblDragAmount[2]; // 0 : Org Graph, 1:Compare Graph
public:
	void SetDragAmount(double iDragAmount, int nInputType);
	double GetDragAmount(int nInputType);
	void SetOnePixelDrawCount(double dblOnePixelDrawCount);
private:
	double m_dblOnePixelDrawCount;
public:
	void SetReviewDragReset(int nInputType);
private:
	int m_nOverlapPixel[2];
public:
	void SetOverlapPixelCount(int nOverlapPixel, int nInputType);
	double GetOnePixelDrawCount();
	void ClearUnitTextPtX(int nInputType);
public:
	void StartRecording(int nIndex);
	void FinishRecording();
private:
	int m_bIsSave;
public:
	void SetIsSepcOutSave(BOOL bIsSave);
	BOOL GetIsSepcOutSave();
	BOOL m_bTest;
private:
	BOOL m_bIsRMSTrigger;
	BOOL m_bIsXTrigger;
	BOOL m_bIsYTrigger;
	BOOL m_bIsZTrigger;

	BOOL m_bIsRMSGoNoGo;
	BOOL m_bIsXGoNoGo;
	BOOL m_bIsYGoNoGo;
	BOOL m_bIsZGoNoGo;
public:
	BOOL GetIsRMSTrigger();  //검사 중인가 아닌가 
	void SetIsRMSTrigger(BOOL bIsTrigger);

	BOOL GetIsXTrigger();
	void SetIsXTrigger(BOOL bIsTrigger);

	BOOL GetIsYTrigger();
	void SetIsYTrigger(BOOL bIsTrigger);

	BOOL GetIsZTrigger();
	void SetIsZTrigger(BOOL bIsTrigger);

	void TriggerReset();

	BOOL GetIsRMSGoNoGo();  //검사 중인가 아닌가 
	void SetIsRMSGoNoGo(BOOL bIsGoNoGo);

	BOOL GetIsXGoNoGo();
	void SetIsXGoNoGo(BOOL bIsGoNoGo);

	BOOL GetIsYGoNoGo();
	void SetIsYGoNoGo(BOOL bIsGoNoGo);

	BOOL GetIsZGoNoGo();
	void SetIsZGoNoGo(BOOL bIsGoNoGo);

	void GoNoGoReset();
private:
	BOOL m_bIsFirstEvent;
public:
	BOOL GetIsFirstEvent();
	void SetIsFirstEvent(bool bIsFirstEvent);
	BOOL IsFilterUse();
private:
	BOOL m_bIsCompare;
public:
	BOOL GetCompareCondition();
	void SetCompareCondition(bool bIsCompare);
private:
	CString m_strCompareType;
public:
	void SetCompareType(CString strCompareType);
	CString GetCompareType();
private:
	BOOL m_bIsOverlay;
public:
	void SetOverlayCondition(bool bIsOverlay);
	BOOL GetOverlayCondition();
	CString m_strMakeDate[2];
	void UpdateReviewComboBox(int nInputType);
	int m_iAlaramRectWidth;
private:
	BOOL m_bIsHold;
public:
	BOOL GetIsHoldCondition();
	void SetIsHoldCondition(bool bIsHold);
	void UpdateViewHoldButton();
	int GetPreRecordTime();
	void GetPreRecordStatus(int &iStateTime, int &iStateUnit);
	int m_iPRStateTime;
	int m_iPRStateUnit;
	void SetPreRecordStatus(int iStateTime, int iStateUnit);
	int m_iNoGoIndicator;
	BOOL m_bSaveActivation;
	CString m_strLastOpenFileName;
	int m_iReportGraphWidth;
	int m_iReportGraphHeight;
	BOOL m_bIsSaveGoNoGo_RMS;
	BOOL m_bIsSaveGoNoGo_X;
	BOOL m_bIsSaveGoNoGo_Y;
	BOOL m_bIsSaveGoNoGo_Z;
	int m_iGoNoGoRecordingStartIndex;
	BOOL m_bIsFilterEnable;
	CString m_strCompareDataFileName;
	int m_iCompareStandardValue;
	int m_iMarkLinePos;
	CString m_strCurLogFile;

	BOOL m_bMarkEditClick;
	BOOL m_bCompareMarkEditClick;
	
	void SetMarkClickEvent(bool bIsClick, int nInputType);
	BOOL GetMarkClickEvent(int nInputType);
	BOOL m_bIsTimeScaleChange;
	int m_iStartPosText;
	BOOL m_bIsCompareTimeScaleChange;
	int m_iCompareStartPosText;
	int m_iDataCount;
	int m_iDataCompareCount;

private:
	int m_iMarkEditStart;
	int m_iCompareMarkEditStart;
public:
	void SetMarkEditStart(int iNewPos, int nInputType);
	int GetMarkEditStart(int nInputType);
	void ExportSetting(CString strSettingFile);
	int GetSettingFrom(CString strSettingFile);
	BOOL m_bIsPlayBack;
	int m_iHoldIndex;
	BOOL m_bIsPlayStop;
	void PlayerStartNFinish(BOOL bIsPlay);
	void ReviewMenuReset();
	void UpdateUITimeDomain();
	CString m_strEventText;
	BOOL m_bSavePreRecData;
	int ReviewDataFiltering();
	void ReviewDataClear(BOOL bIsClearReview, BOOL bIsClearCompare);
	int m_nProgressBarCount;
	static UINT ProcessFilterThread(LPVOID lParam);
	int ReviewCompareDataFiltering();
	static UINT ProcessCompareDataFilterThread(LPVOID lParam);
	int m_iPartialSaveCount;
	BOOL m_bIsThreadStop;
	double GetViewFilteredValue(CString strGraphType, int nInputType, int nIndex);
	int m_nFilterArrayEndIndex;
	BOOL m_bIsFiltering;
	int m_nOverlayDrawPtPixel;
	BOOL m_bIsFFTFullMode;
	BOOL CheckFFTDrawingRange(int nNewPos);
	int FFTDrawingMaxPos();
	int m_iFFTwidth;
	void ReviewComboClear();

	int m_iDataRangle;
};