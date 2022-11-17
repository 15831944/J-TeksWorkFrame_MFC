#pragma once

class CFrame;
class CAlgoDialog;

class CParamsInterface
{
public:
	CParamsInterface(void);
	~CParamsInterface(void);
	virtual int DoTrial(CAlgoDialog *pAlgoDialog);
	virtual int DoApply(CAlgoDialog *pAlgoDialog);
	virtual int DoTrial(void);
	virtual int DoTrial(CFrame *pFrame);
};
