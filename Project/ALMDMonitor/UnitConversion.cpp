#include "stdafx.h"
#include "Status.h"
#include "UnitConversion.h"
#define _USE_MATH_DEFINES
#include <math.h>

CUnitConversion * CUnitConversion::m_pUnitConversion = NULL;
CUnitConversion::CUnitConversion()
	: m_iWaferSize(300)
	, m_dblRoll(0)
	, m_dblPitch(0)
	, m_iCoordinateSystem(0)
	, m_iInclinationType(0)
{
}


CUnitConversion::~CUnitConversion()
{
}


void CUnitConversion::SetRollAndPitch(double dblRoll, double dblPitch)
{
	m_dblRoll = dblRoll;
	m_dblPitch = dblPitch;
}


void CUnitConversion::SetWaferSize(int iSize)
{
	m_iWaferSize = iSize;
}

double CUnitConversion::CalcMagnitude()
{
//	return acos((cos(m_dblRoll / 10)*cos(m_dblPitch / 10))) * 10;
	double dblAns = cos(m_dblRoll*(M_PI / 180))*cos(m_dblPitch*(M_PI / 180));
	return acos(dblAns) * (180/M_PI);
}

double CUnitConversion::CalcDirection()
{
	int Quadrant = CheckQuadrant(m_dblRoll, m_dblPitch);
	double ratio = 0;
	switch (Quadrant)
	{
	case 1:
		ratio = fabs(m_dblPitch) / fabs(m_dblRoll);
		break;
	case 2:
		ratio = fabs(m_dblPitch) / fabs(m_dblRoll);
		break;
	case 3:
		ratio = fabs(m_dblRoll) / fabs(m_dblPitch);
		break;
	case 4:
		ratio = fabs(m_dblRoll) / fabs(m_dblPitch);
		break;
	}

	double Dir = atan(ratio);
	Dir = (Dir * 180) / M_PI;
	switch (Quadrant)
	{
	case 1:
		break;
	case 2:
		Dir += 180;
		break;
	case 3:
		Dir += 90;
		break;
	case 4:
		Dir += 270;
		break;
	}
	return Dir;
}

int CUnitConversion::CheckQuadrant(double dblRoll, double dblPitch)
{
	int rv = 0;
	if		((dblRoll >= 0) && (dblPitch >= 0)) rv = 1;		//+,+  normal
	else if ((dblRoll <= 0) && (dblPitch <= 0)) rv = 2;		//-,-  180' sum
	else if ((dblRoll <= 0) && (dblPitch >= 0)) rv = 3;		//-,+   90' sum
	else if ((dblRoll >= 0) && (dblPitch <= 0)) rv = 4;		//+,-  270' sum

	return rv;
}

void CUnitConversion::GetUnitString(CString &strXUnit, CString &strYUnit)
{
	if (CStatus::Instance()->GetCoordinateSystem() == CStatus::CS_CARTESIAN)
	{
		if (CStatus::Instance()->GetInclinationType() == CStatus::IT_ANGLE)
		{
			if (CStatus::Instance()->GetAngleType() == CStatus::AT_DEGREE)
			{
				strXUnit = _T("¡Æ");
				strYUnit = _T("¡Æ");
			}
			else
			{
				strXUnit = _T("milRad");
				strYUnit = _T("milRad");
			}
		}
		else
		{
			if (CStatus::Instance()->GetRORType() == CStatus::RT_MMD)
			{
				strXUnit = _T("mm/d");
				strYUnit = _T("mm/d");
			}
			else
			{
				strXUnit = _T("mm/r");
				strYUnit = _T("mm/r");
			}
		}
	}else
	{	//! Polar
		if (CStatus::Instance()->GetInclinationType() == CStatus::IT_ANGLE)
		{
			if (CStatus::Instance()->GetAngleType() == CStatus::AT_DEGREE)
			{
				strXUnit = _T("¡Æ");
				strYUnit = _T("¡Æ");
			}
			else
			{
				strXUnit = _T("milRad");
				strYUnit = _T("¡Æ");
			}
		}
		else
		{
			if (CStatus::Instance()->GetRORType() == CStatus::RT_MMD)
			{
				strXUnit = _T("mm/d");
				strYUnit = _T("¡Æ");
			}
			else
			{
				strXUnit = _T("mm/r");
				strYUnit = _T("¡Æ");
			}
		}
	}
}

CUnitConversion * CUnitConversion::Instance()
{
	if (!m_pUnitConversion) m_pUnitConversion = new CUnitConversion;
	return m_pUnitConversion;
}


