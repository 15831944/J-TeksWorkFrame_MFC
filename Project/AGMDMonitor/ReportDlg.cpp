// ReportDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "ReportDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "AGMDMonitorDoc.h"
#include "AGMDMonitorView.h"

// CReportDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CReportDlg, CDialogEx)

CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REPORT, pParent)
	, m_nReportData(0)
{

}

CReportDlg::~CReportDlg()
{
}

void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NREPORTDATA, m_nReportData);
	DDX_Text(pDX, IDC_NREPORTDATA, m_nReportData);
}


BEGIN_MESSAGE_MAP(CReportDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTNLOAD, &CReportDlg::OnBnClickedBtnload)
	ON_BN_CLICKED(IDC_BTN_DATAAPPLY, &CReportDlg::OnBnClickedBtnDataapply)
	ON_BN_CLICKED(IDOK, &CReportDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CReportDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CReportDlg 메시지 처리기입니다.
BOOL CReportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//Initialize();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CReportDlg::Initialize()
{
	UpdateData(TRUE);
	m_nReportData = 0;
	m_ExcelReport.SetFileName();
	m_ExcelReport.OpenExcelFile();
	m_ExcelReport.MakeHeader();
	
	UpdateData(FALSE);
}

void CReportDlg::OnBnClickedBtnload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		CStatus::Instance()->GetRepository()->ReadFromLogFile(FileLoad.GetPathName());
		tAGMDRecord *pMeaInfo = CStatus::Instance()->GetRepository()->GetMeasureInfo(CStatus::Instance()->GetRepository()->m_iDispIndex);
		CStatus::Instance()->SetMeasureData(pMeaInfo);
		CStatus::Instance()->OnInitBrowserUpdate();
		CStatus::Instance()->UpdateScrn();
	}
}

void CReportDlg::OnBnClickedBtnDataapply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
	if (nCount == 0)
	{
		AfxMessageBox(_T("There is no data"));
		return;
	}
	if (m_nReportData == 7)
	{
		AfxMessageBox(_T("Maximum is 7 in report data"));
		return;
	}
	m_nReportData++;
	m_ExcelReport.InseartNewRecord(m_nReportData);
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
	
	if (FileSave.DoModal() == IDOK)
	{
		m_ExcelReport.DataReport();
		m_ExcelReport.RemoveDummy();
		m_ExcelReport.SetFileName(FileSave.GetPathName());
		m_ExcelReport.CloseExcelFile();
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
	m_ExcelReport.CloseExcelNoSave();
	CDialogEx::OnCancel();
}
