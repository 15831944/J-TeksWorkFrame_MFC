#pragma once
#include "Mil.h"
#include "MilBase.h"

/*!
\brief 
* Mil의 Application관련 함수를 Class화함
@warning 어떤 Mil함수를 사용하더라도 먼저 Application초기화 되어야함
@author 전병근
*/
class MIL_DLLDECL CMilApplication:public CMilBase
{
public:
	CMilApplication(void);
	~CMilApplication(void);
	int Create(void);
	void Close(void);
};
