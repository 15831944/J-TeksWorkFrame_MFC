
// AVMDMonitorView.cpp : CAVMDMonitorView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "AVMDMonitor.h"
#endif

#include "AVMDMonitorDoc.h"
#include "AVMDMonitorView.h"
#include "MainFrm.h"
#include "AVMDDisplayDlg.h"
#include "Status.h"
#include "AVMDDefine.h"
#include "ComsetDlg.h"
#include "SetEventSettingDlg.h"
#include "StationInformationDlg.h"
#include "SetFilterDlg.h"
#include "SetTimeDomainFormatDlg.h"
#include "AnnotateMarkDlg.h"
#include "EnvSettingDlg.h"
#include "MarkEditDlg.h"
#include "PartialSaveDlg.h"
#include "XLEzAutomation.h"
#include <Tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAVMDMonitorView

IMPLEMENT_DYNCREATE(CAVMDMonitorView, CView)

BEGIN_MESSAGE_MAP(CAVMDMonitorView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_MESSAGE(WM_UIUPDATE, &CAVMDMonitorView::OnReceiveDlgMinimizeUpdate)
	ON_MESSAGE(WM_GRAPHUPDATE, &CAVMDMonitorView::OnReceiveGraphUpdate)
	ON_MESSAGE(WM_RESET, &CAVMDMonitorView::OnUpdateReviewMenuReset)
	ON_WM_SIZE()
	ON_COMMAND(ID_SETTINGS_COMPORTSETTING, &CAVMDMonitorView::OnSettingsComportsetting)
	ON_COMMAND(ID_SETTINGS_SETSTATIONINFORMATION, &CAVMDMonitorView::OnSettingsSetstationinformation)
	ON_COMMAND(ID_SETTINGS_SETFILTER, &CAVMDMonitorView::OnSettingsSetfilter)
	ON_COMMAND(ID_SETTINGS_SETTIMEDOMAINFORMAT, &CAVMDMonitorView::OnSettingsSettimedomainformat)
//	ON_COMMAND(ID_OPTIONS, &CAVMDMonitorView::OnOptions)
	ON_COMMAND(ID_SETTINGS_ANNOTATION, &CAVMDMonitorView::OnSettingsAnnotation)
	ON_COMMAND(ID_SELECTCOLORS_RMS, &CAVMDMonitorView::OnSelectcolorsRms)
	ON_COMMAND(ID_SELECTCOLORS_X, &CAVMDMonitorView::OnSelectcolorsX)
	ON_COMMAND(ID_SELECTCOLORS_Y, &CAVMDMonitorView::OnSelectcolorsY)
	ON_COMMAND(ID_SELECTCOLORS_Z, &CAVMDMonitorView::OnSelectcolorsZ)
	ON_COMMAND(ID_SETTINGS_ENVIRONMENTSETTING, &CAVMDMonitorView::OnSettingsEnvironmentsetting)
	ON_COMMAND(ID_SETTINGS_MARKEDITOR, &CAVMDMonitorView::OnSettingsMarkeditor)
	ON_COMMAND(ID_EVENT_SETTINGS, &CAVMDMonitorView::OnEventSettings)
	ON_COMMAND(ID_SELECTCOLORS_TRIGGER, &CAVMDMonitorView::OnSelectcolorsTrigger)
	ON_COMMAND(ID_SELECTCOLORS_GONOGO, &CAVMDMonitorView::OnSelectcolorsGonogo)
	ON_COMMAND(ID_SELECTCOLORS_SPEC_IN, &CAVMDMonitorView::OnSelectcolorsSpecIn)
	ON_COMMAND(ID_SELECTCOLORS_SPEC_OUT, &CAVMDMonitorView::OnSelectcolorsSpecOut)
	ON_COMMAND(ID_VIEW_COMPAREDATA, &CAVMDMonitorView::OnViewComparedata)
	ON_COMMAND(ID_VIEW_OVERLAY, &CAVMDMonitorView::OnViewOverlay)
	ON_COMMAND(ID_SELECTCOLORS_OVERLAY, &CAVMDMonitorView::OnSelectcolorsOverlay)
	ON_COMMAND(ID_SELECTCOLORS_MARKS, &CAVMDMonitorView::OnSelectcolorsMarks)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_MARKEDITOR, &CAVMDMonitorView::OnUpdateSettingsMarkeditor)
	ON_UPDATE_COMMAND_UI(ID_EVENT_SETTINGS, &CAVMDMonitorView::OnUpdateEventSettings)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_SETSTATIONINFORMATION, &CAVMDMonitorView::OnUpdateSettingsSetstationinformation)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COMPAREDATA, &CAVMDMonitorView::OnUpdateViewComparedata)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OVERLAY, &CAVMDMonitorView::OnUpdateViewOverlay)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CAVMDMonitorView::OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_SAVE_SETTINGS_AS, &CAVMDMonitorView::OnUpdateSaveSettingsAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOADCOMPAREDATA, &CAVMDMonitorView::OnUpdateFileLoadcomparedata)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, &CAVMDMonitorView::OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CAVMDMonitorView::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_UPDATE, &CAVMDMonitorView::OnUpdateFileUpdate)
	ON_UPDATE_COMMAND_UI(ID_FILE_PARTIALSAVE, &CAVMDMonitorView::OnUpdateFilePartialsave)
	ON_COMMAND(ID_FILE_PARTIALSAVE, &CAVMDMonitorView::OnFilePartialsave)
	ON_COMMAND(ID_SELECTCOLORS_INDICATOR, &CAVMDMonitorView::OnSelectcolorsIndicator)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEAS, &CAVMDMonitorView::OnUpdateFileSaveas)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVECURRENTFILTEREDCSV, &CAVMDMonitorView::OnUpdateFileSavecurrentfilteredcsv)
END_MESSAGE_MAP()

// CAVMDMonitorView 생성/소멸

CAVMDMonitorView::CAVMDMonitorView()
	: m_pPartialSaveDlg(NULL)
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CAVMDMonitorView::~CAVMDMonitorView()
{
}

BOOL CAVMDMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CAVMDMonitorView 그리기

void CAVMDMonitorView::OnDraw(CDC* /*pDC*/)
{
	CAVMDMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CAVMDMonitorView 인쇄

BOOL CAVMDMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	CFileDialog exlReport(FALSE);
	if (exlReport.DoModal() == IDOK)
	{
		CString exlReportForm = CAppInfo::GetAppDir() + EXL_FORM;
		CString strFileToOpen = exlReport.GetPathName();

		if (exlReportForm.MakeUpper() == strFileToOpen.MakeUpper())
		{
			AfxMessageBox(_T("Don't use the System File.."), MB_OK);
			return FALSE;
		}
		//BeginWaitCursor();

		CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::REPORT);
		//CStatus::Instance()->m_nProgressBarCount = 0;
		CProgressBarDlg *pProgressDlg = new CProgressBarDlg;

		CString strFilePath = exlReport.GetPathName();
		AfxBeginThread(ProcessThread, &strFilePath);

		pProgressDlg->DoModal();
		//MakeExcelReport(exlReport.GetPathName());
		//EndWaitCursor();
	}
	return FALSE;
	return DoPreparePrinting(pInfo);
}

void CAVMDMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CAVMDMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CAVMDMonitorView 진단

#ifdef _DEBUG
void CAVMDMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CAVMDMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAVMDMonitorDoc* CAVMDMonitorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAVMDMonitorDoc)));
	return (CAVMDMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CAVMDMonitorView 메시지 처리기


void CAVMDMonitorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	MakeChildWnd();
	WndRepositioning(CStatus::Instance()->MinimizeNum());
	//GetDlgItem(ID_SETTINGS_MARKEDITOR)->EnableWindow(FALSE);
}


