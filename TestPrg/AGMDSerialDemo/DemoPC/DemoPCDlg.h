
// DemoPCDlg.h : 헤더 파일
//

#pragma once

#include "..\SerialLib\SerialMFC.h"		// 필수
#include "..\SerialLib\AVMDPacket.h"	// 필수
#include "afxwin.h"

// CDemoPCDlg 대화 상자
class CDemoPCDlg : public CDialogEx
{
// 생성입니다.
public:
	CDemoPCDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMOPC_DIALOG };
#endif

	protected:

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
	CSerialMFC	m_serial;				// 필수
	CAGMDPacket	m_AVMDPacket;			// 필수

	CString m_editXValue;
	CString m_editYValue;
	CString m_editZValue;
	CComboBox m_wndComPorts;
	CString m_strTemperature;
	CString m_strBattery;
	CString m_strNum;
	CString m_strCalDate;
	CString m_strSerialNo;
	CString m_strState;
	CString m_strTime;
	void UpdateAvailableComPort();
	afx_msg void OnCbnSelendokComports();
	afx_msg LRESULT OnSerialMsg(WPARAM wParam, LPARAM lParam);	// 필수
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedStart();
};
