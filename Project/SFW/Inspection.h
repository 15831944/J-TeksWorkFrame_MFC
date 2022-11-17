#pragma once
#include "Frame.h"
#include <ItfModelBase.h>
#include "afxcoll.h"


class CInspection : public CParamsInterface
{
public:
	CInspection(void);
	~CInspection(void);
	int Inspect(void);
	int Inspect(CFrame* pFrame);
	int LoadAlgorithm(CFrame * pFrame);
	int LoadAlgorithm(CAOIWin * pCompo);
	void SetMainApp(CWinApp * pMainApp);
	int AddAlgo2Frame(CFrame * pFrame);
	static CInspection * Instance(void);
	static CInspection *m_pInstance;
	CPtrList m_regUIAlgo;
	int UpdateAlgo2Frame(CFrame * pFrame);
	int AddAlgDlg(CAlgoDialog * pNewAlgoDlg);
	int DoTrial(void);
	int DoTrial(CAlgoDialog * pSender);
	int DoCompoTrial(CAlgoDialog * pSender);
	int DoTrial(CFrame *pFrame);
	int DoApply(CAlgoDialog * pSender);
	int DoCompoApply(CAlgoDialog * pSender);
	CMilBuffer *m_pInputImg;
	CFrame * GetCurrFrame(void);
	void SetInput(CMilBuffer * pMilBuffer);
	int NewObject(TCHAR * szModelName, CAlgorithmBase ** ppAlgorithm);
	int FindAlgo(CFrame * pFrame, CString strAlgoName, CAlgorithmBase ** ppAlgorithm);
	int FindAlgo(CAOIWin * pCompo, CString strAlgoName, CAlgorithmBase ** ppAlgorithm);
	int FindAlgo(CFrame * pFrame, CString strAlgoName, CPtrList *pLstAlgo);
	int DeleteAlgo(CFrame * pFrame, CString strAlgoName);
	int DeleteAlgo(CAOIWin * pCompo, CString strAlgoName);
	int AddAlgo2Compo(CAOIWin * pCompo);
	int GetUIParam(CAOIWin * pCompo);
	int SetFrameAlgo(CAlgoDialog * pSender, bool bStatus);
	int SetCompoAlgo(CAlgoDialog * pSender, bool bStatus);
	int Inspect(CAOIWin * pCompo);
	int AddNewCompo(CPtrList *plstCompoRect, CPtrList *plstAlgo);
	int DoAutoTeaching(CAlgoDialog * pSender);
};
