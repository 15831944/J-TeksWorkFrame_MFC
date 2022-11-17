#pragma once
#include <Mil.h>
#include "MilBase.h"
#include <UniUtility.h>
class CMilBuffer;

/*!
\brief 
* 이미지의 Gray level windowing의 파라메터전달을 위해사용(CMilBuffer::WndLvlTranform)
*/
struct tWndLv
{
	long OrgStart, OrgEnd, DestStart, DestEnd;
};

/*!
\brief 
* Mil의 Image Process관련  함수군을 Class화하였다
@author 전병근
*/
class MIL_DLLDECL CMilIm:public CMilBase
{
public:
	CMilIm(void);
	~CMilIm(void);

/***********************************************************************************************************************
Operation works: M_ADD, M_AND, M_DIV, M_MAX, M_MIN, M_MULT+,	M_NAND,	M_NOR, M_OR, M_SUB+, M_SUB_ABS+, M_XNOR, M_XOR                                                                      
Option2: M_SATURATION, M_FIXED_POINT
Option3: M_ABS, M_NEG, M_NOT, M_PASS
************************************************************************************************************************/ 
	static void Arith(CMilBuffer* pMilDest, CMilBuffer* pMilSrc1, CMilBuffer* pMilSrc2=M_NULL, MIL_INT Operation=M_ADD);

/***********************************************************************************************************************
pMilSrc와 SConst를 연산하여 pMilDest에 넣는다.
Operation works: M_ADD_CONST+, M_AND_CONST, M_CONST_DIV+, M_CONST_PASS,M_CONST_SUB+, M_DIV_CONST+, M_MAX_CONST, M_MIN_CONST,  
Operation continue: M_MULT_CONST+, M_NAND_CONST,M_NOR_CONST,M_OR_CONST, M_SUB_CONST+, M_XNOR_CONST, M_XOR_CONST
Option2: M_SATURATION, M_FIXED_POINT
Option3: M_ABS, M_NEG, M_NOT, M_PASS
************************************************************************************************************************/
	static void ArithConst(CMilBuffer* pMilDest, CMilBuffer *pMilSrc, MIL_DOUBLE SConst=M_NULL, MIL_INT Operation=M_ADD_CONST);
	static void Resize(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, double FactorX, double FactorY, MIL_INT InterpolationType=M_DEFAULT);
	static void Open(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT NbIteration=1, MIL_INT ProcMode=M_GRAYSCALE);
	static void Close(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT NbIteration=1, MIL_INT ProcMode=M_GRAYSCALE);
	static void Erode(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT NbIteration=1, MIL_INT ProcMode=M_GRAYSCALE);
	static void Dilate(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT NbIteration=1, MIL_INT ProcMode=M_GRAYSCALE);
/**********************************************************************************************************************
M_EDGE_DETECT+, M_EDGE_DETECT2+, M_HORIZ_EDGE+, M_LAPLACIAN_EDGE+, M_LAPLACIAN_EDGE2+ 
M_SHARPEN+, M_SHARPEN2+, M_SMOOTH+, M_VERT_EDGE+
**********************************************************************************************************************/
	static void Convolve(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT FilterID);
	static void Watershed(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT MinimumVariation=M_DEFAULT, MIL_INT ControlFlag=M_DEFAULT);
	static void Binarize(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_DOUBLE CondLow=M_DEFAULT, MIL_DOUBLE CondHigh=M_DEFAULT, MIL_INT Condition=M_GREATER_OR_EQUAL);

/// basis Source buffer to manipulate
	CMilBuffer* m_pBuffer;
	static void Rotate(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_DOUBLE rotAngle, CdblPoint DstCen, CdblPoint SrcCen);
	static int Histogram(CMilBuffer *pMilSrc, MIL_ID histResult);
	static int Histogram(CMilBuffer *pMilSrc, int nValCount, long *pHistoValue);
	static int Shift(CMilBuffer * pMilDest, CMilBuffer * pMilSrc, int nShift);
	static int GetMin(CMilBuffer * pMilSrc);
	static int GetMax(CMilBuffer * pMilSrc);
	static int GetMinMax(CMilBuffer * pMilSrc, long *pMin, long *pMax);
	static int GetLimitedMinMax(CMilBuffer * pMilSrc, int Limit, long *pMin, long *pMax, BOOL bCutHigh=TRUE, BOOL bCutLow=TRUE);
	static int Median(CMilBuffer * pMilDest, CMilBuffer * pMilSrc, int MaskMethod=M_3X3_CROSS, int ProcMode=M_GRAYSCALE);
	static int UserDefine1(CMilBuffer * pMilDest, CMilBuffer * pMilSrc);
	static int Flip(CMilBuffer * pMilDest, CMilBuffer * pMilSrc, int iDir=M_FLIP_HORIZONTAL);
};
