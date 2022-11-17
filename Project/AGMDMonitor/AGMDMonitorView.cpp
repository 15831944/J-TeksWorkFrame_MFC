
// AGMDMonitorView.cpp : CAGMDMonitorView 클래스의 구현
//

#include "stdafx.h"
#include "MainFrm.h"
#include "Status.h"
#include "ComSetDlg.h"
#include "AGMDDefine.h"
#include "SetGapCriteria.h"
#include "EnvSettingDlg.h"
#include "StationInfomationDlg.h"
#include "CalibrationDlg.h"
#include "ReportDlg.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "AGMDMonitor.h"
#endif

#include "AGMDMonitorDoc.h"
#include "AGMDMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAGMDMonitorView

IMPLEMENT_DYNCREATE(CAGMDMonitorView, CView)

BEGIN_MESSAGE_MAP(CAGMDMonitorView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_MESSAGE(WM_UIUPDATE, &CAGMDMonitorView::OnDlgUpdate)
	ON_WM_SIZE()
	ON_COMMAND(ID_SETTINGS_COMPORTSETTING, &CAGMDMonitorView::OnSettingsComportsetting)
	ON_COMMAND(ID_SET_NOTCH_2, &CAGMDMonitorView::OnSetNotch2)
	ON_COMMAND(ID_SET_NOTCH_4, &CAGMDMonitorView::OnSetNotch4)
	ON_COMMAND(ID_SET_NOTCH_6, &CAGMDMonitorView::OnSetNotch6)
	ON_COMMAND(ID_SET_NOTCH_8, &CAGMDMonitorView::OnSetNotch8)
	ON_COMMAND(ID_SET_NOTCH_10, &CAGMDMonitorView::OnSetNotch10)
	ON_COMMAND(ID_SET_NOTCH_12, &CAGMDMonitorView::OnSetNotch12)
	ON_COMMAND(ID_SETTINGS_SETGAPCRITERIA, &CAGMDMonitorView::OnSettingsSetgapcriteria)
	ON_COMMAND(ID_SELECTCOLORS_TOLERANCE, &CAGMDMonitorView::OnSelectcolorsTolerance)
	ON_COMMAND(ID_SELECTCOLORS_SPECIN, &CAGMDMonitorView::OnSelectcolorsSpecin)
	ON_COMMAND(ID_SPECOUT_GAPISTOOSMALL, &CAGMDMonitorView::OnSpecoutGapistoosmall)
	ON_COMMAND(ID_SPECOUT_GAPISTOOBIG, &CAGMDMonitorView::OnSpecoutGapistoobig)
	ON_COMMAND(ID_ENV_SET, &CAGMDMonitorView::OnEnvSet)
	ON_COMMAND(ID_SETTINGS_SETSTATIONINFORMATION, &CAGMDMonitorView::OnSettingsSetstationinformation)
	ON_COMMAND(ID_SETTINGS_CALIBRATION, &CAGMDMonitorView::OnSettingsCalibration)
	ON_COMMAND(ID_FILE_SAVE_AS, &CAGMDMonitorView::OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CAGMDMonitorView::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, &CAGMDMonitorView::OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_SETGAPCRITERIA, &CAGMDMonitorView::OnUpdateSettingsSetgapcriteria)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_SETGAPOFFSETS, &CAGMDMonitorView::OnUpdateSettingsSetgapoffsets)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_CALIBRATION, &CAGMDMonitorView::OnUpdateSettingsCalibration)
	ON_UPDATE_COMMAND_UI(ID_SET_NOTCH_2, &CAGMDMonitorView::OnUpdateSetNotch2)
	ON_UPDATE_COMMAND_UI(ID_SET_NOTCH_4, &CAGMDMonitorView::OnUpdateSetNotch4)
	ON_UPDATE_COMMAND_UI(ID_SET_NOTCH_6, &CAGMDMonitorView::OnUpdateSetNotch6)
	ON_UPDATE_COMMAND_UI(ID_SET_NOTCH_8, &CAGMDMonitorView::OnUpdateSetNotch8)
	ON_UPDATE_COMMAND_UI(ID_SET_NOTCH_10, &CAGMDMonitorView::OnUpdateSetNotch10)
	ON_UPDATE_COMMAND_UI(ID_SET_NOTCH_12, &CAGMDMonitorView::OnUpdateSetNotch12)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CAGMDMonitorView::OnUpdateFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CAGMDMonitorView::OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEASLOGFILE, &CAGMDMonitorView::OnUpdateFileSaveaslogfile)
END_MESSAGE_MAP()

// CAGMDMonitorView 생성/소멸

CAGMDMonitorView::CAGMDMonitorView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CAGMDMonitorView::~CAGMDMonitorView()
{
}

BOOL CAGMDMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CAGMDMonitorView 그리기

