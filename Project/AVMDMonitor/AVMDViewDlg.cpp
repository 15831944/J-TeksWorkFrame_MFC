// AVMDView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "AVMDViewDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "Repository.h"
#include "AVMDDefine.h"

// CAVMDViewDlg 대화 상자입니다.
#define INT_TIME 3
IMPLEMENT_DYNAMIC(CAVMDViewDlg, CDialogEx)

CAVMDViewDlg::CAVMDViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIEW, pParent)
	, m_iTemp(_T(""))
	, m_bPauseSave(FALSE)
	, m_strNextMarkName(_T(""))
	, m_strUserMarkName(_T(""))
	, m_strAddMark(_T(""))
	, m_nMarkPos(0)
	, m_nAutoMarkPos(1)
{
	CStatus::Instance()->SetAVMDViewDlg(this);
}

CAVMDViewDlg::~CAVMDViewDlg()
{
}

void CAVMDViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TEMPERATURE, m_iTemp);
	DDX_Control(pDX, IDC_PROGRESS_BATTERY, m_cBatt);
	DDX_Control(pDX, IDC_PRGCNT, m_cCntStat);
	DDX_Control(pDX, IDC_STATIC_TEMPERATURE, m_cTempStat);
	DDX_Control(pDX, IDC_BATTNUM, m_cBattNUM);
	DDX_Control(pDX, IDC_BTN_RECORD_START, m_btnStartPause);
	DDX_Control(pDX, IDC_CB_RECORDUNIT, m_RecUnit);
	DDX_Control(pDX, IDC_CB_RECORDTIME, m_RecTime);
	DDX_Text(pDX, IDC_EDIT6, m_strNextMarkName);
	DDX_Text(pDX, IDC_EDIT5, m_strUserMarkName);
	DDX_Control(pDX, IDC_PRG_REC, m_cRecPrgrs);
	DDX_Control(pDX, IDC_BTN_HOLD, m_btnHold);
}


BEGIN_MESSAGE_MAP(CAVMDViewDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RECORD_START, &CAVMDViewDlg::OnBnClickedBtnRecordStart)
	ON_BN_CLICKED(IDC_BTN_RECORD_FINISH, &CAVMDViewDlg::OnBnClickedBtnRecordFinish)
	ON_CBN_SELCHANGE(IDC_CB_RECORDUNIT, &CAVMDViewDlg::OnCbnSelchangeCbRecordunit)
	ON_MESSAGE(WM_RECORD_START, &CAVMDViewDlg::OnRecordStart)
	ON_MESSAGE(WM_RECORD_FINISH, &CAVMDViewDlg::OnRecordFinish)
	ON_MESSAGE(WM_UPDATE_BTN_HOLD, &CAVMDViewDlg::UpdateHoldButton)
	ON_BN_CLICKED(IDC_BTN_ADDMARK, &CAVMDViewDlg::OnBnClickedBtnAddmark)
	ON_BN_CLICKED(IDC_BTN_HOLD, &CAVMDViewDlg::OnBnClickedBtnHold)
END_MESSAGE_MAP()


// CAVMDViewDlg 메시지 처리기입니다.


BOOL CAVMDViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cCntStat.Set(FALSE);
	m_cBatt.SetRange(0, 100);
	SetTimer(1, 300, NULL);

	CString strNum;
	SetIntTimeCnt(60);
	m_RecUnit.AddString(_T("seconds"));
	m_RecUnit.AddString(_T("minutes"));
	m_RecUnit.AddString(_T("hours"));

	m_RecTime.SetCurSel(0);
	m_RecUnit.SetCurSel(0);
	m_cRecPrgrs.SetPos(0);
	m_cRecPrgrs.SetRange(0, 100);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAVMDViewDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int PrgrsPos = 0;
	switch (nIDEvent)
	{
	case 1:
		m_cCntStat.Set(FALSE);
		//CStatus::Instance()->UpdateScrn();
		break;
	case INT_TIME:
		PrgrsPos = CStatus::Instance()->GetRepository()->m_nCountOfWrite;
		if (CStatus::Instance()->m_bSavePreRecData)
		{
			if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::ONE_THOUSAND)
			{
				if (PrgrsPos <= CStatus::Instance()->GetPreRecordTime())
					PrgrsPos = 0;
				else PrgrsPos -= CStatus::Instance()->GetPreRecordTime();
			}
			if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::Five_HUNDRED)
			{
				if (PrgrsPos <= CStatus::Instance()->GetPreRecordTime() / 2)PrgrsPos = 0;
				else PrgrsPos -= CStatus::Instance()->GetPreRecordTime() / 2;
			}
		}
		m_cRecPrgrs.SetPos(PrgrsPos);
		if (CStatus::Instance()->GetRepository()->IsSaveTargetIndex())
		{
			OnBnClickedBtnRecordFinish();
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

int CAVMDViewDlg::SetSensorState(int iTemp, int nBatt)
{
	CString strTemp;
	strTemp.Format(_T("%d"), iTemp);
	m_cTempStat.SetWindowTextW(strTemp.GetBuffer());
	m_cBatt.SetPos(nBatt);
	CString strBattNum;
	strBattNum.Format(_T("%d%%"), nBatt);
	m_cBattNUM.SetWindowTextW(strBattNum.GetBuffer());
	m_cCntStat.Set(TRUE);
	SetTimer(1, 300, NULL);
	return 0;
}

void CAVMDViewDlg::OnBnClickedBtnRecordStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStatus::Instance()->m_bSavePreRecData = TRUE;
	RecStartNPause();
}

