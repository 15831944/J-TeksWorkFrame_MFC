#pragma once
#include "SCAlgoDialog.h"
#include "atltypes.h"
class CSCAlgoBase
{
public:
	CSCAlgoBase();
	~CSCAlgoBase();
	virtual void SetParamDlg(CSCAlgoDialog * pParamDlg);
	virtual CDialog * GetParamDlg();
	int m_nInspType;
	CSCAlgoDialog *m_pParamDlg;
	virtual int Inspect(CMilBuffer * pMilORG, CMilBuffer * pMilView, CRect * pRectInsp);
	void SetMilBuffer(CMilBuffer * pMilCamProc, CMilBuffer * pMilCamDraw);
	CMilBuffer *m_pMilBuffORG;
	CMilBuffer *m_pMilBuffView;
	CRect m_rectInsp;
	void SetPixelCalData(CdblSize dblSize);
	CdblSize m_dblPixelCalData;
	BOOL m_bIsPixelCal;
};

