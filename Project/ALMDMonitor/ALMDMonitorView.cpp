
// ALMDMonitorView.cpp : implementation of the CALMDMonitorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ALMDMonitor.h"
#endif

#include "ALMDMonitorDoc.h"
#include "ALMDMonitorView.h"
#include "MainFrm.h"
#include "DispUnitsSetDlg.h"
#include "ComsetDlg.h"
#include "EnvSettingDlg.h"
#include "Status.h"
#include "XLAutomation.h"
#include "XLEzAutomation.h"
#include "ALMDMessages.h"
#include "StabilizationDlg.h"
#include "DiagnosisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CALMDMonitorView

IMPLEMENT_DYNCREATE(CALMDMonitorView, CView)

BEGIN_MESSAGE_MAP(CALMDMonitorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_COMMAND(ID_SETDISP_MODENUNITS, &CALMDMonitorView::OnSettingsSetreferenceplane)
	ON_COMMAND(ID_SETTINGS_COMPORTSETTING, &CALMDMonitorView::OnSettingsComportsetting)
	ON_COMMAND(ID_ENVSET, &CALMDMonitorView::OnEnvset)
	ON_COMMAND(ID_DIAGNOSIS, &CALMDMonitorView::OnDiagnosis)
	ON_COMMAND(ID_FILE_DATAREPORT, &CALMDMonitorView::OnFileDatareport)
	ON_COMMAND(ID_SETTINGS_STABILIZATION, &CALMDMonitorView::OnSettingsStabilization)
	ON_UPDATE_COMMAND_UI(ID_DIAGNOSIS, &CALMDMonitorView::OnUpdateDiagnosis)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENLOG, &CALMDMonitorView::OnUpdateFileOpenlog)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CALMDMonitorView::OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_DATAREPORT, &CALMDMonitorView::OnUpdateFileDatareport)
	ON_UPDATE_COMMAND_UI(ID_LOG_SAVE, &CALMDMonitorView::OnUpdateLogSave)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CALMDMonitorView construction/destruction

CALMDMonitorView::CALMDMonitorView()
	: m_pStatusDlg(NULL)
	, m_pReportDlg(NULL)
{
	// TODO: add construction code here

}

CALMDMonitorView::~CALMDMonitorView()
{
}

BOOL CALMDMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CALMDMonitorView drawing

void CALMDMonitorView::OnDraw(CDC* /*pDC*/)
{
	CALMDMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CALMDMonitorView printing

BOOL CALMDMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return FALSE;
}

void CALMDMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CALMDMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CALMDMonitorView diagnostics

#ifdef _DEBUG
void CALMDMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CALMDMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CALMDMonitorDoc* CALMDMonitorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CALMDMonitorDoc)));
	return (CALMDMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CALMDMonitorView message handlers


void CALMDMonitorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	MakeChildWnd();
	WndRepositioning();
}


int CALMDMonitorView::MakeChildWnd()
{
	CRect rectCur;
	GetClientRect(&rectCur);
	
	m_pStatusDlg = new CALMDStatusDlg;
	m_pViewDlg = new CViewDlg;
	m_pOpPanelDlg = new COpPanelDlg;
	m_pReportDlg = new CReportDlg;

	if (m_pStatusDlg->GetSafeHwnd() != 0) return -1;
	if (m_pViewDlg->GetSafeHwnd() != 0) return -1;
	if (m_pOpPanelDlg->GetSafeHwnd() != 0) return -1;

	if (!m_pStatusDlg->Create(IDD_STATUS, this))
		return -2;
	if (!m_pViewDlg->Create(IDD_VIEW, this))
		return -2;
	if (!m_pOpPanelDlg->Create(IDD_OPPANEL, this))
		return -2;
	m_pStatusDlg->m_pViewDlg = m_pViewDlg;

	if (!m_pReportDlg->Create(IDD_REPORT), NULL)return -2;


	CALMDMonitorDoc *pDoc;
	pDoc = GetDocument();
	m_pStatusDlg->m_pRepository = &(pDoc->m_Repository);	
	m_pOpPanelDlg->m_pRepository = &(pDoc->m_Repository);
	CStatus::Instance()->m_pRepository = &(pDoc->m_Repository);
	CStatus::Instance()->m_pOpPanelDlg = m_pOpPanelDlg;
	CStatus::Instance()->m_pALMDStatusDlg = m_pStatusDlg;
	CStatus::Instance()->m_pViewDlg = m_pViewDlg;
	
	return 0;
}

