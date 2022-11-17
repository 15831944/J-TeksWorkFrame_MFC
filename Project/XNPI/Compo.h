#pragma once
#include "partbase.h"
#include <UniUtility.h>
#include "CompoTracker.h"
#include "afxext.h"

class CCompo : public CPartBase
{
	DECLARE_DYNAMIC(CCompo)
public:
	CCompo(void);
	~CCompo(void);
	int SaveAttr(void);
	int LoadAttr(void);
	CPoint TopLeft(void);
	CPoint PTopLeft(void);
	CPoint BottomRight(void);
	CPoint PBottomRight(void);
	CPoint CenterPoint(void);
	CPoint CenterPPoint(void);
	int RectPWidth(void);
	int RectPHeight(void);
	virtual int SaveParam(CXmlNode* pNode);//, BOOL bUseCurNode=FALSE);
	virtual int LoadParam(CXmlNode* pNode);//, BOOL bUseCurNode=FALSE);

	CString m_strRef;
	CdblRect m_rectROI;

	double m_rectTop;
	double m_rectBottom;
	double m_rectLeft;
	double m_rectRight;

	double m_rectPTop;
	double m_rectPBottom;
	double m_rectPLeft;
	double m_rectPRight;
	virtual int Free(void);
	CCompoTracker *m_pCompoTracker;
};

