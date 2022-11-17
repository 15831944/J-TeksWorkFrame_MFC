
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "AVMDMonitor.h"

#include "MainFrm.h"
#include "Status.h"
#include "Repository.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.x = 100;
	cs.y = 50;
	cs.cx = 1248;
	cs.cy = 1024;
	// 이미 설정된 윈도우 속성 정보에서 FWS_ADDTOTITLE 속성의 제거.. 
	cs.style = cs.style & (~FWS_ADDTOTITLE);

	if (!CFrameWnd::PreCreateWindow(cs)) return FALSE;
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



void CMainFrame::GetStatusBarRect(CRect &rectStatusBar)
{
	m_wndStatusBar.GetClientRect(&rectStatusBar);
}


void CMainFrame::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		CFrameWnd::OnClose();
		return;
	}

	if (AfxMessageBox(_T("Do you want to exit program ?"), MB_OKCANCEL) == IDOK)
	{
		CStatus::Instance()->SetProgramMode(CStatus::PM_REVIEW);
		CStatus::Instance()->GetRepository()->TerminateFileSaveThread();
		CFrameWnd::OnClose();
	}
	else
	{

	}

}


void CMainFrame::SetTitleCaption(CString strCaption)
{
	CString strToView;
	strToView = _T("AVMD-");
	strToView = strToView + strCaption;
	this->SetWindowTextW(strToView);
}

void CMainFrame::SetTitleCaptionReset()
{
	CString strToView;
	strToView = _T("AVMD");
	this->SetWindowTextW(strToView);
}


void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	static BOOL once = TRUE;

	// 창이 보이는 시점이 프로그램 실행후 최초인지 검사하는 구문
	if (bShow && !IsWindowVisible() && once)
	{
		// 프로그램 실행후 이곳은 한번만 실행되도록 once를 false로...
		once = FALSE;
		this->ShowWindow(SW_SHOWMAXIMIZED);

		this->UpdateWindow();

		CStatus::Instance()->UpdateScrn();
	}
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 최소 크기일때 가로폭
	lpMMI->ptMinTrackSize.x = 1092;
	// 최소 크기일때 세로폭
	lpMMI->ptMinTrackSize.y = 1080;
	// 최대 크기일때 가로폭
	lpMMI->ptMaxTrackSize.x = GetSystemMetrics(SM_CXSCREEN);
	// 최대 크기일때 세로폭
	lpMMI->ptMaxTrackSize.y = GetSystemMetrics(SM_CYSCREEN);
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}
