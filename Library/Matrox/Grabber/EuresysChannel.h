#pragma once
#ifdef _MULTICAM
#include "..\stdafx.h"
#include <MultiCam.h>
#include "..\MilBase.h"

class CMilBuffer;

class MIL_DLLDECL CEuresysChannel
{
public:
	CEuresysChannel();
	~CEuresysChannel();
	int m_nChannel;
	int Init();
	MCHANDLE m_Channel;
	int m_nExposeTime;
	int Grab();
	int GrabContinuous();
	int GrabStop();
	void Callback(PMCSIGNALINFO SigInfo);
	int SetMilBuffer(CMilBuffer * pMilBufProc, CMilBuffer * pMilBufDisp = NULL);
	CMilBuffer *m_pMilBufDisp;
	CMilBuffer *m_pMilBufProc;
	MIL_ID m_milIDApplication;
	MIL_ID m_milIDSystem;
	MIL_ID m_milIDBufProc;
	MIL_ID m_milIDBufDisp;
	BOOL	m_bIsGrabDone;
	PVOID m_pCurrent; //!생성자에서 NULL로 해주지 않으면 에러발생!!!!
	void SetExpTime(int nExpTime);
	BOOL IsGrabDone();
	void Close();
};
#endif
