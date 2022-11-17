// ConnectSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ConnectSocket.h"

CConnectSocket::CConnectSocket()
	: m_nCurrentRetryCnt(0)
	, m_bReceived(FALSE)
	, m_nTestCnt(0)
{
}

CConnectSocket::~CConnectSocket()
{
}

void CConnectSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	if (m_pfOnclose != NULL)
		m_pfOnclose();
}

void CConnectSocket::OnReceive(int nErrorCode)
{
	m_nTestCnt--;

	int rcvValueResult = 0;
	byte rcvBuffer[30];
	::ZeroMemory(rcvBuffer, sizeof(rcvBuffer));

	int received = Receive(rcvBuffer, sizeof(rcvBuffer));
	if (received > 0)
	{
		//! 11은 PLC의 input register 데이터를 읽기 요청했을때의 응답, 12는  holding register에 쓰기했을때 응답  - kiy
		if (received == 11 || received == 12)
		{

			WORD trID = MAKEWORD(rcvBuffer[1], rcvBuffer[0]);
			byte fCode = rcvBuffer[7];

			if (m_pfOnReceive != NULL)
			{
				if (fCode == 0x04 && received == 11) //! input register Function Code == 0x04  - kiy
				{
					byte len = rcvBuffer[8]; //! 받은 데이터의 길이가 보관된 배열  - kiy

					for (int i = 0; i < len; i++)
						rcvValueResult += rcvBuffer[8 + i + 1] * (int)(pow(16, (len - (i * 2))));

					m_pfOnReceive(trID, fCode, rcvValueResult);
				}
				else if (fCode == 0x06 && received == 12) //! holding register Function Code == 0x06  - kiy
				{
					//! PLC에 쓴 데이터는 확인할 필요 없으나 디버깅을 위해 남겨둠  - kiy
					rcvValueResult = rcvBuffer[11];
					m_pfOnReceive(trID, fCode, rcvValueResult);
				}
				else
				{
					//! 4, 6외의 값이 들어왔다면 오류가 있는것 [4, 6은 Function Code를 의미]  - kiy
					CString str;
					str.Format(_T("ERR OnReceive FCODE : %d"), fCode);
					AfxMessageBox(str);
				}
			}
		}
		//! 11, 12가 아닌경우는 비정상, 23개씩 올때 있음, 비정상인 경우에는 해당 Receive는 건너뜀.  - kiy
		else
		{
			std::wcout << "ERR OnReceive : " << received << std::endl;
		}
	}

	m_bReceived = TRUE;
	CSocket::OnReceive(nErrorCode);
}

void CConnectSocket::SetOnCloseCallback(tfSocketClosed funcOnClose)
{
	m_pfOnclose = std::move(funcOnClose);
}

void CConnectSocket::SetOnReceiveCallback(tfOnReceive funcOnReceive)
{
	m_pfOnReceive = std::move(funcOnReceive);
}
