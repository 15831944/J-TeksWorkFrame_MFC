// LedONOFF.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "LedONOFF.h"


// CLedONOFF

IMPLEMENT_DYNAMIC(CLedONOFF, CProgressCtrl)

CLedONOFF::CLedONOFF()
{

}

CLedONOFF::~CLedONOFF()
{
}


BEGIN_MESSAGE_MAP(CLedONOFF, CProgressCtrl)
END_MESSAGE_MAP()



// CLedONOFF 메시지 처리기입니다.




void CLedONOFF::Set(bool bLedStatus)
{
	SetWindowTheme(GetSafeHwnd(), L"", L"");
	if(bLedStatus)
		SetBarColor(RGB(100, 200, 0));
	else
		SetBarColor(RGB(255, 0, 0));

	SetRange(0, 1);
	SetPos(1);

}


BOOL CLedONOFF::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	SetWindowTheme(GetSafeHwnd(), L"", L"");
	SetBarColor(RGB(255, 0, 0));
	SetBkColor(RGB(0, 255, 255));
	SetRange(0, 1);
	SetPos(1);

	return CProgressCtrl::Create(dwStyle, rect, pParentWnd, nID);

}
