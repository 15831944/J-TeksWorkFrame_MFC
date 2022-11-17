#pragma once
#include "afxext.h"
#include "atltypes.h"
class CSCAlgoBase;

class CCompoTracker :
	public CRectTracker
{
public:
	CCompoTracker();
	~CCompoTracker();
	CSCAlgoBase *m_pAlgo;
	CRect m_rectBuffer;
};

