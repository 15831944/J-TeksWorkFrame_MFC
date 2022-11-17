
// DemoDeviceDlg.h : 헤더 파일
//

#pragma once

#include "..\SerialLib\SerialMFC.h"
#include "..\SerialLib\AGMDPacket.h"
#include "afxwin.h"
#include "afxcmn.h"

// CDemoDeviceDlg 대화 상자
class CDemoDeviceDlg : public CDialogEx
{
// 생성입니다.
public:
	CDemoDeviceDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMODEVICE_DIALOG };
#endif

protected:
	CSerialMFC	m_serial;

	CAGMDPacket m_AGMDPacket;

	BYTE		m_byBattery;
	short		m_sTemperature;
	BYTE		m_byNum;
	int			m_iGap1Value;
	int			m_iGap2Value;
	int			m_iGap3Value;

	CStatic		m_wndState;
	CString		m_strCalDate;
	CString		m_strSerialNo;

	BOOL		m_bStart;
	BOOL		m_bTimerOn;

	CComboBox	m_wndComPorts;

	CButton		m_wndStart;

protected:
	void UpdateAvailableComPort();

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelendokComports();
	afx_msg LRESULT OnSerialMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedStart();
	CSpinButtonCtrl m_spXValue;
	CSpinButtonCtrl m_spYValue;
	CSpinButtonCtrl m_spZValue;
	CComboBox m_cKindOfWafer;
	CSliderCtrl m_cSldXValue;
	CSliderCtrl m_cSldYValue;
	CSliderCtrl m_cSldZValue;
	afx_msg void OnNMReleasedcaptureSlidXvalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlidYvalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlidZvalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTRBNThumbPosChangingSlidXvalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTRBNThumbPosChangingSlidYvalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTRBNThumbPosChangingSlidZvalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton();
	int m_iValueMode;
	afx_msg void OnBnClickedRauto();
	afx_msg void OnBnClickedRmanual();
	int m_nPacketNum;
	void IncPacketNum();
	LRESULT OnMMTimer(WPARAM wParam, LPARAM lParam);
	UINT m_mmTimerID;
	afx_msg void OnBnClickedRsqure();
	void SetCalDefault();
	CString m_strUserCalDate;
	int m_iCalGap1;
	int m_iCalGap2;
	int m_iCalGap3;
	UINT m_uiJigGap;
	void SetCalUI(char * szUserCalDate, int iJigGap, int CalGap1, int CalGap2, int CalGap3);
	void SetUISetCalInfo(PC_PACKET_SETCAL * pPacket);
};