#pragma region Dlg 사이즈 조정
int CAVMDMonitorView::MakeChildWnd()
{
	CRect rectCur;
	GetClientRect(&rectCur);

	m_pDisplayDlg = new CAVMDDisplayDlg;
	m_pViewDlg = new CAVMDViewDlg;
	m_pReviewDlg = new CAVMDReview;
	m_pPartialSaveDlg = new CPartialSaveDlg;

	m_pGraphRMSDlg = new CAVMDGraphDlg(_T("RMS"), 0);
	m_pGraphXDlg = new CAVMDGraphDlg(_T("X"), 0);
	m_pGraphYDlg = new CAVMDGraphDlg(_T("Y"), 0);
	m_pGraphZDlg = new CAVMDGraphDlg(_T("Z"), 0);

	//
	m_pCompareGraphDlg = new CAVMDGraphDlg(_T("X"), 1); // compare 바뀔수 있음

	m_pFFTGraphRMSDlg = new CAVMDFFTGraphDlg(_T("RMS"), 0);
	m_pFFTGraphXDlg = new CAVMDFFTGraphDlg(_T("X"), 0);
	m_pFFTGraphYDlg = new CAVMDFFTGraphDlg(_T("Y"), 0);
	m_pFFTGraphZDlg = new CAVMDFFTGraphDlg(_T("Z"), 0);
	//
	m_pCompareFFTGraphDlg = new CAVMDFFTGraphDlg(_T("X"), 1);// compare 바뀔수 있음


	m_pGraphRMSSrcollDlg = new CAVMDScrollDlg(_T("RMS"), 0);
	m_pGraphXSrcollDlg = new CAVMDScrollDlg(_T("X"), 0);
	m_pGraphYSrcollDlg = new CAVMDScrollDlg(_T("Y"), 0);
	m_pGraphZSrcollDlg = new CAVMDScrollDlg(_T("Z"), 0);
	//
	m_pCompareGraphSrcollDlg = new CAVMDScrollDlg(_T("X"), 1);// compare 바뀔수 있음
	//
	m_pTempDlg1 = new CTempDlg();
	m_pTempDlg2 = new CTempDlg();

	CString strDegree = CStatus::Instance()->m_strDegree;
	CString strFFTDegree = CStatus::Instance()->m_strFFTDegree;
	double dblUnit = CStatus::Instance()->m_dblUnit;
	int iFFTUnit = CStatus::Instance()->m_iFFTUnit;

	m_pTimeDlg = new CAVMDTimeDlg(strDegree, dblUnit, FALSE, 0);
	m_pFFTTimeDlg = new CAVMDTimeDlg(strFFTDegree, iFFTUnit, TRUE, 0);
	m_pCompareTimeDlg = new CAVMDTimeDlg(strDegree, dblUnit, FALSE, 1);
	m_pCompareFFTTimeDlg = new CAVMDTimeDlg(strFFTDegree, iFFTUnit, TRUE, 1);

	if (m_pDisplayDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pViewDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pReviewDlg->GetSafeHwnd() != 0)
		return -1;

	if (m_pGraphRMSDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pGraphXDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pGraphYDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pGraphZDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pCompareGraphDlg->GetSafeHwnd() != 0)
		return -1;


	if (m_pFFTGraphRMSDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pFFTGraphXDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pFFTGraphYDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pFFTGraphZDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pCompareFFTGraphDlg->GetSafeHwnd() != 0)
		return -1;

	if (m_pTimeDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pFFTTimeDlg->GetSafeHwnd() != 0)
		return -1; 
	if (m_pCompareTimeDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pCompareFFTTimeDlg->GetSafeHwnd() != 0)
		return -1;

	if (m_pGraphRMSSrcollDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pGraphXSrcollDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pGraphYSrcollDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pGraphZSrcollDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pCompareGraphSrcollDlg->GetSafeHwnd() != 0)
		return -1;

	if (m_pTempDlg1->GetSafeHwnd() != 0)
		return -1;
	if (m_pTempDlg2->GetSafeHwnd() != 0)
		return -1;

	if (m_pPartialSaveDlg->GetSafeHwnd() != 0)
		return -1;

	if (!m_pDisplayDlg->Create(IDD_DISPLAY, this))
		return -2;
	if (!m_pViewDlg->Create(IDD_VIEW, this))
		return -2;
	if (!m_pReviewDlg->Create(IDD_REVIEW, this))
		return -2;

	if (!m_pGraphRMSDlg->Create(IDD_GRAPH, this))
		return -2;
	if (!m_pGraphXDlg->Create(IDD_GRAPH, this))
		return -2;
	if (!m_pGraphYDlg->Create(IDD_GRAPH, this))
		return -2;
	if (!m_pGraphZDlg->Create(IDD_GRAPH, this))
		return -2;
	if (!m_pCompareGraphDlg->Create(IDD_GRAPH, this))
		return -2;

	if (!m_pFFTGraphRMSDlg->Create(IDD_FFT_GRAPH, this))
		return -2;
	if (!m_pFFTGraphXDlg->Create(IDD_FFT_GRAPH, this))
		return -2;
	if (!m_pFFTGraphYDlg->Create(IDD_FFT_GRAPH, this))
		return -2;
	if (!m_pFFTGraphZDlg->Create(IDD_FFT_GRAPH, this))
		return -2;
	if (!m_pCompareFFTGraphDlg->Create(IDD_FFT_GRAPH, this))
		return -2;

	if (!m_pTimeDlg->Create(IDD_TIME, this))
		return -2;
	if (!m_pFFTTimeDlg->Create(IDD_TIME, this))
		return -2;
	if (!m_pCompareTimeDlg->Create(IDD_TIME, this))
		return -2;
	if (!m_pCompareFFTTimeDlg->Create(IDD_TIME, this))
		return -2;

	if (!m_pGraphRMSSrcollDlg->Create(IDD_SCROLLDLG, this))
		return -2;
	if (!m_pGraphXSrcollDlg->Create(IDD_SCROLLDLG, this))
		return -2; 
	if (!m_pGraphYSrcollDlg->Create(IDD_SCROLLDLG, this))
		return -2;
	if (!m_pGraphZSrcollDlg->Create(IDD_SCROLLDLG, this))
		return -2;
	if (!m_pCompareGraphSrcollDlg->Create(IDD_SCROLLDLG, this))
		return -2;

	if (!m_pTempDlg1->Create(IDD_TEMPDLG, this))
		return -2;
	if (!m_pTempDlg2->Create(IDD_TEMPDLG, this))
		return -2;
	if (!m_pPartialSaveDlg->Create(IDD_PARTIALSAVE, this))
		return -2;

	
	m_pDisplayDlg->m_pView = this;
	CAVMDMonitorDoc *pDoc;
	pDoc = GetDocument();
	CStatus::Instance()->SetRepository(&(pDoc->m_Repository));
	pDoc->m_Repository.MakeDefaultWorkEnvInfo();
	CStatus::Instance()->SetAVMDView(this);

	return 0;
}

int CAVMDMonitorView::WndRepositioning(int nMinimizeNum)
{
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
		WndViewRepositioning(nMinimizeNum);
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
		WndReViewRepositioning(nMinimizeNum);
	return 0;
}

#pragma endregion

void CAVMDMonitorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	 //TODO: 여기에 메시지 처리기 코드를 추가합니다.
	WndRepositioning(CStatus::Instance()->MinimizeNum());
}

LRESULT CAVMDMonitorView::OnReceiveDlgMinimizeUpdate(WPARAM wParam, LPARAM lParam)
{
	WndRepositioning(CStatus::Instance()->MinimizeNum());
	return LRESULT();
}

void CAVMDMonitorView::DlgDeleteDC(CAVMDGraphDlg* pGraphDlg)
{
	if (pGraphDlg->m_bToRepaint)
	{
		if (!pGraphDlg->m_MemDC.m_hDC)
		{
			CStatus::Instance()->GetReEnter();
			pGraphDlg->m_bToRepaint = FALSE;
			return;
		}
		pGraphDlg->m_MemDC.SelectObject(pGraphDlg->pOldBitmap);
		pGraphDlg->m_MemDC.DeleteDC();
		pGraphDlg->m_MemDC.m_hDC = NULL;
		pGraphDlg->m_bToRepaint = FALSE;
	}

	CStatus::Instance()->GetReEnter();
}

void CAVMDMonitorView::FftDlgDeleteDC(CAVMDFFTGraphDlg* pGraphDlg)
{
	if (pGraphDlg->m_bToRepaint)
	{
		if (!pGraphDlg->m_MemDC.m_hDC)
		{
			CStatus::Instance()->GetReEnter();
			pGraphDlg->m_bToRepaint = FALSE;
			return;
		}
		pGraphDlg->m_MemDC.SelectObject(pGraphDlg->pOldBitmap);
		pGraphDlg->m_MemDC.DeleteDC();
		pGraphDlg->m_MemDC.m_hDC = NULL;
		pGraphDlg->m_bToRepaint = FALSE;
	}

	CStatus::Instance()->GetReEnter();
}

LRESULT CAVMDMonitorView::OnReceiveGraphUpdate(WPARAM wParam, LPARAM lParam)
{	
	
	if(CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
	{
		CStatus::Instance()->GetReEnter();
		/*if (CStatus::Instance()->GetReEnter())
			return;*/

		if (CStatus::Instance()->m_bAllReEnter)
			return LRESULT();
		CStatus::Instance()->m_bAllReEnter = TRUE;
		CStatus::Instance()->m_iDataCount = CStatus::Instance()->GetRepository()->GetDataCount(0);
		if (!CStatus::Instance()->IsMinimizeRMS())
			m_pGraphRMSDlg->MakeGraph();

		if (!CStatus::Instance()->IsMinimizeX())
			m_pGraphXDlg->MakeGraph();

		if (!CStatus::Instance()->IsMinimizeY())
			m_pGraphYDlg->MakeGraph();

		if (!CStatus::Instance()->IsMinimizeZ())
			m_pGraphZDlg->MakeGraph();

		m_pTimeDlg->MakeTimeUnit();

		if (CStatus::Instance()->IsFFTShow())
		{
			if (!CStatus::Instance()->IsMinimizeRMS())
				m_pFFTGraphRMSDlg->MakeGraph();
			if (!CStatus::Instance()->IsMinimizeX())
				m_pFFTGraphXDlg->MakeGraph();
			if (!CStatus::Instance()->IsMinimizeY())
				m_pFFTGraphYDlg->MakeGraph();
			if (!CStatus::Instance()->IsMinimizeZ())
				m_pFFTGraphZDlg->MakeGraph();
			m_pFFTTimeDlg->MakeTimeUnit();
		}
		m_pViewDlg->SetSensorState(CStatus::Instance()->m_iTemperature, CStatus::Instance()->m_byBattery);
	}
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		CStatus::Instance()->GetReEnter();
		if (CStatus::Instance()->m_bAllReEnter)
			return LRESULT();
		CStatus::Instance()->m_bAllReEnter = TRUE;
		CStatus::Instance()->m_iDataCount = CStatus::Instance()->GetRepository()->GetDataCount(0);
		CStatus::Instance()->m_iDataCompareCount = CStatus::Instance()->GetRepository()->GetDataCount(1);

		if (!CStatus::Instance()->IsMinimizeRMS())
			m_pGraphRMSDlg->MakeReviewGraph();

		if (!CStatus::Instance()->IsMinimizeX())
			m_pGraphXDlg->MakeReviewGraph();

		if (!CStatus::Instance()->IsMinimizeY())
			m_pGraphYDlg->MakeReviewGraph();

		if (!CStatus::Instance()->IsMinimizeZ())
			m_pGraphZDlg->MakeReviewGraph();

		if (CStatus::Instance()->GetCompareCondition())
		{
			m_pCompareGraphDlg->MakeReviewGraph();
			m_pCompareTimeDlg->MakeTimeUnit();
		}

		m_pTimeDlg->MakeTimeUnit();

		if (CStatus::Instance()->IsFFTShow())
		{
			if (!CStatus::Instance()->IsMinimizeRMS())
				m_pFFTGraphRMSDlg->MakeGraph();
			if (!CStatus::Instance()->IsMinimizeX())
				m_pFFTGraphXDlg->MakeGraph();
			if (!CStatus::Instance()->IsMinimizeY())
				m_pFFTGraphYDlg->MakeGraph();
			if (!CStatus::Instance()->IsMinimizeZ())
				m_pFFTGraphZDlg->MakeGraph();

			if (CStatus::Instance()->GetCompareCondition())
			{
				m_pCompareFFTGraphDlg->MakeGraph();
				m_pCompareFFTTimeDlg->MakeTimeUnit();
			}
				

			m_pFFTTimeDlg->MakeTimeUnit();
			
		}


		CStatus::Instance()->GraphUpdate();
		CStatus::Instance()->ReviewDlgUpdate();

		//MessagePumping();

	}
	return LRESULT();
}

void CAVMDMonitorView::OnSettingsComportsetting()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CComSetDlg dlg;
	dlg.DoModal();
}

void CAVMDMonitorView::OnSettingsSetstationinformation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStationInformationDlg dlg;
	dlg.DoModal();
}

void CAVMDMonitorView::OnSettingsSetfilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetRepository()->GetFileSaveStatus() == CRepository::SS_WRITE)
	{
		AfxMessageBox(_T("You can not change the filter in the save mode."));
		return;
	}
		
	CSetFilterDlg dlg;
	dlg.DoModal();
}