void CUnitConversion::GetStateValue(double &dblXValue, double &dblYValue)
{
	dblXValue = m_dblRoll;
	dblYValue = m_dblPitch;
	m_iWaferSize =	CStatus::Instance()->GetWaferSize();

	if (CStatus::Instance()->GetCoordinateSystem() == CStatus::CS_CARTESIAN)
	{
		if (CStatus::Instance()->GetInclinationType() == CStatus::IT_ANGLE)
		{
			if (CStatus::Instance()->GetAngleType() == CStatus::AT_DEGREE)
			{
				dblXValue = m_dblRoll;
				dblYValue = m_dblPitch;
			}
			else
			{
				dblXValue = (1000*m_dblRoll*M_PI) / 180.;
				dblYValue = (1000*m_dblPitch*M_PI) / 180.;
			}
		}
		else
		{
			if (CStatus::Instance()->GetRORType() == CStatus::RT_MMD)
			{
				dblXValue = m_iWaferSize*sin((m_dblRoll*M_PI) / 180.);
				dblYValue = m_iWaferSize*sin((m_dblPitch*M_PI) / 180.);
			}
			else
			{
				dblXValue = (m_iWaferSize/2)*sin((m_dblRoll*M_PI) / 180.);
				dblYValue = (m_iWaferSize/2)*sin((m_dblPitch*M_PI) / 180.);

			}
		}
	}
	else
	{  //! Polar
		if (CStatus::Instance()->GetInclinationType() == CStatus::IT_ANGLE)
		{
			if (CStatus::Instance()->GetAngleType() == CStatus::AT_DEGREE)
			{
				dblXValue = CalcMagnitude();
				dblYValue = CalcDirection();
			}
			else
			{
				dblXValue = (1000*CalcMagnitude()*M_PI)/180.;
				dblYValue = CalcDirection();
			}
		}
		else
		{
			if (CStatus::Instance()->GetRORType() == CStatus::RT_MMD)
			{
				dblXValue = m_iWaferSize * sin( (CalcMagnitude()*M_PI)/180.);
				dblYValue = CalcDirection();

			}
			else
			{
				dblXValue = (m_iWaferSize/2) * sin((CalcMagnitude()*M_PI) / 180.);
				dblYValue = CalcDirection();
			}
		}
	}

}


void CUnitConversion::ConvertToDest(CStatus::tCoordinateSystem iCrdiSystem, CStatus::tAngleType iAngleType, CStatus::tIncType iIncType, CStatus::tRORType iRORType, CString *pStrUnit, double *pDestValue, double orgDegree)
{
	int iWaferSize = CStatus::Instance()->GetWaferSize();

	if (iCrdiSystem == CStatus::CS_CARTESIAN)
	{
		if (iIncType == CStatus::IT_ANGLE)
		{
			if (iAngleType == CStatus::AT_DEGREE)
			{
				*pDestValue = orgDegree;
				*pStrUnit = _T("¡Æ");
			}
			else
			{
				*pDestValue = (1000 * orgDegree*M_PI) / 180.;
				*pStrUnit = _T("milRad");
			}
		}
		else
		{
			if (iRORType == CStatus::RT_MMD)
			{
				*pDestValue = iWaferSize*sin((orgDegree*M_PI) / 180.);
				*pStrUnit = _T("mm/d");
			}
			else
			{
				*pDestValue = (iWaferSize / 2)*sin((orgDegree*M_PI) / 180.);
				*pStrUnit = _T("mm/r");
			}
		}
	}
	else
	{  //! Polar
		if (iIncType == CStatus::IT_ANGLE)
		{
			if (iAngleType == CStatus::AT_DEGREE)
			{
				*pDestValue = orgDegree;
				*pStrUnit = _T("¡Æ");
			}
			else
			{
				*pDestValue = (1000 * orgDegree*M_PI) / 180.;
				*pStrUnit = _T("milRad");
			}
		}
		else
		{
			if (iRORType == CStatus::RT_MMD)
			{
				*pDestValue = iWaferSize * sin((orgDegree*M_PI) / 180.);
				*pStrUnit = _T("mm/d");

			}
			else
			{
				*pDestValue = (iWaferSize / 2) * sin((orgDegree*M_PI) / 180.);
				*pStrUnit = _T("mm/r");
			}
		}
	}
}

void CUnitConversion::ConvertToDegree(CStatus::tCoordinateSystem iCrdiSystem, CStatus::tAngleType iAngleType, CStatus::tIncType iIncType, CStatus::tRORType iRORType, double *pDestDegree, double orgValue)
{
	int iWaferSize = CStatus::Instance()->GetWaferSize();

	if (iCrdiSystem == CStatus::CS_CARTESIAN)
	{
		if (iIncType == CStatus::IT_ANGLE)
		{
			if (iAngleType == CStatus::AT_DEGREE)
			{
				*pDestDegree = orgValue;
			}
			else
			{
				*pDestDegree = (180 * orgValue) / (M_PI*1000);
			}
		}
		else
		{
			if (iRORType == CStatus::RT_MMD)
			{
				*pDestDegree = (180 / M_PI)*asin(orgValue / iWaferSize);
			}
			else
			{
				*pDestDegree = (180 / M_PI)*asin(orgValue / (iWaferSize*2));
			}
		}
	}
	else
	{  //! Polar
		if (iIncType == CStatus::IT_ANGLE)
		{
			if (iAngleType == CStatus::AT_DEGREE)
			{
				*pDestDegree = orgValue;
			}
			else
			{
				*pDestDegree = (orgValue * 180) / (1000 * M_PI);
			}
		}
		else
		{
			if (iRORType == CStatus::RT_MMD)
			{
				*pDestDegree = (180./M_PI) * asin(orgValue / iWaferSize);
			}
			else
			{
				*pDestDegree = (180. / M_PI) * asin(orgValue / (iWaferSize/2));
			}
		}
	}
}
