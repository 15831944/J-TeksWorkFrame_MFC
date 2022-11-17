#pragma once
#include "modelbase.h"

class CPartBase :
	public CModelBase
{
DECLARE_DYNAMIC(CPartBase)
public:
	CPartBase(void);
	~CPartBase(void);
	virtual int GetBaseType(void){return typePART;}

};
