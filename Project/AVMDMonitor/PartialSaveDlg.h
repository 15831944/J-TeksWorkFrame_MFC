#pragma once

class CRepository;
// CPartialSaveDlg 대화 상자입니다.

class CPartialSaveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPartialSaveDlg)

public:
	CPartialSaveDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPartialSaveDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARTIALSAVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnNext();
	int m_nStep;
	int OnStepNext();
	static int m_iStartPartialNo;
	static int m_iEndPartialNo;
	CRepository *m_pRepository;
	CString m_strLabelPartialSave;
	virtual BOOL OnInitDialog();
	void Init();
	static UINT PartialSaveThread(LPVOID lParam);
};
