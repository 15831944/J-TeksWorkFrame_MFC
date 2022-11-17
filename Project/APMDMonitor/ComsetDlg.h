#pragma once
#include "afxwin.h"
#include "afxcoll.h"
#include "listports.h"


// CComSetDlg 대화 상자입니다.
//typedef tSerialEnum static BOOL CALLBACK SerialEnum(LPVOID lpCallbackValue, LISTPORTS_PORTINFO* lpPortInfo)
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
	afx_msg void OnCbnSelchangeCbportno();
	static BOOL CALLBACK SerialEnum(LPVOID lpCallbackValue, LISTPORTS_PORTINFO* lpPortInfo);
	//! Port가 번호가 아닌형태일경우 사용할 수 있음.
	CString m_strPortNo;
	int m_iBaudRate;
	CComboBox m_cPortNo;
	CComboBox m_cBaudRate;
	virtual BOOL OnInitDialog();
	CString GetComPortEnum();
	static CPtrList *m_pLstComsName;
	static CPtrList *m_pLstComsNo;
	afx_msg void OnBnClickedOk();
	int FindIndex(TCHAR *szBaudRate, CComboBox * pComboBox);
};
