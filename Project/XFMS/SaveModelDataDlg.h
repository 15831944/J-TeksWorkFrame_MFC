#pragma once
#include "SCButton.h"
//#include <XaGrabber.h>


// CSaveModelData 대화 상자입니다.

class CSaveModelDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveModelDataDlg)

public:
	CSaveModelDataDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSaveModelDataDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SAVE_MODELDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnBrowser();
	CString m_strModelName;
	virtual BOOL OnInitDialog();
	CMilBuffer *SavePCBAllBuff;

	CStatus m_pStatus;

	void CopyfileSetuptoModelFloder(CString OriginPath,CString NewPath);
	CSCButton m_cOK;
	CSCButton m_cCancel;
};
