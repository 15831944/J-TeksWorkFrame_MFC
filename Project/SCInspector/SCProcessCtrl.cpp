#include "StdAfx.h"
#include "SCProcessCtrl.h"
#include "Status.h"
#include "SCInspIO.h"
#include "CamDlg.h"
#include "TabDetailLogDlg.h"
#include "SCResult.h"
#include "Repository.h"

CSCProcessCtrl* CSCProcessCtrl::m_pInstance=NULL;

CSCProcessCtrl::CSCProcessCtrl(void)
: m_Run(0)
, m_nCountOfCam(0)
{
}

CSCProcessCtrl::~CSCProcessCtrl(void)
{
}

int CSCProcessCtrl::CheckAndDo(void)
{
	static int iStep=0;
	static int InScan=FALSE;
	static int StageStep[4];

	if(CStatus::OPAUTO != CStatus::Instance()->GetOpMode())
	{
		iStep = 0;
		return 0;
	}

	if (iStep == 0)
	{
		for (int i = 0; i < 4; i++)
			StageStep[i] = 0;
		iStep = 1;
	}

	int nCountOfStage = CStatus::Instance()->m_nCountOfStage;
	int nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	if (nCountOfCam == 0) return 0;
	m_nCountOfCam = nCountOfCam;
	CSCInspIO *pSCInspIO = CSCInspIO::Instance();
	pSCInspIO->m_nCountOfStage = nCountOfStage;
	
	
	for (int i = 0; i < nCountOfStage; i++)
	{
		StageProcess(i, &StageStep[i]);
	}
	return 0;
}

HANDLE CSCProcessCtrl::MakeWaitableTimer(int setTime)
{
	if(m_hWaitableTimer==NULL)
		m_hWaitableTimer=CreateWaitableTimer(NULL, FALSE, NULL);

	LARGE_INTEGER myLI;
	myLI.QuadPart =-1 * setTime *10000;
	if(!SetWaitableTimer(m_hWaitableTimer, &myLI, 0,  NULL, NULL, 0))
	{
		TRACE("Waitable Timer Set Error");
		return NULL;
	}

	return m_hWaitableTimer;
}

CSCProcessCtrl* CSCProcessCtrl::Instance(tSCInspMachineType nType)
{
	if (m_pInstance)return m_pInstance;
	switch(nType)
	{
	case MA_1:
	if(!m_pInstance)m_pInstance=new CSCProcessCtrl;
		break;
	default:
		if (!m_pInstance)m_pInstance = new CSCProcessCtrl;
		break;
	}

	return m_pInstance;
}


UINT CSCProcessCtrl::ThreadProc(LPVOID pParam)
{
		CSCProcessCtrl *pProcessCtrl=(CSCProcessCtrl *)pParam;

		while(pProcessCtrl->m_Run)
		{
			pProcessCtrl->CheckAndDo();
			Sleep(1);
		}
		return 0;
}

int CSCProcessCtrl::InitThread(void)
{
	m_Run=TRUE;
//	SetConvMode(CONV_STOP);
	AfxBeginThread(ThreadProc, (LPVOID)this);
	return 0;
}


void CSCProcessCtrl::ManStageProcess(int nStage, int * piStageStep)
{
	CSCInspIO *pSCInspIO = CSCInspIO::Instance();
	if (!pSCInspIO->IsManual(nStage))
	{
		*piStageStep = 0;
	}

	switch (*piStageStep)
	{
	case 0:
		pSCInspIO->SetVisionReady(nStage, TRUE);  //SetVision Ready
		for (int i = 0; i < 6; i++)
		{
			pSCInspIO->SetCapDone(nStage, i, FALSE);
			pSCInspIO->SetNG(nStage, i, FALSE);
		}
		pSCInspIO->SetInspDone(nStage, FALSE);
		*piStageStep = 1;
		break;
	case 1:
		if (pSCInspIO->IsUnitStart(nStage))
		{
			*piStageStep = 2;
		}
		break;
	case 2:
		if (IsExitCdt(nStage))
		{
			*piStageStep = 0;
		}
		int FindedPos;
		FindedPos= -1;
		if (pSCInspIO->GetMotionDone(nStage, &FindedPos))
		{
			DoInspect(nStage, FindedPos, FALSE);
			pSCInspIO->SetInspDone(nStage, TRUE);
			*piStageStep = 3;
		}

		break;
	case 3:
		if (!pSCInspIO->IsUnitStart(nStage))
		{
			*piStageStep = 0;
		}
		break;
	}

}


