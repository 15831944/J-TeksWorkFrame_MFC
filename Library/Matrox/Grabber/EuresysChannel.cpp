#include "..\stdafx.h"
#ifdef _MULTICAM
#include <multicam.h>
#include "EuresysChannel.h"
#include "..\Matrox.h"


#define EURESYS_SURFACE_COUNT 3

/////////////////////////////////////////////////////////////////////////////
// GlobalCallback

void WINAPI EuresysCallback(PMCSIGNALINFO SigInfo)
{
	if (SigInfo && SigInfo->Context)
	{
		CEuresysChannel* pChannel = (CEuresysChannel*)SigInfo->Context;
		pChannel->Callback(SigInfo);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Callback


CEuresysChannel::CEuresysChannel()
	: m_nChannel(0)
	, m_nExposeTime(2000)
	, m_pCurrent(NULL)
	, m_milIDBufProc(NULL)
	, m_milIDBufDisp(NULL)
	, m_bIsGrabDone(FALSE)
{
}


CEuresysChannel::~CEuresysChannel()
{
}


int CEuresysChannel::Init()
{
	// Set the board topology (only with a Grablink Expert 2)
	McSetParamInt(MC_BOARD + m_nChannel, MC_BoardTopology, MC_BoardTopology_MONO_DECA);
	// Create a channel and associate it with the first connector on the first board
	McCreate(MC_CHANNEL, &m_Channel);
	McSetParamInt(m_Channel, MC_DriverIndex, m_nChannel);
	// For all GrabLink boards but Grablink Expert 2 and Dualbase

	McSetParamStr(m_Channel, MC_Connector, "M");

	// For Grablink Expert 2 and Dualbase
	//McSetParamStr(m_Channel, MC_Connector, "A");
	// Choose the video standard
	McSetParamStr(m_Channel, MC_CamFile, "STC-SPB510PCL_8bit10TAP_RG");
	// Choose the camera expose duration

	McSetParamInt(m_Channel, MC_Expose_us, m_nExposeTime);
	McSetParamInt(m_Channel, MC_AcquisitionMode, MC_AcquisitionMode_SNAPSHOT);
	McSetParamInt(m_Channel, MC_TrigMode, MC_TrigMode_SOFT);
//	McSetParamInt(m_Channel, MC_NextTrigMode, MC_NextTrigMode_REPEAT);
//	McSetParamInt(m_Channel, MC_SeqLength_Fr, 1);
	// Retrieve image dimensions
	int SizeX, SizeY, BufferPitch;
	McGetParamInt(m_Channel, MC_ImageSizeX, &SizeX);
	McGetParamInt(m_Channel, MC_ImageSizeY, &SizeY);
	McGetParamInt(m_Channel, MC_BufferPitch, &BufferPitch);

	// The memory allocation for the images is automatically done by Multicam when activating the channel.
	// We only set the number of surfaces to be created by MultiCam.
	McSetParamInt(m_Channel, MC_SurfaceCount, EURESYS_SURFACE_COUNT);

	// Enable MultiCam signals
	McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
	McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);

	// Register the callback function
	McRegisterCallback(m_Channel, EuresysCallback, this);
	McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_IDLE);
	McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_ACTIVE);

	// - GrablinkSnapshot Sample Program

	// Initialize MIL
	MbufAlloc2d(m_milIDSystem, SizeX, SizeY, m_pMilBufProc->m_iDepth, m_pMilBufProc->m_BufAttr, &m_milIDBufProc);
	MbufAllocColor(m_milIDSystem, 3, SizeX, SizeY, M_UNSIGNED + 8L, M_IMAGE + M_DISP + M_PROC, &m_milIDBufDisp);

	//// Clear the buffer [CALL TO MIL] 
//	MbufClear(m_milIDBuffer, 0);
	m_pMilBufProc->SetIDBuffer(m_milIDBufProc);
	m_pMilBufDisp->SetIDBuffer(m_milIDBufDisp);

	return 0;
}

void CEuresysChannel::Callback(PMCSIGNALINFO SigInfo)
{
	// + GrablinkSnapshot Sample Program

	switch (SigInfo->Signal)
	{
	case MC_SIG_SURFACE_PROCESSING:
		// Update "current" surface address pointer
		McGetParamInt(SigInfo->SignalInfo, MC_SurfaceAddr, (PINT32)&m_pCurrent);
		m_pMilBufProc->Put(m_pCurrent);
		m_pMilBufDisp->Copy(m_pMilBufProc);
		m_bIsGrabDone = TRUE;
		break;
	case MC_SIG_ACQUISITION_FAILURE:
		MessageBox(NULL, _T("Acquisition Failure !"), _T("SCInspector"), MB_OK);
		break;
	default:
		break;

	}
	// - GrablinkSnapshot Sample Program
}

int CEuresysChannel::Grab()
{
	m_bIsGrabDone = FALSE;
	McSetParamInt(m_Channel, MC_ForceTrig, MC_ForceTrig_TRIG);
	return 0;
}


int CEuresysChannel::GrabContinuous()
{
	McSetParamInt(m_Channel, MC_SeqLength_Fr, MC_INDETERMINATE);
	McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_ACTIVE);
	return 0;
}


int CEuresysChannel::GrabStop()
{
	McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_IDLE);
	McSetParamInt(m_Channel, MC_TrigMode, MC_TrigMode_SOFT);
	McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_ACTIVE);
	return 0;
}


int CEuresysChannel::SetMilBuffer(CMilBuffer * pMilBufProc, CMilBuffer * pMilBufDisp)
{
	m_pMilBufProc = pMilBufProc;
	m_pMilBufDisp = pMilBufDisp;
	
	m_milIDApplication = m_pMilBufProc->m_milIDApplication;;
	m_milIDSystem = m_pMilBufProc->m_milIDSystem;
	return 0;
}
void CEuresysChannel::SetExpTime(int nExpTime)
{
	m_nExposeTime = nExpTime;
	if (!m_milIDBufProc)return;
	McSetParamInt(m_Channel, MC_Expose_us, m_nExposeTime);
}

BOOL CEuresysChannel::IsGrabDone()
{
	return m_bIsGrabDone;
}


void CEuresysChannel::Close()
{
	McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_IDLE);
	McDelete(m_Channel);
}

#endif