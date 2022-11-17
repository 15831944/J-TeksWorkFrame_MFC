// SCClientSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XFMS.h"
#include "SCClientSocket.h"


// CSCClientSocket

CSCClientSocket::CSCClientSocket()
	: m_pWnd(NULL)
{
}

CSCClientSocket::~CSCClientSocket()
{
}


// CSCClientSocket 멤버 함수


void CSCClientSocket::SetRecvWnd(CWnd * pWnd)
{
	m_pWnd = pWnd;
}


void CSCClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_pWnd->SendMessage(WM_CAM_REMOTE, (WPARAM)this);
	CSocket::OnReceive(nErrorCode);
}


void CSCClientSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_pWnd->PostMessage(WM_SOCK_LISTEN);
	CSocket::OnClose(nErrorCode);
}
