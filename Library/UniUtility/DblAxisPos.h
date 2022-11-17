#pragma once
#include "UniUtilBase.h" 

struct tDblAxisPos
{
	double x, y, z, t, r;
};

class DLLDECL CdblAxisPos : public tDblAxisPos
{
public:
	CdblAxisPos(void);
	CdblAxisPos(double vx, double vy, double vz, double vt, double vr);
	~CdblAxisPos(void);
	void Offset(CdblAxisPos *pPos);
	CdblAxisPos operator+(CdblAxisPos &Pos);
	CdblAxisPos operator-(CdblAxisPos &Pos);
	void operator=(CdblAxisPos &Pos);

};

