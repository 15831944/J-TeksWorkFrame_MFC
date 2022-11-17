#include "stdafx.h"
#include "SCAlgoBase.h"
#include "SCFidAlgo.h"
#include "SCAlignAlgo.h"
#include "SCHEdgeDetAlgo.h"
#include "SCGapGageAlgo.h"
#include "SCAlignParamDlg.h"
#include "SCHEdgeDetParamDlg.h"
#include "SCHeightAlgo.h"
#include "SCHeightParamDlg.h"
#include "SCGapGageParamDlg.h"
#include "Status.h"
#include "ModelData.h"
#include "SCFidParamDlg.h"
#include "Repository.h"

CStatus * CStatus::m_pInstance = NULL;
CStatus::CStatus()
	: m_iSelCam(0)
	, m_strModelRepository(_T(""))
	, m_nCountOfCam(1)
	, m_pSCInspPacket(NULL)
	, m_pModelData(NULL)
	, m_nInspTypeToAdd(0)
	, m_bIsOffLine(FALSE)
	, m_ProductName(_T("XFMS"))
	, m_opMode(OPSTOP)
	, m_strIMGSaveFolder(_T(""))
	, m_MsgWndOpen(FALSE)
	, m_nCountOfSensors(2)
	, m_bSaveTestImage(FALSE)
	, m_bDrawCross(FALSE)
	, m_bUseDelOldImg(0)
	, m_nDaysDel(0)
	, m_strRemoteImgName(_T(""))
	, m_ulGraphXRange(21600)
	, m_ulGraphYMax(1700)
	, m_ulGraphYMin(0)
	, m_ulDispMode(0)
	, m_strAVISaveFolder(_T(""))
	, m_nAutoMarkPos(0)
	, m_strMarkToAdd(_T(""))
	, m_pRepository(NULL)
	, m_bUseImgEnhance(FALSE)
	, m_bIsAVIRecording(FALSE)
	, m_wlvLow(0)
	, m_wlvHigh(65535)
	, m_bImgSaveFormatJPG(FALSE)
	, m_bImgSaveFormatTIF(FALSE)
	, m_ulPlcReadCycleDelay_ms(50)
	, m_ulPlcReadCMDHoldingTime(1020)
{
}


CStatus::~CStatus()
{
}


CStatus * CStatus::Instance()
{
	if (!m_pInstance)m_pInstance = new CStatus;
	return m_pInstance;
}

void CStatus::SetOPMode(tOpMode opMode)
{
	m_opMode = opMode;
	switch (m_opMode)
	{
	case OPAUTO:
		break;
	}
}

int CStatus::GetOpMode(void)
{
	return m_opMode;
}

void CStatus::SetSelCam(int nCam)
{
	m_iSelCam = nCam;
}


int CStatus::GetSelCamNo()
{
	return m_iSelCam;
}


CString CStatus::GetModelRepository()
{
	return m_strModelRepository;
}


void CStatus::SetModelRepository(TCHAR * szDataSaveFolder)
{
	m_strModelRepository = szDataSaveFolder;
}

CString CStatus::GetModelName()
{
	return m_strModelName;
}


void CStatus::SetModelName(TCHAR * szModelName)
{
	m_strModelName = szModelName;
}


