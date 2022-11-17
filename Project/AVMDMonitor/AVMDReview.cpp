// AVMDReview.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "AVMDReview.h"
#include "afxdialogex.h"
#include "AVMDDefine.h"
#include "Status.h"
#include "Repository.h"
#include "ProgressBarDlg.h"

// CAVMDReview 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAVMDReview, CDialogEx)

CAVMDReview::CAVMDReview(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REVIEW, pParent)
	, m_iCompareType(0)
	, m_bIsMoveSlider(FALSE)
	, m_bIsTextChange(FALSE)
	, m_iMarkCurrentIndex(0)
	, m_iMarkCompareCurrentIndex(0)
	, m_bIsClick(FALSE)
	, m_bIsPlayClick(FALSE)
	, m_bIsBackPlayClick(FALSE)
	, m_nMoveStep(10)
{
	CStatus::Instance()->SetAVMDReView(this);
}

CAVMDReview::~CAVMDReview()
{
}

void CAVMDReview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_REVIEW, m_Slider_Review);
	DDX_Control(pDX, IDC_CB_MARK, m_cCBMark);
	DDX_Control(pDX, IDC_SLIDER_COMPARE, m_Slider_CompareReview);
	DDX_Radio(pDX, IDC_RADIO_COMPARE_X, m_iCompareType);
	DDX_Control(pDX, IDC_CB_COMPARE_MARK, m_cCB_CompareMark);
	DDX_Control(pDX, IDC_BTN_PLAY, btnPlay);
	DDX_Control(pDX, IDC_CB_SPEED, m_cCBSpeed);
	DDX_Control(pDX, IDC_BTN_BACK_PLAY, btnBackPlay);
	DDX_Control(pDX, IDC_BTN_FAST_BACKPLAY, btnFastBackPlay);
	DDX_Control(pDX, IDC_BTN_FAST_PLAY, btnFastPlay);
}


BEGIN_MESSAGE_MAP(CAVMDReview, CDialogEx)
	ON_MESSAGE(WM_REVIEW_UPDATE, &CAVMDReview::UpdateUI)
	ON_MESSAGE(WM_REVIEW_COMBO_UPDATE, &CAVMDReview::UpdateReviewComboBox)
	ON_MESSAGE(WM_RESET, &CAVMDReview::UpdateReviewComboBoxClear)
	ON_MESSAGE(WM_TIMERSTART, &CAVMDReview::TimerStartNFinish)
//	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_REVIEW, &CAVMDReview::OnNMReleasedcaptureSliderReview)
//	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_COMPARE, &CAVMDReview::OnNMReleasedcaptureSliderCompare)
	ON_BN_CLICKED(IDC_RADIO_COMPARE_X, &CAVMDReview::OnBnClickedRadioCompareX)
	ON_BN_CLICKED(IDC_RADIO_COMPARE_Y, &CAVMDReview::OnBnClickedRadioCompareY)
	ON_BN_CLICKED(IDC_RADIO_COMPARE_Z, &CAVMDReview::OnBnClickedRadioCompareZ)
	ON_BN_CLICKED(IDC_RADIO_COMPARE_RMS, &CAVMDReview::OnBnClickedRadioCompareRms)
	ON_CBN_SELCHANGE(IDC_CB_MARK, &CAVMDReview::OnCbnSelchangeCbMark)
	ON_CBN_SELCHANGE(IDC_CB_COMPARE_MARK, &CAVMDReview::OnCbnSelchangeCbCompareMark)
	ON_BN_CLICKED(IDC_BTN_MARK_PREV, &CAVMDReview::OnBnClickedBtnMarkPrev)
	ON_BN_CLICKED(IDC_BTN_MARK_NEXT, &CAVMDReview::OnBnClickedBtnMarkNext)
	ON_BN_CLICKED(IDC_BTN_MARK_DELETE, &CAVMDReview::OnBnClickedBtnMarkDelete)
	ON_BN_CLICKED(IDC_BTN_MARK_UPDATE, &CAVMDReview::OnBnClickedBtnMarkUpdate)
	ON_BN_CLICKED(IDC_BTN_COMPARE_MARK_PREV, &CAVMDReview::OnBnClickedBtnCompareMarkPrev)
	ON_BN_CLICKED(IDC_BTN_COMPARE_MARK_NEXT, &CAVMDReview::OnBnClickedBtnCompareMarkNext)
	ON_BN_CLICKED(IDC_BTN_COMPARE_MARK_DELETE, &CAVMDReview::OnBnClickedBtnCompareMarkDelete)
	ON_BN_CLICKED(IDC_BTN_COMPARE_MARK_UPDATE, &CAVMDReview::OnBnClickedBtnCompareMarkUpdate)
	ON_BN_CLICKED(IDC_BTN_COMPARE, &CAVMDReview::OnBnClickedBtnCompare)
	ON_BN_CLICKED(IDC_BTN_REVIEW_GO, &CAVMDReview::OnBnClickedBtnReviewGo)
	ON_BN_CLICKED(IDC_BTN_COMPARE_GO, &CAVMDReview::OnBnClickedBtnCompareGo)
	ON_BN_CLICKED(IDC_BTN_PLAY, &CAVMDReview::OnBnClickedBtnPlay)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_BACK_PLAY, &CAVMDReview::OnBnClickedBtnBackPlay)
	ON_CBN_SELCHANGE(IDC_CB_SPEED, &CAVMDReview::OnCbnSelchangeCbSpeed)
	ON_BN_CLICKED(IDC_BTN_FAST_BACKPLAY, &CAVMDReview::OnBnClickedBtnFastBackplay)
	ON_BN_CLICKED(IDC_BTN_FAST_PLAY, &CAVMDReview::OnBnClickedBtnFastPlay)
	ON_WM_HSCROLL()
	ON_EN_UPDATE(IDC_EDIT_DATA, &CAVMDReview::OnEnUpdateEditData)
	ON_EN_UPDATE(IDC_EDIT_COMPARE_DATA, &CAVMDReview::OnEnUpdateEditCompareData)
END_MESSAGE_MAP()

