
// SCInspectorView.cpp : CXNPIView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "XNPI.h"
#include <functional>
#endif

#include "XNPIDoc.h"
#include "XNPIView.h"
#include "CamDlg.h"
#include "OpPanelDlg.h"
#include "TeachingDlg.h"
#include "Status.h"
#include "StatusPanelDlg.h"
#include "TabDetailLogDlg.h"
#include "Monitoring.h"
#include "SCProcessCtrl.h"
#include "SCHEdgeDetParamDlg.h"
#include "SCAlignParamDlg.h"
#include "Compo.h"
#include "SCAlgoBase.h"
#include "SCFidParamDlg.h"
#include "MessageWnd.h"
#include "SCHeightParamDlg.h"
#include "SCGapGageParamDlg.h"
#include "GraphDlg.h"
#include "HistoLevellingDlg.h"
#include "MainFrm.h"
#include "Repository.h"
#include "ToolPanelDlg.h"
#include "SnapshotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//PLC 재접속 관련 define
#define MAX_PLC_CONNECT_RETRY_COUNT		3
#define PLC_CONNECT_RETRY_DELAY_MS		1000

#define WRITE_DATA_CMD_SEND_TIMER_ID	100
#define WRITE_DATA_CMD_SEND_MS			100
//-----

//#define CMD_INVAILD_INTERVAL_MS			1020	//원래 1초이나 여유를 줌 - 190327 config파일에서 읽어오는것으로 변경
#define MODBUS_READ_FCODE				0x04
#define MODBUS_WRITE_FCODE				0x06

// CXNPIView
IMPLEMENT_DYNCREATE(CXNPIView, CView)

BEGIN_MESSAGE_MAP(CXNPIView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_TOOL_HISTOWINDOWLEVEL, &CXNPIView::OnToolHistowindowlevel)
	ON_UPDATE_COMMAND_UI(ID_TOOL_HISTOWINDOWLEVEL, &CXNPIView::OnUpdateToolHistowindowlevel)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CXNPIView 생성/소멸
CXNPIView::CXNPIView()
	:m_pOpPanelDlg(NULL)
	//, m_pTeachingDlg(NULL)
	, m_nCountOfCam(1)
	, m_pStatusPanelDlg(NULL)
	//, m_pGraphDlg(NULL)
	, m_pHistoLevellingDlg(NULL)
	, m_pToolPanelDlg(NULL)
	, m_nPlcSendcmdNum(1)
	, m_tRStartCMDStart(0)
	, m_tRStartCMDEnd(0)
	, m_tREndCMDStart(0)
	, m_tREndCMDEnd(0)
	, m_tRTriggerCMDStart(0)
	, m_tRTriggerCMDEnd(0)
	, m_bStartCmdFlag(TRUE)
	, m_bEndCmdFlag(TRUE)
	, m_bTriggerCmdFlag(TRUE)
	, m_nPrevTriggerNum(-1)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	for (int i = 0; i < 4; i++)
		m_pCamDlg[i] = NULL;
}

CXNPIView::~CXNPIView()
{
}

BOOL CXNPIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CXNPIView 그리기

void CXNPIView::OnDraw(CDC* /*pDC*/)
{
	CXNPIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CXNPIView 인쇄

BOOL CXNPIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CXNPIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CXNPIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CXNPIView 진단

#ifdef _DEBUG
void CXNPIView::AssertValid() const
{
	CView::AssertValid();
}

void CXNPIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CXNPIDoc* CXNPIView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXNPIDoc)));
	return (CXNPIDoc*)m_pDocument;
}
#endif //_DEBUG


// CXNPIView 메시지 처리기

