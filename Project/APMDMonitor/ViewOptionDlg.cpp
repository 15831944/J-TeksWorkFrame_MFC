// ViewOptionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "ViewOptionDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "GraphDlg.h"

// CViewOptionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewOptionDlg, CDialogEx)

CViewOptionDlg::CViewOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIEW_OPTION, pParent)
	, m_DispMode(0)
	, m_strAddMark(_T(""))
	, m_strUserMarkName(_T(""))
	, m_plstMarkInfos(NULL)
	, m_pGraphDlg(NULL)
	, m_pRepository(NULL)
	, m_prevSelIndex(-1)
	, m_strNextMarkName(_T(""))
{
	if (CStatus::GetDispMode() == CStatus::DM_FREQ) m_DispMode = 0; else m_DispMode = 1;
}

CViewOptionDlg::~CViewOptionDlg()
{
}

void CViewOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RDISPMODE, m_DispMode);
	DDX_Control(pDX, IDC_CBSCALEHVU, m_cHScaleUnit);
	DDX_Control(pDX, IDC_CBSCALEV, m_cVScaleValue);
	DDX_Control(pDX, IDC_CBSCALEHV, m_cHScaleValue);
	DDX_Control(pDX, IDC_CB_DISPPARTTYPE, m_cDispParticleType);
	DDX_Text(pDX, IDC_EDT_MM, m_strUserMarkName);
	DDX_Control(pDX, IDC_CB_MARK, m_cCBMarkBrowser);
	DDX_Text(pDX, IDC_NXTMARK, m_strNextMarkName);
}


BEGIN_MESSAGE_MAP(CViewOptionDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_CBSCALEHVU, &CViewOptionDlg::OnCbnSelchangeOptionChange)
	ON_CBN_SELCHANGE(IDC_CBSCALEV, &CViewOptionDlg::OnCbnSelchangeOptionChange)
	ON_CBN_SELCHANGE(IDC_CBSCALEHV, &CViewOptionDlg::OnCbnSelchangeOptionChange)
	ON_CBN_SELCHANGE(IDC_CB_DISPPARTTYPE, &CViewOptionDlg::OnCbnSelchangeOptionChange)
	ON_BN_CLICKED(IDC_ADDMARK, &CViewOptionDlg::OnBnClickedAddmark)
	ON_BN_CLICKED(IDC_BTNBMARKUPDATE, &CViewOptionDlg::OnBnClickedBtnbmarkupdate)
	ON_CBN_SELCHANGE(IDC_CB_MARK, &CViewOptionDlg::OnCbnSelchangeCbMark)
	ON_BN_CLICKED(IDC_BTNBMARKPREV, &CViewOptionDlg::OnBnClickedBtnbmarkprev)
	ON_BN_CLICKED(IDC_BTNBMARKNEXT, &CViewOptionDlg::OnBnClickedBtnbmarknext)
	ON_BN_CLICKED(IDC_BTNBMARKDEL, &CViewOptionDlg::OnBnClickedBtnbmarkdel)
END_MESSAGE_MAP()


// CViewOptionDlg 메시지 처리기입니다.


