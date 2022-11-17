// AVMDScrollDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "AVMDScrollDlg.h"
#include "afxdialogex.h"
#include "Status.h"

// CAVMDScrollDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAVMDScrollDlg, CDialogEx)

CAVMDScrollDlg::CAVMDScrollDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SCROLL, pParent)
	, m_iScrValue(0)
	, m_iStandardValue(0)
	, m_strGraphType(_T(""))
	, m_nInputType(0)
{
	
}

CAVMDScrollDlg::CAVMDScrollDlg(CString strType, int nInputType /*=NULL*/)
	: m_iScrValue(0)
	, m_iStandardValue(0)
	, m_strGraphType(_T(""))
	, m_nInputType(0)
{
	m_strGraphType = strType;
	m_nInputType = nInputType;
}

CAVMDScrollDlg::~CAVMDScrollDlg()
{
}

void CAVMDScrollDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR, m_ctrlScrollValue);
	DDX_Scroll(pDX, IDC_SCROLLBAR, m_iScrValue);
}


BEGIN_MESSAGE_MAP(CAVMDScrollDlg, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAVMDScrollDlg 메시지 처리기입니다.

BOOL CAVMDScrollDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetSrollInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAVMDScrollDlg::SetSrollInfo()
{
	m_scrInfo.cbSize = sizeof(m_scrInfo);
	m_scrInfo.fMask = SIF_ALL;

	if (CStatus::Instance()->m_strTimeDomainUnit == "g-Force")
	{
		m_scrInfo.nMin = (int)-SCROLL_RATIO;
		m_scrInfo.nMax = (int)SCROLL_RATIO;
		m_scrInfo.nPage = 1;
	}
	if (CStatus::Instance()->m_strTimeDomainUnit == "GAL")
	{
		m_scrInfo.nMin = (int)-SCROLL_RATIO;
		m_scrInfo.nMax = (int)SCROLL_RATIO;
		m_scrInfo.nPage = 1;
	}
	
	m_scrInfo.nTrackPos = m_iStandardValue;
	m_scrInfo.nPos = 0;
	m_ctrlScrollValue.SetScrollInfo(&m_scrInfo);

	if (m_nInputType == 0)
	{
		if (m_strGraphType == "RMS")
			CStatus::Instance()->m_iRMSStandardValue = m_iStandardValue;
		else if (m_strGraphType == 'X')
			CStatus::Instance()->m_iXStandardValue = m_iStandardValue;
		else if (m_strGraphType == 'Y')
			CStatus::Instance()->m_iYStandardValue = m_iStandardValue;
		else if (m_strGraphType == 'Z')
			CStatus::Instance()->m_iZStandardValue = m_iStandardValue;
	}
	if (m_nInputType == 1)
	{
		CStatus::Instance()->m_iCompareStandardValue = m_iStandardValue;
	}
	
}

void CAVMDScrollDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int nSize = CStatus::Instance()->GetRepository()->GetDataCount(0);
	if (nSize == 0)
		return;

	if (pScrollBar)
	{
		if (pScrollBar == (CScrollBar*)&m_ctrlScrollValue)
		{
			SCROLLINFO scrInfo;
			if (pScrollBar->GetScrollInfo(&scrInfo))
			{
				switch (nSBCode)
				{
				case SB_LINEUP:
					scrInfo.nPos -= scrInfo.nPage;
					break;
				case SB_LINEDOWN:
					scrInfo.nPos += scrInfo.nPage;
					break;
				case SB_PAGEUP:
					scrInfo.nPos -= scrInfo.nPage;
					break;
				case SB_PAGEDOWN:
					scrInfo.nPos += scrInfo.nPage;
					break;
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
					scrInfo.nPos = scrInfo.nTrackPos;
					break;
				}
				pScrollBar->SetScrollPos(scrInfo.nPos);
				m_iStandardValue = (-1) * scrInfo.nPos;
			
				if (m_iStandardValue > m_scrInfo.nMax)
					m_iStandardValue = m_scrInfo.nMax;
				if (m_iStandardValue < m_scrInfo.nMin)
					m_iStandardValue = m_scrInfo.nMin;
				
				if (m_nInputType == 0)
				{
					if (m_strGraphType == "RMS")
						CStatus::Instance()->m_iRMSStandardValue = m_iStandardValue;
					else if (m_strGraphType == 'X')
						CStatus::Instance()->m_iXStandardValue = m_iStandardValue;
					else if (m_strGraphType == 'Y')
						CStatus::Instance()->m_iYStandardValue = m_iStandardValue;
					else if (m_strGraphType == 'Z')
						CStatus::Instance()->m_iZStandardValue = m_iStandardValue;
				}
				if (m_nInputType == 1)
				{
					CStatus::Instance()->m_iCompareStandardValue = m_iStandardValue;
				}
			}
		}
	}
	CStatus::Instance()->UpdateScrn();
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CAVMDScrollDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect RectClient;
	GetClientRect(&RectClient);
	if (RectClient.IsRectNull())
		return;
	if (!m_ctrlScrollValue)
		return;

	m_ctrlScrollValue.MoveWindow(RectClient.left+5, RectClient.top, RectClient.Width() - 5, RectClient.Height()-10, 1);
	m_ctrlScrollValue.Invalidate();
}
