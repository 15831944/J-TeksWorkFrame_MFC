#include "stdafx.h"
#include "Status.h"
#include "AVMDDefine.h"
#include "AVMDMonitorDoc.h"
#include "AVMDMonitorView.h"
#include "Repository.h"
#include "AVMDDisplayDlg.h"
#include "AVMDReview.h"
#include "AVMDViewDlg.h"
#include "..\Library\UniUtility\UniUtility.h"

CStatus::tProgramMode CStatus::m_ProgramMode = PM_REVIEW;
CStatus::tEventMode CStatus::m_EventMode = Evt_NONE;
CStatus::tProgressBarMode CStatus::m_ProgressBarMode = NONE;
CStatus::tDevice_Frequency CStatus::m_DeviceFrequency = ONE_THOUSAND;
CStatus::tViewRangeMode CStatus::m_viewRangeMode = RANGE_TYPE2_MODE;

//int CStatus::m_nProgressBarCount = 0;
BOOL CStatus::m_bIsMinimizeRMS = FALSE;
BOOL CStatus::m_bIsMinimizeX = FALSE;
BOOL CStatus::m_bIsMinimizeY = FALSE;
BOOL CStatus::m_bIsMinimizeZ = FALSE;
BOOL CStatus::m_bIsChangeMinimize = FALSE;

BOOL CStatus::m_bFFTShowStatus = FALSE;

BOOL CStatus::m_bRMSReEnter = FALSE;
BOOL CStatus::m_bXReEnter = FALSE;
BOOL CStatus::m_bYReEnter = FALSE;
BOOL CStatus::m_bZReEnter = FALSE;
BOOL CStatus::m_bCompareReEnter = FALSE;
BOOL CStatus::m_bTimeDlgReEnter = FALSE;
BOOL CStatus::m_bFFTTimeDlgReEnter = FALSE;

BOOL CStatus::m_bFftRMSReEnter = FALSE;
BOOL CStatus::m_bFftXReEnter = FALSE;
BOOL CStatus::m_bFftYReEnter = FALSE;
BOOL CStatus::m_bFftZReEnter = FALSE;
BOOL CStatus::m_bFFtCompareReEnter = FALSE;

BOOL CStatus::m_bAllReEnter = FALSE;

BOOL CStatus::m_bIsLowPassFilter = FALSE;
BOOL CStatus::m_bIsHighPassFilter = FALSE;
BOOL CStatus::m_bIsBandPassFilter = FALSE;

Point CStatus::prevPt(0,0);
int CStatus::m_iverticalNum = 16;
int CStatus::m_iFfftVerticalNum = 10;
int CStatus::m_ihorizontalNum = 4;
CString CStatus::m_FFT_SampleSize = _T("");
CString CStatus::m_strGraphScale = _T("1");
CStatus* CStatus::m_pInstance = NULL;
CAVMDMonitorView * CStatus::m_pAVMDMonitorView = NULL;
CAVMDDisplayDlg * CStatus::m_pAVMDDisplayDlg = NULL;
CAVMDReview * CStatus::m_pAVMDReviewDlg = NULL;
CAVMDViewDlg * CStatus::m_pAVMDViewDlg = NULL;

CRepository* CStatus::m_pRepository = NULL;

CStatus::CStatus()
	: m_strDataSaveFolder(_T(""))
	, m_strDataFileName(_T(""))
	, m_iFFTUnit(25)
	, m_iFFTShowRatio(1)
	, m_iFFTShowStartPos(0)
	, m_iFFTShowTempStartPos(0)
	, m_dblUnit(0.1)
	, m_iUseWarning(0)
	, m_iUseTemp(0)
	, m_dblThresTemp(0)
	, m_iTimeWarningSel(0)
	, m_iMaxMarkInExlForm(54)
	, m_iRMSStandardValue(0)
	, m_iXStandardValue(0)
	, m_iYStandardValue(0)
	, m_iZStandardValue(0)
	, m_strDegree(_T("Second"))
	, m_strFFTDegree(_T("Hz"))
	, m_nDrawPointNum(0)
	, m_iFilterLowValue(0)
	, m_iFilterHighValue(200)
	, m_strFilterStatus(_T("disable"))
	, m_strTimeDomainUnit(_T("g-Force"))
	, m_strFFTScale(_T("0.025"))
	, m_dblGoNoGoAbove_RMS_Value(0)
	, m_dblGoNoGoAbove_X_Value(0)
	, m_dblGoNoGoAbove_Y_Value(0)
	, m_dblGoNoGoAbove_Z_Value(0)
	, m_dblGoNoGoBelow_X_Value(0)
	, m_dblGoNoGoBelow_Y_Value(0)
	, m_dblGoNoGoBelow_Z_Value(0)
	, m_iGoNoGo_QualityTime(0)
	, m_bIsGoNoGoAbove_RMS(FALSE)
	, m_bIsGoNoGoAbove_X(FALSE)
	, m_bIsGoNoGoAbove_Y(FALSE)
	, m_bIsGoNoGoAbove_Z(FALSE)
	, m_bIsGoNoGoBelow_X(FALSE)
	, m_bIsGoNoGoBelow_Y(FALSE)
	, m_bIsGoNoGoBelow_Z(FALSE)
	, m_dblTriggerAbove_RMS_Value(0)
	, m_dblTriggerAbove_X_Value(0)
	, m_dblTriggerAbove_Y_Value(0)
	, m_dblTriggerAbove_Z_Value(0)
	, m_dblTriggerBelow_X_Value(0)
	, m_dblTriggerBelow_Y_Value(0)
	, m_dblTriggerBelow_Z_Value(0)
	, m_bIsTriggerAbove_RMS(FALSE)
	, m_bIsTriggerAbove_X(FALSE)
	, m_bIsTriggerAbove_Y(FALSE)
	, m_bIsTriggerAbove_Z(FALSE)
	, m_bIsTriggerBelow_X(FALSE)
	, m_bIsTriggerBelow_Y(FALSE)
	, m_bIsTriggerBelow_Z(FALSE)
	, m_bIsReviewReset_RMS(FALSE)
	, m_bIsReviewReset_X(FALSE)
	, m_bIsReviewReset_Y(FALSE)
	, m_bIsReviewReset_Z(FALSE)
	, m_dblOnePixelDrawCount(0)
	, m_bIsSave(0)
	, m_bTest(FALSE)
	, m_bIsRMSTrigger(FALSE)
	, m_bIsXTrigger(FALSE)
	, m_bIsYTrigger(FALSE)
	, m_bIsZTrigger(FALSE)
	, m_bIsRMSGoNoGo(FALSE)
	, m_bIsXGoNoGo(FALSE)
	, m_bIsYGoNoGo(FALSE)
	, m_bIsZGoNoGo(FALSE)
	, m_bIsFirstEvent(TRUE)
	, m_bIsCompare(false)
	, m_strCompareType(_T("X"))
	, m_bIsOverlay(FALSE)
	//, m_strMakeDate(_T(""))
	, m_iAlaramRectWidth(0)
	, m_bIsHold(FALSE)
	, m_iPRStateTime(0)
	, m_iPRStateUnit(0)
	, m_iNoGoIndicator(0)
	, m_bSaveActivation(false)
	, m_strLastOpenFileName(_T(""))
	, m_iReportGraphWidth(0)
	, m_iReportGraphHeight(0)
	, m_bIsSaveGoNoGo_RMS(FALSE)
	, m_bIsSaveGoNoGo_Y(FALSE)
	, m_bIsSaveGoNoGo_Z(FALSE)
	, m_bIsSaveGoNoGo_X(FALSE)
	, m_iGoNoGoRecordingStartIndex(0)
	, m_bIsFilterEnable(FALSE)
	, m_strCompareDataFileName(_T(""))
	, m_iCompareStandardValue(0)
	, m_iMarkLinePos(0)
	, m_strCurLogFile(_T(""))
	, m_bMarkEditClick(FALSE)
	, m_iMarkEditStart(0)
	, m_iCompareMarkEditStart(0)
	, m_bCompareMarkEditClick(FALSE)
	, m_bIsTimeScaleChange(FALSE)
	, m_iStartPosText(0)
	, m_bIsCompareTimeScaleChange(FALSE)
	, m_iCompareStartPosText(0)
	, m_iDataCount(0)
	, m_iDataCompareCount(0)
	, m_bIsPlayBack(FALSE)
	, m_iHoldIndex(0)
	, m_bIsPlayStop(FALSE)
	, m_strEventText(_T(""))
	, m_bSavePreRecData(FALSE)
	, m_nProgressBarCount(0)
	, m_iPartialSaveCount(0)
	, m_bIsThreadStop(FALSE)
	, m_nFilterArrayEndIndex(0)
	, m_bIsFiltering(FALSE)
	, m_nOverlayDrawPtPixel(0)
	, m_bIsFFTFullMode(TRUE)
	, m_iFFTwidth(0)
	, m_iDataRangle(0) // 1962, 2943
	{
	m_iReviewDrawStart[0] = 0;
	m_iReviewDrawStart[1] = 0;
	m_iReviewDrawEnd[0] = 0;
	m_iReviewDrawEnd[1] = 0;
	m_dblDragAmount[0] = 0;
	m_dblDragAmount[1] = 0;
	m_nOverlapPixel[0] = 0;
	m_nOverlapPixel[1] = 0;
	m_iAmountofMousemovement[0] = 0;
	m_iAmountofMousemovement[1] = 0;
	m_strMakeDate[0] = _T("");
	m_strMakeDate[1] = _T("");
}