BOOL CViewOptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cHScaleUnit.AddString(_T("Seconds"));
	m_cHScaleUnit.AddString(_T("Minutes"));
	m_cVScaleValue.AddString(_T("1"));
	m_cVScaleValue.AddString(_T("5"));
	m_cVScaleValue.AddString(_T("10"));
	m_cVScaleValue.AddString(_T("50"));
	m_cVScaleValue.AddString(_T("100"));
	m_cVScaleValue.AddString(_T("500"));
	m_cVScaleValue.AddString(_T("1000"));
	m_cVScaleValue.AddString(_T("5000"));
	m_cVScaleValue.AddString(_T("50000"));
	m_cVScaleValue.AddString(_T("100000"));
	m_cVScaleValue.AddString(_T("500000"));
	m_cVScaleValue.AddString(_T("1000000"));
	m_cVScaleValue.SetCurSel(3);

	m_cHScaleValue.AddString(_T("1"));
	m_cHScaleValue.AddString(_T("2"));
	m_cHScaleValue.AddString(_T("5"));
	m_cHScaleValue.AddString(_T("10"));
	m_cHScaleValue.AddString(_T("20"));
	m_cHScaleValue.AddString(_T("30"));
	m_cHScaleValue.AddString(_T("40"));
	m_cHScaleValue.AddString(_T("50"));
	m_cHScaleValue.AddString(_T("60"));
	m_cHScaleValue.SetCurSel(2);
	m_cHScaleUnit.SetCurSel(0);
	m_cDispParticleType.AddString(_T("0.1um"));
	m_cDispParticleType.AddString(_T("0.3um"));
	m_cDispParticleType.AddString(_T("0.5um"));
	m_cDispParticleType.SetCurSel(0);
	UpdateData(FALSE);

	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
	{
		m_cCBMarkBrowser.EnableWindow(FALSE);
		GetDlgItem(IDC_BTNBMARKPREV)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNBMARKNEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNBMARKDEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNBMARKUPDATE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDT_MM)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADDMARK)->EnableWindow(FALSE);
	}

	OnCbnSelchangeOptionChange();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CViewOptionDlg::OnCbnSelchangeOptionChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	TCHAR szTemp[255];
	CStatus *pStatus;
	pStatus = CStatus::Instance();
	tViewOptions ViewOptions;

	m_cHScaleValue.GetLBText(m_cHScaleValue.GetCurSel(), szTemp);
	int iHScaleValue =_wtoi(szTemp);
	ViewOptions.iXGridUnit = (int)(iHScaleValue*pow(60, m_cHScaleUnit.GetCurSel()));
	ViewOptions.iXSelUnit = m_cHScaleUnit.GetCurSel();

	m_cVScaleValue.GetLBText(m_cVScaleValue.GetCurSel(), szTemp);
	int iVScaleValue = _wtoi(szTemp);
	ViewOptions.iYGridUnit = iVScaleValue;
	ViewOptions.iParticleType = m_cDispParticleType.GetCurSel();

	pStatus->SetViewOptions(ViewOptions);
}


void CViewOptionDlg::OnBnClickedAddmark()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CStatus::GetProgramMode() == CStatus::PM_MONITORING)
	{
		int preLoadMarkCount = m_lstMark.GetCount();
		UpdateData(TRUE);
		if (!m_strUserMarkName.IsEmpty())
		{
			m_strAddMark = m_strUserMarkName;
		}
		else
		{
			if ((preLoadMarkCount != 0) && (m_lstMark.GetCount() > CStatus::Instance()->GetMarkPos()))
			{
				m_strAddMark = m_lstMark.GetAt(CStatus::Instance()->GetMarkPos());
				if (m_lstMark.GetCount() > CStatus::Instance()->GetMarkPos() + 1)
					m_strNextMarkName = m_lstMark.GetAt(CStatus::Instance()->GetMarkPos() + 1);
				else
					m_strNextMarkName = _T("");
			}
			if (m_strAddMark.IsEmpty())
			{
				m_strAddMark.Format(_T("Mark%d"), CStatus::Instance()->GetAutoMarkPos());
				CStatus::Instance()->IncAutoMarkPos();
			}
			CStatus::Instance()->IncMarkPos();
		}
		m_strUserMarkName = _T("");
		UpdateData(FALSE);

	}else
	{
		////! ReviewMode
		//UpdateData(TRUE);
		//if (!m_strUserMarkName.IsEmpty())
		//{
		//	m_strAddMark = m_strUserMarkName;
		//}else
		//{
		//	if ((preLoadMarkCount != 0) && (m_lstMark.GetCount()>CStatus::Instance()->GetMarkCount()))
		//		m_strAddMark = m_lstMark.GetAt(CStatus::Instance()->GetMarkCount());
		//	if (m_strAddMark.IsEmpty())
		//	{
		//		m_strAddMark.Format(_T("Mark%d"), CStatus::Instance()->GetMarkCount());
		//	}
		//}
		//m_strUserMarkName = _T("");
		//UpdateData(FALSE);
	}

}


