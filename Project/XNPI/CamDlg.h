#pragma once
#include <Matrox.h>
#include "afxwin.h"
#include <UniUtility.h>
#include "atltypes.h"

class CCompo;
class CCompoTracker;
class CSCFidAlgo;
class CLineEquation;
class CSCResult;


// CCamDlg 대화 상자입니다.

class CCamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCamDlg)

public:
	CCamDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCamDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	CMilBuffer m_milView;
	virtual BOOL OnInitDialog();
	CMilBuffer *m_pMilCamView; //보이기 위한 버퍼로 Select된것임  그래프작업등을 하면 느려서 CamDraw를 카피해서 보이는 용도로만 사용해야됨 
	CMilBuffer *m_pMilCamDraw; //Graph작업등을 하기 위함 MilCamDisp 랑 같음
	CMilBuffer *m_pMilCamProc;
	CMilBuffer *m_pMilCamRec;

	CMilDisplay *m_pMilDisp;
	CMilDisplay *m_pMilDispDefault;
	CString m_strCamName;
	static UINT ThreadSendRemote(LPVOID pParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	// 하나라도 걸리는것이 있으면 양수 걸리는것이 하나도 없으면 음수
	int CamViewHitTest(CPoint Point, CCompo * &pFindedCompo);
	CPtrList * GetCamList();
	BOOL IsTeachingMode();
	void AddCompoForView(CPoint Point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void DrawAllTrack(CDC * pDC);
	void DrawAllTrack();
	int ClearOverlay();
	CDC *m_pOverlayDC;
	CPtrList * AddCamPart();
	void FreeOVLDC();
	void SetOVLDC();
	BOOL m_bInitialized;
	CCompo *m_pSelectedCompo;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void DeleteSelCompo();
	CdblSize m_dblsizeZoomFactor;
	CPoint GetMapedPoint(CPoint point);
	void GetMapedRect(CRect *pOrgRect);
	int Draw(CDC * pDC, CCompoTracker *pCompoTracker);
	CVWChannel *m_pGrabChannel;
	int m_iCamBuildNo;
	int LoadBufImage(CString strFileName, MIL_INT FileFormat=M_BMP);
	int LoadBufImage(CMilBuffer *pImage);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	int Inspect(CSCResult *pResult=NULL);
	void RefreshView();
	BOOL IsThereFid();
	void DrawToView();
	int SaveImage(BOOL bIsNG=TRUE);
	int Grab();
	BOOL IsGrabDone();
	int DoGrab();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	int FindOffset(CdblPoint * pDblOffset, CMilBuffer *pMilBuffProc, CMilBuffer *pMilBuffDraw);
	CCompo * FindFid();
	int GetFidROI(CRect *rectFidROI);
	CMilBuffer * GetROIChild(CSCFidAlgo *pFidAlgo);
	void ClearView();
	BOOL IsThereHEDet();
	int FindHorizontalEdge(CLineEquation *pLineEquation, CdblPoint * pDblOffset, CMilBuffer *pMilBuffProc, CMilBuffer *pMilBuffDraw);
	CCompo * FindHEdge();
	void DrawCross(CMilBuffer *pDrawBuffer);
	CRect m_rectFidSearchRange;
	BOOL IsPixelCalibrated();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	static BOOL m_bRun;
	BOOL m_bRequestReconnect;
	int SaveManImg();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void LegendUpdate();
	//kiy-----
	CMilBuffer* m_pMilBufAvgAdd;  //! AVG 영상 더한결과 저장하는 버퍼
	CMilBuffer* m_pMilBufAvgResult; //! AVG 처리된 최종 버퍼
	void SetUseAverage(BOOL use);
	void SetAverageCount(int cnt);
	BOOL GetIsAverageDone();
	CView* m_xnpiView;
	CMilBuffer *m_pMilCamAvgResult_Histo; //m_pMilBufAvgResult을 히스토그램 처리한 후의 버퍼 - 0919

	CMilBuffer *m_pMilTempBufAvgResult;	//! Avg 완료된 버퍼 저장전 임시 보관 용도
	CMilBuffer *m_pMilTempBufAvgResult_histo;	//! Avg 완료된 버퍼 저장전 임시 보관 용도

	//void TestSendCallBackGrab(int i); kiy - test용
	//--------
	CStatic mStaticAvgGV;
	int m_gvAvgRegionWidth;
	int m_gvAvgRegionHeight;
	CRect mRectStaticAvgGV;
};
