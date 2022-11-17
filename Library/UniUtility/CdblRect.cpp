#include "StdAfx.h"
#include "DblAtlTypes.h"

CdblRect::CdblRect(void)
{
	dblLeft = 0.0;
	dblRight = 0.0;
	dblTop = 0.0;
	dblBottom = 0.0;
}

CdblRect::~CdblRect(void)
{
}

CdblRect::CdblRect(double Left, double Top, double Right, double Bottom)
{
	dblLeft = Left;
	dblRight = Right;
	dblTop = Top;
	dblBottom = Bottom;
}

CdblRect::CdblRect(tdblRECT& dblRect)
{
	dblLeft = dblRect.dblLeft;
	dblRight = dblRect.dblRight;
	dblTop = dblRect.dblTop;
	dblBottom = dblRect.dblBottom;

}

CdblRect::CdblRect(tdblPOINT dblPoint, tdblSIZE dblSize)
{
	dblLeft = dblPoint.dblPosX;
	dblRight =dblPoint.dblPosX+dblSize.dblCX;
	dblTop = dblPoint.dblPosY;
	dblBottom = dblPoint.dblPosY+dblSize.dblCY;
}

CdblRect::CdblRect(tdblPOINT dblTopLeft, tdblPOINT dblBottomRight)
{
	dblLeft = dblTopLeft.dblPosX;
	dblRight =dblBottomRight.dblPosX;
	dblTop = dblTopLeft.dblPosY;
	dblBottom = dblBottomRight.dblPosY;
}

double CdblRect::Width(void)
{
	return dblRight-dblLeft;
}

double CdblRect::Height(void)
{
	return dblBottom-dblTop;
}

CdblSize CdblRect::Size(void)
{
	return CdblSize(Width(), Height());
}

CdblPoint CdblRect::TopLeft(void)
{
	return CdblPoint(dblLeft, dblTop);
}

CdblPoint CdblRect::BottomRight(void)
{
	return CdblPoint(dblRight, dblBottom);
}

CdblPoint CdblRect::CenterPoint(void)
{
	return CdblPoint( (dblRight+dblLeft)/2, (dblBottom+dblTop)/2);
}

BOOL CdblRect::PtInRect(tdblPOINT& dblPoint)
{
	return ( (dblRight >= dblPoint.dblPosX) && (dblLeft <= dblPoint.dblPosX) && 
			 (dblBottom >= dblPoint.dblPosY) && (dblTop <= dblPoint.dblPosY) );
}

void CdblRect::OffsetRect(double dblX, double dblY)
{
	dblLeft+=dblX;
	dblRight+=dblX;
	dblTop+=dblY;
	dblBottom+=dblY;
}

void CdblRect::OffsetRect(tdblSIZE dblSize)
{
	dblLeft+=dblSize.dblCX;
	dblRight+=dblSize.dblCX;
	dblTop+=dblSize.dblCY;
	dblBottom+=dblSize.dblCY;
}

void CdblRect::OffsetRect(tdblPOINT dblPoint)
{
	dblLeft+=dblPoint.dblPosX;
	dblRight+=dblPoint.dblPosX;
	dblTop+=dblPoint.dblPosY;
	dblBottom+=dblPoint.dblPosY;
}


void CdblRect::operator=(CdblRect& dblRect)
{
	dblLeft	=dblRect.dblLeft;
	dblRight	=dblRect.dblRight;
	dblTop	=dblRect.dblTop;
	dblBottom	=dblRect.dblBottom;
}

BOOL CdblRect::operator==(tdblRECT& dblRect)
{
	return	(dblLeft==dblRect.dblLeft)&&(dblRight==dblRect.dblRight)&&
			(dblTop==dblRect.dblTop)&&(dblBottom==dblRect.dblBottom);
}

BOOL CdblRect::operator!=(tdblRECT& dblRect)
{
	return !(operator==(dblRect));
}

void CdblRect::operator+=(tdblPOINT& dblPoint)
{
	OffsetRect(dblPoint);
}

void CdblRect::operator-=(tdblPOINT& dblPoint)
{
	OffsetRect(-dblPoint.dblPosX, -dblPoint.dblPosY);
}

void CdblRect::operator&=(tdblRECT& dblRect)
{
	dblLeft = max(dblLeft, dblRect.dblLeft);
	dblRight = min(dblRight, dblRect.dblRight);
	dblTop = max(dblTop, dblRect.dblTop);
	dblBottom = min(dblBottom, dblRect.dblBottom);
}

void CdblRect::operator|=(tdblRECT& dblRect)
{
	dblLeft = min(dblLeft, dblRect.dblLeft);
	dblRight = max(dblRight, dblRect.dblRight);
	dblTop = min(dblTop, dblRect.dblTop);
	dblBottom = max(dblBottom, dblRect.dblBottom);
}

CdblRect CdblRect::operator+(tdblPOINT& dblPoint)
{
	return CdblRect(dblLeft+dblPoint.dblPosX, dblTop+dblPoint.dblPosY, 
					dblRight+dblPoint.dblPosX, dblBottom+dblPoint.dblPosY);
}

CdblRect CdblRect::operator-(tdblPOINT& dblPoint)
{
	return CdblRect(dblLeft-dblPoint.dblPosX, dblTop-dblPoint.dblPosY, 
					dblRight-dblPoint.dblPosX, dblBottom-dblPoint.dblPosY);
}



void CdblRect::Set(double Left, double Top, double Right, double Bottom)
{
	dblLeft = Left;
	dblTop = Top;
	dblRight = Right;
	dblBottom = Bottom;
}
