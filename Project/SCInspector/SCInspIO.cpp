#include "stdafx.h"
#include "Defines.h"
#include "SCInspIO.h"

CSCInspIO * CSCInspIO::m_pInstance = NULL;
CString CSCInspIO::m_strBoardName = _T("Ajin");
CIOPort * CSCInspIO::m_pBoard = NULL;
CSCInspIO::CSCInspIO()
	: m_nCountOfStage(0)
{
}


CSCInspIO::~CSCInspIO()
{
}

BOOL CSCInspIO::GetMotionDone(int nStage, int *pErrStageNo)
{
	BOOL rv = FALSE;
	for (int i = 0; i < 6; i++)
	{
		if (IsMotionDone(nStage, i))
		{
			*pErrStageNo = i;
			rv = TRUE;
			break;
		}
	}
	return rv;
}

BOOL CSCInspIO::GetEMG(int *pErrStageNo)
{
	BOOL rv = FALSE;
	for (int i = 0; i < 3; i++)
	{
		if (IsEMG(i))
		{
			*pErrStageNo = i;
			rv = TRUE;
			break;
		}
	}
	return rv;
}


BOOL CSCInspIO::GetErr(int* pErrStageNo)
{
	BOOL rv = FALSE;
	for (int i = 0; i < 3; i++)
	{
		if (IsERR(i))
		{
			*pErrStageNo = i;
			rv = TRUE;
			break;
		}
	}
	return rv;
}


void CSCInspIO::LoadEnv()
{
	LoadPortNo();
	return;
}


CSCInspIO * CSCInspIO::Instance()
{
	if (m_pInstance)return m_pInstance;

	m_pInstance = new CSCInspIO;
	m_pInstance->m_pBoard = CIOPort::Instance(m_strBoardName.GetBuffer());

	return m_pInstance;
}

