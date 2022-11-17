#include "StdAfx.h"
#include "MachineInfo.h"
#include <ItfModelBase.h>

double CMachineInfo::m_Limit_ConvayerStage_X_Pos=99999999;
double CMachineInfo::m_Limit_ConvayerStage_Y_Pos=99999999;
double CMachineInfo::m_Limit_ConvayerStage_Z_Pos=99999999;
double CMachineInfo::m_Limit_ConvayerStage_X_Neg=-99999999;
double CMachineInfo::m_Limit_ConvayerStage_Y_Neg=-99999999;
double CMachineInfo::m_Limit_ConvayerStage_Z_Neg=-99999999;

double CMachineInfo::m_DETECTORSKEWANGLE=0;
double CMachineInfo::m_FrameMoveStageXSpeed=0;
double CMachineInfo::m_FrameMoveStageYSpeed=0;
double CMachineInfo::m_FrameMoveZSpeed=0;
double CMachineInfo::m_FrameMoveTSpeed=200000;
double CMachineInfo::m_FrameMoveORSpeed=0;
double CMachineInfo::m_FrameMoveCBTCONVSpeed=0;
double CMachineInfo::m_ConBeam5AxisEnable;
BOOL CMachineInfo::m_SetVerticalPriority=false;
BOOL CMachineInfo::m_bEnableInlineSoftavr=false;
double CMachineInfo::LoadPosX_Stage=0;
double CMachineInfo::LoadPosY_Stage=0;
double CMachineInfo::LoadPosX_Base=0;
double CMachineInfo::LoadPosY_Base=0;
double CMachineInfo::LoadPosZ=0;
double CMachineInfo::LoadPosT=0;
double CMachineInfo::LoadPosR=0;

double CMachineInfo::unLoadPosX_Stage=0;
double CMachineInfo::unLoadPosY_Stage=0;
double CMachineInfo::unLoadPosX_Base=0;
double CMachineInfo:: unLoadPosY_Base=0;
double CMachineInfo::unLoadPosZ=0;
double CMachineInfo::unLoadPosT=0;
double CMachineInfo::unLoadPosR=0;

double CMachineInfo::m_bDDMotorEnable	=0;
double CMachineInfo::m_bControllerXrayType=0;
double CMachineInfo::E_GearSX=1;

int CMachineInfo::m_bTiltAxisEnable=0;
CMachineInfo* CMachineInfo::m_pMachineInfo=NULL;
double CMachineInfo::m_NGBufferCounter=5;
double CMachineInfo::m_LOADERCounter=5;
CSize CMachineInfo::m_CameraPicxel;
int CMachineInfo::m_XrayPort = 1;
int	CMachineInfo::m_TriggerPort = 2;
int	CMachineInfo::m_BarCodePort =5;
BOOL CMachineInfo::m_bAutoReverse = FALSE;
BOOL CMachineInfo::m_bSaveResult = FALSE;
BOOL CMachineInfo::m_bStopOnDefect = FALSE;
BOOL CMachineInfo::m_bSilent = FALSE;
BOOL CMachineInfo::m_bUseLaser = FALSE;
BOOL CMachineInfo::m_bDrawCross = FALSE;
BOOL CMachineInfo::m_bImgEnhance = FALSE;
tBNState CMachineInfo::m_BrightNormalizeState = {FALSE,TRUE,TRUE,900};
BOOL CMachineInfo::m_bWarpCal = FALSE;
BOOL CMachineInfo::m_bScrnNormalize = FALSE;
BOOL CMachineInfo::m_EnableHFillter = FALSE;

