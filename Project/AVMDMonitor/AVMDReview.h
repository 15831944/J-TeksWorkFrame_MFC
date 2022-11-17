#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <UniUtility.h>


// CAVMDReview 대화 상자입니다.
struct tMarkInfo;
class CAVMDReview : public CDialogEx
{
	DECLARE_DYNAMIC(CAVMDReview)

public:
	CAVMDReview(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAVMDReview();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_Slider_Review; 
	LRESULT UpdateUI(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateReviewComboBox(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateReviewComboBoxClear(WPARAM wParam, LPARAM lParam);
	LRESULT TimerStartNFinish(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnNMReleasedcaptureSliderReview(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	void SetMarkList(CListBox *pList);
	CComboBox m_cCBMark;
	void GetRepoMarkInfo(int nInputType, int nPos=0);
	int GetMarkInfoCB(int iIndex, tMarkInfo *pMarkInfo);
	CSliderCtrl m_Slider_CompareReview;
//	afx_msg void OnNMReleasedcaptureSliderCompare(NMHDR *pNMHDR, LRESULT *pResult);
	int m_iCompareType;
	afx_msg void OnBnClickedRadioCompareX();
	afx_msg void OnBnClickedRadioCompareY();
	afx_msg void OnBnClickedRadioCompareZ();
	afx_msg void OnBnClickedRadioCompareRms();
private:
	void CompareControlHide(bool bIsHide);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CComboBox m_cCB_CompareMark;
	afx_msg void OnCbnSelchangeCbMark();
private:
	BOOL m_bIsMoveSlider;
	BOOL m_bIsTextChange;
public:
	afx_msg void OnCbnSelchangeCbCompareMark();
	afx_msg void OnBnClickedBtnMarkPrev();
	afx_msg void OnBnClickedBtnMarkNext();
	afx_msg void OnBnClickedBtnMarkDelete();
	afx_msg void OnBnClickedBtnMarkUpdate();
private:
	int m_iMarkCurrentIndex;
public:
	afx_msg void OnBnClickedBtnCompareMarkPrev();
private:
	int m_iMarkCompareCurrentIndex;
public:
	afx_msg void OnBnClickedBtnCompareMarkNext();
	afx_msg void OnBnClickedBtnCompareMarkDelete();
	afx_msg void OnBnClickedBtnCompareMarkUpdate();
	afx_msg void OnBnClickedBtnCompare();
	void NewPositionDrawGraph();
	afx_msg void OnBnClickedBtnReviewGo();
	afx_msg void OnBnClickedBtnCompareGo();
	CButtonST btnPlay;
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_cCBSpeed;
	void Player(int nMoveValue);
	CButtonST btnBackPlay;
	CButtonST btnFastBackPlay;
	CButtonST btnFastPlay;
	afx_msg void OnBnClickedBtnBackPlay();
	BOOL m_bIsClick;
	BOOL m_bIsPlayClick;
	BOOL m_bIsBackPlayClick;
	void PlayerControlDisable(bool bIsDisable);
	void TimerActivation();
	int m_nMoveStep;
	afx_msg void OnCbnSelchangeCbSpeed();
	void PlayerStop();
	afx_msg void OnBnClickedBtnFastBackplay();
	afx_msg void OnBnClickedBtnFastPlay();
	void MarkInsert(tMarkInfo *pNewMarkInfo,int nInputType);
	BOOL MarkSearch(int nIndex, int nInputType);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnUpdateEditData();
	afx_msg void OnEnUpdateEditCompareData();
	void StartPlayTimer(int iForward);
};
