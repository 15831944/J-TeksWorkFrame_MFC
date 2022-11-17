#pragma once
#include "ITFBase.h"

class CFrame;
class CAlgoDialog;
class CAlgorithmBase;

class ITFBASEDLLDECL CParamsInterface
{
public:
	CParamsInterface(void);
	~CParamsInterface(void);
	virtual int DoTrial(CAlgoDialog *pAlgoDialog);
	virtual int DoApply(CAlgoDialog *pAlgoDialog);
	virtual int DoTrial(void);
	virtual int DoTrial(CFrame *pFrame);
	virtual int DoCompoTrial(CAlgoDialog * pSender);
	virtual int SetFrameAlgo(CAlgoDialog * pSender, bool bStatus);
	virtual int SetCompoAlgo(CAlgoDialog * pSender, bool bStatus);
	virtual int AddNewCompo(CPtrList *plstCompoRect, CPtrList *plstAlgo);
	virtual int DoAutoTeaching(CAlgoDialog * pSender);
};
