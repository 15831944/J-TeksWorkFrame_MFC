#include "StdAfx.h"
#include "ConfigInfo.h"

CConfigInfo* CConfigInfo::m_pInstance=NULL;
CConfigInfo::CConfigInfo(void)
: m_strSetupPath(_T(""))
, m_strModelPath(_T(""))
{
}

CConfigInfo::~CConfigInfo(void)
{
}

CConfigInfo* CConfigInfo::Instance(void)
{
	if(!m_pInstance)m_pInstance=new CConfigInfo;
	return m_pInstance;
}

int CConfigInfo::Init(void)
{
	TCHAR szInitDir[256];
    CCfgFile* pMyCfg=CCfgFile::Instance();
	pMyCfg->Open();
	m_lstAlgoNames.RemoveAll();
   
    CString XrayPara, MorionPara,IOboardPara; 
	CString strAlgoTag;
	CString strAlgoName;

	XrayPara = pMyCfg->Get(_T("None"), _T("ConfigSet"), _T("Tubetype"));
	MorionPara = pMyCfg->Get(_T("None"), _T("ConfigSet"), _T("MOTION"));
	IOboardPara = pMyCfg->Get(_T("None"), _T("ConfigSet"), _T("IOBOARD"));
	m_PulseForUm = pMyCfg->Get(1L,_T("ConfigSet"),_T("PulseForUm")) ;
	m_strModelPath = pMyCfg->Get(_T("D:\\Model9800"), _T("ConfigSet"), _T("ModelPath"));
	m_LangDIR= pMyCfg->Get(_T("NONE"), _T("ConfigSet"), _T("LangDIR"));
	
	int nAlgo = pMyCfg->Get(0L, _T("Algorithm"), _T("AlgoCount"));
	for(int i=0;i<nAlgo;i++)
	{
		strAlgoTag.Format(_T("Algo%02d"), i);
		strAlgoName=pMyCfg->Get(_T("None"), _T("Algorithm"), strAlgoTag.GetBuffer());
		m_lstAlgoNames.AddTail(strAlgoName);
	}

	pMyCfg->Flush();

	return 0;
}

CString CConfigInfo::GetSetupPath(void)
{
    CCfgFile* pMyCfg=CCfgFile::Instance();
	return pMyCfg->GetPath();
}

CString CConfigInfo::GetModelRepository(void)
{
	return m_strModelPath;
}
int CConfigInfo::LoadInfo(void)
{
	return 0;
}

int CConfigInfo::SaveInfo(void)
{
	return 0;
}

