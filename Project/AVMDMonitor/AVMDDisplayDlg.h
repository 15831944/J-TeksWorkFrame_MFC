#pragma once
#include "..\Library\SerialLib\SerialMFC.h"	// 필수
#include "..\Library\SerialLib\AVMDPacket.h"	// 필수
#include "Repository.h"
#include "afxwin.h"
#include "Status.h"
#include "afxcmn.h"

// CAVMDDisplayDlg 대화 상자입니다.
class CAVMDMonitorView;

class CAVMDDisplayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAVMDDisplayDlg)

public:
	CAVMDDisplayDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAVMDDisplayDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISPLAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSerialMFC	m_serial;				// 필수
	CAVMDPacket	m_AVMDPacket;			// 필수
	BOOL m_bMinimize_RMS;
	BOOL m_bMinimize_X;
	BOOL m_bMinimize_Y;
	BOOL m_bMinimize_Z;
	afx_msg void OnBnClickedChkTrackRMS();
	afx_msg void OnBnClickedChkTrackX();
	afx_msg void OnBnClickedChkTrackY();
	afx_msg void OnBnClickedChkTrackZ();
	afx_msg void OnBnClickedChkFftShow();
	void SerialCheckNInit();
	BOOL m_bFFTShow;
	int RcvSerialMsg(WPARAM wParam);
	void SetStateInfo(DVC_PACKET_MEAINFO * pPacketInfo);
	int MakeWriteInfo(DVC_PACKET_MEAINFO *pPacketInfo, int nInputType);
	afx_msg LRESULT UpdateFilterTextBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ComboUpdateUI(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT TimeComboInialize(WPARAM wParam, LPARAM lParam);
	CString m_strTemperatureStatus;
	afx_msg void OnBnClickedButton1();
	CString m_strBatteryStatus;
	CComboBox m_combo_GraphScale;
	CString m_strGraphScale;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCbTimeScale();
	CComboBox m_combo_GraphTimeUnit;
	CString m_strGraphTimeUnit;
	afx_msg void OnCbnSelchangeCbHorizonScale();
	afx_msg void OnBnClickedRadioView();
	afx_msg void OnBnClickedRadioReview();
	afx_msg LRESULT OnProgramModeChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT UpdateTimeDomainTextBox(WPARAM wParam, LPARAM lParam);
	void SerialClose();
	void SetProgramMode(CStatus::tProgramMode ProgramMode);
	afx_msg void OnClose();
	int m_iProgramMode;
	int m_iEventMode;
	void ProcessCMD(int nEvent);
	CComboBox m_combo_FFTSampleSize;
	CStatic m_ctrlText_FilterStatus;
	CString m_strFilterStatus;
	CStatic m_CtrlEdit_TimeDomain;
	CString m_strTimeDomain;
	afx_msg void OnCbnSelchangeCbSamplesize();
	CComboBox m_combo_SpectrumScale;
	afx_msg void OnCbnSelchangeCbFsScale();
private:
	CString m_strTimeUnit;
public:
	void MakeSimulationData(DVC_PACKET_MEAINFO * pPacketInfo);
	//afx_msg void OnBnClickedBtnHold();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_combo_FFTGraphUnit;
	afx_msg void OnCbnSelchangeCbFftHorizonScale();
	int AppendMarkInfo(int nInputType);
	CButton m_btnHold;
	void SetFlagReceived();
	BOOL m_bReceived;
	BOOL IsCMDReceived();
	BOOL m_bReconnected;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CAVMDMonitorView *m_pView;
	int MessagePumping();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnTestTestfilewrite();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_Slider_FFT_Show;
	int CheckoutPacket();
	afx_msg void OnNMReleasedcaptureSliderFftShow(NMHDR *pNMHDR, LRESULT *pResult);


};
