#pragma once
#include "ioport.h"

//#ifdef _MMC_HALF
//#include "MMCWHP201.h"
//#else
//#include "MMCWFP39.h"
//#endif

#define BIT_REV

class CMMCIOPortH100 :public CIOPort
{
public:
	CMMCIOPortH100(void);
	~CMMCIOPortH100(void);
	virtual int SetPort(int port, long value);
	virtual int GetPort(int nPort, long * pValue);
	virtual int GetOutPort(int nPort, long * pValue);
	virtual int SetBit(int bitNo, int value);
	virtual BOOL GetBit(int nBitNo);
	virtual BOOL GetOutBit(int nBitNo);
	virtual void LoadEnv(void);
};