// CAVMDReview 메시지 처리기입니다.
LRESULT CAVMDReview::UpdateUI(WPARAM wParam, LPARAM lParam)
{
	int nTotalCount = CStatus::Instance()->GetRepository()->GetDataCount(0);
	int nDrawNum = CStatus::Instance()->m_nDrawPointNum;
	int nIndex = CStatus::Instance()->GetReviewDrawStart(0);	// Org Graph
	CString strText;
	
	strText.Format(_T("of %d ms"), nTotalCount);
	GetDlgItem(IDC_STATIC_TOTAL_COUNT)->SetWindowTextW(strText);
	
	if (!m_bIsTextChange)
	{
		strText.Format(_T("%d"), nIndex);
		GetDlgItem(IDC_EDIT_DATA)->SetWindowTextW(strText);
	}
	//m_Slider_Review.SetRange(0, (nTotalCount * 2) - (nDrawNum*2));
	m_Slider_Review.SetRange(0, nTotalCount);
	if (!m_bIsMoveSlider)
	{
		m_Slider_Review.SetPos(nIndex);
	}

	if (CStatus::Instance()->GetCompareCondition())
	{
		nTotalCount = CStatus::Instance()->GetRepository()->GetDataCount(1);
		nDrawNum = CStatus::Instance()->m_nDrawPointNum;
		nIndex = CStatus::Instance()->GetReviewDrawStart(1);	// Compare Graph

		CompareControlHide(FALSE);
		CString stre = CStatus::Instance()->m_strCompareDataFileName;
		GetDlgItem(IDC_EDIT_COMPARE_FILENAME)->SetWindowTextW(CStatus::Instance()->m_strCompareDataFileName);

		strText.Format(_T("of %d ms"), nTotalCount);
		GetDlgItem(IDC_STATIC_COMPARE_TOTAL_COUNT)->SetWindowTextW(strText);
		if (!m_bIsTextChange)
		{
			strText.Format(_T("%d"), nIndex);
			GetDlgItem(IDC_EDIT_COMPARE_DATA)->SetWindowTextW(strText);
		}

		//m_Slider_CompareReview.SetRange(0, (nTotalCount*2) - (nDrawNum*2));
		m_Slider_CompareReview.SetRange(0, nTotalCount);
		if (!m_bIsMoveSlider)
		{
			m_Slider_CompareReview.SetPos(nIndex);

		}
	}
	else
	{
		CompareControlHide(TRUE);
	}
	m_bIsMoveSlider = FALSE;
	m_bIsTextChange = FALSE;
	UpdateData(FALSE);
	return 0;
}

LRESULT CAVMDReview::UpdateReviewComboBox(WPARAM wParam, LPARAM lParam)
{
	int nInputType = (int)wParam;
	if(nInputType ==0)
		GetRepoMarkInfo(0);
	if(nInputType == 1)
		GetRepoMarkInfo(1);
	return 0;
}

LRESULT CAVMDReview::UpdateReviewComboBoxClear(WPARAM wParam, LPARAM lParam)
{
	m_iMarkCurrentIndex = 0;
	m_iMarkCompareCurrentIndex = 0;
	m_cCBMark.ResetContent();
	if (CStatus::Instance()->GetRepository()->m_MarkInfo[0].size() != 0)
		CStatus::Instance()->GetRepository()->m_MarkInfo[0].clear();
	m_cCB_CompareMark.ResetContent();
	if (CStatus::Instance()->GetRepository()->m_MarkInfo[1].size() != 0)
		CStatus::Instance()->GetRepository()->m_MarkInfo[1].clear();
	return 0;
}

LRESULT CAVMDReview::TimerStartNFinish(WPARAM wParam, LPARAM lParam)
{
	if (wParam == TRUE)
	{
		if (m_bIsPlayClick)
			StartPlayTimer(0);
		if(m_bIsBackPlayClick)
			StartPlayTimer(1);
	}
	if (wParam == FALSE)
	{
		KillTimer(0);
		KillTimer(1);
	}
	return 0;
}

BOOL CAVMDReview::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Slider_Review.SetRange(0, 0);
	m_Slider_Review.SetPos(0);
	GetDlgItem(IDC_EDIT_DATA)->SetWindowTextW(_T("0"));

	m_Slider_CompareReview.SetRange(0, 0);
	m_Slider_CompareReview.SetPos(0);
	GetDlgItem(IDC_EDIT_COMPARE_DATA)->SetWindowTextW(_T("0"));

	m_cCBSpeed.AddString(_T("0.1x"));
	m_cCBSpeed.AddString(_T("0.5x"));
	m_cCBSpeed.AddString(_T("1x"));
	m_cCBSpeed.AddString(_T("2x"));
	m_cCBSpeed.AddString(_T("3x"));
	m_cCBSpeed.AddString(_T("4x"));
	m_cCBSpeed.AddString(_T("5x"));
	m_cCBSpeed.AddString(_T("6x"));
	m_cCBSpeed.AddString(_T("7x"));
	m_cCBSpeed.AddString(_T("8x"));
	m_cCBSpeed.AddString(_T("9x"));
	m_cCBSpeed.AddString(_T("10x"));
	m_cCBSpeed.SetCurSel(0);

	btnFastPlay.SetFlat(FALSE);
	btnFastPlay.SetIcon(IDI_ICON_FAST_PLAY);

	btnBackPlay.SetFlat(FALSE);
	btnBackPlay.SetIcon(IDI_ICON_BACKPLAY);

	btnPlay.SetFlat(FALSE);
	btnPlay.SetIcon(IDI_ICON_PLAY);
	
	btnFastBackPlay.SetFlat(FALSE);
	btnFastBackPlay.SetIcon(IDI_ICON_FAST_BACKPLAY);

	CompareControlHide(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAVMDReview::GetRepoMarkInfo(int nInputType, int nPos)
{
	if (nInputType == 0)
	{
		m_cCBMark.ResetContent();
		int nCount = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].size();
		tMarkInfo *pMarkInfo;
		CString strContent;
		for (int i = 0; i < nCount; i++)
		{
			pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].at(i);
			m_cCBMark.AddString(pMarkInfo->strMark);
			m_cCBMark.SetItemData(i, pMarkInfo->iIndex);
		}
		m_cCBMark.SetCurSel(nPos);
		m_iMarkCurrentIndex = nPos;
	}
		
	if (nInputType == 1)
	{
		m_cCB_CompareMark.ResetContent();
		int nCount = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].size();
		tMarkInfo *pMarkInfo;
		CString strContent;
		for (int i = 0; i < nCount; i++)
		{
			pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].at(i);
			m_cCB_CompareMark.AddString(pMarkInfo->strMark);
			m_cCB_CompareMark.SetItemData(i, pMarkInfo->iIndex);
		}
		m_cCB_CompareMark.SetCurSel(nPos);
		m_iMarkCurrentIndex = nPos;
		//UpdateData(TRUE);
		//m_strCompareFileName = CStatus::Instance()->m_strCompareDataFileName;
		//UpdateData(FALSE);
	}
}

