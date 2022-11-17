// SCListenSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "SCListenSocket.h"
#include "SCClientSocket.h"


// CSCListenSocket

CSCListenSocket::CSCListenSocket()
{
}

CSCListenSocket::~CSCListenSocket()
{
}


// CSCListenSocket 멤버 함수


void CSCListenSocket::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CSCClientSocket *pClient = new CSCClientSocket;
	if (Accept((*pClient)))
	{
//		pClient->SetRecvWnd(m_p);
		m_lstClientSockList.AddTail(pClient);
	}
	else
	{
		delete pClient;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}


void CSCListenSocket::CloseClientSock(CSCClientSocket * pClient)
{
	POSITION pos;
	pos = m_lstClientSockList.Find(pClient);
	if (pos != NULL)
	{
		if (pClient != NULL)
		{
			pClient->ShutDown();
			pClient->Close();

		}
		m_lstClientSockList.RemoveAt(pos);
		delete pClient;
	}
}
