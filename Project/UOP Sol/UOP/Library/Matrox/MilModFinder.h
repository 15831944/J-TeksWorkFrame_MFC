#pragma once
#include <mil.h>
#include "MilBase.h"
#include "MilBuffer.h"
#include "MilResult.h"

class CMilContext;

/*!
\brief 
* Model Finder의 결과를 받기 위한 구조체
@todo  CMilResult에 병합되어야 하는데 일정때문에 분리하여 작업하였음
@author 전병근
*/
struct tModResult{
	long NumResults;
	CSize ImageSize;
	CdblPoint Pos;
	double Angle;
	double Scale;
	double Score;
};


//* Alloc과 Close에서 Context와 Result의 Alloc과 Free를 실행한다.
/*!
\brief 
* Mil의 Model Finder관련  함수군을 Class화하였다
@author 전병근
*/
class MIL_DLLDECL CMilModFinder:public CMilBase
{
public:
	enum tModTargetType{MOD_TARGET_RST, MOD_TARGET_CXT};
	CMilModFinder(void);
	~CMilModFinder(void);
	MIL_ID m_mIlIDSystem;
	CMilContext * m_pMilContext;
	CMilBuffer * m_pMilBuffer;
	CMilResult * m_pMilResult;
	int SetContext(CMilContext *pMilContext);
	int GetFeature(void);
	int GetFeature(CRect aoiRect);
	int DelAllFeature(int Index);
	int SetBuffer(CMilBuffer *pImage);
	int SetResult(CMilResult *pResult);
	int Define(long ltype, double dblParam1, double dblParam2, double dblParam3, double dblParam4, double dblParam5);
	int SetAOI(void);
	int SetAOI(CRect aoiRect);
	int Control(void);
	int PreProcess();
	int Find(tModResult *pResult);
	int Find(CMilResult *pResult);
	int DrawContext(void);
	int SetDrawOffset(CSize Offset=CSize(M_DEFAULT, M_DEFAULT));
	int GetDrawBoxOffset(CdblSize *pOffset);
	int DrawResult(void);
	int Alloc(void);
	int Control(long Index, long CtrlType, double CtrlValue, tModTargetType Target=MOD_TARGET_CXT);
	int SetSpeed(long lSpeed=M_VERY_HIGH);
	int SetSearchAngleRange(double dblScore=M_DEFAULT);
	int SetSearchScaleRange(double dblScore=M_DEFAULT);
	int SetAccptanceScore(double dblScore);
	int Close(void);
};
