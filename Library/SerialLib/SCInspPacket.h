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

#pragma pack(push, 1)

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


#pragma pack(pop)

class CSerial;

class CSCInspPacket:public CObject
{
public:
	DECLARE_DYNAMIC(CSCInspPacket)
	CSCInspPacket(CSerial& serial, LONG lTimeout = 20);			// lTimeout : milliseconds.
	virtual ~CSCInspPacket();

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
	enum tLightStatus{ LIGHT_OFF, LIGHT_ON};

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
	int GetQueuePacket(BYTE * pPacket, CSCInspPacket::EPacketType tCmdType);
	BOOL m_bReceiveCMD;
	int m_nMaxRcvSize;
	int LightOnOff(tLightStatus lsStatus, long *pBrightness);
	int LightOnOff(int ch, tLightStatus lsStatus);
	int SendLightOnOffCMD(int nCh, char chCMD);
	int SendLightCMD(int nCh, char chCMD, int Value);
	int SendLightBrightness(int pBrightness[4]);
	int SendLightBrightness(int ch, int iBrightness);
};