int CAVMDReview::GetMarkInfoCB(int iIndex, tMarkInfo *pMarkInfo)
{
	int nCount = m_cCBMark.GetCount();
	if (nCount < iIndex)return NULL;
	m_cCBMark.GetLBText(iIndex, pMarkInfo->strMark);
	pMarkInfo->iIndex = m_cCBMark.GetItemData(iIndex);
	return 0;
}

void CAVMDReview::OnBnClickedRadioCompareX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_iCompareType = 0;
	CStatus::Instance()->SetCompareType(_T("X"));
	CStatus::Instance()->UpdateScrn();
	UpdateData(FALSE);
}

void CAVMDReview::OnBnClickedRadioCompareY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_iCompareType = 1;
	CStatus::Instance()->SetCompareType(_T("Y"));
	CStatus::Instance()->UpdateScrn();
	UpdateData(FALSE);
}

void CAVMDReview::OnBnClickedRadioCompareZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_iCompareType = 2;
	CStatus::Instance()->SetCompareType(_T("Z"));
	CStatus::Instance()->UpdateScrn();
	UpdateData(FALSE);
}

void CAVMDReview::OnBnClickedRadioCompareRms()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_iCompareType = 3; 
	CStatus::Instance()->SetCompareType(_T("RMS"));
	CStatus::Instance()->UpdateScrn();
	UpdateData(FALSE);
}

void CAVMDReview::CompareControlHide(bool bIsHide)
{
	if (bIsHide)
	{
		GetDlgItem(IDC_STATIC_COMPARE_REVIEW)->ShowWindow(FALSE);
		GetDlgItem(IDC_BTN_COMPARE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPARE_FILENAME)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPARE_DATA)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_COMPARE_TOTAL_COUNT)->ShowWindow(FALSE);
		GetDlgItem(IDC_SLIDER_COMPARE)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TYPE)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_COMPARE_X)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_COMPARE_Y)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_COMPARE_Z)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_COMPARE_RMS)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_COMPARE_MARK)->ShowWindow(FALSE);
		GetDlgItem(IDC_CB_COMPARE_MARK)->ShowWindow(FALSE);
		GetDlgItem(IDC_BTN_COMPARE_MARK_PREV)->ShowWindow(FALSE);
		GetDlgItem(IDC_BTN_COMPARE_MARK_NEXT)->ShowWindow(FALSE);
		GetDlgItem(IDC_BTN_COMPARE_MARK_DELETE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BTN_COMPARE_MARK_UPDATE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BTN_COMPARE_GO)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_COMPARE_REVIEW)->ShowWindow(TRUE);
		GetDlgItem(IDC_BTN_COMPARE)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPARE_FILENAME)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPARE_DATA)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_COMPARE_TOTAL_COUNT)->ShowWindow(TRUE);
		GetDlgItem(IDC_SLIDER_COMPARE)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_TYPE)->ShowWindow(TRUE);
		GetDlgItem(IDC_RADIO_COMPARE_X)->ShowWindow(TRUE);
		GetDlgItem(IDC_RADIO_COMPARE_Y)->ShowWindow(TRUE);
		GetDlgItem(IDC_RADIO_COMPARE_Z)->ShowWindow(TRUE);
		GetDlgItem(IDC_RADIO_COMPARE_RMS)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_COMPARE_MARK)->ShowWindow(TRUE);
		GetDlgItem(IDC_CB_COMPARE_MARK)->ShowWindow(TRUE);
		GetDlgItem(IDC_BTN_COMPARE_MARK_PREV)->ShowWindow(TRUE);
		GetDlgItem(IDC_BTN_COMPARE_MARK_NEXT)->ShowWindow(TRUE);
		GetDlgItem(IDC_BTN_COMPARE_MARK_DELETE)->ShowWindow(TRUE);
		GetDlgItem(IDC_BTN_COMPARE_MARK_UPDATE)->ShowWindow(TRUE);
		GetDlgItem(IDC_BTN_COMPARE_GO)->ShowWindow(TRUE);
	}
}

BOOL CAVMDReview::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam ==VK_RETURN))
	{
		NewPositionDrawGraph();
		return TRUE;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CAVMDReview::OnCbnSelchangeCbMark()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	NewPositionDrawGraph();
	tMarkInfo *pMarkInfo;
	int iMark = m_cCBMark.GetCurSel();
	int iNewPos = 0;
	double dblNewDragNum = 0;
	//
	m_iMarkCurrentIndex = iMark;

	pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[0].at(iMark);
	double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

	
	iNewPos = pMarkInfo->iIndex;

	CString strTemp;
	strTemp.Format(_T("%d"), iNewPos);
	GetDlgItem(IDC_EDIT_DATA)->SetWindowTextW(strTemp);

	CStatus::Instance()->SetMarkClickEvent(TRUE, 0);

	CStatus::Instance()->SetMarkEditStart(iNewPos,0);
	dblNewDragNum = (int)((double)(iNewPos) / dblOnePixelDrawCount);
	CStatus::Instance()->SetDragAmount(dblNewDragNum, 0);		// 0 : Org Graph 
	
	m_bIsTextChange = TRUE;
	UpdateData(FALSE);
	CStatus::Instance()->UpdateScrn();
	//CStatus::Instance()->ReviewDlgUpdate();
}

void CAVMDReview::OnCbnSelchangeCbCompareMark()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	tMarkInfo *pMarkInfo;
	int iMark = m_cCB_CompareMark.GetCurSel();
	int iNewPos = 0;
	double dblNewDragNum = 0;
	m_iMarkCompareCurrentIndex = iMark;
	pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[1].at(iMark);
	double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();


	iNewPos = pMarkInfo->iIndex;

	CString strTemp;
	strTemp.Format(_T("%d"), iNewPos);
	GetDlgItem(IDC_EDIT_COMPARE_DATA)->SetWindowTextW(strTemp);
	CStatus::Instance()->SetMarkClickEvent(TRUE, 1);
	CStatus::Instance()->SetMarkEditStart(iNewPos,1);
	dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;

	CStatus::Instance()->SetDragAmount(dblNewDragNum, 1);
	UpdateData(FALSE);
	CStatus::Instance()->UpdateScrn();
	CStatus::Instance()->ReviewDlgUpdate();
}

