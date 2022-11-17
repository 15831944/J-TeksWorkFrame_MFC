#pragma once
#include <UniIOPort.h>
class CCamDlg;

/** 
\brief 
I/O방식이던 켄베이어컨방식이던 장제 작업 Process를 제어하는 Class로 Thread처리가 되어 있어 backgroud로 컨베이어 센서혹은 WT과의 통신, 감시 및 각종 모드별로 동작을 정의해둔다.@n
검사구간 센서를 만나면 Grab sequence를 진행한다.@n
\date 2017.08.31
\author 전 병근
*/
class CSCProcessCtrl
{
public:
	enum tSCInspMachineType{MA_1, MA_NONE};
	enum tProcessMode{PROCESS_STOP, PROCESS_INSP};
	CSCProcessCtrl(void);
	~CSCProcessCtrl(void);
	virtual int CheckAndDo(void);
	virtual HANDLE MakeWaitableTimer(int setTime);
	HANDLE m_hWaitableTimer;
	static CSCProcessCtrl* Instance(tSCInspMachineType nType=MA_1);
	static UINT ThreadProc(LPVOID pParam);
	static CSCProcessCtrl *m_pInstance;
	int m_nCountOfCam;
	int m_Run;
	int InitThread(void);
	void StageProcess(int nStage, int *piStageStep);
	void ManStageProcess(int nStage, int * piStageStep);
	void AutoStageProcess(int nStage, int * piStageStep);
	int IsExitCdt(int nStage);
	CCamDlg **m_pCamDlg;
	int Stop();
	void StageProcessClear(int nStage);
	int DoInspect(int nStage, int nPos, BOOL bSave);
};