void CAVMDMonitorView::OnSettingsSettimedomainformat()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetRepository()->GetFileSaveStatus() == CRepository::SS_WRITE)
	{
		AfxMessageBox(_T("You can not change the filter in the save mode."));
		return;
	}
	CSetTimeDomainFormatDlg dlg;
	dlg.DoModal();
}

void CAVMDMonitorView::OnSettingsAnnotation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CAnnotateMarkDlg dlg;
	dlg.DoModal();
}

void CAVMDMonitorView::OnSettingsEnvironmentsetting()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CEnvSettingDlg dlg;
	dlg.DoModal();
}

void CAVMDMonitorView::OnSettingsMarkeditor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMarkEditDlg EditDlg;
	EditDlg.DoModal();
}

void CAVMDMonitorView::OnEventSettings()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSetEventSettingDlg dlg;
	dlg.DoModal();
}

#pragma region Menu Change Color
void CAVMDMonitorView::OnSelectcolorsRms()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		//Color color;
		colorREF = dlg.GetColor();
		//color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_RMSPenColor = colorREF;
		CStatus::Instance()->SetDefault();
	}

	CStatus::Instance()->UpdateScrn();
}

void CAVMDMonitorView::OnSelectcolorsX()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		//Color color;
		colorREF = dlg.GetColor();
		//color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_XPenColor = colorREF;
		CStatus::Instance()->SetDefault();
	}

	CStatus::Instance()->UpdateScrn();
}

void CAVMDMonitorView::OnSelectcolorsY()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		//Color color;
		colorREF = dlg.GetColor();
		//color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_YPenColor = colorREF;
		CStatus::Instance()->SetDefault();
	}

	CStatus::Instance()->UpdateScrn();
}

void CAVMDMonitorView::OnSelectcolorsZ()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		//Color color;
		colorREF = dlg.GetColor();
		//color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_ZPenColor = colorREF;
		CStatus::Instance()->SetDefault();
	}

	CStatus::Instance()->UpdateScrn();
}

void CAVMDMonitorView::OnSelectcolorsTrigger()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		//Color color;
		colorREF = dlg.GetColor();
		//color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_TriggerColor = colorREF;
		CStatus::Instance()->SetDefault();
	}

	CStatus::Instance()->UpdateScrn();
}

void CAVMDMonitorView::OnSelectcolorsGonogo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		//Color color;
		colorREF = dlg.GetColor();
		//color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_GoNoGoColor = colorREF;
		CStatus::Instance()->SetDefault();
	}

	CStatus::Instance()->UpdateScrn();
}

void CAVMDMonitorView::OnSelectcolorsSpecIn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		//Color color;
		colorREF = dlg.GetColor();
		//color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_SpectInColor = colorREF;
		CStatus::Instance()->SetDefault();
	}

	CStatus::Instance()->UpdateScrn();
}

void CAVMDMonitorView::OnSelectcolorsSpecOut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		//Color color;
		colorREF = dlg.GetColor();
		//color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_SpectOutColor = colorREF;
		CStatus::Instance()->SetDefault();
	}

	CStatus::Instance()->UpdateScrn();
}

void CAVMDMonitorView::OnSelectcolorsOverlay()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		//Color color;
		colorREF = dlg.GetColor();
		//color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_OverlayColor = colorREF;
		CStatus::Instance()->SetDefault();
	}

	CStatus::Instance()->UpdateScrn();
}

void CAVMDMonitorView::OnSelectcolorsMarks()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		//Color color;
		colorREF = dlg.GetColor();
		//color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_MarkColor = colorREF;
		CStatus::Instance()->SetDefault();
	}

	CStatus::Instance()->UpdateScrn();
}
#pragma endregion

int CAVMDMonitorView::WndViewRepositioning(int nMinimizeNum)
{
	CRect mainRect;
	CRect displayRect;
	CRect timeDlgRect;
	CRect scrollDlgRect;
	Point graphFFTAreaStartPoint;
	Point graphAreaStartPoint;
	Point graphScrollStartPoint;

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();


	if (pMainFrame == NULL)
		return 0;
	if ((CStatus::Instance()->MinimizeNum()) == 0)
		return 0;

	GetClientRect(mainRect);
	m_pDisplayDlg->GetClientRect(&displayRect);
	m_pTimeDlg->GetClientRect(&timeDlgRect);
	m_pGraphRMSSrcollDlg->GetClientRect(&scrollDlgRect);

	int nGraphDlgwidth = 0, nGraphFFTDlgWidth =0;
	int nGraphDlgheight = (mainRect.Height() - timeDlgRect.Height()) / nMinimizeNum;
	int nfirstGraph = 0;

	if (CStatus::Instance()->IsFFTShow())
	{
		nGraphDlgwidth = (int)((double)((mainRect.Width() - scrollDlgRect.Width() - displayRect.Width())) * 48. / 100.);
		nGraphFFTDlgWidth = (int)((double)((mainRect.Width() - scrollDlgRect.Width() - displayRect.Width())) *52. / 100.);
	}
		
	else
	{
		nGraphDlgwidth = (mainRect.Width() - scrollDlgRect.Width() - displayRect.Width());
		nGraphFFTDlgWidth = nGraphFFTDlgWidth;
	}
		

	graphFFTAreaStartPoint.X = displayRect.right;
	graphFFTAreaStartPoint.Y = 0;

	graphAreaStartPoint.X = graphFFTAreaStartPoint.X + nGraphFFTDlgWidth;
	graphAreaStartPoint.Y = 0;

	graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
	graphScrollStartPoint.Y = 0;

	m_pDisplayDlg->MoveWindow(0, 0, displayRect.Width(), displayRect.Height());
	m_pViewDlg->MoveWindow(0, displayRect.Height(), displayRect.Width(), mainRect.Height() - displayRect.Height());
	m_pReviewDlg->MoveWindow(0, displayRect.Height(), displayRect.Width(), mainRect.Height() - displayRect.Height());

	m_pViewDlg->ShowWindow(SW_SHOW);
	m_pReviewDlg->ShowWindow(SW_HIDE);
	m_pTempDlg1->ShowWindow(SW_SHOW);
	m_pTempDlg2->ShowWindow(SW_HIDE);
#pragma region 그래프 배치
	if (CStatus::Instance()->IsMinimizeRMS())
	{
		CStatus::Instance()->m_bRMSReEnter = FALSE;
		CStatus::Instance()->m_bFftRMSReEnter = FALSE;

		DlgDeleteDC(m_pGraphRMSDlg);
		FftDlgDeleteDC(m_pFFTGraphRMSDlg);

		m_pGraphRMSDlg->ShowWindow(SW_HIDE);
		m_pGraphRMSSrcollDlg->ShowWindow(SW_HIDE);
		m_pFFTGraphRMSDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		if (nfirstGraph == 0)
			nfirstGraph++;
		if (CStatus::Instance()->IsFFTShow())
		{
			m_pFFTGraphRMSDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
			m_pGraphRMSDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
			m_pGraphRMSSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

			graphFFTAreaStartPoint.Y += nGraphDlgheight;
			graphAreaStartPoint.Y += nGraphDlgheight;
			graphScrollStartPoint.Y += nGraphDlgheight;

			m_pFFTGraphRMSDlg->ShowWindow(SW_SHOW);
			m_pGraphRMSDlg->ShowWindow(SW_SHOW);
			m_pGraphRMSSrcollDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			graphAreaStartPoint = graphFFTAreaStartPoint;
			graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
			graphScrollStartPoint.Y = graphScrollStartPoint.Y;

			m_pGraphRMSDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
			m_pGraphRMSSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

			graphFFTAreaStartPoint.Y += nGraphDlgheight;
			graphScrollStartPoint.Y += nGraphDlgheight;

			CStatus::Instance()->m_bFftRMSReEnter = FALSE;
			FftDlgDeleteDC(m_pFFTGraphRMSDlg);

			m_pFFTGraphRMSDlg->ShowWindow(SW_HIDE);
			m_pGraphRMSDlg->ShowWindow(SW_SHOW);
			m_pGraphRMSSrcollDlg->ShowWindow(SW_SHOW);
		}
	}
	if (CStatus::Instance()->IsMinimizeX())
	{
		CStatus::Instance()->m_bXReEnter = FALSE;
		CStatus::Instance()->m_bFftXReEnter = FALSE;

		DlgDeleteDC(m_pGraphXDlg);
		FftDlgDeleteDC(m_pFFTGraphXDlg);

		m_pGraphXSrcollDlg->ShowWindow(SW_HIDE);
		m_pFFTGraphXDlg->ShowWindow(SW_HIDE);
		m_pGraphXDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		if (nfirstGraph == 0)
			nfirstGraph++;
		if (CStatus::Instance()->IsFFTShow())
		{
			m_pFFTGraphXDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
			m_pGraphXDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
			m_pGraphXSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

			graphFFTAreaStartPoint.Y += nGraphDlgheight;
			graphAreaStartPoint.Y += nGraphDlgheight;
			graphScrollStartPoint.Y += nGraphDlgheight;

			m_pFFTGraphXDlg->ShowWindow(SW_SHOW);
			m_pGraphXDlg->ShowWindow(SW_SHOW);
			m_pGraphXSrcollDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			graphAreaStartPoint = graphFFTAreaStartPoint;
			graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
			graphScrollStartPoint.Y = graphScrollStartPoint.Y;

			m_pGraphXDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
			m_pGraphXSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

			graphFFTAreaStartPoint.Y += nGraphDlgheight;
			graphScrollStartPoint.Y += nGraphDlgheight;

			FftDlgDeleteDC(m_pFFTGraphXDlg);
			CStatus::Instance()->m_bFftXReEnter = FALSE;

			m_pFFTGraphXDlg->ShowWindow(SW_HIDE);
			//
			m_pGraphXDlg->ShowWindow(SW_SHOW);
			m_pGraphXSrcollDlg->ShowWindow(SW_SHOW);
		}
	}

	if (CStatus::Instance()->IsMinimizeY())
	{
		CStatus::Instance()->m_bYReEnter = FALSE;
		CStatus::Instance()->m_bFftYReEnter = FALSE;

		FftDlgDeleteDC(m_pFFTGraphYDlg);
		DlgDeleteDC(m_pGraphYDlg);

		m_pGraphYSrcollDlg->ShowWindow(SW_HIDE);
		m_pFFTGraphYDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		if (nfirstGraph == 0)
			nfirstGraph++;
		if (CStatus::Instance()->IsFFTShow())
		{
			m_pFFTGraphYDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
			m_pGraphYDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
			m_pGraphYSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

			graphFFTAreaStartPoint.Y += nGraphDlgheight;
			graphAreaStartPoint.Y += nGraphDlgheight;
			graphScrollStartPoint.Y += nGraphDlgheight;

			m_pFFTGraphYDlg->ShowWindow(SW_SHOW);
			m_pGraphYDlg->ShowWindow(SW_SHOW);
			m_pGraphYSrcollDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			graphAreaStartPoint = graphFFTAreaStartPoint;
			graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
			graphScrollStartPoint.Y = graphScrollStartPoint.Y;

			m_pGraphYDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
			m_pGraphYSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

			graphFFTAreaStartPoint.Y += nGraphDlgheight;
			graphScrollStartPoint.Y += nGraphDlgheight;

			FftDlgDeleteDC(m_pFFTGraphYDlg);
			CStatus::Instance()->m_bFftYReEnter = FALSE;

			m_pFFTGraphYDlg->ShowWindow(SW_HIDE);
			//
			m_pGraphYDlg->ShowWindow(SW_SHOW);
			m_pGraphYSrcollDlg->ShowWindow(SW_SHOW);
		}
	}

	if (CStatus::Instance()->IsMinimizeZ())
	{
		CStatus::Instance()->m_bZReEnter = FALSE;
		CStatus::Instance()->m_bFftZReEnter = FALSE;

		DlgDeleteDC(m_pGraphZDlg);
		FftDlgDeleteDC(m_pFFTGraphZDlg);

		m_pGraphZSrcollDlg->ShowWindow(SW_HIDE);
		m_pFFTGraphZDlg->ShowWindow(SW_HIDE);
		m_pGraphZDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		if (nfirstGraph == 0)
			nfirstGraph++;
		if (CStatus::Instance()->IsFFTShow())
		{
			m_pFFTGraphZDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
			m_pGraphZDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
			m_pGraphZSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

			graphFFTAreaStartPoint.Y += nGraphDlgheight;
			graphAreaStartPoint.Y += nGraphDlgheight;
			graphScrollStartPoint.Y += nGraphDlgheight;

			m_pFFTGraphZDlg->ShowWindow(SW_SHOW);
			m_pGraphZDlg->ShowWindow(SW_SHOW);
			m_pGraphZSrcollDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			graphAreaStartPoint = graphFFTAreaStartPoint;
			graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
			graphScrollStartPoint.Y = graphScrollStartPoint.Y;

			m_pGraphZDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
			m_pGraphZSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

			graphFFTAreaStartPoint.Y += nGraphDlgheight;
			graphScrollStartPoint.Y += nGraphDlgheight;

			FftDlgDeleteDC(m_pFFTGraphZDlg);
			CStatus::Instance()->m_bFftZReEnter = FALSE;

			m_pFFTGraphZDlg->ShowWindow(SW_HIDE);
			//
			m_pGraphZDlg->ShowWindow(SW_SHOW);
			m_pGraphZSrcollDlg->ShowWindow(SW_SHOW);
		}
	}
#pragma region Time
	if (CStatus::Instance()->IsFFTShow())
	{
		m_pFFTTimeDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, timeDlgRect.Height());
		m_pTimeDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, timeDlgRect.Height());
		//m_pGraphZSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);
		m_pTempDlg1->MoveWindow(graphAreaStartPoint.X + nGraphDlgwidth, graphAreaStartPoint.Y, mainRect.right, timeDlgRect.Height());
		m_pFFTTimeDlg->ShowWindow(SW_SHOW);
		m_pTimeDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		graphAreaStartPoint = graphFFTAreaStartPoint;
		m_pTimeDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, timeDlgRect.Height());
		CStatus::Instance()->m_bFFTTimeDlgReEnter = FALSE;
		m_pFFTTimeDlg->ShowWindow(SW_HIDE);
		m_pTimeDlg->ShowWindow(SW_SHOW);
	}
