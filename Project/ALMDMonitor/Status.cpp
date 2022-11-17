#include "stdafx.h"
#include "Status.h"
#include "Repository.h"
#include "ALMDMessages.h"
#include "ALMDStatusDlg.h"
#include "OpPanelDlg.h"
#include "ViewDlg.h"
#include "Stabilization.h"

CStatus::tProgramMode CStatus::m_ProgramMode= PM_REVIEW;
CStatus::tKeylockStatus CStatus::m_KeylockStatus= LK_NO;
CStatus::tCoordinateSystem CStatus::m_iCoordinateSystem = CS_CARTESIAN;
CStatus * CStatus::m_pInstance = NULL;
COpPanelDlg * CStatus::m_pOpPanelDlg = NULL;
CALMDStatusDlg * CStatus::m_pALMDStatusDlg = NULL;
CViewDlg * CStatus::m_pViewDlg = NULL;
CStatus::CStatus()
	: m_iBaudRate(9600)
	, m_strComportNo(_T(""))
	, m_strDataSaveFolder(_T(""))
	, m_strDataFileName(_T(""))
	, m_nMarkPos(0)
	, m_iUseWarning(0)
	, m_iUseTemp(0)
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
	, m_iAngleType(CStatus::AT_DEGREE)
	, m_iRORType(CStatus::RT_MMD)
	, m_iInclinationType(CStatus::IT_ANGLE)
	, m_iWaferSize(300)
	, m_dblTolerance(2.5)
	, m_pRepository(NULL)
	, m_dptRelPlan(0)
	, m_bIsRel(FALSE)
	, m_iStabilizationValue(20)
	, m_dblStblzSensitivity(0.03)
{
	m_ViewOptions.iParticleType = 1;
	m_ViewOptions.iXGridUnit = 1;
	m_ViewOptions.iYGridUnit = 1;
	memset(&m_curMeasureData, 0,  sizeof(tALMDRecord));
	ClearMarkToAdd();
}


CStatus::~CStatus()
{
}


void CStatus::SetProgramMode(tProgramMode ProgramMode)
{
	m_ProgramMode = ProgramMode;
	if (!m_pALMDStatusDlg)return;
	m_pALMDStatusDlg->SendMessage(WM_PM_CHANGE, NULL, NULL);
	if (!m_pOpPanelDlg)return;
	m_pOpPanelDlg->SendMessage(WM_PM_CHANGE, NULL, NULL);
}

CStatus::tProgramMode CStatus::GetProgramMode()
{
	return m_ProgramMode;
}


BOOL CStatus::SearchKeylock()
{
	////////////////////// USB용 메가락 검사루틴 시작
	//unsigned int usbID;
	//CString szTemp, szMem;
	//usbID = lock_init_usb(8);	// USB용 메가락 찾기, 인수 8은 cpu id의 첫번째 숫자 8~F 까지 사용가능
	//if (usbID) {
	//	//MegaLock is Founded
	//	if (0x10 != lock_read(0, 0xff))
	//		return FALSE;
	//	return TRUE;
	//}
	//else {
	//	//MegaLock is not Founded
	//	return FALSE;
	//}
	return TRUE;
}


void CStatus::SetKeylockStatus(tKeylockStatus KeylockStatus)
{
	m_KeylockStatus = KeylockStatus;
}


CStatus::tKeylockStatus CStatus::GetKeylockStatus()
{
	return m_KeylockStatus;
}


void CStatus::SetCoordinateSystem(tCoordinateSystem CoordinateSystem)
{
	m_iCoordinateSystem = CoordinateSystem;
}


CStatus::tCoordinateSystem CStatus::GetCoordinateSystem()
{
	return m_iCoordinateSystem;
}