CString CViewOptionDlg::GetMark()
{
	CString strSend;
		strSend = m_strAddMark;
		m_strAddMark = _T("");
	return strSend;
}

void CViewOptionDlg::OnCbnSelchangeCbMark()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIndex = m_cCBMarkBrowser.GetCurSel();
	POSITION pos;
	tMarkInfo * pMarkInfo;
	pos = m_plstMarkInfos->FindIndex(nIndex);
	pMarkInfo = (tMarkInfo *)m_plstMarkInfos->GetAt(pos);
	int IndexToUdate = pMarkInfo->nIndex;

	MarkMove(IndexToUdate);
	m_prevSelIndex = IndexToUdate;
}

void CViewOptionDlg::OnBnClickedBtnbmarkprev()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int curIndex = m_pGraphDlg->m_cParticlePlot.GetCurIndexNo();
	tMarkInfo MarkInfo;
	if(!m_pRepository->FindPrevMark(curIndex-1, MarkInfo))
	{
		MarkMove(MarkInfo.nIndex);
		m_prevSelIndex = MarkInfo.nIndex;
	}

}

void CViewOptionDlg::OnBnClickedBtnbmarknext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int curIndex = m_pGraphDlg->m_cParticlePlot.GetCurIndexNo();
	if (curIndex == -1)curIndex = 0;
	tMarkInfo MarkInfo;
	if (!m_pRepository->FindNextMark(curIndex+1, MarkInfo))
	{
		MarkMove(MarkInfo.nIndex);
		m_prevSelIndex = MarkInfo.nIndex;
	}
}

void CViewOptionDlg::OnBnClickedBtnbmarkdel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int curIndex = m_pGraphDlg->m_cParticlePlot.GetCurIndexNo();
	if (curIndex == -1)curIndex = 0;
	m_pRepository->DeleteMark(curIndex);
	m_pRepository->MakeMarkInfos();
	m_pGraphDlg->Invalidate();
	UpdateMarkView();
}

void CViewOptionDlg::OnBnClickedBtnbmarkupdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int curIndex = m_pGraphDlg->m_cParticlePlot.GetCurIndexNo();
	if (curIndex == -1)curIndex = 0;
	CString strUpdate;
	GetDlgItemText(m_cCBMarkBrowser.GetDlgCtrlID(), strUpdate);
	m_pRepository->UpdateMark(curIndex, strUpdate);
	m_pGraphDlg->Invalidate();
	UpdateMarkView();
}

void CViewOptionDlg::SetGraphDlg(CGraphDlg * pGraphDlg)
{
	m_pGraphDlg = pGraphDlg;
}


void CViewOptionDlg::MarkMove(int nIndexToGo)
{
	CStatus::Instance()->SetNaviStatus(true);
	m_pGraphDlg->m_cParticlePlot.SetCurIndexNo(nIndexToGo);
	m_pGraphDlg->PosTextOut();
	m_pGraphDlg->Invalidate();
	CStatus::Instance()->SetNaviStatus(false);
}


void CViewOptionDlg::SetRepository(CRepository * pRepository)
{
	m_pRepository = pRepository;
	m_plstMarkInfos = &(pRepository->m_DataRecords[0].lstMarkInfos);
	UpdateMarkView();
}


void CViewOptionDlg::UpdateMarkView()
{
	POSITION pos;

	int nCount = 0;
	tMarkInfo *pMarkInfo = NULL;
	m_cCBMarkBrowser.ResetContent();

	for (pos = m_plstMarkInfos->GetHeadPosition(); pos != NULL; nCount++)
	{
		pMarkInfo = (tMarkInfo *)m_plstMarkInfos->GetNext(pos);

		m_cCBMarkBrowser.AddString(pMarkInfo->strMarkName);
	}
}

BOOL CViewOptionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
