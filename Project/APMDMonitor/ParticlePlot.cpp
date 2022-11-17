// ParticlePlot.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "ParticlePlot.h"
#include "ModeDlg.h"
#include "GraphDlg.h"

// CParticlePlot

IMPLEMENT_DYNAMIC(CParticlePlot, CStatic)

CParticlePlot::CParticlePlot()
	: m_pUserDC(NULL)
	, m_pRepository(NULL)
	, m_iXGridSpace(60)
	, m_iYGridSpace(60)
	, m_iXGridCount(0)
	, m_iYGridCount(0)
	, m_curIndexNo(-1)
	, m_iXStartIndexNo(0)
	, m_iCountOfRecords(0)
	, m_dblXIndexSpace(0.0)
	, m_iXEndIndexNo(0)
	, m_iXScrnMaxIndexCount(0)
	, m_iXGridUnit(0)
	, m_bDrag(false)
	, m_ptDragStart(0)
	, m_iDragStartIndexNo(0)
	, m_pModeDlg(NULL)
	, m_nKindOfData(0)
{
	m_ViewOptions.iParticleType = 1;
	m_ViewOptions.iXGridUnit = 1;
	m_ViewOptions.iYGridUnit = 1;
}

CParticlePlot::~CParticlePlot()
{
}


BEGIN_MESSAGE_MAP(CParticlePlot, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_MARK_TOOLTIPTIME, OnMarkToolTip)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CParticlePlot 메시지 처리기입니다.



void CParticlePlot::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rectClient;
	GetClientRect(&rectClient);
	COLORREF color = GetSysColor(COLOR_3DFACE);
	dc.FillSolidRect(rectClient, color);

	CalcGrid();
	DrawAxis(&dc);
	PlotViewRect(m_curIndexNo, &dc);
	if ((CStatus::Instance()->IsCompareMode()) && (CStatus::Instance()->IsOverlayMode()))
	{
		int iDiffTime = ((CGraphDlg *)(GetParent()))->GetDiffTime();
		int CompareIndex = m_curIndexNo + iDiffTime;
		PlotCompareViewRect(CompareIndex, &dc, &(m_pRepository->m_DataRecords[1].m_APMDRecords));
	}
	DrawMark(&dc);
					   // 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.
}


void CParticlePlot::SetRepository(CRepository * pRepository, int nKindOfData)
{
	m_pRepository = pRepository;
	m_nKindOfData = nKindOfData;
	m_pDispData = &(m_pRepository->m_DataRecords[nKindOfData].m_APMDRecords);
	OnPaint();
}

/*!
m_PlotRect:그림이 그려질 전체 영역인데 축부분을 제외함
m_iGridSpace:
m_curIndex는 현재 Index값으로 그림상에서는 좌측 첫번째를 의미한다.
Index는 0번부터 즉 0번은 1초임

*/

int CParticlePlot::CalcGrid()
{
	CRect ClientRect;
	GetClientRect(ClientRect);
	m_ClientRect = ClientRect;
	m_ViewOptions = CStatus::Instance()->m_ViewOptions;
	CPtrList *pList = m_pDispData;
	m_iCountOfRecords = pList->GetCount();

	CRect PlotRect(60, 10, ClientRect.right - 10, ClientRect.bottom - 16);
	m_PlotRect = PlotRect;
	
	m_iXGridCount = m_PlotRect.Width() / m_iXGridSpace;
	m_iXGridUnit = m_ViewOptions.iXGridUnit;
	m_dblXIndexSpace = (double)m_iXGridSpace / m_iXGridUnit;
	m_iXScrnMaxIndexCount = (int)(m_PlotRect.Width() / m_dblXIndexSpace);
	if (m_curIndexNo!=-1)  
	{//시작번호가 있으면 해당 시작번호부터 시작
		m_iXStartIndexNo = m_curIndexNo;
		if (m_iXScrnMaxIndexCount > m_iCountOfRecords)
			m_iXEndIndexNo = m_iXScrnMaxIndexCount-1;  //전체 화면 보다 수량이 적을 경우
		else
			m_iXEndIndexNo = m_iXStartIndexNo + (m_iXScrnMaxIndexCount);
	}else
	{//! -1:Append로 추가 
		if (m_iCountOfRecords == 0)
		{
			m_iXEndIndexNo = 0;
			m_iXStartIndexNo = 0;
		}
		else
		{
			m_iXEndIndexNo = m_iCountOfRecords - 1;
			if (m_iXScrnMaxIndexCount > m_iCountOfRecords)
				m_iXStartIndexNo = 0;
			else m_iXStartIndexNo = m_iCountOfRecords - m_iXScrnMaxIndexCount;
		}

	}
	
	m_iYGridCount = m_PlotRect.Height() / m_iYGridSpace;
	return 0;
}

