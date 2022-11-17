#include "StdAfx.h"
#include "AOIWin.h"

IMPLEMENT_DYNAMIC(CAOIWin, CPartBase)
CAOIWin::CAOIWin(void)
{
	m_rectRef = _T("");

	m_rectTop = 0.0;
	m_rectBottom = 0.0;
	m_rectLeft = 0.0;
	m_rectRight = 0.0;

	m_rectPTop = 0.0;
	m_rectPBottom = 0.0;
	m_rectPLeft = 0.0;
	m_rectPRight = 0.0;

	m_BlobArea = 0;
	m_BlobAreaLimit = 0;
	m_BlobOfsX = 10;
	m_BlobOfsY = 10;
	m_ARLimit = 10;

	m_InspDir=0;
	m_typeChip=0;
	m_grayIncline=1;
	m_inspAOI=5;
	m_thresRatio=5;
	m_inspMA=50;
	m_inspMCG=65;
}

CAOIWin::~CAOIWin(void)
{
}

//!Save property of component with XML attribute function.
int CAOIWin::SaveAttr(void)
{
	return 0;
}

//!Load property of component with XML attribute function.
int CAOIWin::LoadAttr(void)
{
	return 0;
}

CPoint CAOIWin::CenterPoint(void)
{
	return CPoint((int)((m_rectRight+m_rectLeft)/2), (int)((m_rectBottom+m_rectTop)/2));
}

CPoint CAOIWin::CenterPPoint(void)
{
	return CPoint((int)((m_rectPRight+m_rectPLeft)/2), (int)((m_rectPBottom+m_rectPTop)/2));
}

int CAOIWin::RectPWidth(void)
{
	return (int)abs(m_rectPLeft-m_rectPRight);
}

int CAOIWin::RectPHeight(void)
{
	return (int)abs(m_rectPBottom-m_rectPTop);
}

CPoint CAOIWin::TopLeft(void)
{
	return CPoint((int)m_rectLeft, (int)m_rectTop);
}

CPoint CAOIWin::PTopLeft(void)
{
	return CPoint((int)m_rectPLeft, (int)m_rectPTop);
}

CPoint CAOIWin::BottomRight(void)
{
	return CPoint((int)m_rectRight, (int)m_rectBottom);
}

CPoint CAOIWin::PBottomRight(void)
{
	return CPoint((int)m_rectPRight, (int)m_rectPBottom);
}

int CAOIWin::SaveParam(CXmlNode* pNode)//, BOOL bUseCurNode)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	CXmlNode *pChildNode = pNode->AddNode(szTemp);
 
	pChildNode->SetString(_T("REF_ID"), m_rectRef);

	pChildNode->SetDouble(_T("Left"), m_rectLeft);
	pChildNode->SetDouble(_T("Right"), m_rectRight);
	pChildNode->SetDouble(_T("Top"), m_rectTop);
	pChildNode->SetDouble(_T("Bottom"),m_rectBottom);

	pChildNode->SetDouble(_T("PLeft"), m_rectPLeft);
	pChildNode->SetDouble(_T("PRight"), m_rectPRight);
	pChildNode->SetDouble(_T("PTop"), m_rectPTop);
	pChildNode->SetDouble(_T("PBottom"), m_rectPBottom);

	pChildNode->SetLong(_T("Area"), m_BlobArea);
	pChildNode->SetDouble(_T("AreaLimit"), m_BlobAreaLimit);
	pChildNode->SetLong(_T("ofsX"), m_BlobOfsX);
	pChildNode->SetLong(_T("OfsY"), m_BlobOfsY);
	pChildNode->SetDouble(_T("ARLimit"), m_ARLimit); ///Aspect Ratio Limit

	CPartBase::SaveParam(pChildNode); //원래 있던 소스

	pChildNode->EndNode();
	return 0;
}

int CAOIWin::LoadParam(CXmlNode* pNode)//, BOOL bUseCurNode)
{
	m_rectRef = pNode->ReadString(0, _T("REF_ID"));

	m_rectLeft = pNode->ReadDouble(0.0, _T("Left"));
	m_rectRight = pNode->ReadDouble(0.0, _T("Right"));
	m_rectTop = pNode->ReadDouble(0.0, _T("Top"));
	m_rectBottom = pNode->ReadDouble(0.0, _T("Bottom"));

	m_rectPLeft = pNode->ReadDouble(0.0, _T("PLeft"));
	m_rectPRight = pNode->ReadDouble(0.0, _T("PRight"));
	m_rectPTop = pNode->ReadDouble(0.0, _T("PTop"));
	m_rectPBottom = pNode->ReadDouble(0.0, _T("PBottom"));

	m_BlobArea = pNode->ReadLong(0, _T("Area"));
	m_BlobAreaLimit = (int)(pNode->ReadDouble(0.0, _T("AreaLimit")));
	m_BlobOfsX = pNode->ReadLong(0, _T("ofsX"));
	m_BlobOfsY = pNode->ReadLong(0, _T("OfsY"));
	m_ARLimit = (int)(pNode->ReadDouble(0.0, _T("ARLimit")));

	CPartBase::LoadParam(pNode); //원래 있던 소스
	return 0;
} 
int CAOIWin::Free(void)
{
	return 0;
}
