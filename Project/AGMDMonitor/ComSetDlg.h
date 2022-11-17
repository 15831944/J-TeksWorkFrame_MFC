#pragma once
#include "afxwin.h"
#include "afxcoll.h"
#include "listports.h"

// CComSetDlg 대화 상자입니다.

class CComSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CComSetDlg)

public:
	CComSetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CComSetDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	static BOOL CALLBACK SerialEnum(LPVOID lpCallbackValue, LISTPORTS_PORTINFO* lpPortInfo);
	CComboBox m_cBaudRate;
	int m_iBaudRate;
	CComboBox m_cPortNo;
	CString m_strPortNo;
	static CPtrList *m_pLstComsName;
	static CPtrList *m_pLstComsNo;
	virtual BOOL OnInitDialog();
	CString GetComPortEnum();
	
	int FindIndex(TCHAR *szBaudRate, CComboBox * pComboBox);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk2();
};