void CAVMDViewDlg::OnBnClickedBtnRecordFinish()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	RecTerminate();
	CStatus::Instance()->m_bSavePreRecData = FALSE;
}

void CAVMDViewDlg::OnCbnSelchangeCbRecordunit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_RecUnit.GetCurSel() == 2)
	{
		SetIntTimeCnt(4);
	}
	else
	{
		SetIntTimeCnt(60);
	}
	m_RecTime.SetCurSel(0);
}

void CAVMDViewDlg::SetIntTimeCnt(int nNum)
{
	CString strNum;
	m_RecTime.ResetContent();
	for (int i = 1; i < (nNum+1); i++)
	{
		strNum.Format(_T("%d"), i);
		m_RecTime.AddString(strNum);
	}
}

int CAVMDViewDlg::GetIntTime()
{
	return  (int)(1000 * (m_RecTime.GetCurSel()+1) *pow(60, m_RecUnit.GetCurSel()));
}

void CAVMDViewDlg::RecStartNPause()
{
	static int bPause = FALSE;
	m_RecTime.EnableWindow(FALSE);
	m_RecUnit.EnableWindow(FALSE);

	switch (GetFileSaveStatus())
	{
	case CRepository::SS_WRITE://Pause상태로 만듦
		CStatus::Instance()->GetRepository()->PauseSave();
		m_btnStartPause.SetWindowTextW(_T("Restart"));
		break;
	case CRepository::SS_PAUSE://Pause->Write상태로 만듦
		m_btnStartPause.SetWindowTextW(_T("Pause"));
		m_strAddMark = _T("Restart");
		CStatus::Instance()->GetRepository()->ReStartSave();
		break;
	case CRepository::SS_STOP:
		//최초 Save시작 STOP->Write임
		RecdNewStart();
		break;
	}
}

void CAVMDViewDlg::RecdNewStart()
{
	int iStartIndex = CStatus::Instance()->GetRepository()->GetLastIndex();
	int nCount = GetIntTime();  
	int nFirstCount = 0;
	int iPreRecTime = 0;
	if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::ONE_THOUSAND)
		nCount = nCount;
		
	if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::Five_HUNDRED)
		nCount = nCount / 2;

	nFirstCount = nCount;
	if (CStatus::Instance()->m_bSavePreRecData)
	{   
		int iStartIndex2 = iStartIndex;
		iPreRecTime = CStatus::Instance()->GetPreRecordTime();

		if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::ONE_THOUSAND)
			iPreRecTime= iPreRecTime; 
		if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::Five_HUNDRED)
			iPreRecTime = iPreRecTime / 2;

		if (iPreRecTime >= iStartIndex)iStartIndex = 0;
		else iStartIndex = iStartIndex - iPreRecTime;
		nCount += (iStartIndex2 - iStartIndex);
	}
	m_cRecPrgrs.SetRange32(0, nFirstCount);
	SetMarkPos(0);
	SetAutoMarkPos(1);
	CStatus::Instance()->GetRepository()->m_nCountOfWrite = 0;
	m_cRecPrgrs.SetPos(0);
	CStatus::Instance()->GetRepository()->m_iCSVDispIndexNo = 0;
	CStatus::Instance()->GetRepository()->BeginSave(iStartIndex, nCount);
	m_btnStartPause.SetWindowTextW(_T("Pause"));
	SetTimer(INT_TIME, 100, NULL);
}

