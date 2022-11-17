#pragma once


// CStationInfomationDlg 대화 상자입니다.

class CStationInfomationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStationInfomationDlg)

public:
	CStationInfomationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStationInfomationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATIONINFOMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnClear();
	CString m_strOperator;
	CString m_strTool;
	CString m_strStation;
	CString m_strComment;
	afx_msg void OnBnClickedBtnApply();
	virtual BOOL OnInitDialog();
	void Initialize();
	LRESULT WorkUpdate(WPARAM wParam, LPARAM lParam);
};
