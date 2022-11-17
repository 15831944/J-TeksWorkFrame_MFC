// HistoLevelingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "HistoLevellingDlg.h"
#include "afxdialogex.h"
#include "XNPIDoc.h"
#include "XNPIView.h"
#include "CamDlg.h"
#include "Status.h"

#define TIME_UPDATEHIOSTO 5000
// CHistoLevellingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHistoLevellingDlg, CDialogEx)

CHistoLevellingDlg::CHistoLevellingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HISTOLEVEL, pParent)
	, m_pView(NULL)
	, m_pOrgMilBuffer(NULL)
{

}

CHistoLevellingDlg::~CHistoLevellingDlg()
{
}

void CHistoLevellingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HISTOAREA, m_cHisotoArea);
	DDX_Control(pDX, IDC_BTN_HMIN, m_cMinHandle);
	DDX_Control(pDX, IDC_BTN_HMAX, m_cMaxHandle);
	DDX_Control(pDX, IDC_HISTO_HDLRECT, m_cHistoHandleRect);
	DDX_Control(pDX, IDC_COMBO_PRESET, m_cbxWLPreset);
}

BEGIN_MESSAGE_MAP(CHistoLevellingDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_HISTO_UPDATE, &CHistoLevellingDlg::OnHistoHDLUpdate)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CHistoLevellingDlg::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BT_LOADIMG, &CHistoLevellingDlg::OnBnClickedBtLoadimg)
	ON_BN_CLICKED(IDC_BTN_SAVWLV, &CHistoLevellingDlg::OnBnClickedBtnSavwlv)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, &CHistoLevellingDlg::OnCbnSelchangeComboPreset)
END_MESSAGE_MAP()


// CHistoLevellingDlg 메시지 처리기입니다.
BOOL CHistoLevellingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rectHistoHDLRect;
	m_cHistoHandleRect.GetWindowRect(&rectHistoHDLRect);
	this->ScreenToClient(&rectHistoHDLRect);
	m_cMinHandle.SetHistoHDLRect(&rectHistoHDLRect);
	m_cMaxHandle.SetHistoHDLRect(&rectHistoHDLRect);
	SetDlgItemInt(IDC_EDT_LVLLOW, 0);
	SetDlgItemInt(IDC_EDT_LVLHIGH, 65535);
	m_cMinHandle.SetPos(0);
	m_cMaxHandle.SetPos(65535);

	//! 윈도우레벨링 프리셋 로드, 총 5개 - kiy 
	CString presetName;
	for (int i = 0; i < 5; i++)
	{
		presetName.Format(_T("%d - W/L Preset %d"), i, i);
		m_cbxWLPreset.AddString(presetName);
	}

	m_cbxWLPreset.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CHistoLevellingDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIME_UPDATEHIOSTO:
		HistoUpdate();
		//Invalidate(TRUE);
		DrawHistogram();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CHistoLevellingDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	int iLow, iHigh;
	iLow = CStatus::Instance()->m_wlvLow;
	iHigh = CStatus::Instance()->m_wlvHigh;

	if (bShow)
	{
		m_pOrgMilBuffer = m_pView->m_pCamDlg[0]->m_pMilCamProc;
		SetDlgItemInt(IDC_EDT_LVLLOW, iLow);
		SetDlgItemInt(IDC_EDT_LVLHIGH, iHigh);
		OnBnClickedBtnApply();
		SetTimer(TIME_UPDATEHIOSTO, 500, NULL);
	}

	else KillTimer(TIME_UPDATEHIOSTO);
}

void CHistoLevellingDlg::HistoUpdate()
{
	CMilIm::Histogram(m_pOrgMilBuffer, 65535, m_Histodata);
}

