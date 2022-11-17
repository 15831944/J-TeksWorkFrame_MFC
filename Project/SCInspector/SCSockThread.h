#pragma once



// CSCSockThread

class CSCSockThread : public CWinThread
{
	DECLARE_DYNCREATE(CSCSockThread)

protected:
	CSCSockThread();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSCSockThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


