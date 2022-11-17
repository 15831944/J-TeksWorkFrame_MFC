#include "StdAfx.h"
#include "Compo.h"
#include "SCAlgoBase.h"
#include "Status.h"
#include "SCFidAlgo.h"

IMPLEMENT_DYNAMIC(CCompo, CPartBase)
CCompo::CCompo(void)
{
	m_strRef = _T("");

	m_rectTop = 0.0;
	m_rectBottom = 0.0;
	m_rectLeft = 0.0;
	m_rectRight = 0.0;

	m_rectPTop = 0.0;
	m_rectPBottom = 0.0;
	m_rectPLeft = 0.0;
	m_rectPRight = 0.0;
}

CCompo::~CCompo(void)
{
}

//!Save property of component with XML attribute function.
int CCompo::SaveAttr(void)
{
	return 0;
}

//!Load property of component with XML attribute function.
int CCompo::LoadAttr(void)
{
	return 0;
}

CPoint CCompo::CenterPoint(void)
{
	return CPoint((int)((m_rectRight+m_rectLeft)/2), (int)((m_rectBottom+m_rectTop)/2));
}

CPoint CCompo::CenterPPoint(void)
{
	return CPoint((int)((m_rectPRight+m_rectPLeft)/2), (int)((m_rectPBottom+m_rectPTop)/2));
}

int CCompo::RectPWidth(void)
{
	return (int)abs(m_rectPLeft-m_rectPRight);
}

int CCompo::RectPHeight(void)
{
	return (int)abs(m_rectPBottom-m_rectPTop);
}

CPoint CCompo::TopLeft(void)
{
	return CPoint((int)m_rectLeft, (int)m_rectTop);
}

CPoint CCompo::PTopLeft(void)
{
	return CPoint((int)m_rectPLeft, (int)m_rectPTop);
}

CPoint CCompo::BottomRight(void)
{
	return CPoint((int)m_rectRight, (int)m_rectBottom);
}

CPoint CCompo::PBottomRight(void)
{
	return CPoint((int)m_rectPRight, (int)m_rectPBottom);
}

int CCompo::SaveParam(CXmlNode* pNode)//, BOOL bUseCurNode)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	CXmlNode *pChildNode = pNode->AddNode(szTemp);
 
	pChildNode->SetString(_T("REF_ID"), m_strRef);

	pChildNode->SetDouble(_T("Left"), m_rectLeft);
	pChildNode->SetDouble(_T("Right"), m_rectRight);
	pChildNode->SetDouble(_T("Top"), m_rectTop);
	pChildNode->SetDouble(_T("Bottom"),m_rectBottom);

	pChildNode->SetDouble(_T("ImageLeft"), m_pCompoTracker->m_rectBuffer.left);
	pChildNode->SetDouble(_T("ImageRight"), m_pCompoTracker->m_rectBuffer.right);
	pChildNode->SetDouble(_T("ImageTop"), m_pCompoTracker->m_rectBuffer.top);
	pChildNode->SetDouble(_T("ImageBottom"), m_pCompoTracker->m_rectBuffer.bottom);

	pChildNode->SetDouble(_T("BandLeft"), m_pCompoTracker->m_rect.left);
	pChildNode->SetDouble(_T("BandRight"), m_pCompoTracker->m_rect.right);
	pChildNode->SetDouble(_T("BandTop"), m_pCompoTracker->m_rect.top);
	pChildNode->SetDouble(_T("BandBottom"), m_pCompoTracker->m_rect.bottom);

	pChildNode->SetLong(_T("InspType"), (long)(m_pCompoTracker->m_pAlgo->m_nInspType));
	CStatus::Instance()->SetAlgoInfo(m_pCompoTracker->m_pAlgo->m_nInspType, m_pCompoTracker->m_pAlgo, pChildNode);

	CPartBase::SaveParam(pChildNode); //원래 있던 소스

	pChildNode->EndNode();
	return 0;
}

int CCompo::LoadParam(CXmlNode* pNode)//, BOOL bUseCurNode)
{
	m_strRef = pNode->ReadString(_T("None"), _T("REF_ID"));

	m_rectLeft = pNode->ReadDouble(0.0, _T("Left"));
	m_rectRight = pNode->ReadDouble(0.0, _T("Right"));
	m_rectTop = pNode->ReadDouble(0.0, _T("Top"));
	m_rectBottom = pNode->ReadDouble(0.0, _T("Bottom"));

	m_pCompoTracker = new CCompoTracker;
	m_pCompoTracker->m_rectBuffer.left = pNode->ReadDouble(0.0, _T("ImageLeft"));
	m_pCompoTracker->m_rectBuffer.right = pNode->ReadDouble(0.0, _T("ImageRight"));
	m_pCompoTracker->m_rectBuffer.top = pNode->ReadDouble(0.0, _T("ImageTop"));
	m_pCompoTracker->m_rectBuffer.bottom = pNode->ReadDouble(0.0, _T("ImageBottom"));

	m_pCompoTracker->m_rect.left = pNode->ReadDouble(0.0, _T("BandLeft"));
	m_pCompoTracker->m_rect.right = pNode->ReadDouble(0.0, _T("BandRight"));
	m_pCompoTracker->m_rect.top = pNode->ReadDouble(0.0, _T("BandTop"));
	m_pCompoTracker->m_rect.bottom = pNode->ReadDouble(0.0, _T("BandBottom"));
	int nInspType = pNode->ReadLong(0, _T("InspType"));
	m_pCompoTracker->m_pAlgo = CStatus::Instance()->GetAlgoInfo(nInspType, pNode);

	CPartBase::LoadParam(pNode); //원래 있던 소스
	return 0;
} 
int CCompo::Free(void)
{
	if (m_pCompoTracker->m_pAlgo->m_nInspType == CStatus::INSPFID)
	{
		((CSCFidAlgo *)(m_pCompoTracker->m_pAlgo))->m_MilContext.Close();
		((CSCFidAlgo *)(m_pCompoTracker->m_pAlgo))->m_modFinder.Close();
	}
	return 0;
}
