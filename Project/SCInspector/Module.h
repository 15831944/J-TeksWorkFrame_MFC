#pragma once
#include "partbase.h"
#include "ModelBase.h"
class CModule :
	public CPartBase
{
public:
	DECLARE_DYNAMIC(CModule)
	CModule(void);
	~CModule(void);

	//int SaveAttr(void);
	//int LoadAttr(void);
	virtual int SaveParam(CXmlNode* pNode);
	virtual int LoadParam(CXmlNode* pNode);
	double m_TESTVALUE;
     int LoadModelData(void);  
public:
     
     static CModelBase * GetModelClass(TCHAR * szModelName);
	
	
	 int SaveAttr(CXmlNode * pNode);
};
