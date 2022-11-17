#pragma once
#include "SCAlgoBase.h"
#include "atltypes.h"
#include <UniUtility.h>

class CSCFidAlgo :
	public CSCAlgoBase
{
public:
	CSCFidAlgo();
	~CSCFidAlgo();
	CMilBuffer *m_pMilBuffer;
	CMilModFinder m_modFinder;
	CMilContext m_MilContext;
	CMilResult m_milResult;
	int Reg(CMilBuffer * pMilBuffer, CRect rectFid);
	int RemoveAllModels();
	int m_nFidCnt;
	BOOL IsEmpty();
	int Find(CMilBuffer * pMilBuffer);
	void ApplyParamToAlgo(void);
	void GetOffset(tModResult * pResult);
	int Draw(BOOL bIsFinded=TRUE);
	int SetDrawOffset(CSize Offset = CSize(M_DEFAULT, M_DEFAULT));
	CdblPoint m_upOffset;
	CdblPoint m_umOffset;
	CdblSize m_dblPixelScale;
	int m_iUseScale;
	int m_iUseAngle;
	int m_iAngleRate;
	int m_iScaleRate;
	int m_iMatRate;
	void CloseMod();
	int SaveFid(TCHAR * szModelPath, int index);
	int LoadFid(TCHAR * szModelPath, int index);
	int m_nCamNo;
	CdblPoint m_ptFidROICenterOffset;
	CdblSize m_SearchRangeFactor;
	CRect m_rectFidSearchRange;
};

