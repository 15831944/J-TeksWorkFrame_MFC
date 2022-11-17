#pragma once
#include "stdafx.h"
#include "ModelBase.h"

class ITFBASEDLLDECL CPartBase :
	public CModelBase
{
DECLARE_DYNAMIC(CPartBase)
public:
	CPartBase(void);
	~CPartBase(void);
	virtual int GetBaseType(void){return typePART;}

};
