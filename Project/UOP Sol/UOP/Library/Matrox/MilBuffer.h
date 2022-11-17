#pragma once
#include "stdafx.h"
#include <mil.h>
#include "MilClass.h"
#include "MilGraph.h"
#include <UniUtility.h>

/*!
\brief 
* 이미지속성을 받아올때사용(CMilBuffer::GetImageProperties)
@author 전병근
*/
struct tImageProperties
{
	CSize ImageSize;
	int Depth;
	int BandSize;
	int Sign;
	int Type;
	MIL_INT64 Attr;

};
class CMilDisplay;
/*!
\brief 
* Mil의 MilBuffer관련  함수군을 Class화하였다
@author 전병근
*/
class MIL_DLLDECL CMilBuffer:public CMilGraph
{
public:
//	enum tMilAttr{BUF_ARRAY=M_ARRAY, BUF_IMAGE=M_IMAGE, BUF_KERNEL=M_KERNEL, BUF_LUT=M_LUT, BUF_DISP=M_DISP, BUF_PROC=M_PROC, BUF_DIB=M_DIB};
//	enum tMilDepth{DEP_1=1, DEP_8=8, DEP_16=16, DEP_32=32, DEP_UNSIGNED=M_UNSIGNED, DEP_SIGNED=M_SIGNED, DEP_FLOAT=M_FLOAT};
	static int m_milIDBuffCounter;
	CMilBuffer(void);
	CMilBuffer(CSize ImageSize);
	~CMilBuffer(void);
    
	int GetMilCounter(void);
	int Restore(CString strFileName);
	void BufLoad(CString strFileName);
	int BufImport(CString strFileName, MIL_INT FileFormat=M_BMP, MIL_INT Operation=M_LOAD);
	//! 디스플레이와 버퍼를 연결 시킨다.
	int Close(void);
	BOOL IsAlloced(void);
	void ConvertToBMPFmt(CMilBuffer *pBMPBuffer, MIL_INT64 BufferAttribute=M_IMAGE+M_PROC);
	void ExportBMP(CString strFileName);
	void ExportJPG(CString strFileName);
	void Export(CString strFileName, int m_iFileFormat = M_TIFF);
	void SetFileFormat(MIL_INT iFileFormat);
	CMilBuffer* operator=(CMilBuffer &  LValue);
	MIL_INT64 SetAttribute(MIL_INT64 BufferAttribute);
	MIL_INT64 GetAttribute(void);
	void Select(CMilDisplay * pMilDisplay);
	void UnSelectDisp(void);
	void SetMilBuffer(CMilBuffer *pMilBuffer);
	void SelectWindow(HWND ClientWindowHandle);
	void SelectWindow(CMilDisplay * pMilDisplay,  HWND ClientWindowHandle);
	void SelectWindow(CWnd *pHWindow);
	void SelectWindow(CMilDisplay * pMilDisplay, CWnd *pHWindow);
	BOOL IsDispAllocated(void);
	void GetImageProperties(tImageProperties *pImageProperty);
    void ResizeBuf(CMilBuffer *pMilDest, CdblSize imageSize);

	MIL_INT m_iDepth;
	MIL_INT64 m_BufAttr;
	MIL_INT m_iFileFormat;

	int Alloc2D(CSize imageSize);
	void Put2D(CSize offsetPos, CSize sizeImage, void *pUserArrayPtr);
	CMilBuffer * Child2D(CRect *pRect);
	CMilBuffer * Child2D(int offsetX, int offsetY, int childWidth, int childHeight);
	int Create2D(CSize sizeImage, void *pAddr, int nDepth=8L+M_UNSIGNED, int Attr=M_IMAGE+M_PROC);
	void * GetAddr(void);
	void SetDepth(int Depth);
	int GetDepth(void);
	CWnd *m_pHWndSelected;
	CMilBuffer* DuplicateBuff(void);
	void Clear(MIL_DOUBLE ClearColor=M_COLOR_BLACK);
	int Copy(CMilBuffer* pMilSrc);
	int CopyClip(CMilBuffer* pMilClipBuffer, CPoint Offset);
	int Get2D(void * pUserArray, CRect rect);
	int SetImageProperties(tImageProperties* pImageProperties);
	int SetDisplay(CMilDisplay* pMilDisplay);
	void CopyColor2D(CMilBuffer *pBufDest, CRect rectCopy, CPoint ofsDest=CPoint(0,0), int bandDest=M_ALL_BANDS , int bandSrc=M_ALL_BANDS);
	CMilDisplay* GetDisplay();
	int WndLvlTranform(CMilBuffer *pBufDest, int OrgStart, int OrgEnd, int DestStart, int DestEnd);
private:
	CMilDisplay *m_pMilDisplay;
public:
	int GetIDBuffer(void);
	void SetIDBuffer(int milID);
	int GetPixelGray(int x, int y, int * vGray);
	int GetPixelGray(CPoint posPixel, int * vGray);
	int GetPixelColor(int x, int y, COLORREF * vColor);
	int GetPixelColor(CPoint posPixel, COLORREF * vColor);
	int LoadRawFile(CString strFileName, CSize rawSize);
};
