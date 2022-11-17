// ModelFile.h: interface for the CModelFile class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "stdafx.h"


class CModelData;

class CModelFile : public CXmlBrowser  
{
	CModelData* m_pModelData;

public:
	CModelFile();
	virtual ~CModelFile();

	BOOL Create(CModelData* pModelData);
	BOOL Open(CModelData* pModelData, CString strFileName = _T(""));
	void Save(CString strFileNameNoExt = _T(""));
	BOOL IsSavable();
	BOOL RemoveReadonly();

	CModelData* GetModelData() { return m_pModelData; }
};
