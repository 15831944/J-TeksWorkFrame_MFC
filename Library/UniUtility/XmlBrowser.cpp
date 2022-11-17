#include "stdafx.h"
#include "FileInfo.h"
#include "XmlBrowser.h"

#pragma warning (disable : 4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CXmlBrowser, CObject);

CXmlBrowser::CXmlBrowser()
{
	m_pRootNode = NULL;
}

CXmlBrowser::~CXmlBrowser()
{
	if ( m_pRootNode != NULL )
		delete m_pRootNode;
}

BOOL CXmlBrowser::Create(LPCTSTR szDefaultData)
{
	TESTHR(m_docPtr.CreateInstance(__uuidof(DOMDocument)));
	if (szDefaultData != NULL) {
		if (m_docPtr->loadXML(_bstr_t(szDefaultData)) == VARIANT_FALSE) {
			ReportError();
			m_docPtr = NULL;
			return FALSE;
		} else {
			m_docPtr->async = VARIANT_FALSE;
			m_pRootNode = new CXmlNode(m_docPtr->documentElement, m_docPtr, this);
		}
	}
	return TRUE;
}

BOOL CXmlBrowser::Open(LPCTSTR lpszPathName)
{
	BOOL result = FALSE;
	wcscpy_s(m_szPathName, 511, lpszPathName);
	if (CFileInfo::FileCheck(lpszPathName)) {
		TESTHR(m_docPtr.CreateInstance(__uuidof(DOMDocument)));

		if ( m_docPtr->load(_bstr_t(lpszPathName)) != VARIANT_FALSE ) {
			m_docPtr->async = VARIANT_FALSE;
			m_pRootNode = new CXmlNode(m_docPtr->documentElement, m_docPtr, this);
			result = TRUE;
		} else {
//			ReportError();
			m_docPtr = NULL;
		}
	}

	return result;
}

CString CXmlBrowser::GetCurrName(void)
{
	return CString(m_szPathName);
}


void CXmlBrowser::ReportError()
{
	MSXML2::IXMLDOMParseErrorPtr pIParseError = m_docPtr->GetparseError();
	if (pIParseError)
	{
		long value = pIParseError->GeterrorCode();
		_bstr_t reason = pIParseError->Getreason();

		MessageBox(NULL, reason, _T("LoadXML Malformed XML Error"), MB_OK);

		pIParseError = NULL;
	}
}

BOOL CXmlBrowser::IsAvailable()
{
	return m_docPtr != NULL;
}

BOOL CXmlBrowser::Save(LPCTSTR lpszPathName)
{
	wcscpy_s(m_szPathName, 511, lpszPathName);
	_variant_t varXml(lpszPathName);
	m_docPtr->preserveWhiteSpace = true;
	if (FAILED(m_docPtr->save(varXml))) {
		ReportError();
		return FALSE;
	}
	return TRUE;
}

CXmlNode* CXmlBrowser::GetRootNode(LPCTSTR nodeName)
{
	if ( m_pRootNode == NULL && nodeName != NULL ) {
		m_pRootNode = new CXmlNode(m_docPtr->appendChild(m_docPtr->createElement(nodeName)), m_docPtr, this);
	}
	
	return m_pRootNode;
}

IMPLEMENT_DYNAMIC(CXmlNode, CObject);
CXmlNode::CXmlNode(MSXML2::IXMLDOMNodePtr nodePtr, MSXML2::IXMLDOMDocumentPtr& docPtr, CXmlBrowser* pXmlBrowser)
{
	m_nodePtr = nodePtr;
	m_docPtr = docPtr;
	m_pXmlBrowser = pXmlBrowser;
}

CXmlNode::~CXmlNode()
{
	POSITION pos = m_nodeList.GetHeadPosition();
	while ( pos != NULL ) {
		delete m_nodeList.GetNext(pos);
	}
	m_nodeList.RemoveAll();
}

CXmlNode* CXmlNode::AddNode(TCHAR* nodeName)
{
	m_nodePtr->appendChild(m_docPtr->createTextNode(_T("\n")));
	AddBlank(m_nodePtr->parentNode);

	CXmlNode* pNewNode = new CXmlNode(m_nodePtr->appendChild(m_docPtr->createElement(nodeName)), m_docPtr, m_pXmlBrowser);
	m_nodeList.AddTail(pNewNode);

	return pNewNode;
}

