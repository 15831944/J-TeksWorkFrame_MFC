
// APMDMonitorView.cpp : CAPMDMonitorView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "APMDMonitor.h"
#endif

#include "APMDMonitorDoc.h"
#include "APMDMonitorView.h"
#include "Status.h"
#include "XLEzAutomation.h"
#include "UniUtility.h"
#include "MainFrm.h"
#include "StatusCompareDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAPMDMonitorView

IMPLEMENT_DYNCREATE(CAPMDMonitorView, CView)

BEGIN_MESSAGE_MAP(CAPMDMonitorView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_COMSET, &CAPMDMonitorView::OnComset)
	ON_WM_SIZE()
	ON_COMMAND(ID_WARNSET, &CAPMDMonitorView::OnWarnset)
	ON_COMMAND(ID_MARKEDITOR, &CAPMDMonitorView::OnMarkeditor)
	ON_COMMAND(ID_ENVSET, &CAPMDMonitorView::OnEnvset)
	ON_COMMAND(ID_VIEW_COMPAREDATA, &CAPMDMonitorView::OnViewComparedata)
	ON_COMMAND(ID_OVERLAY, &CAPMDMonitorView::OnOverlay)
	ON_UPDATE_COMMAND_UI(ID_LOAD_COMPAREDATA, &CAPMDMonitorView::OnUpdateLoadComparedata)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COMPAREDATA, &CAPMDMonitorView::OnUpdateViewComparedata)
	ON_UPDATE_COMMAND_UI(ID_OVERLAY, &CAPMDMonitorView::OnUpdateOverlay)
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CAPMDMonitorView 생성/소멸

CAPMDMonitorView::CAPMDMonitorView()
	: m_pViewOptionDlg(NULL),m_pModeDlg(NULL), m_pStatusDlg(NULL), m_pGraphDlg(NULL)
	, m_pStatusCompareDlg(NULL)
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CAPMDMonitorView::~CAPMDMonitorView()
{
}

BOOL CAPMDMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CAPMDMonitorView 그리기

void CAPMDMonitorView::OnDraw(CDC* /*pDC*/)
{
	CAPMDMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CAPMDMonitorView 인쇄

BOOL CAPMDMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
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
		BeginWaitCursor();
		MakeExcelReport(exlReport.GetPathName());
		EndWaitCursor();
	}
	return FALSE;
	return DoPreparePrinting(pInfo);
}

void CAPMDMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CAPMDMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CAPMDMonitorView 진단

#ifdef _DEBUG
void CAPMDMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CAPMDMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAPMDMonitorDoc* CAPMDMonitorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAPMDMonitorDoc)));
	return (CAPMDMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CAPMDMonitorView 메시지 처리기


void CAPMDMonitorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	MakeChildWnd();
	WndRepositioning();
}


int CAPMDMonitorView::MakeChildWnd()
{
	CRect curRect;
	GetClientRect(&curRect);

	m_pModeDlg = new CModeDlg;
	m_pStatusDlg = new CStatusDlg;
	m_pStatusCompareDlg = new CStatusCompareDlg;
	m_pGraphDlg = new CGraphDlg;
	m_pViewOptionDlg = new CViewOptionDlg;
	
	if (m_pViewOptionDlg->GetSafeHwnd() != 0) return -1;
	if (m_pModeDlg->GetSafeHwnd() != 0) return -1;
	if (m_pStatusDlg->GetSafeHwnd() != 0) return -1;
	if (m_pStatusCompareDlg->GetSafeHwnd() != 0) return -1;
	if (m_pGraphDlg->GetSafeHwnd() != 0) return -1;

	if (!m_pModeDlg->Create(IDD_MODE, this))
		return -2;

	if (!m_pStatusDlg->Create(IDD_STATUS, this))
		return -2;

	if (!m_pStatusCompareDlg->Create(IDD_STATUS_COMPARE, this))
		return -2;

	if (!m_pGraphDlg->Create(IDD_GRAPH, this))
		return -2;

	if (!m_pViewOptionDlg->Create(IDD_VIEW_OPTION, this))
		return -2;
	m_pModeDlg->SetStatusDlg(m_pStatusDlg);
	CAPMDMonitorDoc *pDoc;
	pDoc = GetDocument();
	m_pModeDlg->SetRepository(&(pDoc->m_Repository));
	m_pModeDlg->SetGraphDlg(m_pGraphDlg);
	m_pModeDlg->SetViewOptionDlg(m_pViewOptionDlg);
	m_pViewOptionDlg->SetGraphDlg(m_pGraphDlg);
	m_pViewOptionDlg->SetRepository(&(pDoc->m_Repository));
	m_pGraphDlg->SetViewOptionDlg(m_pViewOptionDlg);
	m_pGraphDlg->m_cParticlePlot.SetModeDlg(m_pModeDlg);
	m_pGraphDlg->m_cComparePlot.SetModeDlg(m_pModeDlg);
	m_pStatusDlg->m_pStatusCompareDlg = m_pStatusCompareDlg;
	return 0;
}


