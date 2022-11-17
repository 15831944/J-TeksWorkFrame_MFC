#pragma once
#include "ioport.h"


#ifdef _UNIIOPORT_DLL
#ifdef _WIN64
#pragma comment(lib, "C:/Program Files (x86)/EzSoftware UC/AXL(Library)/Library/64Bit/AxL.lib")
#else
#pragma comment(lib, "C:/Program Files (x86)/EzSoftware UC/AXL(Library)/Library/32Bit/AxL.lib")
#endif
#endif


class UNIIOPORT_DLLDECL CAjinIOPort : public CIOPort
{
public:
	CAjinIOPort(void);
	~CAjinIOPort(void);
	virtual int SetPort(int port, long value);
	virtual int GetPort(int nPort, long * pValue);
	virtual int GetOutPort(int nPort, long * pValue);
	virtual int SetBit(int bitNo, int value);
	virtual BOOL GetBit(int nBitNo);
	virtual BOOL GetOutBit(int nBitNo);
	int Init();
	void Close();
	int SetOffset();
	int m_nInputModule;
	int m_nOutputModule;
};
