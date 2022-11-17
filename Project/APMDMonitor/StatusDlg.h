#pragma once
#include "LedONOFF.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "AdvancedEdit.h"
#include "Status.h"
class CStatusCompareDlg;


// CStatusDlg 대화 상자입니다.

class CStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatusDlg)

public:
	CStatusDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStatusDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CLedONOFF m_LaserIndicator;
	virtual BOOL OnInitDialog();
	CLedONOFF m_FanIndicator;
	CProgressCtrl m_BatteryState;
	CProgressCtrl m_HumidityState;
	UINT m_uiP01Counter;
	UINT m_uiP03Counter;
	UINT m_uiP05Counter;
	CString m_strAPMDTemp;
	CString m_strPressure;
	void SetRespository(CRepository * pRepository);
	int CheckWarnDisp(bool bUseWarning, bool bIsError, CString strMsg);
	CRepository *m_pRepository;
	CListCtrl m_lstStatChart;
	void UpdateStatisticChart(int iAcc01, int iAcc03, int iAcc05);
	CString m_strWarnDisp;
	CAdvancedEdit m_cWarnDisplay;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int NormalDisplay(CString strMsg);
	CString m_strHumidity;
	CStatic m_cP01Counter;
	CFont m_BigFont;
	CStatic m_cP03Counter;
	CStatic m_cP05Counter;
	CStatic m_cLabelP01;
	CStatic m_cLabelP03;
	CStatic m_cLabelP05;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int UpdateCompareChart(tAPMDDispData *pDispData);
	void UpdateMainChart(tAPMDDispData * pAcqData);
	CStatusCompareDlg *m_pStatusCompareDlg;
	int SetDispData(tAPMDDispData & dispMain, tAPMDDispData & dispCompare);
};
