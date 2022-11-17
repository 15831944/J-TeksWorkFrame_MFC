#include "AVMDPacket.h"
#include "Serial.h"

#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <tchar.h>
#include <mmsystem.h>
#pragma comment(lib, "Setupapi")
#pragma comment(lib, "winmm.lib")

IMPLEMENT_DYNAMIC(CAVMDPacket, CObject);

CAVMDPacket::CAVMDPacket(CSerial& serial, LONG lTimeout)
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


CAVMDPacket::~CAVMDPacket()
{
}

BYTE CAVMDPacket::CalcChecksum(const BYTE* pbyData, int nSize)
{
	if (nSize < 2)
		return 0;

	BYTE byChecksum = 0;

	for (int i = 0; i < nSize - 1; i++)
		byChecksum += pbyData[i];

	return (0);
}

BYTE CAVMDPacket::DVCProcessComBuffer()
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

int CAVMDPacket::AnalyzeData()
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
				if(ETX_RESP == m_rcvDeque.at(sizeof(DVC_PACKET_SENSORINFO)-1))
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

BYTE CAVMDPacket::ProcessComBuffer()
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
	
	while(true)
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

BOOL CAVMDPacket::SendMeasureData(int xValue[], int yValue[], int zValue[], BYTE byTemperature, BYTE byBattery, BYTE nPacket)
{
	DVC_PACKET_MEAINFO packet = { 0, };

	packet.byStart = STX_SENSOR;

	for (int i = 0; i < VALUESET_COUNT; i++)
	{
		SetXYZ((BYTE *)(packet.accInfo), xValue[i], xValue[i], zValue[i]);
	}

	packet.byTemperatue = byTemperature;
	packet.byBattery = byBattery;
	packet.byNum = nPacket;
	packet.byChecksum = CalcChecksum((const BYTE *)&packet, sizeof(DVC_PACKET_MEAINFO));
	packet.byEnd = ETX_RESP;

	return (m_serial.Write((const void*)&packet, sizeof(DVC_PACKET_MEAINFO)) == ERROR_SUCCESS);
}

BOOL CAVMDPacket::SendSensorInfo(BYTE byRadius, char* pchCalDate, char* pchSerialNo)
{
	DVC_PACKET_SENSORINFO packet = { 0 };

	packet.byStart = SENSOR_INFO;
	packet.byRadius = byRadius;
	::CopyMemory(packet.chCalDate, pchCalDate, sizeof(packet.chCalDate));
	::CopyMemory(packet.chSerialNo, pchSerialNo, sizeof(packet.chSerialNo));
	packet.byChecksum = CalcChecksum((const BYTE *)&packet, sizeof(DVC_PACKET_SENSORINFO));
	packet.byEnd = ETX_RESP;

	return (m_serial.Write((const void*)&packet, sizeof(DVC_PACKET_SENSORINFO)) == ERROR_SUCCESS);
}

CAVMDPacket::EPacketType CAVMDPacket::CheckoutPacket()
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

BOOL CAVMDPacket::SendSensorInfoCommand()
{
	PC_PACKET_SENSORINFO packet = { 0, };

	packet.byStart = SENSOR_INFO;
	packet.byBlank = 0x00;
	packet.byCheckSum = 0x30;
	packet.byEnd = 0x0d;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_SENSORINFO)) == ERROR_SUCCESS);
}

BOOL CAVMDPacket::SendMeasureCommand()
{
	PC_PACKET_MEAINFO packet = { 0, };

	packet.byStart = STX_SENSOR;
	packet.byBlack = 0x00;
	packet.byCheckSum = 0x20;
	packet.byEnd = 0x0d;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_MEAINFO)) == ERROR_SUCCESS);
}

int CAVMDPacket::MakeProcessThread()
{
	m_bTerminateThread = FALSE;
	m_pWProcessThread = AfxBeginThread(ProcessThread, this);
	if (!m_pWProcessThread)
		return 0;
	return -1;
}


UINT CAVMDPacket::ProcessThread(LPVOID pSender)
{
	return 0;
}



int CAVMDPacket::GetQueuePacket(BYTE * pPacket, CAVMDPacket::EPacketType tCmdType)
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

struct t12bitCvt
{
	signed int sNum : 12;
};
void CAVMDPacket::GetXYZ(int & iAccX, int & iAccY, int & iAccZ, BYTE *inData)
{
	int reserved;
	t12bitCvt NumCvt;
	reserved = HINIBBLE(inData[0]);
	iAccX = NumCvt.sNum = MAKEWORD(inData[1], LONIBBLE(inData[0]));
	iAccY = NumCvt.sNum = MAKEWORD(MAKEBYTE(HINIBBLE(inData[3]), LONIBBLE(inData[2])), HINIBBLE(inData[2]));
	iAccZ = NumCvt.sNum = MAKEWORD(inData[4], LONIBBLE(inData[3]));
}


void CAVMDPacket::SetXYZ(BYTE * outData, int iAccX, int iAccY, int iAccZ)
{
	int reserved = 0x00;
	outData[0] = MAKEBYTE(LONIBBLE(HIBYTE(iAccX)), reserved);
	outData[1] = LOBYTE(iAccX);
	outData[2] = MAKEBYTE(HINIBBLE(LOBYTE(iAccY)), HIBYTE(iAccY));
	outData[3] = MAKEBYTE(LONIBBLE(HIBYTE(iAccZ)), LONIBBLE(LOBYTE(iAccY)));
	outData[4] = LOBYTE(iAccZ);
}
