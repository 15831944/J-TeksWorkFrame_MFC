#include "ALMDPacket.h"
#include "Serial.h"

#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <tchar.h>

#pragma comment(lib, "Setupapi")

CALMDPacket::CALMDPacket(CSerial& serial, LONG lTimeout)
	: m_serial(serial)
	, m_lTimeout(lTimeout)
{

}


CALMDPacket::~CALMDPacket()
{
}

BYTE CALMDPacket::CalcChecksum(const BYTE* pbyData, int nSize)
{
	if (nSize < 2)
		return 0;

	BYTE byChecksum = 0;

	for (int i = 0; i < nSize - 1; i++)
		byChecksum += pbyData[i];

//	return (-byChecksum);
	return (0);
}

BYTE CALMDPacket::ProcessComBuffer()
{
	::ZeroMemory(m_byBuffer, sizeof(m_byBuffer));

	BYTE byBuffer[MAX_BUF_SIZE];
	BYTE byETX = 0;

	LONG lStartTime = ::GetTickCount();

	// Read STX byte.
	int nBytesToRead = 1;
	DWORD dwRead = 0;

	m_serial.Read(byBuffer, nBytesToRead, &dwRead);
	if (dwRead == nBytesToRead)
	{
		switch (byBuffer[0])
		{
#ifdef _DEVICE_EMULATOR_
		case SENSOR_INFO:
			nBytesToRead = sizeof(PC_PACKET_SENSORINFO) - 1;
			byETX = ETX_RESP;
			break;
		case STX_SENSOR:
			nBytesToRead = sizeof(PC_PACKET_SENSORINFO) - 1;
			byETX = ETX_RESP;
			break;
#else
		case SENSOR_INFO:
			nBytesToRead = sizeof(DVC_PACKET_SENSORINFO) - 1;
			byETX = ETX_RESP;
			break;

		case STX_SENSOR:
			nBytesToRead = sizeof(DVC_PACKET_MEAINFO) - 1;
			byETX = ETX_RESP;
			break;
#endif
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
	} while (nBytesToRead > 0 && ::GetTickCount() < (DWORD)(lStartTime + m_lTimeout));

	if (nBytesToRead > 0)	// Timeout 
		return 0;

	if (byBuffer[nBytesRead - 1] != byETX)		// Corrupted
		return 0;

	BYTE byChecksum = CalcChecksum(byBuffer, nBytesRead);
	if (byChecksum != 0)		// Checksum error
		return 0;

#ifdef _DEVICE_EMULATOR_
	if (byBuffer[0] == STX_SENSOR)
	{
		::CopyMemory(m_byBuffer, byBuffer, sizeof(PC_PACKET_MEAINFO));
	}
	else // SENSOR_INFO
	{
		::CopyMemory(m_byBuffer, byBuffer, sizeof(PC_PACKET_SENSORINFO));
	}
#else
	if (byBuffer[0] == STX_SENSOR)
	{
		::CopyMemory(m_byBuffer, byBuffer, sizeof(DVC_PACKET_MEAINFO));
	}
	else // SENSOR_INFO
	{
		::CopyMemory(m_byBuffer, byBuffer, sizeof(DVC_PACKET_SENSORINFO));
	}
#endif

	// Clean up
	do
	{
		m_serial.Read(byBuffer, MAX_BUF_SIZE, &dwRead);
	} while (MAX_BUF_SIZE == (int)dwRead);

	BYTE outData;
	outData = m_byBuffer[0];

	return outData;
}



BOOL CALMDPacket::SendMeasureData(WORD wRoll, WORD wPitch, BYTE byTemperature, BYTE byBattery)
{
	DVC_PACKET_MEAINFO packet = { 0, };

	packet.byStart = STX_SENSOR;	
	packet.wRoll = (WORD)::htons(wRoll);
	packet.wPitch = (WORD)::htons(wPitch);
	packet.byTemperatue = byTemperature;
	packet.byBattery = byBattery;
	packet.byChecksum = CalcChecksum((const BYTE *)&packet, sizeof(DVC_PACKET_MEAINFO));
	packet.byEnd = ETX_RESP;

	return (m_serial.Write((const void*)&packet, sizeof(DVC_PACKET_MEAINFO)) == ERROR_SUCCESS);
}

BOOL CALMDPacket::SendSensorInfo(BYTE byRadius, char* pchCalDate, char* pchSerialNo)
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

CALMDPacket::EPacketType CALMDPacket::CheckoutPacket()
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
		if (bySTX == STX_SENSOR)
		{
			DVC_PACKET_MEAINFO* pPacket = (DVC_PACKET_MEAINFO*)m_byBuffer;
			pPacket->wRoll = (WORD)::ntohs(pPacket->wRoll);
			pPacket->wPitch = (WORD)::ntohs(pPacket->wPitch);
			eType = EPacketType::Measure;
		}
		else // SENSOR_INFO
		{
			eType = EPacketType::SensorInfo;
			DVC_PACKET_SENSORINFO* pPacket = (DVC_PACKET_SENSORINFO *)m_byBuffer;
		}
#endif
	}

	return eType;
}

BOOL CALMDPacket::SendSensorInfoCommand()
{
	PC_PACKET_SENSORINFO packet = { 0, };

	packet.byStart = SENSOR_INFO;
	packet.byBlank = 0x00;
	packet.byCheckSum = 0x30;
	packet.byEnd = 0x0d;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_SENSORINFO)) == ERROR_SUCCESS);
}

BOOL CALMDPacket::SendMeasureCommand()
{
	PC_PACKET_MEAINFO packet = { 0, };

	packet.byStart = STX_SENSOR;
	packet.byBlack = 0x00;
	packet.byCheckSum = 0x10;
	packet.byEnd = 0x0d;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_MEAINFO)) == ERROR_SUCCESS);
}