void CAVMDReview::OnBnClickedBtnMarkPrev()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCount = CStatus::Instance()->GetRepository()->m_MarkInfo[0].size();

	if (nCount <= 0)
		return;

	CStatus::Instance()->GetReEnter();
	if (CStatus::Instance()->m_bAllReEnter)
		return;

	tMarkInfo *pMarkInfo;
	int iMark = m_cCBMark.GetCurSel();
	int iMarkCount = m_cCBMark.GetCount();
	int iNewPos = 0;
	double dblNewDragNum = 0;
	iMark -= 1;

	if (iMark <= 0)
		iMark = 0;
	if (iMark >= iMarkCount)
		iMark = iMarkCount;

	m_iMarkCurrentIndex = iMark;
	pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[0].at(iMark);
	double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

	//int iMarkInterval = (CStatus::Instance()->m_nDrawPointNum / CStatus::Instance()->m_iFfftVerticalNum) / 2;
	//iNewPos = pMarkInfo->iIndex - iMarkInterval;
	iNewPos = pMarkInfo->iIndex;
	//CStatus::Instance()->m_bMarkEditClick = TRUE;
	CStatus::Instance()->SetMarkClickEvent(TRUE, 0);
	CStatus::Instance()->SetMarkEditStart(iNewPos,0);
	dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;

	CStatus::Instance()->SetDragAmount(dblNewDragNum, 0);		// 0 : Org Graph 
	//m_Slider_Review.SetPos(iNewDragNum * 2);

	m_cCBMark.SetCurSel(iMark);

	UpdateData(FALSE);
	CStatus::Instance()->UpdateScrn();
}

void CAVMDReview::OnBnClickedBtnMarkNext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCount = CStatus::Instance()->GetRepository()->m_MarkInfo[0].size();

	if (nCount <= 0)
		return;
	CStatus::Instance()->GetReEnter();
	if (CStatus::Instance()->m_bAllReEnter)
		return;
	tMarkInfo *pMarkInfo;
	int iMark = m_cCBMark.GetCurSel();
	int iMarkCount = m_cCBMark.GetCount();
	int iNewPos = 0;
	double dblNewDragNum = 0;

	iMark += 1;

	if (iMark <= 0)
		iMark = 0;
	if (iMark >= iMarkCount)
		iMark = iMarkCount-1;

	m_iMarkCurrentIndex = iMark;

	pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[0].at(iMark);
	double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

	//int iMarkInterval = (CStatus::Instance()->m_nDrawPointNum / CStatus::Instance()->m_iFfftVerticalNum) / 2;
	//iNewPos = pMarkInfo->iIndex - iMarkInterval;
	iNewPos = pMarkInfo->iIndex;
	//CStatus::Instance()->m_bMarkEditClick = TRUE;
	CStatus::Instance()->SetMarkEditStart(iNewPos,0);
	CStatus::Instance()->SetMarkClickEvent(TRUE, 0);
	dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;

	CStatus::Instance()->SetDragAmount(dblNewDragNum, 0);		// 0 : Org Graph 
	//m_Slider_Review.SetPos(iNewDragNum*2);

	m_cCBMark.SetCurSel(iMark);
	
	UpdateData(FALSE);
	CStatus::Instance()->UpdateScrn();
}

void CAVMDReview::OnBnClickedBtnMarkDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCount = CStatus::Instance()->GetRepository()->m_MarkInfo[0].size();

	if (nCount <= 0)
		return;

	int iMark = m_cCBMark.GetCurSel();
	int iNewPos = 0;
	double dblNewDragNum = 0;

	int iStartNum = 0;
	if (CStatus::Instance()->GetMarkClickEvent(0) == TRUE)
		iStartNum = CStatus::Instance()->GetMarkEditStart(0);
	else
		iStartNum = CStatus::Instance()->GetReviewDrawStart(0);

	if (!MarkSearch(iStartNum,0))
		return;
	CStatus::Instance()->GetRepository()->m_MarkInfo[0].erase(CStatus::Instance()->GetRepository()->m_MarkInfo[0].begin() + iMark);

	CStatus::Instance()->SetMarkClickEvent(FALSE, 0);
	GetRepoMarkInfo(0);

	UpdateData(FALSE);
	CStatus::Instance()->UpdateScrn();
}

void CAVMDReview::OnBnClickedBtnMarkUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCount = CStatus::Instance()->GetRepository()->m_MarkInfo[0].size();

	if (nCount <= 0)
		return;

	int iMarkNumCount = CStatus::Instance()->GetRepository()->m_MarkInfo[0].size();
	int curIndex = m_cCBMark.GetCurSel();
	CString strChangeText;
	GetDlgItemTextW(m_cCBMark.GetDlgCtrlID(), strChangeText);
	tMarkInfo *pMarkInfo;
	
	int iStartNum = 0;
	if (CStatus::Instance()->GetMarkClickEvent(0) == TRUE)
		iStartNum = CStatus::Instance()->GetMarkEditStart(0);
	else
		iStartNum = CStatus::Instance()->GetReviewDrawStart(0);
	
	if(!MarkSearch(iStartNum, 0))
	{
		tMarkInfo *pNewMarkInfo = new tMarkInfo();
		pNewMarkInfo->iIndex = iStartNum;
		pNewMarkInfo->strMark = strChangeText;
		MarkInsert(pNewMarkInfo, 0);
		
	}
	else
	{
		pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[0].at(m_iMarkCurrentIndex);
		pMarkInfo->strMark = strChangeText;
	}
	int iMarkCount = CStatus::Instance()->GetRepository()->m_MarkInfo[0].size();
	int iSearchIndex = 0;
	for (int i = 0; i < iMarkCount; i++)
	{
		tMarkInfo *pMarkSearch = CStatus::Instance()->GetRepository()->m_MarkInfo[0].at(i);

		if (pMarkSearch->strMark == strChangeText)
		{
			iSearchIndex = i;
		}
	}
	GetRepoMarkInfo(0, iSearchIndex);
	CStatus::Instance()->GetRepository()->ReSaveTagFile(0);
	//CStatus::Instance()->SetReviewDragReset(0);
	CStatus::Instance()->SetMarkClickEvent(FALSE, 0);
	UpdateData(FALSE);
	CStatus::Instance()->UpdateScrn();
}