#define m_nOStageGetDef(stage, base, n) \
m_nOStage[stage].Seq.Pos[n].CapDone	= pMyCfg->Get(base + (long)n*2, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-"#stage"-.Seq.Pos-"#n"-.CapDone"));\
m_nOStage[stage].Seq.Pos[n].NG		= pMyCfg->Get(base+ 1 + (long)n*2, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-"#stage"-.Seq.Pos-"#n"-.NG"));

#define m_nIStageGetDef(stage, base, n) \
m_nIStage[stage].MotionDone[n]		= pMyCfg->Get(base+(long)n, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-"#stage"-.MotionDone-"#n"-"));
void CSCInspIO::LoadPortNo()
{
	CString MachineInputName, MachineOutputName, MachineInputLableName, MachineOutputLabelName, ProductName;

	CCfgFile* pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pMyCfg->Open();
	pMyCfg->SetDepth(2);

	ProductName = PRODUCT_NAME;
	MachineInputName = ProductName + _T("_INPUTSET");
	MachineOutputName = ProductName + _T("_OUTPUTSET");
	MachineInputLableName = MachineInputName + _T("_NAME");
	MachineOutputLabelName = MachineOutputName + _T("_NAME");

	// Input 
	m_nIStage[0].MANUAL = pMyCfg->Get(1L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.MANUAL"));
	//m_nIStage[0].ERR = pMyCfg->Get(1L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.ERR"));
	m_nIStage[0].RUN = pMyCfg->Get(3L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.RUN"));
	m_nIStage[0].IdleReset = pMyCfg->Get(4L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.IdleReset"));

	m_nIStage[0].UnitStart = pMyCfg->Get(5L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.UnitStart"));

	m_nIStageGetDef(0, 6, 0);
	m_nIStageGetDef(0, 6, 1);
	m_nIStageGetDef(0, 6, 2);
	m_nIStageGetDef(0, 6, 3);
	m_nIStageGetDef(0, 6, 4);
	m_nIStageGetDef(0, 6, 5);

	m_nIStage[1].MANUAL = pMyCfg->Get(12L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.MANUAL"));
	m_nIStage[1].RUN = pMyCfg->Get(14L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.RUN"));
	m_nIStage[1].IdleReset = pMyCfg->Get(15L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.IdleReset"));

	m_nIStage[1].UnitStart = pMyCfg->Get(16L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.UnitStart"));

	m_nIStageGetDef(1, 17, 0);
	m_nIStageGetDef(1, 17, 1);
	m_nIStageGetDef(1, 17, 2);
	m_nIStageGetDef(1, 17, 3);
	m_nIStageGetDef(1, 17, 4);
	m_nIStageGetDef(1, 17, 5);

	// Output 
	m_nOSpare0 = pMyCfg->Get(0L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOSpare0"));
	m_nOStage[0].VisionReady = pMyCfg->Get(1L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-0-.VisionReady"));
	m_nOStage[0].VisionNotReady = pMyCfg->Get(2L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-0-.VisionNotReady"));

	m_nOStageGetDef(0, 3, 0);
	m_nOStageGetDef(0, 3, 1);
	m_nOStageGetDef(0, 3, 2);
	m_nOStageGetDef(0, 3, 3);
	m_nOStageGetDef(0, 3, 4);
	m_nOStageGetDef(0, 3, 5);

	m_nOStage[0].Seq.UnitInspDone = pMyCfg->Get(15L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-0-.Seq.UnitInspDone"));

	m_nOStage[1].VisionReady = pMyCfg->Get(16L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-1-.VisionReady"));
	m_nOStage[1].VisionNotReady = pMyCfg->Get(16L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-1-.VisionNotReady"));

	m_nOStageGetDef(1, 18, 0);
	m_nOStageGetDef(1, 18, 1);
	m_nOStageGetDef(1, 18, 2);
	m_nOStageGetDef(1, 18, 3);
	m_nOStageGetDef(1, 18, 4);
	m_nOStageGetDef(1, 18, 5);

	m_nOStage[1].Seq.UnitInspDone = pMyCfg->Get(30L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-1-.Seq.UnitInspDone"));
	m_nOSpare31 = pMyCfg->Get(31L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOSpare31"));
}

int CSCInspIO::Init()
{
	LoadEnv();
	m_pBoard->Init();
	return 0;
}

BOOL CSCInspIO::IsMotionDone(int nStage, int nPos)
{
#ifdef IGNORE_IO
	return TRUE;
#else
	return m_pBoard->GetBit(m_nIStage[nStage].MotionDone[nPos]);
#endif
}

void CSCInspIO::SetCapDone(int nStage, int nPos, BOOL bValue)
{
	m_pBoard->SetBit(m_nOStage[nStage].Seq.Pos[nPos].CapDone, bValue);
}

void CSCInspIO::SetNG(int nStage, int nPos, BOOL bValue)
{
	m_pBoard->SetBit(m_nOStage[nStage].Seq.Pos[nPos].NG, bValue);
}

void CSCInspIO::SetInspDone(int nStage, BOOL bValue)
{
	m_pBoard->SetBit(m_nOStage[nStage].Seq.UnitInspDone, bValue);
}

BOOL CSCInspIO::IsEMG(int nStage)
{
	//if (nStage != -1)
	//	return m_pBoard->GetBit(m_nIStage[nStage].EMG);
	//else
	//{
	//	for (int i = 0; i < m_nCountOfStage; i++)
	//		if (m_pBoard->GetBit(m_nIStage[nStage].EMG))return TRUE;
	//}
	return FALSE;
}

BOOL CSCInspIO::IsERR(int nStage)
{
	//if (nStage != -1)
	//	return m_pBoard->GetBit(m_nIStage[nStage].ERR);
	//else
	//{
	//	for (int i = 0; i < m_nCountOfStage; i++)
	//		if (m_pBoard->GetBit(m_nIStage[nStage].ERR))return TRUE;
	//}
	return FALSE;
}

BOOL CSCInspIO::IsManual(int nStage)
{
	if (nStage != -1)
		return m_pBoard->GetBit(m_nIStage[nStage].MANUAL);
	else
	{
		for (int i = 0; i < m_nCountOfStage; i++)
			if (m_pBoard->GetBit(m_nIStage[nStage].MANUAL))return TRUE;
	}
	return FALSE;
}

BOOL CSCInspIO::IsRun(int nStage)
{
	BOOL result = m_pBoard->GetBit(m_nIStage[nStage].RUN);
	return result;
}


BOOL CSCInspIO::GetIdleReset(int nStage)
{
	return m_pBoard->GetBit(m_nIStage[nStage].IdleReset);
}


int CSCInspIO::SetVisionReady(int nStage, BOOL bState)
{
	if (bState)
	{ //Ready
		m_pBoard->SetBit(m_nOStage[nStage].VisionReady, TRUE);
		m_pBoard->SetBit(m_nOStage[nStage].VisionNotReady, FALSE);
	}
	else
	{ //Not Ready
		m_pBoard->SetBit(m_nOStage[nStage].VisionReady, FALSE);
		m_pBoard->SetBit(m_nOStage[nStage].VisionNotReady, TRUE);
	}
	return 0;
}


BOOL CSCInspIO::IsUnitStart(int nStage)
{
#ifdef IGNORE_IO
	return TRUE;
#else
	return m_pBoard->GetBit(m_nIStage[nStage].UnitStart);
#endif
}




void CSCInspIO::Close()
{
	m_pBoard->Close();
}



void CSCInspIO::SetAllOutputClear()
{
	for(int i=0;i<31;i++)
		m_pBoard->SetBit(i, 0);
}
