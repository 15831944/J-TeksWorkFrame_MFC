#include "stdafx.h"
#include "MilClass.h"
#include <mil.h>
#include "MilSystem.h"
//#include <XaGrabber.h>

CMilApplication *CMilSystem::m_pMilApplication;
vector<CMilSystem *> CMilSystem::m_vMilSystem;
CMilSystem::CMilSystem(void):m_milIDApplication(NULL)
{
	m_pMilSystem=NULL;
}

CMilSystem::CMilSystem(CMilApplication *pMilApplication):m_milIDApplication(NULL)
{
	m_milIDApplication = pMilApplication->m_milIDApplication;
}

CMilSystem::~CMilSystem(void)
{
}

int CMilSystem::Create(CMilApplication *pMilApplication)
{
	m_milIDApplication = pMilApplication->m_milIDApplication;
	return Create(m_strGrabberName.GetBuffer());
}

int CMilSystem::Create(TCHAR* szGrabber)
{
	m_strGrabberName = szGrabber;

	CGrabber gbMaker;
	m_pGrabber = gbMaker.Create(m_strGrabberName.GetBuffer());
	m_pGrabber->m_strGrabberName = m_strGrabberName;

	int mysize=m_vMilSystem.size();
	if(m_vMilSystem.size()!=1)
	{
		m_pGrabber->Init();
		SetSystemID(m_vMilSystem[0]->GetSystemID());
	}else{
		m_pGrabber->Init();
		MsysAlloc(M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &m_milIDSystem);
		m_pGrabber->m_milIDSystem = m_milIDSystem;
	}
	return 0;
}

CMilSystem * CMilSystem::Instance()
{
	CMilSystem *pMilSystem;
	if(m_vMilSystem.size()>0)
	{
		pMilSystem= m_vMilSystem[0];
	}else
	{
		pMilSystem = new CMilSystem;
		pMilSystem->m_strGrabberName = GB_NONE;
		pMilSystem->m_pMilSystem = pMilSystem;
		m_vMilSystem.push_back(pMilSystem);
	}
	return pMilSystem;
}

CMilSystem * CMilSystem::Instance(TCHAR* szGrabber)
{
	CMilSystem *pMilSystem;
	if(m_vMilSystem.size()>0)
	{
		for(UINT i=0;i<m_vMilSystem.size();i++)
		{
			pMilSystem= m_vMilSystem[i];
			if(!pMilSystem->m_strGrabberName.Compare(szGrabber))return pMilSystem;
		}
	}

	pMilSystem = new CMilSystem;
	pMilSystem->m_strGrabberName = szGrabber;
	pMilSystem->m_pMilSystem = pMilSystem;
	m_vMilSystem.push_back(pMilSystem);

	return pMilSystem;
}

int CMilSystem::Close(void)
{
	if(m_milIDSystem!=M_NULL)

	{
		MsysFree(m_milIDSystem);
		m_milIDSystem=M_NULL;
	}
	return 0;
}


MIL_ID CMilSystem::GetSystemID(void)
{
	return m_milIDSystem;
}

void CMilSystem::SetSystemID(MIL_ID idSystem)
{
	m_milIDSystem=idSystem;
}

int CMilSystem::GetDongleType(void)
{
  /*Dongle license Type
	M_LICENSE_BLOB
	M_LICENSE_CAL
	M_LICENSE_CODE
	M_LICENSE_COL
	M_LICENSE_3DCA
	M_LICENSE_DEBUG
	M_LICENSE_DMIL
	M_LICENSE_3DPR
	M_LICENSE_EDGE
	M_LICENSE_GPU
	M_LICENSE_IM
	M_LICENSE_INTERFACE
	M_LICENSE_JPEG2000
	M_LICENSE_JPEGSTD
	M_LICENSE_LITE
	M_LICENSE_MEAS
	M_LICENSE_MET
	M_LICENSE_MOD
	M_LICENSE_OCR
	M_LICENSE_PAT
	M_LICENSE_REG
	M_LICENSE_STR*/

	MIL_INT LicenseType;
	MsysInquire(m_milIDSystem, M_LICENSE_MODULES, &LicenseType);
	return LicenseType;
}

CMilDigitizer* CMilSystem::NewDigitizer(TCHAR *pCamFile)
{
	CMilDigitizer *pMilDigitizer;
	pMilDigitizer = new CMilDigitizer;
	pMilDigitizer->m_strCameraFile = pCamFile;
	
	return pMilDigitizer;
}
