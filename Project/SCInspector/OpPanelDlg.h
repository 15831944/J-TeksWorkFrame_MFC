#pragma once
#include "afxwin.h"
#include "..\Library\SerialLib\SerialMFC.h"	// 필수
#include "..\Library\SerialLib\SCInspPacket.h"	// 필수
#include "SCButton.h"
#include "SCGroupBox.h"

// COpPanelDlg 대화 상자입니다.

class COpPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpPanelDlg)

public:
	COpPanelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COpPanelDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPPANEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSerialMFC		m_serial;				// 필수
	CSCInspPacket	m_SCInspPacket;			// 필수
	CComboBox m_ctrlSelCam;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeSelcam();
	afx_msg void OnBnClickedBtnLoadimage();
	afx_msg void OnBnClickedSetenv();
	afx_msg void OnBnClickedSaveimg();
	afx_msg void OnBnClickedBtnInspimage();
	int InitLight();
	BOOL m_bReceived;
	int SerialCheckNInit();
	void ProcessCMD(int nEvent);
	void SetFlagReceived();
	BOOL IsCMDReceived();
	int RcvSerialMsg(WPARAM wParam);
	int CheckoutPacket();
	void LightOn();
	afx_msg void OnBnClickedBtnTeaching();
	afx_msg void OnBnClickedBtnLoadjobfile();
	afx_msg void OnBnClickedBtnSavejobfile();
	afx_msg void OnBnClickedBtnGrab();
	afx_msg void OnBnClickedChkactiveallcam();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRunstop();
	afx_msg void OnBnClickedBtnSaveas();
	afx_msg void OnBnClickedBtntrial();
	afx_msg void OnBnClickedBtnOptest();
	void DeleteOldFile();
	BOOL IsDate(CString strName, CTime *pTime);
	int DiffDate(CTime * pDate);
	void DelFolder(CString strFolder);
	CSCButton m_cRunStop;
	CSCButton m_cGrabImage;
	CSCButton m_cSaveImage;
	CSCButton m_cSaveInspImg;
	CSCButton m_cLoadImage;
	CSCButton m_cLoadJobFile;
	CSCButton m_cSaveJobFile;
	CSCButton m_cSaveAs;
	CSCButton m_cTeaching;
	CSCButton m_cTrial;
	CSCButton m_cSetEnv;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	static UINT ThreadTest(LPVOID pParam);
};