//! -1 
int CXNPIView::MakeChildWnd()
{
	CRect rectCur;
	GetClientRect(&rectCur);

	for (int i = 0; i < m_nCountOfCam; i++)
	{
		m_pCamDlg[i] = new CCamDlg;
		m_pCamDlg[i]->m_strCamName.Format(_T("%d"), i);
		m_pCamDlg[i]->m_iCamBuildNo = i;
	}

	m_pOpPanelDlg = new COpPanelDlg;
	m_pTeachingDlg = new CTeachingDlg;
	m_pStatusPanelDlg = new CStatusPanelDlg;
	m_pToolPanelDlg = new CToolPanelDlg;
	//m_pGraphDlg = new CGraphDlg;
	m_pHistoLevellingDlg = new CHistoLevellingDlg;
	m_pHistoLevellingDlg->m_pView = this;
	m_pSnapshotDlg = new CSnapshotDlg;

	for (int i = 0; i < m_nCountOfCam; i++)
	{
		if (m_pCamDlg[i]->GetSafeHwnd() != 0)
			return -1;
	}

	if (m_pOpPanelDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pTeachingDlg->GetSafeHwnd() != 0)
		return -1;

	if (m_pStatusPanelDlg->GetSafeHwnd() != 0)
		return -1;

	if (m_pToolPanelDlg->GetSafeHwnd() != 0)
		return -1;

	//if (m_pGraphDlg->GetSafeHwnd() != 0)
	//	return -1;
	if (m_pHistoLevellingDlg->GetSafeHwnd() != 0)
		return -1;

	//m_pSnapshotDlg 추가
	if (m_pSnapshotDlg->GetSafeHwnd() != 0)
		return -1;

	for (int i = 0; i < m_nCountOfCam; i++)
	{
		if (!m_pCamDlg[i]->Create(IDD_CAM, this))
			return -2;
		else
		{
			m_pCamDlg[i]->SetUseAverage(TRUE);	//! Image Avg 사용 설정
			m_pCamDlg[i]->m_xnpiView = this;	//! pipe info등 Tif tag에 쓸정보 가져오기 위해
		}
	}

	//kiy - 0919
	//CSnapshotDlg SaveInspectionImg(...)메서드에서 Take Image()할때 시간이 오래걸린다.
	// Dll로딩문제로 추정돼서 미리 로드하기 위해 출력 디렉토리에 PreLoadMilExportDll.tif파일을 생성한다.
	//TCHAR path[_MAX_PATH];
	//GetModuleFileName(NULL, path, sizeof path);

	//CString outputPath = path;

	//int i = outputPath.ReverseFind('\\');
	//outputPath = outputPath.Left(i);
	//outputPath += _T("\\PreLoadMilExportDll.tif");

	///*CString outputDir = path;
	//outputDir = outputPath.Left(i);
	//outputDir += _T("\\PreLoadDirectoryCreate");*/
	//
	//int y= m_pCamDlg[0]->m_pMilCamProc->GetIDBuffer();
	//int l = m_pCamDlg[0]->m_pMilCamAvgResult_Histo->GetIDBuffer();

	//m_pCamDlg[0]->m_pMilCamProc->Export(outputPath, M_TIFF);
	//m_pCamDlg[0]->m_pMilCamAvgResult_Histo->Export(outputPath, M_TIFF);
	//------------------------------------------------------

	if (!m_pOpPanelDlg->Create(IDD_OPPANEL, this))
		return -2;
	if (!m_pTeachingDlg->Create(IDD_TEACHING, this))
		return -2;
	if (!m_pStatusPanelDlg->Create(IDD_STSPANEL, this))
		return -2;
	if (!m_pToolPanelDlg->Create(IDD_TOOLPANEL, this))
		return -2;
	//	if (!m_pGraphDlg->Create(IDD_GRAPH, this))
	//	return -2;
	if (!m_pHistoLevellingDlg->Create(IDD_HISTOLEVEL, m_pOpPanelDlg))//! 윈도우 레벨링 Dlg를 oppanel의 자식 윈도우로 설정
		return -2;
	if (!m_pSnapshotDlg->Create(IDD_SNAPSHOT_PANEL, this))
		return -2;

	//m_pHistoLevellingDlg->ShowWindow(SW_HIDE);
	m_pHistoLevellingDlg->ShowWindow(SW_SHOW);
	m_pTeachingDlg->ShowWindow(SW_HIDE);
	m_pSnapshotDlg->ShowWindow(SW_SHOW); //kiy

	CSCAlignParamDlg::Instance();
	CSCHEdgeDetParamDlg::Instance();
	CSCFidParamDlg::Instance();
	CSCHeightParamDlg::Instance();
	CSCGapGageParamDlg::Instance();
	CMessageWnd::Instance();
	m_pOpPanelDlg->m_pToolPanelDlg = m_pToolPanelDlg;

	CXNPIApp * pApp = (CXNPIApp *)AfxGetApp();
	CStatus::Instance()->SetRepository(pApp->m_pRepository);

	return 0;
}

