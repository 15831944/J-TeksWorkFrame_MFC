#pragma once

#include "..\..\Library\SerialLib\SerialMFC.h"	// 필수
#include "..\..\Library\SerialLib\AGMDPacket.h"	// 필수
#include "Repository.h"


class CAGMDMonitorView;
class CMonitorDlg;
class CSenSorStatusDlg;
class CSenSorViewDlg;
class COPPanelDlg;
class CBrowserDlg;
class CStationInfomationDlg;
struct tCalData
{
	BOOL bRcvCalInfo;
	UINT	uJigGap;
	int		iGap1;
	int		iGap2;
	int		iGap3;
};

class CStatus
{
public:
	enum tProgramMode { PM_MONITORING, PM_REVIEW };
	enum tNotchMode { TWO_CLOCK, FOUR_CLOCK, SIX_CLOCK, EIGHT_CLOCK, TEN_CLOCK, TWELVE_CLOCK };
	enum tUnitMode { UNIT_INCH, UNIT_MM };
public:
	CStatus();
	~CStatus();
	static CMonitorDlg *m_pMonitorDlg;
	static tProgramMode GetProgramMode();
	static tProgramMode m_ProgramMode;
	static void SetProgramMode(tProgramMode ProgramMode);
	CString GetComportNo();
	void SetBaudRate(int iBaudRate);
	int GetBaudRate();
	void SetComportNo(TCHAR* szComportNo);
	tNotchMode m_NotchMode;
	tNotchMode GetNotchMode();
	void SetNotchMode(tNotchMode NotchMode);

	tUnitMode m_UnitMode;
	tUnitMode GetUnitMode();
	void SetUnitMode(tUnitMode NotchMode);

	BOOL m_bIsGapSameSettings;
	void SetIsGapSameSettings(BOOL bIsGapSameSettings);
	BOOL GetIsGapSameSettings();

	static CStatus *m_pInstance;
	static CStatus* Instance();
	static CAGMDMonitorView* m_pAGMDMonitorView;
	void SetAGMDView(CAGMDMonitorView* pAGMDMonitorView);
	CAGMDMonitorView* GetAGMDView();

	static COPPanelDlg* m_pAGMDOPPanelDlg;
	void SetOPPanelDlg(COPPanelDlg* pOPPanelDlg);
	COPPanelDlg* GetOPPanelDlg();

	static CRepository *m_pRepository;
	CRepository * GetRepository();
	void SetRepository(CRepository * pRepository);

	static CBrowserDlg* m_pBrowserDlg;
	void SetBrowserDlg(CBrowserDlg* pBrowserDlg);
	CBrowserDlg* GetBrowserDlg();

	static CStationInfomationDlg* m_pStationInfoDlg;
	void SetStationInfomationDlg(CStationInfomationDlg* pSationInfoDlg);

	int GetDefault();
	void SetDefault();
	void SetWaferSize(int sizeWafer);
	void SetMonitorDlg(CMonitorDlg * pMonitorDlg);
	CMonitorDlg* GetMonitorDlg();
	void SetMeasureData(tAGMDRecord *pCurData);
	CString m_strComportNo;
	CString m_strDataSaveFolder;
	CString m_strDataFileName;
	long m_iBaudRate;
	CString m_strCalDate;
	CString m_strSerialNo;
	int m_iWaferSize;
	BYTE m_byBattery;
	int m_iTemperature;
	static CSenSorStatusDlg *m_pSensorStatusDlg;
	void SetSensorStatusDlg(CSenSorStatusDlg * pSensorStatusDlg);
	CSenSorStatusDlg* GetSensorStatusDlg();
	static CSenSorViewDlg *m_pSensorViewDlg;
	void SetSensorViewDlg(CSenSorViewDlg * pSensorViewDlg);
	void SetDataSaveFolder(TCHAR * szDataSaveFolder);
	CString GetDataSaveFolder();
	CString GetDataFileName();
	void SetDataFileName(TCHAR * szDataFileName);
	tAGMDRecord m_curMeasureData;
	tAGMDRecord * GetCurMeasureData();
	Color m_ToleranceColor;
	Color m_SpecInColor;
	Color m_SpecSmallOutColor;
	Color m_SpecBigOutColor;
private:
	void SetNotchNUnitMode(int iNotchMode, int iUnitMode);
public:
	void UpdateScrn();
	BOOL m_bReEnter;
private:
	double m_dblTarget[4]; // 0: gap1 , 1: gap2, 2 : gap3, 3 : Allgap
	double m_dblTolerance[4];
	double m_dblOffset[3];
public:
	void SetTarget(int iGapNum, double dblTarget);
	void SetPointTarget(int iGapNum, double dblTarget);
	
	double GetTarget(int iGapNum);
	void SetTolerance(int iGapNum, double dblTolerance); 
	void SetPointTolerance(int iGapNum, double dblTolerance);
	double GetTolerance(int iGapNum);
	double GetGapRangeView();
	void SetGapRangeView(double dblValue);
	void SetOffset(int iGapNum, double dblOffset);
	double GetOffset(int iGapNum);
	double GetCalcReview(double dblValue);
	double m_dblViewRange;
private:
	Color RGBToColor(long iRValue, long iGValue, long iBValue);
	BOOL m_bIsOffsetMode;
public:
	void SetOffsetMode(BOOL bIsOffsetMode);
	BOOL GetIsOffsetMode();
	void OPPanelUnitUpdate();
	void MonitorUpdate();
	void OPPanelOffsetUpdate();
	void OpPanelOffsetInitialize();
	tWorkEnvInfo m_WorkEnvInfo;
	void SetWorkEnvInfo(tWorkEnvInfo *pWorkEnvInfo);
	tWorkEnvInfo GetWorkEnvInfo();
	double GetOrgOffset(int iGapNum);
	CString m_strLastOpenFileName;
	BOOL GetLedStatus();
	void OnInitBrowserUpdate();
	void LogSettingApply(tAGMDRecord *pCurData);
	int m_iLastUnitMode;
	CString GetLastOpenFileName();
	tCalData m_CalData;
	int m_iNewJigGap;
	void AllDlgClear();
	void StationInfoUpdate();
	CString m_strMark;
};

