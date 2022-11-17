#include "XFMSPacket.h"
#include "Serial.h"

#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <tchar.h>
#include <mmsystem.h>
#pragma comment(lib, "Setupapi")
#pragma comment(lib, "winmm.lib")

IMPLEMENT_DYNAMIC(CXFMSPacket, CObject);

CXFMSPacket::CXFMSPacket(CSerial& serial, LONG lTimeout)
	: m_serial(serial)
	, m_lTimeout(lTimeout)
	, m_bIsMeaInfo(FALSE)
	, m_pWProcessThread(NULL)
	, m_bTerminateThread(FALSE)
	, m_bReceiveCMD(FALSE)
	, m_nMaxRcvSize(MAX_BUF_SIZE)
{
	if (m_nMaxRcvSize > MAX_BUF_SIZE)m_nMaxRcvSize = MAX_BUF_SIZE;
}


CXFMSPacket::~CXFMSPacket()
{
}

BYTE CXFMSPacket::CalcChecksum(const BYTE* pbyData, int nSize)
{
	if (nSize < 2)
		return 0;

	BYTE byChecksum = 0;

	for (int i = 0; i < nSize - 1; i++)
		byChecksum += pbyData[i];

	return (0);
}

BYTE CXFMSPacket::DVCProcessComBuffer()
{
	::ZeroMemory(m_byBuffer, sizeof(m_byBuffer));

	BYTE byBuffer[MAX_BUF_SIZE];
	BYTE byETX = 0;

	LONG lStartTime = timeGetTime();

	// Read STX byte.
	int nBytesToRead = 1;
	DWORD dwRead = 0;

	m_serial.Read(byBuffer, nBytesToRead, &dwRead);
	if (dwRead == nBytesToRead)
	{
		switch (byBuffer[0])
		{
		case SENSOR_INFO:
			nBytesToRead = sizeof(PC_PACKET_SENSORINFO) - 1;
			byETX = ETX_RESP;
			break;
		case STX_SENSOR:
			nBytesToRead = sizeof(PC_PACKET_SENSORINFO) - 1;
			byETX = ETX_RESP;
			break;
		default:	// Clean up & return.
			do
			{
				m_serial.Read(byBuffer, MAX_BUF_SIZE, &dwRead);
			} while (MAX_BUF_SIZE == (int)dwRead);

			return 0;
		}
	}

	int nBytesRead = 1;
	do
	{
		m_serial.Read(&byBuffer[nBytesRead], nBytesToRead, &dwRead);
		nBytesRead += dwRead;
		nBytesToRead -= dwRead;
	} while (nBytesToRead > 0 && ::timeGetTime() < (DWORD)(lStartTime + m_lTimeout));

	if (nBytesToRead > 0)	// Timeout 
		return 0;

	if (byBuffer[nBytesRead - 1] != byETX)		// Corrupted
		return 0;

	BYTE byChecksum = CalcChecksum(byBuffer, nBytesRead);
	if (byChecksum != 0)		// Checksum error
		return 0;

	if (byBuffer[0] == STX_SENSOR)
	{
		::CopyMemory(m_byBuffer, byBuffer, sizeof(PC_PACKET_MEAINFO));
	}
	else // SENSOR_INFO
	{
		::CopyMemory(m_byBuffer, byBuffer, sizeof(PC_PACKET_SENSORINFO));
	}

	// Clean up
	do
	{
		m_serial.Read(byBuffer, MAX_BUF_SIZE, &dwRead);
	} while (MAX_BUF_SIZE == (int)dwRead);

	BYTE outData;
	outData = m_byBuffer[0];

	return outData;
}