int CALMDMonitorView::WndRepositioning()
{
	CRect parentRect;
	CRect StatusRect;
	CRect ViewRect;
	CRect OpPanelRect;
	CRect rectStatusBar;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if (pMainFrame == NULL)return 0;
	pMainFrame->GetStatusBarRect(rectStatusBar);
	m_pReportDlg->ShowWindow(SW_HIDE);


	GetClientRect(parentRect);
	m_pStatusDlg->GetClientRect(&StatusRect);
	m_pViewDlg->GetClientRect(&ViewRect);
	m_pOpPanelDlg->GetClientRect(&OpPanelRect);

	//! Mode창
	m_pStatusDlg->MoveWindow(CRect(0, 0, StatusRect.Width(), StatusRect.Height()));


	//View창
	m_pViewDlg->MoveWindow(CRect(StatusRect.Width(), 0, parentRect.right, parentRect.bottom - OpPanelRect.Height()));

	//OPPanel창
	CRect refRect;
	m_pOpPanelDlg->MoveWindow(CRect(StatusRect.Width(), parentRect.bottom - OpPanelRect.Height(), parentRect.right, parentRect.bottom));

	return 0;
}


void CALMDMonitorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	WndRepositioning();
}



void CALMDMonitorView::OnSettingsSetreferenceplane()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CDispUnitsSetDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		m_pStatusDlg->LabelUpdate();
		m_pViewDlg->Invalidate(FALSE);
		m_pOpPanelDlg->SendMessage(WM_UPDATE_STATINFO);
	}
}


void CALMDMonitorView::OnSettingsComportsetting()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CComSetDlg dlg;
	dlg.DoModal();
}


void CALMDMonitorView::OnEnvset()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CEnvSettingDlg dlg;
	dlg.DoModal();
}



void CALMDMonitorView::OnDiagnosis()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CDiagnosisDlg dlg;
	dlg.DoModal();
}



void CALMDMonitorView::OnFileDatareport()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_pReportDlg->Initialize();
	m_pReportDlg->ShowWindow(SW_SHOW);
}


int CALMDMonitorView::MakeExcelReport(CString strFileName)
{
	CString strThisPath;

	CFileDialog SaveExelReport(TRUE);

	CString strLogFile = CStatus::Instance()->GetDataSaveFolder() + _T("\\");
	strLogFile += CStatus::Instance()->GetDataFileName();

	CString exlReportForm = CAppInfo::GetAppDir() + EXL_FORM;

	CXLEzAutomation XL(FALSE); // FALSE: 처리 과정을 화면에 보이지 않는다

	XL.OpenExcelFile(exlReportForm);             // 엑셀 파일 열기
	CString strMsg;
	strMsg.Format(_T(" »  Date : %s"), CStatus::Instance()->m_strMakeDate);
	XL.SetCellValue(1, 3, strMsg);
	strMsg.Format(_T(" »  Sesnsor S/N : %s"), CStatus::Instance()->m_strSerialNo);
	XL.SetCellValue(1, 4, strMsg);
	CString strFullPathFileName = CStatus::Instance()->strLastOpenFileName;
	strMsg.Format(_T(" »  File Name : %s"), strFullPathFileName.Mid(strFullPathFileName.ReverseFind(_T('\\')) + 1, strFullPathFileName.GetLength()));
	XL.SetCellValue(1, 5, strMsg);


	XL.SaveFileAs(strFileName);                  //  엑셀 파일 저장 
	XL.ReleaseExcel();                             // 엑셀 파일 해제
	return 0;
}


void CALMDMonitorView::OnSettingsStabilization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStabilizationDlg dlg;
	dlg.DoModal();
}


void CALMDMonitorView::OnUpdateDiagnosis(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW) pCmdUI->Enable(false);
}


void CALMDMonitorView::OnUpdateFileOpenlog(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING) pCmdUI->Enable(false);
}


void CALMDMonitorView::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING) pCmdUI->Enable(false);
}


void CALMDMonitorView::OnUpdateFileDatareport(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING) pCmdUI->Enable(false);
}


void CALMDMonitorView::OnUpdateLogSave(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING) pCmdUI->Enable(false);
}


BOOL CALMDMonitorView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	COLORREF BKColor = GetSysColor(COLOR_3DFACE);  //다이얼로그 바탕색을 얻어 온다.
	CBrush backBrush(BKColor); 
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect; pDC->GetClipBox(&rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;

//	return CView::OnEraseBkgnd(pDC);
}
