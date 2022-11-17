#pragma once
#include "ITFBase.h"
class CAlgorithmBase;

class ITFBASEDLLDECL CInspResult
{
public:
	CInspResult(void);
	~CInspResult(void);

	void ReleaseResults(void);
	void SetType(CAlgorithmBase *pType);
	CString GetType(void);
	int GetCount(void);
	void SetCount(int nCount);
	void * GetResults(void);
	CAlgorithmBase * GetAlgorithm(void);
	CString m_strInspType;
	CAlgorithmBase *m_pAlgorithm;
	void * m_pResult;
	int m_iCount;
	void Free(void);
	static CListCtrl *m_pListResult;
};
