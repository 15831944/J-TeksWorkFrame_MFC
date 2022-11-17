#pragma once

/////////////////////////////////////////////////////////////////////////////
// CCfgFile window
#include "stdafx.h"
#include "UniUtilBase.h"
#include "XmlBrowser.h"
#include <stdarg.h>

//class CCfgFile 
class DLLDECL CCfgFile
{
// Construction
public:
	enum tMethod{METHOD_XML, METHOD_INI, METHOD_REGISTRY};
	CCfgFile(BOOL fSyncSave = FALSE);
	~CCfgFile();

// Implementation
public:
	CXmlBrowser* GetXmlBrowser(void);
	int Open(void);
	BOOL Flush();
	static CCfgFile* Instance(void);
	BOOL Load(CString fileName);
	void SetDepth(int nDepth);
	void SetPath(CString strPath);
	void SetCfgMethod(int nMethod=METHOD_XML);
	void SetSyncMode(BOOL fSyncSave = TRUE);
	long Get(long lDefValue, TCHAR* szTag, ...);
	CString Get(TCHAR *szDefValue, TCHAR *szTag, ...);
	double Get(double dblDefValue, TCHAR* szTag, ...);
	CString VsGetString(TCHAR* szDefValue, TCHAR* szTag, va_list argsList);
	long Set(long lValue, TCHAR* szTag, ...);
	long Set(double lValue, TCHAR* szTag, ...);
	long VsSetString(TCHAR *szValue, TCHAR *szTag, va_list argLists);
	long Set(TCHAR *szValue, TCHAR *szTag, ...);
	BOOL DeleteSetting(TCHAR* szTag, ...);

protected:
	static CCfgFile* m_pInstance;
	CString m_fileName;
	BOOL m_fSyncSave;
	CString m_strPath;
	int m_nMethod;
	int m_nDepth;
	CXmlBrowser m_xmlBrowser;
	CXmlNode* GetNode(CString keyString, BOOL fAddNodes=FALSE);
public:
	BOOL EraseAll(void);
	CString GetPath(void);
};

