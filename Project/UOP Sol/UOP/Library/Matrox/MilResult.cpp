#include "StdAfx.h"
#include "MilResult.h"
#include <Matrox.h>

CMilResult::CMilResult(void)
{
	m_milIDResult=M_NULL;
	m_fModAlloc=FALSE;
}

CMilResult::~CMilResult(void)
{
	if(m_milIDResult)ModFree();
}

int CMilResult::GetIDResult(void)
{
	return m_milIDResult;
}

int CMilResult::ModAlloc(void)
{
	if(!m_milIDResult)
		MmodAllocResult(CMilSystem::Instance()->GetSystemID(), M_DEFAULT, &m_milIDResult);
	m_fModAlloc=TRUE;
	return m_milIDResult;
}

int CMilResult::ModFree(void)
{
	MmodFree(m_milIDResult);
	m_milIDResult=M_NULL;

	return 0;
}

int CMilResult::Close(void)
{
	if(m_fModAlloc)
	{
		ModFree();
		m_fModAlloc=FALSE;
	}

	return 0;
}

