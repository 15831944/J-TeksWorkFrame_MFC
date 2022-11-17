#include "StdAfx.h"
#include "MatroxChannel.h"
#include <Matrox.h>

CMatroxChannel::CMatroxChannel(void)
{
}

CMatroxChannel::~CMatroxChannel(void)
{
}


int CMatroxChannel::Create(CMilSystem* pMilSystem)
{
	m_pMilSystem = pMilSystem;
	m_milIDApplication = m_pMilSystem->m_milIDApplication;
	m_milIDSystem = pMilSystem->m_milIDSystem;

	Alloc();

	return 0;
}

int CMatroxChannel::GetResolution(CSize& Rect)
{
	MIL_INT cx, cy;
	MdigInquire(m_milIDDigitizer, M_SIZE_X, &cx); 
	MdigInquire(m_milIDDigitizer, M_SIZE_Y, &cy);
	Rect.SetSize(cx,cy);
	return 0;
}

int CMatroxChannel::SetBuffer(CMilBuffer * pMilBuffer)
{
	m_pMilBuffer = pMilBuffer;
	return 0;
}

int CMatroxChannel::StopGrab(void)
{
	MdigHalt(m_milIDDigitizer);
	return 0;
}

int CMatroxChannel::Grab(void)
{
	MdigGrab(m_milIDDigitizer, m_pMilBuffer->GetIDBuffer());
	return 0;
}

int CMatroxChannel::Close(void)
{
	MdigFree(m_milIDDigitizer);
	return 0;
}

int CMatroxChannel::GrabContinuous(void)
{
	MdigGrabContinuous(m_milIDDigitizer, m_pMilBuffer->GetIDBuffer());
	return 0;
}
int CMatroxChannel::Alloc(void)
{
	return 0;
}
