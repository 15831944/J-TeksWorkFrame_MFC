
// ALMDMonitorDoc.cpp : implementation of the CALMDMonitorDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ALMDMonitor.h"
#endif

#include "ALMDMonitorDoc.h"
#include "Status.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CALMDMonitorDoc

IMPLEMENT_DYNCREATE(CALMDMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CALMDMonitorDoc, CDocument)
	ON_COMMAND(ID_FILE_OPENLOG, &CALMDMonitorDoc::OnFileOpenlog)
	ON_COMMAND(ID_LOG_SAVE, &CALMDMonitorDoc::OnLogSave)
END_MESSAGE_MAP()


// CALMDMonitorDoc construction/destruction

CALMDMonitorDoc::CALMDMonitorDoc()
{
	// TODO: add one-time construction code here

}

CALMDMonitorDoc::~CALMDMonitorDoc()
{
}

BOOL CALMDMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CALMDMonitorDoc serialization

void CALMDMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CALMDMonitorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
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

// Support for Search Handlers
void CALMDMonitorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CALMDMonitorDoc::SetSearchContent(const CString& value)
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

// CALMDMonitorDoc diagnostics

#ifdef _DEBUG
void CALMDMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CALMDMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CALMDMonitorDoc commands


BOOL CALMDMonitorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_Repository.ReadFromLogFile(lpszPathName);
	tALMDRecord *pMeaInfo = m_Repository.GetMeasureInfo(m_Repository.m_iDispIndex);
	CStatus::Instance()->SetMeasureData(pMeaInfo);
	CStatus::Instance()->UpdateScrn();

	return TRUE;
}


BOOL CALMDMonitorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_Repository.ExportCSVFile(lpszPathName);
	return TRUE;
//	return CDocument::OnSaveDocument(lpszPathName);
}


void CALMDMonitorDoc::OnFileOpenlog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		m_Repository.ReadFromLogFile(FileLoad.GetPathName());
		tALMDRecord *pMeaInfo = m_Repository.GetMeasureInfo(m_Repository.m_iDispIndex);
		CStatus::Instance()->SetMeasureData(pMeaInfo);
		CStatus::Instance()->UpdateScrn();
	}
}


void CALMDMonitorDoc::OnLogSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog FileSave(FALSE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (!m_Repository.m_DataRecords.m_ALMDRecords.GetCount())
	{
		AfxMessageBox(_T("There is no Data"), MB_OK);
		return;
	}

	if (FileSave.DoModal() == IDOK)
	{
		m_Repository.SaveLogFile(FileSave.GetPathName());
	}
}
