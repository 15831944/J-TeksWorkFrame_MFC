#pragma once
#include"MilBase.h"

/*!
\brief 
* Mil의 Edge Finder관련 함수를 Class화함
@todo PCB이외의 검사등을 위해 작업해야 함.
@author 전병근
*/
class MIL_DLLDECL CMilEdgeFinder:public CMilBase
{
public:
	CMilEdgeFinder(void);
	~CMilEdgeFinder(void);
	int Draw(void);
};
