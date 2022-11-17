// EnvSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "EnvSettingDlg.h"
#include "afxdialogex.h"
#include "Status.h"

// CEnvSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEnvSettingDlg, CDialogEx)

CEnvSettingDlg::CEnvSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ENVSETTING, pParent)
	, m_strSaveFolder(_T(""))
	, m_strDataFileName(_T(""))
{

}

CEnvSettingDlg::~CEnvSettingDlg()
{
}

void CEnvSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_SAVEFOLDER, m_strSaveFolder);
	DDX_Text(pDX, IDC_EDT_DFNAME, m_strDataFileName);
}


BEGIN_MESSAGE_MAP(CEnvSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SETFOLDER, &CEnvSettingDlg::OnBnClickedBtnSetfolder)
	ON_BN_CLICKED(IDOK, &CEnvSettingDlg::OnBnClickedOk)
//	ON_WM_PAINT()
END_MESSAGE_MAP()


// CEnvSettingDlg 메시지 처리기입니다.


void CEnvSettingDlg::OnBnClickedBtnSetfolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFolderDialog FolderDlg(_T("Select the log data save folder."), CStatus::Instance()->GetDataSaveFolder().GetBuffer());
	CString SaveFolder;
	switch(FolderDlg.DoModal())
	{
	case IDOK:
		SaveFolder = FolderDlg.GetFolderPath();
		m_strSaveFolder = SaveFolder;
		UpdateData(FALSE);
		break;
	case IDCANCEL:
		break;
	}


}


BOOL CEnvSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_strSaveFolder = CStatus::Instance()->GetDataSaveFolder();
	m_strDataFileName = CStatus::Instance()->GetDataFileName();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEnvSettingDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CStatus::Instance()->SetDataSaveFolder(m_strSaveFolder.GetBuffer());
	CStatus::Instance()->SetDataFileName(m_strDataFileName.GetBuffer());
	CStatus::Instance()->SetDefault();
	CDialogEx::OnOK();
}

