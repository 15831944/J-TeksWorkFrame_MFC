#include "StdAfx.h"
#include "MilClass.h"
#include "MilApplication.h"

CMilApplication::CMilApplication(void)
{
}

CMilApplication::~CMilApplication(void)
{
	Close();
}

int CMilApplication::Create(void)
{
	if (MappAlloc(M_DEFAULT, &m_milIDApplication) == M_NULL)
		return -1; /// fail
	else
		return 0; /// success
}

void CMilApplication::Close(void)
{
	if(m_milIDApplication!=M_NULL)
	{
		MappFree(m_milIDApplication);
		m_milIDApplication=M_NULL;
	}
}