void CParticlePlot::DrawAxis(CDC *pUserDC)
{
	CDC *pDC;
	pDC = (pUserDC != NULL) ? pUserDC :
		(m_pUserDC != NULL) ? m_pUserDC :
		GetDC();

	CPen myPen;
	myPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&myPen);

	int YAxisXpos = m_PlotRect.left;
	pDC->MoveTo(YAxisXpos, 10);
	pDC->LineTo(YAxisXpos, m_ClientRect.bottom);
	pDC->MoveTo(0, m_ClientRect.bottom - 16);
	pDC->LineTo(m_ClientRect.right - 10, m_ClientRect.bottom - 16);

	pDC->SelectObject(pOldPen);
	myPen.DeleteObject();

	myPen.CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
	pOldPen = pDC->SelectObject(&myPen);

	CStatus::Instance()->m_ViewOptions;

	pDC->SetBkMode(TRANSPARENT);
	CString strXUnitValue;
	pDC->SetTextAlign(TA_CENTER);

	int left = m_PlotRect.left;
	for (int i = m_iXStartIndexNo+1; i < m_iXStartIndexNo+m_iXScrnMaxIndexCount+1; i++)
	{
		//! XlineDraw  draw Vertical lines
		if ((i%m_iXGridUnit) == 0)
		{
			pDC->MoveTo((int)(left+m_dblXIndexSpace*(i-(m_iXStartIndexNo))), m_PlotRect.top);
			pDC->LineTo((int)(left + m_dblXIndexSpace*(i - (m_iXStartIndexNo))), m_PlotRect.bottom);
			MakeHorUnitName(i, strXUnitValue);
//			strXUnitValue.Format(_T("%d"), i);
			pDC->TextOutW((int)(left + m_dblXIndexSpace*(i - (m_iXStartIndexNo))), m_ClientRect.bottom - 16, strXUnitValue);
		}
	}

	pDC->SetTextAlign(TA_RIGHT);
	CString strYUnitValue;
	for (int i = 0; i < m_iYGridCount + 1; i++)
	{
		
		//! YlineDraw draw horizontal lines
		pDC->MoveTo(m_PlotRect.left, m_PlotRect.bottom - m_iYGridSpace*i);
		pDC->LineTo(m_PlotRect.right, m_PlotRect.bottom - m_iYGridSpace*i);
		if (i == 0)continue;
		strYUnitValue.Format(_T("%d"), i*m_ViewOptions.iYGridUnit);
		pDC->TextOutW(m_PlotRect.left - 3, m_PlotRect.bottom - m_iYGridSpace*i, strYUnitValue);
	}
	pDC->SelectObject(pOldPen);
}

