
// XFMSView.h : CXNPIView 클래스의 인터페이스
//

#pragma once
#include "atltypes.h"
#include "ModbusManager.h"

class CCamDlg;
class COpPanelDlg;
class CTeachingDlg;
class CStatusPanelDlg;
class CGraphDlg;
class CHistoLevellingDlg;
class CToolPanelDlg;
class CSnapshotDlg;

class CXNPIView : public CView
{
protected: // serialization에서만 만들어집니다.
	CXNPIView();
	DECLARE_DYNCREATE(CXNPIView)

	// 특성입니다.
public:
	CXNPIDoc* GetDocument() const;

	// 작업입니다.
public:

	// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 구현입니다.
public:
	virtual ~CXNPIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	int MakeChildWnd();
	CCamDlg *m_pCamDlg[4];
	COpPanelDlg *m_pOpPanelDlg;
	CTeachingDlg *m_pTeachingDlg;
	int WndRepositioning();
	CRect CameraDlgPos(int nPos, CRect rectMain, int widthCam, int heightCam, int xOffset = 0);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetOPMode(int OPMode);
	BOOL IsTeachingMode();
	CRect m_rectCamFull;
	int m_nCountOfCam;
	CStatusPanelDlg *m_pStatusPanelDlg;
	void DoTrial();
	int LoadDefaultJob();
	int CloseAll();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//CGraphDlg *m_pGraphDlg;
	CHistoLevellingDlg *m_pHistoLevellingDlg;
	afx_msg void OnToolHistowindowlevel();
	afx_msg void OnUpdateToolHistowindowlevel(CCmdUI *pCmdUI);
	CToolPanelDlg *m_pToolPanelDlg;

	//kiy ------------------------------------------------------
	CSnapshotDlg* m_pSnapshotDlg;
	CStdioFile m_infoFileManager;
	CModbusManager* m_mdManager;

	int m_nCurrentRetryCnt;
	void SocketOnReceive(int transactionID, byte functionCode, int rcvData);

	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	HANDLE m_hReceiveDataFromPLCEvent;
	int m_nPlcSendcmdNum;
	
	BOOL m_bStartCmdFlag;
	BOOL m_bEndCmdFlag;
	BOOL m_bTriggerCmdFlag;

	clock_t m_tRStartCMDStart, m_tRStartCMDEnd;
	clock_t m_tREndCMDStart, m_tREndCMDEnd;
	clock_t m_tRTriggerCMDStart, m_tRTriggerCMDEnd;

	int m_nPrevTriggerNum;
	SYSTEMTIME m_tCur_time; //190327 - 현재시간 밀리초 단위로 구하기(로그 접두사에 붙일 용도)
	int m_nInvalidInterval_ms; //190327 - 기존 CMD_INVAILD_INTERVAL_MS 대용 - config파일에서 읽어오게 변경했기 때문이다.
	//----------------------------------------------------------
};

#ifndef _DEBUG  // SCInspectorView.cpp의 디버그 버전
inline CXNPIDoc* CXNPIView::GetDocument() const
{
	return reinterpret_cast<CXNPIDoc*>(m_pDocument);
}
#endif

