#pragma once
#include "afxwin.h"
#include "afxcoll.h"
#include "Status.h"

class CRepository;
// CViewOptionDlg 대화 상자입니다.
class CGraphDlg;
class CViewOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewOptionDlg)

public:
	CViewOptionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewOptionDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW_OPTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_DispMode;
	virtual BOOL OnInitDialog();
	CComboBox m_cHScaleUnit;
	CComboBox m_cVScaleValue;
	CComboBox m_cHScaleValue;
	CComboBox m_cDispParticleType;
	void OnCbnSelchangeOptionChange();
	afx_msg void OnBnClickedAddmark();
	CString GetMark();
	CString m_strAddMark;
	CStringArray m_lstMark;
	CString m_strUserMarkName;
	CComboBox m_cCBMarkBrowser;
	CPtrList *m_plstMarkInfos;
	afx_msg void OnBnClickedBtnbmarkupdate();
	afx_msg void OnCbnSelchangeCbMark();
	afx_msg void OnBnClickedBtnbmarkprev();
	afx_msg void OnBnClickedBtnbmarknext();
	afx_msg void OnBnClickedBtnbmarkdel();
	void SetGraphDlg(CGraphDlg * pGraphDlg);
	CGraphDlg *m_pGraphDlg;
	void MarkMove(int iComboListPos);
	void SetRepository(CRepository * pRepository);
	CRepository *m_pRepository;
	void UpdateMarkView();
	int m_prevSelIndex;
	afx_msg void OnBnClickedBtnbmarkadd();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strNextMarkName;
};
