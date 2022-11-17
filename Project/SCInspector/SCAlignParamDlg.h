#pragma once
#include "afxwin.h"
#include "resource.h"
#include "SCAlgoBase.h"
#include "SCAlgoDialog.h"
#include "SCButton.h"

// CSCAlignParamDlg 대화 상자입니다.

class CSCAlignParamDlg : public CSCAlgoDialog
{
	DECLARE_DYNAMIC(CSCAlignParamDlg)

public:
	CSCAlignParamDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSCAlignParamDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SCALIGNPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	static CSCAlignParamDlg * Instance();
	static CSCAlignParamDlg *m_pInstance;
	CComboBox m_cSelScanDir;
	virtual BOOL OnInitDialog();
	void LoadParam(CSCAlgoBase * pAlgo);
	afx_msg void OnBnClickedOk();
	CSCButton m_cApply;
	CSCButton m_cCancel;
};
