#pragma once

// CObserver 명령 대상입니다.

class CObserver : public CObject
{
public:
	CObserver();
	virtual ~CObserver();
	void Update(void);
};