int CParticlePlot::PlotViewRect(int CurIndexNo, CDC *pUserDC)
{
	CDC *pDC;
	pDC = (pUserDC != NULL) ? pUserDC :
		(m_pUserDC != NULL) ? m_pUserDC :
		GetDC();
	int StartIndex = 0;

	CPen myPen;
	myPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&myPen);


	POSITION pos;
	tAPMDRecord *pAPMDRecord;
	CPtrList *pList = m_pDispData;

	int left = m_PlotRect.left;
	int bottom = m_PlotRect.bottom;
	int nDispCount = 0;
	for (pos = pList->FindIndex(m_iXStartIndexNo); (pos != NULL) &&  (m_iXStartIndexNo + nDispCount)<= m_iXEndIndexNo;)
	{
		pAPMDRecord = (tAPMDRecord *)pList->GetNext(pos);
		int IndexValue;
		switch (m_ViewOptions.iParticleType)
		{
		case 0:
			IndexValue = pAPMDRecord->Particle01;
			break;
		case 1:
			IndexValue = pAPMDRecord->Particle03;
			break;
		case 2:
			IndexValue = pAPMDRecord->Particle05;
			break;
		}
		int IndexSpaceHeight = (int)(m_iYGridSpace*((double)IndexValue / m_ViewOptions.iYGridUnit));
		int IndexYPosToDraw = bottom - IndexSpaceHeight;
		if (IndexYPosToDraw < 1)IndexYPosToDraw = 1;
		pDC->MoveTo(left + (int)(m_dblXIndexSpace*(nDispCount+1)), bottom);
		pDC->LineTo(left + (int)(m_dblXIndexSpace*(nDispCount+1)), IndexYPosToDraw);
		nDispCount++;
	}

	pDC->SelectObject(pOldPen);
	myPen.DeleteObject();

	return 0;
}


int CParticlePlot::PlotCompareViewRect(int CurIndexNo, CDC * pUserDC, CPtrList * pLstData)
{
	CDC *pDC;
	pDC = (pUserDC != NULL) ? pUserDC :
		(m_pUserDC != NULL) ? m_pUserDC :
		GetDC();
	int StartIndex = 0;

	CPen myPen;
	myPen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&myPen);


	POSITION pos;
	tAPMDRecord *pAPMDRecord;
	CPtrList *pList = pLstData;

	int left = m_PlotRect.left;
	int bottom = m_PlotRect.bottom;
	int nDispCount = 0;
	for (pos = pList->FindIndex(CurIndexNo); (pos != NULL) && (nDispCount <= m_iXScrnMaxIndexCount);)
	{
		pAPMDRecord = (tAPMDRecord *)pList->GetNext(pos);
		int IndexValue;
		switch (m_ViewOptions.iParticleType)
		{
		case 0:
			IndexValue = pAPMDRecord->Particle01;
			break;
		case 1:
			IndexValue = pAPMDRecord->Particle03;
			break;
		case 2:
			IndexValue = pAPMDRecord->Particle05;
			break;
		}
		int IndexSpaceHeight = (int)(m_iYGridSpace*((double)IndexValue / m_ViewOptions.iYGridUnit));
		int IndexYPosToDraw = bottom - IndexSpaceHeight;
		if (IndexYPosToDraw < 1)IndexYPosToDraw = 1;
		pDC->MoveTo(left + (int)(m_dblXIndexSpace*(nDispCount + 1))+2, bottom);
		pDC->LineTo(left + (int)(m_dblXIndexSpace*(nDispCount + 1))+2, IndexYPosToDraw);
		nDispCount++;
	}

	pDC->SelectObject(pOldPen);
	myPen.DeleteObject();

	return 0;
}


