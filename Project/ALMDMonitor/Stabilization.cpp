#include "stdafx.h"
#include "Stabilization.h"


#define DEFAULT_SENSITIVITY 30

CStabilization * CStabilization::m_pInstance = NULL;
CStabilization::CStabilization()
	: m_pPointArray(NULL)
	, m_IndexToWrite(0)
	, m_IndexToRead(0)
	, m_sizeRingBuffer(40)
	, m_nCount(0)
	, m_bIsStableX(FALSE)
	, m_bIsStableY(FALSE)
	, m_bIsStable(FALSE)
{
	m_iSensitivity = GetDefaultSensitivity();
	m_pPointArray = new CPoint[m_sizeRingBuffer];
}


CStabilization::~CStabilization()
{
	delete m_pPointArray;
	m_pPointArray = NULL;
}

//! x값이 3이면 3이면 실제 0.003도임
void CStabilization::Add(CPoint degreeValue)
{
	m_pPointArray[m_IndexToWrite] = degreeValue;
	m_IndexToWrite++;
	if(m_nCount < m_sizeRingBuffer)m_nCount++;
	m_IndexToWrite %= m_sizeRingBuffer;
	CalcStable();
}


BOOL CStabilization::IsStable()
{
	return m_bIsStable;
}


CStabilization * CStabilization::Instance()
{
	if (!m_pInstance)m_pInstance = new CStabilization;
	return m_pInstance;
}


void CStabilization::GetAverage(double *pDblAvrX, double *pDblAvrY)
{
	double sumx = 0.0;
	double sumy = 0.0;
	for (int i = 0; i < m_nCount; i++)
	{
		sumx += m_pPointArray[i].x;
		sumy += m_pPointArray[i].y;
	}

	double AverageX = sumx / m_nCount;
	double AverageY = sumy / m_nCount;
	*pDblAvrX = AverageX;
	*pDblAvrY = AverageY;
}


void CStabilization::Init()
{
	if (m_pPointArray)
		delete m_pPointArray;
	m_pPointArray = new CPoint[m_sizeRingBuffer];
	m_IndexToWrite = 0;
	m_IndexToRead = 0;
	m_sizeRingBuffer = 40;
	m_nCount = 0;
}


void CStabilization::CalcStable()
{
	double avrX = 0, avrY = 0;
	GetAverage(&avrX, &avrY);
	BOOL bIsStableX = true;
	BOOL bIsStableY = true;
	BOOL bIsStable = true;
	if (m_nCount < m_sizeRingBuffer)
	{
		m_bIsStable = m_bIsStableX = m_bIsStableY = false;
		return;
	}

	for (int i = 0; i < m_nCount-1; i++)
	{
		if (fabs(avrX - m_pPointArray[i].x) > m_iSensitivity)
		{
			bIsStableX = false;
			break;
		}
	}
	for (int i = 0; i < m_nCount-1; i++)
	{
		if (fabs(avrY - m_pPointArray[i].y) > m_iSensitivity)
		{
			bIsStableY = false;
			break;
		}
	}
	if (bIsStableX && bIsStableY)
		bIsStable = true;
	else
		bIsStable = false;
	m_bIsStable = bIsStable;
	m_bIsStableX = bIsStableX;
	m_bIsStableY = bIsStableY;
}


// iSensitivity값이 5이면 Sensitivity값이 0.005임
void CStabilization::SetSensitivity(int iSensitivity)
{
	m_iSensitivity = iSensitivity;
}


int CStabilization::GetDefaultSensitivity()
{
	return DEFAULT_SENSITIVITY;
}
