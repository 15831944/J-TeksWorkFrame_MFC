#pragma once

#include "MilClass.h"
#include "MilBase.h"

#define MAX_DIGITIZER 5
#define CREVIS_DCF _T("JMV-BS20A.dcf")
#define ES51_INI _T("XC-ES51.ini")
#define BS20A_INI _T("MV-BS20A.ini")
class CMilDigitizer;
class CMilBlob;
class CMilBuffer;
class CMilPat;
class CMilModFinder;
class CMilEdgeFinder;
class CMilDisplay;

/*!
\brief 
* Mil관련한 Main Class로 이를 통해 버퍼/Display등을 할당하고(나중에 제거등의 편의를 위해), 경계가 모호한 기능들을 만들어 두기 위한 Class임
@author 전병근
*/
class MIL_DLLDECL CMilVLib
{
public:
	CString strGrabberName;
	CMilVLib(void);
	~CMilVLib(void);
	static CMilVLib * Instance(void);
	static CMilVLib	*m_pInstance;
	int Setting(TCHAR* szGrabber, TCHAR *camFile);
	int AddCamera(TCHAR* szGrabber, TCHAR * camFile);
	int DefaultSetting(void);
	void Close(void);
	CMilBuffer* NewImage(void);
	CMilBuffer* NewImage(CMilBuffer *pOrgBuff);
	CMilDisplay* NewDisplay(void);
	CMilDigitizer* NewDigitizer(int tGrabber);
	CMilBlob*	 NewBlobTool(CMilBuffer *pMilBuffer);
	CMilPat*	NewPatTool(void);
	CMilModFinder* NewModFinderTool(void);
	CMilEdgeFinder* NewEdgeFinderTool(void);
	static CString		m_strError;
	static int			m_nPixelMapEntries;
	static CString		m_strPixelMapFilePath;

	static int			**m_pPointDefect;
	static int			**m_pColDefect;
	static int			**m_pRowDefect;

	static int			m_nPointDefectCnt;
	static int			m_nColDefectCnt;
	static int			m_nRowDefectCnt;

private:
	
	CMilApplication	*m_pMilApplication;
	CMilDisplay *m_pMilDisplay;
	CMilDigitizer *m_pMilDigitizer[MAX_DIGITIZER];
	CMilBuffer* m_pMilBuffer;
	int m_currDigiNum;
	int m_nDigitizer;
	
public:
	CMilDisplay * GetDefaultDisp(void);
	void SetDefDigitizer(int nDigitizer);
	int GrabContinuous(void);
	int SingleGrab(void);
	int SingleGrabTrigger(void);
	int StopGrab(void);
	int StopGrabContinuous(void);
	void MilSaveImage(CString DIR);
	CMilDigitizer* GetDigitizer(int iDigitizer=0);
	int SetAverageCounter(int nCounter) ;
	int GetAverageCounter(void);
    int SetAverageCounterSoft(int nCounter) ;
	int GetHRSCLevel(int nLevel);
	int GetAverageCounterSoft(void);
	int GetHRSCLevel(void);
	int SetImgEnhance(int nCount);
	int GetImgEnhance(void);
	
	int SetupTrigger(BOOL OnOff);
	int MakeCombineImage(CString strFileName, CMilBuffer *pMilSrc, int FileType=M_BMP);
	BOOL IsGrabDone(void);
	static int ScrnNormalize(CMilBuffer *pMilResult, CMilBuffer *pMilOrg, CMilBuffer *pMilWhite, CMilBuffer *pMilDark, int MaxWhiteValue);
	int SetAverageStatus(BOOL status);
	CMilBuffer* GetGrabBuffer(void);
};
