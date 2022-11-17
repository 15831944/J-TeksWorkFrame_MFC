// NumEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "NumEdit.h"


// CNumEdit

IMPLEMENT_DYNAMIC(CNumEdit, CEdit)

CNumEdit::CNumEdit()
{

}

CNumEdit::~CNumEdit()
{
}


BEGIN_MESSAGE_MAP(CNumEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CNumEdit 메시지 처리기입니다.




void CNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (
		nChar == VK_RETURN || nChar == VK_BACK ||  // 백스페이스 허용
		(nChar == _T('.') || (_T('0') <= nChar && nChar <= _T('9'))) && // '-'부호와 0 ~ 9 문자 허용
		GetWindowTextLength() <= 8  // 길이 3으로 제한
		)
	{
		if (nChar == _T('-')) // '-' 중복제한
		{
			CString str;
			GetWindowText(str);
			if (str.GetLength() >= 1 && str[0] == _T('-')) return;
		}
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
