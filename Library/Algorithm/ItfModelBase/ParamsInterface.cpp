#include "StdAfx.h"
#include "ParamsInterface.h"

CParamsInterface::CParamsInterface(void)
{
}

CParamsInterface::~CParamsInterface(void)
{
}

int CParamsInterface::DoTrial(CAlgoDialog *pAlgoDialog)
{
	return 0;
}

int CParamsInterface::DoApply(CAlgoDialog *pAlgoDialog)
{
	return 0;
}

int CParamsInterface::DoTrial(void)
{
	return 0;
}

int CParamsInterface::DoTrial(CFrame *pFrame)
{
	return 0;
}

int CParamsInterface::DoCompoTrial(CAlgoDialog * pSender)
{
	return 0;
}

int CParamsInterface::SetFrameAlgo(CAlgoDialog * pSender, bool bStatus)
{
	return 0;
}

int CParamsInterface::SetCompoAlgo(CAlgoDialog * pSender, bool bStatus)
{
	return 0;
}

int CParamsInterface::AddNewCompo(CPtrList *plstCompoRect, CPtrList *plstAlgo)
{
	return 0;
}

int CParamsInterface::DoAutoTeaching(CAlgoDialog * pSender)
{
	return 0;
}
