#include "StdAfx.h"
#include "DblAtlTypes.h"

CdblSize::CdblSize(void)
{
}

CdblSize::~CdblSize(void)
{
}

CdblSize::CdblSize(double CX, double CY)
{
	dblCX = CX;
	dblCY = CY;
}

CdblSize::CdblSize(tdblSIZE& Size)
{
	dblCX = Size.dblCX;
	dblCY = Size.dblCY;
}

CdblSize::CdblSize(SIZE size)
{
	dblCX = size.cx;
	dblCY = size.cy;
}

void CdblSize::operator+=(tdblSIZE& Size)
{
	dblCX += Size.dblCX;
	dblCY += Size.dblCY;
}

void CdblSize::operator-=(tdblSIZE& Size)
{
	dblCX -= Size.dblCX;
	dblCY -= Size.dblCY;
}

void CdblSize::operator=(tdblSIZE& Size)
{
	dblCX = Size.dblCX;
	dblCY = Size.dblCY;
}

CdblSize CdblSize::operator+(tdblSIZE& Size)
{
	return CdblSize(dblCX+Size.dblCX, dblCY+Size.dblCY);
}

CdblSize CdblSize::operator-(tdblSIZE& Size)
{
	return CdblSize(dblCX-Size.dblCX, dblCY-Size.dblCY);
}

CdblSize CdblSize::operator-(void)
{
	return CdblSize(-dblCX, -dblCY);
}

CdblPoint CdblSize::operator+(tdblPOINT& dblPoint)
{
	return CdblPoint(dblCX+dblPoint.dblPosX, dblCY+dblPoint.dblPosY);
}

CdblPoint CdblSize::operator-(tdblPOINT& dblPoint)
{
	return CdblPoint(dblCX-dblPoint.dblPosX, dblCY-dblPoint.dblPosY);
}
