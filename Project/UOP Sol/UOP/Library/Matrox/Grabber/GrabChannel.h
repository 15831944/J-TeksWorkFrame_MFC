#pragma once
#include <Matrox.h>
#include <UniUtility.h>

class CGrabber;
class CMilCal;



/*
\brief 
* Grabber의 Channel관리 기초 Interface Class
* 한Grabber에는 다중 Channel을 둘수 있고 다중 Channel은 각각의 Channel Class를 두게 됨
@author 전병근
*/
class CGrabChannel
{
public:
	CGrabChannel(void);
	virtual ~CGrabChannel(void);
	CGrabber * m_pGrabber;
	CString m_strCameraFile;
	virtual int Create(void);
	virtual int GetResolution(CSize& Rect);
	virtual int SetExposeTime(long epTime);
	virtual int GetExposeTime();
	virtual int SetBuffer(CMilBuffer * pMilBuffer);
	virtual int StopGrab(void);
	virtual int Grab(void);
	virtual	int GrabContinuous(void);
	virtual int GrabContinuousStop(void);
	virtual int StopAvreage(BOOL GRAB);
	virtual int StartAvreage(BOOL GRAB);
	virtual int SetupTrigger(BOOL OnOff); 
	virtual int OnTrigger(BOOL On);
	virtual int RotateImage(BOOL value);
	virtual int HorizonMirr(BOOL value);
	virtual int VerticalMirr(BOOL value);
	virtual int SingleGrab(void);
	virtual int GrabTrigger(void);
	virtual int Close(void);
	CMilBuffer *m_pMilBuffer;
	CMilBuffer *m_pMilChildBuff;
	static CMilBuffer *m_pGrabBuffer;
	CMilBuffer *m_pImageWhite;
	CMilBuffer *m_pImageDark;
	static long m_lExposeTime;
	CMilBuffer *m_pAvrBuffer;
	CMilBuffer *m_pTmpBuffer;
	static CMilBuffer *m_pfillterBuffer1;
	static CMilBuffer *m_pfillterBuffer2;
	static CMilBuffer *m_pfillterBuffer3;
	static CMilBuffer *m_pfillterBuffer4;
	static CMilBuffer *m_pfillterBuffer5;
	static CMilBuffer *m_pfillterBuffer6;
	static CMilBuffer *m_pfillterBuffer7;
	static CMilBuffer *m_pfillterBuffer8;
	static CMilBuffer *m_pfillterBuffer9;
	static CMilBuffer *m_pfillterBuffer10;

	// 현재 인스턴스의 체널번호
	int m_nChannel;
	CMilBuffer* GetBuffer(void);
	virtual int SetAverageCounter(int nCounter);
	virtual int SetAverageCounterSoft(int nCounter);
	virtual int SetHRSCLevel(int nLevel);
	virtual int GetAverageCounter(void);
	virtual int GetAverageCounterSoft(void);
	virtual int GetHRSCLevel(void);
	static int		m_bUseManHisto;
	static int		m_bUseHFilltero;
	static tWndLv	m_WndLv;
	static BOOL		m_bWarpCal;
	static BOOL		m_bScrnNormalize;
	static 	BOOL m_bInlineAvgEnable;
	static long *m_pOrgHistoData;
	static long *m_pDestHistoData;
	static CMilCal	*m_pMilWarpCal;
	virtual long * GetOrgHistoBuff(void);
	virtual long * GetDestHistoBuff(void);
	virtual void SetManHistoMode(BOOL state);
	virtual void SetHFiltero(BOOL state);
	virtual BOOL	IsManHistogram(void);
	virtual BOOL     IsManHFiltero(void);
	virtual void SetInputRange(long lStart, long lEnd);


protected:
	int m_iAverageCounter;
	int m_iAverageCounterSoft;
	int m_iHRSCLevel;
	int m_iMaxHistoValue;
	BOOL m_bImgEnhance;

	tBNState m_bnState;

public:
	virtual void SetInlineAvgEnable(BOOL bValue);
	virtual BOOL GetInlineAvgEnable(void);
	virtual void SetImgEnhance(BOOL bStatus);
	virtual BOOL GetImgEnhance(void);
	virtual void SetWarpCal(BOOL bState);
	virtual BOOL GetWarpCal(void);
	virtual void SetScrnNormalize(BOOL bState);
	virtual BOOL GetScrnNormalize(void);
	virtual void SetBrightNormalize(tBNState bnStatus);
	virtual tBNState GetBrightNormalize(void);
	virtual int GetDepth(void);
	virtual BOOL IsGrabDone(void);
};
