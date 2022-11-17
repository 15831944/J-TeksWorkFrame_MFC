#include "StdAfx.h"
#include "SCProcessCtrl.h"
#include "Status.h"
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

	int nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	if (nCountOfCam == 0) return 0;
	m_nCountOfCam = nCountOfCam;

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


int CSCProcessCtrl::Stop()
{
	m_Run = FALSE;
	return 0;
}


int CSCProcessCtrl::DoInspect(int nStage, int nPos, BOOL bSave)
{
	int nCountOfCam = CStatus::Instance()->m_nCountOfCam;

	BOOL flagNG = FALSE;

	for (int i = 0; i < nCountOfCam; i++)
	{
		m_pCamDlg[i]->Grab();
	}
	for (int i = 0; i < nCountOfCam; i++)
	{
		while (!m_pCamDlg[i]->IsGrabDone());
		if (!(m_pCamDlg[i]->Inspect()))
		{
			//Good
		}
		else
		{
			//NG
			if(bSave)m_pCamDlg[i]->SaveImage();
			flagNG = TRUE;
		}
	}

	return 0;
}
