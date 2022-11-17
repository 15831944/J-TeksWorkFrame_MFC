// CfgFile.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "CfgFile.h"
#include "Tokenizer.h"
#include "FileInfo.h"
#include "XmlBrowser.h"
#include <shlwapi.h>


#define DELIMITER '/'


/////////////////////////////////////////////////////////////////////////////
// CCfgFile
CCfgFile* CCfgFile::m_pInstance = NULL;
CCfgFile::CCfgFile(BOOL fSyncSave)
: m_strPath(_T(""))
, m_nMethod(0)
, m_nDepth(2)
{
	m_fileName = _T("");
	m_fSyncSave = fSyncSave;
}

CCfgFile::~CCfgFile()
{
	if(m_xmlBrowser.IsAvailable() && m_fSyncSave)
		Flush();
}


CXmlBrowser* CCfgFile::GetXmlBrowser(void)
{ 
	return &m_xmlBrowser; 
}

// get a long value
long CCfgFile::Get(long lDefValue, TCHAR* szTag, ...)
{
	CString strDftValue;
	strDftValue.Format(_T("%d"), lDefValue);
	va_list argsList;
	va_start(argsList, szTag);

	return _wtol(VsGetString(strDftValue.GetBuffer(), szTag, argsList).GetBuffer());
}

// get a double value
double CCfgFile::Get(double dblDefValue, TCHAR* szTag, ...)
{
	CString strDftValue;
	strDftValue.Format(_T("%lf"), dblDefValue);
	va_list argsList;
	va_start(argsList, szTag);

	return _wtof(VsGetString(strDftValue.GetBuffer(), szTag, argsList));
}

// get a string value
CString CCfgFile::VsGetString(TCHAR* szDefValue, TCHAR* szTag, va_list argsList)
{
	ASSERT(m_xmlBrowser.IsAvailable());

	CString nameNode = szTag;
	CString strReturn = szDefValue;

	for(int i=0;i<m_nDepth-1;i++)
	{
		szTag=va_arg(argsList, TCHAR *);
		// Add the value to the base key separated by a '/'
		nameNode += _T("/"); 
		nameNode += szTag;
	}

	// returns the last node in the chain
	CXmlNode* pNode = GetNode(nameNode); 
	if (pNode) {
		strReturn = pNode->GetText();
	}
	return strReturn;
}

// get a string value
CString CCfgFile::Get(TCHAR *szDefValue, TCHAR *szTag, ...)
{
	ASSERT(m_xmlBrowser.IsAvailable());
	va_list marker;
	va_start(marker, szTag);

	return VsGetString(szDefValue, szTag, marker);
}

// set a long value
long CCfgFile::Set(long lValue, TCHAR* szTag, ...)
{
	CString valueStr;
	valueStr.Format(_T("%d"), lValue);
	va_list marker;
	va_start(marker, szTag);

	return VsSetString(valueStr.GetBuffer(), szTag, marker);
}

// set a double value
long CCfgFile::Set(double lValue, TCHAR* szTag, ...)
{
	CString valueStr;
	valueStr.Format(_T("%lf"), lValue);
	va_list marker;
	va_start(marker, szTag);
	
	return VsSetString(valueStr.GetBuffer(), szTag, marker);
}

#define SSS_SUCCESS	0
#define SSS_NODE_NOT_FOUND	-1
#define SSS_PUT_TEXT_FAILED	-2
#define SSS_SAVE_FAILED		-3

// set a string value 
long CCfgFile::VsSetString(TCHAR *szValue, TCHAR *szTag, va_list argLists)
{
	ASSERT(m_xmlBrowser.IsAvailable());
	long lRetVal = SSS_SUCCESS;
	CString nameNode=szTag;

	for(int i=0;i<m_nDepth-1;i++)
	{
		szTag=va_arg(argLists, TCHAR *);
	//! Add the values to the blank nameNode var. seperated by a '/'
		nameNode+=_T("/");
		nameNode+=szTag;
	}

	// returns the last node in the chain
	CXmlNode* pNode = GetNode(nameNode, TRUE); 
	if (pNode) {
		// set the text of the node (will be the value we sent)
		pNode->SetString(szValue);

		if (m_fSyncSave)
			m_xmlBrowser.Save(m_fileName);
	} else {
		lRetVal = SSS_NODE_NOT_FOUND;
	}

	return lRetVal;
}

// set a string value 
long CCfgFile::Set(TCHAR *szValue, TCHAR *szTag, ...)
{
	ASSERT(m_xmlBrowser.IsAvailable());
	long lRetVal = SSS_SUCCESS;
	CString nameNode;
	va_list marker;

	va_start(marker, szTag);

	return VsSetString(szValue, szTag, marker);
}

