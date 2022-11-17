#pragma once
#include "stdafx.h"
#include <list>
#include "Observer.h"
// CBroadcaster 명령 대상입니다.
using namespace std;

class CBroadcaster : public CWnd
{
public:
	CBroadcaster();
	virtual ~CBroadcaster();
	void RegObserver(CObserver* pObserver);
	void UnRegObserver(CObserver* pObserver);
	void Notify(void);
protected:
	list<CObserver *> m_ListObservers;
};


