#pragma once
#include <list>
#include "Observer.h"
// CBroadcaster 명령 대상입니다.

class CBroadcaster : public CObject
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


