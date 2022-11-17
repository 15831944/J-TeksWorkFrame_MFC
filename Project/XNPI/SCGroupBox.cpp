// SCGroupBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "SCGroupBox.h"


// CSCGroupBox

IMPLEMENT_DYNAMIC(CSCGroupBox, CStatic)

CSCGroupBox::CSCGroupBox()
{

}

CSCGroupBox::~CSCGroupBox()
{
}


BEGIN_MESSAGE_MAP(CSCGroupBox, CStatic)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CSCGroupBox 메시지 처리기입니다.




BOOL CSCGroupBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);

	CBrush brushBlue(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&brushBlue);

	pDC->Rectangle(rect);
	pDC->SelectObject(pOldBrush);

	return TRUE;
	return CStatic::OnEraseBkgnd(pDC);
}
