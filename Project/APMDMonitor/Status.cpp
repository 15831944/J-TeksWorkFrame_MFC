#include "stdafx.h"
#include "Status.h"
#include "GraphDlg.h"

CStatus::tProgramMode CStatus::m_ProgramMode= PM_REVIEW;
CStatus::tKeylockStatus CStatus::m_KeylockStatus= LK_NO;
CStatus::tDispMode CStatus::m_DispMode = DM_FREQ;
CStatus * CStatus::m_pInstance = NULL;
CStatus::CStatus()
	: m_iBaudRate(9600)
	, m_strComportNo(_T(""))
	, m_strDataSaveFolder(_T(""))
	, m_strDataFileName(_T(""))
	, m_pGraphDlg(NULL)
	, m_nMarkPos(0)
	, m_iUseWarning(0)
	, m_iUseTemp(0)
	, m_iUseP01(0)
	, m_iUseP03(0)
	, m_iUseP05(0)
	, m_iThresP01(0)
	, m_iThresP05(0)
	, m_iThresP03(0)
	, m_iTimeWarningSel(0)
	, m_dblThresTemp(0)
	, m_bOnNavi(false)
	, m_strCalDate(_T(""))
	, m_strSerialNo(_T(""))
	, m_iMaxMarkInExlForm(40)
	, strLastOpenFileName(_T(""))
	, m_strMakeDate(_T(""))
	, m_nAutoMarkPos(1)
	, m_bOverlayMode(false)
	, m_bCompareMode(false)
{
	m_ViewOptions.iParticleType = 1;
	m_ViewOptions.iXGridUnit = 1;
	m_ViewOptions.iYGridUnit = 1;
}


CStatus::~CStatus()
{
}


void CStatus::SetProgramMode(tProgramMode ProgramMode)
{
	m_ProgramMode = ProgramMode;
}

CStatus::tProgramMode CStatus::GetProgramMode()
{
	return m_ProgramMode;
}


BOOL CStatus::SearchKeylock()
{
	////////////////////// USB용 메가락 검사루틴 시작
	unsigned int usbID;
	CString szTemp, szMem;
	usbID = lock_init_usb(8);	// USB용 메가락 찾기, 인수 8은 cpu id의 첫번째 숫자 8~F 까지 사용가능
	if (usbID) {
		//MegaLock is Founded
		if (0x10 != lock_read(0, 0xff))
			return FALSE;
		return TRUE;
	}
	else {
		//MegaLock is not Founded
		return FALSE;
	}
}


void CStatus::SetKeylockStatus(tKeylockStatus KeylockStatus)
{
	m_KeylockStatus = KeylockStatus;
}


CStatus::tKeylockStatus CStatus::GetKeylockStatus()
{
	return m_KeylockStatus;
}


void CStatus::SetDispMode(tDispMode DispMode)
{
	m_DispMode = DispMode;
}


CStatus::tDispMode CStatus::GetDispMode()
{
	return m_DispMode;
}


int CStatus::GetDefault()
{
	CCfgFile *pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pMyCfg->Open();
	m_iBaudRate		= pMyCfg->Get(9600L, _T("COMSET"), _T("BaudRate"));
	m_strComportNo	= pMyCfg->Get(_T("None"), _T("COMSET"), _T("ComportNo"));
	m_strDataSaveFolder = pMyCfg->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	m_strDataFileName	= pMyCfg->Get(_T("APMDLog"), _T("Environment"), _T("DataFileName"));

	m_iUseWarning = pMyCfg->Get(0L, _T("WarningCondition"), _T("UseWarningCondition"));
	m_iUseTemp =	pMyCfg->Get(0L, _T("WarningCondition"), _T("UseTemp"));
	m_iUseP01 =		pMyCfg->Get(0L, _T("WarningCondition"), _T("UseThreshold01"));
	m_iUseP03 =		pMyCfg->Get(0L, _T("WarningCondition"), _T("UseThreshold03"));
	m_iUseP05 =		pMyCfg->Get(0L, _T("WarningCondition"), _T("UseThreshold05"));
	m_iThresP01 =	pMyCfg->Get(0L, _T("WarningCondition"), _T("Threshold01"));
	m_iThresP03 =	pMyCfg->Get(0L, _T("WarningCondition"), _T("Threshold03"));
	m_iThresP05 =	pMyCfg->Get(0L, _T("WarningCondition"), _T("Threshold05"));
	m_dblThresTemp = pMyCfg->Get(20.0, _T("WarningCondition"), _T("ThresholdTemp"));
	m_iTimeWarningSel = pMyCfg->Get(0L, _T("WarningCondition"), _T("TimeWarningSel"));
	m_iMaxMarkInExlForm = pMyCfg->Get(40L, _T("ExlReport"), _T("MaxMark"));
	return 0;
}

