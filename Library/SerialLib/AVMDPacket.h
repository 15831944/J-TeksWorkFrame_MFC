#pragma once
#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers 
#include <afxwin.h>        // MFC core and standard components 
#include <afxext.h>        // MFC extensions 
#ifndef _AFX_NO_AFXCMN_SUPPORT 
#include <afxcmn.h> // MFC support for Windows Common Controls 
#endif // _AFX_NO_AFXCMN_SUPPORT 
#include <deque>
#include <queue>

#define    MAKEBYTE(l, h)    ((BYTE) (((BYTE) (l)) | ((BYTE) (h)) << 4))
#define    HINIBBLE(b)       ((BYTE) (((BYTE) (b) >> 4) & (BYTE) 0x0F))
#define    LONIBBLE(b)       ((BYTE) ((BYTE) (b) & (BYTE) 0x0F))

using namespace std;
//#define _DEVICE_EMULATOR_

#define MAX_CAL_DATE	6
#define MAX_SERIAL_NO	8

#define SENSOR_INFO		0xd0
#define STX_SENSOR		0xe0
#define ETX_RESP		0x0d
#define RCV_NOCMD		-1
#define VALUESET_COUNT	25

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

class CAVMDPacket:public CObject
{
public:
	DECLARE_DYNAMIC(CAVMDPacket)
	CAVMDPacket(CSerial& serial, LONG lTimeout = 20);			// lTimeout : milliseconds.
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
		None = 0,
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

#define MAX_BUF_SIZE	4096

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


	int MakeProcessThread();
	BOOL m_bIsMeaInfo;
	CWinThread *m_pWProcessThread;
	BOOL m_bTerminateThread;
	static UINT ProcessThread(LPVOID pSender);
	BYTE DVCProcessComBuffer();
	deque<BYTE> m_rcvDeque;
	int AnalyzeData();
	int GetQueuePacket(BYTE * pPacket, CAVMDPacket::EPacketType tCmdType);
	BOOL m_bReceiveCMD;
	static void SetXYZ(BYTE * outData, int iAccX, int iAccY, int iAccZ);
	static void GetXYZ(int & iAccX, int & iAccY, int & iAccZ, BYTE *inData);
	// 시리얼에서 읽을 데이터 사이즈 지정(너무 큰 크기로 설정하면 한번에 많은데이터를 받아 처리하려함) 그래프이동이 자연스럽지 않게됨
	int m_nMaxRcvSize;
};