CStatus::~CStatus()
{
}

CStatus* CStatus::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CStatus;
		m_pRepository = new CRepository;
	}
	return m_pInstance;
}

void CStatus::SetAVMDView(CAVMDMonitorView * pAVMDMonitorView)
{
	m_pAVMDMonitorView = pAVMDMonitorView;
	
}

CAVMDMonitorView* CStatus::GetAVMDView()
{
	return m_pAVMDMonitorView;
}

void CStatus::SetAVMDDisPlayView(CAVMDDisplayDlg * pAVMDDisplayDlg)
{
	m_pAVMDDisplayDlg = pAVMDDisplayDlg;
}

void CStatus::SetMinimize(BOOL bChkRMSStatus, BOOL bChkXStatus, BOOL bChkYStatus, BOOL bChkZStatus)
{
	m_bIsChangeMinimize = TRUE;
	m_bIsMinimizeRMS = bChkRMSStatus;
	m_bIsMinimizeX = bChkXStatus;
	m_bIsMinimizeY = bChkYStatus;
	m_bIsMinimizeZ = bChkZStatus;
	
	m_pAVMDMonitorView->SendMessage(WM_UIUPDATE);
}

void CStatus::SetUnit(CString Degree, double unit, BOOL bIsFFT)
{
	if (bIsFFT)
	{
		m_strFFTDegree = Degree;
		m_iFFTUnit = (int)unit;
	}
	else
	{
		m_strDegree = Degree;
		m_dblUnit = unit;
	}
}

void CStatus::SetShowFFTGraph(BOOL bChkStatus)
{
	m_bFFTShowStatus = bChkStatus;

	//if (m_bFFTShowStatus)	
		//m_iverticalNum = 8;
	//else					
		//m_iverticalNum = 16;
		
	m_pAVMDMonitorView->SendMessage(WM_UIUPDATE);
}

int CStatus::MinimizeNum()
{
	m_iMinimizeNum = (int)m_bIsMinimizeRMS + (int)m_bIsMinimizeX + (int)m_bIsMinimizeY + (int)m_bIsMinimizeZ;
	return (4- m_iMinimizeNum);
}

void CStatus::SetWaferSize(int sizeWafer)
{
	m_iWaferSize = sizeWafer;
}

void CStatus::SetMeasureData(tAVMDData *pCurData)
{
	m_byBattery = pCurData->byBattery;
	m_iTemperature = (int)((signed char)(pCurData->byTemperature));
}