#pragma endregion
#pragma endregion

	this->SendMessage(WM_GRAPHUPDATE);
	Invalidate(FALSE);
	return 0;
}

int CAVMDMonitorView::WndReViewRepositioning(int nMinimizeNum)
{
	CRect mainRect;
	CRect displayRect;
	CRect timeDlgRect;
	CRect scrollDlgRect;
	Point graphFFTAreaStartPoint;
	Point graphAreaStartPoint;
	Point graphScrollStartPoint;

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

	if (pMainFrame == NULL)
		return 0;
	if ((CStatus::Instance()->MinimizeNum()) == 0)
		return 0;

	BOOL bIsCompare = CStatus::Instance()->GetCompareCondition();

	GetClientRect(mainRect);
	m_pDisplayDlg->GetClientRect(&displayRect);
	m_pTimeDlg->GetClientRect(&timeDlgRect);
	m_pGraphRMSSrcollDlg->GetClientRect(&scrollDlgRect);

	int nGraphDlgwidth = 0, nGraphFFTDlgWidth =0;
	int nGraphDlgheight = 0;// (mainRect.Height() - timeDlgRect.Height()) / nMinimizeNum;
	int nfirstGraph = 0;

	if (bIsCompare == TRUE)
	{
		nGraphDlgheight = (mainRect.Height() - (2*timeDlgRect.Height())) / (nMinimizeNum+1);	// 2 : TimeDlg 2개 , 1 : CompareDlg 1개 추가
		if (CStatus::Instance()->IsFFTShow())
		{
			//nGraphDlgwidth = (mainRect.Width() - scrollDlgRect.Width() - displayRect.Width()) / 2;
			nGraphDlgwidth = (int)((double)((mainRect.Width() - scrollDlgRect.Width() - displayRect.Width())) * 48. / 100.);
			nGraphFFTDlgWidth = (int)((double)((mainRect.Width() - scrollDlgRect.Width() - displayRect.Width())) *52. / 100.);
		}
			
		else
		{
			nGraphDlgwidth = (mainRect.Width() - scrollDlgRect.Width() - displayRect.Width()); 
			nGraphFFTDlgWidth = nGraphDlgwidth;
		}
			
		graphFFTAreaStartPoint.X = displayRect.right;
		graphFFTAreaStartPoint.Y = 0;

		graphAreaStartPoint.X = graphFFTAreaStartPoint.X + nGraphFFTDlgWidth;
		graphAreaStartPoint.Y = 0;

		graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
		graphScrollStartPoint.Y = 0;

		m_pDisplayDlg->MoveWindow(0, 0, displayRect.Width(), displayRect.Height());
		m_pViewDlg->MoveWindow(0, displayRect.Height(), displayRect.Width(), mainRect.Height() - displayRect.Height());
		m_pReviewDlg->MoveWindow(0, displayRect.Height(), displayRect.Width(), mainRect.Height() - displayRect.Height());

		m_pViewDlg->ShowWindow(SW_HIDE);
		m_pReviewDlg->ShowWindow(SW_SHOW);	
#pragma region 그래프 배치
		if (CStatus::Instance()->IsMinimizeRMS())
		{
			CStatus::Instance()->m_bRMSReEnter = FALSE;
			CStatus::Instance()->m_bFftRMSReEnter = FALSE;

			DlgDeleteDC(m_pGraphRMSDlg);
			FftDlgDeleteDC(m_pFFTGraphRMSDlg);

			m_pGraphRMSDlg->ShowWindow(SW_HIDE);
			m_pGraphRMSSrcollDlg->ShowWindow(SW_HIDE);
			m_pFFTGraphRMSDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			if (nfirstGraph == 0)
				nfirstGraph++;
			if (CStatus::Instance()->IsFFTShow())
			{
				m_pFFTGraphRMSDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
				m_pGraphRMSDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphRMSSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				m_pFFTGraphRMSDlg->ShowWindow(SW_SHOW);
				m_pGraphRMSDlg->ShowWindow(SW_SHOW);
				m_pGraphRMSSrcollDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				graphAreaStartPoint = graphFFTAreaStartPoint;
				graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
				graphScrollStartPoint.Y = graphScrollStartPoint.Y;

				m_pGraphRMSDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphRMSSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				CStatus::Instance()->m_bFftRMSReEnter = FALSE;
				FftDlgDeleteDC(m_pFFTGraphRMSDlg);

				m_pFFTGraphRMSDlg->ShowWindow(SW_HIDE);
				m_pGraphRMSDlg->ShowWindow(SW_SHOW);
				m_pGraphRMSSrcollDlg->ShowWindow(SW_SHOW);
			}
		}
		if (CStatus::Instance()->IsMinimizeX())
		{
			CStatus::Instance()->m_bXReEnter = FALSE;
			CStatus::Instance()->m_bFftXReEnter = FALSE;

			DlgDeleteDC(m_pGraphXDlg);
			FftDlgDeleteDC(m_pFFTGraphXDlg);

			m_pGraphXSrcollDlg->ShowWindow(SW_HIDE);
			m_pFFTGraphXDlg->ShowWindow(SW_HIDE);
			m_pGraphXDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			if (nfirstGraph == 0)
				nfirstGraph++;
			if (CStatus::Instance()->IsFFTShow())
			{
				m_pFFTGraphXDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
				m_pGraphXDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphXSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				m_pFFTGraphXDlg->ShowWindow(SW_SHOW);
				m_pGraphXDlg->ShowWindow(SW_SHOW);
				m_pGraphXSrcollDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				graphAreaStartPoint = graphFFTAreaStartPoint;
				graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
				graphScrollStartPoint.Y = graphScrollStartPoint.Y;

				m_pGraphXDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphXSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				FftDlgDeleteDC(m_pFFTGraphXDlg);
				CStatus::Instance()->m_bFftXReEnter = FALSE;

				m_pFFTGraphXDlg->ShowWindow(SW_HIDE);
				m_pGraphXDlg->ShowWindow(SW_SHOW);
				m_pGraphXSrcollDlg->ShowWindow(SW_SHOW);
			}
		}

		if (CStatus::Instance()->IsMinimizeY())
		{
			CStatus::Instance()->m_bYReEnter = FALSE;
			CStatus::Instance()->m_bFftYReEnter = FALSE;

			FftDlgDeleteDC(m_pFFTGraphYDlg);
			DlgDeleteDC(m_pGraphYDlg);

			m_pGraphYSrcollDlg->ShowWindow(SW_HIDE);
			m_pFFTGraphYDlg->ShowWindow(SW_HIDE);
			m_pGraphYDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			if (nfirstGraph == 0)
				nfirstGraph++;
			if (CStatus::Instance()->IsFFTShow())
			{
				m_pFFTGraphYDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
				m_pGraphYDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphYSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				m_pFFTGraphYDlg->ShowWindow(SW_SHOW);
				m_pGraphYDlg->ShowWindow(SW_SHOW);
				m_pGraphYSrcollDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				graphAreaStartPoint = graphFFTAreaStartPoint;
				graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
				graphScrollStartPoint.Y = graphScrollStartPoint.Y;

				m_pGraphYDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphYSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				FftDlgDeleteDC(m_pFFTGraphYDlg);
				CStatus::Instance()->m_bFftYReEnter = FALSE;

				m_pFFTGraphYDlg->ShowWindow(SW_HIDE);
				m_pGraphYDlg->ShowWindow(SW_SHOW);
				m_pGraphYSrcollDlg->ShowWindow(SW_SHOW);
			}
		}

		if (CStatus::Instance()->IsMinimizeZ())
		{
			CStatus::Instance()->m_bZReEnter = FALSE;
			CStatus::Instance()->m_bFftZReEnter = FALSE;

			DlgDeleteDC(m_pGraphZDlg);
			FftDlgDeleteDC(m_pFFTGraphZDlg);

			m_pGraphZSrcollDlg->ShowWindow(SW_HIDE);
			m_pFFTGraphZDlg->ShowWindow(SW_HIDE);
			m_pGraphZDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			if (nfirstGraph == 0)
				nfirstGraph++;
			if (CStatus::Instance()->IsFFTShow())
			{
				m_pFFTGraphZDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
				m_pGraphZDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphZSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				m_pFFTGraphZDlg->ShowWindow(SW_SHOW);
				m_pGraphZDlg->ShowWindow(SW_SHOW);
				m_pGraphZSrcollDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				graphAreaStartPoint = graphFFTAreaStartPoint;
				graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
				graphScrollStartPoint.Y = graphScrollStartPoint.Y;

				m_pGraphZDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphZSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				FftDlgDeleteDC(m_pFFTGraphZDlg);
				CStatus::Instance()->m_bFftZReEnter = FALSE;


				m_pFFTGraphZDlg->ShowWindow(SW_HIDE);
				m_pGraphZDlg->ShowWindow(SW_SHOW);
				m_pGraphZSrcollDlg->ShowWindow(SW_SHOW);
			}
		}
#pragma region Time
		if (CStatus::Instance()->IsFFTShow())
		{
			m_pFFTTimeDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, timeDlgRect.Height());
			m_pTimeDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, timeDlgRect.Height());

			graphFFTAreaStartPoint.Y += timeDlgRect.Height();
			graphAreaStartPoint.Y += timeDlgRect.Height();

			m_pFFTTimeDlg->ShowWindow(SW_SHOW);
			m_pTimeDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			graphAreaStartPoint = graphFFTAreaStartPoint;
			graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
			graphScrollStartPoint.Y = graphScrollStartPoint.Y;
			m_pTimeDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, timeDlgRect.Height());

			graphFFTAreaStartPoint.Y += timeDlgRect.Height();

			m_pFFTTimeDlg->ShowWindow(SW_HIDE);
			m_pTimeDlg->ShowWindow(SW_SHOW);
		}
