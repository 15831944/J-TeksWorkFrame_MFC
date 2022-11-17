#include "StdAfx.h"
#include "MilClass.h"
#include "MilIm.h"

CMilIm::CMilIm(void)
{
}

CMilIm::~CMilIm(void)
{
}

void CMilIm::Arith(CMilBuffer *pMilDest, CMilBuffer *pMilSrc1, CMilBuffer *pMilSrc2, MIL_INT Operation)
{
	MimArith(pMilSrc1->GetIDBuffer(), pMilSrc2->GetIDBuffer(), pMilDest->GetIDBuffer(), Operation);
}

void CMilIm::ArithConst(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_DOUBLE SConst, MIL_INT Operation)
{
	MimArith(pMilSrc->GetIDBuffer(), SConst, pMilDest->GetIDBuffer(), Operation);
}

void CMilIm::Resize(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, double FactorX, double FactorY, MIL_INT InterpolationType)
{
	MimResize(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), FactorX, FactorY, M_DEFAULT);
}

void CMilIm::Open(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT NbIteration, MIL_INT ProcMode)
{
	MimOpen(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), NbIteration, ProcMode);
}

void CMilIm::Close(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT NbIteration, MIL_INT ProcMode)
{
	MimClose(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), NbIteration, ProcMode);
}

void CMilIm::Erode(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT NbIteration, MIL_INT ProcMode)
{
	MimErode(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), NbIteration, ProcMode);
}

void CMilIm::Dilate(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT NbIteration, MIL_INT ProcMode)
{
	MimDilate(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), NbIteration, ProcMode);
}

void CMilIm::Convolve(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT FilterID)
{
	MimConvolve(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), FilterID);
}

void CMilIm::Watershed(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_INT MinimumVariation, MIL_INT ControlFlag)
{
	MimWatershed(pMilSrc->GetIDBuffer(), M_NULL, pMilDest->GetIDBuffer(), MinimumVariation, ControlFlag);
}

void CMilIm::Binarize(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_DOUBLE CondLow, MIL_DOUBLE CondHigh, MIL_INT Condition)
{
	MimBinarize(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), Condition, CondLow, CondHigh);
}

int CMilIm::Histogram(CMilBuffer *pMilSrc, MIL_ID histResult)
{
	MimHistogram(pMilSrc->GetIDBuffer(), histResult);
	return 0;
}

int CMilIm::Histogram(CMilBuffer *pMilSrc, int nValCount, long *pHistoValue)
{
	MIL_ID histResult;
	MimAllocResult(CMilSystem::Instance()->GetSystemID(), nValCount, M_HIST_LIST, &histResult);
	MimHistogram(pMilSrc->GetIDBuffer(), histResult);
	MimGetResult(histResult, M_VALUE, (MIL_INT *)pHistoValue);
	MimFree(histResult);
	return 0;
}


int CMilIm::Shift(CMilBuffer * pMilDest, CMilBuffer * pMilSrc, int nShift)
{
	MimShift(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), nShift);
	return 0;
}

int CMilIm::GetMin(CMilBuffer * pMilSrc)
{
	int iValue;
	MIL_ID resultExtreme;
	MimAllocResult(CMilSystem::Instance()->GetSystemID(), 1L, M_EXTREME_LIST, &resultExtreme);
	MimFindExtreme(pMilSrc->GetIDBuffer(), resultExtreme,M_MIN_VALUE);
    MimGetResult(resultExtreme, M_VALUE, (MIL_INT *)&iValue);	
	MimFree(resultExtreme);
	return iValue;
}

int CMilIm::GetMax(CMilBuffer * pMilSrc)
{
	int iValue;
	MIL_ID resultExtreme;
	MimAllocResult(CMilSystem::Instance()->GetSystemID(), 1L, M_EXTREME_LIST, &resultExtreme);
	MimFindExtreme(pMilSrc->GetIDBuffer(), resultExtreme,M_MAX_VALUE);
	MimGetResult(resultExtreme, M_VALUE, (MIL_INT *)&iValue);	
	MimFree(resultExtreme);
	return iValue;
}

int CMilIm::GetMinMax(CMilBuffer * pMilSrc, long *pMin, long *pMax)
{
	MIL_ID resultExtreme;
	long MinMax[2];
	MimAllocResult(CMilSystem::Instance()->GetSystemID(), 2L, M_EXTREME_LIST, &resultExtreme);
	MimFindExtreme(pMilSrc->GetIDBuffer(), resultExtreme,M_MIN_VALUE+M_MAX_VALUE);
	MimGetResult(resultExtreme, M_VALUE,MinMax);	
	MimFree(resultExtreme);
	*pMin=MinMax[0];
	*pMax=MinMax[1];
	return 0;
}

///Limit Count
int CMilIm::GetLimitedMinMax(CMilBuffer * pMilSrc, int Limit, long *pMin, long *pMax, BOOL bCutHigh, BOOL bCutLow)
{
	long *pOrgHistoData;
	long vMin, vMax=0, vIn;
	int added=0;

	pOrgHistoData = new long[65536];
	*pMin=0;
	*pMax=65536;

	Histogram(pMilSrc, 65536, pOrgHistoData);	

	BOOL fMinFounded=FALSE;
	for(int i=0;i<65536;i++)
	{
		if(vIn=pOrgHistoData[i])
		{
			if(!fMinFounded)
			{
				vMin=i;
				fMinFounded=TRUE;
			}
			if(vMax<i) vMax=i;
		}
	}

	*pMin=vMin;
	*pMax=vMax;

///Find Limit MinLimit
	if(bCutLow)
		for(int i=vMin;i<65536;i++)
		{
			added+=pOrgHistoData[i];
			if(added>Limit)
			{
				*pMin=i;
				break;
			}
		}

///Find Limit MaxLimit
	added=0;
	if(bCutHigh)
		for(int i=vMax;i>0;i--)
		{
			added+=pOrgHistoData[i];
			if(added>Limit)
			{
				*pMax=i;
				break;
			}
		}

	delete pOrgHistoData;
	return 0;
}

int CMilIm::Median(CMilBuffer * pMilDest, CMilBuffer * pMilSrc, int MaskMethod, int ProcMode)
{
	MimRank(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), MaskMethod, M_MEDIAN, ProcMode);
//	MimRank(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), M_5X5_RECT, M_MEDIAN, ProcMode);
	return 0;
}


int CMilIm::UserDefine1(CMilBuffer * pMilDest, CMilBuffer * pMilSrc)
{
	CMilIm::Convolve(pMilDest, pMilSrc, M_DERICHE_FILTER( M_SHARPEN, 55));
	return 0;
}

int CMilIm::Flip(CMilBuffer * pMilDest, CMilBuffer * pMilSrc, int iDir)
{
	MimFlip(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), iDir, M_DEFAULT);
	return 0;
}


void CMilIm::Rotate(CMilBuffer *pMilDest, CMilBuffer *pMilSrc, MIL_DOUBLE rotAngle, CdblPoint DstCen, CdblPoint SrcCen)
{    
	MimRotate(pMilSrc->GetIDBuffer(), pMilDest->GetIDBuffer(), rotAngle , M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
    
}


