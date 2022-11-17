#pragma once


// CStatusPanelDlg 대화 상자입니다.
class CDetailInfoDlg;

class CStatusPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatusPanelDlg)

public:
	CStatusPanelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStatusPanelDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STSPANEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CDetailInfoDlg *m_pDetailInfoDlg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
