#pragma once

#ifdef _ITFBASE_DLL
#define ITFBASEDLLDECL __declspec(dllexport)
#else
#define ITFBASEDLLDECL __declspec(dllimport)

#ifndef _WIN64
#ifndef _DEBUG
#pragma comment(lib, "ItfModelBase32")
#else
#pragma comment(lib, "ItfModelBaseD32")
#endif
#else
#ifndef _DEBUG
#pragma comment(lib, "ItfModelBase64")
#else
#pragma comment(lib, "ItfModelBaseD64")
#endif
#endif
#endif


