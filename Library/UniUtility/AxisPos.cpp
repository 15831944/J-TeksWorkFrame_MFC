#include "StdAfx.h"
#include "AxisPos.h"

CAxisPos::CAxisPos(void)
{
}

CAxisPos::CAxisPos(long vx, long vy, long vz, long vt, long vr)
{
	x=vx;
	y=vy;
	z=vz;
	t=vt;
	r=vr;
}

CAxisPos::~CAxisPos(void)
{
}

void CAxisPos::Offset(CAxisPos *pPos)
{
	x=x+pPos->x;
	y=y+pPos->y;
	z=z+pPos->z;
	r=r+pPos->r;
	t=t+pPos->t;
}


CAxisPos CAxisPos::operator+(CAxisPos &Pos)
{
	return CAxisPos(x+Pos.x, y+Pos.y, z+Pos.z, t+Pos.t, r+Pos.r);
}

CAxisPos CAxisPos::operator-(CAxisPos &Pos)
{
	return CAxisPos(x-Pos.x, y-Pos.y, z-Pos.z, t-Pos.t, r-Pos.r);
}

void CAxisPos::operator=(CAxisPos &Pos)
{
	x = Pos.x;
	y = Pos.y;
	z = Pos.z;
	t = Pos.t;
	r = Pos.r;
}
