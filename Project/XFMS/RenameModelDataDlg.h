#pragma once

#include "resource.h"
#include "SCButton.h"
// CRenameModelDataDlg 대화 상자입니다.

class CRenameModelDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameModelDataDlg)

public:
	CRenameModelDataDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRenameModelDataDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RENAME_MODELDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRename();
	CString m_strDirName;
	CSCButton m_cModify;
	CSCButton m_cCancel;
};