// delete a key or chain of keys
BOOL CCfgFile::DeleteSetting(TCHAR* szTag, ...)
{
	ASSERT(m_xmlBrowser.IsAvailable());
	long fRetVal = FALSE;
	va_list marker;
	va_start(marker, szTag);
	CString nameNode=szTag;

	for(int i=0;i<m_nDepth;i++)
	{
	// Add the value to the base key separated by a '/'
		szTag=va_arg(marker, TCHAR*);
		nameNode += _T("/");
		nameNode += szTag;
	}

	// returns the last node in the chain
	CXmlNode* pNode = GetNode(nameNode, TRUE); 
	if (pNode) {
		MSXML2::IXMLDOMNodePtr pParentNode = NULL;
		MSXML2::IXMLDOMNodePtr pOldChild;
		pNode->GetNodePtr()->get_parentNode(&pParentNode);
 		pParentNode->removeChild(pNode->GetNodePtr());
		if (m_fSyncSave)
			m_xmlBrowser.Save(m_fileName);
		fRetVal = TRUE;
	}
	return fRetVal;
}

BOOL CCfgFile::Load(CString fileName)
{
	m_fileName = fileName;

	// see if the file exists
	if (!PathFileExists(m_fileName)) {

		CString defaultXml = PROCINST;
		return m_xmlBrowser.Create(defaultXml);
	}
	else  // if the file exists
	{

		// load it
		BOOL fResult = m_xmlBrowser.Open(m_fileName);
		if (!fResult) {
			CString bakFile = m_fileName + ".bak";
			fResult = m_xmlBrowser.Open(m_fileName);
			if (fResult)
				Flush();
		}

		return fResult;
	}
}

void CCfgFile::SetSyncMode(BOOL fSyncSave)
{
	m_fSyncSave = fSyncSave;
}

// save the XML file
BOOL CCfgFile::Flush()
{

	if(!CFileInfo::IsReadonly(m_fileName)) {
		CString bakFile = m_fileName + ".bak";
		CopyFile(m_fileName, bakFile, FALSE);
		return m_xmlBrowser.Save(m_fileName);
	} else
		return FALSE;
}

// find a node given a chain of key names
CXmlNode* CCfgFile::GetNode(CString keyString, BOOL fAddNodes /*FALSE*/)
{
	keyString.Replace(' ', '_');
	keyString.Replace('\\', DELIMITER);
	keyString.TrimRight(DELIMITER);  // remove slashes on the end
	
	CString delimiter(DELIMITER);

	CString token;
	CTokenizer tokenizer(keyString);
	CXmlNode* pNode;
	CXmlNode* pParentNode = m_xmlBrowser.GetRootNode();
	while (tokenizer.IsTokenExist())
	{
		token = tokenizer.GetNextToken(delimiter);
		// find the node named X directly under the parent
		pNode = pParentNode->GetChildNode(token);
		if (pNode == NULL) {
			if (fAddNodes)  { // create the node and append to parent (Set only)
				pNode = pParentNode->AddNode(token.GetBuffer());
			} else {
				return NULL;
			}
		}
		pParentNode = pNode;
	}

	return pParentNode;
}

int CCfgFile::Open(void)
{
	return Load(m_strPath);
}

void CCfgFile::SetDepth(int nDepth)
{
	m_nDepth=nDepth;
}

void CCfgFile::SetPath(CString strPath)
{
	m_strPath=strPath;
}

void CCfgFile::SetCfgMethod(int nMethod)
{
	m_nMethod=nMethod;
}

CCfgFile* CCfgFile::Instance(void)
{
	if(!m_pInstance)m_pInstance=new CCfgFile;	
	return m_pInstance;
}

BOOL CCfgFile::EraseAll(void)
{
	ASSERT(m_xmlBrowser.IsAvailable());
	long fRetVal = FALSE;

	/// return false, if fail.
	CXmlNode* pNode = m_xmlBrowser.GetRootNode();
	CXmlNode* pChildNode=NULL;
	if (pNode)
	{
		while(pChildNode = pNode->GetFirstChildNode())
		{
			/// the next node will be first node, after every deleting process. 
			pNode->GetNodePtr()->removeChild(pChildNode->GetNodePtr());
		}

		if (m_fSyncSave)
			m_xmlBrowser.Save(m_fileName);
		fRetVal = TRUE;
	}

	return fRetVal;
}

CString CCfgFile::GetPath(void)
{
	return CString();
}
