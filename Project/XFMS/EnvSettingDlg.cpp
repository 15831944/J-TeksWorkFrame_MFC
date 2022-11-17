// EnvSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XFMS.h"
#include "EnvSettingDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "ComSetDlg.h"
#include "XFMSDoc.h"
#include "XFMSView.h"
#include "MainFrm.h"
#include "CamDlg.h"

// CEnvSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEnvSettingDlg, CDialogEx)

CEnvSettingDlg::CEnvSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ENVSETTING, pParent)
	, m_strModelRepository(_T(""))
	, m_strDataFileName(_T(""))
	, m_ulCountOfCam(1)
	, m_strIMGSaveFolder(_T(""))
	, m_ulCountOfSensors(2)
	, m_nDaysDelImg(0)
	, m_ulGraphXRange(0)
	, m_ulGraphYMax(0)
	, m_ulGraphYMin(0)
	, m_iDispMode(0)
	, m_strAVISaveFolder(_T(""))
{
	for (int i = 0; i < 6; i++)
		m_strSensorNamePx[i].Format(_T("P%d"), i + 1);
}

CEnvSettingDlg::~CEnvSettingDlg()
{
}

void CEnvSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_SAVEFOLDER, m_strModelRepository);
	DDX_Text(pDX, IDC_EDT_DFNAME, m_strDataFileName);
	DDX_Text(pDX, IDC_EDT_IMGFOLDER, m_strIMGSaveFolder);
	DDX_Control(pDX, IDC_SPINPOS, m_spCountSensors);
	DDX_Text(pDX, IDC_COUNTSENSORS, m_ulCountOfSensors);
	DDX_Control(pDX, IDC_SP_DELOLDFILE, m_spDelOldFile);
	DDX_Text(pDX, IDC_EDT_DELDAY, m_nDaysDelImg);
	DDX_Control(pDX, IDC_BTN_COMSET, m_cCOMSet);
	DDX_Control(pDX, IDC_BTNAPPLY, m_cApply);
	DDX_Control(pDX, IDOK, m_cSave);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
	DDX_Control(pDX, IDC_IP_TEMPSENSOR, m_cIPTempSensorAddr);
	DDX_Control(pDX, IDC_BTN_CALSENSOR, m_cCalSensor);
	DDX_Text(pDX, IDC_EDT_XRANGE, m_ulGraphXRange);
	DDV_MinMaxUInt(pDX, m_ulGraphXRange, 60, 28800);
	DDX_Text(pDX, IDC_EDT_YMAX, m_ulGraphYMax);
	DDX_Text(pDX, IDC_EDT_YMIN, m_ulGraphYMin);
	DDX_Radio(pDX, IDC_RSELDISPLAY, m_iDispMode);
	DDX_Text(pDX, IDC_EDT_AVIFOLDER, m_strAVISaveFolder);
	DDX_Text(pDX, IDC_EDT_P1NAME, m_strSensorNamePx[0]);
	DDX_Text(pDX, IDC_EDT_P2NAME, m_strSensorNamePx[1]);
	DDX_Text(pDX, IDC_EDT_P3NAME, m_strSensorNamePx[2]);
	DDX_Text(pDX, IDC_EDT_P4NAME, m_strSensorNamePx[3]);
	DDX_Text(pDX, IDC_EDT_P5NAME, m_strSensorNamePx[4]);
	DDX_Text(pDX, IDC_EDT_P6NAME, m_strSensorNamePx[5]);
	DDV_MaxChars(pDX, m_strSensorNamePx[0], 3);
	DDV_MaxChars(pDX, m_strSensorNamePx[1], 3);
	DDV_MaxChars(pDX, m_strSensorNamePx[2], 3);
	DDV_MaxChars(pDX, m_strSensorNamePx[3], 3);
	DDV_MaxChars(pDX, m_strSensorNamePx[4], 3);
	DDV_MaxChars(pDX, m_strSensorNamePx[5], 3);
}


BEGIN_MESSAGE_MAP(CEnvSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SETFOLDER, &CEnvSettingDlg::OnBnClickedBtnSetfolder)
	ON_BN_CLICKED(IDOK, &CEnvSettingDlg::OnBnClickedOk)
//	ON_WM_PAINT()
ON_BN_CLICKED(IDOK, &CEnvSettingDlg::OnBnClickedOk)
ON_BN_CLICKED(IDC_BTN_COMSET, &CEnvSettingDlg::OnBnClickedBtnComset)
ON_BN_CLICKED(IDC_BTNAPPLY, &CEnvSettingDlg::OnBnClickedBtnapply)
ON_BN_CLICKED(IDCANCEL, &CEnvSettingDlg::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BTN_SETIMGFOLDER, &CEnvSettingDlg::OnBnClickedBtnSetIMGfolder)
ON_BN_CLICKED(IDC_BTN_CALSENSOR, &CEnvSettingDlg::OnBnClickedBtnCalsensor)
ON_BN_CLICKED(IDC_BTN_SETAVIFOLDER, &CEnvSettingDlg::OnBnClickedBtnSetavifolder)
END_MESSAGE_MAP()


// CEnvSettingDlg 메시지 처리기입니다.


