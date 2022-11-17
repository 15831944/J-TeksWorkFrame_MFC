#include "StdAfx.h"
#include "MilBlob.h"
#include "MilClass.h"

CMilBlob::CMilBlob(void)
{
	m_milResult=NULL;
	m_milIDSystem=NULL;
	m_featureList=NULL;
	m_milIDBuffer=NULL;
	m_ResultList.pCXs=NULL;
	m_ResultList.pCYs=NULL;
	m_ResultList.pBoxLeft=NULL;
	m_ResultList.pBoxBottom=NULL;
	m_ResultList.pBoxRight=NULL;
	m_ResultList.pBoxTop=NULL;
	m_ResultList.pArea=NULL;
}

CMilBlob::CMilBlob(CMilBuffer *pMilBuffer)
{
	SetBuffer(pMilBuffer);
}

CMilBlob::~CMilBlob(void)
{
	Free();
}

void CMilBlob::Caculate(void)
{
//	MblobCalculate(m_pMilBuffer->m_milIDBuffer, M_NULL, m_featureList, m_milResult);
	MblobCalculate(m_pMilBuffer->GetIDBuffer(), M_NULL, m_featureList, m_milResult);
}

int CMilBlob::Control(void)
{
	MblobControl(m_milResult, M_FOREGROUND_VALUE, M_FOREGROUND_BLACK);
	return 0;
}

void CMilBlob::Select(MIL_INT Operation, MIL_INT Feature, MIL_INT Condition, double CondLow, double CondHigh)
{
	MblobSelect(m_milResult, Operation, Feature, Condition, CondLow, CondHigh);
}

int CMilBlob::GetNumber(void)
{
	return MblobGetNumber(m_milResult, M_NULL);
}

int CMilBlob::GetResult(void)
{
	int nBlobCount=GetNumber();
	m_ResultList.nCount=nBlobCount;
	TRACE(_T("Blob Count:%d"), nBlobCount);
	if(!nBlobCount)return -1;

	if(!(m_ResultList.pCXs=(double *)malloc(nBlobCount*sizeof(double))))return -2;
	if(!(m_ResultList.pCYs=(double *)malloc(nBlobCount*sizeof(double))))return -2;
	MblobGetResult(m_milResult, M_CENTER_OF_GRAVITY_X, m_ResultList.pCXs);
	MblobGetResult(m_milResult, M_CENTER_OF_GRAVITY_Y, m_ResultList.pCYs);

	if(!(m_ResultList.pBoxLeft	=(double *)malloc(nBlobCount*sizeof(double))))return -2;
	if(!(m_ResultList.pBoxTop	=(double *)malloc(nBlobCount*sizeof(double))))return -2;
	if(!(m_ResultList.pBoxRight	=(double *)malloc(nBlobCount*sizeof(double))))return -2;
	if(!(m_ResultList.pBoxBottom=(double *)malloc(nBlobCount*sizeof(double))))return -2;
	MblobGetResult(m_milResult, M_BOX_X_MIN, m_ResultList.pBoxLeft);
	MblobGetResult(m_milResult, M_BOX_Y_MIN, m_ResultList.pBoxTop);
	MblobGetResult(m_milResult, M_BOX_X_MAX, m_ResultList.pBoxRight);
	MblobGetResult(m_milResult, M_BOX_Y_MAX, m_ResultList.pBoxBottom);

	if(!(m_ResultList.pArea=(double *)malloc(nBlobCount*sizeof(double))))return -2;
	MblobGetResult(m_milResult, M_AREA, m_ResultList.pArea);
	return 0;
}

void CMilBlob::ReleaseResultMem(void)
{
	if(m_ResultList.pCXs)
	{
		free(m_ResultList.pCXs);
		m_ResultList.pCXs=NULL;
	}
	if(m_ResultList.pCYs)
	{
		free(m_ResultList.pCYs);
		m_ResultList.pCYs=NULL;
	}
	if(m_ResultList.pBoxBottom)
	{
		free(m_ResultList.pBoxBottom);
		m_ResultList.pBoxBottom=NULL;
	}
	if(m_ResultList.pBoxLeft)
	{
		free(m_ResultList.pBoxLeft);
		m_ResultList.pBoxLeft=NULL;
	}
	if(m_ResultList.pBoxRight)
	{
		free(m_ResultList.pBoxRight);
		m_ResultList.pBoxRight=NULL;
	}
	if(m_ResultList.pBoxTop)
	{
		free(m_ResultList.pBoxTop);
		m_ResultList.pBoxTop=NULL;
	}
	if(m_ResultList.pArea)
	{
		free(m_ResultList.pArea);
		m_ResultList.pArea=NULL;
	}
}

int CMilBlob::SelectFeret(void)
{
	return 0;
}

void CMilBlob::SelectFeature(MIL_INT feature)
{
	MblobSelectFeature(m_featureList, feature);
}

int CMilBlob::Reconstruct(void)
{
//	MblobReconstruct(m_milIDBuffer,)
	return 0;
}

int CMilBlob::Fill(void)
{
	return 0;
}

void CMilBlob::Label(void)
{
	MblobLabel(m_milResult, m_pMilBuffer->GetIDBuffer(), M_NO_CLEAR);
}


/*! @fn int CMilBlob::AllocFeatureList(void)
	@brief Feature를 갖어올 리스트를 만듦
	@return M_NULL(0) Alloc Fail */
int CMilBlob::AllocFeatureList(void)
{
	return MblobAllocFeatureList(m_pMilBuffer->m_milIDSystem, &m_featureList);
}

int CMilBlob::AllocResult(void)
{
	return MblobAllocResult(m_pMilBuffer->m_milIDSystem, &m_milResult);
}

void CMilBlob::Draw(CMilDisplay *pMyDisplay, int iLabel)
{
	MblobDraw(pMyDisplay->GetIDGraph(), m_milResult, pMyDisplay->GetIDOverlay(), M_DRAW_BLOBS_CONTOUR+M_DRAW_CENTER_OF_GRAVITY, iLabel, M_DEFAULT);
}

void CMilBlob::Draw(CMilDisplay *pMyDisplay)
{
	MblobDraw(pMyDisplay->GetIDGraph(), m_milResult, pMyDisplay->GetIDOverlay(), M_DRAW_BLOBS_CONTOUR+M_DRAW_CENTER_OF_GRAVITY+M_DRAW_BOX, M_INCLUDED_BLOBS, M_DEFAULT);
}

void CMilBlob::SetMilBuffer(CMilBuffer *pMilBuffer)
{
	m_pMilBuffer=pMilBuffer;
}
int CMilBlob::Free(void)
{
	if(m_milResult)
	{
		MblobFree(m_milResult);
		m_milResult=NULL;
	}
	if(m_featureList)
	{
		MblobFree(m_featureList);
		m_featureList=NULL;
	}
	ReleaseResultMem();
	return 0;
}

void CMilBlob::SetBuffer(CMilBuffer * pMilBuffer)
{
	m_pMilBuffer=pMilBuffer;
	m_milIDSystem = pMilBuffer->m_milIDSystem;
	m_milIDBuffer = m_pMilBuffer->GetIDBuffer();
}
