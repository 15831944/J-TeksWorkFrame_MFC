
// APMDMonitorDoc.cpp : CAPMDMonitorDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "APMDMonitor.h"
#endif

#include "APMDMonitorDoc.h"

#include <propkey.h>

#include "MainFrm.h"
#include "APMDMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAPMDMonitorDoc

IMPLEMENT_DYNCREATE(CAPMDMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CAPMDMonitorDoc, CDocument)
	ON_COMMAND(ID_LOAD_COMPAREDATA, &CAPMDMonitorDoc::OnLoadComparedata)
END_MESSAGE_MAP()


// CAPMDMonitorDoc 생성/소멸

CAPMDMonitorDoc::CAPMDMonitorDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CAPMDMonitorDoc::~CAPMDMonitorDoc()
{
}

BOOL CAPMDMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CAPMDMonitorDoc serialization

void CAPMDMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
//?		m_Repository.ReadFromLogFile(&ar);
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CAPMDMonitorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CAPMDMonitorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAPMDMonitorDoc::SetSearchContent(const CString& value)
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

// CAPMDMonitorDoc 진단

#ifdef _DEBUG
void CAPMDMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAPMDMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAPMDMonitorDoc 명령


BOOL CAPMDMonitorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_Repository.ReadFromLogFile(lpszPathName);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CAPMDMonitorView *pView = (CAPMDMonitorView *)pFrame->GetActiveView();

	pView->m_pGraphDlg->m_cNavi.SetRangeMax(m_Repository.m_DataRecords[0].nCountOfRecords, true);
	pView->m_pGraphDlg->m_cNavi.SetPos(1);
	pView->m_pGraphDlg->m_cParticlePlot.SetCurIndexNo(0);
	pView->m_pGraphDlg->PosTextOut();
	pView->m_pViewOptionDlg->UpdateMarkView();
	pView->Invalidate();

	return FALSE;
//	return TRUE;//UI가 깨져서 임시로 바꿈
}


BOOL CAPMDMonitorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//	m_Repository.TestFileWrite();

	m_Repository.ExportCSVFile(lpszPathName);
	return true;
//	return CDocument::OnSaveDocument(lpszPathName);
}


void CAPMDMonitorDoc::OnLoadComparedata()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog cmpFileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG;*.CSV;|Files(*.*)|*.*|"));

	if (cmpFileLoad.DoModal() == IDOK)
	{
		m_Repository.LoadCompareData(cmpFileLoad.GetPathName());
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CAPMDMonitorView *pView = (CAPMDMonitorView *)pFrame->GetActiveView();
		pView->m_pGraphDlg->m_cComparePlot.SetCurIndexNo(0);
		pView->Invalidate();
		pView->DemendComparedataView();
	}
}
