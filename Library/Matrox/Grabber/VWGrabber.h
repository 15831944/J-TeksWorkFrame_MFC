#pragma once
#ifdef _VIEWWORKS
#include "..\MilBase.h"
#include "VWChannel.h"

class MIL_DLLDECL CVWGrabber
{
public:
	CVWGrabber();
	~CVWGrabber();
	void Close();
	int Init();
	int m_nDeviceNum;
	void * m_pDeviceList;
	CVWChannel * NewChannel();
};
#endif