int CStatus::SaveCFG()
{
	CCfgFile *pCfgFile = CCfgFile::Instance();
	pCfgFile->Open();
	pCfgFile->SetDepth(2);
	pCfgFile->Set(m_iBaudRate, _T("COMSET"), _T("BaudRate"));
	pCfgFile->Set(m_lIPAddr[0], _T("TempSensorIP"), _T("Addr1"));
	pCfgFile->Set(m_lIPAddr[1], _T("TempSensorIP"), _T("Addr2"));
	pCfgFile->Set(m_lIPAddr[2], _T("TempSensorIP"), _T("Addr3"));
	pCfgFile->Set(m_lIPAddr[3], _T("TempSensorIP"), _T("Addr4"));

	pCfgFile->Set(m_ulGraphXRange, _T("Graph"), _T("GraphXRange"));
	pCfgFile->Set(m_ulGraphYMax, _T("Graph"), _T("GraphYMax"));
	pCfgFile->Set(m_ulGraphYMin, _T("Graph"), _T("GraphYMIn"));
	pCfgFile->Set(m_ulDispMode, _T("VISION"), _T("DISPLAYMODE"));
	pCfgFile->Set(m_bUseImgEnhance, _T("VISION"), _T("IMGEnhance"));
	pCfgFile->Set(m_wlvLow, _T("VISION"), _T("WindowLevelLow0"));
	pCfgFile->Set(m_wlvHigh, _T("VISION"), _T("WindowLevelHigh0"));

	//! W/L 프리셋 세이브
	pCfgFile->Set(m_wlnLowPresetArr[0], _T("VISION"), _T("WindowLevelLow0"));
	pCfgFile->Set(m_wlnHighPresetArr[0], _T("VISION"), _T("WindowLevelHigh0"));
	pCfgFile->Set(m_wlnLowPresetArr[1], _T("VISION"), _T("WindowLevelLow1"));
	pCfgFile->Set(m_wlnHighPresetArr[1], _T("VISION"), _T("WindowLevelHigh1"));
	pCfgFile->Set(m_wlnLowPresetArr[2], _T("VISION"), _T("WindowLevelLow2"));
	pCfgFile->Set(m_wlnHighPresetArr[2], _T("VISION"), _T("WindowLevelHigh2"));
	pCfgFile->Set(m_wlnLowPresetArr[3], _T("VISION"), _T("WindowLevelLow3"));
	pCfgFile->Set(m_wlnHighPresetArr[3], _T("VISION"), _T("WindowLevelHigh3"));
	pCfgFile->Set(m_wlnLowPresetArr[4], _T("VISION"), _T("WindowLevelLow4"));
	pCfgFile->Set(m_wlnHighPresetArr[4], _T("VISION"), _T("WindowLevelHigh4"));

	pCfgFile->Set(m_strComportNo.GetBuffer(), _T("COMSET"), _T("ComportNo"));
	pCfgFile->Set(m_nCountOfCam, _T("CamSet"), _T("nCountOfInstall"));
	pCfgFile->Set(m_nCountOfSensors, _T("TempSensor"), _T("nCountOfSensors"));
	pCfgFile->Set(m_strSensorNamePx[0].GetBuffer(), _T("TempSensor"), _T("P1Name"));
	pCfgFile->Set(m_strSensorNamePx[1].GetBuffer(), _T("TempSensor"), _T("P2Name"));
	pCfgFile->Set(m_strSensorNamePx[2].GetBuffer(), _T("TempSensor"), _T("P3Name"));
	pCfgFile->Set(m_strSensorNamePx[3].GetBuffer(), _T("TempSensor"), _T("P4Name"));
	pCfgFile->Set(m_strSensorNamePx[4].GetBuffer(), _T("TempSensor"), _T("P5Name"));
	pCfgFile->Set(m_strSensorNamePx[5].GetBuffer(), _T("TempSensor"), _T("P6Name"));

	pCfgFile->Set(m_bUseDelOldImg, _T("DELOLDIMG"), _T("bIsUse"));
	pCfgFile->Set(m_nDaysDel, _T("DELOLDIMG"), _T("nDaysToDel"));

	pCfgFile->Set(m_strModelRepository.GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	pCfgFile->Set(m_strModelName.GetBuffer(), _T("Environment"), _T("ModelName"));
	pCfgFile->Set(m_strIMGSaveFolder.GetBuffer(), _T("Environment"), _T("IMGSaveFolder"));
	pCfgFile->Set(m_strAVISaveFolder.GetBuffer(), _T("Environment"), _T("AVISaveFolder"));

	//! plc 설정추가
	pCfgFile->Set(m_strPlcIp.GetBuffer(), _T("PLC"), _T("IP"));
	pCfgFile->Set(m_strPlcPort.GetBuffer(), _T("PLC"), _T("PORT"));
	pCfgFile->Set(m_ulPlcReadCycleDelay_ms, _T("PLC"), _T("CYCLE_DELAY"));
	pCfgFile->Set(m_ulPlcReadCMDHoldingTime, _T("PLC"), _T("HOLDING_TIME"));

	//! ImageSaveFormat 저장
	pCfgFile->Set(m_bImgSaveFormatJPG, _T("ImageSaveFormat"), _T("JPG"));
	pCfgFile->Set(m_bImgSaveFormatTIF, _T("ImageSaveFormat"), _T("TIF"));

	//! Etc 저장
	pCfgFile->Set(m_bSaveWithRaw, _T("Etc"), _T("SaveWithRaw"));
	pCfgFile->Set(m_bShowGrayAvg, _T("Etc"), _T("ShowGrayAvg"));

	//! Info 저장
	/*pCfgFile->Set(m_strComp.GetBuffer(), _T("Info"), _T("Comp"));
	pCfgFile->Set(m_strSEPC.GetBuffer(), _T("Info"), _T("SEPC"));
	pCfgFile->Set(m_strSize.GetBuffer(), _T("Info"), _T("Size"));
	pCfgFile->Set(m_strSerial.GetBuffer(), _T("Info"), _T("Serial"));
	pCfgFile->Set(m_strPipeNo.GetBuffer(), _T("Info"), _T("PipeNo"));
	pCfgFile->Set(m_strInfoLine1.GetBuffer(), _T("Info"), _T("Line1"));
	pCfgFile->Set(m_strInfoLine2.GetBuffer(), _T("Info"), _T("Line2"));
	pCfgFile->Set(m_avg, _T("Info"), _T("Average"));*/

	pCfgFile->Set(m_dPixelXSize_mm, _T("Info"), _T("XPixelSize"));
	pCfgFile->Set(m_dPixelYSize_mm, _T("Info"), _T("YPixelSize"));

	pCfgFile->Set(m_pipeStartPos_mm, _T("Info"), _T("PipeStartPosition"));
	pCfgFile->Set(m_pipeSectEachDistance_mm, _T("Info"), _T("PipeSectionEachDistance"));

	pCfgFile->SetDepth(3);
	pCfgFile->Set(m_LightBrightness[0], _T("Light"), _T("Brightness"), _T("Channel0"));
	pCfgFile->Set(m_LightBrightness[1], _T("Light"), _T("Brightness"), _T("Channel1"));
	pCfgFile->Set(m_LightBrightness[2], _T("Light"), _T("Brightness"), _T("Channel2"));
	pCfgFile->Set(m_LightBrightness[3], _T("Light"), _T("Brightness"), _T("Channel3"));

	pCfgFile->Flush();
	return 0;
}

int CStatus::LoadCFG()
{
	CCfgFile *pCfgFile = CCfgFile::Instance();
	pCfgFile->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pCfgFile->Open();
	pCfgFile->SetDepth(2);
	m_nCountOfCam = pCfgFile->Get(1L, _T("CamSet"), _T("nCountOfInstall"));
	m_nCountOfSensors = pCfgFile->Get(2L, _T("TempSensor"), _T("nCountOfSensors"));
	m_strSensorNamePx[0] = pCfgFile->Get(_T("P1"), _T("TempSensor"), _T("P1Name"));
	m_strSensorNamePx[1] = pCfgFile->Get(_T("P2"), _T("TempSensor"), _T("P2Name"));
	m_strSensorNamePx[2] = pCfgFile->Get(_T("P3"), _T("TempSensor"), _T("P3Name"));
	m_strSensorNamePx[3] = pCfgFile->Get(_T("P4"), _T("TempSensor"), _T("P4Name"));
	m_strSensorNamePx[4] = pCfgFile->Get(_T("P5"), _T("TempSensor"), _T("P5Name"));
	m_strSensorNamePx[5] = pCfgFile->Get(_T("P6"), _T("TempSensor"), _T("P6Name"));

	m_bUseDelOldImg = pCfgFile->Get(0L, _T("DELOLDIMG"), _T("bIsUse"));
	m_nDaysDel = pCfgFile->Get(100L, _T("DELOLDIMG"), _T("nDaysToDel"));

	m_iBaudRate = pCfgFile->Get(9600L, _T("COMSET"), _T("BaudRate"));

	m_lIPAddr[0] = pCfgFile->Get(127L, _T("TempSensorIP"), _T("Addr1"));
	m_lIPAddr[1] = pCfgFile->Get(0L, _T("TempSensorIP"), _T("Addr2"));
	m_lIPAddr[2] = pCfgFile->Get(0L, _T("TempSensorIP"), _T("Addr3"));
	m_lIPAddr[3] = pCfgFile->Get(1L, _T("TempSensorIP"), _T("Addr4"));

	m_ulGraphXRange = pCfgFile->Get(21600L, _T("Graph"), _T("GraphXRange"));
	m_ulGraphYMax = pCfgFile->Get(1700L, _T("Graph"), _T("GraphYMax"));
	m_ulGraphYMin = pCfgFile->Get(0L, _T("Graph"), _T("GraphYMIn"));
	m_ulDispMode = pCfgFile->Get(0L, _T("VISION"), _T("DISPLAYMODE"));
	m_bUseImgEnhance = pCfgFile->Get(1L, _T("VISION"), _T("IMGEnhance"));
	m_wlvLow = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelLow0"));
	m_wlvHigh = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelHigh0"));

	//! W/L 프리셋 로드
	m_wlnLowPresetArr[0] = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelLow0"));
	m_wlnHighPresetArr[0] = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelHigh0"));
	m_wlnLowPresetArr[1] = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelLow1"));
	m_wlnHighPresetArr[1] = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelHigh1"));
	m_wlnLowPresetArr[2] = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelLow2"));
	m_wlnHighPresetArr[2] = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelHigh2"));
	m_wlnLowPresetArr[3] = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelLow3"));
	m_wlnHighPresetArr[3] = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelHigh3"));
	m_wlnLowPresetArr[4] = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelLow4"));
	m_wlnHighPresetArr[4] = pCfgFile->Get(0L, _T("VISION"), _T("WindowLevelHigh4"));

	//! GetGvAvgRegion 로드
	m_gvAvgRegionWidth = pCfgFile->Get(0L, _T("VISION"), _T("GetGvAvgRegionWidth"));
	m_gvAvgRegionHeight = pCfgFile->Get(0L, _T("VISION"), _T("GetGvAvgRegionHeight"));

	m_strModelRepository = pCfgFile->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	m_strComportNo = pCfgFile->Get(_T("COM1"), _T("COMSET"), _T("ComportNo"));
	m_strIMGSaveFolder = pCfgFile->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("IMGSaveFolder"));
	m_strAVISaveFolder = pCfgFile->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("AVISaveFolder"));

	m_strModelName = pCfgFile->Get(_T("XNPIJob"), _T("Environment"), _T("ModelName"));

	//! plc 설정 로드
	m_strPlcIp = pCfgFile->Get(_T("192.168.0.2"), _T("PLC"), _T("IP"));
	m_strPlcPort = pCfgFile->Get(_T("502"), _T("PLC"), _T("PORT"));
	m_ulPlcReadCycleDelay_ms = pCfgFile->Get(50L, _T("PLC"), _T("CYCLE_DELAY"));
	m_ulPlcReadCMDHoldingTime = pCfgFile->Get(50L, _T("PLC"), _T("HOLDING_TIME"));

	//! ImageSaveFormat 로드
	m_bImgSaveFormatJPG = pCfgFile->Get(1L, _T("ImageSaveFormat"), _T("JPG"));
	m_bImgSaveFormatTIF = pCfgFile->Get(1L, _T("ImageSaveFormat"), _T("TIF"));

	m_bSaveWithRaw = pCfgFile->Get(1L, _T("Etc"), _T("SaveWithRaw"));
	m_bShowGrayAvg = pCfgFile->Get(1L, _T("Etc"), _T("ShowGrayAvg"));

	//! Info 로드
	m_strComp = pCfgFile->Get(_T("None"), _T("Info"), _T("Comp"));
	m_strSEPC = pCfgFile->Get(_T("None"), _T("Info"), _T("SEPC"));
	m_strSize = pCfgFile->Get(_T("None"), _T("Info"), _T("Size"));
	m_strSerial = pCfgFile->Get(_T("None"), _T("Info"), _T("Serial"));
	m_strPipeNo = pCfgFile->Get(_T("None"), _T("Info"), _T("PipeNo"));
	m_strInfoLine1 = pCfgFile->Get(_T("None"), _T("Info"), _T("Line1"));
	m_strInfoLine2 = pCfgFile->Get(_T("None"), _T("Info"), _T("Line2"));
	m_avg = pCfgFile->Get(1L, _T("Info"), _T("Average"));
	m_avgMinLimit = pCfgFile->Get(1L, _T("Info"), _T("AvgMinimumLimit"));
	
	m_dPixelXSize_mm = pCfgFile->Get(0.0f, _T("Info"), _T("XPixelSize"));		//181016 추가
	m_dPixelYSize_mm = pCfgFile->Get(0.0f, _T("Info"), _T("YPixelSize"));		//181016 추가

	m_pipeStartPos_mm = pCfgFile->Get(0.0f, _T("Info"), _T("PipeStartPosition"));
	m_pipeSectEachDistance_mm = pCfgFile->Get(0.0f, _T("Info"), _T("PipeSectionEachDistance"));

	m_strLastOpendFolder = pCfgFile->Get(_T("None"), _T("Info"), _T("LastOpendFolder"));	//m_strLastOpendFolder는 로드만 구현 save는 SaveLastOpendFolder()사용

	pCfgFile->SetDepth(3);
	m_LightBrightness[0] = pCfgFile->Get(1023L, _T("Light"), _T("Brightness"), _T("Channel0"));
	m_LightBrightness[1] = pCfgFile->Get(1023L, _T("Light"), _T("Brightness"), _T("Channel1"));
	m_LightBrightness[2] = pCfgFile->Get(1023L, _T("Light"), _T("Brightness"), _T("Channel2"));
	m_LightBrightness[3] = pCfgFile->Get(1023L, _T("Light"), _T("Brightness"), _T("Channel3"));

	return 0;
}


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

