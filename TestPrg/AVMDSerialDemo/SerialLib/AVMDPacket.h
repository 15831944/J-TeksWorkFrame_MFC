#pragma once

#include <Windows.h>

#define _DEVICE_EMULATOR_

#define MAX_CAL_DATE	6
#define MAX_SERIAL_NO	8

#define SENSOR_INFO		0xd0
#define STX_SENSOR		0xe0
#define ETX_RESP		0x0d
#define VALUESET_COUNT	25


#define    MAKEBYTE(l, h)    ((BYTE) (((BYTE) (l)) | ((BYTE) (h)) << 4))
#define    HINIBBLE(b)       ((BYTE) (((BYTE) (b) >> 4) & (BYTE) 0x0F))
#define    LONIBBLE(b)       ((BYTE) ((BYTE) (b) & (BYTE) 0x0F))

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

//typedef struct DVC_ACCINFO
//{
//	unsigned int reserved : 4;
//	signed int x : 12;
//	signed int y : 12;
//	signed int z : 12;
//}DVC_ACCINFO;
typedef struct DVC_ACCINFO
{
	BYTE dataXYZ[5];
}DVC_ACCINFO;

typedef struct
{
	BYTE	byStart;		// 0xe0
	BYTE	byTemperatue;
	BYTE	byBattery;
	BYTE	byNum;
	DVC_ACCINFO accInfo[VALUESET_COUNT];
	BYTE	byChecksum;
	BYTE	byEnd;			// 0x0d
} DVC_PACKET_MEAINFO;		// Device -> PC


typedef struct
{
	BYTE	byStart;		// 0xe0
	BYTE	byBlack;		// 0x00
	BYTE	byCheckSum;		// 0x20
	BYTE	byEnd;			// 0x0d
} PC_PACKET_MEAINFO;		// PC -> Device

typedef struct
{
	BYTE byStart;			
	BYTE byBlank;
	BYTE byCheckSum;		
	BYTE byEnd;				
} PC_PACKET_SENSORINFO;
#pragma pack(pop)

class CSerial;

class CAVMDPacket
{
public:
	CAVMDPacket(CSerial& serial, LONG lTimeout = 100);			// lTimeout : milliseconds.
	virtual ~CAVMDPacket();

	typedef enum
	{
		W150MM = 1,	//0x01
		W200MM,		//0x02
		W300MM,		//0x03
		W450MM		//0x04
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
	BYTE* GetPacket() { return m_byBuffer; }

protected:
	BYTE CalcChecksum(const BYTE* pbyData, int nSize);
	BYTE ProcessComBuffer();		// returns STX (success)or 0 (fail)

public:
	BOOL SendMeasureCommand();
	BOOL SendMeasureData(int xValue[15], int yValue[15], int zValue[15], BYTE byTemperature, BYTE byBattery, BYTE nPacket);
	BOOL SendSensorInfo(BYTE byRadius, char* pchCalDate, char* pchSerialNo);

	EPacketType CheckoutPacket();
	//EDeviceState CheckDeviceState(DVC_PACKET_MEAINFO* pPacket);
	BOOL SendSensorInfoCommand();
	static void SetXYZ(BYTE * outData, int iAccX, int iAccY, int iAccZ);
	static void GetXYZ(int & iAccX, int & iAccY, int & iAccZ, BYTE *inData);
};