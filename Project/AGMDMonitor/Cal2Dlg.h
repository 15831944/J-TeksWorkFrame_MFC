#pragma once
#include "afxcmn.h"
#include "Status.h"


// CCal2Dlg 대화 상자입니다.

class CCal2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCal2Dlg)

public:
	CCal2Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCal2Dlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAL2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cResult;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void MakeChart();
	void MakeChartFrame();
	tCalData m_NewCalData;
	void CalculateCalData();
	int m_iNewCal1;
	int m_iNewCal2;
	int m_iNewCal3;
	int m_iNewJigGap;
	void SendCalSetCmd();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
