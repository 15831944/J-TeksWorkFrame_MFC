#pragma once
#include "modelbase.h"
#include "afxcoll.h"
#include <Matrox.h>
#include "ParamsInterface.h"

class CCompoRect;
class CInspResult;
class CFrame;

#define RV_SHOW_RESULT	0x00000001
#define RV_SHOW_DIM		0x00000002

class CAlgorithmBase :
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
	virtual int Inspect(CFrame *pFrame);
	virtual int Inspect(int SelectedCompNo, CInspResult *pResult);

	void SetInput(CMilBuffer *pMilImage);
	int SetOutput(CMilBuffer * pMilImage);
	virtual int GetBaseType(void){return typeALGO;}
protected:
	CPtrList m_ListParams;///Parameter List¸¦ »ðÀÔÇÏ´Â 
	CMilBuffer *m_pInputImg;
	CMilBuffer *m_pProcessImg;
public:
	int SetParams(CPtrList* pListParams);
	virtual int Update(CPtrList *pList);
	CParamsInterface *m_pParamsInterface;
	virtual int GetResultStr(CString &strResult);
	virtual SHORT GetDefectType(void);
	virtual int GetResults(CInspResult* pInspResult);
	bool m_bIsInspDefect;
	virtual int ReleaseResults(CInspResult * pInspResult);
	void SetOverlayDC(HDC* pDC);
	HDC* m_pDC;
	CString GetAlgoName(void);
	int m_nInspType;
};
