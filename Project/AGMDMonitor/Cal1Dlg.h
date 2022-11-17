#pragma once


// CCal1Dlg 대화 상자입니다.

class CCal1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCal1Dlg)

public:
	CCal1Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCal1Dlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAL1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL IsGetRcvCalData();
	void DispValue();
	CString m_strCalInfo;
	CString m_strJigGap;
	BOOL m_bAbleToNext;
	int GetJigGap();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