void CAGMDMonitorView::OnDraw(CDC* /*pDC*/)
{
	CAGMDMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CAGMDMonitorView 인쇄

BOOL CAGMDMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{

	// 기본적인 준비	
	m_pReportDlg->Initialize();
	m_pReportDlg->ShowWindow(SW_SHOW);
	return FALSE;
	//return DoPreparePrinting(pInfo);
}

void CAGMDMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CAGMDMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CAGMDMonitorView 진단

#ifdef _DEBUG
void CAGMDMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CAGMDMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAGMDMonitorDoc* CAGMDMonitorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAGMDMonitorDoc)));
	return (CAGMDMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CAGMDMonitorView 메시지 처리기
void CAGMDMonitorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	MakeChildWnd();
	WndRepositioning();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CAGMDMonitorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	WndRepositioning();
}

int CAGMDMonitorView::MakeChildWnd()
{
	CRect clientRect;
	GetClientRect(&clientRect);

	m_pMonitorDlg = new CMonitorDlg;
	m_pOppanelDlg = new COPPanelDlg;
	m_pSenSorViewDlg = new CSenSorViewDlg;
	m_pSenSorStatusDlg = new CSenSorStatusDlg;
	m_pBrowserDlg = new CBrowserDlg;
	m_pViewDlg = new CViewDlg;
	m_pTempDlg = new CViewDlg;
	m_pReportDlg = new CReportDlg;
	m_pStationInfoDlg = new CStationInfomationDlg;

	if (m_pMonitorDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pOppanelDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pSenSorViewDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pSenSorStatusDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pBrowserDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pViewDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pTempDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pReportDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pStationInfoDlg->GetSafeHwnd() != 0)
		return -1;

	if (!m_pMonitorDlg->Create(IDD_MONITORDLG, this))
		return -2;
	if (!m_pOppanelDlg->Create(IDD_OPPANELDLG, this))
		return -2;
	if (!m_pSenSorStatusDlg->Create(IDD_SENSORSTATUSDLG, this))
		return -2;
	if (!m_pBrowserDlg->Create(IDD_BROWSERDLG, this))
		return -2;

	if (!m_pTempDlg->Create(IDD_VIEWDLG, this))
		return -2;
	if (!m_pViewDlg->Create(IDD_VIEWDLG, this))
		return -2;
	if (!m_pSenSorViewDlg->Create(IDD_SENSORVIEWDLG, this))
		return -2;
	if (!m_pReportDlg->Create(IDD_REPORT, this))
		return -2;

	if (!m_pStationInfoDlg->Create(IDD_STATIONINFOMATION, this))
		return -2;

	CAGMDMonitorDoc *pDoc;
	pDoc = GetDocument();
	CStatus::Instance()->SetRepository(&(pDoc->m_Repository));
	CStatus::Instance()->SetAGMDView(this);

	return 0;
}

int CAGMDMonitorView::WndRepositioning()
{
	CRect mainRect, monitorRect, oppanelRect,sensorStatusRect, browserRect, viewRect, sensorViewRect;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if (pMainFrame == NULL)
		return 0;
	m_pReportDlg->ShowWindow(SW_HIDE);
	m_pStationInfoDlg->ShowWindow(SW_HIDE);

	GetClientRect(&mainRect);
	m_pMonitorDlg->GetClientRect(&monitorRect);
	m_pOppanelDlg->GetClientRect(&oppanelRect);
	m_pSenSorStatusDlg->GetClientRect(&sensorStatusRect);
	m_pBrowserDlg->GetClientRect(&browserRect);

	m_pMonitorDlg->MoveWindow(0, 0, monitorRect.Width(), monitorRect.Height());
	m_pOppanelDlg->MoveWindow(0, monitorRect.Height(), oppanelRect.Width(), oppanelRect.Height());

	int iStartY = monitorRect.Height() + oppanelRect.Height();

	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
	{
		m_pSenSorStatusDlg->ShowWindow(SW_SHOW);
		m_pSenSorStatusDlg->MoveWindow(0, iStartY, sensorStatusRect.Width(), sensorStatusRect.Height());
		m_pBrowserDlg->ShowWindow(SW_HIDE);
		iStartY += sensorStatusRect.Height();
	}
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		m_pBrowserDlg->ShowWindow(SW_SHOW);
		m_pBrowserDlg->MoveWindow(0, iStartY, browserRect.Width(), browserRect.Height());
		m_pSenSorStatusDlg->ShowWindow(SW_HIDE);
		iStartY += browserRect.Height();
	}
	
	m_pTempDlg->MoveWindow(0, iStartY, monitorRect.Width(), mainRect.Height() - iStartY);
	m_pViewDlg->MoveWindow(monitorRect.Width(), 0, mainRect.Width() - monitorRect.Width(), mainRect.Height());

	int iTemp = (mainRect.Width() - monitorRect.Width()) / 2;
	int iSenSorViewStartX = monitorRect.Width() + iTemp - (mainRect.Height() / 2);
	if (iSenSorViewStartX <= monitorRect.Width())
		iSenSorViewStartX = monitorRect.Width();
	m_pSenSorViewDlg->MoveWindow(iSenSorViewStartX, 0, mainRect.Height(), mainRect.Height());
	
	CStatus::Instance()->UpdateScrn();
	Invalidate(FALSE);
	return 0;
}

