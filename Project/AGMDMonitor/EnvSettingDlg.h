#pragma once


// CEnvSettingDlg 대화 상자입니다.

class CEnvSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnvSettingDlg)

public:
	CEnvSettingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEnvSettingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENVSETTINGDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strSaveFolder;
	CString m_strDataFileName;
	afx_msg void OnBnClickedBtnSetfolder();
	afx_msg void OnBnClickedOk();
};
