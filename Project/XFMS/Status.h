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
	enum tInspType {INSPFID, INSPHEDET, INSPALIGN, INSPHEIGHT, INSPGAPGAGE};
	enum tDispMode { DISP_AUTO, DISP_MANUAL};
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
};

