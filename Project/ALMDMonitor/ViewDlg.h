#pragma once
#include "PlotGage.h"


// CViewDlg 대화 상자입니다.

class CViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewDlg)

public:
	CViewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CPlotGage m_Gage;
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnUpdateStatusInfo(WPARAM wParam, LPARAM lParam);
};
