
// AVMDMonitorDoc.cpp : CAVMDMonitorDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "AVMDMonitor.h"
#include "Status.h"
#endif

#include "AVMDMonitorDoc.h"
#include "MainFrm.h"
#include <UniUtility.h>
#include <propkey.h>
#include "AVMDMonitorView.h"
#include "ProgressBarDlg.h"
#include "Status.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAVMDMonitorDoc

IMPLEMENT_DYNCREATE(CAVMDMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CAVMDMonitorDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CAVMDMonitorDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_LOADCOMPAREDATA, &CAVMDMonitorDoc::OnFileLoadcomparedata)
	ON_COMMAND(ID_FILE_UPDATE, &CAVMDMonitorDoc::OnFileUpdate)
	ON_COMMAND(ID_FILE_SAVEAS, &CAVMDMonitorDoc::OnFileSaveas)
	ON_COMMAND(ID_SAVE_SETTINGS_AS, &CAVMDMonitorDoc::OnSaveSettingsAs)
	ON_COMMAND(ID_FILE_LOADSETTINGSFROM, &CAVMDMonitorDoc::OnFileLoadsettingsfrom)
	ON_COMMAND(ID_FILE_SAVECURRENTFILTEREDCSV, &CAVMDMonitorDoc::OnFileSavecurrentfilteredcsv)
END_MESSAGE_MAP()


// CAVMDMonitorDoc 생성/소멸

CAVMDMonitorDoc::CAVMDMonitorDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CAVMDMonitorDoc::~CAVMDMonitorDoc()
{
}

BOOL CAVMDMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CAVMDMonitorDoc serialization

void CAVMDMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CAVMDMonitorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CAVMDMonitorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAVMDMonitorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAVMDMonitorDoc 진단

#ifdef _DEBUG
void CAVMDMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAVMDMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAVMDMonitorDoc 명령

void CAVMDMonitorDoc::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		CStatus::Instance()->ReviewDataClear(TRUE, FALSE);
		m_Repository.ReadFromLogFile(FileLoad.GetPathName(), 0);
		//CStatus::Instance()->SetFilterType(FALSE, FALSE, FALSE);
		//CStatus::Instance()->m_strFilterStatus = _T("disable");
		CStatus::Instance()->SetReviewDragReset(0);
		CStatus::Instance()->UpdateReviewComboBox(0);

		CString strFileName = FileLoad.GetFileName();
		CStatus::Instance()->m_strCurLogFile = strFileName;
		this->SetTitle(strFileName);
		CStatus::Instance()->SetMarkClickEvent(FALSE, 0);

		if (CStatus::Instance()->IsFilterUse())
		{
			CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::FILTER);
			CStatus::Instance()->m_nProgressBarCount = 0;
			CProgressBarDlg *pProgressDlg = new CProgressBarDlg;
			CStatus::Instance()->ReviewDataClear(TRUE, TRUE);
			CStatus::Instance()->ReviewDataFiltering();
			pProgressDlg->DoModal();
		}
		CStatus::Instance()->UpdateScrn();
	}
}

void CAVMDMonitorDoc::OnFileLoadcomparedata()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		CStatus::Instance()->ReviewDataClear(FALSE, TRUE);
		m_Repository.ReadFromLogFile(FileLoad.GetPathName(), 1);
		
		//CStatus::Instance()->SetFilterType(FALSE, FALSE, FALSE);
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
		//CStatus::Instance()->UpdateScrn();
		CMainFrame  *pFrame = (CMainFrame*)AfxGetMainWnd();
		CheckMenuItem(::GetMenu(pFrame->m_hWnd), ID_VIEW_COMPAREDATA, MF_CHECKED);
		CStatus::Instance()->SetCompareCondition(TRUE);
		CStatus::Instance()->GraphUpdate();

		CStatus::Instance()->UpdateScrn();
	}
}


BOOL CAVMDMonitorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDocument::OnSaveDocument(lpszPathName);
}


void CAVMDMonitorDoc::OnFileUpdate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_Repository.ReSaveTagFile(0);
}




void CAVMDMonitorDoc::OnFileSaveas()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(FALSE);

	if (dlg.DoModal()==IDOK)
	{
		CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::SAVE_AS);
		CStatus::Instance()->m_nProgressBarCount = 0;
		CProgressBarDlg *pProgressDlg = new CProgressBarDlg;
		CString strFilePath = dlg.GetPathName();
		AfxBeginThread(SaveAsThread, &strFilePath);
		pProgressDlg->DoModal();
		//m_Repository.ExportDataFile(&strFilePath);
	}
}

void CAVMDMonitorDoc::OnSaveSettingsAs()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(FALSE, _T("*.CFG"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CFG File|*.CFG|Files(*.*)|*.*|"));

	if (dlg.DoModal() == IDOK)
	{
		CStatus::Instance()->ExportSetting(dlg.GetPathName());
	}
}


void CAVMDMonitorDoc::OnFileLoadsettingsfrom()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, _T("*.CFG"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("CFG File|*.CFG|Files(*.*)|*.*|"));

	if (dlg.DoModal() == IDOK)
	{
		CStatus::Instance()->GetSettingFrom(dlg.GetPathName());
	}

}


void CAVMDMonitorDoc::OnFileSavecurrentfilteredcsv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(FALSE, _T("*.CSV"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CSV File|*.CSV|Files(*.*)|*.*|"));
	{
		if (dlg.DoModal() == IDOK)
		{
			CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::EXPORT_EXCEL);
			CStatus::Instance()->m_nProgressBarCount = 0;
			CProgressBarDlg *pProgressDlg = new CProgressBarDlg;
			CString strFilePath = dlg.GetPathName();
			AfxBeginThread(ProcessThread, &strFilePath);
			pProgressDlg->DoModal();
		}
	}
}

UINT CAVMDMonitorDoc::ProcessThread(LPVOID lParam)
{
	CString *strFileName = (CString*)lParam;
	if (CStatus::Instance()->IsFilterUse())
	{
		CStatus::Instance()->GetRepository()->ExportFilteredCSVFile(*strFileName, 0);
	}
	else
	{
		CStatus::Instance()->GetRepository()->ExportCSVFile(*strFileName, -1);
	}
	CStatus::Instance()->m_bIsThreadStop = FALSE;
	return 0;
}

UINT CAVMDMonitorDoc::SaveAsThread(LPVOID lParam)
{
	CString *strFilePath = (CString *)lParam;
	CStatus::Instance()->GetRepository()->ExportDataFile(*strFilePath);
	CStatus::Instance()->m_bIsThreadStop = FALSE;
	return 0;
}