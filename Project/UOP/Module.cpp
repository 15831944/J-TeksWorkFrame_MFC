#include "StdAfx.h"
#include "Module.h"
#include "Frame.h"
IMPLEMENT_DYNAMIC(CModule, CPartBase)
CModule::CModule(void)
{
}

CModule::~CModule(void)
{
}


int CModule::SaveParam(CXmlNode* pNode)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	CXmlNode *pChildNode = pNode->AddNode(szTemp);

	SaveAttr(pChildNode);

	CPartBase::SaveParam(pChildNode);
	CPartBase::SaveAttr(pChildNode);

	pChildNode->EndNode();
	return 0;
}


int CModule::LoadParam(CXmlNode* pNode)
{
	CPartBase::LoadParam(pNode);
	return 0;
}




int CModule::SaveAttr(CXmlNode * pNode)
{
	return 0;
}

