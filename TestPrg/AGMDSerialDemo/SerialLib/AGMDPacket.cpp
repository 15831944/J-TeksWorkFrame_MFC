#include "AGMDPacket.h"
#include "Serial.h"

#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <tchar.h>

#pragma comment(lib, "Setupapi")

CAGMDPacket::CAGMDPacket(CSerial& serial, LONG lTimeout)
	: m_serial(serial)
	, m_lTimeout(lTimeout)
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

BYTE CAGMDPacket::ProcessComBuffer()
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
		case STX_GAPSENSOR:
			nBytesToRead = sizeof(PC_PACKET_SENSORINFO) - 1;
			byETX = ETX_RESP;
			break;
		case STX_CALINFO:
			nBytesToRead = sizeof(PC_PACKET_CALINFO) - 1;
			byETX = ETX_RESP;
			break;
		case STX_CALSET:
			nBytesToRead = sizeof(PC_PACKET_SETCAL) - 1;
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
	switch (byBuffer[0])
	{
	case STX_GAPSENSOR:
		::CopyMemory(m_byBuffer, byBuffer, sizeof(PC_PACKET_GAPINFO));
		break;
	case SENSOR_INFO:
		::CopyMemory(m_byBuffer, byBuffer, sizeof(PC_PACKET_SENSORINFO));
		break;
	case STX_CALINFO:
		::CopyMemory(m_byBuffer, byBuffer, sizeof(PC_PACKET_CALINFO));
		break;
	case STX_CALSET:
		::CopyMemory(m_byBuffer, byBuffer, sizeof(PC_PACKET_SETCAL));
		break;
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

BOOL CAGMDPacket::SendMeasureData(int iGap1Value, int iGap2Value, int iGap3Value, BYTE byTemperature, BYTE byBattery, BYTE nPacket)
{
	DVC_PACKET_GAPINFO packet = { 0, };

	packet.byStart = STX_GAPSENSOR;

	packet.byTemperatue = byTemperature;
	packet.byBattery = byBattery;
	packet.usGap1 = (WORD)::ntohs(iGap1Value);
	packet.usGap2 = (WORD)::ntohs(iGap2Value);
	packet.usGap3 = (WORD)::ntohs(iGap3Value);
	packet.byCheckSum = CalcChecksum((const BYTE *)&packet, sizeof(DVC_PACKET_GAPINFO));
	packet.byEnd = ETX_RESP;

	return (m_serial.Write((const void*)&packet, sizeof(DVC_PACKET_GAPINFO)) == ERROR_SUCCESS);
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

BOOL CAGMDPacket::SendCalInfo(char* pchCalDate, int iCalTarget, int iCal1, int iCal2, int iCal3)
{
	DVC_PACKET_CALINFO packet = { 0 };

	packet.byStart = STX_CALINFO;
	::CopyMemory(packet.chCalDate, pchCalDate, sizeof(packet.chCalDate));
	packet.usCalTargetDist = (unsigned short int)iCalTarget;
	packet.sGap1 = iCal1;
	packet.sGap2 = iCal2;
	packet.sGap3 = iCal3;
	packet.byReserved = 0x00;
	packet.byCheckSum = CalcChecksum((const BYTE *)&packet, sizeof(DVC_PACKET_CALINFO));
	packet.byEnd = ETX_RESP;

	return (m_serial.Write((const void*)&packet, sizeof(DVC_PACKET_CALINFO)) == ERROR_SUCCESS);
}

BOOL CAGMDPacket::SendRepSetCal(char* pchCalDate, int iCalTarget, int iCal1, int iCal2, int iCal3)
{
	DVC_PACKET_CALINFO packet = { 0 };

	packet.byStart = STX_CALSET;
	::CopyMemory(packet.chCalDate, pchCalDate, sizeof(packet.chCalDate));
	packet.usCalTargetDist = (unsigned short int)iCalTarget;
	packet.sGap1 = iCal1;
	packet.sGap2 = iCal2;
	packet.sGap3 = iCal3;
	packet.byReserved = 0x00;
	packet.byCheckSum = CalcChecksum((const BYTE *)&packet, sizeof(DVC_PACKET_CALINFO));
	packet.byEnd = ETX_RESP;

	return (m_serial.Write((const void*)&packet, sizeof(DVC_PACKET_CALINFO)) == ERROR_SUCCESS);
}


CAGMDPacket::EPacketType CAGMDPacket::CheckoutPacket()
{
	EPacketType eType = EPacketType::None;

	BYTE bySTX = ProcessComBuffer();

	BYTE byTEST = bySTX;
	if (bySTX != 0)
	{
#ifdef _DEVICE_EMULATOR_
		switch (bySTX)
		{
		case SENSOR_INFO:
			eType = EPacketType::SensorInfo;
			break;
		case STX_GAPSENSOR:
			eType = EPacketType::GapMeasure;
			break;
		case STX_CALINFO:
			eType = EPacketType::CalInfo;
			break;
		case STX_CALSET:
			eType = EPacketType::SetCalInfo;
			break;
		}
		//PC_PACKET_CTRL* pPacket = (PC_PACKET_CTRL*)m_byBuffer;

		//pPacket->wPurgeTime = (WORD)::ntohs(pPacket->wPurgeTime);
		//pPacket->wMeasureTime = (WORD)::ntohs(pPacket->wMeasureTime);

#else
		if (bySTX == STX_SENSOR)
		{
			DVC_PACKET_MEAINFO* pPacket = (DVC_PACKET_MEAINFO*)m_byBuffer;
			/*pPacket->wRoll = (WORD)::ntohs(pPacket->wRoll);
			pPacket->wPitch = (WORD)::ntohs(pPacket->wPitch);*/
			//pPacket->

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
	packet.byCheckSum = 0x20;
	packet.byEnd = 0x0d;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_GAPINFO)) == ERROR_SUCCESS);
}