#pragma endregion
		m_pTempDlg1->ShowWindow(SW_SHOW);
		m_pTempDlg1->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, timeDlgRect.Width(), timeDlgRect.Height());
		graphScrollStartPoint.Y += timeDlgRect.Height();
#pragma region Compare Dlg
		if (CStatus::Instance()->IsFFTShow())
		{
			m_pCompareFFTGraphDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
			
			m_pCompareGraphDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
			m_pCompareGraphSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

			m_pCompareFFTGraphDlg->ShowWindow(SW_SHOW);
			m_pCompareGraphDlg->ShowWindow(SW_SHOW);
			m_pCompareGraphSrcollDlg->ShowWindow(SW_SHOW);
			
			graphFFTAreaStartPoint.Y += nGraphDlgheight;
			graphAreaStartPoint.Y += nGraphDlgheight;
			graphScrollStartPoint.Y += nGraphDlgheight;

			m_pCompareFFTTimeDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, timeDlgRect.Height());
			m_pCompareTimeDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, timeDlgRect.Height());

			m_pCompareFFTTimeDlg->ShowWindow(SW_SHOW);
			m_pCompareTimeDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			graphAreaStartPoint = graphFFTAreaStartPoint;

			m_pCompareGraphDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
			m_pCompareGraphSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);
			graphScrollStartPoint.Y += nGraphDlgheight;
			graphAreaStartPoint.Y += nGraphDlgheight;

			m_pCompareTimeDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, timeDlgRect.Height());

			CStatus::Instance()->m_bFFtCompareReEnter = FALSE;
			FftDlgDeleteDC(m_pCompareFFTGraphDlg);

			m_pCompareFFTGraphDlg->ShowWindow(SW_HIDE);
			m_pCompareFFTTimeDlg->ShowWindow(SW_HIDE);

			m_pCompareGraphDlg->ShowWindow(SW_SHOW);
			m_pCompareGraphSrcollDlg->ShowWindow(SW_SHOW);
			m_pCompareTimeDlg->ShowWindow(SW_SHOW);
		}
#pragma endregion
		m_pTempDlg2->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, timeDlgRect.Width(), timeDlgRect.Height());
		m_pTempDlg2->ShowWindow(SW_SHOW);
#pragma endregion
	}
	if (bIsCompare == FALSE)
	{
		DlgDeleteDC(m_pCompareGraphDlg);
		m_pCompareGraphDlg->ShowWindow(SW_HIDE);
		m_pCompareGraphSrcollDlg->ShowWindow(SW_HIDE);
		nGraphDlgheight = (mainRect.Height() - timeDlgRect.Height()) / nMinimizeNum;
		if (CStatus::Instance()->IsFFTShow())
		{
			//nGraphDlgwidth = (double)((mainRect.Width() - scrollDlgRect.Width() - displayRect.Width())) / 2;
			nGraphDlgwidth = (int)((double)((mainRect.Width() - scrollDlgRect.Width() - displayRect.Width())) * 48. / 100.);
			nGraphFFTDlgWidth = (int)((double)((mainRect.Width() - scrollDlgRect.Width() - displayRect.Width())) *52. / 100.);
		}
			
		else
		{
			nGraphDlgwidth = (mainRect.Width() - scrollDlgRect.Width() - displayRect.Width());
			nGraphFFTDlgWidth = nGraphDlgwidth;
		}
			
		graphFFTAreaStartPoint.X = displayRect.right;
		graphFFTAreaStartPoint.Y = 0;

		graphAreaStartPoint.X = graphFFTAreaStartPoint.X + nGraphFFTDlgWidth;
		graphAreaStartPoint.Y = 0;

		graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
		graphScrollStartPoint.Y = 0;

		m_pDisplayDlg->MoveWindow(0, 0, displayRect.Width(), displayRect.Height());
		m_pViewDlg->MoveWindow(0, displayRect.Height(), displayRect.Width(), mainRect.Height() - displayRect.Height());
		m_pReviewDlg->MoveWindow(0, displayRect.Height(), displayRect.Width(), mainRect.Height() - displayRect.Height());

		m_pViewDlg->ShowWindow(SW_HIDE);
		m_pReviewDlg->ShowWindow(SW_SHOW);

		CStatus::Instance()->m_bCompareReEnter = FALSE;
		CStatus::Instance()->m_bFFtCompareReEnter = FALSE;

		DlgDeleteDC(m_pCompareGraphDlg);
		FftDlgDeleteDC(m_pCompareFFTGraphDlg);
		m_pCompareFFTGraphDlg->ShowWindow(SW_HIDE);
		m_pCompareGraphDlg->ShowWindow(SW_HIDE);
		m_pCompareGraphSrcollDlg->ShowWindow(SW_HIDE);

		m_pCompareTimeDlg->ShowWindow(SW_HIDE);
		m_pCompareFFTTimeDlg->ShowWindow(SW_HIDE);
#pragma region 그래프 배치
		if (CStatus::Instance()->IsMinimizeRMS())
		{
			CStatus::Instance()->m_bRMSReEnter = FALSE;
			CStatus::Instance()->m_bFftRMSReEnter = FALSE;
			//CStatus::Instance()->m_bFftXReEnter = FALSE;
			//CStatus::Instance()->m_bFftYReEnter = FALSE;
			//CStatus::Instance()->m_bFftZReEnter = FALSE;

			DlgDeleteDC(m_pGraphRMSDlg);
			FftDlgDeleteDC(m_pFFTGraphRMSDlg);

			m_pGraphRMSDlg->ShowWindow(SW_HIDE);
			m_pGraphRMSSrcollDlg->ShowWindow(SW_HIDE);
			m_pFFTGraphRMSDlg->ShowWindow(SW_HIDE);		
		}
		else
		{
			if (nfirstGraph == 0)
				nfirstGraph++;
			if (CStatus::Instance()->IsFFTShow())
			{
				m_pFFTGraphRMSDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
				m_pGraphRMSDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphRMSSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				m_pFFTGraphRMSDlg->ShowWindow(SW_SHOW);
				m_pGraphRMSDlg->ShowWindow(SW_SHOW);
				m_pGraphRMSSrcollDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				graphAreaStartPoint = graphFFTAreaStartPoint;
				graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
				graphScrollStartPoint.Y = graphScrollStartPoint.Y;

				m_pGraphRMSDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphRMSSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				CStatus::Instance()->m_bFftRMSReEnter = FALSE;
				FftDlgDeleteDC(m_pFFTGraphRMSDlg);

				m_pFFTGraphRMSDlg->ShowWindow(SW_HIDE);
				m_pGraphRMSDlg->ShowWindow(SW_SHOW);
				m_pGraphRMSSrcollDlg->ShowWindow(SW_SHOW);
			}
		}
		if (CStatus::Instance()->IsMinimizeX())
		{
			CStatus::Instance()->m_bXReEnter = FALSE;
			//CStatus::Instance()->m_bFftRMSReEnter = FALSE;
			CStatus::Instance()->m_bFftXReEnter = FALSE;
			//CStatus::Instance()->m_bFftYReEnter = FALSE;
			//CStatus::Instance()->m_bFftZReEnter = FALSE;

			DlgDeleteDC(m_pGraphXDlg);
			FftDlgDeleteDC(m_pFFTGraphXDlg);

			m_pGraphXSrcollDlg->ShowWindow(SW_HIDE);
			m_pFFTGraphXDlg->ShowWindow(SW_HIDE);
			m_pGraphXDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			if (nfirstGraph == 0)
				nfirstGraph++;
			if (CStatus::Instance()->IsFFTShow())
			{
				m_pFFTGraphXDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
				m_pGraphXDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphXSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				m_pFFTGraphXDlg->ShowWindow(SW_SHOW);
				m_pGraphXDlg->ShowWindow(SW_SHOW);
				m_pGraphXSrcollDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				graphAreaStartPoint = graphFFTAreaStartPoint;
				graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
				graphScrollStartPoint.Y = graphScrollStartPoint.Y;

				m_pGraphXDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphXSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				FftDlgDeleteDC(m_pFFTGraphXDlg);
				CStatus::Instance()->m_bFftXReEnter = FALSE;

				m_pFFTGraphXDlg->ShowWindow(SW_HIDE);
				m_pGraphXDlg->ShowWindow(SW_SHOW);
				m_pGraphXSrcollDlg->ShowWindow(SW_SHOW);
			}
		}

		if (CStatus::Instance()->IsMinimizeY())
		{
			CStatus::Instance()->m_bYReEnter = FALSE;
			//CStatus::Instance()->m_bFftRMSReEnter = FALSE;
			//CStatus::Instance()->m_bFftXReEnter = FALSE;
			CStatus::Instance()->m_bFftYReEnter = FALSE;
			//CStatus::Instance()->m_bFftZReEnter = FALSE;

			FftDlgDeleteDC(m_pFFTGraphYDlg);
			DlgDeleteDC(m_pGraphYDlg);
			//m_pGraphYDlg->Invalidate();

			m_pGraphYSrcollDlg->ShowWindow(SW_HIDE);
			m_pFFTGraphYDlg->ShowWindow(SW_HIDE);
			m_pGraphYDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			if (nfirstGraph == 0)
				nfirstGraph++;
			if (CStatus::Instance()->IsFFTShow())
			{
				m_pFFTGraphYDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
				m_pGraphYDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphYSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				m_pFFTGraphYDlg->ShowWindow(SW_SHOW);
				m_pGraphYDlg->ShowWindow(SW_SHOW);
				m_pGraphYSrcollDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				graphAreaStartPoint = graphFFTAreaStartPoint;
				graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
				graphScrollStartPoint.Y = graphScrollStartPoint.Y;

				m_pGraphYDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphYSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				FftDlgDeleteDC(m_pFFTGraphYDlg);
				CStatus::Instance()->m_bFftYReEnter = FALSE;

				m_pFFTGraphYDlg->ShowWindow(SW_HIDE);
				m_pGraphYDlg->ShowWindow(SW_SHOW);
				m_pGraphYSrcollDlg->ShowWindow(SW_SHOW);
			}
		}

		if (CStatus::Instance()->IsMinimizeZ())
		{
			CStatus::Instance()->m_bZReEnter = FALSE;
			//CStatus::Instance()->m_bFftRMSReEnter = FALSE;
			//CStatus::Instance()->m_bFftXReEnter = FALSE;
			//CStatus::Instance()->m_bFftYReEnter = FALSE;
			CStatus::Instance()->m_bFftZReEnter = FALSE;

			DlgDeleteDC(m_pGraphZDlg);
			FftDlgDeleteDC(m_pFFTGraphZDlg);

			m_pGraphZSrcollDlg->ShowWindow(SW_HIDE);
			m_pFFTGraphZDlg->ShowWindow(SW_HIDE);
			m_pGraphZDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			if (nfirstGraph == 0)
				nfirstGraph++;
			if (CStatus::Instance()->IsFFTShow())
			{
				m_pFFTGraphZDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, nGraphDlgheight);
				m_pGraphZDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphZSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				m_pFFTGraphZDlg->ShowWindow(SW_SHOW);
				m_pGraphZDlg->ShowWindow(SW_SHOW);
				m_pGraphZSrcollDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				graphAreaStartPoint = graphFFTAreaStartPoint;
				graphScrollStartPoint.X = graphAreaStartPoint.X + nGraphDlgwidth;
				graphScrollStartPoint.Y = graphScrollStartPoint.Y;

				m_pGraphZDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, nGraphDlgheight);
				m_pGraphZSrcollDlg->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, scrollDlgRect.Width(), nGraphDlgheight);

				graphFFTAreaStartPoint.Y += nGraphDlgheight;
				graphScrollStartPoint.Y += nGraphDlgheight;

				FftDlgDeleteDC(m_pFFTGraphZDlg);
				CStatus::Instance()->m_bFftZReEnter = FALSE;
			//.	CStatus::Instance()->m_bCompareReEnter = FALSE;

				m_pFFTGraphZDlg->ShowWindow(SW_HIDE);
				m_pGraphZDlg->ShowWindow(SW_SHOW);
				m_pGraphZSrcollDlg->ShowWindow(SW_SHOW);
			}
		}
