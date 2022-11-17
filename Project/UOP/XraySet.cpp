#include "StdAfx.h"
#include "XraySet.h"

CXraySet::CXraySet(void)
{
	m_dblVol = 0;
	m_dblCur = 0;
}

CXraySet::~CXraySet(void)
{
}

void CXraySet::operator=(CXraySet& xraySet)
{
	m_dblVol=xraySet.m_dblVol;
	m_dblCur=xraySet.m_dblCur;
}

BOOL CXraySet::operator==(CXraySet& xraySet)
{
	if( (m_dblVol==xraySet.m_dblVol) && (m_dblCur==xraySet.m_dblCur))return TRUE;
	else return FALSE;
}

CXraySet CXraySet::operator+(CXraySet& xraySet)
{
	CXraySet tmpSet;
	tmpSet.m_dblCur = m_dblCur+xraySet.m_dblCur;
	tmpSet.m_dblVol = m_dblVol+xraySet.m_dblVol;
	return tmpSet;
}

CXraySet CXraySet::operator-(CXraySet& xraySet)
{
	CXraySet tmpSet;
	tmpSet.m_dblCur = m_dblCur-xraySet.m_dblCur;
	tmpSet.m_dblVol = m_dblVol-xraySet.m_dblVol;
	return tmpSet;
}

CXraySet::CXraySet(double setVol, double setCur)
{
	m_dblVol=(long)setVol;
	m_dblCur=(long)setCur;
}
