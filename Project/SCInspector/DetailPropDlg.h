#pragma once
#include "TabDetailLogDlg.h"
#include "TabDetailResultDlg.h"


// CDetailPropDlg

/*\brief 결과창에 Log나 Result붙이기 위한 Container다이얼로그.@n 
\date 2010.01.05
\author 전 병근
*/
class CDetailPropDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CDetailPropDlg)

public:
	CDetailPropDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDetailPropDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDetailPropDlg();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CTabDetailLogDlg *m_pTabDetailLogDlg;
	CTabDetailResultDlg m_TabDetailResultDlg;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBitmap m_background;
};


