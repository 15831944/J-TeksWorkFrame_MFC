#pragma once
#include "afxcmn.h"

class CInspResult;

// CTabDetailResultDlg dialog
/**
\brief CDetailPropDlg에 붙어서 검사결과와 같은 정보를 나타내기 위한 Log창.@n 
\date 2010.01.05
\author 전 병근
*/
class CTabDetailResultDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CTabDetailResultDlg)

public:
	CTabDetailResultDlg();
	virtual ~CTabDetailResultDlg();

// Dialog Data
	enum { IDD = IDD_TAB_DETAIL_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetResultDisplay(CInspResult *pInspResult);
    
	virtual BOOL OnInitDialog();
	CListCtrl m_listResult;

	static CTabDetailResultDlg* Instance(void);   ///<Instance 선언 

	static CTabDetailResultDlg* m_pInstance;      ///<자기 참조 포인터 
    
};
