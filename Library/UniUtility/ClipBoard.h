#pragma once
#include "UniUtilBase.h"
class DLLDECL CClipBoard :	public CUniUtilBase
{
public:
	CClipBoard();
	~CClipBoard();
	static void CopyWndImgToClipboard(CWnd * pWnd);
	static void CopyWndImgToResizeClipboard(CWnd * pWnd, int iReportGraphWidth, int iReportGraphHeight);
	static void CopyWndImgToClipboard_AVMD(CWnd * pWnd,int iReportGraphWidth,int iReportGraphHeight,int iShiftXWidth);
};

