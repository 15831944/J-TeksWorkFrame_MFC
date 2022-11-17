#include "StdAfx.h"
#include "InspResult.h"
#include "AlgorithmBase.h"

CListCtrl * CInspResult::m_pListResult = NULL;
CInspResult::CInspResult(void)
: m_iCount(0)
{
	m_pAlgorithm=NULL;
}

CInspResult::~CInspResult(void)
{
	ReleaseResults();
}

void CInspResult::ReleaseResults(void)
{
	if(!m_pAlgorithm)return;
	m_pAlgorithm->ReleaseResults(this);
	m_pAlgorithm=NULL;
}

void CInspResult::SetType(CAlgorithmBase *pAlgorithm)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(pAlgorithm->GetRuntimeClass()->m_lpszClassName);
	m_strInspType=szTemp;
	m_pAlgorithm = pAlgorithm;
}

CString CInspResult::GetType(void)
{
	return m_strInspType;
}

/*! @fn CAlgorithmBase * CInspResult::GetAlgorithm(void)
	@brief 결과를 받아온 알고리즘 
	@n 내부에서 사용되어야 하는것으로 외부에서는 사용하면 안됨
	@n 알고리즘에서 결과 설정을 하고나서는 언제 해지될지 모르는 상황이기 때문임
*/
CAlgorithmBase * CInspResult::GetAlgorithm(void)
{
	return m_pAlgorithm;
}

int CInspResult::GetCount(void)
{
	return m_iCount;
}

void CInspResult::SetCount(int nCount)
{
	m_iCount=nCount;
}

void * CInspResult::GetResults(void)
{
	return m_pResult;
}

void CInspResult::Free(void)
{
	ReleaseResults();
}
