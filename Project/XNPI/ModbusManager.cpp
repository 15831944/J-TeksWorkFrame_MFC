#include "stdafx.h"
#include "Status.h"
#include "ModbusManager.h"

CModbusManager* CModbusManager::m_instance = nullptr;
bool m_bIsSendLoopAct = false;

CModbusManager::CModbusManager()
	:m_bReadCmdRoopFlag(TRUE)
	, m_nCurrentRetryCnt(0)
	, m_bReconnectTryWhenSocketClosed(TRUE)
	, m_nStartTR_ID(-1)
	, m_nEndTR_ID(-1)
	, m_nTriggerTR_ID(-1)
{
	m_mbInstance = Modbus::GetInstance();
	m_mbInstance->m_unitId = 1;
	m_mbInstance->Create();
	m_mbInstance->SetOnCloseCallback(std::bind(&CModbusManager::SocketOnClose, this));
}

CModbusManager::~CModbusManager()
{
}

CModbusManager* CModbusManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new CModbusManager();
	}

	return m_instance;
}

//! PLC 서버에 접속이 끊겼을때 재접속을 시도한다. [1초간격으로 3번]
void CModbusManager::SocketOnClose()
{
	if (m_bReconnectTryWhenSocketClosed)
	{
		m_mbInstance->Close();
		m_mbInstance->Create();

		if (m_nCurrentRetryCnt >= MAX_RECONNECT_TRY_NUM)
		{
			AfxMessageBox(_T("ERROR: Disconnected from PLC server"));
		}
		else
		{
			m_nCurrentRetryCnt++;

			if (m_mbInstance->Connect(m_strIp, m_nPort))
			{
				m_nCurrentRetryCnt = 0;
			}
			else
			{
				Wait(RECONNECT_TRY_DELAY_MS);
				SocketOnClose();
			}
		}
	}
}

void CModbusManager::Wait(DWORD dw_MS)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dw_MS)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

UINT CModbusManager::ReadCmdSendRoopThread(LPVOID method)
{
	CModbusManager* pMm = (CModbusManager*)method;

	int cmdIndex = 1;
	while (TRUE)
	{
		while (pMm->m_bReadCmdRoopFlag)
		{
			m_bIsSendLoopAct = true;
			pMm->m_mbInstance->SetDataReceived(FALSE);

			switch (cmdIndex)
			{
			case START:
				pMm->m_nStartTR_ID = pMm->m_mbInstance->SendReadDataCmd(START);
				break;
			case END:
				pMm->m_nEndTR_ID = pMm->m_mbInstance->SendReadDataCmd(END);
				break;
			case TRIGGER:
				pMm->m_nTriggerTR_ID = pMm->m_mbInstance->SendReadDataCmd(TRIGGER);
				break;
			}

			while (!(pMm->m_mbInstance->GetDataReceived()))	//데이터 Rcv까지 대기
			{
				Sleep(1);
			}

			if (cmdIndex < 3)
				cmdIndex++;
			else
				cmdIndex = 1;

			Sleep(CStatus::Instance()->m_ulPlcReadCycleDelay_ms);
		}

		m_bIsSendLoopAct = false;
		Sleep(1);
	}

	return 0;
}

UINT CModbusManager::WriteCmdSendRoopThread(LPVOID method)
{
	CModbusManager* pMm = (CModbusManager*)method;
	pMm->m_bReadCmdRoopFlag = FALSE; //ReadCmdRoop 일시정지

	while (m_bIsSendLoopAct)	//ReadCmdSendRoopThread 끝날때까지 대기
	{
		Sleep(1);
	}

	pMm->m_mbInstance->SetDataReceived(FALSE);
	pMm->m_mbInstance->SendWriteDataCmd(pMm->m_wWriteCMDAddr, pMm->m_wWriteCMDData);

	while (!(pMm->m_mbInstance->GetDataReceived()))	//WriteCmdSendRoop 데이터 Rcv까지 대기
	{
		Sleep(1);
	}

	Sleep(CStatus::Instance()->m_ulPlcReadCycleDelay_ms);
	pMm->m_bReadCmdRoopFlag = TRUE;

	return 0;
}

BOOL CModbusManager::Connect(CString ip, int port)
{
	m_strIp = ip;
	m_nPort = port;

	return m_mbInstance->Connect(m_strIp, m_nPort);
}

void CModbusManager::SetOnReceiveCallback(tfOnReceive funcOnReceive)
{
	m_mbInstance->SetOnReceiveCallback(funcOnReceive);
}

void CModbusManager::ReadCmdRoopStart()
{
	if (m_pReadCmdThread != NULL)
		return;

	m_pReadCmdThread = AfxBeginThread(ReadCmdSendRoopThread, this);

	if (m_pReadCmdThread == NULL)
		AfxMessageBox(_T("m_pThread is NULL"));
}

void CModbusManager::ReadCmdRoopEnd()
{
	if (m_pReadCmdThread == NULL)
		return;

	m_mbInstance->SetDataReceived(FALSE);

	m_pReadCmdThread->SuspendThread();

	DWORD dwResult;
	::GetExitCodeThread(m_pReadCmdThread->m_hThread, &dwResult);

	delete m_pReadCmdThread;
	m_pReadCmdThread = NULL;
	m_bIsSendLoopAct = false;
}

void CModbusManager::WriteCmdData(WORD address, WORD writeData)
{
	m_wWriteCMDAddr = address;
	m_wWriteCMDData = writeData;

	//m_pWriteCmdThread = AfxBeginThread(WriteCmdSendRoopThread, this);

	//if (m_pWriteCmdThread == NULL)
	//	AfxMessageBox(_T("m_pThread is NULL"));

	m_mbInstance->SendWriteDataCmd(m_wWriteCMDAddr, m_wWriteCMDData);
}