int CXFMSPacket::AnalyzeData()
{
	int count = (int)m_rcvDeque.size();
	//printf("\nBuffer size:%d", count);
	for (int i = 0; i < count; i++)
	{
		count = (int)m_rcvDeque.size();
		BYTE byCurr = m_rcvDeque.at(0);
		switch (byCurr)
		{
		case SENSOR_INFO:
			if (count >= sizeof(DVC_PACKET_SENSORINFO))
			{
				if (ETX_RESP == m_rcvDeque.at(sizeof(DVC_PACKET_SENSORINFO) - 1))
					return SENSOR_INFO;
				else
					m_rcvDeque.pop_front();
			}

			break;
		case STX_SENSOR:
			if (count >= sizeof(DVC_PACKET_MEAINFO))
			{
				if (ETX_RESP == m_rcvDeque.at(sizeof(DVC_PACKET_MEAINFO) - 1))
					return STX_SENSOR;
				else
					m_rcvDeque.pop_front();
			}

			break;

		default:
			m_rcvDeque.pop_front();
		}

	}
	return RCV_NOCMD;
}

BYTE CXFMSPacket::ProcessComBuffer()
{
#ifdef _DEVICE_EMULATOR_
	return DVCProcessComBuffer();
#endif

	::ZeroMemory(m_byBuffer, sizeof(m_byBuffer));

	BYTE rcvData[MAX_BUF_SIZE];
	BYTE byETX = 0;

	LONG lStartTime = timeGetTime();

	// Read STX byte.
	DWORD dwRead = 0;
	int dwReadMax = 0;

	while (true)
	{
		LONG lResult = m_serial.Read(rcvData, m_nMaxRcvSize, &dwRead);
		if (lResult != ERROR_SUCCESS)
		{
			TRACE(_T("m_serial.Read error : %ld\n"), lResult);
			break;
		}

		if (!(dwRead > 0))
		{
			break;
		}


		dwReadMax += dwRead;
		TRACE(_T("dwRead:%d, dwReadMax:%d\n"), dwRead, dwReadMax);

		for (int i = 0; i < (int)dwRead; i++)
			m_rcvDeque.push_back(rcvData[i]);

	}

	switch (AnalyzeData())
	{
	case SENSOR_INFO:
		return SENSOR_INFO;
		break;
	case STX_SENSOR:
		return STX_SENSOR;
		break;
	case RCV_NOCMD:
		return RCV_NOCMD;
		break;
	}

	return RCV_NOCMD;
}

CXFMSPacket::EPacketType CXFMSPacket::CheckoutPacket()
{
	EPacketType eType = EPacketType::None;

	BYTE bySTX = ProcessComBuffer();

	BYTE byTEST = bySTX;
	if (bySTX != 0)
	{
#ifdef _DEVICE_EMULATOR_
		if (bySTX == SENSOR_INFO)
		{
			eType = EPacketType::SensorInfo;
		}
		else
		{
			eType = EPacketType::Measure;
		}
		//PC_PACKET_CTRL* pPacket = (PC_PACKET_CTRL*)m_byBuffer;

		//pPacket->wPurgeTime = (WORD)::ntohs(pPacket->wPurgeTime);
		//pPacket->wMeasureTime = (WORD)::ntohs(pPacket->wMeasureTime);

#else
		switch (bySTX)
		{
		case STX_SENSOR:
			//			DVC_PACKET_MEAINFO* pPacket = (DVC_PACKET_MEAINFO*)m_byBuffer;
			eType = EPacketType::Measure;
			break;
		case SENSOR_INFO:
			eType = EPacketType::SensorInfo;
			//			DVC_PACKET_SENSORINFO* pPacket = (DVC_PACKET_SENSORINFO *)m_byBuffer;
			break;
		}
#endif
	}

	return eType;
}

int CXFMSPacket::MakeProcessThread()
{
	m_bTerminateThread = FALSE;
	m_pWProcessThread = AfxBeginThread(ProcessThread, this);
	if (!m_pWProcessThread)
		return 0;
	return -1;
}


UINT CXFMSPacket::ProcessThread(LPVOID pSender)
{
	return 0;
}



