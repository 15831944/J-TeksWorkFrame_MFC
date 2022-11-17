#pragma once


// CXMLTestDlg 대화 상자입니다.

class CXMLTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CXMLTestDlg)

public:
	CXMLTestDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CXMLTestDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_XML };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnXmlRead();
	afx_msg void OnBnClickedBtnXmlWrite();
};
