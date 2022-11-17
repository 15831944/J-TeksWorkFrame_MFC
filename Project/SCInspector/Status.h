#pragma once
#include "..\Library\SerialLib\SerialMFC.h"	// 필수
#include "..\Library\SerialLib\SCInspPacket.h"	// 필수
#include <UniUtility.h>
class CMilVLib;
class CModelData;
class CSCAlgoBase;
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
	long m_camInspStage[4];
	CString GetComportNo();
	void SetBaudRate(int iBaudRate);
	int GetBaudRate();
	void SetComportNo(TCHAR* szComportNo);
	long m_iBaudRate;
	CString m_strComportNo;
	long m_LightBrightness[4];
	CSCInspPacket * GetComm();
	CSCInspPacket *m_pSCInspPacket;
	void SetComm(CSCInspPacket * pSCInspPacket);
	int GetRVOption();
	CModelData * GetModelData();
	CModelData *m_pModelData;
	void SetModelData(CModelData * pModelData);
	CdblSize m_CamFov[4];
	void SetCurAddAlgoIndex(int nInspType);
	int m_nInspTypeToAdd;
	int GetCurAlgoToAddIndex();
	CSCAlgoBase * GetCurAlgoToAdd();
	CSCAlgoBase * GetAlgoInfo(int nInspType, CXmlNode* pNode = NULL);
	void SetAlgoInfo(int nInspType, CSCAlgoBase *pAlgo, CXmlNode* pNode);
	long m_lCamExpTime[4];
	BOOL m_bIsOffLine;
	CString m_ProductName;
	tInfoCamIng m_infoCamMousePos;
	int m_opMode;
	CString m_strNGSaveFolder;
	BOOL m_MsgWndOpen;
	long m_nCountOfStage;
	int GetCamNo();
	long m_nCountOfPos;
	BOOL m_bSaveTestImage;
	BOOL m_bDrawCross;
	long m_bUseDelOldImg;
	long m_nDaysDel;
	CdblSize GetPixelCalData(int iCamIndex);
	long m_nCountOfCamRemote;
	CString m_strRemoteCamFolder;
	long m_lIPAddr[4];
	long m_bUseRemoteCamUpdate;
	CString GetRemoteIPString();
	long m_bSaveAllImage;
	CString m_strRemoteImgName;
};

