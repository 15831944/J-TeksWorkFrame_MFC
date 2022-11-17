#pragma once
#include "UniUtilBase.h" 

struct tAxisPos
{
	int x, y, z, t, r;
};

class DLLDECL CAxisPos : public tAxisPos
{
public:
	CAxisPos(void);
	CAxisPos(long vx, long vy, long vz, long vt, long vr);
	~CAxisPos(void);
	void Offset(CAxisPos *pPos);
	CAxisPos operator+(CAxisPos &Pos);
	CAxisPos operator-(CAxisPos &Pos);
	void operator=(CAxisPos &Pos);
};