void CAVMDReview::OnBnClickedBtnCompareMarkPrev()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCount = CStatus::Instance()->GetRepository()->m_MarkInfo[0].size();

	if (nCount <= 0)
		return;
	CStatus::Instance()->GetReEnter();
	if (CStatus::Instance()->m_bAllReEnter)
		return;
	tMarkInfo *pMarkInfo;
	int iMark = m_cCB_CompareMark.GetCurSel();
	int iMarkCount = m_cCB_CompareMark.GetCount();
	int iNewPos = 0;
	double dblNewDragNum = 0;

	iMark -= 1;

	if (iMark <= 0)
		iMark = 0;
	if (iMark >= iMarkCount)
		iMark = iMarkCount;

	m_iMarkCompareCurrentIndex = iMark;
	pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[1].at(iMark);
	double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

	//int iMarkInterval = (CStatus::Instance()->m_nDrawPointNum / CStatus::Instance()->m_iFfftVerticalNum) / 2;
	//iNewPos = pMarkInfo->iIndex - iMarkInterval;
	iNewPos = pMarkInfo->iIndex;
//	CStatus::Instance()->m_bMarkEditClick = TRUE;
	CStatus::Instance()->SetMarkClickEvent(TRUE, 1);
	CStatus::Instance()->SetMarkEditStart(iNewPos, 1);
	dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;
	
	CStatus::Instance()->SetDragAmount(dblNewDragNum, 1);
	//m_Slider_CompareReview.SetPos(iNewDragNum * 2);

	m_cCB_CompareMark.SetCurSel(iMark);

	UpdateData(FALSE);
	CStatus::Instance()->UpdateScrn();
}

void CAVMDReview::OnBnClickedBtnCompareMarkNext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCount = CStatus::Instance()->GetRepository()->m_MarkInfo[0].size();

	if (nCount <= 0)
		return;
	CStatus::Instance()->GetReEnter();
	if (CStatus::Instance()->m_bAllReEnter)
		return;
	tMarkInfo *pMarkInfo;
	int iMark = m_cCB_CompareMark.GetCurSel();
	int iMarkCount = m_cCB_CompareMark.GetCount();
	int iNewPos = 0;
	double dblNewDragNum = 0;

	iMark += 1;

	if (iMark <= 0)
		iMark = 0;
	if (iMark >= iMarkCount)
		iMark = iMarkCount - 1;

	m_iMarkCompareCurrentIndex = iMark;

	pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[1].at(iMark);
	double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

	//int iMarkInterval = (CStatus::Instance()->m_nDrawPointNum / CStatus::Instance()->m_iFfftVerticalNum) / 2;
	//iNewPos = pMarkInfo->iIndex - iMarkInterval;
	iNewPos = pMarkInfo->iIndex;
//	CStatus::Instance()->m_bMarkEditClick = TRUE;
	CStatus::Instance()->SetMarkClickEvent(TRUE, 1);
	CStatus::Instance()->SetMarkEditStart(iNewPos, 1);
	dblNewDragNum = (double)(iNewPos) / dblOnePixelDrawCount;

	CStatus::Instance()->SetDragAmount(dblNewDragNum, 1);		// 0 : Org Graph 
	//m_Slider_CompareReview.SetPos(iNewDragNum * 2);

	m_cCB_CompareMark.SetCurSel(iMark);

	UpdateData(FALSE);
	CStatus::Instance()->UpdateScrn();
}

void CAVMDReview::OnBnClickedBtnCompareMarkDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCount = CStatus::Instance()->GetRepository()->m_MarkInfo[0].size();

	if (nCount <= 0)
		return;
	int iMark = m_cCB_CompareMark.GetCurSel();
	int iNewPos = 0, iNewDragNum = 0;

	int iStartNum = 0;
	if (CStatus::Instance()->GetMarkClickEvent(1) == TRUE)
		iStartNum = CStatus::Instance()->GetMarkEditStart(1);
	else
		iStartNum = CStatus::Instance()->GetReviewDrawStart(1);

	if (!MarkSearch(iStartNum, 1))
		return;

	CStatus::Instance()->GetRepository()->m_MarkInfo[1].erase(CStatus::Instance()->GetRepository()->m_MarkInfo[1].begin() + iMark);
	CStatus::Instance()->SetMarkClickEvent(FALSE, 1);
	GetRepoMarkInfo(1);

	UpdateData(FALSE);
	CStatus::Instance()->UpdateScrn();
}

void CAVMDReview::OnBnClickedBtnCompareMarkUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCount = CStatus::Instance()->GetRepository()->m_MarkInfo[1].size();
	if (nCount <= 0)
		return;

	int iMarkNumCount = CStatus::Instance()->GetRepository()->m_MarkInfo[1].size();
	int curIndex = m_cCB_CompareMark.GetCurSel();
	CString strChangeText;
	GetDlgItemTextW(m_cCB_CompareMark.GetDlgCtrlID(), strChangeText);
	tMarkInfo *pMarkInfo;

	int iStartNum = 0;
	if (CStatus::Instance()->GetMarkClickEvent(1) == TRUE)
		iStartNum = CStatus::Instance()->GetMarkEditStart(1);
	else
		iStartNum = CStatus::Instance()->GetReviewDrawStart(1);

	if (!MarkSearch(iStartNum, 1))
	{
		tMarkInfo *pNewMarkInfo = new tMarkInfo();
		pNewMarkInfo->iIndex = iStartNum;
		pNewMarkInfo->strMark = strChangeText;
		MarkInsert(pNewMarkInfo, 1);
	}
	else
	{
		pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[1].at(m_iMarkCompareCurrentIndex);
		pMarkInfo->strMark = strChangeText;
	}
	int iMarkCount = CStatus::Instance()->GetRepository()->m_MarkInfo[1].size();
	int iSearchIndex = 0;
	for (int i = 0; i < iMarkCount; i++)
	{
		tMarkInfo *pMarkSearch = CStatus::Instance()->GetRepository()->m_MarkInfo[1].at(i);

		if (pMarkSearch->strMark == strChangeText)
		{
			iSearchIndex = i;
		}
	}
	GetRepoMarkInfo(1, iSearchIndex);

	CStatus::Instance()->GetRepository()->ReSaveTagFile(1);
	//CStatus::Instance()->SetReviewDragReset(1);
	CStatus::Instance()->SetMarkClickEvent(FALSE, 1);
	UpdateData(FALSE);
	CStatus::Instance()->UpdateScrn();
}