int CStatus::GetDefault()
{
	long iRValue, iGValue, iBValue;
	CCfgFile *pMyCfg = CCfgFile::Instance();
	pMyCfg->Open();
	m_iBaudRate = pMyCfg->Get(9600L, _T("COMSET"), _T("BaudRate"));
	m_strComportNo = pMyCfg->Get(_T("None"), _T("COMSET"), _T("ComportNo"));
	m_strDataSaveFolder = pMyCfg->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	m_strDataFileName = pMyCfg->Get(_T("AVMDLog"), _T("Environment"), _T("DataFileName"));

	m_iUseWarning = pMyCfg->Get(0L, _T("WarningCondition"), _T("UseWarningCondition"));
	m_iUseTemp = pMyCfg->Get(0L, _T("WarningCondition"), _T("UseTemp"));
	m_dblThresTemp = pMyCfg->Get(20.0, _T("WarningCondition"), _T("ThresholdTemp"));
	m_iTimeWarningSel = pMyCfg->Get(0L, _T("WarningCondition"), _T("TimeWarningSel"));
	m_iMaxMarkInExlForm = pMyCfg->Get(40L, _T("ExlReport"), _T("MaxMark"));

	m_DeviceFrequency = (tDevice_Frequency)(pMyCfg->Get(1000L, _T("MODE"), _T("Frequency")));
	m_viewRangeMode = (tViewRangeMode)(pMyCfg->Get(2L, _T("MODE"), _T("ViewRange")));
#pragma region 그래프 색
	iRValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("RMS_Value_R"));
	iGValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("RMS_Value_G"));
	iBValue = pMyCfg->Get(255L, _T("GRAPH_COLOR"), _T("RMS_Value_B"));
	m_RMSPenColor = RGB(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("X_Value_R"));
	iGValue = pMyCfg->Get(128L, _T("GRAPH_COLOR"), _T("X_Value_G"));
	iBValue = pMyCfg->Get(64L, _T("GRAPH_COLOR"), _T("X_Value_B"));
	m_XPenColor = RGB(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(255L, _T("GRAPH_COLOR"), _T("Y_Value_R"));
	iGValue = pMyCfg->Get(128L, _T("GRAPH_COLOR"), _T("Y_Value_G"));
	iBValue = pMyCfg->Get(64L, _T("GRAPH_COLOR"), _T("Y_Value_B"));
	m_YPenColor = RGB(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("Z_Value_R"));
	iGValue = pMyCfg->Get(128L, _T("GRAPH_COLOR"), _T("Z_Value_G"));
	iBValue = pMyCfg->Get(255L, _T("GRAPH_COLOR"), _T("Z_Value_B"));
	m_ZPenColor = RGB(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("Trigger_Value_R"));
	iGValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("Trigger_Value_G"));
	iBValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("Trigger_Value_B"));
	m_TriggerColor = RGB(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("GoNoGo_Value_R"));
	iGValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("GoNoGo_Value_G"));
	iBValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("GoNoGo_Value_B"));
	m_GoNoGoColor = RGB(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("SpecIn_Value_R"));
	iGValue = pMyCfg->Get(128L, _T("GRAPH_COLOR"), _T("SpecIn_Value_G"));
	iBValue = pMyCfg->Get(64L, _T("GRAPH_COLOR"), _T("SpecIn_Value_B"));
	m_SpectInColor = RGB(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(255L, _T("GRAPH_COLOR"), _T("SpecOut_Value_R"));
	iGValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("SpecOut_Value_G"));
	iBValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("SpecOut_Value_B"));
	m_SpectOutColor = RGB(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("Overlay_Value_R"));
	iGValue = pMyCfg->Get(128L, _T("GRAPH_COLOR"), _T("Overlay_Value_G"));
	iBValue = pMyCfg->Get(128L, _T("GRAPH_COLOR"), _T("Overlay_Value_B"));
	m_OverlayColor = RGB(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("Mark_Value_R"));
	iGValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("Mark_Value_G"));
	iBValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("Mark_Value_B"));
	m_MarkColor = RGB(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(0L, _T("GRAPH_COLOR"), _T("Indicator_Value_R"));
	iGValue = pMyCfg->Get(128L, _T("GRAPH_COLOR"), _T("Indicator_Value_G"));
	iBValue = pMyCfg->Get(128L, _T("GRAPH_COLOR"), _T("Indicator_Value_B"));
	m_IndicatorColor = RGB(iRValue, iGValue, iBValue);
#pragma endregion

	m_bSaveActivation = pMyCfg->Get(0L, _T("SAVE"), _T("Save_Activation"));
	m_iReportGraphWidth = pMyCfg->Get(1000L, _T("REPORT"), _T("Report_Graph_width"));
	m_iReportGraphHeight = pMyCfg->Get(400L, _T("REPORT"), _T("Report_Graph_Height"));
#pragma region GoNoGo
	m_dblGoNoGoAbove_RMS_Value = pMyCfg->Get(0L, _T("GONOGO"), _T("Above_RMS"));	//단위 환산 안함
	m_dblGoNoGoAbove_X_Value = pMyCfg->Get(0L, _T("GONOGO"), _T("Above_X"));
	m_dblGoNoGoAbove_Y_Value = pMyCfg->Get(0L, _T("GONOGO"), _T("Above_Y"));
	m_dblGoNoGoAbove_Z_Value = pMyCfg->Get(0L, _T("GONOGO"), _T("Above_Z"));
	m_dblGoNoGoBelow_X_Value = pMyCfg->Get(0L, _T("GONOGO"), _T("Below_X"));
	m_dblGoNoGoBelow_Y_Value = pMyCfg->Get(0L, _T("GONOGO"), _T("Below_Y"));
	m_dblGoNoGoBelow_Z_Value = pMyCfg->Get(0L, _T("GONOGO"), _T("Below_Z"));

	m_iGoNoGo_QualityTime = pMyCfg->Get(0L, _T("GONOGO"), _T("Quality_Time"));
	m_iNoGoIndicator = pMyCfg->Get(5L, _T("GONOGO"), _T("NoGoIndicator_Second"));

	m_bIsGoNoGoAbove_RMS = pMyCfg->Get(0L, _T("GONOGO"), _T("Above_Enable_RMS"));	//단위 환산 안함
	m_bIsGoNoGoAbove_X = pMyCfg->Get(0L, _T("GONOGO"), _T("Above_Enable_X"));
	m_bIsGoNoGoAbove_Y = pMyCfg->Get(0L, _T("GONOGO"), _T("Above_Enable_Y"));
	m_bIsGoNoGoAbove_Z = pMyCfg->Get(0L, _T("GONOGO"), _T("Above_Enable_Z"));
	m_bIsGoNoGoBelow_X = pMyCfg->Get(0L, _T("GONOGO"), _T("Below_Enable_X"));
	m_bIsGoNoGoBelow_Y = pMyCfg->Get(0L, _T("GONOGO"), _T("Below_Enable_Y"));
	m_bIsGoNoGoBelow_Z = pMyCfg->Get(0L, _T("GONOGO"), _T("Below_Enable_Z"));
#pragma endregion

#pragma region Trigger
	m_dblTriggerAbove_RMS_Value = pMyCfg->Get(0L, _T("TRIGGER"), _T("Above_RMS"));	//단위 환산 안함
	m_dblTriggerAbove_X_Value = pMyCfg->Get(0L, _T("TRIGGER"), _T("Above_X"));
	m_dblTriggerAbove_Y_Value = pMyCfg->Get(0L, _T("TRIGGER"), _T("Above_Y"));
	m_dblTriggerAbove_Z_Value = pMyCfg->Get(0L, _T("TRIGGER"), _T("Above_Z"));
	m_dblTriggerBelow_X_Value = pMyCfg->Get(0L, _T("TRIGGER"), _T("Below_X"));
	m_dblTriggerBelow_Y_Value = pMyCfg->Get(0L, _T("TRIGGER"), _T("Below_Y"));
	m_dblTriggerBelow_Z_Value = pMyCfg->Get(0L, _T("TRIGGER"), _T("Below_Z"));

	m_bIsTriggerAbove_RMS = pMyCfg->Get(0L, _T("TRIGGER"), _T("Above_Enable_RMS"));	//단위 환산 안함
	m_bIsTriggerAbove_X = pMyCfg->Get(0L, _T("TRIGGER"), _T("Above_Enable_X"));
	m_bIsTriggerAbove_Y = pMyCfg->Get(0L, _T("TRIGGER"), _T("Above_Enable_Y"));
	m_bIsTriggerAbove_Z = pMyCfg->Get(0L, _T("TRIGGER"), _T("Above_Enable_Z"));
	m_bIsTriggerBelow_X = pMyCfg->Get(0L, _T("TRIGGER"), _T("Below_Enable_X"));
	m_bIsTriggerBelow_Y = pMyCfg->Get(0L, _T("TRIGGER"), _T("Below_Enable_Y"));
	m_bIsTriggerBelow_Z = pMyCfg->Get(0L, _T("TRIGGER"), _T("Below_Enable_Z"));
#pragma endregion
	return 0;
}



int CStatus::GetSettingFrom(CString strSettingFile)
{
	CString strPrevFileName;
	CCfgFile *pCfgFile = CCfgFile::Instance();
	strPrevFileName = pCfgFile->GetPath();
	pCfgFile->SetPath(strSettingFile);
	GetDefault();
	pCfgFile->SetPath(strPrevFileName);
	return 0;
}

