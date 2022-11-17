#pragma once
#include "afxcmn.h"


// CMapDlg 대화 상자입니다.

class CMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMapDlg)

public:
	CMapDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabMapSel;
	virtual BOOL OnInitDialog();
};
