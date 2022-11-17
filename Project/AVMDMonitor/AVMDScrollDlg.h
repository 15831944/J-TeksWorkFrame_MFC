#pragma once
#include "afxwin.h"


// CAVMDScrollDlg 대화 상자입니다.

class CAVMDScrollDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAVMDScrollDlg)

public:
	CAVMDScrollDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CAVMDScrollDlg(CString strType, int nInputType);
	virtual ~CAVMDScrollDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCROLL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CScrollBar m_ctrlScrollValue;
	int m_iScrValue;
private:
	void SetSrollInfo();
	SCROLLINFO m_scrInfo;
	int m_iStandardValue;
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CString m_strGraphType;
	int m_nInputType;
};
