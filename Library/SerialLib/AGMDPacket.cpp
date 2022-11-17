#include "AGMDPacket.h"
#include "Serial.h"

#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <tchar.h>
#include <mmsystem.h>
#pragma comment(lib, "Setupapi")
#pragma comment(lib, "winmm.lib")

IMPLEMENT_DYNAMIC(CAGMDPacket, CObject);

CAGMDPacket::CAGMDPacket(CSerial& serial, LONG lTimeout)
	: m_serial(serial)
	, m_lTimeout(lTimeout)
	, m_bIsMeaInfo(FALSE)
	, m_pWProcessThread(NULL)
	, m_bTerminateThread(FALSE)
	, m_bReceiveCMD(FALSE)
{

}


CAGMDPacket::~CAGMDPacket()
{
}

BYTE CAGMDPacket::CalcChecksum(const BYTE* pbyData, int nSize)
{
	if (nSize < 2)
		return 0;

	BYTE byChecksum = 0;

	for (int i = 0; i < nSize - 1; i++)
		byChecksum += pbyData[i];

	return (0);
}

BYTE CAGMDPacket::DVCProcessComBuffer()
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
		case STX_GAPSENSOR:
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

	if (byBuffer[0] == STX_GAPSENSOR)
	{
		::CopyMemory(m_byBuffer, byBuffer, sizeof(DVC_PACKET_GAPINFO));
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

int CAGMDPacket::AnalyzeData()
{
	int count = (int)m_rcvDeque.size();
	for (int i = 0; i < count; i++)
	{
		count = (int)m_rcvDeque.size();
		BYTE byCurr = m_rcvDeque.at(0);
		switch (byCurr)
		{
		case SENSOR_INFO:
			if (count >= sizeof(DVC_PACKET_SENSORINFO))
			{
				if(ETX_RESP == m_rcvDeque.at(sizeof(DVC_PACKET_SENSORINFO)-1))
					return SENSOR_INFO;
				else
					m_rcvDeque.pop_front();
			}

			break;
		case STX_GAPSENSOR:
			if (count >= sizeof(DVC_PACKET_GAPINFO))
			{
				if (ETX_RESP == m_rcvDeque.at(sizeof(DVC_PACKET_GAPINFO) - 1))
					return STX_GAPSENSOR;
				else
					m_rcvDeque.pop_front();
			}

			break;
		case STX_CALINFO:
			if (count >= sizeof(DVC_PACKET_CALINFO))
			{
				if (ETX_RESP == m_rcvDeque.at(sizeof(DVC_PACKET_CALINFO) - 1))
					return STX_CALINFO;
				else
					m_rcvDeque.pop_front();
			}
			break;
		case STX_CALSET:
			if (count >= sizeof(DVC_PACKET_CALINFO))
			{
				if (ETX_RESP == m_rcvDeque.at(sizeof(DVC_PACKET_CALINFO) - 1))
					return STX_CALSET;
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

BYTE CAGMDPacket::ProcessComBuffer()
{
#ifdef _DEVICE_EMULATOR_
	return DVCProcessComBuffer();
#endif

	::ZeroMemory(m_byBuffer, sizeof(m_byBuffer));

	BYTE rcvData[MAX_BUF_SIZE];
	BYTE byETX = 0;

	LONG lStartTime = timeGetTime();

	// Read STX byte.
	int dwRead = 0;
	int dwReadMax = 0;
	
	while (TRUE)
	{
		LONG lResult = m_serial.Read(rcvData, MAX_BUF_SIZE, (DWORD *)(&dwRead));
		if (lResult != ERROR_SUCCESS)
		{
			TRACE(_T("m_serial.Read error : %ld\n"), lResult);
			break;
		}

		if (dwRead == 0)
		{
			break;
		}

		dwReadMax += dwRead;
//		TRACE(_T("dwReadMax:%d\n"), dwReadMax);

		for (int i = 0; i < dwRead; i++)
			m_rcvDeque.push_back(rcvData[i]);

	}

	switch (AnalyzeData())
	{
	case SENSOR_INFO:
		return SENSOR_INFO;
		break;
	case STX_GAPSENSOR:
		return STX_GAPSENSOR;
		break;
	case STX_CALINFO:
		return STX_CALINFO;
		break;
	case STX_CALSET:
		return STX_CALSET;
	case RCV_NOCMD:
		return RCV_NOCMD;
		break;
	}

	return RCV_NOCMD;
}

BOOL CAGMDPacket::SendSensorInfo(BYTE byRadius, char* pchCalDate, char* pchSerialNo)
{
	DVC_PACKET_SENSORINFO packet = { 0 };

	packet.byStart = SENSOR_INFO;
	packet.byRadius = byRadius;
	::CopyMemory(packet.chCalDate, pchCalDate, sizeof(packet.chCalDate));
	::CopyMemory(packet.chSerialNo, pchSerialNo, sizeof(packet.chSerialNo));
	packet.byCheckSum = CalcChecksum((const BYTE *)&packet, sizeof(DVC_PACKET_SENSORINFO));
	packet.byEnd = ETX_RESP;

	return (m_serial.Write((const void*)&packet, sizeof(DVC_PACKET_SENSORINFO)) == ERROR_SUCCESS);
}

CAGMDPacket::EPacketType CAGMDPacket::CheckoutPacket()
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
		case STX_GAPSENSOR:
//			DVC_PACKET_GAPINFO* pPacket = (DVC_PACKET_GAPINFO*)m_byBuffer;
			eType = EPacketType::GapMeasure;
			break;
		case SENSOR_INFO:
			eType = EPacketType::SensorInfo;
//			DVC_PACKET_SENSORINFO* pPacket = (DVC_PACKET_SENSORINFO *)m_byBuffer;
			break;
		case STX_CALINFO:
			eType = EPacketType::CalInfo;
			break;
		case STX_CALSET:
			eType = EPacketType::SetCalInfo;
			break;
		}
#endif
	}

	return eType;
}

BOOL CAGMDPacket::SendSensorInfoCommand()
{
	PC_PACKET_SENSORINFO packet = { 0, };

	packet.byStart = SENSOR_INFO;
	packet.byBlank = 0x00;
	packet.byCheckSum = 0x30;
	packet.byEnd = 0x0d;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_SENSORINFO)) == ERROR_SUCCESS);
}