void CEnvSettingDlg::OnBnClickedBtnSetfolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFolderDialog FolderDlg(_T("Select the job file folder."), CStatus::Instance()->GetModelRepository().GetBuffer());
	CString SaveFolder;
	switch(FolderDlg.DoModal())
	{
	case IDOK:
		SaveFolder = FolderDlg.GetFolderPath();
		m_strModelRepository = SaveFolder;
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
	m_strModelRepository = CStatus::Instance()->GetModelRepository();
	m_strDataFileName = CStatus::Instance()->GetModelName();
	m_strIMGSaveFolder = CStatus::Instance()->m_strIMGSaveFolder;
	m_strAVISaveFolder = CStatus::Instance()->m_strAVISaveFolder;

	m_spCountSensors.SetRange(2, 6);
	m_ulCountOfCam = CStatus::Instance()->m_nCountOfCam;
	
	for (int i = 0; i < 6; i++)
		m_strSensorNamePx[i] = CStatus::Instance()->m_strSensorNamePx[i];

	m_cIPTempSensorAddr.SetAddress(CStatus::Instance()->m_lIPAddr[0], CStatus::Instance()->m_lIPAddr[1], CStatus::Instance()->m_lIPAddr[2], CStatus::Instance()->m_lIPAddr[3]);
	m_ulCountOfSensors = CStatus::Instance()->m_nCountOfSensors;
	m_nDaysDelImg = CStatus::Instance()->m_nDaysDel;
	m_ulGraphXRange = CStatus::Instance()->m_ulGraphXRange;
	m_ulGraphYMax = CStatus::Instance()->m_ulGraphYMax;
	m_ulGraphYMin = CStatus::Instance()->m_ulGraphYMin;
	m_iDispMode = CStatus::Instance()->m_ulDispMode;

	m_spDelOldFile.SetRange(1, 365);

	CheckDlgButton(IDC_CHK_DELOLDFILE, CStatus::Instance()->m_bUseDelOldImg);
	CheckDlgButton(IDC_CHK_SAVETESTIMG, CStatus::Instance()->m_bSaveTestImage);
	CheckDlgButton(IDC_CHK_SCRNCROSS, CStatus::Instance()->m_bDrawCross);
	CheckDlgButton(IDC_CHK_IMGENHANCE, CStatus::Instance()->m_bUseImgEnhance);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEnvSettingDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CStatus::Instance()->m_nCountOfCam = m_ulCountOfCam;
	CStatus::Instance()->SetModelRepository(m_strModelRepository.GetBuffer());
	CStatus::Instance()->SetModelName(m_strDataFileName.GetBuffer());
	CStatus::Instance()->m_strIMGSaveFolder = m_strIMGSaveFolder;
	CStatus::Instance()->m_strAVISaveFolder = m_strAVISaveFolder;

	CStatus::Instance()->m_nCountOfSensors = m_ulCountOfSensors;
	CStatus::Instance()->m_bSaveTestImage = IsDlgButtonChecked(IDC_CHK_SAVETESTIMG);
	CStatus::Instance()->m_bDrawCross = IsDlgButtonChecked(IDC_CHK_SCRNCROSS);
	CStatus::Instance()->m_bUseDelOldImg = IsDlgButtonChecked(IDC_CHK_DELOLDFILE);
	CStatus::Instance()->m_bUseImgEnhance = IsDlgButtonChecked(IDC_CHK_IMGENHANCE);
	CStatus::Instance()->m_nDaysDel = m_nDaysDelImg;
	CStatus::Instance()->m_ulGraphXRange = m_ulGraphXRange;
	CStatus::Instance()->m_ulGraphYMax = m_ulGraphYMax;
	CStatus::Instance()->m_ulGraphYMin = m_ulGraphYMin;
	CStatus::Instance()->m_ulDispMode = m_iDispMode;

	BYTE byField[4];

	m_cIPTempSensorAddr.GetAddress(byField[0], byField[1], byField[2], byField[3]);

	for (int i = 0; i < 4; i++)
	{
		CStatus::Instance()->m_lIPAddr[i] = byField[i];
	}

	for (int i = 0; i < 6; i++)
	{
		CStatus::Instance()->m_strSensorNamePx[i] = m_strSensorNamePx[i];
	}

	CStatus::Instance()->SaveCFG();
	CDialogEx::OnOK();
}

void CEnvSettingDlg::OnBnClickedBtnComset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CComSetDlg dlg;
	dlg.DoModal();
}


void CEnvSettingDlg::OnBnClickedBtnapply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	int nCountOfCam = CStatus::Instance()->m_nCountOfCam;

}


void CEnvSettingDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_strModelRepository = CStatus::Instance()->GetModelRepository();
	m_strDataFileName = CStatus::Instance()->GetModelName();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	m_ulCountOfCam = CStatus::Instance()->m_nCountOfCam;

	CDialogEx::OnCancel();
}


void CEnvSettingDlg::OnBnClickedBtnSetIMGfolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFolderDialog FolderDlg(_T("Select the Image folder."), CStatus::Instance()->GetModelRepository().GetBuffer());
	CString SaveFolder;
	switch (FolderDlg.DoModal())
	{
	case IDOK:
		SaveFolder = FolderDlg.GetFolderPath();
		m_strIMGSaveFolder = SaveFolder;
		UpdateData(FALSE);
		break;
	case IDCANCEL:
		break;
	}


}

void CEnvSettingDlg::OnBnClickedBtnSetavifolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFolderDialog FolderDlg(_T("Select the AVI folder."), CStatus::Instance()->GetModelRepository().GetBuffer());
	CString SaveFolder;
	switch (FolderDlg.DoModal())
	{
	case IDOK:
		SaveFolder = FolderDlg.GetFolderPath();
		m_strAVISaveFolder = SaveFolder;
		UpdateData(FALSE);
		break;
	case IDCANCEL:
		break;
	}

}


void CEnvSettingDlg::OnBnClickedBtnCalsensor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