#pragma region Time
		if (CStatus::Instance()->IsFFTShow())
		{
			m_pFFTTimeDlg->MoveWindow(graphFFTAreaStartPoint.X, graphFFTAreaStartPoint.Y, nGraphFFTDlgWidth, timeDlgRect.Height());
			m_pTimeDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, timeDlgRect.Height());

			m_pFFTTimeDlg->ShowWindow(SW_SHOW);
			m_pTimeDlg->ShowWindow(SW_SHOW);	
		}
		else
		{
			graphAreaStartPoint = graphFFTAreaStartPoint;
			m_pTimeDlg->MoveWindow(graphAreaStartPoint.X, graphAreaStartPoint.Y, nGraphDlgwidth, timeDlgRect.Height());

			CStatus::Instance()->m_bFFTTimeDlgReEnter = FALSE;
			m_pFFTTimeDlg->ShowWindow(SW_HIDE);
			m_pTimeDlg->ShowWindow(SW_SHOW);
		}
#pragma endregion

		m_pTempDlg1->MoveWindow(graphScrollStartPoint.X, graphScrollStartPoint.Y, timeDlgRect.Width(), timeDlgRect.Height());
		m_pTempDlg2->ShowWindow(SW_HIDE);
#pragma endregion
	}
	
	//m_pReviewDlg->MoveWindow(0, displayRect.Height(), displayRect.Width(), mainRect.Height() - displayRect.Height());
	
	this->SendMessage(WM_GRAPHUPDATE);
	Invalidate(FALSE);
	return 0;
}

void CAVMDMonitorView::OnViewComparedata()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame  *pFrame = (CMainFrame*)AfxGetMainWnd();
	UINT   state = pFrame->GetMenu()->GetMenuState(ID_VIEW_COMPAREDATA, MF_BYCOMMAND);
	if (state & MF_CHECKED)
	{
		CheckMenuItem(::GetMenu(pFrame->m_hWnd), ID_VIEW_COMPAREDATA, MF_UNCHECKED);
		CStatus::Instance()->SetCompareCondition(FALSE);
	}
	else
	{
		CheckMenuItem(::GetMenu(pFrame->m_hWnd), ID_VIEW_COMPAREDATA, MF_CHECKED);
		CStatus::Instance()->SetCompareCondition(TRUE);
	}
	CStatus::Instance()->GraphUpdate();
}

void CAVMDMonitorView::OnViewOverlay()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame  *pFrame = (CMainFrame*)AfxGetMainWnd();
	UINT   state = pFrame->GetMenu()->GetMenuState(ID_VIEW_OVERLAY, MF_BYCOMMAND);
	if (state & MF_CHECKED)
	{
		CheckMenuItem(::GetMenu(pFrame->m_hWnd), ID_VIEW_OVERLAY, MF_UNCHECKED);
		CStatus::Instance()->SetOverlayCondition(FALSE);
	}
	else
	{
		CheckMenuItem(::GetMenu(pFrame->m_hWnd), ID_VIEW_OVERLAY, MF_CHECKED);
		CStatus::Instance()->SetOverlayCondition(TRUE);
	}
	CStatus::Instance()->GraphUpdate();
}

