#pragma once
#include "MilClass.h"

/*!
\brief
* Mil의 Code(바코드 리더)관련 함수를 Class화함
@author 김인영
*/

class MIL_DLLDECL MilCode :public CMilBase
{
public:
	MilCode();
	~MilCode();

	MIL_ID m_milIDCode;
	int Alloc();
	int Free();
	long m_lCodeType;

};

