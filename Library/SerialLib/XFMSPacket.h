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

#define STX_LIGHT		0x02
#define ETX_LIGHT		0x03
#define SENSOR_INFO		0xd0
#define STX_SENSOR		0xe0
#define ETX_RESP		0x0d
#define RCV_NOCMD		-1
#define VALUESET_COUNT	25

//kiy--------------------------------
#define STX				0x02
#define CARRAGE_RETURE	0x0d	//<cr>
#define ETX				0x03
#define CMD_MODE		0X4d	//F - focal spot
#define CMD_KV			0x55	//U - kv
#define CMD_MA			0X49	//I - ma
#define CMD_TIME		0X54	//T - time
#define CMD_FOCAL		0X46	//F - focal spot
//STX -> COM(ex) U1234) -> CARRAGE_RETURE -> ETX
//-----------------------------------

#pragma pack(push, 1)

//kiy--------------------------------
typedef struct
{
	BYTE byStart = STX;				//0x02
	BYTE byMode[4] = { 'M','4','0','0' };
	BYTE byCR = CARRAGE_RETURE;		//0x0d - CARRAGE_RETURE
	BYTE byEnd = ETX;				//0x03
} PC_PACKET_SELECT_M400;

typedef struct
{
	BYTE byStart = STX;				//0x02
	BYTE byMode[4] = { 'M','4','0','1' };
	BYTE byCR = CARRAGE_RETURE;		//0x0d - CARRAGE_RETURE
	BYTE byEnd = ETX;				//0x03
} PC_PACKET_SELECT_M401;

typedef struct
{
	BYTE byStart = STX;				//0x02
	BYTE byMode[4] = { 'M','9','3','0' };
	BYTE byCR = CARRAGE_RETURE;		//0x0d - CARRAGE_RETURE
	BYTE byEnd = ETX;				//0x03
} PC_PACKET_SELECT_M930;

typedef struct
{
	BYTE byStart;		//0x02
	BYTE byOnOffCmd[2];	//0x4f 0x4e - ON,	0x4f 0x46 - OFF
	BYTE byCR;			//0x0d - CARRAGE_RETURE
	BYTE byEnd;			//0x03
} PC_PACKET_XRAYONOFF;

typedef struct
{
	BYTE byStart;	//0x02
	BYTE byFocalSize[2];
	BYTE byCR;		//0x0d - CARRAGE_RETURE
	BYTE byEnd;		//0x03
} PC_PACKET_FOCAL_SPOT_SIZE;
//-----------------------------------

typedef struct
{
	BYTE	byStart;		// 0x02
	BYTE	byChannel;		// 0x00~0x03
	BYTE	byCMD;			// 'o':on 'f':off 'w' Data전송
	char	chValue[4];   	// 0000~1023
	BYTE	byEnd;			// 0x03
} PC_PACKET_LIGHT;			// PC -> Device

typedef struct
{
	BYTE	byStart;		// 0x02
	BYTE	byChannel;		// 0x00~0x03
	BYTE	byCMD;			// 'o':on 'f':off 'w' Data전송
	BYTE	byEnd;			// 0x03
} PC_PACKET_LIGHTONOFF;		// PC -> Device

typedef struct
{
	BYTE	byStart;		// 0xd0
	BYTE	byRadius;		// 
	char	chCalDate[MAX_CAL_DATE];
	char	chSerialNo[MAX_SERIAL_NO];
	BYTE	byChecksum;
	BYTE	byEnd;			// 0x0d
} DVC_PACKET_SENSORINFO;	// Device -> PC

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

class CXFMSPacket :public CObject
{
public:
	DECLARE_DYNAMIC(CXFMSPacket)
	CXFMSPacket(CSerial& serial, LONG lTimeout = 20);			// lTimeout : milliseconds.
	virtual ~CXFMSPacket();

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
	enum tLightStatus { LIGHT_OFF, LIGHT_ON };

	CSerial& GetSerial() { return m_serial; }
	BYTE* GetPacket() { return m_byBuffer; }

protected:
	BYTE CalcChecksum(const BYTE* pbyData, int nSize);
	BYTE ProcessComBuffer();		// returns STX (success)or 0 (fail)

public:
	EPacketType CheckoutPacket();

	int MakeProcessThread();
	BOOL m_bIsMeaInfo;
	CWinThread *m_pWProcessThread;
	BOOL m_bTerminateThread;
	static UINT ProcessThread(LPVOID pSender);
	BYTE DVCProcessComBuffer();
	deque<BYTE> m_rcvDeque;
	int AnalyzeData();
	int GetQueuePacket(BYTE * pPacket, CXFMSPacket::EPacketType tCmdType);
	BOOL m_bReceiveCMD;
	int m_nMaxRcvSize;
	int LightOnOff(tLightStatus lsStatus, long *pBrightness);
	int LightOnOff(int ch, tLightStatus lsStatus);
	int SendLightOnOffCMD(int nCh, char chCMD);
	int SendLightCMD(int nCh, char chCMD, int Value);
	int SendLightBrightness(int pBrightness[4]);
	int SendLightBrightness(int ch, int iBrightness);

	//kiy 17.12.01-----------------------
	int SendSetXrayOnOff(BOOL xrayOn);
	int SendSetFocalSpot(BOOL SetStandardSize);
	int SendSetKvCMD(double kv);
	int SendSetMaCmd(double ma);
	//-----------------------------------
};