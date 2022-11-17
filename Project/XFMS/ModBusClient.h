#pragma once
#include <afxsock.h>
// CModBusClient 명령 대상입니다.
#define WM_REMOTECAM_RECV WM_USER + 20

#pragma pack(push, 1)
typedef struct
{
	WORD	wID;			// 0x04
	WORD	wProtocolID;	// 0x00
	BYTE	byDummy4;		// 0x00
	BYTE	byLenth;		// 0x6 MessageSize
	BYTE	byUnit;		    // 0x00
	BYTE	byFuncCode;		// 0x04
	WORD	wStartAddr;		// 0x00 Start Address
	BYTE	byDummy10;		// 0x00
	BYTE	byQuanOfReg;		//Quantity to Read Registers
} REQ_READ_INPUTREG;		// Request Read Input Register

typedef struct
{
	BYTE	byDummpy[9];	// 0;
	WORD	wTemp[6];		// 
} DATA_INPUTREG;		// Input Register Data  


#pragma pack(pop)

class CModBusClient : public CAsyncSocket
{
public:
	CModBusClient();
	virtual ~CModBusClient();
	virtual void OnConnect(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void Init(CWnd *pWnd, int nPort = 0);
	CModBusClient *m_pRecv;
	CWnd *m_pWnd;
	void SetRecvSocket(CModBusClient * pRecvSocket);
};