int CAVMDMonitorView::MakeExcelReport(CString strFileName)
{
	CString strThisPath;
	CAVMDMonitorView *pMonitorView = CStatus::Instance()->GetAVMDView();
	CFileDialog SaveExelReport(TRUE); // True : 파일 열기

	CString strLogFile = CStatus::Instance()->GetDataSaveFolder() + _T("\\");
	strLogFile += CStatus::Instance()->GetDataFileName();

	CString strExcelReportForm = CAppInfo::GetAppDir() + EXL_FORM;
	CXLEzAutomation XL(FALSE); // FALSE: 처리 과정을 화면에 보이지 않는다.

	XL.OpenExcelFile(strExcelReportForm);             // 엑셀 파일 열기
	CString strMsg;

	tWorkEnvInfo *workEnvInfo;
	if (!CStatus::Instance()->GetRepository()->m_WorkEnvInfo[0].size())
	{
		workEnvInfo = new tWorkEnvInfo;
		memset(workEnvInfo, 0, sizeof(tWorkEnvInfo));
		CStatus::Instance()->GetRepository()->m_WorkEnvInfo[0].push_back(workEnvInfo);
	}
	else
		workEnvInfo = CStatus::Instance()->GetRepository()->m_WorkEnvInfo[0].at(0);

	CString strMakeDate = CStatus::Instance()->m_strMakeDate[0];

#pragma region Excel에 상세 정보 쓰기
	strMsg.Format(_T(" »  Recording Date : 20%s-%s-%s"), strMakeDate.Mid(0, 2), strMakeDate.Mid(2, 2), strMakeDate.Mid(4, 2));
	XL.SetCellValue(1, 3, strMsg);
	strMsg.Format(_T(" »  Sensor S/N : %s"), CStatus::Instance()->m_strSerialNo[0]);
	XL.SetCellValue(1, 4, strMsg);
	CString strFullPathFileName = CStatus::Instance()->m_strLastOpenFileName;
	strMsg.Format(_T(" »  File Name : %s"), strFullPathFileName.Mid(strFullPathFileName.ReverseFind(_T('\\')) + 1, strFullPathFileName.GetLength()));
	XL.SetCellValue(1, 5, strMsg);

	strMsg.Format(_T(" »  Time domain format : %s"), CStatus::Instance()->m_strTimeDomainUnit);
	XL.SetCellValue(1, 6, strMsg);
	strMsg.Format(_T(" »  Filter Status : %s"), CStatus::Instance()->m_strFilterStatus);
	XL.SetCellValue(1, 7, strMsg);

	CString strSampleSize = CStatus::Instance()->m_FFT_SampleSize;
	int iSampleSize = _ttoi(strSampleSize);
	strMsg.Format(_T(" »  FFT Sample Size : %d"), iSampleSize);
	XL.SetCellValue(1, 8, strMsg);
#pragma endregion

	CRect rect;
	pMonitorView->m_pDisplayDlg->GetClientRect(&rect);

	int iReportGraphWidth = CStatus::Instance()->m_iReportGraphWidth;
	int iReportGraphHeight = CStatus::Instance()->m_iReportGraphHeight;
	CClipBoard::CopyWndImgToClipboard_AVMD((CWnd *)(pMonitorView), iReportGraphWidth, iReportGraphHeight, -rect.Width());
	XL.InsertPictureFromClipboard(1, 11);

	int nMarkCount = CStatus::Instance()->GetRepository()->m_MarkInfo[0].size();

	ReportCalcResult calcResult;
	CDrawingHelper drawingHelper;
	CDrawingHelper::tReportFFTResult reportFFTResult;
	tMarkInfo *markInfo, *PrevmarkInfo;

	PrevmarkInfo = new tMarkInfo;
	markInfo = new tMarkInfo;
	int iDataCount = CStatus::Instance()->GetRepository()->GetDataCount(0);
	int iWriteFFTSampleSize = _ttoi(CStatus::Instance()->m_FFT_SampleSize);
	int nFrequeny = (int)(CStatus::Instance()->GetDeviceFrequency());
	if (nMarkCount == 0)
	{
		strMsg = CStatus::Instance()->m_strTimeDomainUnit;
		XL.SetCellValue(6, 14, strMsg);
		XL.SetCellValue(11, 14, strMsg);
		XL.SetCellValue(16, 14, strMsg);
		
		reportFFTResult = drawingHelper.ReportFFT(_T("X"), 0);

		if (CStatus::Instance()->m_bIsThreadStop == TRUE)
		{
			int nDeleteLine = CStatus::Instance()->m_iMaxMarkInExlForm - nMarkCount + 2;
			for (int i = 0; i < nDeleteLine; i++)
			{
				XL.DeleteRow(15 + nMarkCount);
			}
			XL.SaveFileAs(strFileName);                  //  엑셀 파일 저장 
			XL.ReleaseExcel();                             // 엑셀 파일 해제
			KillProcess();
			return -1;
		}
			
		if (reportFFTResult.bIsFFTFail == FALSE)
		{
			double dblCalcMaxHz = (double)nFrequeny / (double)iWriteFFTSampleSize * (double)reportFFTResult.iMaxHz;
			strMsg.Format(_T("%.2f"), dblCalcMaxHz);
			XL.SetCellValue(5, 15, strMsg);
			strMsg.Format(_T("%.3f"), reportFFTResult.dblMaxValue);
			XL.SetCellValue(6, 15, strMsg);
		}


		reportFFTResult = drawingHelper.ReportFFT(_T("Y"), 0);
		if (CStatus::Instance()->m_bIsThreadStop == TRUE)
		{
			int nDeleteLine = CStatus::Instance()->m_iMaxMarkInExlForm - nMarkCount + 2;
			for (int i = 0; i < nDeleteLine; i++)
			{
				XL.DeleteRow(15 + nMarkCount);
			}
			XL.SaveFileAs(strFileName);                  //  엑셀 파일 저장 
			XL.ReleaseExcel();                             // 엑셀 파일 해제
			KillProcess();
			return -1;
		}
		if (reportFFTResult.bIsFFTFail == FALSE)
		{
			double dblCalcMaxHz = (double)nFrequeny / (double)iWriteFFTSampleSize * (double)reportFFTResult.iMaxHz;
			strMsg.Format(_T("%.2f"), dblCalcMaxHz);
			XL.SetCellValue(10, 15, strMsg);
			strMsg.Format(_T("%.3f"), reportFFTResult.dblMaxValue);
			XL.SetCellValue(11, 15, strMsg);
		}


		reportFFTResult = drawingHelper.ReportFFT(_T("Z"), 0);
		if (CStatus::Instance()->m_bIsThreadStop == TRUE)
		{
			int nDeleteLine = CStatus::Instance()->m_iMaxMarkInExlForm - nMarkCount + 2;
			for (int i = 0; i < nDeleteLine; i++)
			{
				XL.DeleteRow(15 + nMarkCount);
			}
			XL.SaveFileAs(strFileName);                  //  엑셀 파일 저장 
			XL.ReleaseExcel();                             // 엑셀 파일 해제
			KillProcess();
			return -1;
		}
		if (reportFFTResult.bIsFFTFail == FALSE)
		{
			double dblCalcMaxHz = (double)nFrequeny / (double)iWriteFFTSampleSize * (double)reportFFTResult.iMaxHz;
			strMsg.Format(_T("%.2f"), dblCalcMaxHz);
			XL.SetCellValue(15, 15, strMsg);
			strMsg.Format(_T("%.3f"), reportFFTResult.dblMaxValue);
			XL.SetCellValue(16, 15, strMsg);
		}


		calcResult = CalcMaxMin(0, iDataCount);

		strMsg.Format(_T("%.3f"), calcResult.dblMin_X);
		XL.SetCellValue(2, 15, strMsg);
		strMsg.Format(_T("%.3f"), calcResult.dblMax_X);
		XL.SetCellValue(3, 15, strMsg);
		strMsg.Format(_T("%.3f"), calcResult.dblMax_X - calcResult.dblMin_X);
		XL.SetCellValue(4, 15, strMsg);

		strMsg.Format(_T("%.3f"), calcResult.dblMin_Y);
		XL.SetCellValue(7, 15, strMsg);
		strMsg.Format(_T("%.3f"), calcResult.dblMax_Y);
		XL.SetCellValue(8, 15, strMsg);
		strMsg.Format(_T("%.3f"),calcResult.dblMax_Y - calcResult.dblMin_Y);
		XL.SetCellValue(9, 15, strMsg);

		strMsg.Format(_T("%.3f"), calcResult.dblMin_Z);
		XL.SetCellValue(12, 15, strMsg);
		strMsg.Format(_T("%.3f"), calcResult.dblMax_Z);
		XL.SetCellValue(13, 15, strMsg);
		strMsg.Format(_T("%.3f"), calcResult.dblMax_Z - calcResult.dblMin_Z);
		XL.SetCellValue(14, 15, strMsg);

		strMsg = _T("End");
		XL.SetCellValue(1, 16, strMsg);

		int nDeleteLine = CStatus::Instance()->m_iMaxMarkInExlForm - nMarkCount + 2;
		for (int i = 0; i < nDeleteLine; i++)
		{
			XL.DeleteRow(17 + nMarkCount);
		}
	}
	else
	{
		for (int i = 0; i < nMarkCount - 1; i++)
		{
			PrevmarkInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[0].at(i);
			markInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[0].at(i + 1);

			calcResult = CalcMaxMin(PrevmarkInfo->iIndex, markInfo->iIndex);

			if (CStatus::Instance()->m_bIsThreadStop == TRUE)
			{
				int nDeleteLine = CStatus::Instance()->m_iMaxMarkInExlForm - nMarkCount + 2;
				for (int i = 0; i < nDeleteLine; i++)
				{
					XL.DeleteRow(15 + nMarkCount);
				}
				XL.SaveFileAs(strFileName);                  //  엑셀 파일 저장 
				XL.ReleaseExcel();                             // 엑셀 파일 해제
				KillProcess();
				return -1;
			}
#pragma region FFT Report
			for (int k = 0; k < 3; k++)
			{
				strMsg = CStatus::Instance()->m_strTimeDomainUnit;
				XL.SetCellValue(6, 14, strMsg);
				XL.SetCellValue(11, 14, strMsg);
				XL.SetCellValue(16, 14, strMsg);
				if (k == 0)
				{
					reportFFTResult = drawingHelper.ReportFFT(_T("X"), PrevmarkInfo->iIndex);
					if (reportFFTResult.bIsFFTFail == FALSE)
					{
						double dblCalcMaxHz = (double)nFrequeny / (double)iWriteFFTSampleSize * (double)reportFFTResult.iMaxHz;
						strMsg.Format(_T("%.2f"), dblCalcMaxHz);
						XL.SetCellValue(5, 15 + i, strMsg);
						strMsg.Format(_T("%.3f"), reportFFTResult.dblMaxValue);
						XL.SetCellValue(6, 15 + i, strMsg);
					}
				}
				if (k == 1)
				{
					reportFFTResult = drawingHelper.ReportFFT(_T("Y"), PrevmarkInfo->iIndex);
					if (reportFFTResult.bIsFFTFail == FALSE)
					{
						double dblCalcMaxHz = (double)nFrequeny / (double)iWriteFFTSampleSize * (double)reportFFTResult.iMaxHz;
						strMsg.Format(_T("%.2f"), dblCalcMaxHz);
						XL.SetCellValue(10, 15 + i, strMsg);
						strMsg.Format(_T("%.3f"), reportFFTResult.dblMaxValue);
						XL.SetCellValue(11, 15 + i, strMsg);
					}
				}
				if (k == 2)
				{
					reportFFTResult = drawingHelper.ReportFFT(_T("Z"), PrevmarkInfo->iIndex);
					if (reportFFTResult.bIsFFTFail == FALSE)
					{
						double dblCalcMaxHz = (double)nFrequeny / (double)iWriteFFTSampleSize * (double)reportFFTResult.iMaxHz;
						strMsg.Format(_T("%.2f"), dblCalcMaxHz);
						XL.SetCellValue(15, 15 + i, strMsg);
						strMsg.Format(_T("%.3f"), reportFFTResult.dblMaxValue);
						XL.SetCellValue(16, 15 + i, strMsg);
					}
				}
	
			}
#pragma endregion

#pragma region Excel에 쓰기
			strMsg.Format(_T("%s"), PrevmarkInfo->strMark);
			XL.SetCellValue(1, 15 + i, strMsg);

			strMsg.Format(_T("%.3f"), calcResult.dblMin_X);
			XL.SetCellValue(2, 15 + i, strMsg);
			strMsg.Format(_T("%.3f"), calcResult.dblMax_X);
			XL.SetCellValue(3, 15 + i, strMsg);
			strMsg.Format(_T("%.3f"), calcResult.dblMax_X - calcResult.dblMin_X);
			XL.SetCellValue(4, 15 + i, strMsg);

			strMsg.Format(_T("%.3f"), calcResult.dblMin_Y);
			XL.SetCellValue(7, 15 + i, strMsg);
			strMsg.Format(_T("%.3f"), calcResult.dblMax_Y);
			XL.SetCellValue(8, 15 + i, strMsg);
			strMsg.Format(_T("%.3f"), calcResult.dblMax_Y - calcResult.dblMin_Y);
			XL.SetCellValue(9, 15 + i, strMsg);

			strMsg.Format(_T("%.3f"), calcResult.dblMin_Z);
			XL.SetCellValue(12, 15 + i, strMsg);
			strMsg.Format(_T("%.3f"), calcResult.dblMax_Z);
			XL.SetCellValue(13, 15 + i, strMsg);
			strMsg.Format(_T("%.3f"), calcResult.dblMax_Z - calcResult.dblMin_Z);
			XL.SetCellValue(14, 15 + i, strMsg);

			PrevmarkInfo = markInfo;
#pragma endregion
		}
#pragma region Mark End 일 때 Excel에 쓰고 남은 행 지우기
		markInfo = CStatus::Instance()->GetRepository()->m_MarkInfo[0].at(nMarkCount - 1);
		////strMsg = _T("End");
		XL.SetCellValue(1, 15 + (nMarkCount - 1), markInfo->strMark);

		int nDeleteLine = CStatus::Instance()->m_iMaxMarkInExlForm - nMarkCount + 2;
		for (int i = 0; i < nDeleteLine; i++)
		{
			XL.DeleteRow(15 + nMarkCount);
		}
#pragma endregion
	}
	XL.SaveFileAs(strFileName);                  //  엑셀 파일 저장 
	XL.ReleaseExcel();                             // 엑셀 파일 해제
	KillProcess();
	return 0;
}

