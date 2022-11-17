
// DemoPCDlg.h : 헤더 파일
//

#pragma once

#include "SerialMFC.h"		// 필수
#include "XFMSPacket.h"	//kiy
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
	CSerialMFC	m_serial;				// 필수
	CXFMSPacket m_XFMSPacket;			//kiy

	CString		m_strCount01;
	CString		m_strCount03;
	CString		m_strCount05;
	CString		m_strTemperature;
	CString		m_strPressure;
	CString		m_strBattery;
	CString		m_strHumidity;
	CString		m_strCalDate;
	CString		m_strSerialNo;
	CString		m_strState;
	CString		m_strTime;

	CEdit		m_wndPruge;
	CEdit		m_wndMeasure;

	CEdit m_wndKV;
	CEdit m_wndMA;

	CComboBox	m_wndComPorts;

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
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnCbnSelendokComports();
	afx_msg LRESULT OnSerialMsg(WPARAM wParam, LPARAM lParam);	// 필수
	afx_msg void OnBnClickedXrayOn();
	afx_msg void OnBnClickedXrayOff();
	afx_msg void OnBnClickedFocalStandard();
	afx_msg void OnBnClickedFocalSmall();
	afx_msg void OnBnClickedSetKv();
	afx_msg void OnBnClickedSetMa();
};
