#pragma once
#include "PartBase.h"
#include <UniUtility.h>

class CCamPart : public CPartBase
{
	DECLARE_DYNAMIC(CCamPart)
public:
	CCamPart();
	~CCamPart();
	// ! 0..3까지의 카메라번호
	int m_nCamNo;
	virtual int SaveParam(CXmlNode* pNode);//, BOOL bUseCurNode=FALSE);
	virtual int LoadParam(CXmlNode* pNode);//, BOOL bUseCurNode=FALSE);
};

