// EnvSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "EnvSettingDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "IOSetDlg.h"
#include "ComSetDlg.h"
#include "LightSetDlg.h"
#include "SCInspectorDoc.h"
#include "SCInspectorView.h"
#include "MainFrm.h"
#include "CamDlg.h"

// CEnvSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEnvSettingDlg, CDialogEx)

CEnvSettingDlg::CEnvSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ENVSETTING, pParent)
	, m_strModelRepository(_T(""))
	, m_strDataFileName(_T(""))
	, m_ulCountOfCam(0)
	, m_ulCountOfStage(1)
	, m_strNGSaveFolder(_T(""))
	, m_ulCountOfPos(0)
	, m_nDaysDelImg(0)
	, m_ulCountOfCamRemote(0)
	, m_strRemoteCamFolder(_T(""))
{

}

CEnvSettingDlg::~CEnvSettingDlg()
{
}

void CEnvSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_SAVEFOLDER, m_strModelRepository);
	DDX_Text(pDX, IDC_EDT_DFNAME, m_strDataFileName);
	DDX_Control(pDX, IDC_SPINCAM, m_spCountOfCam);
	DDX_Text(pDX, IDC_INCAMNO, m_ulCountOfCam);
	DDX_Text(pDX, IDC_STAGENO, m_ulCountOfStage);
	DDX_Control(pDX, IDC_SELCAM1, m_cbCam1);
	DDX_Control(pDX, IDC_SELCAM2, m_cbCam2);
	DDX_Control(pDX, IDC_SELCAM3, m_cbCam3);
	DDX_Control(pDX, IDC_SELCAM4, m_cbCam4);
	DDX_Control(pDX, IDC_CAM1FOVX, m_cCam1FovX);
	DDX_Control(pDX, IDC_CAM1FOVY, m_cCam1FovY);
	DDX_Control(pDX, IDC_CAM2FOVX, m_Cam2FovX);
	DDX_Control(pDX, IDC_CAM2FOVY, m_cCam2FovY);
	DDX_Control(pDX, IDC_CAM3FOVX, m_cCam3FovX);
	DDX_Control(pDX, IDC_CAM3FOVY, m_cCam3FovY);
	DDX_Control(pDX, IDC_CAM4FOVX, m_cCam4FovX);
	DDX_Control(pDX, IDC_CAM4FOVY, m_cCam4FovY);
	DDX_Text(pDX, IDC_CAM1FOVX, m_dblCamFov[0].dblCX);
	DDV_MinMaxDouble(pDX, m_dblCamFov[0].dblCX, 0, 100);
	DDX_Text(pDX, IDC_CAM1FOVY, m_dblCamFov[0].dblCY);
	DDV_MinMaxDouble(pDX, m_dblCamFov[0].dblCY, 0, 100);
	DDX_Text(pDX, IDC_CAM2FOVX, m_dblCamFov[1].dblCX);
	DDV_MinMaxDouble(pDX, m_dblCamFov[1].dblCX, 0, 100);
	DDX_Text(pDX, IDC_CAM2FOVY, m_dblCamFov[1].dblCY);
	DDV_MinMaxDouble(pDX, m_dblCamFov[1].dblCY, 0, 100);
	DDX_Text(pDX, IDC_CAM3FOVX, m_dblCamFov[2].dblCX);
	DDV_MinMaxDouble(pDX, m_dblCamFov[1].dblCX, 0, 100);
	DDX_Text(pDX, IDC_CAM3FOVY, m_dblCamFov[2].dblCY);
	DDV_MinMaxDouble(pDX, m_dblCamFov[1].dblCY, 0, 100);
	DDX_Text(pDX, IDC_CAM4FOVX, m_dblCamFov[3].dblCX);
	DDV_MinMaxDouble(pDX, m_dblCamFov[1].dblCX, 0, 100);
	DDX_Text(pDX, IDC_CAM4FOVY, m_dblCamFov[3].dblCY);
	DDV_MinMaxDouble(pDX, m_dblCamFov[1].dblCY, 0, 100);
	DDX_Control(pDX, IDC_CAM1EXP, m_cCam1Exp);
	DDX_Control(pDX, IDC_CAM2EXP, m_cCam2Exp);
	DDX_Control(pDX, IDC_CAM3EXP, m_cCam3Exp);
	DDX_Control(pDX, IDC_CAM4EXP, m_cCam4Exp);
	DDX_Text(pDX, IDC_CAM1EXP, m_uiCamExpTime[0]);
	DDV_MinMaxUInt(pDX, m_uiCamExpTime[0], 20, 10000);
	DDX_Text(pDX, IDC_CAM2EXP, m_uiCamExpTime[1]);
	DDV_MinMaxUInt(pDX, m_uiCamExpTime[1], 20, 10000);
	DDX_Text(pDX, IDC_CAM3EXP, m_uiCamExpTime[2]);
	DDV_MinMaxUInt(pDX, m_uiCamExpTime[2], 20, 10000);
	DDX_Text(pDX, IDC_CAM4EXP, m_uiCamExpTime[3]);
	DDV_MinMaxUInt(pDX, m_uiCamExpTime[3], 20, 10000);
	DDX_Text(pDX, IDC_EDT_NGFOLDER, m_strNGSaveFolder);
	DDX_Control(pDX, IDC_SPINSTAGE, m_spCountOfStage);
	DDX_Control(pDX, IDC_SPINPOS, m_spCountPOS);
	DDX_Text(pDX, IDC_POSCOUNT, m_ulCountOfPos);
	DDX_Control(pDX, IDC_SP_DELOLDFILE, m_spDelOldFile);
	DDX_Text(pDX, IDC_EDT_DELDAY, m_nDaysDelImg);
	DDX_Control(pDX, IDC_BTN_IOSET, m_cIOTest);
	DDX_Control(pDX, IDC_BTN_LIGHT, m_cLight);
	DDX_Control(pDX, IDC_BTN_COMSET, m_cCOMSet);
	DDX_Control(pDX, IDC_BTNAPPLY, m_cApply);
	DDX_Control(pDX, IDOK, m_cSave);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
	DDX_Control(pDX, IDC_SPINREMOTECAM, m_spCountOfRemoteCam);
	DDX_Text(pDX, IDC_EDT_NREMOTECAM, m_ulCountOfCamRemote);
	DDX_Text(pDX, IDC_EDT_RMTCAMFOLDER, m_strRemoteCamFolder);
	DDX_Control(pDX, IDC_IP_REMOTE, m_cIPRemoteAddr);
}


