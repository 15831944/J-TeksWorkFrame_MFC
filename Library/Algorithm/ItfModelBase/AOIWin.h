#pragma once
#include "PartBase.h"

class ITFBASEDLLDECL CAOIWin : public CPartBase
{
	DECLARE_DYNAMIC(CAOIWin)
public:
	CAOIWin(void);
	~CAOIWin(void);
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

	CString m_rectRef;
	CdblRect m_rectROI;

	double m_rectTop;
	double m_rectBottom;
	double m_rectLeft;
	double m_rectRight;

	double m_rectPTop;
	double m_rectPBottom;
	double m_rectPLeft;
	double m_rectPRight;

	int m_BlobArea;
	int m_BlobAreaLimit;
	int m_BlobOfsX;
	int m_BlobOfsY;
	int m_ARLimit;

	int m_InspDir;
	int m_typeChip;
	int m_grayIncline;
	int m_inspAOI;
	int m_thresRatio;
	int m_inspMA;
	int m_inspMCG;

	virtual int Free(void);
};

