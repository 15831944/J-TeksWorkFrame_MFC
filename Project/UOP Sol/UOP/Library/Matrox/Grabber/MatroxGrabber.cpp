#include "StdAfx.h"
#include "MatroxGrabber.h"
#include "MatroxChannel.h"
#include <matrox.h>

CMatroxGrabber::CMatroxGrabber(void)
{
}

CMatroxGrabber::~CMatroxGrabber(void)
{
}

int CMatroxGrabber::Init(void)
{
	return MsysAlloc(M_SYSTEM_SOLIOS, M_DEFAULT, M_DEFAULT, &m_milIDSystem);
}

CGrabChannel* CMatroxGrabber::NewChannel(TCHAR * szCamFile)
{
	CMatroxChannel *pChannel;
	pChannel= new CMatroxChannel;
	
	pChannel->m_strCameraFile = szCamFile;
	return pChannel;
}
