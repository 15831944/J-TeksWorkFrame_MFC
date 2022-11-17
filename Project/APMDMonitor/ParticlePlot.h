#pragma once
#include "atltypes.h"
#include "Status.h"
#include "Repository.h"
#include "MarkToolTip.h"
// CParticlePlot

class CModeDlg;
class CParticlePlot : public CStatic
{
	DECLARE_DYNAMIC(CParticlePlot)

public:
	CParticlePlot();
	virtual ~CParticlePlot();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawAxis(CDC *pUserDC=NULL);
	CDC *m_pUserDC;
	afx_msg void OnPaint();
	void SetRepository(CRepository * pRepository, int nKindOfData=0);
	CRepository *m_pRepository;
	CPtrList *m_pDispData;
	int CalcGrid();
	CRect m_PlotRect;
	CRect m_ClientRect;
	//! Index는 0번부터 시작한다. 0번인덱스가 1초
	int m_iXGridSpace; //! pixel단위로 화면상에서 X축방향으로 몇 pixel간격인지
	int m_iYGridSpace; //! pixel단위로 화면상에서 Y축방향으로 몇 pixel간격인지
	double m_dblXIndexSpace;  //!pixel단위로 Index간의 거리가 X축의 몇 pixel간격인지
	// X축으로 몇개의 라인을 그릴 수 있는지
	int m_iXGridCount;
	int m_iYGridCount;
//	int m_iXTotalIndexCount;
	tViewOptions m_ViewOptions;
	int m_iXStartIndexNo;  //! 화면상의 가장 좌측 IndexNo
	int m_iXEndIndexNo;    //! 화면상의 가장 우측 IndexNo
	int m_iCountOfRecords; //! 현재 레코드갯수 최대 Index갯수
	int DrawMark(CDC *pUserDC = NULL);
	int PlotViewRect(int CurIndexNo=0, CDC *pUserDC=NULL);
	LRESULT RecordUpdated(WPARAM wparam, LPARAM lparam);
	int SetGridUnit(int iXGridUnit, int iYGridUnit);
	// 화면상에서 X축방향으로 들어갈 수 있는 최대 Index갯수
	int m_iXScrnMaxIndexCount;
	int m_iXGridUnit;
	int GetMarkLinePos();
	int GetMarkLineIndex();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	bool m_bDrag;
	CPoint m_ptDragStart;
	int GetIndexNo(CPoint ptToGet);
	int m_iDragStartIndexNo;
	int IndexMoveToPt(int IndexNo, CPoint ptToMove);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void SetCurIndexNo(int curIndexNo);
	CModeDlg *m_pModeDlg;
	void SetModeDlg(CModeDlg * pModelDlg);
	int GetCurIndexNo();
protected:
	int m_curIndexNo;  // !좌측최초의 Index값-Navi에 의해 지정되어지는 값

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int DrawMarkToolTip(CPoint curPoint);
	LRESULT OnMarkToolTip(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void MakeHorUnitName(int iSec, CString & strUnitName);
	int PlotCompareViewRect(int CurIndexNo, CDC * pUserDC, CPtrList * pLstData);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int m_nKindOfData;//! 0:Main 1:Compare용
};


