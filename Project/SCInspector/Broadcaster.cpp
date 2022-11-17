// Broadcaster.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "Broadcaster.h"


// CBroadcaster

CBroadcaster::CBroadcaster()
{
}

CBroadcaster::~CBroadcaster()
{
}


// CBroadcaster 멤버 함수

void CBroadcaster::RegObserver(CObserver* pObserver)
{
	m_ListObservers.push_front(pObserver);
}

void CBroadcaster::UnRegObserver(CObserver* pObserver)
{
	list<CObserver*>::iterator iter;
	for(iter=m_ListObservers.begin();iter != m_ListObservers.end(); iter++)
		if(*iter == pObserver)iter=m_ListObservers.erase(iter);
}

void CBroadcaster::Notify(void)
{
	list<CObserver*>::iterator iter;
	for(iter=m_ListObservers.begin();iter != m_ListObservers.end(); iter++)
		(*iter)->Update();
}
