#pragma once
#include <Matrox.h>
#include "algorithmbase.h"

struct tFidCamCdt
{
	BOOL bImgEnhance;
	tBNState bnState;
	BOOL bScrnNormalize;
};
class CFidAlgo :
	public CAlgorithmBase
{
	DECLARE_DYNAMIC(CFidAlgo)
public:
	CFidAlgo(void);
	~CFidAlgo(void);
	int SaveParam(CXmlNode* pNode);
	int LoadParam(CXmlNode* pNode);
	int m_nCamNo; //Xray, AOI
	int m_nFidCnt;
	int m_iMatRate;
	int m_iFidKind;/// Normal, Hal
	int m_iFidType;/// Circle Rectangle
	int m_iUseScale;
	int m_iUseAngle;
	int m_iScaleRate;
	int m_iAngleRate;
	int m_iSizeLLimit;
	int m_iSizeULimit;
	CdblSize m_dblPixelScale;
	CMilBuffer *m_pMilBuffer[4];
	CMilContext m_MilContext[4];
	CdblAxisPos m_lFidPos[4];
	CdblAxisPos m_upOffset[4];  ///Unit:Pixel offset 
	CdblAxisPos m_umOffset[4];	///Unit:uM offset
	CSize m_upFid[4];
	CSize m_upROI[4];			///ROI Area Information
	CPoint m_lCurFidPos[4];
	double m_theta;
	tFidCamCdt m_CamStatus[4];
	CMilResult m_milResult;
	CMilModFinder m_modFinder;
	int RemoveAllModels(int FidIndex);
	int Reg(int FidIndex, CMilBuffer* pMilBuffer, CAxisPos *pPos);
	double CalcTheta();
	double CalcVectorTheta(CPoint origin, CPoint Pos1, CPoint Pos2);
	int AdjustPos(CdblAxisPos *pModified, CdblAxisPos *pOriginal);
	void GetAxisPos(int FidIndex, CdblAxisPos *pPos);
	void GetAxisOffset(int FidIndex, tModResult *pResult);
	void SetPixelScale(CdblSize dblSize);
	BOOL IsEmpty(void);
	void ApplyParamToAlgo(void);
	int SetDrawOffset(CSize Offset=CSize(M_DEFAULT, M_DEFAULT));
	int Find(int FidIndex, CMilBuffer* pMilBuffer);
	int LoadFid(TCHAR * szModelPath);
	int SaveFid(TCHAR * szModelPath);
	int Init(void);
	int Draw(void);
	int Free(void);
	void UnloadFid(void);
};
