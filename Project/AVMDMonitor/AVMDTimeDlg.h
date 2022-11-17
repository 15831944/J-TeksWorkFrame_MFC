#pragma once
#include "DrawingHelper.h"

// CAVMDTimeDlg 대화 상자입니다.

class CAVMDTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAVMDTimeDlg)

public:
	CAVMDTimeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CAVMDTimeDlg(CString strDegree, double unit, BOOL bIsFFTDlg, int nInputType);
	virtual ~CAVMDTimeDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIME};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CString strDegree;
	double dblUnit;
	BOOL bIsFFT;
public:
	CDrawingHelper m_drawHelper;
	
	CString strFFTdegree;
	afx_msg void OnPaint();
private:
	int m_nInputType;
	CDC m_MemDC;
	CDC* m_pDC;
	CBitmap* pOldBitmap;
	CRect m_ClienRect;
	CRect m_OldClientRect;
	BOOL m_bToRepaint;
public:
	void MakeTimeUnit();
	void DrawView();
};