BOOL CAGMDPacket::SendMeasureCommand()
{
	PC_PACKET_GAPINFO packet = { 0, };

	packet.byStart = STX_GAPSENSOR;
	packet.byBlank = 0x00;
	packet.byCheckSum = 0x40;
	packet.byEnd = 0x0d;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_GAPINFO)) == ERROR_SUCCESS);
}


BOOL CAGMDPacket::SendCalInfoCmd()
{
	PC_PACKET_CALINFO packet = { 0, };

	packet.byStart = STX_CALINFO;
	packet.byBlank = 0x00;
	packet.byCheckSum = 0x3f;
	packet.byEnd = 0x0d;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_CALINFO)) == ERROR_SUCCESS);
}


BOOL CAGMDPacket::SendCalSetCmd(char * chCalDate, int iTargetDist, int iGap1, int iGap2, int iGap3)
{
	PC_PACKET_SETCAL packet = { 0, };
	
	packet.byStart = STX_CALSET;
	strncpy_s(packet.chCalDate, chCalDate, 6);
	packet.usCalTargetDist = iTargetDist;
	packet.sGap1 = iGap1;
	packet.sGap2 = iGap2;
	packet.sGap3 = iGap3;
	packet.byReserved = 0x00;
	packet.byCheckSum = CalcChecksum((const BYTE *)&packet, sizeof(PC_PACKET_SETCAL));
	packet.byEnd = 0x0d;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_SETCAL)) == ERROR_SUCCESS);
}

int CAGMDPacket::MakeProcessThread()
{
	m_bTerminateThread = FALSE;
	m_pWProcessThread = AfxBeginThread(ProcessThread, this);
	if (!m_pWProcessThread)
		return 0;
	return -1;
}


UINT CAGMDPacket::ProcessThread(LPVOID pSender)
{
	return 0;
}



int CAGMDPacket::GetQueuePacket(BYTE * pPacket, CAGMDPacket::EPacketType tCmdType)
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
	case EPacketType::GapMeasure:
		sizePacket = sizeof(DVC_PACKET_GAPINFO);
		for (int i = 0; i < sizePacket; i++)
		{
			pPacket[i] = m_rcvDeque.front();
			m_rcvDeque.pop_front();
		}
		//! Change litte indian to Big indian
		((DVC_PACKET_GAPINFO *)pPacket)->usGap1 = (WORD)::ntohs(((DVC_PACKET_GAPINFO *)pPacket)->usGap1);
		((DVC_PACKET_GAPINFO *)pPacket)->usGap2 = (WORD)::ntohs(((DVC_PACKET_GAPINFO *)pPacket)->usGap2);
		((DVC_PACKET_GAPINFO *)pPacket)->usGap3 = (WORD)::ntohs(((DVC_PACKET_GAPINFO *)pPacket)->usGap3);
		break;
	case EPacketType::CalInfo:
		sizePacket = sizeof(DVC_PACKET_CALINFO);
		for (int i = 0; i < sizePacket; i++)
		{
			pPacket[i] = m_rcvDeque.front();
			m_rcvDeque.pop_front();
		}
		break;

	case EPacketType::SetCalInfo:
		sizePacket = sizeof(DVC_PACKET_CALINFO);
		for (int i = 0; i < sizePacket; i++)
		{
			pPacket[i] = m_rcvDeque.front();
			m_rcvDeque.pop_front();
		}
		break;
	}
	return 0;
}


