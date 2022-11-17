#include "..\stdafx.h"
#ifdef _VIEWWORKS
#include "VWGrabber.h"
#include "ViVIX.T_def.h"
#include "ViVIX.T_API.h"
using namespace VWSDK;
using namespace VWSDK::T;

//#pragma comment(lib, "ViVIX.T_SDK")

CVWGrabber::CVWGrabber()
{
}


CVWGrabber::~CVWGrabber()
{
}




void CVWGrabber::Close()
{
}


int CVWGrabber::Init()
{
	RESULT	result = RESULT_ERROR;
	DEVICE_DISCOVERY_INFO *pDeviceList = (DEVICE_DISCOVERY_INFO *)m_pDeviceList;

	result = VwInitialize();
	if (RESULT_SUCCESS != result && RESULT_ERROR_OPENED_ALREADY != result)
	{
		AfxMessageBox(_T("Create detector failed."));
		return -1;
	}

	result = VwDiscoveryDevice(&m_nDeviceNum, &pDeviceList);
	if (result != RESULT_SUCCESS)
	{
		return -2;
	}

	if (m_nDeviceNum > 0)
	{
		for (int i = 0; i<m_nDeviceNum; i++)
		{
			CString strDevice(pDeviceList[i].model);
			//			m_cbDevice.AddString(strDevice);
		}
	}
	else
		return -3;

	return 0;
}


CVWChannel * CVWGrabber::NewChannel()
{
	static int nChannel = 0;

	if (nChannel >= m_nDeviceNum)return NULL;

	CVWChannel *pChannel = new CVWChannel;
	pChannel->m_nChannel = nChannel;
	nChannel++;
	return pChannel;
}
#endif