void CAPMDMonitorView::OnComset()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CComSetDlg comsetDlg;
	comsetDlg.DoModal();
}


void CAPMDMonitorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	WndRepositioning();
}


void CAPMDMonitorView::WndRepositioning()
{
	if (!m_pModeDlg)return;
	CRect parentRect;
	CRect ModeRect;
	CRect StatusRect;
	CRect GraphRect;
	CRect ViewOptionRect;

	GetClientRect(parentRect);
	//! Mode창
	m_pModeDlg->GetClientRect(&ModeRect);
	m_pModeDlg->MoveWindow(CRect(0, 0, ModeRect.Width(), ModeRect.Height()));

	if (CStatus::Instance()->IsCompareMode()) 
	{
		//! StatusCompare창
		m_pStatusCompareDlg->GetClientRect(&StatusRect);
		m_pStatusCompareDlg->MoveWindow(CRect(0, ModeRect.Height(), ModeRect.Width(), parentRect.bottom));
		m_pStatusCompareDlg->ShowWindow(true);
		m_pStatusDlg->ShowWindow(false);
	}
	else
	{
		//! Status창
		m_pStatusDlg->GetClientRect(&StatusRect);
		m_pStatusDlg->MoveWindow(CRect(0, ModeRect.Height(), ModeRect.Width(), parentRect.bottom));
		m_pStatusCompareDlg->ShowWindow(false);
		m_pStatusDlg->ShowWindow(true);
	}

	//ViewOption창
	CRect refRect;
	m_pViewOptionDlg->GetClientRect(&ViewOptionRect);
	if (ModeRect.Width()>StatusRect.Width())
		m_pViewOptionDlg->MoveWindow(CRect(ModeRect.Width(), parentRect.bottom-ViewOptionRect.Height(), parentRect.right, parentRect.bottom));
	else
		m_pViewOptionDlg->MoveWindow(CRect(StatusRect.Width(), parentRect.bottom - ViewOptionRect.Height(), parentRect.right, parentRect.bottom));

	////! Graph창
	m_pGraphDlg->GetClientRect(&GraphRect);
	if(ModeRect.Width()>StatusRect.Width())
		m_pGraphDlg->MoveWindow(CRect(ModeRect.Width(), 0, parentRect.right, parentRect.Height()-ViewOptionRect.Height()));
	else
		m_pGraphDlg->MoveWindow(CRect(StatusRect.Width(), 0, parentRect.right, parentRect.Height() - ViewOptionRect.Height()));

}


void CAPMDMonitorView::OnWarnset()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWarningSetDlg WarningSetDlg;
	WarningSetDlg.DoModal();
}


void CAPMDMonitorView::OnMarkeditor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMarkEditDlg EditDlg;
	EditDlg.SetViewOptionDlg(m_pViewOptionDlg);
	EditDlg.DoModal();
}


void CAPMDMonitorView::OnEnvset()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CEnvSettingDlg EnvSettingDlg;
	EnvSettingDlg.DoModal();
}


int CAPMDMonitorView::MakeExcelReport(CString strFileName)
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

	CAPMDMonitorDoc *pDoc;
	pDoc = GetDocument();
	CRepository *pRepository= &(pDoc->m_Repository);
	strMsg.Format(_T(" »  Measurement Time : %d sec"), pRepository->m_DataRecords[0].nCountOfRecords);
	XL.SetCellValue(1, 6, strMsg); 
	int nCount01, nCount03, nCount05;
	if (pRepository->CalcParticleCount(nCount01, nCount03, nCount05) == 0)
		strMsg.Format(_T(" »  Particle Count(Total):%d"), nCount01+nCount03+nCount05);
	else
		strMsg=_T(" »  Particle Count(Total):---");
	XL.SetCellValue(1, 7, strMsg); 
	CClipBoard::CopyWndImgToClipboard(&(m_pGraphDlg->m_cParticlePlot));
	XL.InsertPictureFromClipboard(1,10);