int CXNPIView::WndRepositioning()
{
	CRect rectMain;
	CRect rectCam;

	if (!m_pOpPanelDlg)return 0;
	GetClientRect(rectMain);

	CRect rectOpPanel;
	m_pOpPanelDlg->GetWindowRect(&rectOpPanel);
	m_pOpPanelDlg->MoveWindow(0, rectMain.top, rectOpPanel.Width(), rectOpPanel.Height());
	m_pTeachingDlg->MoveWindow(0, rectMain.top, rectOpPanel.Width(), rectOpPanel.Height());
	CRect rectStatusPanel;

	double dblAspectRatio = ((double)CAM_HRES) / CAM_VRES;

	m_rectCamFull.SetRect(CPoint(rectOpPanel.Width(), 0), CPoint(rectMain.right, rectMain.bottom));

	m_pStatusPanelDlg->GetClientRect(&rectStatusPanel);
	m_pStatusPanelDlg->MoveWindow(0, rectOpPanel.Height(), rectOpPanel.Width(), rectStatusPanel.Height());

	CRect rectToolPanel;
	m_pToolPanelDlg->GetClientRect(&rectToolPanel);
	m_pToolPanelDlg->MoveWindow(rectOpPanel.Width() + SPACE_CAMX, rectMain.top, rectToolPanel.Width(), rectOpPanel.Height() + rectStatusPanel.Height());

	//! ToolPanel UI변경, HistoLevel dlg 붙이기
	CRect histoRect;
	m_pHistoLevellingDlg->GetClientRect(&histoRect);

	int opW, histoW, leftMargin;
	int topMargin = 85;

	opW = rectOpPanel.Width();
	histoW = histoRect.Width();
	leftMargin = opW - histoW;
	leftMargin /= 2;

	m_pHistoLevellingDlg->MoveWindow(rectOpPanel.left + leftMargin, rectOpPanel.top + topMargin, histoRect.Width(), histoRect.Height());
	//-----------------------------------------------------------

	//! Snapshot Dlg, 전체 UI의 하단에 추가
	CRect rectView;
	this->GetClientRect(&rectView);
	int x = 0;
	int y = rectOpPanel.Height() + rectStatusPanel.Height() + 5;
	int width = rectView.Width();
	int height = rectView.Height() - y;

	m_pSnapshotDlg->MoveWindow(x, y, width, height);
	//-----------------------------------------------------------

	rectCam.SetRect(rectOpPanel.Width() + rectToolPanel.Width() + SPACE_CAMX * 2, 0, rectMain.right, rectOpPanel.Height() + rectStatusPanel.Height());
	if (m_pCamDlg[0])
		m_pCamDlg[0]->MoveWindow(rectCam);

	return 0;
}

CRect CXNPIView::CameraDlgPos(int nPos, CRect rectMain, int widthCam, int heightCam, int xOffset)
{
	CRect rectCamera;
	CPoint ptStart, ptEnd;
	switch (nPos)
	{
	case 0:
		ptStart.SetPoint(xOffset + rectMain.left, 0);
		break;
	case 1:
		ptStart.SetPoint(xOffset + rectMain.left + widthCam + SPACE_CAMX, 0);
		break;
	case 2:
		ptStart.SetPoint(xOffset + rectMain.left, heightCam + SPACE_CAMY);
		break;
	case 3:
		ptStart.SetPoint(xOffset + rectMain.left + widthCam + SPACE_CAMX, heightCam + SPACE_CAMY);
		break;
	case 4:
		ptStart.SetPoint(xOffset + rectMain.left + (widthCam * 2) + SPACE_CAMX * 2, heightCam + SPACE_CAMY);
		break;
	}
	ptEnd = ptStart;
	ptEnd.Offset(widthCam, heightCam);
	rectCamera.SetRect(ptStart, ptEnd);
	return rectCamera;
}

