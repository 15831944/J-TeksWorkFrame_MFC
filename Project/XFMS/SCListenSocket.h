#pragma once
#include "afxcoll.h"
class CSCClientSocket;

// CSCListenSocket 명령 대상입니다.

class CSCListenSocket : public CAsyncSocket
{
public:
	CSCListenSocket();
	virtual ~CSCListenSocket();
	CPtrList m_lstClientSockList;
	virtual void OnAccept(int nErrorCode);
	void CloseClientSock(CSCClientSocket * pClient);
};