BEGIN_MESSAGE_MAP(CEnvSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SETFOLDER, &CEnvSettingDlg::OnBnClickedBtnSetfolder)
	ON_BN_CLICKED(IDOK, &CEnvSettingDlg::OnBnClickedOk)
//	ON_WM_PAINT()
ON_BN_CLICKED(IDC_BTN_IOSET, &CEnvSettingDlg::OnBnClickedBtnIoset)
ON_BN_CLICKED(IDOK, &CEnvSettingDlg::OnBnClickedOk)
ON_BN_CLICKED(IDC_BTN_IOMAP, &CEnvSettingDlg::OnBnClickedBtnIomap)
ON_BN_CLICKED(IDC_BTN_LIGHT, &CEnvSettingDlg::OnBnClickedBtnLight)
ON_BN_CLICKED(IDC_BTN_COMSET, &CEnvSettingDlg::OnBnClickedBtnComset)
ON_BN_CLICKED(IDC_BTNAPPLY, &CEnvSettingDlg::OnBnClickedBtnapply)
ON_BN_CLICKED(IDCANCEL, &CEnvSettingDlg::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BTN_SETNGFOLDER, &CEnvSettingDlg::OnBnClickedBtnSetngfolder)
ON_BN_CLICKED(IDC_BTN_SETNREMOTEFOLDER, &CEnvSettingDlg::OnBnClickedBtnSetnremotefolder)
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
	m_strNGSaveFolder = CStatus::Instance()->m_strNGSaveFolder;
	m_strRemoteCamFolder = CStatus::Instance()->m_strRemoteCamFolder;
	m_spCountOfCam.SetRange(2, 3);
	m_spCountOfRemoteCam.SetRange(0, 2);
	m_spCountOfStage.SetRange(1, 2);
	m_spCountPOS.SetRange(5, 6);
	m_ulCountOfCam = CStatus::Instance()->m_nCountOfCam;
	m_ulCountOfCamRemote = CStatus::Instance()->m_nCountOfCamRemote;
	
	m_cIPRemoteAddr.SetAddress(CStatus::Instance()->m_lIPAddr[0], CStatus::Instance()->m_lIPAddr[1], CStatus::Instance()->m_lIPAddr[2], CStatus::Instance()->m_lIPAddr[3]);
	CheckDlgButton(IDC_CHK_CAMRMT, CStatus::Instance()->m_bUseRemoteCamUpdate);
	m_ulCountOfStage = CStatus::Instance()->m_nCountOfStage;
	m_ulCountOfPos = CStatus::Instance()->m_nCountOfPos;
	m_nDaysDelImg = CStatus::Instance()->m_nDaysDel;
	CString strNo;
	for (int i = 0; i < 3; i++)
	{
		strNo.Format(_T("Index %d"),i+1);
		m_cbCam1.AddString(strNo);
		m_cbCam2.AddString(strNo);
		m_cbCam3.AddString(strNo);
		m_cbCam4.AddString(strNo);
	}
	m_cbCam1.SetCurSel(CStatus::Instance()->m_camInspStage[0]);
	m_cbCam2.SetCurSel(CStatus::Instance()->m_camInspStage[1]);
	m_cbCam3.SetCurSel(CStatus::Instance()->m_camInspStage[2]);
	m_cbCam4.SetCurSel(CStatus::Instance()->m_camInspStage[3]);
	m_spDelOldFile.SetRange(1, 365);

	

	for (int i = 0; i < 4; i++)
	{
		m_dblCamFov[i] = CStatus::Instance()->m_CamFov[i];
		m_uiCamExpTime[i] = CStatus::Instance()->m_lCamExpTime[i];
	}

	CheckDlgButton(IDC_CHK_DELOLDFILE, CStatus::Instance()->m_bUseDelOldImg);
	CheckDlgButton(IDC_CHK_SAVETESTIMG, CStatus::Instance()->m_bSaveTestImage);
	CheckDlgButton(IDC_CHK_SCRNCROSS, CStatus::Instance()->m_bDrawCross);
	CheckDlgButton(IDC_CHK_SAVEALL, CStatus::Instance()->m_bSaveAllImage);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEnvSettingDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CStatus::Instance()->m_nCountOfCam = m_ulCountOfCam;
	CStatus::Instance()->m_nCountOfCamRemote = m_ulCountOfCamRemote;
	CStatus::Instance()->m_nCountOfStage = m_ulCountOfStage;
	CStatus::Instance()->m_camInspStage[0] = m_cbCam1.GetCurSel();
	CStatus::Instance()->m_camInspStage[1] = m_cbCam2.GetCurSel();
	CStatus::Instance()->m_camInspStage[2] = m_cbCam3.GetCurSel();
	CStatus::Instance()->m_camInspStage[3] = m_cbCam4.GetCurSel();
	CStatus::Instance()->SetModelRepository(m_strModelRepository.GetBuffer());
	CStatus::Instance()->m_strRemoteCamFolder = m_strRemoteCamFolder;
	CStatus::Instance()->SetModelName(m_strDataFileName.GetBuffer());
	CStatus::Instance()->m_strNGSaveFolder = m_strNGSaveFolder;
	CStatus::Instance()->m_nCountOfPos = m_ulCountOfPos;
	CStatus::Instance()->m_bSaveTestImage = IsDlgButtonChecked(IDC_CHK_SAVETESTIMG);
	CStatus::Instance()->m_bDrawCross = IsDlgButtonChecked(IDC_CHK_SCRNCROSS);
	CStatus::Instance()->m_bUseDelOldImg = IsDlgButtonChecked(IDC_CHK_DELOLDFILE);
	CStatus::Instance()->m_nDaysDel = m_nDaysDelImg;
	CStatus::Instance()->m_bSaveAllImage = IsDlgButtonChecked(IDC_CHK_SAVEALL);

	for (int i = 0; i < 4; i++)
	{
		CStatus::Instance()->m_CamFov[i] = m_dblCamFov[i];
		CStatus::Instance()->m_lCamExpTime[i] = m_uiCamExpTime[i];
	}

	BYTE byField[4];

	m_cIPRemoteAddr.GetAddress(byField[0], byField[1], byField[2], byField[3]);

	for (int i = 0; i < 4; i++)
	{
		CStatus::Instance()->m_lIPAddr[i] = byField[i];
	}

	CStatus::Instance()->m_bUseRemoteCamUpdate = IsDlgButtonChecked(IDC_CHK_CAMRMT);

	CStatus::Instance()->SaveCFG();
	CDialogEx::OnOK();
}



