#pragma once
#include "afxwin.h"


// CDiagnosis4Dlg 대화 상자입니다.

class CDiagnosis4Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiagnosis4Dlg)

public:
	CDiagnosis4Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDiagnosis4Dlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void HideResult();
	CStatic m_cGResult;
	CStatic m_cRPResult;
	CStatic m_cRRResult;
	CStatic m_cPPResult;
	void ShowResult();
	void SetResult(double dblRPResult, double dblRRResult, double dblPPResult);
	CString m_strRPResult;
	CString m_strRRResult;
	CString m_strPPResult;
};
