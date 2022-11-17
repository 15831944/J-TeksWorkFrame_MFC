#pragma once
//#include "keylock.h"
#include "..\..\Library\SerialLib\SerialMFC.h"	// 필수
#include "..\..\Library\SerialLib\ALMDPacket.h"	// 필수
#include "Repository.h"
#include "..\..\Library\UniUtility\UniUtility.h"

class CGraphDlg;
class CALMDStatusDlg;
class COpPanelDlg;
class CViewDlg;

#define MARK_END _T("End")


struct tViewOptions
{
	int iXGridUnit;
	int iXSelUnit;
	int iYGridUnit;
	int iParticleType;
};

struct tAdjustAngle
{
	int iAdjRoll;
	int iAdjPitch;
};

struct tMarkInfo;

class CStatus
{
public:
	enum tProgramMode { PM_REVIEW, PM_MONITORING };
	enum tKeylockStatus { LK_NO, LK_INSTALLED };
	enum tCoordinateSystem { CS_CARTESIAN, CS_POLAR };
	enum tAngleType {AT_DEGREE, AT_RADIAN};
	enum tIncType {IT_ANGLE, IT_ROR};
	enum tRORType{RT_MMD, RT_MMR};

private:
	static CStatus *m_pInstance;
	static tProgramMode m_ProgramMode;
	static tKeylockStatus m_KeylockStatus;
	static tCoordinateSystem m_iCoordinateSystem;
	static CStatus *m_pInstace;

public:
	CStatus();
	~CStatus();
	static BOOL SearchKeylock();
	static tProgramMode GetProgramMode();
	static void SetProgramMode(tProgramMode ProgramMode);
	static void SetKeylockStatus(tKeylockStatus KeylockStatus);
	static tKeylockStatus GetKeylockStatus();
	static void SetCoordinateSystem(tCoordinateSystem DispMode);
	void SetAngleType(tAngleType iKofAngle);
	void SetRORType(tRORType iKofROR);
	static tCoordinateSystem GetCoordinateSystem();
	tAngleType GetAngleType();
	tRORType GetRORType();
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
	int m_nMarkPos;
	int GetMarkPos();
	void SetMarkPos(int nPos);
	void IncMarkPos();
	long m_iUseWarning;
	long m_iUseTemp;
	double m_dblThresTemp;
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
	int GetInclinationType();
	tAngleType m_iAngleType;
	tRORType m_iRORType;
	void SetInclinationType(tIncType iIncType);
	tIncType m_iInclinationType;
	void SetWaferSize(int sizeWafer);
	int m_iWaferSize;
	int GetWaferSize();
	void SetAdjuestAngle(tAdjustAngle AdjustAngle);
	tAdjustAngle m_AdjustAngle;
	tAdjustAngle GetAdjustAngle();
	void SetMeasureData(tALMDRecord * pCurData);
	tALMDRecord m_curMeasureData;
	tALMDRecord * GetCurMeasureData();
	void SetTolerance(double dblTolerance);
	double m_dblTolerance;
	double GetTolerance();
	tMarkInfo GetMarkToAdd();
	void SetMarkToAdd(tMarkInfo * pMarkToAdd);
	tMarkInfo m_MarkToAdd;
	void ClearMarkToAdd();
	static COpPanelDlg *m_pOpPanelDlg;
	static CALMDStatusDlg *m_pALMDStatusDlg;
	static CViewDlg *m_pViewDlg;
	CRepository *m_pRepository;
	void UpdateScrn();
	CdblPoint m_dptRelPlan;
	BOOL IsRelative();
	BOOL m_bIsRel;
	void SetRelMode(bool bRelMode);
	double m_iStabilizationValue;
	double m_dblStblzSensitivity;
	double GetStblzSensitivity();
	void SetStblzSensitivity(double dblSensitivity);
	CString GetLastOpenFileName();
	BOOL IsDigit2Mode();
};

