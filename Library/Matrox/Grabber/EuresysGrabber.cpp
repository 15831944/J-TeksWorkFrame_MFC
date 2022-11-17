#include "..\stdafx.h"
#ifdef _MULTICAM
#include "multicam.h"
#include "EuresysChannel.h"
#include "EuresysGrabber.h"
#include <Mil.h>
#pragma comment(lib, "Multicam")

CEuresysGrabber::CEuresysGrabber()
	: m_pMilBuffer(NULL)
{
}


CEuresysGrabber::~CEuresysGrabber()
{
}

//! return -1 Error 0:OK
int CEuresysGrabber::Init()
{
	// Initialize driver and error handling
	int BoardCount = 0;
	if(McOpenDriver(NULL)!=MC_OK)return -1;
	 McGetParamInt(MC_CONFIGURATION, MC_BoardCount, &BoardCount);
	 if (!BoardCount)return -1;
	// Activate message box error handling and generate an error log file
	McSetParamInt(MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_MSGBOX);
	McSetParamStr(MC_CONFIGURATION, MC_ErrorLog, "error.log");
	return 0;
}

void CEuresysGrabber::Close()
{
	McCloseDriver();
}

CEuresysChannel * CEuresysGrabber::NewChannel()
{
	static int nChannel = 0;

	CEuresysChannel *pChannel = new CEuresysChannel;
	pChannel->m_nChannel = nChannel;
	nChannel++;
	return pChannel;
}

#endif