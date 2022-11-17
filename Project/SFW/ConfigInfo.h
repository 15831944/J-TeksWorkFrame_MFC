#pragma once
#include <UniUtility.h>

class CConfigInfo
{
public:
	CConfigInfo(void);
	~CConfigInfo(void);
	static CConfigInfo *m_pInstance;
	int Init(void);
	static CConfigInfo* Instance(void);
	CString GetModelRepository(void);
	CString GetSetupPath(void);

	CString m_strSetupPath;
	CString m_strModelPath;
	CString m_LangDIR;
	CStringList m_lstAlgoNames;
	double m_PulseForUm;
	int LoadInfo(void);
	int SaveInfo(void);
};