CXFMSPacket * CStatus::GetComm()
{
	return m_pSCInspPacket;
}


void CStatus::SetComm(CXFMSPacket * pSCInspPacket)
{
	m_pSCInspPacket = pSCInspPacket;
}


int CStatus::GetRVOption()
{
	return 0;
}


CModelData * CStatus::GetModelData()
{
	return m_pModelData;
}


void CStatus::SetModelData(CModelData * pModelData)
{
	m_pModelData = pModelData;
}


void CStatus::SetCurAddAlgoIndex(int nInspType)
{
	m_nInspTypeToAdd = nInspType;
}


int CStatus::GetCurAlgoToAddIndex()
{
	return m_nInspTypeToAdd;
}

CSCAlgoBase * CStatus::GetCurAlgoToAdd()
{
	CSCAlgoBase *pAlgo = NULL;
	pAlgo = GetAlgoInfo(m_nInspTypeToAdd);
	if (m_nInspTypeToAdd == INSPFID)
	{
		((CSCFidAlgo *)pAlgo)->m_nCamNo = GetSelCamNo();
	}
	return pAlgo;
}

CSCAlgoBase * CStatus::GetAlgoInfo(int nInspType, CXmlNode* pNode)
{
	CSCAlgoBase * pAlgo = NULL;
	switch (nInspType)
	{
	case INSPFID:
	{
		pAlgo = new CSCFidAlgo;
		CString strPathName = GetModelRepository() + _T("\\") + GetModelName();
		pAlgo->m_pParamDlg = CSCFidParamDlg::Instance();
		if (pNode)
		{
			((CSCFidAlgo *)pAlgo)->m_iMatRate = pNode->ReadLong(60, _T("FidScore"));
			((CSCFidAlgo *)pAlgo)->m_nCamNo = pNode->ReadLong(0, _T("CamNo"));
			((CSCFidAlgo *)pAlgo)->LoadFid(strPathName.GetBuffer(), ((CSCFidAlgo *)pAlgo)->m_nCamNo);
			((CSCFidAlgo *)pAlgo)->m_SearchRangeFactor.dblCX = pNode->ReadDouble(2.0, _T("SearchRangeFactorX"));
			((CSCFidAlgo *)pAlgo)->m_SearchRangeFactor.dblCY = pNode->ReadDouble(2.0, _T("SearchRangeFactorY"));
		}
	}
	break;
	case INSPHEDET:
		pAlgo = new CSCHEdgeDetAlgo;
		if (pNode)
		{//0 top->buttom, 1 buttom->top
			((CSCHEdgeDetAlgo *)pAlgo)->m_iHEdgeScanDir = pNode->ReadLong(0, _T("HEdgeScnDir"));
			((CSCHEdgeDetAlgo *)pAlgo)->m_bUseCenterLine = pNode->ReadLong(0, _T("HEdgeUseCntrLine"));
		}
		pAlgo->m_pParamDlg = CSCHEdgeDetParamDlg::Instance();
		break;
	case INSPALIGN:
		pAlgo = new CSCAlignAlgo;
		if (pNode)
		{
			((CSCAlignAlgo *)pAlgo)->m_iScanDir = pNode->ReadLong(0, _T("ScanDir"));
			((CSCAlignAlgo *)pAlgo)->m_iCriteria = pNode->ReadLong(10, _T("AlignCriteria"));
		}
		pAlgo->m_pParamDlg = CSCAlignParamDlg::Instance();
		break;
	case INSPHEIGHT:
		pAlgo = new CSCHeightAlgo;
		if (pNode)
		{//0 top->buttom, 1 buttom->top
			((CSCHeightAlgo *)pAlgo)->m_iCriteriaLow = pNode->ReadLong(0, _T("HeightCriteriaLOW"));
			((CSCHeightAlgo *)pAlgo)->m_iCriteriaHi = pNode->ReadLong(0, _T("HeightCriteriaHi"));
			((CSCHeightAlgo *)pAlgo)->m_iScanDir = pNode->ReadLong(0, _T("HeightScanDir"));
		}
		pAlgo->m_pParamDlg = CSCHeightParamDlg::Instance();
		break;
	case INSPGAPGAGE:
		pAlgo = new CSCGapGageAlgo;
		if (pNode)
		{
			((CSCGapGageAlgo *)pAlgo)->m_iCriteria = pNode->ReadLong(5, _T("GapGageCriteria"));
		}
		pAlgo->m_pParamDlg = CSCGapGageParamDlg::Instance();

		break;
	}
	if (!pAlgo)
		AfxMessageBox(_T("Undefined Algorithm!!"));
	else
		pAlgo->m_nInspType = nInspType;

	return pAlgo;
}

