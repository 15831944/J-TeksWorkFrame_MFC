#include "StdAfx.h"
#include "MilDigitizer.h"
#include ".\Grabber\GrabChannel.h"

CMilDigitizer::CMilDigitizer(void)
{
}

CMilDigitizer::~CMilDigitizer(void)
{
	Close();
}

int CMilDigitizer::Create(CMilApplication * pMilApplication, CMilSystem *pMilSystem)
{
	m_milIDApplication = pMilApplication->m_milIDApplication;
	m_pMilSystem = pMilSystem;
	m_milIDSystem = pMilSystem->m_milIDSystem;
	m_pInstance = pMilSystem->NewDigitizer(m_strCameraFile.GetBuffer());
	m_pGrabChannel = pMilSystem->m_pGrabber->NewChannel(m_strCameraFile.GetBuffer());
	m_pGrabChannel->SetBuffer(m_pMilBuffer);
	return Alloc();
}

int CMilDigitizer::Alloc(void)
{
	return m_pGrabChannel->Create();
}

int CMilDigitizer::SetContinuous(void)
{
	return m_pGrabChannel->GrabContinuous();
}

int CMilDigitizer::Inquire(void)
{
	return 0;
}

int CMilDigitizer::GetResolution(CSize& Rect)
{
	return m_pGrabChannel->GetResolution(Rect);
}

int CMilDigitizer::SetBuffer(CMilBuffer *pMilBuffer)
{
	m_pMilBuffer = pMilBuffer;
	return 0;
}

int CMilDigitizer::Close(void)
{
	return m_pGrabChannel->Close();
}
int CMilDigitizer::SetupTrigger(BOOL OnOff)
{
   return m_pGrabChannel->SetupTrigger(OnOff);
}

int CMilDigitizer::GrabTrigger(void)
{

   return m_pGrabChannel->GrabTrigger();
}

int CMilDigitizer::Grab(void)
{
	return m_pGrabChannel->SingleGrab();
}


int CMilDigitizer::StopGrab(void)
{
	return m_pGrabChannel->StopGrab();
}

int  CMilDigitizer::StopGrabContinuous(void)
{
   return m_pGrabChannel->GrabContinuousStop();
}
CMilBuffer * CMilDigitizer::GetGrabBuffer(void)
{
	return m_pMilBuffer;
}