BOOL CXmlNode::RemoveNode(LPCTSTR nodeName)
{
	CXmlNode* pNode = GetChildNode(nodeName);
	if (pNode != NULL) {
		MSXML2::IXMLDOMParseErrorPtr oldPtr = m_nodePtr->removeChild(pNode->GetNodePtr());
		if (oldPtr) {
			oldPtr = NULL;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CXmlNode::RemoveNode(CXmlNode* pNode)
{
	if (pNode != NULL) {
		MSXML2::IXMLDOMParseErrorPtr oldPtr = m_nodePtr->removeChild(pNode->GetNodePtr());
		if (oldPtr) {
			oldPtr = NULL;
			return TRUE;
		}
	}
	return FALSE;
}

CString CXmlNode::GetNodeName()
{
	return CString((char*)m_nodePtr->nodeName);
}

BOOL CXmlNode::CheckNode(LPCTSTR nodeName)
{
	if ( wcscmp ( nodeName , m_nodePtr->nodeName ) == 0 )
		return TRUE;
	else 
		return FALSE;
}

void CXmlNode::AddBlank(MSXML2::IXMLDOMNodePtr parentNode)
{
	while ( parentNode != NULL ) {
		m_nodePtr->appendChild(m_docPtr->createTextNode("  "));
		parentNode = parentNode->parentNode;
	}
}

void CXmlNode::EndNode()
{
	m_nodePtr->appendChild(m_docPtr->createTextNode("\n"));
	AddBlank(m_nodePtr->parentNode->parentNode);
}

MSXML2::IXMLDOMNodePtr CXmlNode::GetNodePtr()
{
	return m_nodePtr;
}

CXmlBrowser* CXmlNode::GetXmlBrowser()
{
	return m_pXmlBrowser;
}

CString CXmlNode::GetText(LPCTSTR defaultValue )
{
	CString str = m_nodePtr->text;
	if (str == "")
		str = defaultValue;

	return str;
}

CString CXmlNode::GetValue(LPCTSTR name, LPCTSTR defaultValue)
{
	TCHAR *pTemp;
	MSXML2::IXMLDOMNodePtr attribNode = m_nodePtr->Getattributes()->getNamedItem(name);
	if (attribNode != NULL) 
	{
		pTemp=attribNode->text;
		return pTemp;
	}
	else
		return defaultValue;
}

CXmlNode* CXmlNode::GetChildNode(LPCTSTR name, CXmlNode* pFindAfter)
{
	CString nodeName;
	MSXML2::IXMLDOMNodePtr childNode;
	if (pFindAfter == NULL)
		childNode = m_nodePtr->firstChild;
	else 
		childNode = pFindAfter->GetNodePtr()->nextSibling;

	while ( childNode != NULL ) {
		nodeName = (char*)childNode->nodeName;
		if ( nodeName == name ) {
			CXmlNode * pNewNode = new CXmlNode(childNode,m_docPtr, m_pXmlBrowser);
			m_nodeList.AddTail(pNewNode);
			return pNewNode;
		}
		childNode = childNode->nextSibling;
	}
	return NULL;
}

CXmlNode* CXmlNode::GetChildNode(LPCTSTR name, LPCTSTR attr, LPCTSTR value)
{
	CXmlNode* pNode = GetChildNode(name);
	while (pNode != NULL) {
		if (pNode->GetValue(attr) == value) {
			return pNode;
		}
		pNode = GetChildNode(name, pNode);
	}
	return NULL;
}

void CXmlNode::SetLong ( long value )
{
	TCHAR temp[20];
	wsprintf ( temp , _T("%ld") , value );
	m_nodePtr->text = temp;
}

void CXmlNode::SetDouble ( double value )
{
	TCHAR temp[20];
	wsprintf ( temp , _T("%lf") , value );
	m_nodePtr->text = temp;
}

void CXmlNode::SetString ( LPCTSTR value )
{
	m_nodePtr->text = value;
}

void CXmlNode::SetLong ( LPCTSTR nodeName , long value )
{
	CString temp;
	temp.Format(_T("%ld") , value );

	SetString ( nodeName , temp );
}

void CXmlNode::SetDouble ( LPCTSTR nodeName , double value )
{
	CString temp;
	temp.Format(_T("%lf") , value );

	SetString ( nodeName , temp );
}

void CXmlNode::SetString ( LPCTSTR nodeName , LPCTSTR value )
{
	MSXML2::IXMLDOMAttributePtr attributePtr = m_docPtr->createAttribute(nodeName);
	attributePtr->text = value;

	m_nodePtr->Getattributes()->setNamedItem(attributePtr);
}

CXmlNode* CXmlNode::GetFirstChildNode()
{
	if ( m_nodePtr->hasChildNodes() != NULL ) {
		CXmlNode * pNewNode = new CXmlNode(m_nodePtr->firstChild,m_docPtr, m_pXmlBrowser);
		m_nodeList.AddTail(pNewNode);
		return pNewNode;
	} else {
		return NULL;
	}
}

CXmlNode* CXmlNode::GetNextNode()
{
	if ( m_nodePtr->nextSibling != NULL ) {
		CXmlNode * pNewNode = new CXmlNode(m_nodePtr->nextSibling,m_docPtr, m_pXmlBrowser);
		m_nodeList.AddTail(pNewNode);
		return pNewNode;
	} else {
		return NULL;
	}
}

double CXmlNode::ReadDouble(double defValue, LPCTSTR szName)
{
	CString strDefValue;
	strDefValue.Format(_T("%f"), defValue);
	return _wtof(GetValue(szName, strDefValue));
}

long CXmlNode::ReadLong(long defValue, LPCTSTR szName)
{
	CString strDefValue;
	strDefValue.Format(_T("%d"), defValue);
	return _wtol(GetValue(szName, strDefValue));
}

CString CXmlNode::ReadString(LPCTSTR szDefValue, LPCTSTR szName)
{
	return (GetValue(szName, szDefValue));
}
