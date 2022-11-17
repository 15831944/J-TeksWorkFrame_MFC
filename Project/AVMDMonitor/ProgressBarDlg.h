#pragma once
#include "afxcmn.h"
#include <UniUtility.h>

// CProgressBarDlg 대화 상자입니다.

class CProgressBarDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressBarDlg)

public:
	CProgressBarDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CProgressBarDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESSBARDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTextProgressCtrl m_ctrl_Progress;
	void SetTextMesaage(CString strMessage);
	void SetMaxRange(int nMaxCount);
	void ProgressBarStart();
	static BOOL m_bIsActive;
	void ProgressBarFinish();
	CWinThread *m_pThread;
	static UINT ProcessThread(LPVOID lParam);
	static int m_iValue;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	CString m_strMessage;
	virtual BOOL OnInitDialog();
	int m_nDataCount;
	int m_iTimeSecond;
};
