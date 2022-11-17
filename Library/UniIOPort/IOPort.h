#pragma once

#ifdef _UNIIOPORT_DLL
#define UNIIOPORT_DLLDECL __declspec(dllexport)
#else
#define UNIIOPORT_DLLDECL __declspec(dllimport)
#ifndef _WIN64
#ifndef _DEBUG
#pragma comment(lib, "UniIOPort32")
#else
#pragma comment(lib, "UniIOPortD32")
#endif
#else
#ifndef _DEBUG
#pragma comment(lib, "UniIOPort64")
#else
#pragma comment(lib, "UniIOPortD64")
#endif
#endif
#endif

#define DIO_INPUT	0
#define DIO_OUTPUT	1

struct tIOName
{
	int* pnNum;
	CString strLabelName;
};

 #include "UniUtility.h"
class UNIIOPORT_DLLDECL CIOPort
{
public:
	enum _tSrc {SRC_UP, SRC_STOP, SRC_DOWN};
	enum tPORTSTATE{PORTOFF, PORTON};
	enum tTOWERLAMP{TLAMP_RED, TLAMP_YELLOW, TLAMP_GREEN};
	CIOPort(void);
	~CIOPort(void);
	virtual int SetPort(int port, long value);
	virtual int GetPort(int nPort, long * pValue);
	virtual int GetOutPort(int nPort, long * pValue);
	virtual int SetBit(int bitNo, int value);
	static CIOPort * Instance(TCHAR * szBoardName=NULL);
	static CIOPort * m_pInstance;
 	virtual int Init(void);
	virtual BOOL GetBit(int nBitNo);
	virtual BOOL GetOutBit(int nBitNo);
	virtual void InitIOData(void);
	CString GetBoardName(void);

	/************input IO ***************/
   tIOName	m_IOInput[32];
     
    int inputio0;
	int inputio1;
	int inputio2;
	int inputio3;
	int inputio4;
	int inputio5;
	int inputio6;
	int inputio7;
	int inputio8;
	int inputio9;
	int inputio10;
	int inputio11;
	int inputio12;
	int inputio13;
	int inputio14;
	int inputio15;
	int inputio16;
	int inputio17;
	int inputio18;
	int inputio19;
	int inputio20;
	int inputio21;
	int inputio22;
	int inputio23;
	int inputio24;
	int inputio25;
	int inputio26;
	int inputio27;
	int inputio28;
	int inputio29;
	int inputio30;
	int inputio31;
	

	/************output***************/

	 tIOName	m_IOOutput[32];

    int outputio0;
	int outputio1;
	int outputio2;
	int outputio3;
	int outputio4;
	int outputio5;
	int outputio6;
	int outputio7;
	int outputio8;
	int outputio9;
	int outputio10;
	int outputio11;
	int outputio12;
	int outputio13;
	int outputio14;
	int outputio15;
	int outputio16;
	int outputio17;
	int outputio18;
	int outputio19;
	int outputio20;
	int outputio21;
	int outputio22;
	int outputio23;
	int outputio24;
	int outputio25;
	int outputio26;
	int outputio27;
	int outputio28;
	int outputio29;
	int outputio30;
	int outputio31;
	int outputio32;
	
 	CString m_strBoardName;
	virtual void Close();
};
