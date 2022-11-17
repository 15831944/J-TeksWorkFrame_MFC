// OpPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "OpPanelDlg.h"
#include "afxdialogex.h"
#include "Repository.h"
#include "ALMDMessages.h"
#include "UnitConversion.h"
#include "Status.h"

// COpPanelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COpPanelDlg, CDialogEx)

COpPanelDlg::COpPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPPANEL, pParent)
	, m_iRefPlan(0)
	, m_strMOP(_T(""))
	, m_strMTool(_T(""))
	, m_strMStation(_T(""))
	, m_strRelRoll(_T(""))
	, m_strRelPitch(_T(""))
	, m_strMComment(_T(""))
	, m_pRepository(NULL)
	, m_dptRefDegree(0)
	, m_strXValueUnit(_T(""))
	, m_strYValueUnit(_T(""))
	, m_DispRefValue(0)
{

}

COpPanelDlg::~COpPanelDlg()
{
}

void COpPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RREFPLAN, m_iRefPlan);
	DDX_Text(pDX, IDC_EDT_MOP, m_strMOP);
	DDX_Text(pDX, IDC_EDT_MTOOL, m_strMTool);
	DDX_Text(pDX, IDC_EDT_MSTATION, m_strMStation);
	DDX_Text(pDX, IDC_EDT_RROL, m_strRelRoll);
	DDX_Text(pDX, IDC_EDT_RPITCH, m_strRelPitch);
	DDX_Text(pDX, IDC_EDT_MCMT, m_strMComment);
	DDX_Control(pDX, IDC_EDT_MOP, m_cMOperator);
	DDX_Control(pDX, IDC_BTN_MPREV, m_cBtnMPrev);
	DDX_Control(pDX, IDC_BTN_MNEXT, m_cBtnMNext);
	DDX_Control(pDX, IDC_EDT_MPOS, m_cEdtMPos);
	DDX_Control(pDX, IDC_LNOF, m_cLNOf);
	DDX_Control(pDX, IDC_LTOT, m_cLTOT);
	DDX_Control(pDX, IDC_EDT_MTOOL, m_cMTool);
	DDX_Control(pDX, IDC_EDT_MSTATION, m_cMStation);
	DDX_Control(pDX, IDC_EDT_MCMT, m_cMComment);
	DDX_Text(pDX, IDC_XVALUEUNIT, m_strXValueUnit);
	DDX_Text(pDX, IDC_YVALUEUNIT, m_strYValueUnit);
}


BEGIN_MESSAGE_MAP(COpPanelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SETPLAN, &COpPanelDlg::OnBnClickedBtnSetplan)
	ON_BN_CLICKED(IDC_BTN_MARKAPPLY, &COpPanelDlg::OnBnClickedBtnMarkapply)
	ON_BN_CLICKED(IDC_BTN_MARKCLEAR, &COpPanelDlg::OnBnClickedBtnMarkclear)
	ON_BN_CLICKED(IDC_RREFPLAN, &COpPanelDlg::OnBnClickedRrefplan)
	ON_MESSAGE(WM_PM_CHANGE, &COpPanelDlg::OnPMChange)
	ON_MESSAGE(WM_UPDATE_STATINFO, &COpPanelDlg::OnUpdateStatus)
//	ON_BN_CLICKED(IDC_RREL, &COpPanelDlg::OnBnClickedRrel)
ON_BN_CLICKED(IDC_RREL, &COpPanelDlg::OnBnClickedRrel)
ON_BN_CLICKED(IDC_BTN_MPREV, &COpPanelDlg::OnBnClickedBtnMprev)
ON_BN_CLICKED(IDC_BTN_MNEXT, &COpPanelDlg::OnBnClickedBtnMnext)
END_MESSAGE_MAP()


// COpPanelDlg 메시지 처리기입니다.


BOOL COpPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_iRefPlan = 0;
	ClearMark();
	UpdateData(false);
	SendMessage(WM_UPDATE_STATINFO);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL COpPanelDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			if (pMsg->wParam == VK_RETURN)
			{
				if (GetDlgItem(IDC_EDT_MPOS)->GetSafeHwnd() == pMsg->hwnd)
				{
					int pos = GetDlgItemInt(IDC_EDT_MPOS, NULL, 0);
					GoMarkPos(pos);
				}
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void COpPanelDlg::OnBnClickedBtnSetplan()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	tALMDRecord * pMeasureData;
	pMeasureData = CStatus::Instance()->GetCurMeasureData();
	CStatus::Instance()->m_dptRelPlan.dblPosX = pMeasureData->sRawRoll / 1000.;
	CStatus::Instance()->m_dptRelPlan.dblPosY = pMeasureData->sRawPitch / 1000.;

	UpdateData(FALSE);
	SendMessage(WM_UPDATE_STATINFO);
	//
}


