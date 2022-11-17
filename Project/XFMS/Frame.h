#pragma once
#include "partbase.h"

class CFrame:public CPartBase
{
public:
	DECLARE_DYNAMIC(CFrame)
	CFrame(void);
	~CFrame(void);
	int LoadAttr(void);
	virtual int SaveParam(CXmlNode* pNode);
	virtual int LoadParam(CXmlNode* pNode);
	double m_dblPosZ;
	double m_dblTilt;
	double m_dblRotate;
	double m_dblDetecterZoom;
private:
	int SaveAttr(CXmlNode* pNode);
public:
	virtual int Free(void);
};
