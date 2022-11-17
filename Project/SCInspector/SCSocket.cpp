// SCSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "SCSocket.h"
#include "SCClientSocket.h"

// CSCSocket

CSCSocket::CSCSocket()
	: m_pHWnd(NULL)
	, m_pSockReceiver(NULL)
	, m_bConnected(FALSE)
	, m_iPortNo(0)
	, m_bVerbose(FALSE)
{
}

CSCSocket::~CSCSocket()
{
}


// CSCSocket 멤버 함수


void CSCSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CAsyncSocket::OnClose(nErrorCode);
	m_pHWnd->PostMessage(WM_SOCK_RECONNECT);
}


void CSCSocket::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_bConnected)
	{

	}
	else
	{
		m_pSockReceiver = new CSCClientSocket;
		if(Accept(*m_pSockReceiver))
		{
			m_pSockReceiver->SetRecvWnd(m_pHWnd);
		}
		m_bConnected = TRUE;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}


void CSCSocket::OnConnect(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_bVerbose)
		if (0 != nErrorCode)
		{
			switch (nErrorCode)
			{
			case WSAEADDRINUSE:
				AfxMessageBox(_T("The specified address is already in use.\n"));
				break;
			case WSAEADDRNOTAVAIL:
				AfxMessageBox(_T("The specified address is not available from ")
					_T("the local machine.\n"));
				break;
			case WSAEAFNOSUPPORT:
				AfxMessageBox(_T("Addresses in the specified family cannot be ")
					_T("used with this socket.\n"));
				break;
			case WSAECONNREFUSED:
				AfxMessageBox(_T("The attempt to connect was forcefully rejected.\n"));
				break;
			case WSAEDESTADDRREQ:
				AfxMessageBox(_T("A destination address is required.\n"));
				break;
			case WSAEFAULT:
				AfxMessageBox(_T("The lpSockAddrLen argument is incorrect.\n"));
				break;
			case WSAEINVAL:
				AfxMessageBox(_T("The socket is already bound to an address.\n"));
				break;
			case WSAEISCONN:
				AfxMessageBox(_T("The socket is already connected.\n"));
				break;
			case WSAEMFILE:
				AfxMessageBox(_T("No more file descriptors are available.\n"));
				break;
			case WSAENETUNREACH:
				AfxMessageBox(_T("The network cannot be reached from this host ")
					_T("at this time.\n"));
				break;
			case WSAENOBUFS:
				AfxMessageBox(_T("No buffer space is available. The socket ")
					_T("cannot be connected.\n"));
				break;
			case WSAENOTCONN:
				AfxMessageBox(_T("The socket is not connected.\n"));
				break;
			case WSAENOTSOCK:
				AfxMessageBox(_T("The descriptor is a file, not a socket.\n"));
				break;
			case WSAETIMEDOUT:
				AfxMessageBox(_T("The attempt to connect timed out without ")
					_T("establishing a connection. \n"));
				break;
			default:
				TCHAR szError[256];
				_stprintf_s(szError, _T("OnConnect error: %d"), nErrorCode);
				AfxMessageBox(szError);
				break;
			}
			AfxMessageBox(_T("Please close the application"));
		}

	CAsyncSocket::OnConnect(nErrorCode);
}


void CSCSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_pHWnd->SendMessage(WM_CAM_REMOTE, (WPARAM)this);
	CAsyncSocket::OnReceive(nErrorCode);
}


void CSCSocket::Init(CWnd * pWnd, int iPortNo)
{
	m_bConnected = FALSE;
	m_pHWnd = pWnd;
	m_iPortNo = iPortNo;
	Create(iPortNo);
}


void CSCSocket::CloseClient()
{
	m_bConnected = FALSE;
	m_pSockReceiver->ShutDown();
	m_pSockReceiver->Close();
	delete m_pSockReceiver;
	m_pSockReceiver = NULL;
}
