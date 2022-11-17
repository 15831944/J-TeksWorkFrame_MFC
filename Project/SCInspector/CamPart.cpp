#include "stdafx.h"
#include "CamPart.h"

IMPLEMENT_DYNAMIC(CCamPart, CPartBase)
CCamPart::CCamPart()
	: m_nCamNo(0)
{
}


CCamPart::~CCamPart()
{
}

int CCamPart::SaveParam(CXmlNode* pNode)//, BOOL bUseCurNode)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	CXmlNode *pChildNode = pNode->AddNode(szTemp);

	pChildNode->SetLong(_T("CamNo"), m_nCamNo);

	CPartBase::SaveParam(pChildNode); //원래 있던 소스

	pChildNode->EndNode();
	return 0;
}

int CCamPart::LoadParam(CXmlNode* pNode)//, BOOL bUseCurNode)
{
	m_nCamNo = pNode->ReadLong(0L, _T("CamNo"));

	CPartBase::LoadParam(pNode); //원래 있던 소스
	return 0;
}