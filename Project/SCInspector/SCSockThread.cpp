// SCSockThread.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "SCSockThread.h"


// CSCSockThread

IMPLEMENT_DYNCREATE(CSCSockThread, CWinThread)

CSCSockThread::CSCSockThread()
{
}

CSCSockThread::~CSCSockThread()
{
}

BOOL CSCSockThread::InitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 초기화를 수행합니다.
	return TRUE;
}

int CSCSockThread::ExitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 정리를 수행합니다.
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSCSockThread, CWinThread)
END_MESSAGE_MAP()


// CSCSockThread 메시지 처리기입니다.
