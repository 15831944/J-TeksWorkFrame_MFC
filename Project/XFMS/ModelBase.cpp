// ModelBase.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XFMS.h"
#include "ModelBase.h"
#include "ModelData.h"
#include "AtlConv.h"
#include "UniUtility.h"


// CModelBase
IMPLEMENT_DYNAMIC(CModelBase, CObject)
CModelBase::CModelBase()
{
}

CModelBase::~CModelBase()
{
}


// CModelBase 멤버 함수

int CModelBase::LoadParam(CXmlNode* pNode)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);

	m_dblRect.dblLeft	= pNode->ReadDouble(0, _T("Left"));
	m_dblRect.dblRight	= pNode->ReadDouble(0, _T("Right"));
	m_dblRect.dblTop	= pNode->ReadDouble(0, _T("Top"));
	m_dblRect.dblBottom	= pNode->ReadDouble(0, _T("Bottom"));

	CString nameElement=_T("");
	
	CModelBase* pElement=NULL;
	CXmlNode * pXmlChildNode = pNode->GetFirstChildNode();
	while(pXmlChildNode)
	{
		nameElement = pXmlChildNode->GetNodeName();
		pElement = CModelData::GetModelClass(nameElement.GetBuffer());
		if(pElement){
			pElement->LoadParam(pXmlChildNode);
			m_ListElement.AddTail(pElement);
		}///등록되지 않은 알고리즘이라 Skip한다

		pXmlChildNode  = pXmlChildNode->GetNextNode();
	}

	return 0;
}

int CModelBase::SaveParam(CXmlNode* pNode)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	POSITION pos;

	SaveAttr(pNode);

	if (m_ListElement.GetCount()) {
		CModelBase* pChildElement=NULL;
		pos=m_ListElement.GetHeadPosition();
		while(pos!=NULL)
		{
			pChildElement = (CModelBase*)m_ListElement.GetNext(pos);
			pChildElement->SaveParam(pNode);
		}	
	}
	return 0;
}

// Return Value 0:Find
//              -1:Not Founded
BOOL CModelBase::IsExist(CModelBase* pElement)
{
	CModelBase* CurElement;
	POSITION pos;

	for(pos=m_ListElement.GetHeadPosition();pos!=NULL;)
	{
		CurElement=(CModelBase *)m_ListElement.GetNext(pos);
		if(CurElement==pElement)return TRUE;
	}
	return FALSE;
}

CRect CModelBase::GetAccumulatedOffset(void)
{
	return CRect();
}

int CModelBase::LoadAttr(void)
{
	return 0;
}

int CModelBase::SaveAttr(CXmlNode* pNode)
{
	pNode->SetDouble(_T("Left"), m_dblRect.dblLeft);
	pNode->SetDouble(_T("Right"), m_dblRect.dblRight);
	pNode->SetDouble(_T("Top"), m_dblRect.dblTop);
	pNode->SetDouble(_T("Bottom"), m_dblRect.dblBottom);

	return 0;
}

int CModelBase::Free(void)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	POSITION pos;

	if (m_ListElement.GetCount()) {
		CModelBase* pChildElement=NULL;
		pos=m_ListElement.GetHeadPosition();
		while(pos!=NULL)
		{
			pChildElement = (CModelBase*)m_ListElement.GetNext(pos);
			pChildElement->Free();
			delete pChildElement;
		}	

		m_ListElement.RemoveAll();
	}

	return 0;
}
