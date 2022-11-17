#pragma once


// COPPanelDlg 대화 상자입니다.

class COPPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COPPanelDlg)

public:
	COPPanelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COPPanelDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPPANELDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strGap1Offset;
	CString m_strGap2Offset;
	CString m_strGap3Offset;
	afx_msg void OnBnClickedRadioNone();
	afx_msg void OnBnClickedRadioOffset();
	void ControlDisable(BOOL bIsDisable);
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnPaint();
	CString m_strOpGapUnit1;
	CString m_strOpGapUnit2;
	CString m_strOpGapUnit3;
	LRESULT afx_msg OnDlgUnitUpdate(WPARAM wParam, LPARAM lParam);
	LRESULT afx_msg OnOffsetChangeUpdate(WPARAM wParam, LPARAM lParam);
	LRESULT afx_msg OnInitOffset(WPARAM wParam, LPARAM lParam); 
	LRESULT afx_msg OnDlgInit(WPARAM wParam, LPARAM lParam);
private:
	double CalcValueToOriginalValue(CString strOffsetValue, int iGapUnit);
	double OriginalValueToCalcValue(double dblGapValue, int iGapUnit);
	void UnitUpdate();
	int m_iLastUnitMode;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
