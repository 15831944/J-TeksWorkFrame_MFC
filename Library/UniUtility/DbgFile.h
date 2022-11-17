#pragma once
#include "UniUtilBase.h"

class DLLDECL CDbgFile
{
public:
	CDbgFile(void);
	~CDbgFile(void);
	static CDbgFile *m_pInstance;
	static CDbgFile * Instance(void);
	void SetDbgFileName(CString strName);
	void Write(TCHAR * pFileName, ...);
	CString strFileName;
	long m_lIndex;
};
