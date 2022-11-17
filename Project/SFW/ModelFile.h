// ModelFile.h: interface for the CModelFile class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "stdafx.h"
#include <UniUtility.h>


class CModelData;

class CModelFile : public CXmlBrowser  
{
	CModelData* m_pModelData;

public:
	CModelFile();
	virtual ~CModelFile();

	BOOL Create(CModelData* pModelData);
	BOOL Open(CModelData* pModelData);
	void Save();
	BOOL IsSavable();
	BOOL RemoveReadonly();

	CModelData* GetModelData() { return m_pModelData; }
};
