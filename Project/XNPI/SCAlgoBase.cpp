#include "stdafx.h"
#include "SCAlgoBase.h"

CSCAlgoBase::CSCAlgoBase()
	: m_pParamDlg(NULL)
	, m_pMilBuffORG(NULL)
	, m_pMilBuffView(NULL)
	, m_bIsPixelCal(FALSE)
{
}


CSCAlgoBase::~CSCAlgoBase()
{
}


void CSCAlgoBase::SetParamDlg(CSCAlgoDialog * pParamDlg)
{
	m_pParamDlg = pParamDlg;
}


CDialog * CSCAlgoBase::GetParamDlg()
{
	return m_pParamDlg;
}


void CSCAlgoBase::SetMilBuffer(CMilBuffer * pMilCamProc, CMilBuffer * pMilCamDraw)
{
	m_pMilBuffORG = pMilCamProc;
	m_pMilBuffView = pMilCamDraw;
}

int CSCAlgoBase::Inspect(CMilBuffer * pMilORG, CMilBuffer * pMilView, CRect * pRectInsp)
{
	m_pMilBuffORG = pMilORG;
	m_pMilBuffView = pMilView;
	m_rectInsp = *pRectInsp;
	return 0;
}




void CSCAlgoBase::SetPixelCalData(CdblSize dblSize)
{
	if (dblSize.dblCX == 0.0) { m_bIsPixelCal = FALSE; return; }
	if (dblSize.dblCY == 0.0) { m_bIsPixelCal = FALSE; return; }
	m_bIsPixelCal = TRUE;
	m_dblPixelCalData = dblSize;
}
