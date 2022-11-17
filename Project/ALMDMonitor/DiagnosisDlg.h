#pragma once
#include "afxwin.h"
#include "atltypes.h"

class CDiagnosis1Dlg;
class CDiagnosis2Dlg;
class CDiagnosis3Dlg;
class CDiagnosis4Dlg;


// CDiagnosisDlg 대화 상자입니다.

class CDiagnosisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiagnosisDlg)

public:
	CDiagnosisDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDiagnosisDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAGNOSIS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CDiagnosis1Dlg *m_pDiagnosis1Dlg;
	CDiagnosis2Dlg *m_pDiagnosis2Dlg;
	CDiagnosis3Dlg *m_pDiagnosis3Dlg;
	CDiagnosis4Dlg *m_pDiagnosis4Dlg;
	int MakeChildWnd();
	int WndRepositioning();
	afx_msg void OnClose();
	void SetStep(int nStep);
	afx_msg void OnBnClickedDiagnext();
	int m_nStep;
	CButton m_cDiagPrev;
	CButton m_cDiagNext;
	afx_msg void OnBnClickedDiagprev();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CPoint m_ptDiagData[4];
	void CalcResult(double * pDblRPResult, double * pDblRRResult, double * pDblPPResult);
	CButton m_cBtnCancel;
};
