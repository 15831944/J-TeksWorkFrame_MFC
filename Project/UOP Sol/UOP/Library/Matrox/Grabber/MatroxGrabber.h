#pragma once
#include "Grabber.h"
#include "GrabChannel.h"
#include <mil.h>

class CMilDigitizer;

class CMatroxGrabber :public CGrabber
{
public:
	CMatroxGrabber(void);
	~CMatroxGrabber(void);
	int Init(void);
	CMilDigitizer *m_pMilDigitizer;
	CGrabChannel* NewChannel(TCHAR * szCamFile);
};
