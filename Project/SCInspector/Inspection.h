#pragma once
#include "Frame.h"
#include "AlgorithmBase.h"
#include "afxcoll.h"
#include "ParamsInterface.h"


class CInspection : public CParamsInterface
{
public:
	CInspection(void);
	~CInspection(void);
	int Inspect(void);
	int Inspect(CFrame* pFrame);
	int LoadAlgorithm(CFrame * pFrame);
	int AddAlgo2Frame(CFrame * pFrame);
	static CInspection * Instance(void);
	static CInspection *m_pInstance;
	CPtrList m_regUIAlgo;
	int UpdateAlgo2Frame(CFrame * pFrame);
	int AddAlgDlg(CAlgoDialog * pNewAlgoDlg);
	int DoTrial(void);
	int DoTrial(CFrame *pFrame);
	int DoApply(CAlgoDialog * pAlgoDialog);
	CFrame *m_pCurrFrame;
	CMilBuffer *m_pInputImg;
	CFrame * GetCurrFrame(void);
	void SetInput(CMilBuffer * pMilBuffer);
	int NewObject(TCHAR * szModelName, CAlgorithmBase ** ppAlgorithm);
	int FindAlgo(CFrame * pFrame, CString strAlgoName, CAlgorithmBase ** ppAlgorithm);
};