void CHistoLevellingDlg::DrawHistogram()
{
	CRect rectHisto;
	CDC *pDC = m_cHisotoArea.GetDC();
	m_cHisotoArea.GetClientRect(&rectHisto);

	CDC memDC;
	CBitmap *pOldBitmap, bitmap;

	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, rectHisto.right, rectHisto.bottom);
	pOldBitmap = memDC.SelectObject(&bitmap);

	CPen *pOldPen = NULL;
	CBrush *pOldBrush = NULL;

	CPen pNewPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush pNewBrush(RGB(234, 239, 245));

	pOldPen = memDC.SelectObject(&pNewPen);
	pOldBrush = memDC.SelectObject(&pNewBrush);

	int heightHisto = rectHisto.Height();
	int widthHisto = rectHisto.Width();
	int *pQuantazation = new int[widthHisto];

	for (int i = 0; i < widthHisto; i++)
		pQuantazation[i] = 0;

	int posQuantazation;
	int iHighestQV = 0;
	int iHighestQ = 0;

	for (int i = 0; i < 65535; i++)
	{
		int iCount = (int)m_Histodata[i];
		posQuantazation = (i * widthHisto) / 65535;
		pQuantazation[posQuantazation] += iCount;

		if (iHighestQV < pQuantazation[posQuantazation])
		{
			iHighestQV = pQuantazation[posQuantazation];
			iHighestQ = posQuantazation;
		}
	}

	if (!iHighestQV)iHighestQV = CAM_HRES * CAM_VRES;
	memDC.MoveTo(0, heightHisto);
	double rateHeight = (double)heightHisto / iHighestQV;
	memDC.Rectangle(&rectHisto);

	for (int i = 0; i < widthHisto; i++)
	{
		memDC.LineTo(i, (int)(heightHisto - (rateHeight * pQuantazation[i])));
		memDC.MoveTo(i + 1, heightHisto);
	}

	pDC->BitBlt(0, 0, rectHisto.right, rectHisto.bottom, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldPen);
	memDC.SelectObject(pOldBrush);
	memDC.SelectObject(pOldBitmap);

	bitmap.DeleteObject();
	memDC.DeleteDC();

	m_cHisotoArea.ReleaseDC(pDC);
}

void CHistoLevellingDlg::OnBnClickedBtnApply()
{
	int iLow, iHigh;
	int uiLvlLow = GetDlgItemInt(IDC_EDT_LVLLOW);
	int uiLvlHigh = GetDlgItemInt(IDC_EDT_LVLHIGH);

	if (uiLvlHigh > 65535)
	{
		uiLvlHigh = 65535;
		SetDlgItemInt(IDC_EDT_LVLHIGH, uiLvlHigh);
	}

	if (uiLvlLow <= uiLvlHigh)
	{
		iLow = uiLvlLow;
		iHigh = uiLvlHigh;
	}
	else
	{
		iLow = uiLvlHigh;
		iHigh = uiLvlLow;
		SetDlgItemInt(IDC_EDT_LVLLOW, iLow);
		SetDlgItemInt(IDC_EDT_LVLHIGH, iHigh);
	}
	//kiy	//TEST CHECK
	if (!CStatus::Instance()->m_bIsOffLine)	//Detector is Online
		m_pView->m_pCamDlg[0]->m_pGrabChannel->SetWndLvl(iLow, iHigh);

	CStatus::Instance()->m_wlvLow = iLow;
	CStatus::Instance()->m_wlvHigh = iHigh;

	int selNum = m_cbxWLPreset.GetCurSel();

	CStatus::Instance()->m_wlnLowPresetArr[selNum] = iLow;
	CStatus::Instance()->m_wlnHighPresetArr[selNum] = iHigh;

	m_cMinHandle.SetPos(iLow);
	m_cMaxHandle.SetPos(iHigh);
}

BOOL CHistoLevellingDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedBtnApply();
			return TRUE;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CHistoLevellingDlg::OnHistoHDLUpdate(WPARAM wparam, LPARAM lparam)
{
	SetDlgItemInt(IDC_EDT_LVLLOW, m_cMinHandle.GetPos());
	SetDlgItemInt(IDC_EDT_LVLHIGH, m_cMaxHandle.GetPos());
	OnBnClickedBtnApply();
	return LRESULT();
}

void CHistoLevellingDlg::OnBnClickedBtLoadimg()
{
	CFileDialog LoadFile(TRUE, _TEXT("*.tif;*.bmp;*.jpg"));
	if (LoadFile.DoModal())
	{
		CString path = (WCHAR *)(LoadFile.GetPathName().GetString());
		m_pOrgMilBuffer->Restore(path);
	}
}

void CHistoLevellingDlg::OnBnClickedBtnSavwlv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStatus::Instance()->SaveCFG();
}

BOOL CHistoLevellingDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CHistoLevellingDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CHistoLevellingDlg::OnCbnSelchangeComboPreset()
{
	int selNum = m_cbxWLPreset.GetCurSel();
	int min, max;

	min = CStatus::Instance()->m_wlnLowPresetArr[selNum];
	max = CStatus::Instance()->m_wlnHighPresetArr[selNum];

	SetDlgItemInt(IDC_EDT_LVLLOW, min);
	SetDlgItemInt(IDC_EDT_LVLHIGH, max);

	OnBnClickedBtnApply();
}