void CAVMDViewDlg::RecTerminate()
{
	printf("\nRecTerminate 1");
	m_RecTime.EnableWindow(TRUE);
	m_RecUnit.EnableWindow(TRUE);
	printf("\nRecTerminate 2");
	CStatus::Instance()->GetRepository()->StopSave();
	m_btnStartPause.SetWindowTextW(_T("Start"));
	printf("\nRecTerminate 3");
	CStatus::Instance()->GetRepository()->TerminateFileSaveThread();
	KillTimer(INT_TIME);
	printf("\nRecTerminate 4");
}

int CAVMDViewDlg::GetFileSaveStatus()
{
	return (int)(CStatus::Instance()->GetRepository()->GetFileSaveStatus());
}

LRESULT CAVMDViewDlg::OnRecordStart(WPARAM wParam, LPARAM lParam)
{
	CStatus::Instance()->m_bSavePreRecData = FALSE;
	RecdNewStart();
	return 0;
}

LRESULT CAVMDViewDlg::OnRecordFinish(WPARAM wParam, LPARAM lParam)
{
	RecTerminate();
	return 0;
}

void CAVMDViewDlg::OnBnClickedBtnAddmark()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (GetFileSaveStatus() != CRepository::SS_WRITE)return;
	m_strAddMark = _T("");
	int preLoadMarkCount = m_lstMark.GetCount();
	UpdateData(TRUE);
	if (!m_strUserMarkName.IsEmpty())
	{
		m_strAddMark = m_strUserMarkName;
	}
	else
	{
		if ((preLoadMarkCount != 0) && (m_lstMark.GetCount() > GetMarkPos()))
		{
			m_strAddMark = m_lstMark.GetAt(GetMarkPos());
			if (m_lstMark.GetCount() > GetMarkPos() + 1)
				m_strNextMarkName = m_lstMark.GetAt(GetMarkPos() + 1);
			else
				m_strNextMarkName = _T("");
		}
		if (m_strAddMark.IsEmpty())
		{
			m_strAddMark.Format(_T("Mark%d"), GetAutoMarkPos());
			IncAutoMarkPos();
		}
		IncMarkPos();
	}
	m_strUserMarkName = _T("");
	
	UpdateData(FALSE);
	CStatus::Instance()->GetRepository()->GetDataCount();
}

void CAVMDViewDlg::SetMarkList(CListBox *pList)
{
	CString strMark;
	m_lstMark.RemoveAll();
	int nCount = pList->GetCount();
	if (!nCount)return;
	for (int i = 0; i < nCount; i++)
	{
		pList->GetText(i, strMark);
		m_lstMark.Add(strMark);
	}
	pList->GetText(0, strMark);
	m_strNextMarkName = strMark;
	UpdateData(FALSE);
	SetMarkPos(0);
}

int CAVMDViewDlg::GetMarkPos()
{
	return m_nMarkPos;
}

int CAVMDViewDlg::GetAutoMarkPos()
{
	return m_nAutoMarkPos;
}

void CAVMDViewDlg::SetAutoMarkPos(int nPos)
{
	m_nAutoMarkPos = nPos;
}

void CAVMDViewDlg::IncAutoMarkPos()
{
	m_nAutoMarkPos++;
}

void CAVMDViewDlg::IncMarkPos()
{
	m_nMarkPos++;
}

void CAVMDViewDlg::SetMarkPos(int nPos)
{
	m_nMarkPos = nPos;
}

CString CAVMDViewDlg::GetMark()
{
	CString strSend;
	strSend = m_strAddMark;
	m_strAddMark = _T("");
	return strSend;
}

void CAVMDViewDlg::OnBnClickedBtnHold()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strButtonStatus;
	
	UpdateData(TRUE);
	m_btnHold.GetWindowTextW(strButtonStatus);
	if (strButtonStatus == "Hold")
	{
		CStatus::Instance()->SetIsHoldCondition(TRUE);
		m_btnHold.SetWindowTextW(_T("Release"));
	}
	if (strButtonStatus == "Release")
	{
		CStatus::Instance()->SetIsHoldCondition(FALSE);
		m_btnHold.SetWindowTextW(_T("Hold"));
	}
	UpdateData(FALSE);
}

LRESULT CAVMDViewDlg::UpdateHoldButton(WPARAM wParam, LPARAM lParam)
{
	m_btnHold.SetWindowTextW(_T("Hold"));
	CStatus::Instance()->SetIsHoldCondition(FALSE);
	return 0;
}

BOOL CAVMDViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


