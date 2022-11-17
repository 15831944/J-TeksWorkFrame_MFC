#pragma once


// CStationInformationDlg 대화 상자입니다.

class CStationInformationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStationInformationDlg)

public:
	CStationInformationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStationInformationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATION_INFORMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnApply();
	CString m_strOperator;
	CString m_strTool;
	CString m_strStation;
	CString m_strComment;
	afx_msg void OnBnClickedBtnClear();
	void GetWorkEnvData();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