void CStatus::SetAlgoInfo(int nInspType, CSCAlgoBase *pAlgo, CXmlNode* pNode)
{
	CString ModelFolder;
	switch (nInspType)
	{
	case INSPFID:
	{
		CString strPathName = GetModelRepository() + _T("\\") + GetModelName();
		pNode->SetLong(_T("FidScore"), ((CSCFidAlgo *)pAlgo)->m_iMatRate);
		pNode->SetLong(_T("CamNo"), ((CSCFidAlgo *)pAlgo)->m_nCamNo);

		pNode->SetDouble((_T("SearchRangeFactorX"), (CSCFidAlgo *)pAlgo)->m_SearchRangeFactor.dblCX);
		pNode->SetDouble((_T("SearchRangeFactorY"), (CSCFidAlgo *)pAlgo)->m_SearchRangeFactor.dblCY);

		if (((CSCFidAlgo *)pAlgo)->IsEmpty())AfxMessageBox(_T("You don't registrate the fiducial!!"));
		else
			((CSCFidAlgo *)pAlgo)->SaveFid(strPathName.GetBuffer(), ((CSCFidAlgo *)pAlgo)->m_nCamNo);
	}
	break;
	case INSPHEDET:
		pNode->SetLong(_T("HEdgeScnDir"), ((CSCHEdgeDetAlgo *)pAlgo)->m_iHEdgeScanDir);
		pNode->SetLong(_T("HEdgeUseCntrLine"), ((CSCHEdgeDetAlgo *)pAlgo)->m_bUseCenterLine);
		break;
	case INSPALIGN:
		pNode->SetLong(_T("ScanDir"), ((CSCAlignAlgo *)pAlgo)->m_iScanDir);
		pNode->SetLong(_T("AlignCriteria"), ((CSCAlignAlgo *)pAlgo)->m_iCriteria);
		break;
	case INSPHEIGHT:
		pNode->SetLong(_T("HeightCriteriaLOW"), ((CSCHeightAlgo *)pAlgo)->m_iCriteriaLow);
		pNode->SetLong(_T("HeightCriteriaHi"), ((CSCHeightAlgo *)pAlgo)->m_iCriteriaHi);
		pNode->SetLong(_T("HeightScanDir"), ((CSCHeightAlgo *)pAlgo)->m_iScanDir);
		break;
	case INSPGAPGAGE:
		pNode->SetLong(_T("GapGageCriteria"), ((CSCGapGageAlgo *)pAlgo)->m_iCriteria);
		break;

	}
}



