#include "StdAfx.h"
#include "DblAtlTypes.h"

// Initialize CdblPoint
CdblPoint::CdblPoint(void)
{
	dblPosX = 0.0;
	dblPosY = 0.0;
}

CdblPoint::CdblPoint(double dblX, double dblY)
{
	SetPoint(dblX, dblY);
}

CdblPoint::CdblPoint(CPoint& point)
{
	SetPoint(point);
}

CdblPoint::CdblPoint(CdblPoint& dblPoint)
{
	SetPoint(dblPoint);
}

CdblPoint::CdblPoint(CSize& size)
{
	SetPoint(size);
}

CdblPoint::CdblPoint(tdblSIZE& dblSize)
{
	SetPoint(dblSize);
}

CdblPoint::CdblPoint(LPARAM dwPoint)
{
	dblPosX = LOWORD(dwPoint);
	dblPosY = LOWORD(dwPoint);
}

CdblPoint::~CdblPoint(void)
{
}

// translate the point
void CdblPoint::SetPoint(double dblX, double dblY)
{
	dblPosX = dblX;
	dblPosY = dblY;
}

void CdblPoint::SetPoint(tagPOINT& point)
{
	dblPosX = point.x;
	dblPosY = point.y;
}

void CdblPoint::SetPoint(tdblPoint& dblPoint)
{
	dblPosX= dblPoint.dblPosX;
	dblPosY= dblPoint.dblPosY;
}

void CdblPoint::SetPoint(CSize& size)
{
	dblPosX= size.cx;
	dblPosY= size.cy;
}

void CdblPoint::SetPoint(tdblSIZE& dblSize)
{
	dblPosX= dblSize.dblCX;
	dblPosY= dblSize.dblCY;
}

// Offset calculate
void CdblPoint::Offset(double dblXOffset, double dblYOffset)
{
	dblPosX +=dblXOffset;
	dblPosY +=dblYOffset;
}

void CdblPoint::Offset(tagPOINT& point)
{
	dblPosX+=point.x;
	dblPosY+=point.y;
}

void CdblPoint::Offset(tdblPOINT& dblPoint)
{
	dblPosX+=dblPoint.dblPosX;
	dblPosY+=dblPoint.dblPosY;
}

void CdblPoint::Offset(tagSIZE& size)
{
	dblPosX=size.cx;
	dblPosY=size.cy;
}

void CdblPoint::Offset(tdblSIZE& dblSize)
{
	dblPosX=dblSize.dblCX;
	dblPosY=dblSize.dblCY;
}

// operator define
void CdblPoint::operator=(tagPOINT& point)
{
	dblPosX= point.x;
	dblPosY= point.y;
}

void CdblPoint::operator=(tdblPOINT& dblPoint)
{
	dblPosX=dblPoint.dblPosX;
	dblPosY=dblPoint.dblPosY;
}

void CdblPoint::operator=(tagSIZE& size)
{
	dblPosX=size.cx;
	dblPosY=size.cy;
}

void CdblPoint::operator=(tdblSIZE& dblSize)
{
	dblPosX=dblSize.dblCX;
	dblPosY=dblSize.dblCY;
}

void CdblPoint::operator+=(tdblSIZE& dblSize)
{
	dblPosX+=dblSize.dblCX;
	dblPosY+=dblSize.dblCY;
}

void CdblPoint::operator-=(tdblSIZE& dblSize)
{
	dblPosX-=dblSize.dblCX;
	dblPosY-=dblSize.dblCY;
}

void CdblPoint::operator+=(tdblPOINT& dblPoint)
{
	dblPosX+=dblPoint.dblPosX;
	dblPosY+=dblPoint.dblPosY;
}

void CdblPoint::operator-=(tdblPOINT& dblPoint)
{
	dblPosX-=dblPoint.dblPosX;
	dblPosY-=dblPoint.dblPosY;
}


// Compare
BOOL CdblPoint::operator==(tdblPOINT& dblPoint)
{
	return (dblPosX==dblPoint.dblPosX) && (dblPosY==dblPoint.dblPosY);
}

BOOL CdblPoint::operator!=(tdblPOINT& dblPoint)
{
	return (dblPosX!=dblPoint.dblPosX) || (dblPosY!=dblPoint.dblPosY);
}


// Operators returning CdblPoint values
CdblPoint CdblPoint::operator+(tdblPoint& dblPoint)
{
	return CdblPoint(dblPosX+dblPoint.dblPosX, dblPosY+dblPoint.dblPosY);
}

CdblPoint CdblPoint::operator-(tdblPOINT& dblPoint)
{
	return CdblPoint(dblPosX-dblPoint.dblPosX, dblPosY-dblPoint.dblPosY);
}

CdblPoint CdblPoint::operator-()
{
	return CdblPoint(-dblPosX, -dblPosY);
}

