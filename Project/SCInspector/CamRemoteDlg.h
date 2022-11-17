#pragma once
#include "SCSocket.h"

// CCamRemoteDlg 대화 상자입니다.

class CCamRemoteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCamRemoteDlg)

public:
	CCamRemoteDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCamRemoteDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMREMOTE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CMilBuffer *m_pMilCamView;
	CMilDisplay *m_pMilDisp;
	int m_iCamBuildNo;
	CString m_strCamName;
	CSCSocket *m_pSockContainer;
	static int m_iRemoteIndex;
	LRESULT OnReceiveCMD(WPARAM wparam, LPARAM lparam);
	void LoadFile(CString strRemoteFileName);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitRemoteCam();
	LRESULT OnSockInitListen(WPARAM wparam, LPARAM lparam);
	void InitSockListen();
	void CreateSockListen();
};
