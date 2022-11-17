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
#include "SCInspIO.h"

CStatus * CStatus::m_pInstance = NULL;
CStatus::CStatus()
	: m_iSelCam(0)
	, m_strModelRepository(_T(""))
	, m_nCountOfCam(3)
	, m_pSCInspPacket(NULL)
	, m_pModelData(NULL)
	, m_nInspTypeToAdd(0)
	, m_bIsOffLine(FALSE)
	, m_ProductName(_T("SCInspector"))
	, m_opMode(OPSTOP)
	, m_strNGSaveFolder(_T(""))
	, m_MsgWndOpen(FALSE)
	, m_nCountOfStage(0)
	, m_nCountOfPos(6)
	, m_bSaveTestImage(FALSE)
	, m_bDrawCross(FALSE)
	, m_bUseDelOldImg(0)
	, m_nDaysDel(0)
	, m_nCountOfCamRemote(0)
	, m_strRemoteCamFolder(_T(""))
	, m_bUseRemoteCamUpdate(0)
	, m_bSaveAllImage(0)
	, m_strRemoteImgName(_T(""))
{
	memset(m_camInspStage, 0, sizeof(m_camInspStage));
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
		CSCInspIO::Instance()->SetAllOutputClear();
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
	pCfgFile->Set(m_lIPAddr[0], _T("RemoteIP"), _T("Addr1"));
	pCfgFile->Set(m_lIPAddr[1], _T("RemoteIP"), _T("Addr2"));
	pCfgFile->Set(m_lIPAddr[2], _T("RemoteIP"), _T("Addr3"));
	pCfgFile->Set(m_lIPAddr[3], _T("RemoteIP"), _T("Addr4"));
	pCfgFile->Set(m_bUseRemoteCamUpdate, _T("RemoteIP"), _T("UseRemoteCamUpdate"));

	pCfgFile->Set(m_strComportNo.GetBuffer(), _T("COMSET"), _T("ComportNo"));
	pCfgFile->Set(m_nCountOfCam, _T("CamSet"), _T("nCountOfInstall"));
	pCfgFile->Set(m_nCountOfCamRemote, _T("CamSet"), _T("nCountOfRemoteCam"));
	pCfgFile->Set(m_nCountOfStage, _T("Stage"), _T("nCountOfInstall"));
	pCfgFile->Set(m_nCountOfPos, _T("Stage"), _T("nCountOfPos"));

	pCfgFile->Set(m_bUseDelOldImg, _T("DELOLDIMG"), _T("bIsUse"));
	pCfgFile->Set(m_nDaysDel, _T("DELOLDIMG"), _T("nDaysToDel"));

	pCfgFile->Set(m_strModelRepository.GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	pCfgFile->Set(m_strModelName.GetBuffer(), _T("Environment"), _T("ModelName"));
	pCfgFile->Set(m_strNGSaveFolder.GetBuffer(), _T("Environment"), _T("NGSaveFolder"));
	pCfgFile->Set(m_strRemoteCamFolder.GetBuffer(), _T("Environment"), _T("RemoteCamFolder"));
	pCfgFile->Set(m_bSaveAllImage, _T("Environment"), _T("SaveAllImage"));


	pCfgFile->SetDepth(3);
	pCfgFile->Set(m_camInspStage[0], _T("CamSet"), _T("CamNo1"), _T("InspStage"));
	pCfgFile->Set(m_camInspStage[1], _T("CamSet"), _T("CamNo2"), _T("InspStage"));
	pCfgFile->Set(m_camInspStage[2], _T("CamSet"), _T("CamNo3"), _T("InspStage"));
	pCfgFile->Set(m_camInspStage[3], _T("CamSet"), _T("CamNo4"), _T("InspStage"));

	pCfgFile->Set(m_CamFov[0].dblCX, _T("CamSet"), _T("CamNo1"), _T("FovX"));
	pCfgFile->Set(m_CamFov[0].dblCY, _T("CamSet"), _T("CamNo1"), _T("FovY"));
	pCfgFile->Set(m_CamFov[1].dblCX, _T("CamSet"), _T("CamNo2"), _T("FovX"));
	pCfgFile->Set(m_CamFov[1].dblCY, _T("CamSet"), _T("CamNo2"), _T("FovY"));
	pCfgFile->Set(m_CamFov[2].dblCX, _T("CamSet"), _T("CamNo3"), _T("FovX"));
	pCfgFile->Set(m_CamFov[2].dblCY, _T("CamSet"), _T("CamNo3"), _T("FovY"));
	pCfgFile->Set(m_CamFov[3].dblCX, _T("CamSet"), _T("CamNo4"), _T("FovX"));
	pCfgFile->Set(m_CamFov[3].dblCY, _T("CamSet"), _T("CamNo4"), _T("FovY"));

	pCfgFile->Set(m_lCamExpTime[0], _T("CamSet"), _T("CamNo1"), _T("ExposeTime"));
	pCfgFile->Set(m_lCamExpTime[1], _T("CamSet"), _T("CamNo2"), _T("ExposeTime"));
	pCfgFile->Set(m_lCamExpTime[2], _T("CamSet"), _T("CamNo3"), _T("ExposeTime"));
	pCfgFile->Set(m_lCamExpTime[3], _T("CamSet"), _T("CamNo4"), _T("ExposeTime"));

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
	m_nCountOfCam = pCfgFile->Get(0L, _T("CamSet"), _T("nCountOfInstall"));
	m_nCountOfCamRemote = pCfgFile->Get(0L, _T("CamSet"), _T("nCountOfRemoteCam"));
	m_nCountOfStage = pCfgFile->Get(1L, _T("Stage"), _T("nCountOfInstall"));
	m_nCountOfPos = pCfgFile->Get(6L, _T("Stage"), _T("nCountOfPos"));

	m_bUseDelOldImg = pCfgFile->Get(0L, _T("DELOLDIMG"), _T("bIsUse"));
	m_nDaysDel = pCfgFile->Get(100L, _T("DELOLDIMG"), _T("nDaysToDel"));
	
	m_iBaudRate = pCfgFile->Get(9600L, _T("COMSET"), _T("BaudRate"));

	m_lIPAddr[0] = pCfgFile->Get(127L, _T("RemoteIP"), _T("Addr1"));
	m_lIPAddr[1] = pCfgFile->Get(0L, _T("RemoteIP"), _T("Addr2"));
	m_lIPAddr[2] = pCfgFile->Get(0L, _T("RemoteIP"), _T("Addr3"));
	m_lIPAddr[3] = pCfgFile->Get(1L, _T("RemoteIP"), _T("Addr4"));
	m_bUseRemoteCamUpdate = pCfgFile->Get(0L, _T("RemoteIP"), _T("UseRemoteCamUpdate"));

	m_strModelRepository = pCfgFile->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	m_strComportNo = pCfgFile->Get(_T("COM1"), _T("COMSET"), _T("ComportNo"));
	m_strNGSaveFolder = pCfgFile->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("NGSaveFolder"));
	m_strRemoteCamFolder = pCfgFile->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("RemoteCamFolder"));
	m_strModelName = pCfgFile->Get(_T("SCInsp"), _T("Environment"), _T("ModelName"));
	m_bSaveAllImage = pCfgFile->Get(0L, _T("Environment"), _T("SaveAllImage"));
	pCfgFile->SetDepth(3);
	m_camInspStage[0] = pCfgFile->Get(0L, _T("CamSet"), _T("CamNo1"), _T("InspStage"));
	m_camInspStage[1] = pCfgFile->Get(0L, _T("CamSet"), _T("CamNo2"), _T("InspStage"));
	m_camInspStage[2] = pCfgFile->Get(0L, _T("CamSet"), _T("CamNo3"), _T("InspStage"));
	m_camInspStage[3] = pCfgFile->Get(0L, _T("CamSet"), _T("CamNo4"), _T("InspStage"));

	m_CamFov[0].dblCX = pCfgFile->Get(0.0, _T("CamSet"), _T("CamNo1"), _T("FovX"));
	m_CamFov[0].dblCY = pCfgFile->Get(0.0, _T("CamSet"), _T("CamNo1"), _T("FovY"));
	m_CamFov[1].dblCX = pCfgFile->Get(0.0, _T("CamSet"), _T("CamNo2"), _T("FovX"));
	m_CamFov[1].dblCY = pCfgFile->Get(0.0, _T("CamSet"), _T("CamNo2"), _T("FovY"));
	m_CamFov[2].dblCX = pCfgFile->Get(0.0, _T("CamSet"), _T("CamNo3"), _T("FovX"));
	m_CamFov[2].dblCY = pCfgFile->Get(0.0, _T("CamSet"), _T("CamNo3"), _T("FovY"));
	m_CamFov[3].dblCX = pCfgFile->Get(0.0, _T("CamSet"), _T("CamNo4"), _T("FovX"));
	m_CamFov[3].dblCY = pCfgFile->Get(0.0, _T("CamSet"), _T("CamNo4"), _T("FovY"));

	m_lCamExpTime[0] = pCfgFile->Get(2000L, _T("CamSet"), _T("CamNo1"), _T("ExposeTime"));
	m_lCamExpTime[1] = pCfgFile->Get(2000L, _T("CamSet"), _T("CamNo2"), _T("ExposeTime"));
	m_lCamExpTime[2] = pCfgFile->Get(2000L, _T("CamSet"), _T("CamNo3"), _T("ExposeTime"));
	m_lCamExpTime[3] = pCfgFile->Get(2000L, _T("CamSet"), _T("CamNo4"), _T("ExposeTime"));

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

CSCInspPacket * CStatus::GetComm()
{
	return m_pSCInspPacket;
}


void CStatus::SetComm(CSCInspPacket * pSCInspPacket)
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
	CSCAlgoBase * pAlgo=NULL;
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
	if( (m_CamFov[iCamIndex].dblCX == 0.0) || (m_CamFov[iCamIndex].dblCY ==0.0))
		return CdblSize(0.0, 0.0);
	CdblSize dblSize;
	dblSize.dblCX = m_CamFov[iCamIndex].dblCX / CAM_HRES;
	dblSize.dblCY = m_CamFov[iCamIndex].dblCY / CAM_VRES;
	return dblSize;
}



CString CStatus::GetRemoteIPString()
{
	CString strIP;
	strIP.Format(_T("%d.%d.%d.%d"), m_lIPAddr[0], m_lIPAddr[1], m_lIPAddr[2], m_lIPAddr[3]);
	return strIP;
}
