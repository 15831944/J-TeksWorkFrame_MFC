#pragma once
#include "afxcmn.h"


// CCamViewDlg 대화 상자입니다.

class CCamViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCamViewDlg)

public:
	CCamViewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCamViewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CAMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabCamSel;
	virtual BOOL OnInitDialog();
};
