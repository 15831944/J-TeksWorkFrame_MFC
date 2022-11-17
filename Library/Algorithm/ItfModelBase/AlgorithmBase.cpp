#include "StdAfx.h"
#include "AlgorithmBase.h"
#include "InspResult.h"

IMPLEMENT_DYNAMIC(CAlgorithmBase, CModelBase)
CAlgorithmBase::CAlgorithmBase(void)
: m_pInputImg(NULL)
, m_pParamsInterface(NULL)
, m_bIsInspDefect(false)
, m_pDC(NULL)
{
}

CAlgorithmBase::~CAlgorithmBase(void)
{
}

int CAlgorithmBase::CalcFeature(void)
{
	return 0;
}

int CAlgorithmBase::Free(void)
{
	return 0;
}


int CAlgorithmBase::DrawFeature(int drawOption)
{
	return 0;
}

int CAlgorithmBase::Inspect(void)
{
	return 0;
}

int CAlgorithmBase::Inspect(CRect inspRect, CInspResult * pResult)
{
	return 0;
}

int CAlgorithmBase::Inspect(int SelectedCompNo, CInspResult *pResult)
{
	return 0;
}

void CAlgorithmBase::SetInput(CMilBuffer * pMilImage)
{
	m_pInputImg=pMilImage;
}

int CAlgorithmBase::SetOutput(CMilBuffer * pMilImage)
{
	return 0;
}

int CAlgorithmBase::SetParams(CPtrList* pListParams)
{
	return 0;
}

int CAlgorithmBase::Update(CPtrList *pList)
{
//	Inspect();
	return 0;
}

int CAlgorithmBase::GetResultStr(CString &strResult)
{
	return 0;
}

SHORT CAlgorithmBase::GetDefectType(void)
{
	return 0;
}

int CAlgorithmBase::GetResults(CInspResult* pInspResult)
{
	pInspResult->SetType(this);
	return 0;
}

int CAlgorithmBase::ReleaseResults(CInspResult * pInspResult)
{
	return 0;
}

void CAlgorithmBase::SetOverlayDC(HDC* pDC)
{
	m_pDC=pDC;
}

CString CAlgorithmBase::GetAlgoName(void)
{
	USES_CONVERSION;
	TCHAR* szTemp =A2W(this->GetRuntimeClass()->m_lpszClassName);
	CString strToRet = szTemp;
	
	return strToRet;
}


int CAlgorithmBase::Inspect(CPtrList * pLstAlgo)
{
	return 0;
}