//!Write Mark Info. 
	int nMaxMark = CStatus::Instance()->m_iMaxMarkInExlForm;

	POSITION pos;
	CString strMark;
	tMarkSectionInfo MarkSectionInfo;

	pRepository->ScanMarkFirst(pos);
	int countMark = 0;
	bool bIsEnd = false;
	do
	{
		if (pos == NULL)break;
		if (pRepository->ScanMarkSection(pos, MarkSectionInfo)==-1)
		{
			bIsEnd = true;
		}
		XL.SetCellValue(1, 14 + countMark, MarkSectionInfo.strStartMark);
		strMsg.Format(_T("%d"), MarkSectionInfo.particle01.iMin);
		XL.SetCellValue(5, 14 + countMark, strMsg);
		strMsg.Format(_T("%d"), MarkSectionInfo.particle01.iMax);
		XL.SetCellValue(6, 14 + countMark, strMsg);
		strMsg.Format(_T("%d"), MarkSectionInfo.particle01.iSum);
		XL.SetCellValue(7, 14 + countMark, strMsg);
		strMsg.Format(_T("%d"), MarkSectionInfo.particle03.iMin);
		XL.SetCellValue(8, 14 + countMark, strMsg);
		strMsg.Format(_T("%d"), MarkSectionInfo.particle03.iMax);
		XL.SetCellValue(9, 14 + countMark, strMsg);
		strMsg.Format(_T("%d"), MarkSectionInfo.particle03.iSum);
		XL.SetCellValue(10, 14 + countMark, strMsg);
		strMsg.Format(_T("%d"), MarkSectionInfo.particle05.iMin);
		XL.SetCellValue(11, 14 + countMark, strMsg);
		strMsg.Format(_T("%d"), MarkSectionInfo.particle05.iMax);
		XL.SetCellValue(12, 14 + countMark, strMsg);
		strMsg.Format(_T("%d"), MarkSectionInfo.particle05.iSum);
		XL.SetCellValue(13, 14 + countMark, strMsg);
		strMsg.Format(_T("%d"), MarkSectionInfo.Humi.iMin);
		XL.SetCellValue(14, 14 + countMark, strMsg);
		strMsg.Format(_T("%d"), MarkSectionInfo.Humi.iMax);
		XL.SetCellValue(15, 14 + countMark, strMsg);
		strMsg.Format(_T("%.2f"), (float)MarkSectionInfo.Humi.dblAvg);
		XL.SetCellValue(16, 14 + countMark, strMsg);
		countMark++;
		if (bIsEnd)break;
	} while (true);

	int toDeleteLine = CStatus::Instance()->m_iMaxMarkInExlForm - countMark + 2;
	for (int i = 0; i < toDeleteLine; i++)
	{
		XL.DeleteRow(14+countMark);
	}

	XL.SaveFileAs(strFileName);                  //  엑셀 파일 저장 
	XL.ReleaseExcel();                             // 엑셀 파일 해제
	return 0;
}


void CAPMDMonitorView::OnViewComparedata()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMenu *hMenu = pFrame->GetMenu();

	if (hMenu->GetMenuState(ID_VIEW_COMPAREDATA, MF_BYCOMMAND) != MF_CHECKED) {
		hMenu->CheckMenuItem(ID_VIEW_COMPAREDATA, MF_CHECKED);
		CStatus::Instance()->SetCompareMode(true);
	}
	else {
		hMenu->CheckMenuItem(ID_VIEW_COMPAREDATA, MF_UNCHECKED);
		CStatus::Instance()->SetCompareMode(false);
	}

	ApplyCompareMode();
}

void CAPMDMonitorView::DemendComparedataView()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMenu *hMenu = pFrame->GetMenu();

	if (hMenu->GetMenuState(ID_VIEW_COMPAREDATA, MF_BYCOMMAND) != MF_CHECKED) {
		hMenu->CheckMenuItem(ID_VIEW_COMPAREDATA, MF_CHECKED);
		CStatus::Instance()->SetCompareMode(true);
	}

	ApplyCompareMode();
}

void CAPMDMonitorView::OnOverlay()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMenu *hMenu = pFrame->GetMenu();

	if (hMenu->GetMenuState(ID_OVERLAY, MF_BYCOMMAND) != MF_CHECKED) {
		hMenu->CheckMenuItem(ID_OVERLAY, MF_CHECKED);
		CStatus::Instance()->SetOverlayMode(true);
	}
	else {
		hMenu->CheckMenuItem(ID_OVERLAY, MF_UNCHECKED);
		CStatus::Instance()->SetOverlayMode(false);
	}
	ApplyCompareMode();
}


void CAPMDMonitorView::ApplyCompareMode()
{
	if(CStatus::Instance()->IsCompareMode())
	{
		if (CStatus::Instance()->IsOverlayMode())
		{
			m_pGraphDlg->SetDualMode(false);
		}
		else
		{
			m_pGraphDlg->SetDualMode(true);
		}
	}
	else
	{
		m_pGraphDlg->SetDualMode(false);
	}
	WndRepositioning();
}


void CAPMDMonitorView::OnUpdateLoadComparedata(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
		if (CStatus::Instance()->GetProgramMode()==CStatus::PM_REVIEW)
		{
			pCmdUI->Enable(true);
		}
		else
		{
			pCmdUI->Enable(false);
		}
}


void CAPMDMonitorView::OnUpdateViewComparedata(CCmdUI *pCmdUI)
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


void CAPMDMonitorView::OnUpdateOverlay(CCmdUI *pCmdUI)
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