void CXNPIView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	if (MakeChildWnd())exit(-1);
	if (WndRepositioning())exit(-1);


	CString strTime;
	GetLocalTime(&m_tCur_time);
	strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
		m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);


	CTabDetailLogDlg::Instance()->SetTextLog(strTime + _T("[SYS] Program StartUp"), RGB(50, 50, 50), FALSE);
	CTabDetailLogDlg::Instance()->SetTextLog(strTime + _T("[SYS]XNPI Inspector"), RGB(50, 50, 50), FALSE);
	if (CStatus::Instance()->m_bIsOffLine)
		CTabDetailLogDlg::Instance()->SetTextLog(strTime + _T("[SYS] Offline Mode Start"), RGB(50, 50, 50), FALSE);
	else
	{
		CTabDetailLogDlg::Instance()->SetTextLog(strTime + _T("[SYS] Inline Mode Start"), RGB(50, 50, 50), FALSE);
	}

	if (!CStatus::Instance()->m_bIsOffLine)
	{
		CSCProcessCtrl::Instance()->m_pCamDlg = m_pCamDlg;
		CSCProcessCtrl::Instance()->InitThread();
		CMonitoring::Instance()->Start(); //Emg등을 검사할 Thread
	}


	GetLocalTime(&m_tCur_time);
	strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
		m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

	//! PLC 통신 소켓 초기화
	if (!AfxSocketInit())
	{
		CTabDetailLogDlg::Instance()->SetTextLog(strTime + _T("[SYS] SOCKET INIT FALIED"), RGB(50, 50, 50), FALSE);
		//AfxMessageBox(_T("SOCKET INIT FALIED"));
		MessageBox(_T("SOCKET INIT FALIED"), _T("Socket Error"),
			MB_ICONERROR | MB_OK | MB_SETFOREGROUND | MB_TOPMOST);
	}

	CString plcIp = CStatus::Instance()->m_strPlcIp;
	CString plcPort = CStatus::Instance()->m_strPlcPort;

	m_mdManager = CModbusManager::GetInstance();
	m_mdManager->SetOnReceiveCallback(std::bind(&CXNPIView::SocketOnReceive, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));


	GetLocalTime(&m_tCur_time);
	strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
		m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);


	//! 실제 PLC주소[192.168.0.2], 현재는 시뮬레이터 주소 사용중, 설정에서 변경 가능. //TEST CHECK
	//plcIp = _T("192.168.56.1");
	if (m_mdManager->Connect(plcIp, _ttoi(plcPort)))
	{
		m_mdManager->ReadCmdRoopStart();
		CTabDetailLogDlg::Instance()->SetTextLog(strTime + _T("[SYS] PLC Connected"), RGB(50, 50, 50), FALSE);
	}
	else
	{
		CTabDetailLogDlg::Instance()->SetTextLog(strTime + _T("[SYS] ERROR: Failed to connect PLC server"), RGB(50, 50, 50), FALSE);
		MessageBox(_T("ERROR: Failed to connect PLC server"), _T("PLC Error"),
			MB_ICONERROR | MB_OK | MB_SETFOREGROUND | MB_TOPMOST);
	}
	//----------------------------------------------------------------

	LoadDefaultJob();
}

void CXNPIView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	WndRepositioning();
}

void CXNPIView::SetOPMode(int OPMode)
{
	CStatus::Instance()->SetOPMode((CStatus::tOpMode)OPMode);
	if (OPMode == CStatus::OPTEACHING)
	{
		int nSelCam = CStatus::Instance()->GetSelCamNo();
		m_pOpPanelDlg->ShowWindow(SW_HIDE);
		m_pTeachingDlg->ShowWindow(SW_SHOW);
		//m_pGraphDlg->ShowWindow(SW_HIDE);

		int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;
		for (int i = 0; i < m_nCountOfCam; i++)
		{
			if (i == nSelCam)
			{
				m_pCamDlg[i]->MoveWindow(m_rectCamFull);
				m_pCamDlg[i]->ShowWindow(SW_SHOW);
				m_pCamDlg[i]->RefreshView();
				m_pCamDlg[i]->DrawAllTrack();
			}
			else
				m_pCamDlg[i]->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_pTeachingDlg->ShowWindow(SW_HIDE);
		m_pOpPanelDlg->ShowWindow(SW_SHOW);
		//m_pGraphDlg->ShowWindow(SW_SHOW);
		WndRepositioning();
		for (int i = 0; i < m_nCountOfCam; i++)m_pCamDlg[i]->ShowWindow(SW_SHOW);
	}
}

BOOL CXNPIView::IsTeachingMode()
{
	return m_pTeachingDlg->IsWindowVisible();
}

void CXNPIView::DoTrial()
{
	int nCamNo = CStatus::Instance()->GetSelCamNo();
	CCamDlg *pSelCamDlg = m_pCamDlg[nCamNo];
	if (CStatus::Instance()->m_bIsOffLine)
		pSelCamDlg->ClearView();
	else
		pSelCamDlg->DoGrab();
	pSelCamDlg->Inspect();
	return;

}

int CXNPIView::LoadDefaultJob()
{
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		AfxMessageBox(_T("COM ERROR\r\nCoInitialize Failed!"));
		return -1;
	}
	CStatus::Instance()->GetModelData()->SetModelPath(CStatus::Instance()->GetModelRepository().GetBuffer());
	CStatus::Instance()->GetModelData()->SetMDName(CStatus::Instance()->GetModelName().GetBuffer());
	CStatus::Instance()->GetModelData()->LoadModelData();
	return 0;
}

