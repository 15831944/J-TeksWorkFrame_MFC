#pragma once

// CDiagnosis1Dlg 대화 상자입니다.

class CDiagnosis1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiagnosis1Dlg)

public:
	CDiagnosis1Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDiagnosis1Dlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