void CAGMDMonitorView::OnSettingsComportsetting()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CComSetDlg dlg;
	dlg.DoModal();
}
LRESULT CAGMDMonitorView::OnDlgUpdate(WPARAM wParam, LPARAM lParam)
{
	WndRepositioning();
	return 0;
}

void CAGMDMonitorView::OnSettingsSetgapcriteria()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
		if (nCount == 0)
		{
			AfxMessageBox(_T("There is no Data."));
			return;
		}
	}
	
	CSetGapCriteria dlg;
	dlg.DoModal();
}

#pragma region MenuNotch
void CAGMDMonitorView::OnSetNotch2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::TWO_CLOCK);
	if(CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetDefault();
	CStatus::Instance()->UpdateScrn();
}

void CAGMDMonitorView::OnSetNotch4()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::FOUR_CLOCK);
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetDefault();
	CStatus::Instance()->UpdateScrn();
}

void CAGMDMonitorView::OnSetNotch6()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::SIX_CLOCK);
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetDefault();
	CStatus::Instance()->UpdateScrn();
}

void CAGMDMonitorView::OnSetNotch8()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::EIGHT_CLOCK);
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetDefault();
	CStatus::Instance()->UpdateScrn();
}

void CAGMDMonitorView::OnSetNotch10()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::TEN_CLOCK);
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetDefault();
	CStatus::Instance()->UpdateScrn();
}

void CAGMDMonitorView::OnSetNotch12()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStatus::Instance()->SetNotchMode(CStatus::tNotchMode::TWELVE_CLOCK);
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetDefault();
	CStatus::Instance()->UpdateScrn();
}
#pragma endregion

#pragma region Menu Color
void CAGMDMonitorView::OnSelectcolorsTolerance()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		Color color;
		colorREF = dlg.GetColor();
		color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_ToleranceColor = color;
		//if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetDefault();
	}
	CStatus::Instance()->UpdateScrn();
}

void CAGMDMonitorView::OnSelectcolorsSpecin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		Color color;
		colorREF = dlg.GetColor();
		color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_SpecInColor = color;
		//if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetDefault();
	}
	CStatus::Instance()->UpdateScrn();
}

void CAGMDMonitorView::OnSpecoutGapistoosmall()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		Color color;
		colorREF = dlg.GetColor();
		color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_SpecSmallOutColor = color;
		//if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetDefault();
	}
	CStatus::Instance()->UpdateScrn();
}

void CAGMDMonitorView::OnSpecoutGapistoobig()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COLORREF colorREF;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		Color color;
		colorREF = dlg.GetColor();
		color.SetFromCOLORREF(colorREF);
		CStatus::Instance()->m_SpecBigOutColor = color;
		//if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetDefault();
	}
	CStatus::Instance()->UpdateScrn();
}

#pragma endregion

void CAGMDMonitorView::OnEnvSet()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CEnvSettingDlg dlg;
	dlg.DoModal();
}

void CAGMDMonitorView::OnSettingsSetstationinformation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//CStationInfomationDlg dlg;
	//dlg.DoModal();
	m_pStationInfoDlg->ShowWindow(SW_SHOW);
	m_pStationInfoDlg->Initialize();
}

void CAGMDMonitorView::OnSettingsCalibration()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CCalibrationDlg dlg;
	dlg.DoModal();
}

void CAGMDMonitorView::OnFileSaveAs()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.	
}

void CAGMDMonitorView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateFilePrint(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateSettingsSetgapcriteria(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateSettingsSetgapoffsets(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateSettingsCalibration(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateSetNotch2(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateSetNotch4(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateSetNotch6(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateSetNotch8(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateSetNotch10(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateSetNotch12(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnUpdateFileSaveaslogfile(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING) pCmdUI->Enable(false);
}

void CAGMDMonitorView::OnFileSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
	if (nCount == 0)
	{
		AfxMessageBox(_T("There is no data"));
		return;
	}
	CFileDialog FileSave(FALSE,NULL, NULL, OFN_FILEMUSTEXIST, _T("EXCEL File|*.*;|Files(*.*)|*.*|"));

	if (FileSave.DoModal() == IDOK)
	{
		CString strFilePath = FileSave.GetFolderPath();
		CString strFileName = FileSave.GetFileName();
		CString strFullFileName = strFilePath + _T("\\") + strFileName;
		CStatus::Instance()->GetRepository()->ExportCSVFile(strFullFileName);
	}

}

