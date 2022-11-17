#pragma once

/************************************************************************/
/* @brief BrightNormalize관련 구조체
/************************************************************************/
struct tBNState
{
	BOOL bBrightNormalize;
	BOOL bCutHigh;
	BOOL bCutLow;
	long uiBNLimit;
};

