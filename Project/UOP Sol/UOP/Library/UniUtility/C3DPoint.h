#pragma once
#include "UniUtilBase.h"

struct t3DPoint
{
	int x,y,z;
};

class DLLDECL C3DPoint:t3DPoint
{
public:
	C3DPoint(void);
	~C3DPoint(void);
};
