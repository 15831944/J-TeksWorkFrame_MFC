#pragma once
#include "DrawingHelper.h"

// CSenSorViewDlg 대화 상자입니다.

class CSenSorViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSenSorViewDlg)

public:
	CSenSorViewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSenSorViewDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENSORVIEWDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CDC m_MemDC;
	CDC* m_pDC;
	CBitmap* m_pOldBitmap;
	afx_msg void OnPaint();
	CDrawingHelper m_drawHelper;
	LRESULT afx_msg OnUpdateSensorView(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
