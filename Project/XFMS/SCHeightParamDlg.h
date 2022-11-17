#pragma once

#include "SCAlgoDialog.h"
#include "afxwin.h"
#include "SCButton.h"
// CSCHeightParamDlg 대화 상자입니다.

class CSCHeightParamDlg : public CSCAlgoDialog
{
	DECLARE_DYNAMIC(CSCHeightParamDlg)

public:
	CSCHeightParamDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSCHeightParamDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCHEIGHTPARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	static CSCHeightParamDlg * Instance();
	static CSCHeightParamDlg *m_pInstance;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual void LoadParam(CSCAlgoBase * pAlgo);
	CComboBox m_cScanDir;
	CSCButton m_cApply;
};
