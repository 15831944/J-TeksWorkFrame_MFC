#pragma once


// CTempDlg 대화 상자입니다.

class CTempDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTempDlg)

public:
	CTempDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTempDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEMPDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