BOOL CMachineInfo::m_bRuler = FALSE;
BOOL CMachineInfo::m_ImageMargin_V = 0;
BOOL CMachineInfo::m_ImageMargin_H = 0;
BOOL CMachineInfo::m_bBarcode=false;
BOOL CMachineInfo::m_bNGImageSave = false;
BOOL CMachineInfo::m_bGoodImageSave = false;
BOOL CMachineInfo::m_bCompoImageSave=false;
BOOL CMachineInfo::m_bInfomationViewer = false;
CString CMachineInfo::m_PIPCAM_NAME = _T("");
int CMachineInfo::m_PIPCAMNUM=0;
long CMachineInfo::m_iExpostime=1035;
int CMachineInfo::m_iAvg = 0;
int CMachineInfo::m_iAvgSoft =0;
int CMachineInfo::m_Filter = 0;
int CMachineInfo::m_moduleNum = 0;
int CMachineInfo::m_frameNum = 0;
double CMachineInfo::m_RotateCenterforviewORG_BaseX=0;
double CMachineInfo::m_RotateCenterForviewORG_BaseY=0;
double CMachineInfo::m_ZeroAngleForTiltORG=0;

CString CMachineInfo::m_stMotion_type=_T("");
CString CMachineInfo::m_stOIborad_type=_T("");
CString CMachineInfo::m_stXraytube_type=_T("");

CString CMachineInfo::m_ModelName = _T("");
CString CMachineInfo::m_BarCodeNUM = _T("");
CString CMachineInfo::m_LengDir=_T("");
CString CMachineInfo::m_ProductName=_T("");
double CMachineInfo:: m_MahcineFID=0;
double CMachineInfo:: m_Tanx=0;

BOOL CMachineInfo::m_bRVShowDim=0;

double CMachineInfo::m_RotateZMin0DgressFor2D=0;
double CMachineInfo::m_RoRateZMaxODgreesFor2D=0;
double CMachineInfo::m_RateCalValeFor2dZeroDgres=0;

double CMachineInfo::m_Angle_Per_One_Pulse=0.0002017699115;          //한펄스당 틸트각도 
double CMachineInfo::m_mm_Per_pulseZ=0;                 //한펄스당 z 축의 이동거리 mm
double CMachineInfo::m_Limited_ZPos_Max=0;              //z 축의 리미트 (h100)용
double CMachineInfo::m_Limited_ZPos_Min=0;               // Z  축의 리미트 

double CMachineInfo::m_iBreaktypeZ=0;  //  z 축의 브레이크 수동타입 
double CMachineInfo::m_RotateCenterforview_BaseX=0;     // OCT 중공축의중심이 화면의 중심이되는 베이스좌표
double CMachineInfo::m_RotateCenterForview_BaseY=0;
double CMachineInfo::m_ZeroAngleForTilt=0;    

double CMachineInfo::m_AxisRTotalPulse=0; // R 축의1회전펄스 
double CMachineInfo::m_RlimitPosMin=0 ;    // r축의 -리밋
double CMachineInfo::m_RlimitPosMax=0;    // r축의 +리밋
double CMachineInfo::m_AxisR_ZeroPos=0;   //r 축의 0위치 값
double CMachineInfo::m_Offset_3d_MeasurePoint=0;//3d 계측오차 보상
double CMachineInfo::m_DetectorPixelSize=0;

// 스테이지 X-Y 부분 리밋 범위 설정 값
double CMachineInfo::m_Limit_Stage_X_Pos=0;
double CMachineInfo::m_Limit_Stage_X_Neg=0;
double CMachineInfo::m_Limit_Stage_Y_Pos=0;
double CMachineInfo::m_Limit_Stage_Y_Neg=0;
double CMachineInfo::m_lfLimit_T_Neg = 0;
double CMachineInfo::m_lfLimit_T_Pos = 0;


// Cd data 저장 경로 및 방법 설정 관련 변수들 

CString CMachineInfo::m_stCTdataDir=(_T(""));
CString CMachineInfo::m_st3dPC_CTDir=(_T(""));
CString CMachineInfo::m_CTDATA_Save_Mode=(_T(""));
CString CMachineInfo::m_RamDrive_CTDir=(_T(""));

