#include "stdafx.h"
#include "Status.h"
#include "AGMDMonitorDoc.h"
#include "AGMDMonitorView.h"
#include "Repository.h"
#include "MonitorDlg.h"
#include "AGMDDefine.h"
#include "BrowserDlg.h"
#include "MainFrm.h"
#include "StationInfomationDlg.h"

CStatus::tProgramMode CStatus::m_ProgramMode = PM_REVIEW;
CStatus* CStatus::m_pInstance = NULL;
CAGMDMonitorView* CStatus::m_pAGMDMonitorView = NULL;
CMonitorDlg * CStatus::m_pMonitorDlg = NULL;
CSenSorStatusDlg * CStatus::m_pSensorStatusDlg = NULL;
CSenSorViewDlg * CStatus::m_pSensorViewDlg = NULL;
COPPanelDlg * CStatus::m_pAGMDOPPanelDlg = NULL;
CRepository* CStatus::m_pRepository = NULL;
CBrowserDlg* CStatus::m_pBrowserDlg = NULL;
CStationInfomationDlg* CStatus::m_pStationInfoDlg = NULL;
CStatus::CStatus()
	: m_strComportNo(_T(""))
	, m_iBaudRate(0)
	, m_strCalDate(_T(""))
	, m_strSerialNo(_T(""))
	, m_iWaferSize(0)
	, m_byBattery(0)
	, m_iTemperature(0)
	, m_bReEnter(FALSE)
	, m_bIsGapSameSettings(FALSE)
	, m_dblViewRange(0)
	, m_bIsOffsetMode(FALSE)
	, m_strLastOpenFileName(_T(""))
	, m_iLastUnitMode(0)
	, m_iNewJigGap(0)
	, m_strMark(_T(""))
{
	wsprintf(m_WorkEnvInfo.szOperator, _T(""));
	wsprintf(m_WorkEnvInfo.szTool, _T(""));
	wsprintf(m_WorkEnvInfo.szStation, _T(""));
	wsprintf(m_WorkEnvInfo.szComment, _T(""));
	
	m_dblTarget[0] = 0, m_dblTarget[1] = 0, m_dblTarget[2] = 0, m_dblTarget[3] = 0;
	m_dblTolerance[0] = 0, m_dblTolerance[1] = 0, m_dblTolerance[2] = 0, m_dblTolerance[3] = 0;
	m_dblOffset[0] = 0, m_dblOffset[1] = 0, m_dblOffset[2] = 0;
	memset(&m_CalData, 0, sizeof(m_CalData));
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

void CStatus::SetAGMDView(CAGMDMonitorView* pAGMDMonitorView)
{
	m_pAGMDMonitorView = pAGMDMonitorView;
}

CAGMDMonitorView* CStatus::GetAGMDView()
{
	return m_pAGMDMonitorView;
}

void CStatus::SetOPPanelDlg(COPPanelDlg* pOPPanelDlg)
{
	m_pAGMDOPPanelDlg = pOPPanelDlg;
}

COPPanelDlg* CStatus::GetOPPanelDlg()
{
	return m_pAGMDOPPanelDlg;
}

void CStatus::SetBrowserDlg(CBrowserDlg* pBrowserDlg)
{
	m_pBrowserDlg = pBrowserDlg;
}

void CStatus::SetStationInfomationDlg(CStationInfomationDlg* pSationInfoDlg)
{
	m_pStationInfoDlg = pSationInfoDlg;
}

CBrowserDlg* CStatus::GetBrowserDlg()
{
	return m_pBrowserDlg;
}

CStatus::tProgramMode CStatus::GetProgramMode()
{
	return m_ProgramMode;
}

CRepository * CStatus::GetRepository()
{
	return m_pRepository;
}

void CStatus::SetRepository(CRepository * pRepository)
{
	m_pRepository = pRepository;
}

void CStatus::SetProgramMode(tProgramMode ProgramMode)
{
	m_ProgramMode = ProgramMode;
	if (!m_pMonitorDlg)
		return;
	m_pMonitorDlg->SendMessage(WM_PM_CHANGE, NULL, NULL);

	if (!m_pAGMDMonitorView)
		return;
	m_pAGMDMonitorView->SendMessage(WM_UIUPDATE);

}

void CStatus::SetMonitorDlg(CMonitorDlg * pMonitorDlg)
{
	m_pMonitorDlg = pMonitorDlg;
}

CMonitorDlg* CStatus::GetMonitorDlg()
{
	return m_pMonitorDlg;
}

CString CStatus::GetDataFileName()
{
	return m_strDataFileName;
}

void CStatus::SetDataFileName(TCHAR * szDataFileName)
{
	m_strDataFileName = szDataFileName;
}

void CStatus::SetDataSaveFolder(TCHAR * szDataSaveFolder)
{
	m_strDataSaveFolder = szDataSaveFolder;
}

CString CStatus::GetDataSaveFolder()
{
	return m_strDataSaveFolder;
}
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

int CStatus::GetDefault()
{
	CCfgFile *pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pMyCfg->Open();
	m_iBaudRate = pMyCfg->Get(9600L, _T("COMSET"), _T("BaudRate"));
	m_strComportNo = pMyCfg->Get(_T("None"), _T("COMSET"), _T("ComportNo"));
	m_strDataSaveFolder = pMyCfg->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	m_strDataFileName = pMyCfg->Get(_T("AGMDLog"), _T("Environment"), _T("DataFileName"));
	
	int iNotchMode = pMyCfg->Get(0L, _T("Setting"), _T("NotchMode"));
	int iUnitMode = pMyCfg->Get(1L, _T("Setting"), _T("UnitMode"));

	SetNotchNUnitMode(iNotchMode, iUnitMode);
	m_bIsGapSameSettings = pMyCfg->Get(0L, _T("Setting"), _T("GapSameSettings"));
	m_dblViewRange = pMyCfg->Get(100L, _T("Setting"), _T("View Range"));
	m_bIsOffsetMode = pMyCfg->Get(0L, _T("Setting"), _T("Offset Mode"));

	m_dblTarget[0] = pMyCfg->Get(15025L, _T("Target"), _T("GAP_1"));
	m_dblTarget[1] = pMyCfg->Get(15025L, _T("Target"), _T("GAP_2"));
	m_dblTarget[2] = pMyCfg->Get(15025L, _T("Target"), _T("GAP_3"));
	m_dblTarget[3] = pMyCfg->Get(15025L, _T("Target"), _T("GAP_ALL"));
	m_dblTolerance[0] = pMyCfg->Get(25L, _T("Tolerance"), _T("GAP_1"));
	m_dblTolerance[1] = pMyCfg->Get(25L, _T("Tolerance"), _T("GAP_2"));
	m_dblTolerance[2] = pMyCfg->Get(25L, _T("Tolerance"), _T("GAP_3"));
	m_dblTolerance[3] = pMyCfg->Get(25L, _T("Tolerance"), _T("GAP_ALL"));

	m_dblOffset[0] = pMyCfg->Get(0L, _T("Offset"), _T("GAP_1"));
	m_dblOffset[1] = pMyCfg->Get(0L, _T("Offset"), _T("GAP_2"));
	m_dblOffset[2] = pMyCfg->Get(0L, _T("Offset"), _T("GAP_3"));

#pragma region Color
	long iRValue, iGValue, iBValue;
	iRValue = pMyCfg->Get(0L, _T("COLOR"), _T("Tolerance_R"));
	iGValue = pMyCfg->Get(128L, _T("COLOR"), _T("Tolerance_G"));
	iBValue = pMyCfg->Get(0L, _T("COLOR"), _T("Tolerance_B"));
	m_ToleranceColor = RGBToColor(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(0L, _T("COLOR"), _T("SpecIn_R"));
	iGValue = pMyCfg->Get(128L, _T("COLOR"), _T("SpecIn_G"));
	iBValue = pMyCfg->Get(0L, _T("COLOR"), _T("SpecIn_B"));
	m_SpecInColor = RGBToColor(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(0L, _T("COLOR"), _T("SpecSmallOut_R"));
	iGValue = pMyCfg->Get(0L, _T("COLOR"), _T("SpecSmallOut_G"));
	iBValue = pMyCfg->Get(255L, _T("COLOR"), _T("SpecSmallOut_B"));
	m_SpecSmallOutColor = RGBToColor(iRValue, iGValue, iBValue);

	iRValue = pMyCfg->Get(255L, _T("COLOR"), _T("SpecBigOut_R"));
	iGValue = pMyCfg->Get(0L, _T("COLOR"), _T("SpecBigOut_G"));
	iBValue = pMyCfg->Get(0L, _T("COLOR"), _T("SpecBigOut_B"));
	m_SpecBigOutColor = RGBToColor(iRValue, iGValue, iBValue);
#pragma endregion
	return 0;
}

void CStatus::SetDefault()
{
	long iTemp = 0;
	CCfgFile *pCfgFile = CCfgFile::Instance();
	pCfgFile->Open();
	pCfgFile->Set(m_iBaudRate, _T("COMSET"), _T("BaudRate"));
	pCfgFile->Set(m_strComportNo.GetBuffer(), _T("COMSET"), _T("ComportNo"));
	pCfgFile->Set(m_strDataSaveFolder.GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	pCfgFile->Set(m_strDataFileName.GetBuffer(), _T("Environment"), _T("DataFileName"));

	pCfgFile->Set((long)m_NotchMode, _T("Setting"), _T("NotchMode"));
	pCfgFile->Set((long)m_UnitMode, _T("Setting"), _T("UnitMode"));
	pCfgFile->Set((long)m_bIsGapSameSettings, _T("Setting"), _T("GapSameSettings"));
	pCfgFile->Set((long)m_bIsOffsetMode, _T("Setting"), _T("Offset Mode"));
	pCfgFile->Set(m_dblViewRange, _T("Setting"), _T("View Range"));

	pCfgFile->Set((long)m_dblTarget[0], _T("Target"), _T("GAP_1"));
	pCfgFile->Set((long)m_dblTarget[1], _T("Target"), _T("GAP_2"));
	pCfgFile->Set((long)m_dblTarget[2], _T("Target"), _T("GAP_3"));
	pCfgFile->Set((long)m_dblTarget[3], _T("Target"), _T("GAP_ALL"));
	pCfgFile->Set((long)m_dblTolerance[0], _T("Tolerance"), _T("GAP_1"));
	pCfgFile->Set((long)m_dblTolerance[1], _T("Tolerance"), _T("GAP_2"));
	pCfgFile->Set((long)m_dblTolerance[2], _T("Tolerance"), _T("GAP_3"));
	pCfgFile->Set((long)m_dblTolerance[3], _T("Tolerance"), _T("GAP_ALL"));

	pCfgFile->Set((long)m_dblOffset[0], _T("Offset"), _T("GAP_1"));
	pCfgFile->Set((long)m_dblOffset[1], _T("Offset"), _T("GAP_2"));
	pCfgFile->Set((long)m_dblOffset[2], _T("Offset"), _T("GAP_3"));
#pragma region Color
	long iRValue, iGValue, iBValue;
	iRValue = m_ToleranceColor.GetR();
	iGValue = m_ToleranceColor.GetG();
	iBValue = m_ToleranceColor.GetB();
	pCfgFile->Set(iRValue, _T("COLOR"), _T("Tolerance_R"));
	pCfgFile->Set(iGValue, _T("COLOR"), _T("Tolerance_G"));
	pCfgFile->Set(iBValue, _T("COLOR"), _T("Tolerance_B"));

	iRValue = m_SpecInColor.GetR();
	iGValue = m_SpecInColor.GetG();
	iBValue = m_SpecInColor.GetB();
	pCfgFile->Set(iRValue, _T("COLOR"), _T("SpecIn_R"));
	pCfgFile->Set(iGValue, _T("COLOR"), _T("SpecIn_G"));
	pCfgFile->Set(iBValue, _T("COLOR"), _T("SpecIn_B"));

	iRValue = m_SpecSmallOutColor.GetR();
	iGValue = m_SpecSmallOutColor.GetG();
	iBValue = m_SpecSmallOutColor.GetB();
	pCfgFile->Set(iRValue, _T("COLOR"), _T("SpecSmallOut_R"));
	pCfgFile->Set(iGValue, _T("COLOR"), _T("SpecSmallOut_G"));
	pCfgFile->Set(iBValue, _T("COLOR"), _T("SpecSmallOut_B"));

	iRValue = m_SpecBigOutColor.GetR();
	iGValue = m_SpecBigOutColor.GetG();
	iBValue = m_SpecBigOutColor.GetB();
	pCfgFile->Set(iRValue, _T("COLOR"), _T("SpecBigOut_R"));
	pCfgFile->Set(iGValue, _T("COLOR"), _T("SpecBigOut_G"));
	pCfgFile->Set(iBValue, _T("COLOR"), _T("SpecBigOut_B"));
#pragma endregion

	pCfgFile->Flush();
}

void CStatus::SetWaferSize(int sizeWafer)
{
	m_iWaferSize = sizeWafer;
}

void CStatus::SetMeasureData(tAGMDRecord *pCurData)
{
	m_curMeasureData = *pCurData;
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
	{
		m_pSensorStatusDlg->SendMessage(WM_UIUPDATE);
	}
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		LogSettingApply(&m_curMeasureData);
		m_pAGMDOPPanelDlg->SendMessage(WM_UIUPDATE);
		m_pAGMDOPPanelDlg->SendMessage(WM_UNITUPDATE);
	}
	
	m_pMonitorDlg->SendMessage(WM_UIUPDATE);
}

tAGMDRecord * CStatus::GetCurMeasureData()
{
	return &m_curMeasureData;
}

CStatus::tNotchMode CStatus::GetNotchMode()
{
	return m_NotchMode;
}

void CStatus::SetNotchMode(tNotchMode NotchMode)
{
	m_NotchMode = NotchMode;
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		tAGMDRecord *pMeaInfo = CStatus::Instance()->GetRepository()->GetMeasureInfo(CStatus::Instance()->GetRepository()->m_iDispIndex);
		pMeaInfo->iNotchMode = (int)NotchMode;
	}
}

CStatus::tUnitMode CStatus::GetUnitMode()
{
	return m_UnitMode;
}

void CStatus::SetUnitMode(tUnitMode unitNude)
{
	m_UnitMode = unitNude;
}

void CStatus::SetSensorStatusDlg(CSenSorStatusDlg * pSensorStatusDlg)
{
	m_pSensorStatusDlg = pSensorStatusDlg;
}

CSenSorStatusDlg* CStatus::GetSensorStatusDlg()
{
	return m_pSensorStatusDlg;
}

void CStatus::SetSensorViewDlg(CSenSorViewDlg * pSensorViewDlg)
{
	m_pSensorViewDlg = pSensorViewDlg;
}

void CStatus::SetNotchNUnitMode(int iNotchMode, int iUnitMode)
{
	if (iNotchMode == 0)
		m_NotchMode = CStatus::tNotchMode::TWO_CLOCK;
	if (iNotchMode == 1)
		m_NotchMode = CStatus::tNotchMode::FOUR_CLOCK;
	if (iNotchMode == 2)
		m_NotchMode = CStatus::tNotchMode::SIX_CLOCK;
	if (iNotchMode == 3)
		m_NotchMode = CStatus::tNotchMode::EIGHT_CLOCK;
	if (iNotchMode == 4)
		m_NotchMode = CStatus::tNotchMode::TEN_CLOCK;
	if (iNotchMode == 5)
		m_NotchMode = CStatus::tNotchMode::TWELVE_CLOCK;
	////
	if (iUnitMode == 0)
		m_UnitMode = CStatus::tUnitMode::UNIT_INCH;
	if (iUnitMode == 1)
		m_UnitMode = CStatus::tUnitMode::UNIT_MM;
}

void CStatus::UpdateScrn()
{
	if (!m_bReEnter)
	{
		if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
		{
			OPPanelUnitUpdate();
			OpPanelOffsetInitialize();
			
		}
		
		m_pSensorViewDlg->SendMessage(WM_UIUPDATE);
		//m_pAGMDMonitorView->SendMessage(WM_UIUPDATE);
	}
	else
	{
		printf("--Skip Update Scrn");
	}
}

void CStatus::SetIsGapSameSettings(BOOL bIsGapSameSettings)
{
	m_bIsGapSameSettings = bIsGapSameSettings;
}

BOOL CStatus::GetIsGapSameSettings()
{
	return m_bIsGapSameSettings;
}

void CStatus::SetTarget(int iGapNum, double dblTarget)
{
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
		m_dblTarget[iGapNum] = dblTarget * UNIT_MM_VALUE;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
		m_dblTarget[iGapNum] = dblTarget * UNIT_MM_VALUE / UNIT_INCH_VALUE;
}

void CStatus::SetPointTarget(int iGapNum, double dblTarget)
{
	tAGMDRecord *pMeaInfo = CStatus::Instance()->GetRepository()->GetMeasureInfo(CStatus::Instance()->GetRepository()->m_iDispIndex);

	if (iGapNum == 0)
		pMeaInfo->dblTarget_Gap1 = dblTarget;
	if (iGapNum == 1)
		pMeaInfo->dblTarget_Gap2 = dblTarget;
	if (iGapNum == 2)
		pMeaInfo->dblTarget_Gap3 = dblTarget;
	if (iGapNum == 3)
		pMeaInfo->dblTarget_AllGap = dblTarget;
}

void CStatus::SetPointTolerance(int iGapNum, double dblTolerance)
{
	tAGMDRecord *pMeaInfo = CStatus::Instance()->GetRepository()->GetMeasureInfo(CStatus::Instance()->GetRepository()->m_iDispIndex);

	if (iGapNum == 0)
		pMeaInfo->dblTolerance_Gap1 = dblTolerance;
	if (iGapNum == 1)
		pMeaInfo->dblTolerance_Gap2 = dblTolerance;
	if (iGapNum == 2)
		pMeaInfo->dblTolerance_Gap3 = dblTolerance;
	if (iGapNum == 3)
		pMeaInfo->dblTolerance_AllGap = dblTolerance;

}

double CStatus::GetTarget(int iGapNum)
{
	double dblCalcNum = 0;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
		dblCalcNum = m_dblTarget[iGapNum] / UNIT_MM_VALUE;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
		dblCalcNum = m_dblTarget[iGapNum] / UNIT_MM_VALUE * UNIT_INCH_VALUE;

	return dblCalcNum;
}

void CStatus::SetTolerance(int iGapNum, double dblTolerance)
{
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
		m_dblTolerance[iGapNum] = dblTolerance * UNIT_MM_VALUE;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
		m_dblTolerance[iGapNum] = dblTolerance * UNIT_MM_VALUE / UNIT_INCH_VALUE;
}

double CStatus::GetTolerance(int iGapNum)
{
	double dblCalcNum = 0;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
		dblCalcNum = m_dblTolerance[iGapNum] / UNIT_MM_VALUE;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
		dblCalcNum = m_dblTolerance[iGapNum] / UNIT_MM_VALUE * UNIT_INCH_VALUE;

	return dblCalcNum;
}

double CStatus::GetGapRangeView()
{
	double dblCalcNum = 0;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
		dblCalcNum = m_dblViewRange / UNIT_MM_VALUE;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
		dblCalcNum = m_dblViewRange / UNIT_MM_VALUE * UNIT_INCH_VALUE;

	return dblCalcNum;
}

void CStatus::SetGapRangeView(double dblValue)
{
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
		m_dblViewRange = dblValue * UNIT_MM_VALUE;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
		m_dblViewRange = dblValue * UNIT_MM_VALUE / UNIT_INCH_VALUE;
}

void CStatus::SetOffset(int iGapNum, double dblOffset)
{
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
		m_dblOffset[iGapNum] = dblOffset * UNIT_MM_VALUE;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
		m_dblOffset[iGapNum] = dblOffset * UNIT_MM_VALUE / UNIT_INCH_VALUE;
}

double CStatus::GetOffset(int iGapNum)
{
	double dblCalcNum = 0;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
		dblCalcNum = m_dblOffset[iGapNum] / UNIT_MM_VALUE;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
		dblCalcNum = m_dblOffset[iGapNum] / UNIT_MM_VALUE * UNIT_INCH_VALUE;

	return dblCalcNum;
}

double CStatus::GetCalcReview(double dblValue)
{
	double dblCalcNum = 0;
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
	{
		dblCalcNum = dblValue / UNIT_MM_VALUE;
	}	
	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
	{
		dblCalcNum = dblValue / UNIT_MM_VALUE * UNIT_INCH_VALUE;
	}
	return dblCalcNum;
}
double CStatus::GetOrgOffset(int iGapNum)
{
	return m_dblOffset[iGapNum];
}

Color CStatus::RGBToColor(long iRValue, long iGValue, long iBValue)
{
	Color color;
	COLORREF colorREF;
	colorREF = RGB(iRValue, iGValue, iBValue);
	color.SetFromCOLORREF(colorREF);

	return color;
}

void CStatus::SetOffsetMode(BOOL bIsOffsetMode)
{
	m_bIsOffsetMode = bIsOffsetMode;
}

BOOL CStatus::GetIsOffsetMode()
{
	return m_bIsOffsetMode;
}

void CStatus::OPPanelUnitUpdate()
{
	m_pAGMDOPPanelDlg->SendMessage(WM_UNITUPDATE);
}
void CStatus::MonitorUpdate()
{
	m_pMonitorDlg->SendMessage(WM_UIUPDATE);
}

void CStatus::OPPanelOffsetUpdate()
{
	m_pAGMDOPPanelDlg->SendMessage(WM_UIUPDATE);
}

void CStatus::OpPanelOffsetInitialize()
{
	m_pAGMDOPPanelDlg->SendMessage(WM_INITIALIZE);
}

void CStatus::SetWorkEnvInfo(tWorkEnvInfo *pWorkEnvInfo)
{
	m_WorkEnvInfo = *pWorkEnvInfo;
}

tWorkEnvInfo CStatus::GetWorkEnvInfo()
{
	return m_WorkEnvInfo;
}

BOOL CStatus::GetLedStatus()
{
	return m_pSensorStatusDlg->m_cCntStat.m_bLedStatus;
}

void CStatus::OnInitBrowserUpdate()
{
	m_pBrowserDlg->SendMessage(WM_INITIALIZE);
}
#pragma warning(disable: 4996)
void CStatus::LogSettingApply(tAGMDRecord *pCurData)
{
	// Log 값 적용 시키기
	if (pCurData->iUnitMode == CStatus::tUnitMode::UNIT_INCH)
		CStatus::Instance()->SetUnitMode(CStatus::tUnitMode::UNIT_INCH);
	if (pCurData->iUnitMode == CStatus::tUnitMode::UNIT_MM)
		CStatus::Instance()->SetUnitMode(CStatus::tUnitMode::UNIT_MM);

	//Notch
	if (pCurData->iNotchMode == CStatus::tNotchMode::TWO_CLOCK)
		CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::TWO_CLOCK);
	if (pCurData->iNotchMode == CStatus::tNotchMode::FOUR_CLOCK)
		CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::FOUR_CLOCK);
	if (pCurData->iNotchMode == CStatus::tNotchMode::SIX_CLOCK)
		CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::SIX_CLOCK);
	if (pCurData->iNotchMode == CStatus::tNotchMode::EIGHT_CLOCK)
		CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::EIGHT_CLOCK);
	if (pCurData->iNotchMode == CStatus::tNotchMode::TEN_CLOCK)
		CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::TEN_CLOCK);
	if (pCurData->iNotchMode == CStatus::tNotchMode::TWELVE_CLOCK)
		CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::TWELVE_CLOCK);

	CStatus::Instance()->SetOffsetMode(pCurData->bIsOffsetMode);

	if (pCurData->bIsOffsetMode)
	{
		CStatus::Instance()->SetOffset(0, pCurData->dblOffset_Gap1);
		CStatus::Instance()->SetOffset(1, pCurData->dblOffset_Gap2);
		CStatus::Instance()->SetOffset(2, pCurData->dblOffset_Gap3);
	}
	else
	{
		CStatus::Instance()->SetOffset(0, 0.);
		CStatus::Instance()->SetOffset(1, 0.);
		CStatus::Instance()->SetOffset(2, 0.);
	}
	CStatus::Instance()->SetTarget(0, pCurData->dblTarget_Gap1);
	CStatus::Instance()->SetTarget(1, pCurData->dblTarget_Gap2);
	CStatus::Instance()->SetTarget(2, pCurData->dblTarget_Gap3);

	CStatus::Instance()->SetTolerance(0, pCurData->dblTolerance_Gap1);
	CStatus::Instance()->SetTolerance(1, pCurData->dblTolerance_Gap2);
	CStatus::Instance()->SetTolerance(2, pCurData->dblTolerance_Gap3);

	CStatus::Instance()->SetTarget(3, pCurData->dblTarget_AllGap);
	CStatus::Instance()->SetTolerance(3, pCurData->dblTolerance_AllGap);
	CStatus::Instance()->m_bIsGapSameSettings = pCurData->bIsAllGapSame;
	CStatus::Instance()->SetGapRangeView(pCurData->dblGapViewRange);
	CStatus::Instance()->SetWorkEnvInfo(&(pCurData->workEnvInfo));

	struct tm CalibrationDateTime = *localtime(&(pCurData->dateCalibration));
	CString strClibrationDate;
		
	strClibrationDate.Format(_T("%04d%02d%02d"), CalibrationDateTime.tm_year + 1900, CalibrationDateTime.tm_mon + 1, CalibrationDateTime.tm_mday); // 현재 일 반환

	CString strYear = strClibrationDate.Mid(2, 2);
	CString strMon = strClibrationDate.Mid(4, 2);
	CString strDay = strClibrationDate.Mid(6, 2);
	CStatus::Instance()->m_strCalDate = strYear + strMon + strDay;
}

CString CStatus::GetLastOpenFileName()
{
	return m_strLastOpenFileName;
}

void CStatus::AllDlgClear()
{
	m_pBrowserDlg->SendMessage(WM_DLG_INITIALIZE);
	m_pAGMDOPPanelDlg->SendMessage(WM_DLG_INITIALIZE);
	m_pMonitorDlg->SendMessage(WM_DLG_INITIALIZE);
}

void CStatus::StationInfoUpdate()
{
	m_pStationInfoDlg->SendMessage(WM_WORK_UPDATE);
}