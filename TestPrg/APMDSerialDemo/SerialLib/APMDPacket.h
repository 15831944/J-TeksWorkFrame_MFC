#pragma once

#include <Windows.h>

//장비 Emulate모드일경우에는 활성화 아니면 비활성화
#define _DEVICE_EMULATOR_

#define MAX_CAL_DATE	6
#define MAX_SERIAL_NO	8

#define STX_CTRL		0x93
#define ETX_CTRL		0x73

#define STX_MEASURE		0x93
#define ETX_MEASURE		0x73

#define STX_STATE		0x94
#define ETX_STATE		0x74

#pragma pack(push, 1)

typedef struct
{
	BYTE	byStart;		// 0x93
	WORD	wSec;
	WORD	wCount01;
	WORD	wCount03;
	WORD	wCount05;
	short	sTemperature;
	WORD	wPressure;
	BYTE	byBattery;
	BYTE	byHumidity;
	BYTE	byReserved;
	BYTE	byChecksum;
	BYTE	bybEnd;			// 0x73
} DVC_PACKET_MEASURE;		// Device -> PC

typedef struct
{
	BYTE	byStart;		// 0x94
	BYTE	byBlank;		// 0x00
	char	chCalDate[MAX_CAL_DATE];
	char	chSerialNo[MAX_SERIAL_NO];
	BYTE	byChecksum;
	BYTE	byEnd;			// 0x74
} DVC_PACKET_STATE;	// Device -> PC

typedef struct
{
	BYTE	byStart;		// 0x93
	WORD	wPurgeTime;
	WORD	wMeasureTime;
	WORD	wReserved;
	BYTE	byChecksum;
	BYTE	byEnd;			// 0x73
} PC_PACKET_CTRL;		// PC -> DEvice

#pragma pack(pop)

class CSerial;

class CAPMDPacket
{
public:
	CAPMDPacket(CSerial& serial, LONG lTimeout = 100);			// lTimeout : milliseconds.
	virtual ~CAPMDPacket();

	typedef enum
	{
		None = -1,
		Start,
		Stop,
		Measure,
		State
	} EPacketType;

	typedef enum
	{
		StateUnknown = -1,
		StateStandby,
		StatePurge,
		StateMeasure
	} EDeviceState;

protected:
	CSerial&	m_serial;
	LONG		m_lTimeout;			// milliseconds.

#define MAX_BUF_SIZE	256

	BYTE		m_byBuffer[MAX_BUF_SIZE];

public:
	CSerial& GetSerial() { return m_serial; }
	BYTE* GetPacket()    { return m_byBuffer; }

protected:
	BYTE CalcChecksum(const BYTE* pbyData, int nSize);
	BYTE ProcessComBuffer();		// returns STX (success)or 0 (fail)

public:
	BOOL SendStartCommand(WORD wPurgeTime, WORD wMeasureTime);
	BOOL SendStopCommand();
	BOOL SendMeasureData(WORD wSec, WORD wCount01, WORD wCount03, WORD wCount05,
		short sTemperature, WORD wPressure, BYTE byBattery, BYTE byHumidity);
	BOOL SendStateData(char* pchCalDate, char* pchSerialNo);

	EPacketType CheckoutPacket();
	EDeviceState CheckDeviceState(DVC_PACKET_MEASURE* pPacket);
};

