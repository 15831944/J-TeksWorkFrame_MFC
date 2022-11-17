#pragma once

#include <UniUtility.h>

/** 
\brief 
하드웨어와 같은 정보조회(일반적으로 Config 및 Setting을 통해) 및 설정에 활용한다.@n
\date 2014.06.27
\author 전 병근
*/
class CMachineInfo
{
public:
	enum tOpMode{OPAUTO, OPTEACHING, OPSTOP};
	CMachineInfo(void);
	~CMachineInfo(void);
	static CMachineInfo* Instance(void);
	static CMachineInfo* m_pMachineInfo;
	void SetOPMode(tOpMode opMode);

	CCfgFile *pMyCfg;

	
	int GetOpMode(void);
	int m_opMode;
	int IsOnline(void);
	void SetOnline(bool currStatus);
	int m_bIsOnline;
    
	static  int m_XrayPort ;
	static	int m_TriggerPort ;
	static	int m_BarCodePort ;
	static BOOL m_bAutoReverse;
	static BOOL m_bSaveResult;
	static BOOL m_bStopOnDefect;
	static BOOL m_bSilent;
	static BOOL m_bUseLaser;
	static BOOL m_bDrawCross;
	static BOOL m_bImgEnhance;   //ImageEnhance기능
	static BOOL m_bRuler;
	static int m_iAvg;
	static int m_iAvgSoft;
	static int m_Filter;
	static tBNState m_BrightNormalizeState;
	static BOOL m_bWarpCal;
	static BOOL m_bScrnNormalize;
	static BOOL m_EnableHFillter;
	static BOOL m_SetVerticalPriority;
	static int m_moduleNum;
	static int m_frameNum;
	static long m_iExpostime;
	static BOOL m_bBarcode;
	static BOOL m_bNGImageSave ;
	static BOOL m_bGoodImageSave;
	static BOOL m_bCompoImageSave;
	static BOOL m_bInfomationViewer;
	static CString m_PIPCAM_NAME; // pip cam 이름
	static int m_PIPCAMNUM;
	static int m_ImageMargin_V;   // 이미지 마진 용 
	static int m_ImageMargin_H;   // 이미지 마진 용 

	static CString m_ModelName;   //모델폴더명 저장
	static CString m_BarCodeNUM;  //PCB 바코드 넘버 저장
	
	static CString m_stXraytube_type;      // 현재 사용하고 있는 엑스레이 튜브종류 저장 
	static CString m_stOIborad_type;      // 현재 사용하고 있는 IO 보드
	static CString m_stMotion_type;      // 현재 사용하고 있는  모션보드 
    static CString m_LengDir ;           // 현재 사용하고 있는 랭귀지파일경로 
	static CString m_ProductName;        // 현재 설치된 장비명
	static double m_MahcineFID;         // 현재 장비의 FID 값
    
	static double m_Angle_Per_One_Pulse ;          //한펄스당 틸트각도 
	static double m_mm_Per_pulseZ;                 //한펄스당 z 축의 이동거리 mm
	static double m_Limited_ZPos_Max;              //z 축의 리미트 (h100)용
	static double m_Limited_ZPos_Min;
	static double m_RotateZMin0DgressFor2D;
	static double m_RoRateZMaxODgreesFor2D;
	static double m_RateCalValeFor2dZeroDgres;
	
     
    static double m_bControllerXrayType; ///> 엑스레이 컨트롤러 타입
	static double E_GearSX; ///> 전자 기어비 X ; 


	static int m_bTiltAxisEnable; // 틸트 축 사용 불가능 입력
	static double m_iBreaktypeZ; // 수동 브레이크 타입의 z 축 
    static double m_RotateCenterforview_BaseX;     // OCT 중공축의중심이 화면의 중심이되는 베이스좌표
	static double m_RotateCenterForview_BaseY;
	static double m_ZeroAngleForTilt;              // T 축상의 영도셋팅홈값
    static double m_Tanx;
	static double m_RotateCenterforviewORG_BaseX;
    static double m_RotateCenterForviewORG_BaseY;
    static double m_ZeroAngleForTiltORG;
	static double m_AxisRTotalPulse ;// R 축의1회전펄스 
	static double m_RlimitPosMin;// R축의 -리밋
	static double m_RlimitPosMax;// r축의 +리밋
	static double m_AxisR_ZeroPos; //r축의 0도 위치 
	static CSize m_CameraPicxel ; // 카메라의 픽셀 갯수 Ex) 바슬러는 1000x1000 
	static double m_Offset_3d_MeasurePoint;  
	static double m_DetectorPixelSize;
	static double m_bDDMotorEnable	;
	/////////// 스테이지 x-y 리밋 설정 범위 값/////
	static double m_Limit_Stage_X_Pos;
	static double m_Limit_Stage_X_Neg;
	static double m_Limit_Stage_Y_Pos;
	static double m_Limit_Stage_Y_Neg;
	///////////////////////////////////////////////////
	static double m_lfLimit_T_Neg;
	static double m_lfLimit_T_Pos;

