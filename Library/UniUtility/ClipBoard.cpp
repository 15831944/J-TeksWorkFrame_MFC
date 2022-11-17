#include "stdafx.h"
#include "ClipBoard.h"


CClipBoard::CClipBoard()
{
}


CClipBoard::~CClipBoard()
{
}


void CClipBoard::CopyWndImgToClipboard(CWnd * pWnd)
{
	CBitmap 	bitmap;
	CClientDC	dc(pWnd);
	CDC 		memDC;
	CRect		rect;

	memDC.CreateCompatibleDC(&dc);

	pWnd->GetWindowRect(rect);

	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);

	pWnd->OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bitmap.GetSafeHandle());
	CloseClipboard();

	memDC.SelectObject(pOldBitmap);
	bitmap.Detach();
}

void CClipBoard::CopyWndImgToResizeClipboard(CWnd * pWnd, int iReportGraphWidth, int iReportGraphHeight)
{
	CBitmap 	bitmap;
	CClientDC	dc(pWnd);
	CDC 		memDC;
	CRect		rect;

	memDC.CreateCompatibleDC(&dc);

	pWnd->GetWindowRect(rect);

	bitmap.CreateCompatibleBitmap(&dc, iReportGraphWidth, iReportGraphHeight);

	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.StretchBlt(0, 0, iReportGraphWidth, iReportGraphHeight, &dc, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	pWnd->OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bitmap.GetSafeHandle());
	CloseClipboard();

	memDC.SelectObject(pOldBitmap);
	bitmap.Detach();
}

void CClipBoard::CopyWndImgToClipboard_AVMD(CWnd * pWnd, int iReportGraphWidth, int iReportGraphHeight, int iShiftXWidth)
{
	CBitmap 	bitmap;
	CClientDC	dc(pWnd);
	CDC 		memDC;
	CRect		rect;

	memDC.CreateCompatibleDC(&dc);

	pWnd->GetWindowRect(rect);

	bitmap.CreateCompatibleBitmap(&dc, iReportGraphWidth, iReportGraphHeight);

	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.StretchBlt(0, 0, iReportGraphWidth, iReportGraphHeight, &dc, -iShiftXWidth, 0, rect.Width() + iShiftXWidth, rect.Height(), SRCCOPY);

	pWnd->OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bitmap.GetSafeHandle());
	CloseClipboard();

	memDC.SelectObject(pOldBitmap);
	bitmap.Detach();
}