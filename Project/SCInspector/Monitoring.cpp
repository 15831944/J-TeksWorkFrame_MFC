#include "StdAfx.h"
#include "Monitoring.h"
#include "Status.h"
#include "SCInspIO.h"
#include "MessageWnd.h"

BOOL CMonitoring::m_bRun=NULL;
CMonitoring* CMonitoring::m_pInstance=NULL;
CMonitoring::CMonitoring(void)
{
}

CMonitoring::~CMonitoring(void)
{
}

UINT CMonitoring::ThreadMonitoring(LPVOID pParam)
{
	static BOOL bInSensor=FALSE;
	static HANDLE hInSensorTime=NULL;
	CSCInspIO *pSCInspIO = CSCInspIO::Instance();
	
	m_bRun=TRUE;
	while(m_bRun)
	{
		//EMG, ERR사용하지 않음.
		//int nEMGStage = -1;
		//if (pSCInspIO->GetEMG(&nEMGStage))
		//{
		//	CMessageWnd::Instance()->ShowWindow(SW_SHOW);
		//}

		//int nERRStage = -1;
		//if (pSCInspIO->GetErr(&nERRStage))
		//{
		//	CMessageWnd::Instance()->ShowWindow(SW_SHOW);
		//}

	}

	return 0;
}

int CMonitoring::Start(void)
{
	AfxBeginThread(ThreadMonitoring, AfxGetMainWnd());
	return 0;
}

CMonitoring* CMonitoring::Instance(void)
{
	if(!m_pInstance)m_pInstance=new CMonitoring;
	return m_pInstance;
}

void CMonitoring::Stop(void)
{
	m_bRun=FALSE;
}