int CXFMSPacket::GetQueuePacket(BYTE * pPacket, CXFMSPacket::EPacketType tCmdType)
{
	int sizePacket;

	switch (tCmdType)
	{
	case EPacketType::SensorInfo:
		sizePacket = sizeof(DVC_PACKET_SENSORINFO);
		for (int i = 0; i < sizePacket; i++)
		{
			pPacket[i] = m_rcvDeque.front();
			m_rcvDeque.pop_front();
		}
		break;
	case EPacketType::Measure:
		sizePacket = sizeof(DVC_PACKET_MEAINFO);
		for (int i = 0; i < sizePacket; i++)
		{
			pPacket[i] = m_rcvDeque.front();
			m_rcvDeque.pop_front();
		}
		break;
	}
	return 0;
}



int CXFMSPacket::LightOnOff(tLightStatus lsStatus, long *pBrightness)
{
	char chCmd;
	if (lsStatus == LIGHT_ON)
		chCmd = 'o';
	else
		chCmd = 'f';

	for (int i = 0; i < 4; i++)
	{
		if (chCmd == 'o')
			SendLightCMD(i, chCmd, pBrightness[i]);
		else
			SendLightOnOffCMD(i, chCmd);
	}
	return 0;
}

int CXFMSPacket::LightOnOff(int ch, tLightStatus lsStatus)
{
	char chCmd;
	if (lsStatus == LIGHT_ON)
		chCmd = 'o';
	else
		chCmd = 'f';

	SendLightOnOffCMD(ch, chCmd);
	return 0;
}


int CXFMSPacket::SendLightBrightness(int pBrightness[4])
{
	char chCmd = 'w';

	for (int i = 0; i < 4; i++)
		SendLightCMD(i, chCmd, pBrightness[i]);

	return 0;
}

int CXFMSPacket::SendLightBrightness(int ch, int iBrightness)
{
	char chCmd = 'w';

	SendLightCMD(ch, chCmd, iBrightness);

	return 0;
}


//kiy------------------------------
int CXFMSPacket::SendSetXrayOnOff(BOOL xrayOn)
{
	//모드 400으로 변경
	PC_PACKET_SELECT_M400 m400;
	m_serial.Write((const void*)&m400, sizeof(PC_PACKET_SELECT_M400));

	PC_PACKET_XRAYONOFF packet;
	packet.byStart = STX;
	packet.byCR = CARRAGE_RETURE;
	packet.byEnd = ETX;

	if (xrayOn)
	{
		packet.byOnOffCmd[0] = 'O';
		packet.byOnOffCmd[1] = 'N';
	}
	else
	{
		packet.byOnOffCmd[0] = 'O';
		packet.byOnOffCmd[1] = 'F';
	}

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_XRAYONOFF)) == ERROR_SUCCESS);
}

int CXFMSPacket::SendSetFocalSpot(BOOL SetStandardSize)
{
	//모드 400으로 변경
	PC_PACKET_SELECT_M400 m400;
	m_serial.Write((const void*)&m400, sizeof(PC_PACKET_SELECT_M400));

	PC_PACKET_FOCAL_SPOT_SIZE packet;
	packet.byStart = STX;
	packet.byCR = CARRAGE_RETURE;
	packet.byEnd = ETX;

	if (SetStandardSize)
	{
		packet.byFocalSize[0] = 'F';//Large
		packet.byFocalSize[1] = '0'; //숫자 0임
	}
	else
	{
		packet.byFocalSize[0] = 'F'; //Small
		packet.byFocalSize[1] = '1';
	}

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_FOCAL_SPOT_SIZE)) == ERROR_SUCCESS);
}