void CAVMDReview::OnBnClickedBtnCompare()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		CStatus::Instance()->ReviewDataClear(FALSE, TRUE);
		CStatus::Instance()->GetRepository()->ReadFromLogFile(FileLoad.GetPathName(), 1);
		CStatus::Instance()->SetReviewDragReset(1);
		CStatus::Instance()->UpdateReviewComboBox(1);
		CString strPathName = FileLoad.GetPathName();
		CString strFileName = FileLoad.GetFileName();
		strPathName.TrimRight(_T("\\") + strFileName);
		CString strFullName;

		strFullName = strPathName + _T("\\") + strFileName;
		CStatus::Instance()->m_strCompareDataFileName = strFullName;
		CStatus::Instance()->SetMarkClickEvent(FALSE, 1);

		if (CStatus::Instance()->IsFilterUse())
		{
			CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::COMPARE_FILTER);
			CStatus::Instance()->m_nProgressBarCount = 0;
			CProgressBarDlg *pProgressDlg = new CProgressBarDlg;
			CStatus::Instance()->ReviewDataClear(TRUE, TRUE);
			CStatus::Instance()->ReviewDataFiltering();
			pProgressDlg->DoModal();

		}
		CStatus::Instance()->UpdateScrn();

		CStatus::Instance()->SetCompareCondition(TRUE);
		CStatus::Instance()->GraphUpdate();
	}
}

void CAVMDReview::NewPositionDrawGraph()
{
	CString strStartPos;
	int iNewPos = 0, iNewDragNum = 0;
	double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();

	GetDlgItem(IDC_EDIT_DATA)->GetWindowTextW(strStartPos);
	iNewPos = _ttoi(strStartPos);
	int nReviewDataCount = CStatus::Instance()->GetRepository()->GetDataCount(0);
	if (iNewPos >= nReviewDataCount)
	{
		iNewPos = nReviewDataCount;
		CString strTemp;
		strTemp.Format(_T("%d"), iNewPos);
		GetDlgItem(IDC_EDIT_DATA)->SetWindowTextW(strTemp);
	}
		
	iNewDragNum = (int)((double)iNewPos / dblOnePixelDrawCount);

	CStatus::Instance()->SetDragAmount(iNewDragNum, 0);		// 1 : Org Graph 
															/////////////////////////////
	GetDlgItem(IDC_EDIT_COMPARE_DATA)->GetWindowTextW(strStartPos);
	iNewPos = _ttoi(strStartPos);

	int nCompareDataCount = CStatus::Instance()->GetRepository()->GetDataCount(1);
	if (iNewPos >= nCompareDataCount)
	{
		iNewPos = nCompareDataCount;
		CString strTemp;
		strTemp.Format(_T("%d"), iNewPos);
		GetDlgItem(IDC_EDIT_COMPARE_DATA)->SetWindowTextW(strTemp);
	}
	iNewDragNum = (int)((double)iNewPos / dblOnePixelDrawCount);

	CStatus::Instance()->SetDragAmount(iNewDragNum, 1);		// 1 : Org Graph 

	UpdateData(FALSE);

	m_bIsTextChange = TRUE;
	CStatus::Instance()->UpdateScrn();
}

void CAVMDReview::OnBnClickedBtnReviewGo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStatus::Instance()->SetMarkClickEvent(FALSE, 0);
	NewPositionDrawGraph();
}

void CAVMDReview::OnBnClickedBtnCompareGo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStatus::Instance()->SetMarkClickEvent(FALSE, 1);
	NewPositionDrawGraph();
}

void CAVMDReview::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 0:
		Player(m_nMoveStep);
		break;
	case 1:
		Player(-m_nMoveStep);
		break;
	case 2:
		if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
		{
			if (CStatus::Instance()->GetReEnter())
				return;
			CStatus::Instance()->UpdateScrn();
			KillTimer(2);
		}
		break;
	default:
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

void CAVMDReview::Player(int nMoveValue)
{		
	/*if (CStatus::Instance()->m_bIsTimeScalePlay)
	{
		CStatus::Instance()->UpdateScrn();
		return;
	}*/
	//if (CStatus::Instance()->m_bIsPlayStop)
	//{
	//	KillTimer(0);
	//	KillTimer(1);

	//	return;
	//}
	CString strStartPos;
	int iNewPos = 0, iNewDragNum = 0;
	double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();
	int nReviewDataCount = CStatus::Instance()->GetRepository()->GetDataCount(0);
	BOOL bFinishReviewPlay = FALSE, bFinishComparePlay = FALSE;

	GetDlgItem(IDC_EDIT_DATA)->GetWindowTextW(strStartPos);
	iNewPos = _ttoi(strStartPos) + nMoveValue;
	if (iNewPos <= 0)
	{
		iNewPos = 0;
		bFinishReviewPlay = TRUE;
	}
		
	if (iNewPos > nReviewDataCount)
	{
		iNewPos = nReviewDataCount;
		bFinishReviewPlay = TRUE;
	}
	strStartPos.Format(_T("%d"), iNewPos);
	GetDlgItem(IDC_EDIT_DATA)->SetWindowTextW(strStartPos);

	CStatus::Instance()->m_iStartPosText = iNewPos;
	if (iNewPos >= nReviewDataCount)
	{
		iNewPos = nReviewDataCount;
		CString strTemp;
		strTemp.Format(_T("%d"), iNewPos);
		GetDlgItem(IDC_EDIT_DATA)->SetWindowTextW(strTemp);
	}

	iNewDragNum = (int)((double)iNewPos / dblOnePixelDrawCount);

	CStatus::Instance()->SetDragAmount(iNewDragNum, 0);		// 1 : Org Graph 

	if (CStatus::Instance()->GetCompareCondition())
	{
		int nCompareDataCount = CStatus::Instance()->GetRepository()->GetDataCount(1);

		GetDlgItem(IDC_EDIT_COMPARE_DATA)->GetWindowTextW(strStartPos);
		iNewPos = _ttoi(strStartPos) + nMoveValue;

		if (iNewPos <= 0)
		{
			iNewPos = 0;
			bFinishComparePlay = TRUE;
		}
			
		if (iNewPos >= nCompareDataCount)
		{
			iNewPos = nCompareDataCount;
			bFinishComparePlay = TRUE;
		}
	
		strStartPos.Format(_T("%d"), iNewPos);
		GetDlgItem(IDC_EDIT_COMPARE_DATA)->SetWindowTextW(strStartPos);

		if (iNewPos >= nCompareDataCount)
		{
			iNewPos = nCompareDataCount;
			CString strTemp;
			strTemp.Format(_T("%d"), iNewPos);
			GetDlgItem(IDC_EDIT_COMPARE_DATA)->SetWindowTextW(strTemp);
		}
		iNewDragNum = (int)((double)iNewPos / dblOnePixelDrawCount);
		CStatus::Instance()->m_iCompareStartPosText = iNewPos;
		CStatus::Instance()->SetDragAmount(iNewDragNum, 1);		// 1 : Org Graph 
		
	}
	else
	{
		bFinishComparePlay = TRUE;
	}

	m_bIsTextChange = TRUE;
	CStatus::Instance()->UpdateScrn();

	if (bFinishReviewPlay &&bFinishComparePlay)
		PlayerStop();
}