int CParticlePlot::DrawMark(CDC *pUserDC)
{
	CDC *pDC;
	pDC = (pUserDC != NULL) ? pUserDC :
		(m_pUserDC != NULL) ? m_pUserDC :
		GetDC();

	CPen myPen;
	myPen.CreatePen(PS_DASHDOT, 3, RGB(0, 255, 255));
	CPen* pOldPen = pDC->SelectObject(&myPen);

	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		pDC->MoveTo(GetMarkLinePos(), m_ClientRect.top);
		pDC->LineTo(GetMarkLinePos(), m_ClientRect.bottom + 8);
	}

	pDC->SelectObject(pOldPen);
	myPen.DeleteObject();

	tAPMDRecord *pAPMDRecord;
	CPtrList *pList = m_pDispData;
	int iCountOfRecords = pList->GetCount();
	if (!iCountOfRecords)return -1;

	myPen.CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&myPen);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextAlign(TA_CENTER);
	POSITION pos;
	pos = pList->FindIndex(m_iXStartIndexNo);
	int left = m_PlotRect.left;
	int yMarkPos = m_PlotRect.top + 20;

	int DeltaY = 2;
	for (int i = 0; i < (m_iCountOfRecords - m_iXStartIndexNo); i++)
	{
		pAPMDRecord = (tAPMDRecord *)pList->GetNext(pos);
		if (!pAPMDRecord->strMark.IsEmpty())
		{
			pDC->MoveTo((int)(left + m_dblXIndexSpace*(i+1) + 3), m_PlotRect.top);
			pDC->LineTo((int)(left + m_dblXIndexSpace*(i+1) + 3), m_PlotRect.bottom);
			pDC->TextOutW((int)(left + m_dblXIndexSpace*(i+1)), yMarkPos + 20 * (DeltaY % 2), pAPMDRecord->strMark);
			DeltaY++;
		}
	}

	pDC->SelectObject(pOldPen);
	myPen.DeleteObject();

	return 0;
}

LRESULT CParticlePlot::RecordUpdated(WPARAM wparam, LPARAM lparam)
{
	return 0;
}



int CParticlePlot::SetGridUnit(int iXGridUnit, int iYGridUnit)
{
	m_ViewOptions.iXGridUnit = iXGridUnit;
	m_ViewOptions.iYGridUnit = iYGridUnit;
	return 0;
}


int CParticlePlot::GetMarkLinePos()
{
//	return 	m_PlotRect.left + (int)((m_iXScrnMaxIndexCount / 2)*m_dblXIndexSpace);
	return 	(int)(m_PlotRect.left + m_dblXIndexSpace+ 2);
}


int CParticlePlot::GetMarkLineIndex()
{
	return m_iXScrnMaxIndexCount/2;
}


void CParticlePlot::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ReleaseCapture();
	m_bDrag = false;
//	GetParent()->Invalidate();
	CStatic::OnLButtonUp(nFlags, point);
}


void CParticlePlot::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCapture();
	m_ptDragStart = point;
	m_iDragStartIndexNo = GetIndexNo(point);
	m_bDrag = true;
	CStatic::OnLButtonDown(nFlags, point);
}


void CParticlePlot::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDrag)
	{
		int iTargetIndex = IndexMoveToPt(m_iDragStartIndexNo, point);
		if (!m_nKindOfData)
		{
			((CGraphDlg *)(GetParent()))->m_cNavi.SetPos(GetCurIndexNo() + 1);
			((CGraphDlg *)(GetParent()))->PosTextOut();
			int iDiffTime = ((CGraphDlg *)(GetParent()))->GetDiffTime();
			int iComapareMoveTo = iTargetIndex + iDiffTime;
			if (iComapareMoveTo>= 0)
			{
				SetCurIndexNo(iTargetIndex);
				((CGraphDlg *)(GetParent()))->m_cComparePlot.SetCurIndexNo(iComapareMoveTo);
				((CGraphDlg *)(GetParent()))->m_cComparePlot.Invalidate();
			}
		}else
		{
			SetCurIndexNo(iTargetIndex);
			((CGraphDlg *)(GetParent()))->DiffTimeTextOut();
		}
		Invalidate(false);
	}
	CStatic::OnMouseMove(nFlags, point);
}


int CParticlePlot::GetIndexNo(CPoint ptToGet)
{
	int xPtInPlotRect = ptToGet.x - m_PlotRect.left;
	int IndexCount = (int)(xPtInPlotRect / m_dblXIndexSpace);

	return IndexCount + m_iXStartIndexNo;
}


int CParticlePlot::IndexMoveToPt(int IndexNo, CPoint ptToMove)
{
	int iTargetIndex;
	int xPtInPlotRect = ptToMove.x - m_PlotRect.left;
	int IndexCount = (int)(xPtInPlotRect / m_dblXIndexSpace);
	
	iTargetIndex = IndexNo - IndexCount;
	if(iTargetIndex <= 0)iTargetIndex = 0;
	if(iTargetIndex >= (m_iCountOfRecords - 1))iTargetIndex = m_iCountOfRecords - 1;

//	SetCurIndexNo(iTargetIndex);
	return iTargetIndex;
}


