#pragma once
#include "afxcmn.h"

#include "Status.h"
// CStatusCompareDlg 대화 상자입니다.

class CStatusCompareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatusCompareDlg)

public:
	CStatusCompareDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStatusCompareDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATUS_COMPARE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_lstStatChart;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int SetDispData(tAPMDDispData & dispMain, tAPMDDispData & dispCompare);
	void SetDispCore(tAPMDDispData * pAPMDDispData, int KindOfData);
};