int CStatus::GetCamNo()
{
	return 0;
}


CdblSize CStatus::GetPixelCalData(int iCamIndex)
{
	return CdblSize(0.0, 0.0);
}



CString CStatus::GetTempSensorIPString()
{
	CString strIP;
	strIP.Format(_T("%d.%d.%d.%d"), m_lIPAddr[0], m_lIPAddr[1], m_lIPAddr[2], m_lIPAddr[3]);
	return strIP;
}


int CStatus::GetAutoMarkPos()
{
	return m_nAutoMarkPos;
}


void CStatus::SetAutoMarkPos(int nPos)
{
	m_nAutoMarkPos = nPos;
}


void CStatus::IncAutoMarkPos()
{
	m_nAutoMarkPos++;
}



void CStatus::SetMarkString(CString strToAdd)
{
	m_strMarkToAdd = strToAdd;
}


CString CStatus::GetMarkString()
{
	CString strToReturn;
	strToReturn = m_strMarkToAdd;
	m_strMarkToAdd.Empty();
	return strToReturn;
}


CRepository * CStatus::GetRepository()
{
	return m_pRepository;
}


void CStatus::SetRepository(CRepository * pRepository)
{
	m_pRepository = pRepository;
}

//! PipeInfo는 save 여부와는 상관없이 마지막 입력정보를 유지해야 해서 추가.
void CStatus::SaveOnlyPipeInfoCFG()
{
	CCfgFile *pCfgFile = CCfgFile::Instance();
	pCfgFile->Open();
	pCfgFile->SetDepth(2);

	pCfgFile->Set(m_strComp.GetBuffer(), _T("Info"), _T("Comp"));
	pCfgFile->Set(m_strSEPC.GetBuffer(), _T("Info"), _T("SEPC"));
	pCfgFile->Set(m_strSize.GetBuffer(), _T("Info"), _T("Size"));
	pCfgFile->Set(m_strSerial.GetBuffer(), _T("Info"), _T("Serial"));
	pCfgFile->Set(m_strPipeNo.GetBuffer(), _T("Info"), _T("PipeNo"));
	pCfgFile->Set(m_strInfoLine1.GetBuffer(), _T("Info"), _T("Line1"));
	pCfgFile->Set(m_strInfoLine2.GetBuffer(), _T("Info"), _T("Line2"));
	pCfgFile->Set(m_avg, _T("Info"), _T("Average"));

	pCfgFile->Flush();
}

void CStatus::SaveLastOpendFolder()
{
	CCfgFile *pCfgFile = CCfgFile::Instance();
	pCfgFile->Open();
	pCfgFile->SetDepth(2);

	pCfgFile->Set(m_strLastOpendFolder.GetBuffer(), _T("Info"), _T("LastOpendFolder"));

	pCfgFile->Flush();
}