//  시퀀스 로봇 속도 조절 용 레이트 

double CMachineInfo::m_SpeedRate =1; 
double CMachineInfo::m_SpeedRate2=1; 
double CMachineInfo::m_SpeedRate3=1; 
    

double CMachineInfo::m_AccDccRate=1;  
double CMachineInfo::m_AccDccRate2=1; 
double CMachineInfo::m_AccDccRate3=1;

int CMachineInfo::m_OverKillRecheck=1;
int CMachineInfo::m_AlgoTimeover=3000;


	////////////////CBT 모드 관련///////////
     double CMachineInfo::m_CBTLimitPosStageX=0;
	 double CMachineInfo::m_CBTLimitPosStageY=0;
	 double CMachineInfo::m_CBTLimitPosStageZ=0;
	 double CMachineInfo::m_CBTLimitPosStageR=0;

	 double CMachineInfo::m_CBTLimitnNegStageX=0;
	 double CMachineInfo::m_CBTLimitnNegStageY=0;
	 double CMachineInfo::m_CBTLimitnNegStageZ=0;
	 double CMachineInfo::m_CBTLimitnNegStageR=0;

	 double CMachineInfo::m_CBTStartDefualtStageXpos=0;
	 double CMachineInfo::m_CBTStartDefualtStageYpos=0;
	 double CMachineInfo::m_CBTStartDefualtStageRpos=0;

     double CMachineInfo::m_CBTStartStartStageXpos=0;
	 double CMachineInfo::m_CBTStartStartStageYpos=0;
	 double CMachineInfo::m_CBTStartStartStageRpos=0;
	 double CMachineInfo::m_CBTAxisRMotionPulse = 0;


////////////////////


	 ///////////////////H160 관련 Z 축 최저 높히 관련 
	 double CMachineInfo::m_30DgressZNegPosLimitForTilt=0;    //30도 옵셋
	 double CMachineInfo::m_35DgressZNegPosLimitForTilt=0;    //35도 옵셋
	 double CMachineInfo::m_40DgressZNegPosLimitForTilt=0;    //40도 옵셋
	 double CMachineInfo::m_45DgressZNegPosLimitForTilt=0;    //45도 옵셋
	 double CMachineInfo::m_50DgressZNegPosLimitForTilt=0;    //50도 옵셋
	 double CMachineInfo::m_55DgressZNegPosLimitForTilt=0;    //55도 옵셋
	 double CMachineInfo::m_60DgressZNegPosLimitForTilt=0;    //60도 옵셋



	 ////////////////////

CMachineInfo::CMachineInfo(void)
: m_opMode(0)
, m_bIsOnline(FALSE)
{
}

CMachineInfo::~CMachineInfo(void)
{
}

CMachineInfo* CMachineInfo::Instance(void)
{
	if(!m_pMachineInfo)
	{
		m_pMachineInfo=new CMachineInfo;
		m_pMachineInfo->LoadCFG();
	}
	return m_pMachineInfo;
}

void CMachineInfo::SetOPMode(tOpMode opMode)
{
	m_opMode=opMode;
}

int CMachineInfo::GetOpMode(void)
{
	return m_opMode;
}

BOOL CMachineInfo::IsOnline(void)
{
	return m_bIsOnline;
}

void CMachineInfo::SetOnline(bool currStatus)
{
	m_bIsOnline=currStatus;
}

BOOL CMachineInfo::GetAutoReverse()
{
	return m_bAutoReverse;
}

BOOL CMachineInfo::GetSaveResult()
{
	return m_bSaveResult;
}

BOOL CMachineInfo::GetStopDefect()
{
	return m_bStopOnDefect;
}

BOOL CMachineInfo::GetSilentMode()
{
	return m_bSilent;
}

BOOL CMachineInfo::GetUseLaser()
{
	return m_bUseLaser;
}

BOOL CMachineInfo::GetDrawCross()
{
	return m_bDrawCross;
}