void CStatus::SetDefault()
{
	long iRValue, iGValue, iBValue;
	long iTemp = 0;
	CCfgFile *pCfgFile = CCfgFile::Instance();
	pCfgFile->Open();
	pCfgFile->Set(m_iBaudRate, _T("COMSET"), _T("BaudRate"));
	pCfgFile->Set(m_strComportNo.GetBuffer(), _T("COMSET"), _T("ComportNo"));
	pCfgFile->Set(m_strDataSaveFolder.GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	pCfgFile->Set(m_strDataFileName.GetBuffer(), _T("Environment"), _T("DataFileName"));

	pCfgFile->Set(m_iUseWarning, _T("WarningCondition"), _T("UseWarningCondition"));
	pCfgFile->Set(m_iUseTemp, _T("WarningCondition"), _T("UseTemp"));
	pCfgFile->Set(m_dblThresTemp, _T("WarningCondition"), _T("ThresholdTemp"));
	pCfgFile->Set(m_iTimeWarningSel, _T("WarningCondition"), _T("TimeWarningSel"));
	pCfgFile->Set(m_iMaxMarkInExlForm, _T("ExlReport"), _T("MaxMark"));

	pCfgFile->Set((long)(int)m_DeviceFrequency, _T("MODE"), _T("Frequency"));
	pCfgFile->Set((long)(int)m_viewRangeMode, _T("MODE"), _T("ViewRange"));
#pragma region 그래프 색
	iRValue = GetRValue(m_RMSPenColor);
	iGValue = GetGValue(m_RMSPenColor);
	iBValue = GetBValue(m_RMSPenColor);
	pCfgFile->Set(iRValue, _T("GRAPH_COLOR"), _T("RMS_Value_R"));
	pCfgFile->Set(iGValue, _T("GRAPH_COLOR"), _T("RMS_Value_G"));
	pCfgFile->Set(iBValue, _T("GRAPH_COLOR"), _T("RMS_Value_B"));

	iRValue = GetRValue(m_XPenColor);
	iGValue = GetGValue(m_XPenColor);
	iBValue = GetBValue(m_XPenColor);
	pCfgFile->Set(iRValue, _T("GRAPH_COLOR"), _T("X_Value_R"));
	pCfgFile->Set(iGValue, _T("GRAPH_COLOR"), _T("X_Value_G"));
	pCfgFile->Set(iBValue, _T("GRAPH_COLOR"), _T("X_Value_B"));

	iRValue = GetRValue(m_YPenColor);
	iGValue = GetGValue(m_YPenColor);
	iBValue = GetBValue(m_YPenColor);
	pCfgFile->Set(iRValue, _T("GRAPH_COLOR"), _T("Y_Value_R"));
	pCfgFile->Set(iGValue, _T("GRAPH_COLOR"), _T("Y_Value_G"));
	pCfgFile->Set(iBValue, _T("GRAPH_COLOR"), _T("Y_Value_B"));

	iRValue = GetRValue(m_ZPenColor);
	iGValue = GetGValue(m_ZPenColor);
	iBValue = GetBValue(m_ZPenColor);
	pCfgFile->Set(iRValue, _T("GRAPH_COLOR"), _T("Z_Value_R"));
	pCfgFile->Set(iGValue, _T("GRAPH_COLOR"), _T("Z_Value_G"));
	pCfgFile->Set(iBValue, _T("GRAPH_COLOR"), _T("Z_Value_B"));

	iRValue = GetRValue(m_TriggerColor);
	iGValue = GetGValue(m_TriggerColor);
	iBValue = GetBValue(m_TriggerColor);
	pCfgFile->Set(iRValue, _T("GRAPH_COLOR"), _T("Trigger_Value_R"));
	pCfgFile->Set(iGValue, _T("GRAPH_COLOR"), _T("Trigger_Value_G"));
	pCfgFile->Set(iBValue, _T("GRAPH_COLOR"), _T("Trigger_Value_B"));

	iRValue = GetRValue(m_GoNoGoColor);
	iGValue = GetGValue(m_GoNoGoColor);
	iBValue = GetBValue(m_GoNoGoColor);
	pCfgFile->Set(iRValue, _T("GRAPH_COLOR"), _T("GoNoGo_Value_R"));
	pCfgFile->Set(iGValue, _T("GRAPH_COLOR"), _T("GoNoGo_Value_G"));
	pCfgFile->Set(iBValue, _T("GRAPH_COLOR"), _T("GoNoGo_Value_B"));

	iRValue = GetRValue(m_SpectInColor);
	iGValue = GetGValue(m_SpectInColor);
	iBValue = GetBValue(m_SpectInColor);
	pCfgFile->Set(iRValue, _T("GRAPH_COLOR"), _T("SpecIn_Value_R"));
	pCfgFile->Set(iGValue, _T("GRAPH_COLOR"), _T("SpecIn_Value_G"));
	pCfgFile->Set(iBValue, _T("GRAPH_COLOR"), _T("SpecIn_Value_B"));

	iRValue = GetRValue(m_SpectOutColor);
	iGValue = GetGValue(m_SpectOutColor);
	iBValue = GetBValue(m_SpectOutColor);
	pCfgFile->Set(iRValue, _T("GRAPH_COLOR"), _T("SpecOut_Value_R"));
	pCfgFile->Set(iGValue, _T("GRAPH_COLOR"), _T("SpecOut_Value_G"));
	pCfgFile->Set(iBValue, _T("GRAPH_COLOR"), _T("SpecOut_Value_B"));

	iRValue = GetRValue(m_OverlayColor);
	iGValue = GetGValue(m_OverlayColor);
	iBValue = GetBValue(m_OverlayColor);
	pCfgFile->Set(iRValue, _T("GRAPH_COLOR"), _T("Overlay_Value_R"));
	pCfgFile->Set(iGValue, _T("GRAPH_COLOR"), _T("Overlay_Value_G"));
	pCfgFile->Set(iBValue, _T("GRAPH_COLOR"), _T("Overlay_Value_B"));

	iRValue = GetRValue(m_MarkColor);
	iGValue = GetGValue(m_MarkColor);
	iBValue = GetBValue(m_MarkColor);
	pCfgFile->Set(iRValue, _T("GRAPH_COLOR"), _T("Mark_Value_R"));
	pCfgFile->Set(iGValue, _T("GRAPH_COLOR"), _T("Mark_Value_G"));
	pCfgFile->Set(iBValue, _T("GRAPH_COLOR"), _T("Mark_Value_B"));

	iRValue = GetRValue(m_IndicatorColor);
	iGValue = GetGValue(m_IndicatorColor);
	iBValue = GetBValue(m_IndicatorColor);
	pCfgFile->Set(iRValue, _T("GRAPH_COLOR"), _T("Indicator_Value_R"));
	pCfgFile->Set(iGValue, _T("GRAPH_COLOR"), _T("Indicator_Value_G"));
	pCfgFile->Set(iBValue, _T("GRAPH_COLOR"), _T("Indicator_Value_B"));

#pragma endregion
	iTemp = (int)m_bSaveActivation;
	pCfgFile->Set(iTemp, _T("SAVE"), _T("Save_Activation"));
	iTemp = (long)m_iReportGraphWidth;
	pCfgFile->Set(iTemp, _T("REPORT"), _T("Report_Graph_width"));
	iTemp = (long)m_iReportGraphHeight;
	pCfgFile->Set(iTemp, _T("REPORT"), _T("Report_Graph_Height"));
#pragma region GoNoGo
	pCfgFile->Set(m_dblGoNoGoAbove_RMS_Value, _T("GONOGO"), _T("Above_RMS"));	//단위 환산 안함
	pCfgFile->Set(m_dblGoNoGoAbove_X_Value, _T("GONOGO"), _T("Above_X"));
	pCfgFile->Set(m_dblGoNoGoAbove_Y_Value, _T("GONOGO"), _T("Above_Y"));
	pCfgFile->Set(m_dblGoNoGoAbove_Z_Value, _T("GONOGO"), _T("Above_Z"));
	pCfgFile->Set(m_dblGoNoGoBelow_X_Value, _T("GONOGO"), _T("Below_X"));
	pCfgFile->Set(m_dblGoNoGoBelow_Y_Value, _T("GONOGO"), _T("Below_Y"));
	pCfgFile->Set(m_dblGoNoGoBelow_Z_Value, _T("GONOGO"), _T("Below_Z"));

	pCfgFile->Set(m_iGoNoGo_QualityTime, _T("GONOGO"), _T("Quality_Time"));
	pCfgFile->Set((long)m_iNoGoIndicator, _T("GONOGO"), _T("NoGoIndicator_Second"));


	iTemp = (int)m_bIsGoNoGoAbove_RMS;
	pCfgFile->Set(iTemp, _T("GONOGO"), _T("Above_Enable_RMS"));	//단위 환산 안함

	iTemp = (int)m_bIsGoNoGoAbove_X;
	pCfgFile->Set(iTemp, _T("GONOGO"), _T("Above_Enable_X"));

	iTemp = (int)m_bIsGoNoGoAbove_Y;
	pCfgFile->Set(iTemp, _T("GONOGO"), _T("Above_Enable_Y"));

	iTemp = (int)m_bIsGoNoGoAbove_Z;
	pCfgFile->Set(iTemp, _T("GONOGO"), _T("Above_Enable_Z"));

	iTemp = (int)m_bIsGoNoGoBelow_X;
	pCfgFile->Set(iTemp, _T("GONOGO"), _T("Below_Enable_X"));

	iTemp = (int)m_bIsGoNoGoBelow_Y;
	pCfgFile->Set(iTemp, _T("GONOGO"), _T("Below_Enable_Y"));

	iTemp = (int)m_bIsGoNoGoBelow_Z;
	pCfgFile->Set(iTemp, _T("GONOGO"), _T("Below_Enable_Z"));
#pragma endregion

#pragma region Trigger
	pCfgFile->Set(m_dblTriggerAbove_RMS_Value, _T("TRIGGER"), _T("Above_RMS"));	//단위 환산 안함
	pCfgFile->Set(m_dblTriggerAbove_X_Value, _T("TRIGGER"), _T("Above_X"));
	pCfgFile->Set(m_dblTriggerAbove_Y_Value, _T("TRIGGER"), _T("Above_Y"));
	pCfgFile->Set(m_dblTriggerAbove_Z_Value, _T("TRIGGER"), _T("Above_Z"));
	pCfgFile->Set(m_dblTriggerBelow_X_Value, _T("TRIGGER"), _T("Below_X"));
	pCfgFile->Set(m_dblTriggerBelow_Y_Value, _T("TRIGGER"), _T("Below_Y"));
	pCfgFile->Set(m_dblTriggerBelow_Z_Value, _T("TRIGGER"), _T("Below_Z"));

	iTemp = (int)m_bIsTriggerAbove_RMS;
	pCfgFile->Set(iTemp, _T("TRIGGER"), _T("Above_Enable_RMS"));	//단위 환산 안함

	iTemp = (int)m_bIsTriggerAbove_X;
	pCfgFile->Set(iTemp, _T("TRIGGER"), _T("Above_Enable_X"));

	iTemp = (int)m_bIsTriggerAbove_Y;
	pCfgFile->Set(iTemp, _T("TRIGGER"), _T("Above_Enable_Y"));

	iTemp = (int)m_bIsTriggerAbove_Z;
	pCfgFile->Set(iTemp, _T("TRIGGER"), _T("Above_Enable_Z"));

	iTemp = (int)m_bIsTriggerBelow_X;
	pCfgFile->Set(iTemp, _T("TRIGGER"), _T("Below_Enable_X"));

	iTemp = (int)m_bIsTriggerBelow_Y;
	pCfgFile->Set(iTemp, _T("TRIGGER"), _T("Below_Enable_Y"));

	iTemp = (int)m_bIsTriggerBelow_Z;
	pCfgFile->Set(iTemp, _T("TRIGGER"), _T("Below_Enable_Z"));
#pragma endregion

	pCfgFile->Flush();
}


void CStatus::ExportSetting(CString strSettingFile)
{
	CString strPrevFileName;
	CCfgFile *pCfgFile = CCfgFile::Instance();
	strPrevFileName = pCfgFile->GetPath();
	pCfgFile->SetPath(strSettingFile);
	SetDefault();
	pCfgFile->SetPath(strPrevFileName);
}

void CStatus::SpectrumComboBoxUpdate()
{
	m_pAVMDDisplayDlg->SendMessage(WM_DISPLAYCOMBOBOXUPDATE);
}

Color CStatus::RGBToColor(long iRValue, long iGValue, long iBValue)
{
	Color color;
	COLORREF colorREF;
	colorREF = RGB(iRValue, iGValue, iBValue);
	color.SetFromCOLORREF(colorREF);

	return color;
}

#pragma region 플래그 상태
CStatus::tProgramMode CStatus::GetProgramMode()
{
	return m_ProgramMode;
}
CStatus::tProgressBarMode CStatus::GetProgressBarMode()
{
	return m_ProgressBarMode;
}

CStatus::tEventMode CStatus::GetEventMode()
{
	return m_EventMode;
}

void CStatus::SetProgramMode(tProgramMode ProgramMode)
{
	m_ProgramMode = ProgramMode;
	if (!m_pAVMDDisplayDlg)
		return;
	m_pAVMDDisplayDlg->SendMessage(WM_PM_CHANGE, NULL, NULL);
	m_pAVMDMonitorView->SendMessage(WM_UIUPDATE);
}

void CStatus::SetProgressBarMode(tProgressBarMode ProgressBarMode)
{
	m_ProgressBarMode = ProgressBarMode;
}

void CStatus::SetEventMode(tEventMode EventMode)
{
	m_EventMode = EventMode;
	if (!m_pAVMDDisplayDlg)
		return;
}

BOOL CStatus::IsMinimizeRMS()
{
	return m_bIsMinimizeRMS;
}

BOOL CStatus::IsMinimizeX()
{
	return m_bIsMinimizeX;
}

BOOL CStatus::IsMinimizeY()
{
	return m_bIsMinimizeY;
}

BOOL CStatus::IsMinimizeZ()
{
	return m_bIsMinimizeZ;
}

BOOL CStatus::IsFFTShow()
{
	return m_bFFTShowStatus;
}

BOOL CStatus::GetReEnter()
{
	BOOL test = m_bRMSReEnter == FALSE && m_bXReEnter == FALSE && m_bYReEnter == FALSE && m_bZReEnter == FALSE
		&&m_bFftRMSReEnter == FALSE && m_bFftXReEnter == FALSE && m_bFftYReEnter == FALSE && m_bFftZReEnter == FALSE
		&& m_bCompareReEnter == FALSE && m_bFFtCompareReEnter == FALSE;

	if (m_bRMSReEnter == FALSE && m_bXReEnter == FALSE && m_bYReEnter == FALSE && m_bZReEnter == FALSE
		 &&m_bFftRMSReEnter == FALSE && m_bFftXReEnter == FALSE && m_bFftYReEnter == FALSE && m_bFftZReEnter == FALSE
		&& m_bCompareReEnter == FALSE && m_bFFtCompareReEnter == FALSE && m_bFFTTimeDlgReEnter == FALSE && m_bTimeDlgReEnter == FALSE)// &&m_bIsHold == FALSE)
		m_bAllReEnter = FALSE;
	else
		m_bAllReEnter = TRUE;
	return m_bAllReEnter;
}

#pragma endregion

#pragma region 시리얼
void CStatus::SetComportNo(TCHAR* szComportNo)
{
	m_strComportNo = szComportNo;
}

CString CStatus::GetComportNo()
{
	return m_strComportNo;
}

void CStatus::SetBaudRate(int iBaudRate)
{
	m_iBaudRate = iBaudRate;
}

int CStatus::GetBaudRate()
{
	return m_iBaudRate;
}
#pragma endregion

CStatus::tViewRangeMode CStatus::GetViewRangeMode()
{
	return m_viewRangeMode;
}

CStatus::tDevice_Frequency CStatus::GetDeviceFrequency()
{
	return m_DeviceFrequency;
}

CString CStatus::GetDataSaveFolder()
{
	return m_strDataSaveFolder;
}

void CStatus::SetDataSaveFolder(TCHAR * szDataSaveFolder)
{
	m_strDataSaveFolder = szDataSaveFolder;
}

CString CStatus::GetDataFileName()
{
	return m_strDataFileName;
}

void CStatus::SetDataFileName(TCHAR * szDataFileName)
{
	m_strDataFileName = szDataFileName;
}

void CStatus::SetWorkEnvInfoToAdd(tWorkEnvInfo * pWorkEnvInfoToAdd)
{
	m_WorkEnvInfoToAdd = *pWorkEnvInfoToAdd;
}

CRepository * CStatus::GetRepository()
{
	return m_pRepository;
}

void CStatus::SetRepository(CRepository * pRepository)
{
	m_pRepository = pRepository;
}

void CStatus::SetFilterType(BOOL bLowPass, BOOL bHighPass, BOOL bBandPass)
{
	m_bIsLowPassFilter = bLowPass;
	m_bIsHighPassFilter = bHighPass;
	m_bIsBandPassFilter = bBandPass;
}

void CStatus::DisplayDlgUpdateUI()
{
	m_pAVMDDisplayDlg->SendMessage(WM_DISPLAYUIUPDATE);
	if(GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
		m_pAVMDMonitorView->SendMessage(WM_GRAPHUPDATE);
}

void CStatus::UpdateScrn()
{
	if (!GetReEnter())
	{
		m_pAVMDDisplayDlg->SendMessage(WM_DISPLAYUIUPDATE);
		m_pAVMDMonitorView->SendMessage(WM_GRAPHUPDATE);
	}
	else
	{
		printf("--Skip Update Scrn\n");
	}
}

void CStatus::GraphUpdate()
{
	m_pAVMDMonitorView->SendMessage(WM_UIUPDATE);
}

void CStatus::SetAmountofMouseMovement(int mouseMovement, int nInputType)
{
	
	m_iAmountofMousemovement[nInputType] = mouseMovement;
	m_dblDragAmount[nInputType] += mouseMovement;
}

int CStatus::GetAmountofMouseMovement(int nInputType)
{
	return m_iAmountofMousemovement[nInputType];
}

void CStatus::SetUnitTextPtX(int iPointX,CString strUnit, int nInputType)
{
	tUnitTextInfo Info;
	Info.iTextPt = iPointX;
	Info.strUnit = strUnit;
	m_iTextPt_X[nInputType].push_back(Info);
}

CStatus::tUnitTextInfo CStatus::GetUnitTextPtX(int nInputType)
{
	tUnitTextInfo Info = m_iTextPt_X[nInputType].front();
	m_iTextPt_X[nInputType].pop_front();
	return Info;
}

void CStatus::ClearUnitTextPtX(int nInputType)
{
	while (m_iTextPt_X[nInputType].size() != 0)
	{
		m_iTextPt_X[nInputType].pop_front();
	}
}

int CStatus::GetUnitTextPtXCount(int nInputType)
{
	int iCount = m_iTextPt_X[nInputType].size();
	return iCount;
}

void CStatus::SetReviewDrawStart(int iDrawStartNum, int nInputType)
{
	m_iReviewDrawStart[nInputType] = iDrawStartNum;
}

int CStatus::GetReviewDrawStart(int nInputType)
{
	int iDataCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
	if (iDataCount == 0)
		return 0;
	m_iReviewDrawStart[nInputType] = int(m_dblOnePixelDrawCount * m_dblDragAmount[nInputType]);

	if (CStatus::Instance()->m_bIsLowPassFilter || CStatus::Instance()->m_bIsHighPassFilter || CStatus::Instance()->m_bIsBandPassFilter)
	{
		if (m_iReviewDrawStart[nInputType] + m_nDrawPointNum >= iDataCount + m_nDrawPointNum)
		{
			int iCalcPt = 0;
			if (iDataCount - m_nDrawPointNum <= 0)
				iCalcPt = 0;
			else
			{
				iCalcPt = iDataCount - m_nDrawPointNum;
				m_iReviewDrawStart[nInputType] = iDataCount;
				m_iReviewDrawEnd[nInputType] = iDataCount;
			}
			double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();
			int iNewDragNum = (int)((double)m_iReviewDrawStart[nInputType] / dblOnePixelDrawCount);

			CStatus::Instance()->SetDragAmount(iNewDragNum, nInputType);
		}
	}
	else
	{
		if (m_iReviewDrawStart[nInputType] + m_nDrawPointNum >= (iDataCount + m_nDrawPointNum))
		{
			int iCalcPt = 0;
			//if (iDataCount - m_nDrawPointNum <= 0)
			//	iCalcPt = 0;
			//else
			//{
				iCalcPt = iDataCount - m_nDrawPointNum;
				m_iReviewDrawStart[nInputType] = iDataCount;
				m_iReviewDrawEnd[nInputType] = iDataCount;
			//}
			double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();
			int iNewDragNum = (int)((double)m_iReviewDrawStart[nInputType] / dblOnePixelDrawCount);

			CStatus::Instance()->SetDragAmount(iNewDragNum, nInputType);
		}
	}
	if (m_iReviewDrawStart[nInputType] <= 0)
	{
		m_iReviewDrawStart[nInputType] = 0;
		CStatus::Instance()->SetReviewDragReset(nInputType);
	}
	return m_iReviewDrawStart[nInputType];
}

void CStatus::SetReviewDrawEnd(int iDrawEndNum, int nInputType)
{
	m_iReviewDrawEnd[nInputType] = iDrawEndNum;
}

int CStatus::GetReviewDrawEnd(int nInputType)
{
	int iDataCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
	
	m_iReviewDrawEnd[nInputType] = m_iReviewDrawStart[nInputType] + m_nDrawPointNum;

	if (m_iReviewDrawStart[nInputType] + m_nDrawPointNum >= iDataCount)
		m_iReviewDrawEnd[nInputType] = iDataCount;

	return m_iReviewDrawEnd[nInputType];
}

void CStatus::TimeUnitInitialize()
{
	m_pAVMDDisplayDlg->SendMessage(WM_TIME_UNIT);
}

void CStatus::ReviewDlgUpdate()
{
	m_pAVMDReviewDlg->SendMessage(WM_REVIEW_UPDATE);
}

void CStatus::SetAVMDReView(CAVMDReview * pAVMDReviewDlg)
{
	m_pAVMDReviewDlg = pAVMDReviewDlg;
}

void CStatus::SetAVMDViewDlg(CAVMDViewDlg * pAVMDViewDlg)
{
	m_pAVMDViewDlg = pAVMDViewDlg;
}

void CStatus::SetDragAmount(double dblDragAmount, int nInputType)
{
	m_dblDragAmount[nInputType] = dblDragAmount;
}

double CStatus::GetDragAmount(int nInputType)
{
	return m_dblDragAmount[nInputType];
}

void CStatus::SetOnePixelDrawCount(double dblOnePixelDrawCount)
{
	m_dblOnePixelDrawCount = dblOnePixelDrawCount;
}

void CStatus::SetReviewDragReset(int nInputType)
{
	m_dblDragAmount[nInputType] = 0;
	m_iAmountofMousemovement[nInputType] = 0;
}

void CStatus::SetOverlapPixelCount(int nOverlapPixel, int nInputType)
{
	m_nOverlapPixel[nInputType] = nOverlapPixel;
}

double CStatus::GetOnePixelDrawCount()
{
	return m_dblOnePixelDrawCount;
}

void CStatus::StartRecording(int nIndex)
{
	m_pAVMDViewDlg->SendMessage(WM_RECORD_START, nIndex);
}

void CStatus::FinishRecording()
{
	m_pAVMDViewDlg->SendMessage(WM_RECORD_FINISH);
}

void CStatus::SetIsSepcOutSave(BOOL bIsSave)
{
	m_bIsSave = (int)bIsSave;
}

BOOL CStatus::GetIsSepcOutSave()
{
	return m_bIsSave;
}

BOOL CStatus::GetIsRMSTrigger()
{
	return m_bIsRMSTrigger;
}

void CStatus::SetIsRMSTrigger(BOOL bIsTrigger)
{
	m_bIsRMSTrigger = bIsTrigger;
}

BOOL CStatus::GetIsXTrigger()
{
	return m_bIsXTrigger;
}

void CStatus::SetIsXTrigger(BOOL bIsTrigger)
{
	m_bIsXTrigger = bIsTrigger;
}

BOOL CStatus::GetIsYTrigger()
{
	return m_bIsYTrigger;
}

void CStatus::SetIsYTrigger(BOOL bIsTrigger)
{
	m_bIsYTrigger = bIsTrigger;
}

BOOL CStatus::GetIsZTrigger()
{
	return m_bIsZTrigger;
}

void CStatus::SetIsZTrigger(BOOL bIsTrigger)
{
	m_bIsZTrigger = bIsTrigger;
}

void CStatus::TriggerReset()
{
	CStatus::Instance()->SetIsRMSTrigger(FALSE);
	CStatus::Instance()->SetIsXTrigger(FALSE);
	CStatus::Instance()->SetIsYTrigger(FALSE);
	CStatus::Instance()->SetIsZTrigger(FALSE);
	SetIsFirstEvent(TRUE);
}

BOOL CStatus::GetIsRMSGoNoGo()
{
	return m_bIsRMSGoNoGo;
}

void CStatus::SetIsRMSGoNoGo(BOOL bIsGoNoGo)
{
	m_bIsRMSGoNoGo = bIsGoNoGo;
}

BOOL CStatus::GetIsXGoNoGo()
{
	return m_bIsXGoNoGo;
}

void CStatus::SetIsXGoNoGo(BOOL bIsGoNoGo)
{
	m_bIsXGoNoGo = bIsGoNoGo;
}

BOOL CStatus::GetIsYGoNoGo()
{
	return m_bIsYGoNoGo;
}

void CStatus::SetIsYGoNoGo(BOOL bIsGoNoGo)
{
	m_bIsYGoNoGo = bIsGoNoGo;
}

BOOL CStatus::GetIsZGoNoGo()
{
	return m_bIsZGoNoGo;
}

void CStatus::SetIsZGoNoGo(BOOL bIsGoNoGo)
{
	m_bIsZGoNoGo = bIsGoNoGo;
}

void CStatus::GoNoGoReset()
{
	CStatus::Instance()->SetIsRMSGoNoGo(FALSE);
	CStatus::Instance()->SetIsXGoNoGo(FALSE);
	CStatus::Instance()->SetIsYGoNoGo(FALSE);
	CStatus::Instance()->SetIsZGoNoGo(FALSE);
	SetIsFirstEvent(TRUE);
}

BOOL CStatus::GetIsFirstEvent()
{//없애야함
	return m_bIsFirstEvent;
}

void CStatus::SetIsFirstEvent(bool bIsFirstEvent)
{//없애야함
	m_bIsFirstEvent = bIsFirstEvent;
}

BOOL CStatus::IsFilterUse()
{
	return m_bIsLowPassFilter || m_bIsHighPassFilter || m_bIsBandPassFilter;
}

BOOL CStatus::GetCompareCondition()
{
	return m_bIsCompare;
}

void CStatus::SetCompareCondition(bool bIsCompare)
{
	m_bIsCompare = bIsCompare;
}

void CStatus::SetCompareType(CString strCompareType)
{
	m_strCompareType = strCompareType;
}

CString CStatus::GetCompareType()
{
	return m_strCompareType;
}

void CStatus::SetOverlayCondition(bool bIsOverlay)
{
	m_bIsOverlay = bIsOverlay;
}

BOOL CStatus::GetOverlayCondition()
{
	return m_bIsOverlay;
}

void CStatus::UpdateReviewComboBox(int nInputType)
{
	m_pAVMDReviewDlg->SendMessage(WM_REVIEW_COMBO_UPDATE, nInputType);
}

BOOL CStatus::GetIsHoldCondition()
{
	return m_bIsHold;
}

void CStatus::SetIsHoldCondition(bool bIsHold)
{
	m_bIsHold = bIsHold;
}

void CStatus::UpdateViewHoldButton()
{
	m_pAVMDViewDlg->SendMessage(WM_UPDATE_BTN_HOLD, NULL, NULL);
}

int CStatus::GetPreRecordTime()
{
	return (int)(pow(60, m_iPRStateUnit)*(m_iPRStateTime)*1000);//Milisecond단위로 return
}

void CStatus::GetPreRecordStatus(int &iStateTime, int &iStateUnit)
{
	iStateTime = m_iPRStateTime;
	iStateUnit = m_iPRStateUnit;
}

void CStatus::SetPreRecordStatus(int iStateTime, int iStateUnit)
{
	m_iPRStateTime = iStateTime;
	m_iPRStateUnit = iStateUnit;
}

void CStatus::SetMarkClickEvent(bool bIsClick, int nInputType)
{
	if (nInputType == 0)
		CStatus::Instance()->m_bMarkEditClick = bIsClick;
	if (nInputType == 1)
		CStatus::Instance()->m_bCompareMarkEditClick = bIsClick;
}

BOOL CStatus::GetMarkClickEvent(int nInputType)
{
	if (nInputType == 0)
		return CStatus::Instance()->m_bMarkEditClick;
	else //if (nInputType == 1)
		return CStatus::Instance()->m_bCompareMarkEditClick;
}

void CStatus::SetMarkEditStart(int iNewPos, int nInputType)
{
	if (nInputType == 0)
		m_iMarkEditStart = iNewPos;
	if(nInputType == 1)
		m_iCompareMarkEditStart = iNewPos;
}

int CStatus::GetMarkEditStart(int nInputType)
{
	if (nInputType == 0)
		return m_iMarkEditStart;
	if (nInputType == 1)
		return m_iCompareMarkEditStart;
	return 0;
}

void CStatus::PlayerStartNFinish(BOOL bIsPlay)
{
	m_pAVMDReviewDlg->SendMessage(WM_TIMERSTART, bIsPlay);
}

void CStatus::ReviewMenuReset()
{
	m_pAVMDMonitorView->SendMessage(WM_RESET);
}

void CStatus::UpdateUITimeDomain()
{
	m_pAVMDDisplayDlg->SendMessage(WM_UIUPDATE);
}

int CStatus::ReviewDataFiltering()
{
	AfxBeginThread(ProcessFilterThread, this);
	//AfxMessageBox(_T("End"));
	return 0;
}

UINT CStatus::ProcessFilterThread(LPVOID lParam)
{
	CStatus::Instance()->m_bIsFiltering = TRUE;
	if (CStatus::Instance()->GetRepository()->GetDataCount(0) != 0)
	{
		m_pAVMDMonitorView->m_pGraphRMSDlg->m_drawHelper.ReviewAllDataFilter(0);
		m_pAVMDMonitorView->m_pGraphXDlg->m_drawHelper.ReviewAllDataFilter(0);
		m_pAVMDMonitorView->m_pGraphYDlg->m_drawHelper.ReviewAllDataFilter(0);
		m_pAVMDMonitorView->m_pGraphZDlg->m_drawHelper.ReviewAllDataFilter(0);
	}
	if (CStatus::Instance()->GetRepository()->GetDataCount(1) != 0)
	{
		m_pAVMDMonitorView->m_pCompareGraphDlg->m_drawHelper.ReviewAllDataFilter(1);
	}
	CStatus::Instance()->m_bIsThreadStop = FALSE;
	CStatus::Instance()->m_bIsFiltering = FALSE;
	return 0;
}

int CStatus::ReviewCompareDataFiltering()
{
	AfxBeginThread(ProcessCompareDataFilterThread, this);
	return 0;
}

UINT CStatus::ProcessCompareDataFilterThread(LPVOID lParam)
{
	if (CStatus::Instance()->GetRepository()->GetDataCount(1) != 0)
	{
//		m_pAVMDMonitorView->m_pCompareGraphDlg->m_drawHelper.AllDataFilter(1);
	}
	CStatus::Instance()->m_bIsThreadStop = FALSE;
	return 0;
}

void CStatus::ReviewDataClear(BOOL bIsClearReview, BOOL bIsClearCompare)
{
	if (bIsClearReview)
	{
		if (CStatus::Instance()->GetRepository()->GetDataCount(0) != 0)
		{
			m_pAVMDMonitorView->m_pGraphRMSDlg->m_drawHelper.AllFilterDataClear(0);
			m_pAVMDMonitorView->m_pGraphXDlg->m_drawHelper.AllFilterDataClear(0);
			m_pAVMDMonitorView->m_pGraphYDlg->m_drawHelper.AllFilterDataClear(0);
			m_pAVMDMonitorView->m_pGraphZDlg->m_drawHelper.AllFilterDataClear(0);
		}
	}
	if (bIsClearCompare)
	{
		if (CStatus::Instance()->GetRepository()->GetDataCount(1) != 0)
		{
			m_pAVMDMonitorView->m_pCompareGraphDlg->m_drawHelper.AllFilterDataClear(1);
		}
	}
	
}

double CStatus::GetViewFilteredValue(CString strGraphType, int nInputType, int nIndex)
{
	double dblValue = 0;
	if (nInputType == 0)
	{
		if (strGraphType == "RMS")
			dblValue = m_pAVMDMonitorView->m_pGraphRMSDlg->m_drawHelper.m_pdblFilteredData[nInputType][nIndex];
		if (strGraphType == "X")
			dblValue = m_pAVMDMonitorView->m_pGraphXDlg->m_drawHelper.m_pdblFilteredData[nInputType][nIndex];
		if (strGraphType == "Y")
			dblValue = m_pAVMDMonitorView->m_pGraphYDlg->m_drawHelper.m_pdblFilteredData[nInputType][nIndex];
		if (strGraphType == "Z")
		{
			dblValue = m_pAVMDMonitorView->m_pGraphZDlg->m_drawHelper.m_pdblFilteredData[nInputType][nIndex];
			//int size = sizeof(m_pAVMDMonitorView->m_pGraphZDlg->m_drawHelper.m_pdblFilteredData[nInputType][nIndex]);
			//printf("%d\n", size);
			//printf("Value %.2f\n", dblValue);
		}
	}
	if (nInputType == 1)
	{
		if (strGraphType == "RMS")
			dblValue = m_pAVMDMonitorView->m_pCompareGraphDlg->m_drawHelper.m_pdblFilterCompareData[0][nIndex];
		if (strGraphType == "X")
			dblValue = m_pAVMDMonitorView->m_pCompareGraphDlg->m_drawHelper.m_pdblFilterCompareData[1][nIndex];
		if (strGraphType == "Y")
			dblValue = m_pAVMDMonitorView->m_pCompareGraphDlg->m_drawHelper.m_pdblFilterCompareData[2][nIndex];
		if (strGraphType == "Z")
			dblValue = m_pAVMDMonitorView->m_pCompareGraphDlg->m_drawHelper.m_pdblFilterCompareData[3][nIndex];
	}
	return dblValue;
}


BOOL CStatus::CheckFFTDrawingRange(int nNewPos)
{
	int iFFTStartHz = nNewPos;
	int FFTShowRatio = CStatus::Instance()->m_iFFTShowRatio;
	int iFftSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	int iGraphWidthOneInterval = 0;

	if (iFftSampleSize == 32)	iGraphWidthOneInterval = 128 * FFTShowRatio;
	if (iFftSampleSize == 64)	iGraphWidthOneInterval = 64 * FFTShowRatio;
	if (iFftSampleSize == 128)	iGraphWidthOneInterval = 32 * FFTShowRatio;
	if (iFftSampleSize == 256)	iGraphWidthOneInterval = 16 * FFTShowRatio;
	if (iFftSampleSize == 512)	iGraphWidthOneInterval = 8 * FFTShowRatio;
	if (iFftSampleSize == 1024)	iGraphWidthOneInterval = 4 * FFTShowRatio;
	if (iFftSampleSize == 2048)	iGraphWidthOneInterval = 2 * FFTShowRatio;
	if (iFftSampleSize == 4096)	iGraphWidthOneInterval = 1 * FFTShowRatio;

	double dblInterval = (double)FFT_SAMPLING_FREQUENCY / (double)4096;
	int iFFTwidth = CStatus::Instance()->m_iFFTwidth;
	double dblHzAmount = dblInterval * (double)iFFTwidth / (double)FFTShowRatio;

	int iMaxHz = (int)CStatus::Instance()->m_DeviceFrequency / 2;
	if (((double)iFFTStartHz + dblHzAmount) > (iMaxHz-1))
		return FALSE;
	return TRUE;
}


int CStatus::FFTDrawingMaxPos()
{
	int FFTShowRatio = CStatus::Instance()->m_iFFTShowRatio;
	int iFftSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	int iGraphWidthOneInterval = 0;

	if (iFftSampleSize == 32)	iGraphWidthOneInterval = 128 * FFTShowRatio;
	if (iFftSampleSize == 64)	iGraphWidthOneInterval = 64 * FFTShowRatio;
	if (iFftSampleSize == 128)	iGraphWidthOneInterval = 32 * FFTShowRatio;
	if (iFftSampleSize == 256)	iGraphWidthOneInterval = 16 * FFTShowRatio;
	if (iFftSampleSize == 512)	iGraphWidthOneInterval = 8 * FFTShowRatio;
	if (iFftSampleSize == 1024)	iGraphWidthOneInterval = 4 * FFTShowRatio;
	if (iFftSampleSize == 2048)	iGraphWidthOneInterval = 2 * FFTShowRatio;
	if (iFftSampleSize == 4096)	iGraphWidthOneInterval = 1 * FFTShowRatio;

	double dblInterval = (double)FFT_SAMPLING_FREQUENCY / (double)4096;
	int iFFTwidth = CStatus::Instance()->m_iFFTwidth;
	double dblHzAmount = dblInterval * (double)iFFTwidth / (double)FFTShowRatio;

	int iMaxHz = (int)CStatus::Instance()->m_DeviceFrequency / 2;
	int nMaxPos = (int)((double)iMaxHz - dblHzAmount);
	
	return nMaxPos;
}


void CStatus::ReviewComboClear()
{
	m_pAVMDReviewDlg->SendMessage(WM_RESET);
}
