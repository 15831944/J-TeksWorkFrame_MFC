#pragma once
#include "keylock.h"

class CGraphDlg;

#define MARK_END _T("End")


struct tAPMDDispData
{
	int iParticle01, iParticle03, iParticle05;
	int nAcc01, nAcc03, nAcc05;
	int iHumi;
	int iBattery;
	double dblTemp;
	double dblPressure;
};

struct tViewOptions
{
	int iXGridUnit;
	int iXSelUnit;
	int iYGridUnit;
	int iParticleType;
};

class CStatus
{
public:
	enum tProgramMode { PM_REVIEW, PM_MONITORING };
	enum tKeylockStatus { LK_NO, LK_INSTALLED };
	enum tDispMode { DM_FREQ, DM_DEN };

private:
	static CStatus *m_pInstance;
	static tProgramMode m_ProgramMode;
	static tKeylockStatus m_KeylockStatus;
	static tDispMode m_DispMode;
	static CStatus *m_pInstace;

public:
	CStatus();
	~CStatus();
	static BOOL SearchKeylock();
	static tProgramMode GetProgramMode();
	static void SetProgramMode(tProgramMode ProgramMode);
	static void SetKeylockStatus(tKeylockStatus KeylockStatus);
	static tKeylockStatus GetKeylockStatus();
	static void SetDispMode(tDispMode DispMode);
	static tDispMode GetDispMode();
	static CStatus * Instance();
	int GetDefault();
	long m_iBaudRate;
	int GetBaudRate();
	void SetBaudRate(int iBaudRate);
	void SetComportNo(TCHAR *szComportNo);
	CString GetComportNo();
	CString m_strComportNo;
	void SetDefault();
	CString GetDataSaveFolder();
	void SetDataSaveFolder(TCHAR * szDataSaveFolder);
	CString m_strDataSaveFolder;
	// 폴더를 제외한 이름 확장자는 자동생성
	CString m_strDataFileName;
	CString GetDataFileName();
	void SetDataFileName(TCHAR * szDataFileName);
	tViewOptions m_ViewOptions;
	void SetViewOptions(tViewOptions ViewOptions);
	CGraphDlg * m_pGraphDlg;
	int m_nMarkPos;
	int GetMarkPos();
	void SetMarkPos(int nPos);
	void IncMarkPos();
	long m_iUseWarning;
	long m_iUseTemp;
	long m_iUseP01;
	long m_iUseP03;
	long m_iUseP05;
	double m_dblThresTemp;
	long m_iThresP01; //! Particle 갯수 상한 0.1um
	long m_iThresP03;
	long m_iThresP05;
	long m_iTimeWarningSel;
	void SetNaviStatus(bool bStatus);
	bool m_bOnNavi;
	bool IsOnNavi();
	CString m_strCalDate;
	CString m_strSerialNo;
	long m_iMaxMarkInExlForm;
	CString strLastOpenFileName;
	CString m_strMakeDate;
	int m_nAutoMarkPos;
	int GetAutoMarkPos();
	void IncAutoMarkPos();
	void SetAutoMarkPos(int nPos);
	void SetCompareMode(bool bCompare);
	void SetOverlayMode(bool bOverlayMode);
	bool IsOverlayMode();
	bool m_bOverlayMode;
	bool IsCompareMode();
	bool m_bCompareMode;
};

