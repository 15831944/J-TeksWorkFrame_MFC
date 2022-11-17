#pragma once
#include "_Font.h"

// 컴퓨터에서 Microsoft Visual C++를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

// 참고: 이 파일의 내용을 수정하지 마십시오. Microsoft Visual C++에서
//  이 클래스를 다시 생성할 때 수정한 내용을 덮어씁니다.

/////////////////////////////////////////////////////////////////////////////
// CHisto_org 래퍼 클래스입니다.

class CHistoGraph : public CWnd
{
protected:
	DECLARE_DYNCREATE(CHistoGraph)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x65F48D5E, 0xF093, 0x4C2D, { 0xA2, 0x9E, 0xDF, 0xBD, 0x18, 0x5B, 0x10, 0x1D } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 특성입니다.
public:


// 작업입니다.
public:

// _DLKOscilloscope

// Functions
//

	void BackupColorSetting()
	{
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RestoreColorSetting()
	{
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ResetColorSetting()
	{
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetColorBackByRGB(unsigned long crBack)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, crBack);
	}
	void SetColorFocusByRGB(unsigned long crFocus)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, crFocus);
	}
	void SetColorLineByRGB(unsigned long crLine)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, crLine);
	}
	void SetColorTextByRGB(unsigned long crText)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, crText);
	}
	void GetOtherColorsByRGB(unsigned long * pcrBack, unsigned long * pcrFocus, unsigned long * pcrLine, unsigned long * pcrText)
	{
		static BYTE parms[] = VTS_PUI4 VTS_PUI4 VTS_PUI4 VTS_PUI4 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pcrBack, pcrFocus, pcrLine, pcrText);
	}
	void GetGraphColorsByRGB(unsigned long * pcrGraph1, unsigned long * pcrGraph2, unsigned long * pcrGraph3, unsigned long * pcrGraph4, unsigned long * pcrGraph5)
	{
		static BYTE parms[] = VTS_PUI4 VTS_PUI4 VTS_PUI4 VTS_PUI4 VTS_PUI4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pcrGraph1, pcrGraph2, pcrGraph3, pcrGraph4, pcrGraph5);
	}
	void SetColorGraphByRGB(unsigned long crGraph1, unsigned long crGraph2, unsigned long crGraph3, unsigned long crGraph4, unsigned long crGraph5)
	{
		static BYTE parms[] = VTS_UI4 VTS_UI4 VTS_UI4 VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, crGraph1, crGraph2, crGraph3, crGraph4, crGraph5);
	}
	void SetActiveData(short nIndex)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nIndex);
	}
	BOOL SetData(short nIndex, double * pdXData, double * pdYData, short nNum)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_PR8 VTS_PR8 VTS_I2 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nIndex, pdXData, pdYData, nNum);
		return result;
	}
	BOOL GetData(short nIndex, double * pdXData, double * pdYData, short nNum)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_PR8 VTS_PR8 VTS_I2 ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nIndex, pdXData, pdYData, nNum);
		return result;
	}
	void EnableGraph(short nIndex)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nIndex);
	}
	void DisableGraph(short nIndex)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nIndex);
	}
	void ClearData(short nIndex)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nIndex);
	}
	BOOL GetDataLong(short nIndex, double * pdXData, double * pdYData, long nNum)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_PR8 VTS_PR8 VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nIndex, pdXData, pdYData, nNum);
		return result;
	}
	BOOL SetDataLong(short nIndex, double * pdXData, double * pdYData, long nNum)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_PR8 VTS_PR8 VTS_I4 ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nIndex, pdXData, pdYData, nNum);
		return result;
	}
	void SetVertGuideLineNum(short nNum)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nNum);
	}
	void SetVertGuideLineValue(short nIndex, double dValue)
	{
		static BYTE parms[] = VTS_I2 VTS_R8 ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nIndex, dValue);
	}
	void SetActiveVertGuideLine(short nIndex, short nActive)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nIndex, nActive);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

