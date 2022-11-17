#pragma once


// CSetGapCriteria 대화 상자입니다.

class CSetGapCriteria : public CDialogEx
{
	DECLARE_DYNAMIC(CSetGapCriteria)

public:
	CSetGapCriteria(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetGapCriteria();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETGAPCRITERIADLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_iGapUnit;
	virtual BOOL OnInitDialog();
	BOOL m_bAllGapSameSettings;
private:
	void DisableControl(BOOL bIsGapSameSetting);
	void ChangeDlgUnit();
	void GetControlValue();
	void SetControlValue();

public:
	afx_msg void OnBnClickedRadioInches();
	afx_msg void OnBnClickedRadioMm();
	afx_msg void OnBnClickedCheckAllGapSame();
	afx_msg void OnBnClickedOk();
	
	CString m_strAllTarget;
	CString m_strGap1Target;
	CString m_strGap2Target;
	CString m_strGap3Target;
	CString m_strALLTolerance;
	CString m_strGap1Tolerance;
	CString m_strGap2Tolerance;
	CString m_strGap3Tolerance;
private:
	double OriginalValueToCalcValue(double dblGapValue, int iGapUnit);
	double CalcValueToOriginalValue(CString strGapValue, int iGapUnit);
public:
	struct tCheckRange
	{
		CString strMessage;
		BOOL bRangeOver;
	};
private:
	void CalcControlValue();
	//int CalcSaveConfigValue(CString strGapValue, int iGapUnit);
public:
	CString m_strViewRange;
private:
	tCheckRange CheckRange();
public:
	CString checkEachRange();


};
