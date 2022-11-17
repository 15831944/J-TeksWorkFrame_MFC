#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "NumEdit.h"
#include "SCButton.h"


// CEnvSettingDlg 대화 상자입니다.

class CEnvSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnvSettingDlg)

public:
	CEnvSettingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEnvSettingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENVSETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strModelRepository;
	afx_msg void OnBnClickedBtnSetfolder();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strDataFileName;
	afx_msg void OnBnClickedBtnIoset();
	CSpinButtonCtrl m_spCountOfCam;
	UINT m_ulCountOfCam;
	CComboBox m_cbCam1;
	CComboBox m_cbCam2;
	CComboBox m_cbCam3;
	CComboBox m_cbCam4;
	afx_msg void OnBnClickedBtnIomap();
	afx_msg void OnBnClickedBtnLight();
	afx_msg void OnBnClickedBtnComset();
	CNumEdit m_cCam1FovX;
	CNumEdit m_cCam1FovY;
	CNumEdit m_Cam2FovX;
	CNumEdit m_cCam2FovY;
	CNumEdit m_cCam3FovX;
	CNumEdit m_cCam3FovY;
	CNumEdit m_cCam4FovX;
	CNumEdit m_cCam4FovY;
	CdblSize m_dblCamFov[4];
	CNumEdit m_cCam1Exp;
	CNumEdit m_cCam2Exp;
	CNumEdit m_cCam3Exp;
	CNumEdit m_cCam4Exp;
	UINT m_uiCamExpTime[4];
	afx_msg void OnBnClickedBtnapply();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnSetngfolder();
	CString m_strNGSaveFolder;
	CSpinButtonCtrl m_spCountOfStage;
	UINT m_ulCountOfStage;
	CSpinButtonCtrl m_spCountPOS;
	UINT m_ulCountOfPos;
	CSpinButtonCtrl m_spDelOldFile;
	UINT m_nDaysDelImg;
	CSCButton m_cIOTest;
	CSCButton m_cLight;
	CSCButton m_cCOMSet;
	CSCButton m_cApply;
	CSCButton m_cSave;
	CSCButton m_cCancel;
	CSpinButtonCtrl m_spCountOfRemoteCam;
	UINT m_ulCountOfCamRemote;
	afx_msg void OnBnClickedBtnSetnremotefolder();
	CString m_strRemoteCamFolder;
	CIPAddressCtrl m_cIPRemoteAddr;
};
