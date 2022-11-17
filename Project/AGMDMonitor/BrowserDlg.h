#pragma once
#include "afxwin.h"


// CBrowserDlg 대화 상자입니다.

class CBrowserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBrowserDlg)

public:
	CBrowserDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBrowserDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BROWSERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBrowser;
	CString m_strDisplayNum;
	CString m_strTotalDisplayNum;
	afx_msg void OnBnClickedBtnPrev();
	afx_msg void OnBnClickedBtnNext();
private:
	void BrowserUpdate();
public:
	CComboBox m_ctrl_CBBrowser;
	LRESULT afx_msg OnInitBrowser(WPARAM wParam, LPARAM lParam);
	LRESULT afx_msg OnDlgInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeCbBrowser();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
