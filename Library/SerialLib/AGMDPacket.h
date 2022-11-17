#pragma once
#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers 
#include <afxwin.h>        // MFC core and standard components 
#include <afxext.h>        // MFC extensions 
#ifndef _AFX_NO_AFXCMN_SUPPORT 
#include <afxcmn.h> // MFC support for Windows Common Controls 
#endif // _AFX_NO_AFXCMN_SUPPORT 
#include <deque>

#define    MAKEBYTE(l, h)    ((BYTE) (((BYTE) (l)) | ((BYTE) (h)) << 4))
#define    HINIBBLE(b)       ((BYTE) (((BYTE) (b) >> 4) & (BYTE) 0x0F))
#define    LONIBBLE(b)       ((BYTE) ((BYTE) (b) & (BYTE) 0x0F))

using namespace std;
//#define _DEVICE_EMULATOR_

#define MAX_CAL_DATE	6
#define MAX_SERIAL_NO	8

#define SENSOR_INFO		0xd0
#define STX_GAPSENSOR	0xc0
#define STX_CALINFO		0xc1
#define STX_CALSET		0xc2
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
	BYTE	byCheckSum;
	BYTE	byEnd;			// 0x0d
} DVC_PACKET_SENSORINFO;	// Device -> PC

typedef struct
{
	BYTE	byStart;		// 0xc0
	BYTE	byTemperatue;
	BYTE	byBattery;
	unsigned short int usGap1;
	unsigned short int usGap2;
	unsigned short int usGap3;
	BYTE	byReserved;
	BYTE	byCheckSum;
	BYTE	byEnd;			// 0x0d
} DVC_PACKET_GAPINFO;		// Device -> PC

typedef struct
{
	BYTE byStart; //0xc1
	char	chCalDate[MAX_CAL_DATE]; //
	unsigned short int usCalTargetDist;
	short sGap1;
	short sGap2;
	short sGap3;
	BYTE	byReserved;
	BYTE byCheckSum;
	BYTE byEnd;
} DVC_PACKET_CALINFO;

typedef struct
{
	BYTE	byStart;		// 0xc0
	BYTE	byBlank;		// 0x00
	BYTE	byCheckSum;		// 0x40
	BYTE	byEnd;			// 0x0d
} PC_PACKET_GAPINFO;		// PC -> Device

typedef struct
{
	BYTE byStart;
	BYTE byBlank;
	BYTE byCheckSum;
	BYTE byEnd;
} PC_PACKET_SENSORINFO;

typedef struct
{
	BYTE byStart;
	BYTE byBlank;
	BYTE byCheckSum;
	BYTE byEnd;
} PC_PACKET_CALINFO;

typedef struct
{
	BYTE byStart; //0xc1
	char	chCalDate[MAX_CAL_DATE]; //
	unsigned short int usCalTargetDist;
	short sGap1;
	short sGap2;
	short sGap3;
	BYTE byReserved;
	BYTE byCheckSum;
	BYTE byEnd;
} PC_PACKET_SETCAL;

#pragma pack(pop)

class CSerial;

class CAGMDPacket:public CObject
{
public:
	DECLARE_DYNAMIC(CAGMDPacket)
	CAGMDPacket(CSerial& serial, LONG lTimeout = 20);			// lTimeout : milliseconds.
	virtual ~CAGMDPacket();

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
		GapMeasure,
		CalInfo,
		SetCalInfo,
	} EPacketType;

	typedef enum
	{
		StateUnknown = -1,
		StateNotConnected,
		StateUnstable,
		StateGapMeasure
	} EDeviceState;

protected:
	CSerial&	m_serial;
	LONG		m_lTimeout;			// milliseconds.

#define MAX_BUF_SIZE	2048

	BYTE		m_byBuffer[MAX_BUF_SIZE];

public:
	CSerial& GetSerial() { return m_serial; }
	BYTE* GetPacket() { return m_byBuffer; }

protected:
	BYTE CalcChecksum(const BYTE* pbyData, int nSize);
	BYTE ProcessComBuffer();		// returns STX (success)or 0 (fail)

public:
	BOOL SendMeasureCommand();
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
	int GetQueuePacket(BYTE * pPacket, CAGMDPacket::EPacketType tCmdType);
	BOOL m_bReceiveCMD;
	BOOL SendCalInfoCmd();
	BOOL SendCalSetCmd(char * chCalDate, int iTargetDist, int iGap1, int iGap2, int iGap3);
};