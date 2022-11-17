#pragma once
#include <UniIOPort.h>
struct tOSubSeq
{
	long CapDone;
	long NG;
};

struct tOSeq
{
	tOSubSeq Pos[6];
	long UnitInspDone;
};

struct tOStage
{
	long VisionReady;
	long VisionNotReady;
	tOSeq Seq;
};

struct tIStage
{
	long MANUAL;
//	long EMG;
//	long ERR;
	long RUN;
	long IdleReset;
	long UnitStart;
	long MotionDone[6];
};

class CSCInspIO :
	public CIOPort
{
public:
	CSCInspIO();
	~CSCInspIO();
	BOOL GetEMG(int *pErrStatgeNo);
	void LoadEnv();
	static CSCInspIO * Instance();
	static CSCInspIO *m_pInstance;
	static CIOPort *m_pBoard;
	static CString m_strBoardName;

//Input
	tIStage m_nIStage[2];

//Output
	tOStage m_nOStage[2];
	int m_nOSpare0;
	int m_nOSpare31;

	void LoadPortNo();
	int Init();
	void SetCapDone(int nStage, int nPos, BOOL bValue);
	void SetNG(int nStage, int nPos, BOOL bValue);
	void SetInspDone(int nStage, BOOL bValue);
	BOOL IsEMG(int nStage=-1);
	BOOL IsERR(int nStage=-1);
	BOOL IsRun(int nStage=-1);
	BOOL IsManual(int nStage =-1);
	BOOL GetIdleReset(int nStage);
	int SetVisionReady(int nStage, BOOL bState);
	BOOL IsUnitStart(int nStage);
	BOOL IsMotionDone(int nStage, int nPos);
	int m_nCountOfStage;
	BOOL GetErr(int* pErrStageNo);
	void Close();
	void SetAllOutputClear();
	BOOL GetMotionDone(int nStage, int *pErrStageNo);
};

