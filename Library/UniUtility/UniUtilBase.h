#pragma once

#ifdef _UNIUTILITY_DLL
#define DLLDECL __declspec(dllexport)
#else
#define DLLDECL __declspec(dllimport)

#ifndef _WIN64
#ifndef _DEBUG
#pragma comment(lib, "UniUtility32")
#else
#pragma comment(lib, "UniUtilityD32")
#endif
#else
#ifndef _DEBUG
#pragma comment(lib, "UniUtility64")
#else
#pragma comment(lib, "UniUtilityD64")
#endif
#endif
#endif

class DLLDECL CUniUtilBase
{
public:
	CUniUtilBase(void);
	~CUniUtilBase(void);
};
