#pragma once


// CDispView 대화 상자입니다.

class CDispView : public CDialogEx
{
	DECLARE_DYNAMIC(CDispView)

public:
	CDispView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDispView();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
