#pragma once
#include "Modbus.h"
#include <functional>

//PLC -> PC
#define START		0001
#define END			0002
#define TRIGGER		0003

//PC -> PLC
#define RESET			0001
#define PROC_DONE		0002
#define TRIGGER_REPEAT	0003

//#define THREAD_POLLING_DALAY_MS			50
#define THREAD_RECEIVE_WAIT_DELAY_MS	3000

#define MAX_RECONNECT_TRY_NUM	3
#define RECONNECT_TRY_DELAY_MS	250

class CModbusManager
{
private:
	CModbusManager();
	Modbus* m_mbInstance;
	CString m_strIp;
	int	m_nPort;
	int m_nCurrentRetryCnt;
	static CModbusManager* m_instance;
	static UINT ReadCmdSendRoopThread(LPVOID method);
	static UINT WriteCmdSendRoopThread(LPVOID method);
	CWinThread* m_pReadCmdThread;
	CWinThread* m_pWriteCmdThread;
	BOOL m_bReadCmdRoopFlag;
	WORD m_wWriteCMDAddr;
	WORD m_wWriteCMDData;
	void SocketOnClose();
	void Wait(DWORD dw_MS);
	BOOL m_bReconnectTryWhenSocketClosed;
public:
	~CModbusManager();
	static CModbusManager* GetInstance();

	BOOL Connect(CString ip, int port = 502);
	void ReadCmdRoopStart();
	void WriteCmdData(WORD address, WORD writeData);
	void SetOnReceiveCallback(tfOnReceive funcOnReceive);
	void ReadCmdRoopEnd();

	int m_nStartTR_ID;
	int m_nEndTR_ID;
	int m_nTriggerTR_ID;
};

