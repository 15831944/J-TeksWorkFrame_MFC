#include "stdafx.h"
#include <ipp.h>
#include <ipps.h>
#include "FilterIIR.h"


CFilterIIR::CFilterIIR()
	: m_dblFS(0)
	, m_iOrder(0)
	, m_dblCutOffFreq(0)
	, m_iBufferSizeGen(0)
	, m_iBufferSizeRIIR(0)
	, m_pBufIIRIIR(NULL)
	, m_pTaps(NULL)
	, m_pStateIIRIIR(NULL)
{
}


CFilterIIR::~CFilterIIR()
{
	Free();
}


int CFilterIIR::Init(double dblFS, double dblCutOffFreq, tFILTERTYPE iFilterType, int iOrder)
{
	m_dblFS = dblFS;
	m_iOrder = iOrder;
	m_dblCutOffFreq = dblCutOffFreq;
	/* get size of mem buffers for IIR and IIRGen functions */
	m_ippStatus = ippsIIRIIRGetStateSize64f_32f(m_iOrder, &m_iBufferSizeRIIR);
	if (ippStsNoErr != m_ippStatus)
		return -1;
	m_ippStatus = ippsIIRGenGetBufferSize(m_iOrder, &m_iBufferSizeGen);
	if (ippStsNoErr != m_ippStatus)
		return -1;
	/* we can allocate MAX size from two - operations are sequential and therefore buffer can be reused */
	m_pBufIIRIIR = ippsMalloc_8u(IPP_MAX(m_iBufferSizeGen, m_iBufferSizeRIIR)*2);
	m_pTaps = ippsMalloc_64f(2 * (m_iOrder + 1));
	/* gen LowPass filter coefficients */
	switch (iFilterType)
	{
	case FT_LOWPASS:
			m_ippStatus = ippsIIRGenLowpass_64f((double)dblCutOffFreq / dblFS, 0.0, m_iOrder, m_pTaps, ippButterworth, m_pBufIIRIIR);
		break;
	case FT_HIGHPASS:
		m_ippStatus = ippsIIRGenHighpass_64f((double)dblCutOffFreq / dblFS, 0.0, m_iOrder, m_pTaps, ippButterworth, m_pBufIIRIIR);
		break;
	}
	if (ippStsNoErr != m_ippStatus)
	{
		ippsFree(m_pTaps);
		ippsFree(m_pBufIIRIIR);
		return -1;
	}
	/* init state structure for IIRIIR */
	m_ippStatus = ippsIIRIIRInit_64f(&m_pStateIIRIIR, m_pTaps, m_iOrder, NULL, m_pBufIIRIIR);
	if (ippStsNoErr != m_ippStatus)
	{
		ippsFree(m_pTaps);
		ippsFree(m_pBufIIRIIR);
		return -1;
	}
	return 0;
}


int CFilterIIR::DoFilter(double * dblOrgData, double * dblTargetData, int iLen)
{
	m_ippStatus = ippsIIRIIR_64f(dblOrgData, dblTargetData, iLen, m_pStateIIRIIR);
	if (ippStsNoErr != m_ippStatus)
		return -1;
	return 0;
}


int CFilterIIR::DoFilterInThePlace(double * dblOrgTargetData, int iLen)
{
	m_ippStatus = ippsIIRIIR_64f_I(dblOrgTargetData, iLen, m_pStateIIRIIR);
	if (ippStsNoErr != m_ippStatus)
		return -1;
	return 0;
}


void CFilterIIR::Free()
{
	if (m_pTaps)
	{
		ippsFree(m_pTaps);
		m_pTaps = NULL;
	}
	if (m_pBufIIRIIR)
	{
		ippsFree(m_pBufIIRIIR);
		m_pBufIIRIIR = NULL;
	}
}

