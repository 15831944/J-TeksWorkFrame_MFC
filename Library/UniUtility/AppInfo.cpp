#include "StdAfx.h"
#include "AppInfo.h"
#include <iostream>
#include <string>

CAppInfo::CAppInfo(void)
{
}

CAppInfo::~CAppInfo(void)
{
}

CString CAppInfo::GetAppDir(void)
{
	CString strTemp;
	
	_TCHAR currDir[MAX_PATH];
	GetModuleFileName(NULL,currDir,MAX_PATH);
	strTemp.Format(L"%s..", currDir);
	int nindex=strTemp.ReverseFind('\\');
	strTemp=strTemp.Left(nindex);
	return strTemp;
}

int CAppInfo::GetTest(void)
{
	return 0;
}
