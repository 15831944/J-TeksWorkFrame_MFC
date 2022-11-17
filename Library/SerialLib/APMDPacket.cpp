#include "APMDPacket.h"
#include "Serial.h"

#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <tchar.h>

#pragma comment(lib, "Setupapi")

CAPMDPacket::CAPMDPacket(CSerial& serial, LONG lTimeout)
	: m_serial(serial)
	, m_lTimeout(lTimeout)
{

}


CAPMDPacket::~CAPMDPacket()
{
}

BYTE CAPMDPacket::CalcChecksum(const BYTE* pbyData, int nSize)
{
	if (nSize < 2)
		return 0;

	BYTE byChecksum = 0;

	for (int i = 1; i < nSize - 2; i++)
		byChecksum += pbyData[i];

	return (-byChecksum);
}

BYTE CAPMDPacket::ProcessComBuffer()
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
		case STX_CTRL:
			nBytesToRead = sizeof(PC_PACKET_CTRL) - 1;
			byETX = ETX_CTRL;
			break;
#else
		case STX_MEASURE:
			nBytesToRead = sizeof(DVC_PACKET_MEASURE) - 1;
			byETX = ETX_MEASURE;
			break;

		case STX_STATE:
			nBytesToRead = sizeof(DVC_PACKET_STATE) - 1;
			byETX = ETX_STATE;
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
	if (byChecksum != byBuffer[nBytesRead - 2])		// Checksum error
		return 0;

#ifdef _DEVICE_EMULATOR_
	::CopyMemory(m_byBuffer, byBuffer, sizeof(PC_PACKET_CTRL));
#else
	if (byBuffer[0] == STX_MEASURE)
	{
		::CopyMemory(m_byBuffer, byBuffer, sizeof(DVC_PACKET_MEASURE));
	}
	else // STX_STATE
	{
		::CopyMemory(m_byBuffer, byBuffer, sizeof(DVC_PACKET_STATE));
	}
#endif

	// Clean up
	do
	{
		m_serial.Read(byBuffer, MAX_BUF_SIZE, &dwRead);
	} while (MAX_BUF_SIZE == (int)dwRead);

	return m_byBuffer[0];		// STX
}

BOOL CAPMDPacket::SendStartCommand(WORD wPurgeTime, WORD wMeasureTime)
{
	PC_PACKET_CTRL packet = { 0, };

	packet.byStart = STX_CTRL;
	packet.wPurgeTime = (WORD)::htons(wPurgeTime);
	packet.wMeasureTime = (WORD)::htons(wMeasureTime);
	packet.byChecksum = CalcChecksum((const BYTE *)&packet, sizeof(PC_PACKET_CTRL));
	packet.byEnd = ETX_CTRL;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_CTRL)) == ERROR_SUCCESS);
}

BOOL CAPMDPacket::SendStopCommand()
{
	PC_PACKET_CTRL packet = { 0, };

	packet.byStart = STX_CTRL;
	packet.byChecksum = CalcChecksum((const BYTE *)&packet, sizeof(PC_PACKET_CTRL));
	packet.byEnd = ETX_CTRL;

	return (m_serial.Write((const void*)&packet, sizeof(PC_PACKET_CTRL)) == ERROR_SUCCESS);
}

BOOL CAPMDPacket::SendMeasureData(WORD wSec, WORD wCount01, WORD wCount03, WORD wCount05,
	short sTemperature, WORD wPressure, BYTE byBattery, BYTE byHumidity)
{
	DVC_PACKET_MEASURE packet = { 0, };

	packet.byStart = STX_MEASURE;		// 0x93
	packet.wSec = (WORD)::htons(wSec);
	packet.wCount01 = (WORD)::htons(wCount01);
	packet.wCount03 = (WORD)::htons(wCount03);
	packet.wCount05 = (WORD)::htons(wCount05);
	packet.sTemperature = ::htons(sTemperature);
	packet.wPressure = (WORD)::htons(wPressure);
	packet.byBattery = byBattery;
	packet.byHumidity = byHumidity;
	packet.byReserved = 0;
	packet.byChecksum = CalcChecksum((const BYTE *)&packet, sizeof(DVC_PACKET_MEASURE));
	packet.bybEnd = ETX_MEASURE;

	return (m_serial.Write((const void*)&packet, sizeof(DVC_PACKET_MEASURE)) == ERROR_SUCCESS);
}

BOOL CAPMDPacket::SendStateData(char* pchCalDate, char* pchSerialNo)
{
	DVC_PACKET_STATE packet = { 0 };

	packet.byStart = STX_STATE;
	packet.byBlank = 0x00;
	::CopyMemory(packet.chCalDate, pchCalDate, sizeof(packet.chCalDate));
	::CopyMemory(packet.chSerialNo, pchSerialNo, sizeof(packet.chSerialNo));
	packet.byChecksum = CalcChecksum((const BYTE *)&packet, sizeof(DVC_PACKET_STATE));
	packet.byEnd = ETX_STATE;

	return (m_serial.Write((const void*)&packet, sizeof(DVC_PACKET_STATE)) == ERROR_SUCCESS);
}

CAPMDPacket::EPacketType CAPMDPacket::CheckoutPacket()
{
	EPacketType eType = EPacketType::None;

	BYTE bySTX = ProcessComBuffer();
	if (bySTX != 0)
	{
#ifdef _DEVICE_EMULATOR_
		PC_PACKET_CTRL* pPacket = (PC_PACKET_CTRL*)m_byBuffer;

		pPacket->wPurgeTime = (WORD)::ntohs(pPacket->wPurgeTime);
		pPacket->wMeasureTime = (WORD)::ntohs(pPacket->wMeasureTime);

		if (pPacket->wPurgeTime != 0 || pPacket->wMeasureTime != 0)	// Start
		{
			eType = EPacketType::Start;
		}
		else // Stop
		{
			eType = EPacketType::Stop;
		}
#else
		if (bySTX == STX_MEASURE)
		{
			DVC_PACKET_MEASURE* pPacket = (DVC_PACKET_MEASURE*)m_byBuffer;

			pPacket->wSec = (WORD)::ntohs(pPacket->wSec);
			pPacket->wCount01 = (WORD)::ntohs(pPacket->wCount01);
			pPacket->wCount03 = (WORD)::ntohs(pPacket->wCount03);
			pPacket->wCount05 = (WORD)::ntohs(pPacket->wCount05);
			pPacket->sTemperature = ::ntohs(pPacket->sTemperature);
			pPacket->wPressure = (WORD)::ntohs(pPacket->wPressure);

			eType = EPacketType::Measure;
		}
		else // STX_STATE
		{
			eType = EPacketType::State;
		}
#endif
	}

	return eType;
}

CAPMDPacket::EDeviceState CAPMDPacket::CheckDeviceState(DVC_PACKET_MEASURE* pPacket)
{
	if (pPacket->wSec == 0)
	{
		if (pPacket->wCount01 == 0 && pPacket->wCount03 == 0 && pPacket->wCount05 == 0)
		{
			return EDeviceState::StateStandby;
		}
		else
		{
			return EDeviceState::StatePurge;
		}
	}

	return EDeviceState::StateMeasure;
}

