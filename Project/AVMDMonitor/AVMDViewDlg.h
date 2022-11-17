#pragma once
#include "afxcmn.h"
#include "LedONOFF.h"
#include "afxwin.h"
#include "afxcoll.h"
#include <UniUtility.h>

// CAVMDViewDlg 대화 상자입니다.

class CAVMDViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAVMDViewDlg)

public:
	CAVMDViewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAVMDViewDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_iTemp;
	CProgressCtrl m_cBatt;
	CLedONOFF m_cCntStat;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int SetSensorState(int iTemp, int nBatt);
	CStatic m_cTempStat;
	CStatic m_cBattNUM;
	afx_msg void OnBnClickedBtnRecordStart();
	afx_msg void OnBnClickedBtnRecordFinish();
	CButton m_btnStartPause;
	BOOL m_bPauseSave;
	CComboBox m_RecUnit;
	afx_msg void OnCbnSelchangeCbRecordunit();
	CComboBox m_RecTime;
	void SetIntTimeCnt(int nNum);
	int GetIntTime();
	void RecStartNPause();
	void RecTerminate();
	int GetFileSaveStatus();
	void SetMarkList(CListBox *pList);
	LRESULT OnRecordStart(WPARAM wParam, LPARAM lParam);
	LRESULT OnRecordFinish(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateHoldButton(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnAddmark();
	CStringArray m_lstMark;
	CString m_strNextMarkName;
	CString m_strUserMarkName;
	CString m_strAddMark;
	int GetMarkPos();
	int m_nMarkPos;
	int m_nAutoMarkPos;
	int GetAutoMarkPos();
	void IncAutoMarkPos();
	void IncMarkPos();
	void SetMarkPos(int nPos);
	void SetAutoMarkPos(int nPos);
	CString GetMark();
	CTextProgressCtrl m_cRecPrgrs;
	afx_msg void OnBnClickedBtnHold();
	CButton m_btnHold;
private:
	void RecdNewStart();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
