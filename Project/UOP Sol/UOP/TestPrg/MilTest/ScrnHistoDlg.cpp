// ScrnHistoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MilTest.h"
#include "ScrnHistoDlg.h"


// CScrnHistoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CScrnHistoDlg, CDialog)

CScrnHistoDlg::CScrnHistoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScrnHistoDlg::IDD, pParent)
	, m_SliderLeft(0)
	, m_SliderRight(0)
	, m_IsManual(FALSE)
{
	m_pHistoValueOrg=NULL;
	m_pHistoValueModified=NULL;
}

CScrnHistoDlg::~CScrnHistoDlg()
{
}

void CScrnHistoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HISTO_ORG, m_HistoOrg);
	DDX_Control(pDX, IDC_HISTO_MODI, m_HistoModified);
	DDX_Control(pDX, IDC_SLIDER_HISTOGRAM, m_HistoSlider);
	DDX_Text(pDX, IDC_SLIDERLEFT, m_SliderLeft);
	DDX_Text(pDX, IDC_SLIDERRIGHT, m_SliderRight);
	DDX_Check(pDX, IDC_CHK_MAN, m_IsManual);
}


BEGIN_MESSAGE_MAP(CScrnHistoDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_REGISTERED_MESSAGE(RANGE_CHANGED, OnAdjModeUpdated)
//	ON_REGISTERED_MESSAGE(RANGE_CHANGED, OnAdjRangeUpdated)
//	ON_REGISTERED_MESSAGE(RANGE_CHANGED, OnAdjRangeChanged)
END_MESSAGE_MAP()


// CScrnHistoDlg 메시지 처리기입니다.
//실제 사용되는 값의 최대값
//#define MAX_VALUE 255
#define MAX_VALUE 255
#define MAX_RESOULTION 1024
BOOL CScrnHistoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

//Histogram메모리 초기화

	for(int i=0;i<65536;i++)m_dXValPro[i]=i;
//
	m_HistoOrg.SetXMaxRange(4096);
	m_HistoModified.SetXMaxRange(4096);

	m_HistoOrg.EnableGraph(0);
	m_HistoOrg.SetActiveData(0);
	m_HistoModified.EnableGraph(0);
	m_HistoModified.SetActiveData(0);
	m_HistoOrg.SetVertGuideLineNum(2);
	m_HistoModified.SetVertGuideLineNum(2);

//Histogram의 Slide
	m_HistoSlider.SetMinMax(0,4096);
	OnAdjModeUpdated(0,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


LRESULT CScrnHistoDlg::OnAdjModeUpdated(UINT wParam, LONG lParam)
{
	int nLeft, nRight;
	m_HistoSlider.GetRange(nLeft, nRight);
	m_HistoOrg.SetActiveVertGuideLine(0, 1);
	m_HistoOrg.SetActiveVertGuideLine(1, 1);
	m_HistoOrg.SetVertGuideLineValue(0, nLeft);
	m_HistoOrg.SetVertGuideLineValue(1, nRight);
	m_SliderLeft = nLeft;
	m_SliderRight = nRight;
	UpdateData(FALSE);

	return 0;
}

void CScrnHistoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_HistoOrg.SetDataLong(0, m_dXValPro, (double *)m_pHistoValueOrg,4096);
	m_HistoOrg.Invalidate();
	//m_HistoModified.SetDataLong(0, m_dXValPro, (double *)m_pHistoValueModified,4096);
	//m_HistoModified.Invalidate();
}

// HistoSlider
LRESULT CScrnHistoDlg::OnAdjRangeUpdated(UINT wParam, LONG lParam)
{
	m_HistoSlider.SetRange((int)wParam, (int)lParam);
	m_HistoOrg.SetVertGuideLineValue(0, (int)wParam);
	m_HistoOrg.SetVertGuideLineValue(1, (int)lParam);

	return 0;
}

// HistoSlider
LRESULT CScrnHistoDlg::OnAdjRangeChanged(UINT wParam, LONG lParam)
{
	m_HistoSlider.SetRange((int)wParam, (int)lParam);
	m_HistoOrg.SetVertGuideLineValue(0, (int)wParam);
	m_HistoOrg.SetVertGuideLineValue(1, (int)lParam);
	return 0;
}