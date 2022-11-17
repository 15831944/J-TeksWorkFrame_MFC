#pragma once

// CSCSocket 명령 대상입니다.
class CSCClientSocket;

class CSCSocket : public CAsyncSocket
{
public:
	CSCSocket();
	virtual ~CSCSocket();
	virtual void OnClose(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	CWnd *m_pHWnd;
	void Init(CWnd * pWnd, int iPortNo=0);
	CSCClientSocket *m_pSockReceiver;
	BOOL m_bConnected;
	int m_iPortNo;
	BOOL m_bVerbose;
	void CloseClient();
};


