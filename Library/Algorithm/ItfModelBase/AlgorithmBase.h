#pragma once
#include "modelbase.h"
#include <Matrox.h>
#include "ParamsInterface.h"

#define ALGO_FRAME 1
#define ALGO_COMPO 2
#define ALGO_COMBI 4

class CInspResult;
class CFrame;

#define RV_SHOW_RESULT	0x00000001
#define RV_SHOW_DIM		0x00000002

class ITFBASEDLLDECL CAlgorithmBase :
	public CModelBase
{
	DECLARE_DYNAMIC(CAlgorithmBase)
public:
	CAlgorithmBase(void);
	~CAlgorithmBase(void);
	virtual int Free(void);
	virtual int CalcFeature(void);
	virtual int DrawFeature(int drawOption=0);
	virtual int Inspect(void);
	virtual int Inspect(CPtrList * pLstAlgo);
	virtual int Inspect(CRect inspRect, CInspResult * pResult);
	virtual int Inspect(int SelectedCompNo, CInspResult *pResult);
	virtual int Update(CPtrList *pList);
	virtual int GetResultStr(CString &strResult);
	virtual SHORT GetDefectType(void);
	virtual int GetResults(CInspResult* pInspResult);
	virtual int ReleaseResults(CInspResult * pInspResult);
	virtual int GetBaseType(void){return typeALGO;}

	void SetInput(CMilBuffer *pMilImage);
	int SetOutput(CMilBuffer * pMilImage);
protected:
	CPtrList m_ListParams;///Parameter List¸¦ »ðÀÔÇÏ´Â 
	CMilBuffer *m_pInputImg;
	void *m_pInputImgRaw;
	CMilBuffer *m_pProcessImg;
public:
	int SetParams(CPtrList* pListParams);
	CParamsInterface *m_pParamsInterface;
	bool m_bIsInspDefect;
	void SetOverlayDC(HDC* pDC);
	HDC* m_pDC;
	CString GetAlgoName(void);
};