	///CT 이미지 저장 방식 
	static CString m_stCTdataDir;
	static CString m_st3dPC_CTDir;
	static CString m_CTDATA_Save_Mode;
	static CString m_RamDrive_CTDir;
	//////////////// 
	static double m_NGBufferCounter;
	static double m_LOADERCounter;
	///// 로드 겟수 및 버퍼에 차있는 NG 카운터하는 한계를 설정해주는 변수 추가 안지훈 12 06 21 
     /// 스피드 계산용 

	static double m_SpeedRate ;
	static double m_SpeedRate2; 
	static double m_SpeedRate3 ;
    

	static double m_AccDccRate  ;
	static double m_AccDccRate2 ;
	static double m_AccDccRate3;
	static int m_OverKillRecheck;
	static int m_AlgoTimeover;
	static BOOL m_bRVShowDim;
 ///////////////////////////////////////

	/////////////// 로드 언로드 위치 결정변수 
	static double LoadPosX_Stage;
	static double LoadPosY_Stage;
	static double LoadPosX_Base;
	static double LoadPosY_Base;
	static double LoadPosZ;
	static double LoadPosT;
	static double LoadPosR;

    static double unLoadPosX_Stage;
	static double unLoadPosY_Stage;
	static double unLoadPosX_Base;
	static double unLoadPosY_Base;
	static double unLoadPosZ;
	static double unLoadPosT;
	static double unLoadPosR;
	/////////////////////////////////

	/////////////// Setup 창 관련
    static BOOL m_bEnableInlineSoftavr;
	BOOL GetAutoReverse();
	BOOL GetSaveResult();
	BOOL GetStopDefect();
	BOOL GetSilentMode();
	BOOL GetUseLaser();
	BOOL GetDrawCross();
	BOOL GetImageEnhance();
	tBNState GetBrightNormalize();
	BOOL GetRuler();
	CString GetModelName();
	void SetModelName(TCHAR * nameModel);
	int GetAverage();
	int GetAverageSoft();
	int GetExposTime();
	int GetFilter();
	int GetScrnNormalize();
	int GetWarpCal();
	void SaveCFG(void);
	void LoadCFG(void);
	void CamerainfoLoad();
	CString GetBarcodeNum(void);
	////////////////////////////////////

	////////////////CBT 모드 관련///////////
    static double m_CBTLimitPosStageX;
	static double m_CBTLimitPosStageY;
	static double m_CBTLimitPosStageZ;
	static double m_CBTLimitPosStageR;

	static double m_CBTLimitnNegStageX;
	static double m_CBTLimitnNegStageY;
	static double m_CBTLimitnNegStageZ;
	static double m_CBTLimitnNegStageR;

	static double m_CBTStartDefualtStageXpos;
	static double m_CBTStartDefualtStageYpos;
	static double m_CBTStartDefualtStageRpos;

    static double m_CBTStartStartStageXpos;
	static double m_CBTStartStartStageYpos;
	static double m_CBTStartStartStageRpos;

	static double m_CBTAxisRMotionPulse;
	static double m_ConBeam5AxisEnable;
	///////////////////////////////////////


	///////////////h160 틸트 z 축 최저 값 리밋 용 ///////////
	 static double m_30DgressZNegPosLimitForTilt;    //30도 옵셋
	 static double m_35DgressZNegPosLimitForTilt;    //35도 옵셋
	 static double m_40DgressZNegPosLimitForTilt;    //40도 옵셋
	 static double m_45DgressZNegPosLimitForTilt;    //45도 옵셋
	 static double m_50DgressZNegPosLimitForTilt;    //50도 옵셋
	 static double m_55DgressZNegPosLimitForTilt;    //55도 옵셋
	 static double m_60DgressZNegPosLimitForTilt;    //60도 옵셋

	 unsigned long GetRVOption(void);
	static double m_FrameMoveStageXSpeed;
	static double m_FrameMoveStageYSpeed;
	static double m_FrameMoveZSpeed;
	static double m_FrameMoveTSpeed;
	static double m_FrameMoveORSpeed;
	static double m_FrameMoveCBTCONVSpeed;
	static double m_DETECTORSKEWANGLE;

	//////////////////////////9800 컨베어타입 테이블 의 리밋/////
	static double m_Limit_ConvayerStage_X_Pos;
	static double m_Limit_ConvayerStage_Y_Pos;
	static double m_Limit_ConvayerStage_X_Neg;
    static double m_Limit_ConvayerStage_Y_Neg;
	static double m_Limit_ConvayerStage_Z_Pos;
	static double m_Limit_ConvayerStage_Z_Neg;

	////////////////
	
	///////////////////////////////////////////
};
