#pragma once

#include "SCAlgoDialog.h"
#include "afxwin.h"
#include "SCButton.h"
// CSCHEdgeDetParamDlg 대화 상자입니다.

class CSCHEdgeDetParamDlg : public CSCAlgoDialog
{
	DECLARE_DYNAMIC(CSCHEdgeDetParamDlg)

public:
	CSCHEdgeDetParamDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSCHEdgeDetParamDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCHEDGEDETPARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	static CSCHEdgeDetParamDlg * Instance();
	static CSCHEdgeDetParamDlg *m_pInstance;
	virtual BOOL OnInitDialog();
	virtual void LoadParam(CSCAlgoBase * pAlgo);
	CComboBox m_cHEDetDir;
	afx_msg void OnBnClickedOk();
	CSCButton m_cApply;
	CSCButton m_cCancel;
};