int CXNPIView::CloseAll()
{
	CSCAlignParamDlg::Instance()->ShowWindow(SW_HIDE);
	CSCHEdgeDetParamDlg::Instance()->ShowWindow(SW_HIDE);
	CSCFidParamDlg::Instance()->ShowWindow(SW_HIDE);
	CSCHeightParamDlg::Instance()->ShowWindow(SW_HIDE);
	CSCGapGageParamDlg::Instance()->ShowWindow(SW_HIDE);
	CMessageWnd::Instance()->ShowWindow(SW_HIDE);

	int nCamCount = CStatus::Instance()->m_nCountOfCam;
	for (int i = 0; i < nCamCount; i++)
	{
		if (m_pCamDlg[i])
		{
			m_pCamDlg[i]->SendMessage(WM_CLOSE);
			Sleep(200);
			delete m_pCamDlg[i];
		}
	}

	return 0;
}

BOOL CXNPIView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CRect rect;
	//GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(234, 239, 245));

	//return TRUE;
	return CView::OnEraseBkgnd(pDC);
}

void CXNPIView::OnToolHistowindowlevel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMenu *hMenu = pFrame->GetMenu();

	if (hMenu->GetMenuState(ID_TOOL_HISTOWINDOWLEVEL, MF_BYCOMMAND) != MF_CHECKED)
	{
		m_pHistoLevellingDlg->ShowWindow(SW_SHOW);
		hMenu->CheckMenuItem(ID_TOOL_HISTOWINDOWLEVEL, MF_CHECKED);
	}
	else
	{
		m_pHistoLevellingDlg->ShowWindow(SW_HIDE);
		hMenu->CheckMenuItem(ID_TOOL_HISTOWINDOWLEVEL, MF_UNCHECKED);
	}

}

void CXNPIView::OnUpdateToolHistowindowlevel(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->m_ulDispMode == CStatus::DISP_MANUAL)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

