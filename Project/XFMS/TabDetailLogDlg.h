#pragma once
#include "afxwin.h"
#include "resource.h"
#include <UniUtility.h>

/** \brief Log 창을 생성하여 각종 동작 정보및 상태를 표시 해줄 수 있도록 하였습니다.
Log 창은 CEdit 를 기반으로 작성하였으며 기본 검은색 바탕의 흰색 글씨로 설정 하였습니다.

일정 텍스트 이상 넘어가면 자동으로 클리어되도록 하였습니다.

X-ray 상태, Grib, 상태 등등 여러 면에서 활용 할 수 있도록 

Instance 함수를 만들어 각 다른 클레스에서 메세지를 전달 할 수 있도록 하였습니다.
(싱글톤 방식)


\date 2009.12.28
\author Ji Hoon Ahn
*/
class  CTabDetailLogDlg : public CPropertyPage
	

{
	DECLARE_DYNAMIC(CTabDetailLogDlg)

public:

	CTabDetailLogDlg();
	virtual ~CTabDetailLogDlg();

// Dialog Data
	enum { IDD = IDD_TAB_DETAIL_LOG };
     


	static CTabDetailLogDlg* Instance(void);   ///<Instance 선언 

	static CTabDetailLogDlg* m_pInstance;      ///<자기 참조 포인터 
    
	


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);





     CBrush *brush1;
	void SetTextLog(CString Text,COLORREF RGBcolor,BOOL ERR);  ///< Log 창에 Text 를 써넣을 수 있도록 하는 함수 
	virtual BOOL OnInitDialog();    
	afx_msg void OnBnClickedTextLogClear();   ///< LogText Clear 버튼 
	
	CEdit m_EditLog;
	CButtonST m_ClearBtn;

//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_StringErr;
	CEdit m_LogdataEditControl;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