void CAVMDReview::OnBnClickedBtnPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount();
	if (nDataCount == 0)
		return;
	CStatus::Instance()->SetMarkClickEvent(FALSE, 0);
	CStatus::Instance()->SetMarkClickEvent(FALSE, 1);
	if (m_bIsPlayClick && m_bIsBackPlayClick)
	{
		BOOL playClick = m_bIsPlayClick;
		BOOL BackplayClick = m_bIsBackPlayClick;
		btnBackPlay.SetIcon(IDI_ICON_BACKPLAY);
		btnPlay.SetIcon(IDI_ICON_PAUSE);
		m_bIsPlayClick = TRUE;
		m_bIsBackPlayClick = FALSE;
		PlayerControlDisable(FALSE);
		KillTimer(1);
		StartPlayTimer(0);
		//SetTimer(0, 300, NULL);
		CStatus::Instance()->m_bIsPlayBack = TRUE;
	}
	else
	{
		if (m_bIsPlayClick)
		{
			btnBackPlay.SetIcon(IDI_ICON_BACKPLAY);
			btnPlay.SetIcon(IDI_ICON_PLAY);
			m_bIsPlayClick = FALSE;
			PlayerControlDisable(TRUE);
			KillTimer(0);
			KillTimer(1);
			CStatus::Instance()->m_bIsPlayBack = FALSE;
		}
		else
		{
			btnBackPlay.SetIcon(IDI_ICON_BACKPLAY);
			btnPlay.SetIcon(IDI_ICON_PAUSE);
			m_bIsPlayClick = TRUE;
			m_bIsBackPlayClick = FALSE;
			PlayerControlDisable(FALSE);
			StartPlayTimer(0);
			//SetTimer(0, 300, NULL);
			KillTimer(1);
			CStatus::Instance()->m_bIsPlayBack = TRUE;
		}
	}	
}

void CAVMDReview::OnBnClickedBtnBackPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount();
	if (nDataCount == 0)
		return;
	CStatus::Instance()->SetMarkClickEvent(FALSE, 0);
	CStatus::Instance()->SetMarkClickEvent(FALSE, 1);

	if (m_bIsPlayClick && m_bIsBackPlayClick)
	{
		btnPlay.SetIcon(IDI_ICON_PLAY);
		btnBackPlay.SetIcon(IDI_ICON_PAUSE);
		m_bIsBackPlayClick = TRUE;
		m_bIsPlayClick = FALSE;
		PlayerControlDisable(FALSE);
		KillTimer(0);
		StartPlayTimer(1);
		//SetTimer(1, 300, NULL);
		CStatus::Instance()->m_bIsPlayBack = TRUE;
	}
	else
	{
		if (m_bIsBackPlayClick)
		{
			btnPlay.SetIcon(IDI_ICON_PLAY);
			btnBackPlay.SetIcon(IDI_ICON_BACKPLAY);
			m_bIsBackPlayClick = FALSE;
			PlayerControlDisable(TRUE);
			KillTimer(0);
			KillTimer(1);
			CStatus::Instance()->m_bIsPlayBack = FALSE;
		}
		else
		{
			btnPlay.SetIcon(IDI_ICON_PLAY);
			btnBackPlay.SetIcon(IDI_ICON_PAUSE);
			m_bIsBackPlayClick = TRUE;
			m_bIsPlayClick = FALSE;
			PlayerControlDisable(FALSE);
			KillTimer(0);
			StartPlayTimer(1);
			//SetTimer(1, 300, NULL);
			CStatus::Instance()->m_bIsPlayBack = TRUE;
		}
	}
}

void CAVMDReview::OnBnClickedBtnFastBackplay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount();
	if (nDataCount == 0)
		return;
	CStatus::Instance()->SetMarkClickEvent(FALSE, 0);
	CStatus::Instance()->SetMarkClickEvent(FALSE, 1);
	Player(-m_nMoveStep);
}

void CAVMDReview::OnBnClickedBtnFastPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount();
	if (nDataCount == 0)
		return;
	CStatus::Instance()->SetMarkClickEvent(FALSE, 0);
	CStatus::Instance()->SetMarkClickEvent(FALSE, 1);
	Player(m_nMoveStep);
}

void CAVMDReview::PlayerControlDisable(bool bIsDisable)
{
	GetDlgItem(IDC_BTN_FAST_BACKPLAY)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTN_FAST_PLAY)->EnableWindow(bIsDisable);

	GetDlgItem(IDC_CB_MARK)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTN_MARK_PREV)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTN_MARK_NEXT)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTN_MARK_DELETE)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTN_MARK_UPDATE)->EnableWindow(bIsDisable);

	GetDlgItem(IDC_EDIT_DATA)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTN_REVIEW_GO)->EnableWindow(bIsDisable);

	GetDlgItem(IDC_CB_COMPARE_MARK)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTN_COMPARE_MARK_PREV)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTN_COMPARE_MARK_NEXT)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTN_COMPARE_MARK_DELETE)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTN_COMPARE_MARK_UPDATE)->EnableWindow(bIsDisable);

	GetDlgItem(IDC_EDIT_COMPARE_DATA)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTN_COMPARE_GO)->EnableWindow(bIsDisable);
}

void CAVMDReview::TimerActivation()
{
	int iSpeed;
	iSpeed = m_cCBSpeed.GetCurSel();

	if (iSpeed == 0)	m_nMoveStep = 10;
	if (iSpeed == 1)	m_nMoveStep = 50;
	if (iSpeed == 2)	m_nMoveStep = 100;
	if (iSpeed == 3)	m_nMoveStep = 200;
	if (iSpeed == 4)	m_nMoveStep = 300;
	if (iSpeed == 5)	m_nMoveStep = 400;
	if (iSpeed == 6)	m_nMoveStep = 500;
	if (iSpeed == 7)	m_nMoveStep = 600;
	if (iSpeed == 8)	m_nMoveStep = 700;
	if (iSpeed == 9)	m_nMoveStep = 800;
	if (iSpeed == 10)	m_nMoveStep = 900;
	if (iSpeed == 11)	m_nMoveStep = 1000;
}

