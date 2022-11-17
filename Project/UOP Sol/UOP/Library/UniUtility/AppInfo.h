#pragma once
#include "UniUtilBase.h"

class DLLDECL CAppInfo :CUniUtilBase
{
public:
	CAppInfo(void);
	~CAppInfo(void);
	static CString GetAppDir(void);
	static int GetTest(void);
};
