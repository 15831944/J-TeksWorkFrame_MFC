#include "StdAfx.h"
#include "DbgFile.h"

CDbgFile *CDbgFile::m_pInstance=NULL;
CDbgFile::CDbgFile(void)
: m_lIndex(0)
{
	strFileName=_T("DebugFile.txt");
	CStdioFile DebugFile;
	DebugFile.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	DebugFile.SeekToEnd();
	DebugFile.Close();
}

CDbgFile::~CDbgFile(void)
{
}

CDbgFile * CDbgFile::Instance(void)
{
	if(!m_pInstance)
	{
		m_pInstance=new CDbgFile;
		m_pInstance->m_lIndex=0;
	}
	return m_pInstance;
}

void CDbgFile::SetDbgFileName(CString strName)
{
	strFileName=strName;
}

void CDbgFile::Write(TCHAR * pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);
	CString strToWrite, strPrifix, strSuffix;
	strToWrite.FormatV(pFormat, args);
	m_lIndex++;
	strSuffix.Format(_T("\n"));
	strPrifix.Format(_T("%ld:"),m_lIndex);
	strToWrite=strPrifix+strToWrite+strSuffix;

	CStdioFile DebugFile;
	DebugFile.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	DebugFile.SeekToEnd();
	DebugFile.WriteString(strToWrite);
	DebugFile.Close();
}
