#include "stdafx.h"
#include "resource.h"
#include "FileInfo.h"

#include <windows.h>

BOOL CFileInfo::IsReadonly(LPCTSTR path)
{
	ASSERT(wcscmp(path,_T("")) != 0);

	DWORD result;
	if (((result=GetFileAttributes(path)) != -1 ) && (result&FILE_ATTRIBUTE_READONLY) ) {
		return TRUE;
	}

	return FALSE;
}


BOOL CFileInfo::FileCheck(LPCTSTR path, BOOL fOkAtBlank )
{
	if ( fOkAtBlank && wcscmp(path,_T("")) == 0 ) {
		return TRUE;
	}

	DWORD result;
	if (((result=GetFileAttributes(path)) == -1 ) || (result&FILE_ATTRIBUTE_DIRECTORY) ) {
		return FALSE;
	}

	return TRUE;
}

BOOL CFileInfo::RemoveReadonly(LPCTSTR path)
{
	DWORD result;
	if (((result=GetFileAttributes(path)) != -1 ) && (result&FILE_ATTRIBUTE_READONLY) ) {
		result &= ~FILE_ATTRIBUTE_READONLY;
		return SetFileAttributes(path, result);
	}
	return TRUE;
}

BOOL CFileInfo::CreateFolder( LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes ) 
{
	CString temp = lpPathName;
	temp.Replace(_T("\\"), _T("/"));
	CString parentFolder = temp.Left(temp.ReverseFind('/'));
	if (parentFolder == _T(""))
		return FALSE;

	DWORD dwResult;

	BOOL fResult = TRUE;
	if ( (dwResult = GetFileAttributes(parentFolder)) == -1 ) {
		fResult = CFileInfo::CreateFolder(parentFolder , lpSecurityAttributes);
	}

	if ( fResult && ( dwResult & FILE_ATTRIBUTE_DIRECTORY ) ) {
		if ( (dwResult = GetFileAttributes(lpPathName)) == -1 ) {
			return ::CreateDirectory ( lpPathName , lpSecurityAttributes ); 
		} 
	} 
	
	// Got Some Error
	return FALSE;
}

void CFileInfo::DeleteFolder(LPCTSTR lpPathName, BOOL fConfirm)
{
	CFileFind finder;
	CString strModelname;

	CString strFullPath = lpPathName;
	strFullPath.TrimRight(_T("\\/ "));

	try {
		CFileInfo::FolderCheck(strFullPath, 0);
	} catch ( UINT ) {
		TRACE(_T("The path is not directory or does not exist."));
		return;
	}

	CString msg;
	msg.Format(_T("Do you want to delete the directory [ %s ]"), lpPathName);
	if (fConfirm && AfxMessageBox(msg, MB_YESNO) == IDCANCEL)
		return;

	CString searchPath;
	searchPath.Format(_T("%s/*.*"), lpPathName);
	BOOL bWorking = finder.FindFile(searchPath);
	while (bWorking){
		bWorking = finder.FindNextFile();
		strModelname = finder.GetFileName();
		strFullPath.Format(_T("%s/%s"), lpPathName, strModelname);
		
		if(strModelname.Right(1) != ".") {
			if (finder.IsDirectory()) {
				DeleteFolder(strFullPath, fConfirm);
			} else {
				SetFileAttributes( strFullPath, FILE_ATTRIBUTE_NORMAL );
				DeleteFile( strFullPath );
			}
		}
	}
	finder.Close();
	SetFileAttributes( lpPathName, FILE_ATTRIBUTE_NORMAL );
	if (!::RemoveDirectory(lpPathName)) {
		DWORD result = GetLastError();
	}
}

BOOL CFileInfo::FolderCheck(LPCTSTR path, BOOL fOkAtBlank )
{
	if ( fOkAtBlank && wcscmp(path,_T("")) == 0 ) {
		return TRUE;
	}

	DWORD result;
	if (((result=GetFileAttributes(path)) == -1 ) || !(result&FILE_ATTRIBUTE_DIRECTORY) ) {
		return FALSE;
	}

	return TRUE;
}

BOOL CFileInfo::ExtractPathName(TCHAR *PathName, TCHAR * FullName)
{
	int pos=wcslen(FullName);
	for(;pos!=0;pos--)
	{
		if(FullName[pos]=='\\')
			break;
	}
	if(pos!=0)
	{
		wcsncpy_s(PathName, 511, FullName, pos);
		PathName[pos]='\0';
	}else
		return -1;

	return TRUE;
}