void COpPanelDlg::OnBnClickedBtnMarkapply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	tMarkInfo MarkInfo;
	UpdateData();
	wsprintf(MarkInfo.szOperator, m_strMOP.GetBuffer());
	wsprintf(MarkInfo.szTool, m_strMTool.GetBuffer());
	wsprintf(MarkInfo.szStation, m_strMStation.GetBuffer());
	wsprintf(MarkInfo.szComment, m_strMComment.GetBuffer());
	if(CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		CStatus::Instance()->SetMarkToAdd(&MarkInfo);
	else{
		int iDispIndex = m_pRepository->m_iDispIndex;
		int nCount = m_pRepository->m_DataRecords.nCountOfRecords;
		if (nCount == 0)return;
		tALMDRecord *pMeaInfo = m_pRepository->GetMeasureInfo(m_pRepository->m_iDispIndex);
		pMeaInfo->MarkInfo = MarkInfo;
	}
}


void COpPanelDlg::OnBnClickedBtnMarkclear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClearMark();
}


void COpPanelDlg::ClearMark()
{
	CStatus::Instance()->ClearMarkToAdd();
	tMarkInfo MarkInfo = CStatus::Instance()->GetMarkToAdd();
	m_strMTool = MarkInfo.szTool;
	m_strMStation = MarkInfo.szStation;
	m_strMComment = MarkInfo.szComment;
	m_strMOP = MarkInfo.szOperator;
	UpdateData(FALSE);
}


void COpPanelDlg::OnBnClickedRrefplan()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if (m_iRefPlan)
		CStatus::Instance()->SetRelMode(true);
	else
		CStatus::Instance()->SetRelMode(false);
}

void COpPanelDlg::OnBnClickedRrel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if (m_iRefPlan)
		CStatus::Instance()->SetRelMode(true);
	else
		CStatus::Instance()->SetRelMode(false);

}


void COpPanelDlg::SetProgramMode(CStatus::tProgramMode ProgramMode)
{
	if (ProgramMode == CStatus::PM_MONITORING)
	{
		m_cLNOf.ShowWindow(SW_HIDE);
		m_cLTOT.ShowWindow(SW_HIDE);
		m_cEdtMPos.ShowWindow(SW_HIDE);
		m_cBtnMNext.ShowWindow(SW_HIDE);
		m_cBtnMPrev.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RREFPLAN)->EnableWindow(true);
		GetDlgItem(IDC_RREL)->EnableWindow(true);
		GetDlgItem(IDC_BTN_SETPLAN)->EnableWindow(true);
	}
	else
	{
		m_cLNOf.ShowWindow(SW_SHOW);
		m_cLTOT.ShowWindow(SW_SHOW);
		m_cEdtMPos.ShowWindow(SW_SHOW);
		m_cBtnMNext.ShowWindow(SW_SHOW);
		m_cBtnMPrev.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RREFPLAN)->EnableWindow(false);
		GetDlgItem(IDC_RREL)->EnableWindow(false);
		GetDlgItem(IDC_BTN_SETPLAN)->EnableWindow(false);
	}
}


LRESULT COpPanelDlg::OnPMChange(WPARAM wParam, LPARAM lParam)
{
	SetProgramMode(CStatus::Instance()->GetProgramMode());
	return 0;
}


void COpPanelDlg::OnBnClickedBtnMprev()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iDispIndex = m_pRepository->m_iDispIndex;
	int nCount = m_pRepository->m_DataRecords.nCountOfRecords;
	if (nCount == 0)return;
	if (iDispIndex == 0)return;
	m_pRepository->m_iDispIndex--;
	tALMDRecord *pMeaInfo = m_pRepository->GetMeasureInfo(m_pRepository->m_iDispIndex);
	CStatus::Instance()->SetRelMode(pMeaInfo->bRel);
	CStatus::Instance()->SetTolerance(pMeaInfo->dblGoNoGoValue);
	CStatus::Instance()->m_dptRelPlan.SetPoint(pMeaInfo->dblRefRoll, pMeaInfo->dblRefPitch);;
	CStatus::Instance()->SetMeasureData(pMeaInfo);
	SendMessage(WM_UPDATE_STATINFO);
}


