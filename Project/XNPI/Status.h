#pragma once
#include "..\Library\SerialLib\SerialMFC.h"	// 필수
#include "..\Library\SerialLib\XFMSPacket.h"	// 필수
#include <UniUtility.h>
class CMilVLib;
class CModelData;
class CSCAlgoBase;
class CRepository;
struct tInfoCamIng
{
	int nCamNo;
	CdblPoint posImage; //Mouse point가 가르키는 실제 이미지상의(Zoom적용되지않은) 좌표
	int GrayValue;      //Mouse point위치의 Gray값
};

class CStatus
{
public:
	enum tOpMode { OPAUTO, OPTEACHING, OPSTOP };
	enum tInspType { INSPFID, INSPHEDET, INSPALIGN, INSPHEIGHT, INSPGAPGAGE };
	enum tDispMode { DISP_AUTO, DISP_MANUAL };
	CStatus();
	~CStatus();
	static CStatus * Instance();
	static CStatus *m_pInstance;
	void SetOPMode(tOpMode opMode);
	int GetOpMode(void);
	void SetSelCam(int nCam);
	int m_iSelCam;
	int GetSelCamNo();
	CString GetModelRepository();
	void SetModelRepository(TCHAR * szDataSaveFolder);
	CString m_strModelRepository;
	CString m_strModelName;
	CString GetModelName();
	void SetModelName(TCHAR * szModelName);
	int SaveCFG();
	int LoadCFG();
	long m_nCountOfCam;
	CString GetComportNo();
	void SetBaudRate(int iBaudRate);
	int GetBaudRate();
	void SetComportNo(TCHAR* szComportNo);
	long m_iBaudRate;
	CString m_strComportNo;
	long m_LightBrightness[4];
	CXFMSPacket * GetComm();
	CXFMSPacket *m_pSCInspPacket;
	void SetComm(CXFMSPacket * pSCInspPacket);
	int GetRVOption();
	CModelData * GetModelData();
	CModelData *m_pModelData;
	void SetModelData(CModelData * pModelData);
	void SetCurAddAlgoIndex(int nInspType);
	int m_nInspTypeToAdd;
	int GetCurAlgoToAddIndex();
	CSCAlgoBase * GetCurAlgoToAdd();
	CSCAlgoBase * GetAlgoInfo(int nInspType, CXmlNode* pNode = NULL);
	void SetAlgoInfo(int nInspType, CSCAlgoBase *pAlgo, CXmlNode* pNode);
	BOOL m_bIsOffLine;
	CString m_ProductName;
	tInfoCamIng m_infoCamMousePos;
	int m_opMode;
	CString m_strIMGSaveFolder;
	BOOL m_MsgWndOpen;
	int GetCamNo();
	long m_nCountOfSensors;
	BOOL m_bSaveTestImage;
	BOOL m_bDrawCross;
	long m_bUseDelOldImg;
	long m_nDaysDel;
	CdblSize GetPixelCalData(int iCamIndex);
	long m_lIPAddr[4];
	CString GetTempSensorIPString();
	CString m_strRemoteImgName;
	long m_lTemperature[6];
	long m_ulGraphXRange;
	long m_ulGraphYMax;
	long m_ulGraphYMin;
	long m_ulDispMode;
	CString m_strAVISaveFolder;
	int GetAutoMarkPos();
	void SetAutoMarkPos(int nPos);
	void IncAutoMarkPos();
	int m_nAutoMarkPos;
	CString m_strMarkToAdd;
	void SetMarkString(CString strToAdd);
	CString GetMarkString();
	CRepository *m_pRepository;
	CRepository * GetRepository();
	void SetRepository(CRepository * pRepository);
	long m_bUseImgEnhance;
	BOOL m_bIsAVIRecording;
	CString m_strSensorNamePx[6];
	long m_wlvLow;
	long m_wlvHigh;

	//kiy plc 관련설정
	CString m_strPlcIp;
	CString m_strPlcPort;
	long m_ulPlcReadCycleDelay_ms;

	//kiy 190327 - XNPIView - CMD_INVAILD_INTERVAL_MS 대신 쓸 변수, PLC로부터 읽어올때의 유지시간,
	//이 수치가 작으면 PLC에서 커맨드를 1초간 유지할때 해당 커맨드를 한번만 인식하지 않고 여러번으로 인식할수 있다.
	//현재 0번 이미지만 다른 폴더에 저장된다는걸 봐선, Start 커맨드를 plc에서 1초간 유지할때, 2번으로 인식하는것으로 추정된다.
	long m_ulPlcReadCMDHoldingTime;

	//kiy wndLev 관련설정
	long m_wlnLowPresetArr[5];
	long m_wlnHighPresetArr[5];

	//kiy Image save 포맷 지정 설정
	long m_bImgSaveFormatJPG;
	long m_bImgSaveFormatTIF;

	//kiy Info 관련설정
	CString m_strComp;
	CString m_strSEPC;
	CString m_strSize;
	CString m_strSerial;
	CString m_strPipeNo;
	CString m_strInfoLine1;		//181019 line1,2 요청으로 추가
	CString m_strInfoLine2;	    


	long m_avg;

	//kiy Avg Min Limit - avg 최소값 강제 0906
	long m_avgMinLimit;
	//kiy - 마지막으로 생성된 폴더경로(중간에 꺼졌을때 기존 검사 이어서 하기위해서) 0911
	CString m_strLastOpendFolder;

	void SaveOnlyPipeInfoCFG();
	void SaveLastOpendFolder();

	//kiy CamDlg에서 Region으로 밝기값 가져오기 위한 변수 0919
	long m_gvAvgRegionWidth;
	long m_gvAvgRegionHeight;

	//kiy - 0919
	long m_bSaveWithRaw;	//검사 이미지 저장시에 Raw파일도 같이 저장 할 것인가
	long m_bShowGrayAvg;	//영상위에 마우스 올렸을때 리젼잡고 그 리젼의 밝기 평균값 표시

	//kiy - 1016, 픽셀 사이즈 추가 Tiff태그 XResolution, YResolution에 각각 대응
	double m_dPixelXSize_mm;	//XPixelSize
	double m_dPixelYSize_mm;	//YPixelSize

	//kiy - 1016
	double m_pipeStartPos_mm;			//파이프 시작 위치
	double m_pipeSectEachDistance_mm;	//파이프 각 섹션 간의 거리
};

