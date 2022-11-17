#pragma once
#include "GrabChannel.h"
#include <mil.h>

class CMilSystem;
class CMilBuffer;
///Mil Digitizer를 활용한다.
class CMatroxChannel :
	public CGrabChannel
{
public:
	CMatroxChannel(void);
	~CMatroxChannel(void);
	int Create(CMilSystem* pMilSystem);
	int GetResolution(CSize& Rect);
	int SetBuffer(CMilBuffer * pMilBuffer);
	int StopGrab(void);
	int Grab(void);
	int GrabContinuous(void);
	int Close(void);

	MIL_ID m_milIDApplication, m_milIDSystem, m_milIDDigitizer;
	CMilSystem *m_pMilSystem;
	CMilBuffer *m_pMilBuffer;
	CString m_strCameraFile;
	int Alloc(void);
};
