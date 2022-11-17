#ifndef __FILESINFO_H
#define __FILESINFO_H

#include "UniUtilBase.h"
#include "UniUtility.h"

class DLLDECL CFileInfo:public CUniUtilBase
{
public:
	static BOOL IsReadonly(LPCTSTR path);
	static BOOL FileCheck(LPCTSTR path, BOOL fOkAtBlank = FALSE );
	static BOOL RemoveReadonly(LPCTSTR path);
	static BOOL CreateFolder( LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes=NULL ); 
	static void DeleteFolder(LPCTSTR lpPathName, BOOL fConfirm);
	static BOOL SHDeleteFolder(LPCTSTR lpPathName, BOOL fConfirm = FALSE);
	static BOOL FolderCheck(LPCTSTR path, BOOL fOkAtBlank );
	static BOOL ExtractPathName(TCHAR *PathName, TCHAR * FullName);
	static CString ExtractFileName(CString strPathName);
	static int GetFileTimeInfo(CString strFileName, SYSTEMTIME *pTimeCreate, SYSTEMTIME *pTimeAccess, SYSTEMTIME *pTimeWrite);
	static int GetElapseTimeAfterCreate(CString strFileName, int *piElapseTimeDay);
};

#define PC_SUCCESS	1
#define PC_FAILED	0
#define PC_CANCELED	-1

#endif
