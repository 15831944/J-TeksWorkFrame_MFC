#pragma once
#include <functional>
#include <iostream>
// CConnectSocket command target

typedef std::function<void()> tfSocketClosed;
typedef std::function<void(int, byte, int)> tfOnReceive;

class CConnectSocket : public CSocket
{
private:
	int m_nCurrentRetryCnt;
	tfSocketClosed m_pfOnclose;
	tfOnReceive m_pfOnReceive;
	int m_prev = 0;

public:
	CConnectSocket();
	virtual ~CConnectSocket();
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	
	BOOL m_bReceived;
	void SetOnCloseCallback(tfSocketClosed funcOnClose);
	void SetOnReceiveCallback(tfOnReceive funcOnReceive);

	int m_nTestCnt;
};


