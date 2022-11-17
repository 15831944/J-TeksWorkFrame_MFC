#include "StdAfx.h"
#include "Grabber.h"
#include "MatroxGrabber.h"
#include "MatroxChannel.h"

CGrabber::CGrabber(void)
{
}

CGrabber::~CGrabber(void)
{
}

int CGrabber::Init(void)
{
	return 0;
}

CGrabber * CGrabber::Create(TCHAR *strGrabber)
{
	CGrabber *pInstance;

	m_strGrabberName = strGrabber;

	if(!m_strGrabberName.Compare(GB_NONE)) pInstance = new CGrabber;
	if(!m_strGrabberName.Compare(GB_METER4M)) pInstance = new CMatroxGrabber;

	return pInstance;
}


int CGrabber::Alloc(void)
{
	return 0;
}

CGrabChannel* CGrabber::NewChannel(TCHAR * szCamFile)
{
	CGrabChannel* pGrabChannel;

	if(!m_strGrabberName.Compare(GB_NONE)) pGrabChannel = new CGrabChannel;
	if(!m_strGrabberName.Compare(GB_METER4M)) pGrabChannel = new CMatroxChannel;
	return pGrabChannel;
}
