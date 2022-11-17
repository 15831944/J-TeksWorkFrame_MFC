#include <ipp.h>
#include <ipps.h>
#pragma once
class CFilterIIR
{
public:
	CFilterIIR();
	~CFilterIIR();
	enum tFILTERTYPE { FT_LOWPASS, FT_HIGHPASS, FT_BANDPASS };
	double m_dblFS;
	int m_iOrder;
	double m_dblCutOffFreq;
	IppStatus m_ippStatus;
	int Init(double dblFS, double dblCutOffFreq, tFILTERTYPE iFilterType, int iOrder);
	int m_iBufferSizeGen;
	int m_iBufferSizeRIIR;
	Ipp8u *m_pBufIIRIIR;
	Ipp64f *m_pTaps;
	int DoFilter(double * dblOrgData, double * dblTargetData, int iLen);
	int DoFilterInThePlace(double * dblOrgTargetData, int iLen);
	IppsIIRState_64f *m_pStateIIRIIR;
	void Free();
};