int CStatus::GetDefault()
{
	CCfgFile *pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pMyCfg->Open();
	m_iBaudRate		= pMyCfg->Get(9600L, _T("COMSET"), _T("BaudRate"));
	m_strComportNo	= pMyCfg->Get(_T("None"), _T("COMSET"), _T("ComportNo"));
	m_strDataSaveFolder = pMyCfg->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	m_strDataFileName	= pMyCfg->Get(_T("ALMDLog"), _T("Environment"), _T("DataFileName"));

	m_iUseWarning = pMyCfg->Get(0L, _T("WarningCondition"), _T("UseWarningCondition"));
	m_iUseTemp =	pMyCfg->Get(0L, _T("WarningCondition"), _T("UseTemp"));
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


int CStatus::GetInclinationType()
{
	return m_iInclinationType;
}


CStatus::tAngleType CStatus::GetAngleType()
{
	return m_iAngleType;
}


CStatus::tRORType CStatus::GetRORType()
{
	return m_iRORType;
}


void CStatus::SetAngleType(tAngleType iAngleType)
{
	m_iAngleType = iAngleType;
}

//! Kind of Rise of Run
void CStatus::SetRORType(tRORType iRORType)
{
	m_iRORType = iRORType;
}


void CStatus::SetInclinationType(CStatus::tIncType iIncType)
{
	m_iInclinationType = iIncType;
}


void CStatus::SetWaferSize(int sizeWafer)
{
	m_iWaferSize = sizeWafer;
}


int CStatus::GetWaferSize()
{
	return m_iWaferSize;
}


void CStatus::SetAdjuestAngle(tAdjustAngle AdjustAngle)
{
	m_AdjustAngle = AdjustAngle;
}


tAdjustAngle CStatus::GetAdjustAngle()
{
	return m_AdjustAngle;
}


void CStatus::SetMeasureData(tALMDRecord *pCurData)
{
	m_curMeasureData = *pCurData;
	m_pALMDStatusDlg->SendMessage(WM_UPDATE_STATINFO);
	m_pViewDlg->SendMessage(WM_UPDATE_STATINFO);
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		m_pOpPanelDlg->SendMessage(WM_UPDATE_STATINFO);
	}
}


tALMDRecord * CStatus::GetCurMeasureData()
{
	return &m_curMeasureData;
}


void CStatus::SetTolerance(double dblTolerance)
{
	m_dblTolerance = dblTolerance;
}


double CStatus::GetTolerance()
{
	return m_dblTolerance;
}


tMarkInfo CStatus::GetMarkToAdd()
{
	tMarkInfo tempMark;
	tempMark = m_MarkToAdd;
	return tempMark;
}


void CStatus::SetMarkToAdd(tMarkInfo * pMarkToAdd)
{
	m_MarkToAdd = *pMarkToAdd;
}


void CStatus::ClearMarkToAdd()
{
//	m_MarkToAdd.szComment
	memset(&m_MarkToAdd, 0, sizeof(tMarkInfo));
	wsprintf(m_MarkToAdd.szOperator, TEXT(""));
	wsprintf(m_MarkToAdd.szStation, TEXT(""));
}


void CStatus::UpdateScrn()
{
	m_pALMDStatusDlg->SendMessage(WM_UPDATE_STATINFO);
	m_pViewDlg->SendMessage(WM_UPDATE_STATINFO);
	m_pOpPanelDlg->SendMessage(WM_UPDATE_STATINFO);
}



BOOL CStatus::IsRelative()
{
	return m_bIsRel;
}


void CStatus::SetRelMode(bool bRelMode)
{
	m_bIsRel = bRelMode;
}


double CStatus::GetStblzSensitivity()
{
	return m_dblStblzSensitivity;
}


void CStatus::SetStblzSensitivity(double dblSensitivity)
{
	m_dblStblzSensitivity = dblSensitivity;
	CStabilization::Instance()->SetSensitivity((int)(dblSensitivity * 1000));
}


CString CStatus::GetLastOpenFileName()
{
	return strLastOpenFileName;
}


BOOL CStatus::IsDigit2Mode()
{
	return FALSE;
}
