#pragma once
#include "MilBase.h"

/*!
\brief 
* Mil의 각종결과를 받아 원하는 형태로 가공하여 제공하는 Class화함
@todo  아직 완성된것이 아님 추가작업필요
@author 전병근
*/
class MIL_DLLDECL CMilResult:public CMilBase
{
public:
	CMilResult(void);
	~CMilResult(void);
	MIL_ID m_milIDResult;
	int GetIDResult(void);
	int ModAlloc(void);
	int ModFree(void);
	int Close(void);
	BOOL m_fModAlloc;
};
