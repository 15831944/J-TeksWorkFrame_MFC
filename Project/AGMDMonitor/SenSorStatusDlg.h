#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "LedONOFF.h"


// CSenSorStatusDlg 대화 상자입니다.

class CSenSorStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSenSorStatusDlg)

public:
	CSenSorStatusDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSenSorStatusDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENSORSTATUSDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CLedONOFF m_cCntStat;
	CStatic m_cTempStat;
	CProgressCtrl m_cBatt;
	CStatic m_cBattNUM;
	int SetSensorState(int iTemp, int nBatt);
	LRESULT OnReceiveUpdateUI(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