void CAVMDReview::OnCbnSelchangeCbSpeed()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStatus::Instance()->SetMarkClickEvent(FALSE, 0);
	CStatus::Instance()->SetMarkClickEvent(FALSE, 1);
	TimerActivation();
}

void CAVMDReview::PlayerStop()
{
	GetDlgItem(IDC_BTN_FAST_BACKPLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_FAST_PLAY)->EnableWindow(TRUE);
	PlayerControlDisable(TRUE);

	KillTimer(0);
	KillTimer(1);

	btnFastPlay.SetIcon(IDI_ICON_FAST_PLAY);
	btnBackPlay.SetIcon(IDI_ICON_BACKPLAY);
	btnPlay.SetIcon(IDI_ICON_PLAY);
	btnFastBackPlay.SetIcon(IDI_ICON_FAST_BACKPLAY);
}

void CAVMDReview::MarkInsert(tMarkInfo *pNewMarkInfo, int nInputType)
{
	int iMarkNumCount = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].size();

	tMarkInfo *pMarkInfo;
	int iNewMarkIndex = pNewMarkInfo->iIndex;
	int nIndex = 0;
	for (int i = 0; i < iMarkNumCount; i++)
	{
		pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].at(i);
		if (pMarkInfo->iIndex < iNewMarkIndex)
		{
			nIndex++;
		}
		else
		{
			CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].insert(CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].begin() + nIndex, pNewMarkInfo);
			return;
		}
	}
}

BOOL CAVMDReview::MarkSearch(int nIndex, int nInputType)
{
	int nCount = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].size();
	tMarkInfo *pMarkInfo;
	for (int i = 0; i < nCount; i++)
	{
		pMarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[nInputType].at(i);
		if (pMarkInfo->iIndex == nIndex)
			return TRUE;
	}
	return FALSE;
}


void CAVMDReview::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar)
	{
		// 어떤 슬라이더인지 검사
		if (pScrollBar == (CScrollBar*)&m_Slider_Review)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_Slider_Review.GetPos();
			int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(0);
			int nDrawTotalNum = CStatus::Instance()->m_nDrawPointNum;
			int nMaxStartNum = nDataCount - nDrawTotalNum;
			int iTempNum = nPos;
			CString strStartPos;
			strStartPos.Format(_T("%d"), iTempNum);
			GetDlgItem(IDC_EDIT_DATA)->SetWindowTextW(strStartPos);

			m_bIsMoveSlider = TRUE;
			//CStatus::Instance()->m_bZEditClick = FALSE;
			CStatus::Instance()->SetMarkClickEvent(FALSE, 0);
			if (iTempNum >= nDataCount)
				iTempNum = nDataCount;

			double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();
			int iNewDragNum = 0;
			if (nPos == 0)
			{
				iNewDragNum = 0;
			}
			else if (nPos == nDataCount)
			{
				nPos = nDataCount;
				iNewDragNum = (int)((double)nPos / dblOnePixelDrawCount);
			}
			else
			{
				iNewDragNum = (int)((double)nPos / dblOnePixelDrawCount);
			}

			CStatus::Instance()->SetDragAmount(iNewDragNum, 0);		// 0 : Org Graph 
			int test = m_Slider_Review.GetRangeMax();

			m_Slider_Review.SetPos(nPos);
			UpdateData(FALSE);
			if (CStatus::Instance()->GetReEnter())
			{
				SetTimer(2, 200, NULL);
				return;
			}
			CStatus::Instance()->UpdateScrn();
		}
		if (pScrollBar == (CScrollBar*)&m_Slider_CompareReview)
		{
			int nPos = m_Slider_CompareReview.GetPos();
			int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(1);
			int nDrawTotalNum = CStatus::Instance()->m_nDrawPointNum;
			int nMaxStartNum = nDataCount - nDrawTotalNum;
			int iTempNum = nPos;
			CString strStartPos;
			strStartPos.Format(_T("%d"), iTempNum);
			GetDlgItem(IDC_EDIT_COMPARE_DATA)->SetWindowTextW(strStartPos);

			m_bIsMoveSlider = TRUE;
			CStatus::Instance()->SetMarkClickEvent(FALSE, 1);
			if (iTempNum >= nMaxStartNum)
				iTempNum = nMaxStartNum;

			double dblOnePixelDrawCount = CStatus::Instance()->GetOnePixelDrawCount();
			int iNewDragNum = 0;
			if (nPos == 0)
			{
				iNewDragNum = 0;
			}
			else if (nPos == nDataCount)
			{
				nPos = nDataCount;
				iNewDragNum = (int)((double)nPos / dblOnePixelDrawCount);
			}
			else
			{
				iNewDragNum = (int)((double)nPos / dblOnePixelDrawCount);
			}

			CStatus::Instance()->SetDragAmount(iNewDragNum, 1);		// 0 : Org Graph 

			m_Slider_CompareReview.SetPos(nPos);
			UpdateData(FALSE);
			if (CStatus::Instance()->GetReEnter())
			{
				SetTimer(2, 200, NULL);
				return;
			}
			CStatus::Instance()->UpdateScrn();
		}
	}
	else
	{
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CAVMDReview::OnEnUpdateEditData()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strNum;
	GetDlgItemText(IDC_EDIT_DATA, strNum);
	CStatus::Instance()->m_iStartPosText = _ttoi(strNum);
	if (CStatus::Instance()->m_bIsPlayBack)
	{
		int iNewPos = CStatus::Instance()->m_iStartPosText;
		CStatus::Instance()->SetMarkEditStart(iNewPos, 0);
		CStatus::Instance()->SetMarkClickEvent(TRUE, 0);
	}
}

void CAVMDReview::OnEnUpdateEditCompareData()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strNum;
	GetDlgItemText(IDC_EDIT_COMPARE_DATA, strNum);
	CStatus::Instance()->m_iCompareStartPosText = _ttoi(strNum);
	if (CStatus::Instance()->m_bIsPlayBack)
	{
		int iNewPos = CStatus::Instance()->m_iCompareStartPosText;
		CStatus::Instance()->SetMarkEditStart(iNewPos, 1);
		CStatus::Instance()->SetMarkClickEvent(TRUE, 1);
	}
}


void CAVMDReview::StartPlayTimer(int iForward)
{
	SetTimer(iForward, 300, NULL);
}