BOOL CMachineInfo::GetImageEnhance()
{
	return m_bImgEnhance;
}

tBNState CMachineInfo::GetBrightNormalize()
{
	return m_BrightNormalizeState;
}

BOOL CMachineInfo::GetRuler()
{
	return m_bRuler;
}

int CMachineInfo::GetAverage()
{
	return m_iAvg;
}

int CMachineInfo::GetAverageSoft()
{
	return m_iAvgSoft;
}


int CMachineInfo::GetExposTime()
{
	return m_iExpostime;
}

int CMachineInfo::GetFilter()
{
	return m_Filter;
}

int CMachineInfo::GetScrnNormalize()
{
	return m_bScrnNormalize;
}

int CMachineInfo::GetWarpCal()
{
	return m_bWarpCal;
}

#define CFG_FILE _T("CFG.INI")

void CMachineInfo::SaveCFG(void)
{
	pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir()+_T("\\..\\Setup\\")+CFG_FILE);
	pMyCfg->SetDepth(2);
	pMyCfg->Open();

	pMyCfg->Set(m_BrightNormalizeState.uiBNLimit, _T("Setup"), _T("BNLimit"));


	// 포트 설정
		



	pMyCfg->Flush();
}

void CMachineInfo::CamerainfoLoad()
{


}

void CMachineInfo::LoadCFG(void)
{
	pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir()+_T("\\..\\Setup\\")+CFG_FILE);
    
	
	 pMyCfg->SetDepth(2);
	 pMyCfg->Open();

    
	CMachineInfo::m_SpeedRate = pMyCfg->Get(1L, _T("ConvSetting"), _T("m_SpeedRate"), _T("m_SpeedRate"));
	CMachineInfo::m_SpeedRate2 = pMyCfg->Get(1L, _T("ConvSetting"), _T("m_SpeedRate2"), _T("m_SpeedRate2"));
	CMachineInfo::m_SpeedRate3 = pMyCfg->Get(1L, _T("ConvSetting"), _T("m_SpeedRate3"), _T("m_SpeedRate3"));
    

	CMachineInfo::m_AccDccRate = pMyCfg->Get(1L, _T("ConvSetting"), _T("m_AccDccRate"), _T("m_AccDccRate"));
	CMachineInfo::m_AccDccRate2 = pMyCfg->Get(1L, _T("ConvSetting"), _T("m_AccDccRate2"), _T("m_AccDccRate2"));
	CMachineInfo::m_AccDccRate3 = pMyCfg->Get(1L, _T("ConvSetting"), _T("m_AccDccRate3"), _T("m_AccDccRate3"));
	CMachineInfo::m_OverKillRecheck = pMyCfg->Get(1L, _T("ConvSetting"), _T("m_OverKillRecheck"), _T("m_OverKillRecheck"));
	CMachineInfo::m_AlgoTimeover = pMyCfg->Get(3000L, _T("ConvSetting"), _T("m_AlgoTimeover"), _T("m_AlgoTimeover"));
	CMachineInfo::m_bControllerXrayType = pMyCfg->Get(0L, _T("m_bControllerXrayType"), _T("m_bControllerXrayType"));  //    엑스레이컨트롤타입
	CMachineInfo::m_bDDMotorEnable = pMyCfg->Get(0L, _T("m_bDDMotorEnable"), _T("m_bDDMotorEnable"));  //   OCT모터타입

}

CString CMachineInfo::GetModelName()
{
	return m_ModelName;
}

void CMachineInfo::SetModelName(TCHAR * nameModel)
{
	m_ModelName=nameModel;
}

CString CMachineInfo::GetBarcodeNum(void)
{
	return m_BarCodeNUM;
}

unsigned long CMachineInfo::GetRVOption(void)
{
	unsigned long rv=0;
	if(m_bRVShowDim)
		rv|=RV_SHOW_DIM;
	return rv;
}
