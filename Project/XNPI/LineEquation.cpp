#include "stdafx.h"
#include "LineEquation.h"


CLineEquation::CLineEquation()
	: m_coeA(0)
	, m_coeB(0)
	, m_coeC(0)
	, m_bIsSetCoe(FALSE)
	, m_iDir(0)
{
}


CLineEquation::~CLineEquation()
{
}


double CLineEquation::GetY(double dblX)
{
	if(m_iDir)
		return -1 * ((m_coeA / m_coeB)*dblX) + m_coeC;
	else
		return -1*((m_coeA/m_coeB)*dblX) - m_coeC;
}


void CLineEquation::SetCoe(double coeA, double coeB, double coeC, int iDir)
{
	m_coeA = coeA;
	m_coeB = coeB;
	m_coeC = coeC;
	m_iDir = iDir;
	m_bIsSetCoe = TRUE;
}


BOOL CLineEquation::IsSetCoe()
{
	return m_bIsSetCoe;
}