UINT CAVMDMonitorView::ProcessThread(LPVOID lParam)
{
	CString *strFilePath = (CString *)lParam;

	MakeExcelReport(*strFilePath);

	CStatus::Instance()->m_bIsThreadStop = FALSE;
	return 0;
}

CAVMDMonitorView::ReportCalcResult CAVMDMonitorView::CalcMaxMin(int iStartNum, int iEndNum, int nInputType)
{
	ReportCalcResult tResult;
	tAVMDData Data;
	CString strTimeUnit = CStatus::Instance()->m_strTimeDomainUnit;
	CAVMDMonitorView *pMonitorView = CStatus::Instance()->GetAVMDView();
	
	if(CStatus::Instance()->IsFilterUse())
	{
		//첫 검색 인덱스로 초기화
		tResult.dblMax_X = -10000;	tResult.dblMin_X = 10000;
		tResult.dblMax_Y = -10000;	tResult.dblMin_Y = 10000;
		tResult.dblMax_Z = -10000;	tResult.dblMin_Z = 10000;
		for (int i = iStartNum; i < iEndNum; i++)
		{
			if (CStatus::Instance()->m_bIsThreadStop == TRUE)
				return tResult;
			double dblMax_X = pMonitorView->m_pGraphXDlg->m_drawHelper.GetFilterData(i);
			double dblMin_X = dblMax_X;
			double dblMax_Y = pMonitorView->m_pGraphYDlg->m_drawHelper.GetFilterData(i);
			double dblMin_Y = dblMax_Y;
			double dblMax_Z = pMonitorView->m_pGraphZDlg->m_drawHelper.GetFilterData(i);
			double dblMin_Z = dblMax_Z;

			if (dblMax_X >= tResult.dblMax_X)	tResult.dblMax_X = dblMax_X;
			if (dblMin_X < tResult.dblMin_X)	tResult.dblMin_X = dblMin_X;

			if (dblMax_Y >= tResult.dblMax_Y)	tResult.dblMax_Y = dblMax_Y;
			if (dblMin_Y < tResult.dblMin_Y)	tResult.dblMin_Y = dblMin_Y;

			if (dblMax_Z >= tResult.dblMax_Z)	tResult.dblMax_Z = dblMax_Z;
			if (dblMax_Z < tResult.dblMin_Z)	tResult.dblMin_Z = dblMax_Z;
		}
	}
	else
	{
		//첫 검색 인덱스로 초기화
		CStatus::Instance()->GetRepository()->GetAtData(&Data, iStartNum, nInputType);
		tResult.dblMax_X = -10000;	tResult.dblMin_X = 10000;
		tResult.dblMax_Y = -10000;	tResult.dblMin_Y = 10000;
		tResult.dblMax_Z = -10000;	tResult.dblMin_Z = 10000;
		for (int i = iStartNum; i < iEndNum; i++)
		{
			if (CStatus::Instance()->m_bIsThreadStop == TRUE)
				return tResult;
			CStatus::Instance()->GetRepository()->GetAtData(&Data, i, nInputType);

			if (Data.iXValue >= tResult.dblMax_X)	tResult.dblMax_X = Data.iXValue;
			if (Data.iXValue < tResult.dblMin_X)	tResult.dblMin_X = Data.iXValue;

			if (Data.iYValue >= tResult.dblMax_Y)	tResult.dblMax_Y = Data.iYValue;
			if (Data.iYValue < tResult.dblMin_Y)	tResult.dblMin_Y = Data.iYValue;

			if (Data.iZValue >= tResult.dblMax_Z)	tResult.dblMax_Z = Data.iZValue;
			if (Data.iZValue < tResult.dblMin_Z)	tResult.dblMin_Z = Data.iZValue;
		}
	}
	if (strTimeUnit == "g-Force")
	{
		tResult.dblMax_X /= 981.;
		tResult.dblMin_X /= 981.;
		tResult.dblMax_Y /= 981.;
		tResult.dblMin_Y /= 981.;
		tResult.dblMax_Z /= 981.;
		tResult.dblMin_Z /= 981.;
	}
	return tResult;
}

void CAVMDMonitorView::OnUpdateSettingsMarkeditor(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		pCmdUI->Enable(false);
	}
	else
	{
		pCmdUI->Enable(true);
	}
}

void CAVMDMonitorView::OnUpdateEventSettings(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		pCmdUI->Enable(false);
	}
	else
	{
		pCmdUI->Enable(true);
	}
}

void CAVMDMonitorView::OnUpdateSettingsSetstationinformation(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	/*if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		pCmdUI->Enable(false);
	}
	else
	{
		pCmdUI->Enable(true);
	}*/
}

void CAVMDMonitorView::OnUpdateViewComparedata(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CAVMDMonitorView::OnUpdateViewOverlay(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CAVMDMonitorView::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CAVMDMonitorView::OnUpdateSaveSettingsAs(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CAVMDMonitorView::OnUpdateFileLoadcomparedata(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CAVMDMonitorView::OnUpdateFilePrint(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CAVMDMonitorView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CAVMDMonitorView::KillProcess()
{
	DWORD dwSize = 250;
	HANDLE hSnapShot;
	PROCESSENTRY32 pEntry;
	BOOL bCrrent = FALSE;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	pEntry.dwSize = sizeof(pEntry);
	Process32First(hSnapShot, &pEntry);
	//
	while (1)
	{
		// 다음번 프로세스의 정보를 가져온다.
		BOOL hRes = Process32Next(hSnapShot, &pEntry);

		if (hRes == FALSE)
			break;
		CString strTemp(pEntry.szExeFile);
		if (strTemp == _T("EXCEL.EXE"))
		{
			bCrrent = TRUE;
		}
		if (bCrrent)
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEntry.th32ProcessID);
			if (hProcess)
			{
				if (TerminateProcess(hProcess, 0))
				{
					unsigned long nCode; //프로세스 종료 상태
					GetExitCodeProcess(hProcess, &nCode);
				}
				CloseHandle(hProcess);
			}
			break;
		}
	}
}



void CAVMDMonitorView::OnUpdateFileUpdate(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
	{
		pCmdUI->Enable(false);
	}
	else
	{
		pCmdUI->Enable(true);
	}
}




void CAVMDMonitorView::OnUpdateFilePartialsave(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
	{
		pCmdUI->Enable(false);
	}
	else
	{
		if(CStatus::Instance()->m_strCurLogFile.IsEmpty())
			pCmdUI->Enable(false);
		else
			pCmdUI->Enable(true);
	}
}

void CAVMDMonitorView::OnFilePartialsave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_pPartialSaveDlg->Init();
	m_pPartialSaveDlg->ShowWindow(SW_SHOW);
}

void CAVMDMonitorView::OnSelectcolorsIndicator()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		//Color color;
		colorREF = dlg.GetColor();
		//color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_IndicatorColor = colorREF;
		CStatus::Instance()->SetDefault();
	}

	CStatus::Instance()->UpdateScrn();
}

void CAVMDMonitorView::OnUpdateFileSaveas(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
	{
		pCmdUI->Enable(false);
	}
	else
	{
		if (CStatus::Instance()->m_strCurLogFile.IsEmpty())
			pCmdUI->Enable(false);
		else
			pCmdUI->Enable(true);
	}
}

void CAVMDMonitorView::SetDrawPointNum()
{
	m_pGraphRMSDlg->m_drawHelper.SetDrawPointNum();
}

LRESULT CAVMDMonitorView::OnUpdateReviewMenuReset(WPARAM wParam, LPARAM lParam)
{
	CMainFrame  *pFrame = (CMainFrame*)AfxGetMainWnd();
	CheckMenuItem(::GetMenu(pFrame->m_hWnd), ID_VIEW_COMPAREDATA, MF_UNCHECKED);
	CheckMenuItem(::GetMenu(pFrame->m_hWnd), ID_VIEW_OVERLAY, MF_UNCHECKED);

	return LRESULT();
}

int CAVMDMonitorView::MessagePumping()
{
	static MSG msg;
	if (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE) == TRUE) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	BOOL test = CStatus::Instance()->m_bIsPlayBack;
	BOOL tes = CStatus::Instance()->m_bIsPlayStop;
	if (CStatus::Instance()->m_bIsPlayBack && CStatus::Instance()->m_bIsPlayStop)
	{
		BOOL bIsFinishAllGraphDraw = CStatus::Instance()->GetReEnter();
		if (bIsFinishAllGraphDraw == TRUE)
		{
			CStatus::Instance()->PlayerStartNFinish(TRUE);
			CStatus::Instance()->m_bIsPlayStop = FALSE;
		}
	}
	return 0;
}


void CAVMDMonitorView::OnUpdateFileSavecurrentfilteredcsv(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
	{
		pCmdUI->Enable(false);
	}
	else
	{
		if (CStatus::Instance()->GetRepository()->GetDataCount(0) == 0)
			pCmdUI->Enable(false);
		else
			pCmdUI->Enable(true);
	}
}
