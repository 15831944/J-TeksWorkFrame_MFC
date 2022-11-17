#pragma once
#include "..\Library\SerialLib\SerialMFC.h"	// 필수
#include "..\Library\SerialLib\AGMDPacket.h"	// 필수
#include "Status.h"
#include "afxcmn.h"
#include "LedONOFF.h"
#include "afxwin.h"

// CMonitorDlg 대화 상자입니다.
class CRepository;
class CMonitorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorDlg)

public:
	CMonitorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonitorDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONITORDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSerialMFC	m_serial;				// 필수
	CAGMDPacket	m_AGMDPacket;			// 필수
	afx_msg void OnBnClickedRadioView();
	afx_msg void OnBnClickedRadioReview();
	CFont m_BigFont;
	virtual BOOL OnInitDialog();
	BOOL m_iProgramMode;
	LRESULT OnProgramModeChange(WPARAM wParam, LPARAM lParam);
	void SetProgramMode(CStatus::tProgramMode ProgramMode);
	void SerialClose();
	void SerialCheckNInit();
	void ProcessCMD(int nEvent);
	int RcvSerialMsg(WPARAM wParam);
	void SetStateInfo(DVC_PACKET_GAPINFO * pPacketInfo);
	int MakeWriteInfo(tAGMDRecord  * pNewRecord, DVC_PACKET_GAPINFO *pPacketInfo, int nInputType);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT OnReceiveUpdateUI(WPARAM wParam, LPARAM lParam);
	LRESULT OnDlgInit(WPARAM wParam, LPARAM lParam);
	CProgressCtrl m_cBatt;
	CStatic m_cTempStat;
	CStatic m_cBattNUM;
	BOOL m_bReceived;
	void SetFlagReceived();
	BOOL IsCMDReceived();
	//CRepository *m_pRepository;
	CString m_strGap1;
	CString m_strGap2;
	CString m_strGap3;
	void WriteToleranceValue();
	CString m_strGap1Unit;
	CString m_strGap2Unit;
	CString m_strGap3Unit;
	CString m_strTest;
	CString m_strGap1Tol;
	CString m_strGap2Tol;
	CString m_strGap3Tol;
private:
	void OnInitText();
public:
	afx_msg void OnBnClickedBtnRead();
	CComboBox m_ctrlIntNo;
	CComboBox m_ctrlIntervalUnit;
private:
	void EnableControl(bool bIsDisable);
public:
	BOOL m_bUseAutoCmd;
	afx_msg void OnBnClickedChkAutoCmd();
	void SetCalInfo(DVC_PACKET_CALINFO * pPacketInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_ctrl_Gap1;
	CStatic m_ctrl_Gap2;
	CStatic m_ctrl_Gap3;
	afx_msg void OnClose();
};
