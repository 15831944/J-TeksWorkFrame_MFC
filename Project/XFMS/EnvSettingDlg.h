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
	UINT m_ulCountOfCam;
	afx_msg void OnBnClickedBtnComset();
	afx_msg void OnBnClickedBtnapply();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnSetIMGfolder();
	CString m_strIMGSaveFolder;
	CString m_strAVISaveFolder;
	CSpinButtonCtrl m_spCountSensors;
	UINT m_ulCountOfSensors;
	CSpinButtonCtrl m_spDelOldFile;
	UINT m_nDaysDelImg;
	CSCButton m_cCOMSet;
	CSCButton m_cApply;
	CSCButton m_cSave;
	CSCButton m_cCancel;
	CSCButton m_cCalSensor;
	CIPAddressCtrl m_cIPTempSensorAddr;
	afx_msg void OnBnClickedBtnCalsensor();
	UINT m_ulGraphXRange;
	UINT m_ulGraphYMax;
	UINT m_ulGraphYMin;
	int m_iDispMode;
	afx_msg void OnBnClickedBtnSetavifolder();
	CString m_strSensorNamePx[6];
};