//kiy - Modbus Receive
void CXNPIView::SocketOnReceive(int transactionID, byte functionCode, int rcvData)
{
	m_mdManager = CModbusManager::GetInstance();
	m_nInvalidInterval_ms = CStatus::Instance()->m_ulPlcReadCMDHoldingTime;

	int start = m_mdManager->m_nStartTR_ID;
	int end = m_mdManager->m_nEndTR_ID;
	int trigger = m_mdManager->m_nTriggerTR_ID;

	int trID = transactionID;
	if (functionCode == MODBUS_READ_FCODE)
	{
		if (start == trID)
		{
			//0, 1 외의 값이 들어올 경우 로그에 에러 메시지 남김
			if (rcvData != 0 && rcvData != 1)
			{
				CTabDetailLogDlg::Instance()->
					SetTextLog(_T("SocketOnReceive [start == trID] ERR!"), RGB(50, 50, 50), FALSE);
				return;
			}

			if (rcvData == 0)
				return;

			if (m_tRStartCMDStart == 0)
				m_tRStartCMDStart = clock();

			m_tRStartCMDEnd = clock();
			clock_t t = m_tRStartCMDEnd - m_tRStartCMDStart;

			//1초이상 지났다면
			if (t > m_nInvalidInterval_ms ||
				m_bStartCmdFlag)
			{
				//log
				{
					GetLocalTime(&m_tCur_time);

					CString strTime;
					strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
						m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

					CTabDetailLogDlg::Instance()->
						SetTextLog(strTime + _T("[CMD RECV] Start Received"), RGB(50, 50, 50), FALSE);
				}

				m_pSnapshotDlg->NewPipeInspectionStart();

				m_tRStartCMDStart = 0;
				m_tRStartCMDEnd = 0;
				m_bStartCmdFlag = FALSE;
			}
		}
		//ERR 처리 추가, 머신 에러 메시지 띄우고 촬영 종료
		//END에서는 현재 ERR처리 이외에 특별한 동작을 수행하지는 않는다
		else if (end == trID) //파이프 검사 종료 CMD
		{
			//0, 1, 2외의 값이 들어올 경우 로그에 에러 메시지 남김
			if (rcvData != 0 && rcvData != 1 && rcvData != 2)
			{
				CTabDetailLogDlg::Instance()->
					SetTextLog(_T("SocketOnReceive [end == trID] ERR!"), RGB(50, 50, 50), FALSE);
				return;
			}

			if (rcvData == 0)
				return;

			if (rcvData == 1)	//End
			{
				if (m_tREndCMDStart == 0)
					m_tREndCMDStart = clock();

				m_tREndCMDEnd = clock();
				clock_t t = m_tREndCMDEnd - m_tREndCMDStart;

				//1초이상 지났다면
				if (t > m_nInvalidInterval_ms ||
					m_bEndCmdFlag)
				{
					//log
					{
						GetLocalTime(&m_tCur_time);

						CString strTime;
						strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
							m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

						CTabDetailLogDlg::Instance()->
							SetTextLog(strTime + _T("[CMD RECV] End Received"), RGB(50, 50, 50), FALSE);
					}

					m_tREndCMDStart = 0;
					m_tREndCMDEnd = 0;
					m_bEndCmdFlag = FALSE;
				}
			}
			if (rcvData == 2) //머신 ERR
			{
				//촬영 중지
				m_pOpPanelDlg->m_bCamIsActivated = FALSE;
				m_pOpPanelDlg->UpdateData(FALSE);
				m_pOpPanelDlg->OnBnClickedChkactiveallcam();

				//에러 메시지 띄움.
				AfxMessageBox(_T("Machine Error Occurred!"), MB_ICONERROR);
			}
		}
		else if (trigger == trID)
		{
			int sectionNum = rcvData;
			if (sectionNum <= 0)
				return;

			if (m_tRTriggerCMDStart == 0)
				m_tRTriggerCMDStart = clock();

			m_tRTriggerCMDEnd = clock();
			clock_t t = m_tRTriggerCMDEnd - m_tRTriggerCMDStart;

			//1초이상 지났다면
			if (t > m_nInvalidInterval_ms ||
				m_bTriggerCmdFlag)
			{
				//log
				{
					GetLocalTime(&m_tCur_time);

					CString strTime;
					strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
						m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

					CString logStr;
					logStr.Format(strTime + _T("[CMD RECV] Trigger Received : %d"), sectionNum);
					CTabDetailLogDlg::Instance()->SetTextLog(logStr, RGB(50, 50, 50), FALSE);
				}

				m_pToolPanelDlg->Triggered(sectionNum);

				m_tRTriggerCMDStart = 0;
				m_tRTriggerCMDEnd = 0;
				m_bTriggerCmdFlag = FALSE;
			}
			else
			{
				//트리거가 1초이내에 발생했을때 다른 Section
				if (m_nPrevTriggerNum != sectionNum)
				{
					//일단 보류
				}
			}

			m_nPrevTriggerNum = sectionNum;
		}
	}

	else if (functionCode == MODBUS_WRITE_FCODE)	//Write Cmd - 확인할 필요는 없음. 디버깅 대비용
		return;
}

void CXNPIView::OnDestroy()
{
	CView::OnDestroy();
	CStatus::Instance()->SaveOnlyPipeInfoCFG();
}

void CXNPIView::OnTimer(UINT_PTR nIDEvent)
{
	CView::OnTimer(nIDEvent);
}