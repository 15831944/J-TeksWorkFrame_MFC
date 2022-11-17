#pragma once

#include "stdafx.h"
#include "UniUtilBase.h"
#import "msxml6.dll" //rename_namespace("XA_MSXML")

#define PROCINST _T("<?xml version=\"1.0\"?><X2XML></X2XML>")
#define TESTHR(hr) { if FAILED(hr) return FALSE; }

class CXmlBrowser;

class DLLDECL CXmlNode : public CObject
{
	DECLARE_DYNAMIC(CXmlNode);

	CList<CXmlNode*,CXmlNode*> m_nodeList;
	MSXML2::IXMLDOMDocumentPtr m_docPtr;
	MSXML2::IXMLDOMNodePtr m_nodePtr;
	CXmlBrowser* m_pXmlBrowser;
public:
	CXmlNode(MSXML2::IXMLDOMNodePtr nodePtr, MSXML2::IXMLDOMDocumentPtr& docPtr, CXmlBrowser* pXmlBrowser);
	virtual ~CXmlNode();

	CString GetNodeName();
	BOOL CheckNode(LPCTSTR nodeName);
	CXmlNode* AddNode(TCHAR* nodeName);
	BOOL RemoveNode(LPCTSTR nodeName);
	BOOL RemoveNode(CXmlNode* pNode);

	void AddBlank(MSXML2::IXMLDOMNodePtr parentNode);
	void EndNode();

	MSXML2::IXMLDOMNodePtr GetNodePtr();
	CXmlNode* GetChildNode(LPCTSTR nodeName, CXmlNode* pFindAfter = NULL);
	CXmlNode* GetChildNode(LPCTSTR name, LPCTSTR attr, LPCTSTR value);
	CXmlNode* GetFirstChildNode();
	CXmlNode* GetNextNode();
	CXmlBrowser* GetXmlBrowser();

	CString GetText(LPCTSTR defaultValue = _T(""));
	CString GetValue(LPCTSTR name, LPCTSTR defaultValue = _T(""));
	void SetLong ( long value );
	void SetDouble ( double value );
	void SetString ( LPCTSTR value );
	void SetLong ( LPCTSTR nodeName , long value );
	void SetDouble ( LPCTSTR nodeName , double value );
	void SetString ( LPCTSTR nodeName , LPCTSTR value );
	double ReadDouble(double defValue, LPCTSTR szName);
	long ReadLong(long defValue, LPCTSTR szName);
	CString ReadString(LPCTSTR szDefValue, LPCTSTR szName);
};


class DLLDECL CXmlBrowser : CObject
{
	DECLARE_DYNAMIC( CXmlBrowser );

	MSXML2::IXMLDOMDocumentPtr m_docPtr;
	CXmlNode* m_pRootNode;
public:
	CXmlBrowser();
	virtual ~CXmlBrowser();

	BOOL Create(LPCTSTR defaultData = NULL);
	BOOL Open(LPCTSTR lpszPathName);
	BOOL Save(LPCTSTR lpszPathName);
	CXmlNode* GetRootNode(LPCTSTR nodeName = NULL);
	WCHAR m_szPathName[512];
	CString GetCurrName(void);
	void ReportError();
	BOOL IsAvailable();
};