double GetXMinRange()
{
	double result;
	GetProperty(0xa, VT_R8, (void*)&result);
	return result;
}
void SetXMinRange(double propVal)
{
	SetProperty(0xa, VT_R8, propVal);
}
double GetXMaxRange()
{
	double result;
	GetProperty(0xb, VT_R8, (void*)&result);
	return result;
}
void SetXMaxRange(double propVal)
{
	SetProperty(0xb, VT_R8, propVal);
}
double GetYMinRange()
{
	double result;
	GetProperty(0xc, VT_R8, (void*)&result);
	return result;
}
void SetYMinRange(double propVal)
{
	SetProperty(0xc, VT_R8, propVal);
}
double GetYMaxRange()
{
	double result;
	GetProperty(0xd, VT_R8, (void*)&result);
	return result;
}
void SetYMaxRange(double propVal)
{
	SetProperty(0xd, VT_R8, propVal);
}
short GetHorzLineNum()
{
	short result;
	GetProperty(0xe, VT_I2, (void*)&result);
	return result;
}
void SetHorzLineNum(short propVal)
{
	SetProperty(0xe, VT_I2, propVal);
}
short GetVertLineNum()
{
	short result;
	GetProperty(0xf, VT_I2, (void*)&result);
	return result;
}
void SetVertLineNum(short propVal)
{
	SetProperty(0xf, VT_I2, propVal);
}
short GetMarginX()
{
	short result;
	GetProperty(0x10, VT_I2, (void*)&result);
	return result;
}
void SetMarginX(short propVal)
{
	SetProperty(0x10, VT_I2, propVal);
}
short GetMarginY()
{
	short result;
	GetProperty(0x11, VT_I2, (void*)&result);
	return result;
}
void SetMarginY(short propVal)
{
	SetProperty(0x11, VT_I2, propVal);
}
CString GetColorBack()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}
void SetColorBack(CString propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}
CString GetColorLine()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}
void SetColorLine(CString propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}
CString GetColorText()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}
void SetColorText(CString propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}
CString GetColorFocus()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}
void SetColorFocus(CString propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}
short GetBevelWidth()
{
	short result;
	GetProperty(0x12, VT_I2, (void*)&result);
	return result;
}
void SetBevelWidth(short propVal)
{
	SetProperty(0x12, VT_I2, propVal);
}
BOOL GetColorBackAsParent()
{
	BOOL result;
	GetProperty(0x13, VT_BOOL, (void*)&result);
	return result;
}
void SetColorBackAsParent(BOOL propVal)
{
	SetProperty(0x13, VT_BOOL, propVal);
}
COleFont GetFont()
{
	LPDISPATCH result;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&result);
	return COleFont(result);
}
void SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}
BOOL GetBorder()
{
	BOOL result;
	GetProperty(0x14, VT_BOOL, (void*)&result);
	return result;
}
void SetBorder(BOOL propVal)
{
	SetProperty(0x14, VT_BOOL, propVal);
}
BOOL GetShowFocusValue()
{
	BOOL result;
	GetProperty(0x15, VT_BOOL, (void*)&result);
	return result;
}
void SetShowFocusValue(BOOL propVal)
{
	SetProperty(0x15, VT_BOOL, propVal);
}
BOOL GetShowGuideText()
{
	BOOL result;
	GetProperty(0x16, VT_BOOL, (void*)&result);
	return result;
}
void SetShowGuideText(BOOL propVal)
{
	SetProperty(0x16, VT_BOOL, propVal);
}
CString GetColorGraph1()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}
void SetColorGraph1(CString propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}
CString GetColorGraph2()
{
	CString result;
	GetProperty(0x6, VT_BSTR, (void*)&result);
	return result;
}
void SetColorGraph2(CString propVal)
{
	SetProperty(0x6, VT_BSTR, propVal);
}
CString GetColorGraph3()
{
	CString result;
	GetProperty(0x7, VT_BSTR, (void*)&result);
	return result;
}
void SetColorGraph3(CString propVal)
{
	SetProperty(0x7, VT_BSTR, propVal);
}
CString GetColorGraph4()
{
	CString result;
	GetProperty(0x8, VT_BSTR, (void*)&result);
	return result;
}
void SetColorGraph4(CString propVal)
{
	SetProperty(0x8, VT_BSTR, propVal);
}
CString GetColorGraph5()
{
	CString result;
	GetProperty(0x9, VT_BSTR, (void*)&result);
	return result;
}
void SetColorGraph5(CString propVal)
{
	SetProperty(0x9, VT_BSTR, propVal);
}
BOOL GetAutoRange()
{
	BOOL result;
	GetProperty(0x17, VT_BOOL, (void*)&result);
	return result;
}
void SetAutoRange(BOOL propVal)
{
	SetProperty(0x17, VT_BOOL, propVal);
}
short GetVTextOffsetX()
{
	short result;
	GetProperty(0x18, VT_I2, (void*)&result);
	return result;
}
void SetVTextOffsetX(short propVal)
{
	SetProperty(0x18, VT_I2, propVal);
}
short GetVTextOffsetY()
{
	short result;
	GetProperty(0x19, VT_I2, (void*)&result);
	return result;
}
void SetVTextOffsetY(short propVal)
{
	SetProperty(0x19, VT_I2, propVal);
}


};
