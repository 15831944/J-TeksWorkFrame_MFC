#include "StdAfx.h"
#include "Frame.h"

IMPLEMENT_DYNAMIC(CFrame, CPartBase)
CFrame::CFrame(void)
{
	m_dblPosZ=0;
	m_dblTilt=0;
	m_dblRotate=0;
	m_dblDetecterZoom = 0.0;

}

CFrame::~CFrame(void)
{
}

//!Save property of component with XML attribute function.
int CFrame::SaveAttr(CXmlNode* pNode)
{
	pNode->SetDouble(_T("AxisZ"), m_dblPosZ);
	pNode->SetDouble(_T("Tilt"), m_dblTilt);
	pNode->SetDouble(_T("Rotate"), m_dblRotate);
	pNode->SetDouble(_T("DetectorZoom"),m_dblDetecterZoom);
	CPartBase::SaveAttr(pNode);

	return 0;
}

//!Load property of component with XML attribute function.
int CFrame::LoadAttr(void)
{
	return 0;
}

int CFrame::SaveParam(CXmlNode* pNode)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	CXmlNode *pChildNode = pNode->AddNode(szTemp);
    
	SaveAttr(pChildNode);

	CPartBase::SaveParam(pChildNode);

	pChildNode->EndNode();
	return 0;
}

int CFrame::LoadParam(CXmlNode* pNode)
{
	m_dblPosZ = pNode->ReadDouble(0.0, _T("AxisZ"));
	m_dblTilt = pNode->ReadDouble(0.0, _T("Tilt"));
	m_dblRotate = pNode->ReadDouble(0.0, _T("Rotate"));
	m_dblDetecterZoom = pNode->ReadDouble(0.0,_T("DetectorZoom"));
	CPartBase::LoadParam(pNode);

	return 0;
}


int CFrame::Free(void)
{
	return 0;
}