void COpPanelDlg::OnBnClickedBtnMnext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iDispIndex = m_pRepository->m_iDispIndex;
	int nCount = m_pRepository->m_DataRecords.nCountOfRecords;
	if (nCount == 0)return;
	if ((iDispIndex + 1) == nCount)return;
	m_pRepository->m_iDispIndex++;
	tALMDRecord *pMeaInfo = m_pRepository->GetMeasureInfo(m_pRepository->m_iDispIndex);
	CStatus::Instance()->SetRelMode(pMeaInfo->bRel);	
	CStatus::Instance()->SetTolerance(pMeaInfo->dblGoNoGoValue);
	CStatus::Instance()->m_dptRelPlan.SetPoint(pMeaInfo->dblRefRoll, pMeaInfo->dblRefPitch);;
	CStatus::Instance()->SetMeasureData(pMeaInfo);
	SendMessage(WM_UPDATE_STATINFO);
}


void COpPanelDlg::GoMarkPos(int iPos)
{
	int nCount = m_pRepository->m_DataRecords.nCountOfRecords;
	if (nCount == 0)return;

	if (iPos<0 || iPos>(nCount - 1))return;

	m_pRepository->m_iDispIndex = iPos - 1;
	tALMDRecord *pMeaInfo = m_pRepository->GetMeasureInfo(m_pRepository->m_iDispIndex);
	CStatus::Instance()->SetRelMode(pMeaInfo->bRel);
	CStatus::Instance()->m_dptRelPlan.SetPoint(pMeaInfo->dblRefRoll, pMeaInfo->dblRefPitch);;
	CStatus::Instance()->SetMeasureData(pMeaInfo);
	SendMessage(WM_UPDATE_STATINFO);
}



LRESULT COpPanelDlg::OnUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	if (!m_pRepository)return 0;

	USES_CONVERSION;
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		int curPos = m_pRepository->m_iDispIndex;
		int nCount = m_pRepository->m_DataRecords.nCountOfRecords;
		tALMDRecord *pMeaInfo = m_pRepository->GetMeasureInfo(m_pRepository->m_iDispIndex);
		if (!pMeaInfo)return 0;
		m_iRefPlan = pMeaInfo->bRel;
		tMarkInfo *pMarkInfo = &(pMeaInfo->MarkInfo);
		CString strOperator = pMarkInfo->szOperator;
		CString strStation = pMarkInfo->szStation;
		CString strTool = pMarkInfo->szTool;
		CString strComment = pMarkInfo->szComment;
		CString strCurPos;
		strCurPos.Format(_T("%d"), curPos + 1);
		CString strCount;
		strCount.Format(_T("%d"), nCount);
		m_strMOP = strOperator;
		m_strMStation = strStation;
		m_strMTool = strTool;
		m_strMComment = strComment;
		m_cEdtMPos.SetWindowTextW(strCurPos);
		m_cLTOT.SetWindowTextW(strCount);
		m_cMOperator.SetWindowTextW(strOperator);
		m_cMStation.SetWindowTextW(strStation);
		m_cMTool.SetWindowTextW(strTool);
		m_cMComment.SetWindowTextW(strComment);
	}
	RefDispUpdate();
	UpdateData(false);
	UpdateData();
	if (m_iRefPlan)
		CStatus::Instance()->SetRelMode(true);
	else
		CStatus::Instance()->SetRelMode(false);

	return 0;
}


int COpPanelDlg::RefDispUpdate()
{
	m_dptRefDegree = CStatus::Instance()->m_dptRelPlan;

	CUnitConversion::Instance()->ConvertToDest(CStatus::Instance()->GetCoordinateSystem(), CStatus::Instance()->GetAngleType(), (CStatus::tIncType)CStatus::Instance()->GetInclinationType(), CStatus::Instance()->GetRORType(), &m_strXValueUnit, &(m_DispRefValue.dblPosX), m_dptRefDegree.dblPosX);
	CUnitConversion::Instance()->ConvertToDest(CStatus::Instance()->GetCoordinateSystem(), CStatus::Instance()->GetAngleType(), (CStatus::tIncType)CStatus::Instance()->GetInclinationType(), CStatus::Instance()->GetRORType(), &m_strYValueUnit, &(m_DispRefValue.dblPosY), m_dptRefDegree.dblPosY);
	if (CStatus::Instance()->IsDigit2Mode())
	{
		m_strRelRoll.Format(_T("%5.2f"), m_DispRefValue.dblPosX);
		m_strRelPitch.Format(_T("%5.2f"), m_DispRefValue.dblPosY);
	}
	else
	{
		m_strRelRoll.Format(_T("%5.3f"), m_DispRefValue.dblPosX);
		m_strRelPitch.Format(_T("%5.3f"), m_DispRefValue.dblPosY);
	}
	return 0;
}

