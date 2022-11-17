// ReportDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "ReportDlg.h"
#include "afxdialogex.h"
#include "ALMDMonitorDoc.h"
#include "ALMDMonitorView.h"
#include "MainFrm.h"
#include "Status.h"


// CReportDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CReportDlg, CDialogEx)

CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REPORT, pParent)
	, m_nReportData(0)
	, m_strExcelFile(_T(""))
	, m_bIsSelectLog(FALSE)
{

}

CReportDlg::~CReportDlg()
{
}

void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NREPORTDATA, m_nReportData);
}


BEGIN_MESSAGE_MAP(CReportDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTNLOAD, &CReportDlg::OnBnClickedBtnload)
	ON_BN_CLICKED(IDC_BTN_DATAAPPLY, &CReportDlg::OnBnClickedBtnDataapply)
	ON_BN_CLICKED(IDOK, &CReportDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CReportDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CReportDlg 메시지 처리기입니다.


void CReportDlg::OnBnClickedBtnload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		m_bIsSelectLog = TRUE;
		CString strFileName = FileLoad.GetPathName();
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CALMDMonitorDoc *pDoc = (CALMDMonitorDoc *)pFrame->GetActiveDocument();
		pDoc->OnOpenDocument(strFileName);
	}
}


void CReportDlg::OnBnClickedBtnDataapply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CALMDMonitorView *pView = (CALMDMonitorView *)pFrame->GetActiveView();
	if (m_nReportData == 7)
	{
		AfxMessageBox(_T("Maximum is 7 in report data"));
		return;
	}
	if (!m_bIsSelectLog)
	{
		AfxMessageBox(_T("Please select the LOG file"));
		return;
	}
	pView->m_ExcelReport.InseartNewRecord(++m_nReportData);
	UpdateData(FALSE);
}


BOOL CReportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CReportDlg::Initialize(CString strExcelFile)
{
	m_strExcelFile = strExcelFile;
	m_nReportData = 0;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CALMDMonitorView *pView = (CALMDMonitorView *)pFrame->GetActiveView();
	pView->m_ExcelReport.SetFileName(strExcelFile);
	pView->m_ExcelReport.OpenExcelFile();
	pView->m_ExcelReport.MakeHeader();
	m_bIsSelectLog = FALSE;
	UpdateData(FALSE);
}


void CReportDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_nReportData)
	{
		AfxMessageBox(_T("There is no data"));
		return;
	}
	CFileDialog FileSave(FALSE, _T("*.XLSX"), NULL, OFN_FILEMUSTEXIST, _T("EXCEL File|*.XLSX;|Files(*.*)|*.*|"));
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CALMDMonitorView *pView = (CALMDMonitorView *)pFrame->GetActiveView();
	if (FileSave.DoModal() == IDOK)
	{
		pView->m_ExcelReport.MakeChart();
		pView->m_ExcelReport.RemoveDummy();
		pView->m_ExcelReport.SetFileName(FileSave.GetPathName());
		pView->m_ExcelReport.CloseExcelFile();
	}
	else
	{
		return;
	}
	CDialogEx::OnOK();
}


void CReportDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CALMDMonitorView *pView = (CALMDMonitorView *)pFrame->GetActiveView();
	pView->m_ExcelReport.CloseExcelNoSave();
	CDialogEx::OnCancel();
}