void CEnvSettingDlg::OnBnClickedBtnIoset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CIOSetDlg myDlg;
	myDlg.DoModal();

}



void CEnvSettingDlg::OnBnClickedBtnIomap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CI
}


void CEnvSettingDlg::OnBnClickedBtnLight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CLightSetDlg dlg;
	dlg.DoModal();
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
	CSCInspectorView *pView = (CSCInspectorView *)pFrame->GetActiveView();
	int nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	for (int i = 0; i < nCountOfCam; i++)
	{
		pView->m_pCamDlg[i]->m_pEuresysChannel->SetExpTime(m_uiCamExpTime[i]);
	}
}


void CEnvSettingDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_strModelRepository = CStatus::Instance()->GetModelRepository();
	m_strDataFileName = CStatus::Instance()->GetModelName();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CSCInspectorView *pView = (CSCInspectorView *)pFrame->GetActiveView();
	m_ulCountOfCam = CStatus::Instance()->m_nCountOfCam;

	for (int i = 0; i < m_ulCountOfCam; i++)
	{
		m_dblCamFov[i] = CStatus::Instance()->m_CamFov[i];
		m_uiCamExpTime[i] = CStatus::Instance()->m_lCamExpTime[i];
		if(!CStatus::Instance()->m_bIsOffLine)
			pView->m_pCamDlg[i]->m_pEuresysChannel->SetExpTime(m_uiCamExpTime[i]);
	}

	CDialogEx::OnCancel();
}


void CEnvSettingDlg::OnBnClickedBtnSetngfolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFolderDialog FolderDlg(_T("Select the NG Image folder."), CStatus::Instance()->GetModelRepository().GetBuffer());
	CString SaveFolder;
	switch (FolderDlg.DoModal())
	{
	case IDOK:
		SaveFolder = FolderDlg.GetFolderPath();
		m_strNGSaveFolder = SaveFolder;
		UpdateData(FALSE);
		break;
	case IDCANCEL:
		break;
	}


}


void CEnvSettingDlg::OnBnClickedBtnSetnremotefolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFolderDialog FolderDlg(_T("Select the Remote Camera Image folder."), CStatus::Instance()->m_strRemoteCamFolder);
	CString SaveFolder;
	switch (FolderDlg.DoModal())
	{
	case IDOK:
		SaveFolder = FolderDlg.GetFolderPath();
		m_strRemoteCamFolder = SaveFolder;
		UpdateData(FALSE);
		break;
	case IDCANCEL:
		break;
	}
}
