#pragma once
#include "afxcoll.h"
#include "atltypes.h"
#include <UniUtility.h>

class CModelBase;

/// CModelBase 명령 대상입니다.

class CModelBase : public CObject
{
	DECLARE_DYNAMIC(CModelBase)
public:
	enum enumType{typeBASE, typePART, typeALGO};
	CModelBase();
	virtual ~CModelBase();
	/// Component를 구성하는 Component Attribute에 의해 Frame등의 특성으로 변경됨.
	CdblRect m_dblRect;
	/// Base, Part, Algorithm이 있음.
	enumType Type;
	/// Sub에 적용될 OffSet필요하면 상위 Offset까지 읽어, 더해서 작업함.
	CRect m_Offset;
	virtual int LoadParam(CXmlNode* pNode);
	virtual int SaveParam(CXmlNode* pNode);
	CRect GetAccumulatedOffset(void);
	virtual int LoadAttr(void);
	virtual int SaveAttr(CXmlNode* pNode);
	virtual BOOL IsExist(CModelBase* pElement);
	virtual int GetBaseType(void){return typeBASE;}
	virtual CPtrList *GetList(void){return &m_ListElement;}
	virtual int Free(void);
public:
	CPtrList m_ListElement;
};