BOOL CParticlePlot::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, IDC_HAND));
	return TRUE;
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}


void CParticlePlot::SetCurIndexNo(int curIndexNo)
{
	m_curIndexNo = curIndexNo;
	if (m_curIndexNo < -1)m_curIndexNo = -1;
	if( (m_curIndexNo != -1) )
		m_pModeDlg->SendMessage(WM_UPDATE_STATINFO);
}


void CParticlePlot::SetModeDlg(CModeDlg * pModeDlg)
{
	m_pModeDlg = pModeDlg;
}


int CParticlePlot::GetCurIndexNo()
{
	return m_curIndexNo;
}


BOOL CParticlePlot::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CStatic::PreTranslateMessage(pMsg);
}


int CParticlePlot::DrawMarkToolTip(CPoint curPoint)
{
	CDC *pDC=GetDC();
	CString strMsg;
	CPen myPen;
	myPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&myPen);

	CPoint AlignPoint;
	AlignPoint = curPoint;
	int iIndexNo = (int)( (curPoint.x-m_PlotRect.left) / m_dblXIndexSpace);
	iIndexNo += m_iXStartIndexNo;

	tMarkSectionInfo MarkSectionInfo;
	if(m_pRepository->AnalizeMarkToMark(iIndexNo, MarkSectionInfo, m_nKindOfData)!=0)return -1;

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextAlign(TA_LEFT);
	strMsg.Format(_T("%s <-> %s"), MarkSectionInfo.strStartMark, MarkSectionInfo.strEndMark);
	pDC->TextOutW((int)AlignPoint.x, (int)AlignPoint.y, strMsg);
	strMsg.Format(_T("    %6d sec"), MarkSectionInfo.iCumulativeTime);
	pDC->TextOutW((int)AlignPoint.x, (int)AlignPoint.y+16, strMsg);
	strMsg.Format(_T("0.1um %6d ea"), MarkSectionInfo.particle01.iSum);
	pDC->TextOutW((int)AlignPoint.x, (int)AlignPoint.y+16*2, strMsg);
	strMsg.Format(_T("0.3um %6d ea"), MarkSectionInfo.particle03.iSum);
	pDC->TextOutW((int)AlignPoint.x, (int)AlignPoint.y+16*3, strMsg);
	strMsg.Format(_T("0.5um %6d ea"), MarkSectionInfo.particle05.iSum);
	pDC->TextOutW((int)AlignPoint.x, (int)AlignPoint.y+16*4, strMsg);
	strMsg.Format(_T("Humidity %0.2f%%"), (float)(MarkSectionInfo.Humi.dblAvg));
	pDC->TextOutW((int)AlignPoint.x, (int)AlignPoint.y+16*5, strMsg);
	pDC->SelectObject(pOldPen);

	return 0;
}


LRESULT CParticlePlot::OnMarkToolTip(WPARAM wParam, LPARAM lParam)
{
	CPoint curPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	DrawMarkToolTip(curPoint);
	return LRESULT();
}


void CParticlePlot::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	DrawMarkToolTip(point);
	CStatic::OnLButtonDblClk(nFlags, point);
}


void CParticlePlot::MakeHorUnitName(int iSec, CString & strUnitName)
{
	tViewOptions viewOptions= CStatus::Instance()->m_ViewOptions;

	//int iHours, iMinutes, iSeconds;
	//iHours = iSec / 3600;
	//iMinutes = (iSec % 3600) / 60;
	//iSeconds = (iSec % 3600) % 60;

	switch (viewOptions.iXSelUnit)
	{
	case 0:
		strUnitName.Format(_T("%d"), iSec);
		break;
	case 1:
		strUnitName.Format(_T("%d"), iSec / 60);
		break;
	}
}

BOOL CParticlePlot::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return CStatic::OnEraseBkgnd(pDC);
}
