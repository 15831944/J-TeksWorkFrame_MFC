#pragma once


// CAnnotateMarkDlg 대화 상자입니다.

class CAnnotateMarkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnnotateMarkDlg)

public:
	CAnnotateMarkDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnnotateMarkDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANNOTATE_MARK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
