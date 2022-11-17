#pragma once
#include "DrawingHelper.h"
#include "atltypes.h"

// CAVMDFFTGraphDlg 대화 상자입니다.

class CAVMDFFTGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAVMDFFTGraphDlg)

public:
	CAVMDFFTGraphDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CAVMDFFTGraphDlg(CString strDegree, int nInputType);
	virtual ~CAVMDFFTGraphDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FFT_GRAPH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	
	CDrawingHelper m_drawHelper;
	afx_msg void OnPaint();
	CString m_strGraphType;
	void MakeGraph();
	void DrawView();

public:
	BOOL m_bToRepaint;
	CDC m_MemDC;
	CDC* m_pDC;
	CBitmap* pOldBitmap;
	CRect m_ClientRect;
	CRect m_OldClientRect;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CPoint m_fftMeasurePos;
private:
	int m_nInputType;
};