int CXFMSPacket::SendSetKvCMD(double kv)
{
	//모드 400으로 변경
	PC_PACKET_SELECT_M400 m400;
	m_serial.Write((const void*)&m400, sizeof(PC_PACKET_SELECT_M400));

	int nkv = kv * 10;

	BYTE byStart = 0x02;
	BYTE byCmdLetter = CMD_KV;
	BYTE byCR = 0x0d;
	BYTE byEnd = 0x03;

	char tempBuffer[30];
	int len = sprintf_s(tempBuffer, "%d", nkv);
	int kvBufferSize = len * sizeof(BYTE);
	int resultBufferrSize = kvBufferSize + sizeof(BYTE) * 4;

	BYTE*  kvBuffer = (BYTE*)malloc(kvBufferSize);
	BYTE*  resultBuffer = (BYTE*)malloc(resultBufferrSize);

	for (int i = 0; i < len; i++)
	{
		kvBuffer[i] = tempBuffer[i];
	}

	int j = 2;
	resultBuffer[0] = byStart;
	resultBuffer[1] = byCmdLetter;

	for (j = 2; j < len + 2; j++)
	{
		resultBuffer[j] = kvBuffer[j - 2];
	}

	resultBuffer[j] = byCR;
	resultBuffer[j + 1] = byEnd;

	LONG err = m_serial.Write((const void*)resultBuffer, resultBufferrSize);

	free(resultBuffer);
	free(kvBuffer);

	return err;
}

int CXFMSPacket::SendSetMaCmd(double ma)
{
	int nMa = ma * 100;

	if (nMa > 60)
	{
		PC_PACKET_SELECT_M400 m400;
		m_serial.Write((const void*)&m400, sizeof(PC_PACKET_SELECT_M400));
	}
	else //0 ~ 6.00 mA 범위는 모드401
	{
		PC_PACKET_SELECT_M401 m401;
		m_serial.Write((const void*)&m401, sizeof(PC_PACKET_SELECT_M401));
	}

	BYTE byStart = 0x02;
	BYTE byCmdLetter = CMD_MA;
	BYTE byCR = 0x0d;
	BYTE byEnd = 0x03;

	char tempBuffer[30];
	int len = sprintf_s(tempBuffer, "%d", nMa);
	int maBufferSize = len * sizeof(BYTE);
	int resultBufferrSize = maBufferSize + sizeof(BYTE) * 4;

	BYTE*  maBuffer = (BYTE*)malloc(maBufferSize);
	BYTE*  resultBuffer = (BYTE*)malloc(resultBufferrSize);

	for (int i = 0; i < len; i++)
	{
		maBuffer[i] = tempBuffer[i];
	}

	int j = 2;
	resultBuffer[0] = byStart;
	resultBuffer[1] = byCmdLetter;

	for (j = 2; j < len + 2; j++)
	{
		resultBuffer[j] = maBuffer[j - 2];
	}

	resultBuffer[j] = byCR;
	resultBuffer[j + 1] = byEnd;

	LONG err = m_serial.Write((const void*)resultBuffer, resultBufferrSize);

	free(resultBuffer);
	free(maBuffer);

	return err;
}
//-----------------------------------

int CXFMSPacket::SendLightOnOffCMD(int nCh, char chCMD)
{
	PC_PACKET_LIGHTONOFF packet = { 0, };
	packet.byStart = STX_LIGHT;
	packet.byChannel = nCh + '0';
	packet.byCMD = chCMD;
	packet.byEnd = ETX_LIGHT;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_LIGHTONOFF)) == ERROR_SUCCESS);
}

int CXFMSPacket::SendLightCMD(int nCh, char chCMD, int Value)
{
	PC_PACKET_LIGHT packet = { 0, };
	char szBrightness[20];
	packet.byStart = STX_LIGHT;
	packet.byChannel = nCh + '0';
	packet.byCMD = chCMD;
	sprintf_s(szBrightness, 20, "%04d", Value);
	memcpy(packet.chValue, szBrightness, 4);
	packet.byEnd = ETX_LIGHT;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_LIGHT)) == ERROR_SUCCESS);
}


