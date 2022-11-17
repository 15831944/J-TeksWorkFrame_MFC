#pragma once

#include <Windows.h>

#define _DEVICE_EMULATOR_

#define MAX_CAL_DATE	6
#define MAX_SERIAL_NO	8

#define SENSOR_INFO		0xd0
#define STX_SENSOR		0xf0
#define ETX_RESP		0x0d


#pragma pack(push, 1)

typedef struct
{
	BYTE	byStart;		// 0xd0
	BYTE	byRadius;		// 
	char	chCalDate[MAX_CAL_DATE];
	char	chSerialNo[MAX_SERIAL_NO];
	BYTE	byChecksum;
	BYTE	byEnd;			// 0x0d
} DVC_PACKET_SENSORINFO;	// Device -> PC

typedef struct
{
	BYTE	byStart;		// 0xf0
	BYTE	byTemperatue;	
	BYTE	byBattery;
	WORD	wRoll;
	WORD	wPitch;
	BYTE	byChecksum;			
	BYTE	byEnd;			// 0x0d
} DVC_PACKET_MEAINFO;		// Device -> PC

typedef struct
{
	BYTE	byStart;		// 0xf0
	BYTE	byBlack;		// 0x00
	BYTE	byCheckSum;		// 0x10
	BYTE	byEnd;			// 0x0d
} PC_PACKET_MEAINFO;		// PC -> Device

typedef struct
{
	BYTE byStart;			//0xd0
	BYTE byBlank;		
	BYTE byCheckSum;		//0xf0
	BYTE byEnd;				//0x0d
} PC_PACKET_SENSORINFO;
#pragma pack(pop)

class CSerial;

class CALMDPacket
{
public:
	CALMDPacket(CSerial& serial, LONG lTimeout = 100);			// lTimeout : milliseconds.
	virtual ~CALMDPacket();

	typedef enum
	{
		W150MM=1,
		W200MM,
		W300MM,
		W450MM
	}EPacketWRadius;

	typedef enum
	{
		None = -1,
		SensorInfo,
		Measure
	} EPacketType;

	typedef enum
	{
		StateUnknown = -1,
		StateNotConnected,
		StateUnstable,
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
	BOOL SendMeasureCommand();
	BOOL SendMeasureData(WORD wRoll, WORD wPitch, BYTE byTemperature, BYTE byBattery);
	BOOL SendSensorInfo(BYTE byRadius, char* pchCalDate, char* pchSerialNo);

	EPacketType CheckoutPacket();
	EDeviceState CheckDeviceState(DVC_PACKET_MEAINFO* pPacket);
	BOOL SendSensorInfoCommand();
};

