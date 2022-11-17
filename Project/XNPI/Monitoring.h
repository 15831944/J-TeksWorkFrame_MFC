#pragma once

/**
\brief 시스템 전체적으로 모니터링하는 업무를 맡게 된다.Emergency같은 각종 상태를 감시하고 Alarm을 띄우는 등의 조치를 취한다.@n 
\date 2010.01.05
\author 전 병근
*/
class CMonitoring
{
public:
	CMonitoring(void);
	~CMonitoring(void);
	static UINT ThreadMonitoring(LPVOID pParam);
	int Start(void);
	static CMonitoring* Instance(void);
	static CMonitoring* m_pInstance;
	static BOOL m_bRun;
	void Stop(void);
};
