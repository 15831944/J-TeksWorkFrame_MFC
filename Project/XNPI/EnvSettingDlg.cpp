// EnvSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "EnvSettingDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "ComSetDlg.h"
#include "XNPIDoc.h"
#include "XNPIView.h"
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
	, m_strPlcIp(_T(""))
	, m_strPlcPort(_T(""))
	, m_bImgSaveFormatJPG(FALSE)
	, m_bImgSaveFormatTIF(FALSE)
	, m_strPlcReadCycleOfPlc(_T(""))
	, m_bSaveWithRaw(FALSE)
	, m_bShowGrayAvg(FALSE)
	, m_strXPixelSize(_T(""))
	, m_strYPixelSize(_T(""))
	, m_strStartPos(_T(""))
	, m_strEachDistance(_T(""))
	, m_strPlcReadCMDHoldingTime(_T(""))
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
	DDX_Text(pDX, IDC_EDIT_PLC_IP, m_strPlcIp);
	DDX_Text(pDX, IDC_EDIT_PLC_PORT, m_strPlcPort);
	DDX_Check(pDX, IDC_CHECK_TIF_SAVE, m_bImgSaveFormatTIF);
	DDX_Check(pDX, IDC_CHECK_JPG_SAVE, m_bImgSaveFormatJPG);
	DDX_Text(pDX, IDC_EDIT_PLC_READ_CYCLE, m_strPlcReadCycleOfPlc);
	DDX_Check(pDX, IDC_CHECK_JPG_SAVE2, m_bSaveWithRaw);
	DDX_Check(pDX, IDC_CHECK_JPG_SAVE3, m_bShowGrayAvg);
	DDX_Text(pDX, IDC_EDIT_XPIXEL_SIZE, m_strXPixelSize);
	DDX_Text(pDX, IDC_EDIT_YPIXEL_SIZE, m_strYPixelSize);
	DDX_Text(pDX, IDC_EDIT_START_POS, m_strStartPos);
	DDX_Text(pDX, IDC_EDIT_EACH_DISTANCE, m_strEachDistance);
	DDX_Text(pDX, IDC_EDIT_PLC_HOLDING_TIME, m_strPlcReadCMDHoldingTime);
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

	m_cIPTempSensorAddr.SetAddress(
		(BYTE)(CStatus::Instance()->m_lIPAddr[0]),
		(BYTE)(CStatus::Instance()->m_lIPAddr[1]),
		(BYTE)(CStatus::Instance()->m_lIPAddr[2]),
		(BYTE)(CStatus::Instance()->m_lIPAddr[3]));
	m_ulCountOfSensors = CStatus::Instance()->m_nCountOfSensors;
	m_nDaysDelImg = CStatus::Instance()->m_nDaysDel;
	m_ulGraphXRange = CStatus::Instance()->m_ulGraphXRange;
	m_ulGraphYMax = CStatus::Instance()->m_ulGraphYMax;
	m_ulGraphYMin = CStatus::Instance()->m_ulGraphYMin;
	m_iDispMode = CStatus::Instance()->m_ulDispMode;

	//! PLC IP및 포트 설정, 모드버스의 경우 포트는 502 고정이긴 하나 만약을 위해 설정할수 있게함 - kiy
	m_strPlcIp = CStatus::Instance()->m_strPlcIp;
	m_strPlcPort = CStatus::Instance()->m_strPlcPort;

	//픽셀 사이즈 로드 181016
	CString tempNum;
	tempNum.Format(_T("%.5f"), CStatus::Instance()->m_dPixelXSize_mm);
	m_strXPixelSize = tempNum;

	tempNum.Format(_T("%.5f"), CStatus::Instance()->m_dPixelYSize_mm);
	m_strYPixelSize = tempNum;

	//Current Position 로드 181016
	tempNum.Format(_T("%.2f"), CStatus::Instance()->m_pipeStartPos_mm);
	m_strStartPos = tempNum;

	tempNum.Format(_T("%.2f"), CStatus::Instance()->m_pipeSectEachDistance_mm);
	m_strEachDistance = tempNum;


	m_strPlcReadCycleOfPlc.Format(_T("%ld"), CStatus::Instance()->m_ulPlcReadCycleDelay_ms);

	m_strPlcReadCMDHoldingTime.Format(_T("%ld"), CStatus::Instance()->m_ulPlcReadCMDHoldingTime);

	//! 이미지의 저장 포맷 설정, JPG, TIF 선택가능, 둘다 선택도 가능하나 반드시 하나이상 설정해야한다. - kiy
	m_bImgSaveFormatJPG = CStatus::Instance()->m_bImgSaveFormatJPG;
	m_bImgSaveFormatTIF = CStatus::Instance()->m_bImgSaveFormatTIF;

	m_bSaveWithRaw = CStatus::Instance()->m_bSaveWithRaw;
	m_bShowGrayAvg = CStatus::Instance()->m_bShowGrayAvg;

	m_spDelOldFile.SetRange(1, 365);

	CheckDlgButton(IDC_CHK_DELOLDFILE, CStatus::Instance()->m_bUseDelOldImg);
	CheckDlgButton(IDC_CHK_SAVETESTIMG, CStatus::Instance()->m_bSaveTestImage);
	CheckDlgButton(IDC_CHK_SCRNCROSS, CStatus::Instance()->m_bDrawCross);
	CheckDlgButton(IDC_CHK_IMGENHANCE, CStatus::Instance()->m_bUseImgEnhance);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEnvSettingDlg::OnBnClickedBtnSetfolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFolderDialog FolderDlg(_T("Select the job file folder."), CStatus::Instance()->GetModelRepository().GetBuffer());
	CString SaveFolder;
	switch (FolderDlg.DoModal())
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

void CEnvSettingDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (!m_bImgSaveFormatJPG &&
		!m_bImgSaveFormatTIF)
	{
		AfxMessageBox(_T("Must select [Image Save Format] either 'JPG' or 'TIF'"));
		return;
	}

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

	CStatus::Instance()->m_strPlcIp = m_strPlcIp;
	CStatus::Instance()->m_strPlcPort = m_strPlcPort;
	CStatus::Instance()->m_ulPlcReadCycleDelay_ms = _wtol(m_strPlcReadCycleOfPlc);
	CStatus::Instance()->m_ulPlcReadCMDHoldingTime = _wtol(m_strPlcReadCMDHoldingTime);

	CStatus::Instance()->m_bImgSaveFormatJPG = m_bImgSaveFormatJPG;
	CStatus::Instance()->m_bImgSaveFormatTIF = m_bImgSaveFormatTIF;

	CStatus::Instance()->m_bSaveWithRaw = m_bSaveWithRaw;
	CStatus::Instance()->m_bShowGrayAvg = m_bShowGrayAvg;

	//픽셀 사이즈 추가
	CStatus::Instance()->m_dPixelXSize_mm = _wtof(m_strXPixelSize);
	CStatus::Instance()->m_dPixelYSize_mm = _wtof(m_strYPixelSize);

	//파이프 포지션 추가
	CStatus::Instance()->m_pipeStartPos_mm = _wtof(m_strStartPos);
	CStatus::Instance()->m_pipeSectEachDistance_mm = _wtof(m_strEachDistance);

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
	UpdateData(TRUE);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	int nCountOfCam = CStatus::Instance()->m_nCountOfCam;
}

void CEnvSettingDlg::OnBnClickedCancel()
{
	m_strModelRepository = CStatus::Instance()->GetModelRepository();
	m_strDataFileName = CStatus::Instance()->GetModelName();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXNPIView *pView = (CXNPIView *)pFrame->GetActiveView();
	m_ulCountOfCam = CStatus::Instance()->m_nCountOfCam;

	CDialogEx::OnCancel();
}

void CEnvSettingDlg::OnBnClickedBtnSetIMGfolder()
{
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

