#pragma once
#include "stdafx.h"

enum tBaudRate { B9600, B19200, B38400, B115200 };

struct tComPortInfo
{
	tBaudRate BaudRate;
};

class CSetup
{
public:
	CSetup();
	~CSetup();
	int GetComPortInfo(tComPortInfo portInfo);
};
