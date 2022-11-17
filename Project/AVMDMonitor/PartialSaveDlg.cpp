// PartialSaveDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "PartialSaveDlg.h"
#include "afxdialogex.h"
#include "Repository.h"
#include "Status.h"
#include "ProgressBarDlg.h"

// CPartialSaveDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPartialSaveDlg, CDialogEx)

int CPartialSaveDlg::m_iStartPartialNo = 0;
int CPartialSaveDlg::m_iEndPartialNo = 0;
CPartialSaveDlg::CPartialSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PARTIALSAVE, pParent)
	, m_nStep(0)
	//, m_iStartPartialNo(0)
	//, m_iEndPartialNo(0)
	, m_pRepository(NULL)
	, m_strLabelPartialSave(_T(""))
{

}

CPartialSaveDlg::~CPartialSaveDlg()
{
}

void CPartialSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LSELPOS, m_strLabelPartialSave);
}


BEGIN_MESSAGE_MAP(CPartialSaveDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CPartialSaveDlg::OnBnClickedBtnNext)
END_MESSAGE_MAP()


// CPartialSaveDlg 메시지 처리기입니다.


void CPartialSaveDlg::OnBnClickedBtnNext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnStepNext();
}


int CPartialSaveDlg::OnStepNext()
{
	m_pRepository = CStatus::Instance()->GetRepository();
	switch(m_nStep)
	{
	case 0://Start Pos 설정
		this->SetWindowTextW(_T("Set End Position"));
		m_iStartPartialNo = CStatus::Instance()->m_iStartPosText;
		m_strLabelPartialSave = _T("Move to the end position and push next button");
		UpdateData(FALSE);
		break;
	case 1://End Po 설정
		m_strLabelPartialSave = _T("To set the partial Log File Name, push next button");
		UpdateData(FALSE);
		m_iEndPartialNo = CStatus::Instance()->m_iStartPosText;
		break;
	case 2:	//Save
	{
		CFileDialog dlg(FALSE);
		if (dlg.DoModal() == IDOK)
		{
			CStatus::Instance()->m_iPartialSaveCount = m_iEndPartialNo - m_iStartPartialNo;
			CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::PRATICAL_SAVE);
			CStatus::Instance()->m_nProgressBarCount = 0;
			CProgressBarDlg *pProgressDlg = new CProgressBarDlg;
			CString strFilePath = dlg.GetPathName();
			AfxBeginThread(PartialSaveThread, &strFilePath);
			pProgressDlg->DoModal();
			//m_pRepository->ExportDataFile(dlg.GetPathName(), m_iStartPartialNo, m_iEndPartialNo);
		}
	}
	this->ShowWindow(SW_HIDE);
	break;
	case 3:
		break;
	}

	m_nStep++;
	m_nStep %= 4;
	return 0;
}


BOOL CPartialSaveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_strLabelPartialSave = _T("Move to the start position and push the next button");
	this->SetWindowTextW(_T("Set Start Position"));
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPartialSaveDlg::Init()
{
	m_nStep = 0;
	m_strLabelPartialSave = _T("Move to the start position and push the next button");
	this->SetWindowTextW(_T("Set Start Position"));
	UpdateData(FALSE);
}

UINT CPartialSaveDlg::PartialSaveThread(LPVOID lParam)
{
	//m_pRepository->ExportDataFile(dlg.GetPathName(), m_iStartPartialNo, m_iEndPartialNo);
	CString *strFilePath = (CString *)lParam;
	CStatus::Instance()->GetRepository()->ExportDataFile(*strFilePath, m_iStartPartialNo, m_iEndPartialNo);
	CStatus::Instance()->m_bIsThreadStop = FALSE;
	return 0;
}