void CSCProcessCtrl::AutoStageProcess(int nStage, int *piStageStep)
{
	CSCInspIO *pSCInspIO = CSCInspIO::Instance();
	static int nProcPos[4] = { 0,0,0,0 };
	int nCountOfPos = CStatus::Instance()->m_nCountOfPos;

	if (pSCInspIO->IsManual(nStage))
	{
		*piStageStep = 0;
	}

	switch (*piStageStep)
	{
	case 0:
		pSCInspIO->SetInspDone(nStage, FALSE);
		for (int i = 0; i < nCountOfPos; i++)
		{
			pSCInspIO->SetCapDone(nStage, i, FALSE);
			pSCInspIO->SetNG(nStage, i, FALSE);
		}
		pSCInspIO->SetVisionReady(nStage, TRUE);  //SetVision Ready
		nProcPos[nStage] = 0;
		if (pSCInspIO->IsRun(nStage))*piStageStep = 1;
		break;
	case 1:
		*piStageStep = 2;
		break;
	case 2:
		if (pSCInspIO->IsUnitStart(nStage))
		{
//			pSCInspIO->SetVisionReady(nStage, FALSE);  //SetVision Ready
			*piStageStep = 3;
			nProcPos[nStage] = 0;
		}
		break;
	case 3:
		if (IsExitCdt(nStage))
		{
			*piStageStep = 0;
		}

		if (pSCInspIO->IsMotionDone(nStage, nProcPos[nStage]))
		{
			DoInspect(nStage, nProcPos[nStage], TRUE);
			nProcPos[nStage]++;
		}
		if (nProcPos[nStage] == nCountOfPos)
		{
			nProcPos[nStage] = 0;
			*piStageStep = 4;
		}
		break;
	case 4:
		pSCInspIO->SetInspDone(nStage, TRUE);
		*piStageStep = 5;
		break;
	case 5:
		if (!pSCInspIO->IsUnitStart(nStage))
		{
			*piStageStep = 0;
		}
		break;
	}
}

void CSCProcessCtrl::StageProcess(int nStage, int *piStageStep)
{
	static BOOL PrevStageSatus[4] = { FALSE,FALSE,FALSE,FALSE }; //FALSE-->Auto TRUE-->Man
	CSCInspIO *pSCInspIO = CSCInspIO::Instance();

	BOOL bCurStageStatus= pSCInspIO->IsManual(nStage);
	if (PrevStageSatus[nStage] != bCurStageStatus)
	{
		*piStageStep = 0;//Mode가 바뀌면 초기화
		StageProcessClear(nStage);
		CString strMsg;
		if (bCurStageStatus)
			strMsg.Format(_T("Stage %d:Manual Mode" ), nStage);
		else
			strMsg.Format(_T("Stage %d:Auto Mode"), nStage);
		CTabDetailLogDlg::Instance()->SetTextLog(strMsg.GetBuffer(), RGB(50, 50, 50), FALSE);
	}
	PrevStageSatus[nStage] = bCurStageStatus;  

	if (bCurStageStatus)
	{
		ManStageProcess(nStage, piStageStep);
	}
	else
	{
		AutoStageProcess(nStage, piStageStep);
	}
}

int CSCProcessCtrl::IsExitCdt(int nStage)
{
	CSCInspIO *pSCInspIO = CSCInspIO::Instance();
	return 0;
}


int CSCProcessCtrl::Stop()
{
	m_Run = FALSE;
	return 0;
}



void CSCProcessCtrl::StageProcessClear(int nStage)
{
	CSCInspIO *pSCInspIO = CSCInspIO::Instance();

	pSCInspIO->SetVisionReady(nStage, FALSE);  //Reset Vision Ready
	for (int i = 0; i < 6; i++)
	{
		pSCInspIO->SetCapDone(nStage, i, FALSE);
		pSCInspIO->SetNG(nStage, i, FALSE);
	}
	pSCInspIO->SetInspDone(nStage, FALSE);
}

int CSCProcessCtrl::DoInspect(int nStage, int nPos, BOOL bSave)
{
	CSCInspIO *pSCInspIO = CSCInspIO::Instance();
	
	int nCountOfCam = CStatus::Instance()->m_nCountOfCam;

	BOOL flagNG = FALSE;

	for (int i = 0; i < nCountOfCam; i++)
	{
		if (m_pCamDlg[i]->m_inspStage != nStage)continue;
		m_pCamDlg[i]->Grab();
	}
	for (int i = 0; i < nCountOfCam; i++)
	{
		if (m_pCamDlg[i]->m_inspStage != nStage)continue;
		while (!m_pCamDlg[i]->IsGrabDone());
		if (!(m_pCamDlg[i]->Inspect()))
		{
			//Good
			if(CStatus::Instance()->m_bSaveAllImage)
				if (bSave)m_pCamDlg[i]->SaveImage(FALSE);
		}
		else
		{
			//NG
			if(bSave)m_pCamDlg[i]->SaveImage();
			flagNG = TRUE;
		}
	}

	if (flagNG)
		pSCInspIO->SetNG(nStage, nPos, TRUE);
	else
		pSCInspIO->SetNG(nStage, nPos, FALSE);
	pSCInspIO->SetCapDone(nStage, nPos, TRUE);

	return 0;
}
