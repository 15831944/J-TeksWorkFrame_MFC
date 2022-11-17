
// AGMDMonitorDoc.cpp : CAGMDMonitorDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "AGMDMonitor.h"
#endif

#include "AGMDMonitorDoc.h"
#include "Status.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAGMDMonitorDoc

IMPLEMENT_DYNCREATE(CAGMDMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CAGMDMonitorDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CAGMDMonitorDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CAGMDMonitorDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVEASLOGFILE, &CAGMDMonitorDoc::OnFileSaveaslogfile)
END_MESSAGE_MAP()


// CAGMDMonitorDoc 생성/소멸

CAGMDMonitorDoc::CAGMDMonitorDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CAGMDMonitorDoc::~CAGMDMonitorDoc()
{
}

BOOL CAGMDMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CAGMDMonitorDoc serialization

void CAGMDMonitorDoc::Serialize(CArchive& ar)
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
void CAGMDMonitorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CAGMDMonitorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAGMDMonitorDoc::SetSearchContent(const CString& value)
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

// CAGMDMonitorDoc 진단

#ifdef _DEBUG
void CAGMDMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAGMDMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAGMDMonitorDoc 명령


void CAGMDMonitorDoc::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		m_Repository.ReadFromLogFile(FileLoad.GetPathName());
		tAGMDRecord *pMeaInfo = m_Repository.GetMeasureInfo(m_Repository.m_iDispIndex);
		CStatus::Instance()->SetMeasureData(pMeaInfo);
		CStatus::Instance()->OnInitBrowserUpdate();
		CStatus::Instance()->UpdateScrn();
	}
}

BOOL CAGMDMonitorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return FALSE;
	//return CDocument::OnSaveDocument(lpszPathName);
}



void CAGMDMonitorDoc::OnFileSaveaslogfile()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
	if (nCount == 0)
	{
		AfxMessageBox(_T("There is no Data."));
		return;
	}
		
	CFileDialog dlg(FALSE, _T("*.LOG"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("LOG File|*.LOG|Files(*.*)|*.*|"));
	//CFileDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{	
		CStatus::Instance()->GetRepository()->NewMakeLogFile(dlg.GetPathName());
	}
}
