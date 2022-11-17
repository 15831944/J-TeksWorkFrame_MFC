#pragma once

#include "UniUtilBase.h"

class CdblPoint;
class CdblSize;
class CdblRect;
struct tdblPOINT;
struct tdblSIZE;
struct tdblRECT;


typedef struct tdblPOINT
{
	double  dblPosX;
	double  dblPosY;
}tdblPoint;

class DLLDECL CdblPoint : public tdblPoint
{
public:
	// Constructors

	// create an uninitialized point
	CdblPoint() throw();
	// create from two doubles
	CdblPoint(double dblX, double dblY) throw();
	// create from another point
	CdblPoint(CPoint& initPt) throw();
	// create from CdblPoint
	CdblPoint(CdblPoint& initPt) throw();
	// creaqte from a size
	CdblPoint(CSize& initSize) throw();
	// create from a dblSize
	CdblPoint(tdblSIZE& initSize) throw();
	// create from an LPARAM: x = LOWORD(dw) y = HIWORD(dw)
	CdblPoint(LPARAM dwPoint) throw();

	// Distructor
	~CdblPoint(void) throw();

	// Operations

	// translate the point
	void Offset(double xOffset, double yOffset) throw();
	void Offset(tagPOINT& point) throw();
	void Offset(tdblPOINT& dblPoint) throw();
	void Offset(tagSIZE& size) throw();
	void Offset(tdblSIZE& size) throw();

	void SetPoint(double dblX, double dblY) throw();
	void SetPoint(tagPOINT& point) throw();
	void SetPoint(tdblPoint& dblPoint) throw();
	void SetPoint(CSize& size) throw();
	void SetPoint(tdblSIZE& dblSize) throw();

	void operator=(tdblPOINT& dblSrcPoint);
	void operator=(tagPOINT& point);
	void operator=(tagSIZE& size);
	void operator=(tdblSIZE& dblSize);
	void operator+=(tdblSIZE& size) throw();
	void operator-=(tdblSIZE& size) throw();
	void operator+=(tdblPOINT& point) throw();
	void operator-=(tdblPOINT& point) throw();

	BOOL operator==(tdblPOINT& dblPoint) throw();
	BOOL operator!=(tdblPOINT& dblPoint) throw();

	// Operators returning CdblPoint values
	CdblPoint operator+(tdblPOINT& dblPoint) throw();
	CdblPoint operator-() throw();
	CdblPoint operator-(tdblPOINT& dblPoint) throw();
};

typedef struct tdblSIZE
{
	double dblCX;
	double dblCY;
} tDblSize;

class DLLDECL CdblSize:public tDblSize
{
public:
	CdblSize(void);
	~CdblSize(void);

	CdblSize(double CX, double CY);
	CdblSize(tdblSIZE& Size);
	CdblSize(SIZE size);
	void operator+=(tdblSIZE& Size);
	void operator-=(tdblSIZE& Size);
	CdblSize operator+(tdblSIZE& Size);
	CdblSize operator-(tdblSIZE& Size);
	CdblSize operator-(void);
	CdblPoint operator+(tdblPOINT& dblPoint);
	CdblPoint operator-(tdblPOINT& dblPoint);
};

typedef struct tdblRECT
{
	double dblLeft;
	double dblTop;
	double dblRight;
	double dblBottom;
}tdblRect;

class DLLDECL CdblRect:public tdblRECT
{
public:
	CdblRect(void);
	~CdblRect(void);

	CdblRect(double Left, double Top, double Right, double Bottom);
	CdblRect(tdblRECT& dblRect);
	CdblRect(tdblPOINT dblPoint, tdblSIZE dblSize);
	CdblRect(tdblPOINT dblTopLeft, tdblPOINT dblBottomRight);
	double Width(void);
	double Height(void);
	CdblSize Size(void);
	CdblPoint TopLeft(void);
	CdblPoint BottomRight(void);
	CdblPoint CenterPoint(void);
	BOOL PtInRect(tdblPOINT& dblPoint);
	void OffsetRect(double dblX, double dblY);
	void OffsetRect(tdblSIZE dblSize);
	void OffsetRect(tdblPOINT dblPoint);
	void operator=(CdblRect& dblRect);
	BOOL operator==(tdblRECT& dblRect);
	BOOL operator!=(tdblRECT& dblRect);
	void operator+=(tdblPOINT& dblPoint);
	void operator-=(tdblPOINT& dblPoint);
	void operator&=(tdblRECT& dblRect);
	void operator|=(tdblRECT& dblRect);
	CdblRect operator+(tdblPOINT& dblPoint);
	CdblRect operator-(tdblPOINT& dblPoint);
};