void CStatus::SetDefault()
{
	CCfgFile *pCfgFile = CCfgFile::Instance();
	pCfgFile->Open();
	pCfgFile->Set(m_iBaudRate, _T("COMSET"), _T("BaudRate"));
	pCfgFile->Set(m_strComportNo.GetBuffer(), _T("COMSET"), _T("ComportNo"));
	pCfgFile->Set(m_strDataSaveFolder.GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	pCfgFile->Set(m_strDataFileName.GetBuffer(), _T("Environment"), _T("DataFileName"));

	pCfgFile->Set(m_iUseWarning, _T("WarningCondition"), _T("UseWarningCondition"));
	pCfgFile->Set(m_iUseTemp, _T("WarningCondition"), _T("UseTemp"));
	pCfgFile->Set(m_iUseP01, _T("WarningCondition"), _T("UseThreshold01"));
	pCfgFile->Set(m_iUseP03, _T("WarningCondition"), _T("UseThreshold03"));
	pCfgFile->Set(m_iUseP05, _T("WarningCondition"), _T("UseThreshold05"));
	pCfgFile->Set(m_iThresP01, _T("WarningCondition"), _T("Threshold01"));
	pCfgFile->Set(m_iThresP03, _T("WarningCondition"), _T("Threshold03"));
	pCfgFile->Set(m_iThresP05, _T("WarningCondition"), _T("Threshold05"));
	pCfgFile->Set(m_dblThresTemp, _T("WarningCondition"), _T("ThresholdTemp"));
	pCfgFile->Set(m_iTimeWarningSel, _T("WarningCondition"), _T("TimeWarningSel"));
	pCfgFile->Set(m_iMaxMarkInExlForm, _T("ExlReport"), _T("MaxMark"));
	pCfgFile->Flush();
}

int CStatus::GetBaudRate()
{
	return m_iBaudRate;
}


void CStatus::SetBaudRate(int iBaudRate)
{
	m_iBaudRate = iBaudRate;
}


CStatus * CStatus::Instance()
{
	if(m_pInstance == NULL)m_pInstance = new CStatus;
	return m_pInstance;
}


void CStatus::SetComportNo(TCHAR* szComportNo)
{
	m_strComportNo = szComportNo;
}


CString CStatus::GetComportNo()
{
	return m_strComportNo;
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


void CStatus::SetViewOptions(tViewOptions ViewOptions)
{
	m_ViewOptions = ViewOptions;
	if (m_pGraphDlg)m_pGraphDlg->Invalidate();
}


int CStatus::GetMarkPos()
{
	return m_nMarkPos;
}


void CStatus::SetMarkPos(int nPos)
{
	m_nMarkPos = nPos;
}


void CStatus::IncMarkPos()
{
	m_nMarkPos++;
}



void CStatus::SetNaviStatus(bool bStatus)
{
	m_bOnNavi = bStatus;
}


bool CStatus::IsOnNavi()
{
	return m_bOnNavi;
}


int CStatus::GetAutoMarkPos()
{
	return m_nAutoMarkPos;
}


void CStatus::IncAutoMarkPos()
{
	m_nAutoMarkPos++;
}


void CStatus::SetAutoMarkPos(int nPos)
{
	m_nAutoMarkPos = nPos;
}


void CStatus::SetCompareMode(bool bCompare)
{
	m_bCompareMode = bCompare;

	if(bCompare)
	{
			
	}
	else
	{

	}
	return;
}


void CStatus::SetOverlayMode(bool bOverlayMode)
{
	m_bOverlayMode = bOverlayMode;
}


bool CStatus::IsOverlayMode()
{
	return m_bOverlayMode;
}


bool CStatus::IsCompareMode()
{
	return m_bCompareMode;
}
