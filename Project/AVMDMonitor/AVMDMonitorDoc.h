
// AVMDMonitorDoc.h : CAVMDMonitorDoc 클래스의 인터페이스
//

#include "Repository.h"
#pragma once

class CAVMDMonitorDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CAVMDMonitorDoc();
	DECLARE_DYNCREATE(CAVMDMonitorDoc)

// 특성입니다.
public:
	
// 작업입니다.
public:
// 재정의입니다.
	
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CAVMDMonitorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	CRepository m_Repository;
	afx_msg void OnFileOpen();
	afx_msg void OnFileLoadcomparedata();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileUpdate();
	afx_msg void OnFileSaveas();
	afx_msg void OnSaveSettingsAs();
	afx_msg void OnFileLoadsettingsfrom();
	afx_msg void OnFileSavecurrentfilteredcsv();
	static UINT ProcessThread(LPVOID lParam);
	static UINT SaveAsThread(LPVOID lParam);
};
