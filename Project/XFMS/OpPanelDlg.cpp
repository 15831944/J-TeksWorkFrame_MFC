// OpPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XFMS.h"
#include "OpPanelDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "XFMSDoc.h"
#include "XFMSView.h"
#include "MainFrm.h"
#include "CamDlg.h"
#include "EnvSettingDlg.h"
#include "Repository.h"
#include "TabDetailLogDlg.h"
#include "MessageWnd.h"
#include "OpenModelDataDlg.h"
#include "SaveModelDataDlg.h"
#include "Repository.h"
#include "ModBusClient.h"
#include "HistoLevellingDlg.h"
#include "GraphDlg.h"
#include "ToolPanelDlg.h"
#include "Defines.h"


// COpPanelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COpPanelDlg, CDialogEx)

COpPanelDlg::COpPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPPANEL, pParent)
	, m_XFMSPacket(m_serial)
	, m_bReceived(FALSE)
	, m_selFocalSpotSize(0)
	, m_selImgAST(0)
	, m_strUserMarkName(_T(""))
	, m_strSampleName(_T(""))
	, m_strMakCondition(_T(""))
	, m_bWriteCSV(FALSE)
	, m_strFullCSVFileName(_T(""))
	, m_selAVIAST(0)
	, m_bImgAuto(FALSE)
	, m_pToolPanelDlg(NULL)
	, m_bAVIAuto(FALSE)
{

}

COpPanelDlg::~COpPanelDlg()
{
}

void COpPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELCAM, m_ctrlSelCam);
	DDX_Control(pDX, IDC_AISTART, m_cRunStop);
	DDX_Control(pDX, IDC_BTN_AIFIN, m_cFinish);
	DDX_Control(pDX, IDC_BTN_LOADIMAGE, m_cLoadImage);
	DDX_Control(pDX, IDC_BTN_LOADJOBFILE, m_cLoadJobFile);
	DDX_Control(pDX, IDC_BTN_SAVEJOBFILE, m_cSaveJobFile);
	DDX_Control(pDX, IDC_BTN_SAVEAS, m_cSaveAs);
	DDX_Control(pDX, IDC_BTN_TEACHING, m_cTeaching);
	DDX_Control(pDX, IDC_BTNTRIAL, m_cTrial);
	DDX_Control(pDX, IDC_SETENV, m_cSetEnv);
	DDX_Control(pDX, IDC_TEMP_P1, m_cTempP1);
	DDX_Control(pDX, IDC_TEMP_P2, m_cTempP2);
	DDX_Control(pDX, IDC_TEMP_P3, m_cTempP3);
	DDX_Control(pDX, IDC_TEMP_P4, m_cTempP4);
	DDX_Control(pDX, IDC_LABELP1, m_cLabelP1);
	DDX_Control(pDX, IDC_LABELP2, m_cLabelP2);
	DDX_Control(pDX, IDC_LABELP3, m_cLabelP3);
	DDX_Control(pDX, IDC_LABELP4, m_cLabelP4);
	DDX_Radio(pDX, IDC_SELFOCALSPOT, m_selFocalSpotSize);
	DDX_Radio(pDX, IDC_IMG_AST, m_selImgAST);
	DDX_Control(pDX, IDC_BTN_XRAYON, m_cBtnXrayON);
	DDX_Control(pDX, IDC_BTN_XRAYOFF, m_cBtnXrayOFF);
	DDX_Text(pDX, IDC_EDT_MM, m_strUserMarkName);
	DDX_Text(pDX, IDC_EDT_SMPNAME, m_strSampleName);
	DDX_Text(pDX, IDC_EDT_MAKCONDITION, m_strMakCondition);
	DDX_Control(pDX, IDC_ADDMARK, m_cMarkAdd);
	DDX_Control(pDX, IDC_BTN_SAVCSV, m_cTempCSVSave);
	DDX_Control(pDX, IDC_EDT_VOL, m_cVol);
	DDX_Control(pDX, IDC_EDT_CUR, m_cCur);
	DDX_Control(pDX, IDC_BTN_AAFIN, m_cAAFin);
	DDX_Control(pDX, IDC_AASTART, m_cAAStart);
	DDX_Radio(pDX, IDC_IMG_AST2, m_selAVIAST);
	DDX_Control(pDX, IDC_TPICK_IASS, m_cImgTimeStart);
	DDX_Control(pDX, IDC_TPICK_IASE, m_cImgTimeEnd);
	DDX_Control(pDX, IDC_TPICK_AASS, m_cAVITimeStart);
	DDX_Control(pDX, IDC_TPICK_AASE, m_cAVITimeEnd);
	DDX_Control(pDX, IDC_LABELP5, m_cLabelP5);
	DDX_Control(pDX, IDC_LABELP6, m_cLabelP6);
	DDX_Control(pDX, IDC_TEMP_P5, m_cTempP5);
	DDX_Control(pDX, IDC_TEMP_P6, m_cTempP6);
}


BEGIN_MESSAGE_MAP(COpPanelDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_SELCAM, &COpPanelDlg::OnCbnSelchangeSelcam)
	ON_BN_CLICKED(IDC_BTN_LOADIMAGE, &COpPanelDlg::OnBnClickedBtnLoadimage)
	ON_BN_CLICKED(IDC_SETENV, &COpPanelDlg::OnBnClickedSetenv)
	ON_BN_CLICKED(IDC_BTN_TEACHING, &COpPanelDlg::OnBnClickedBtnTeaching)
	ON_BN_CLICKED(IDC_BTN_LOADJOBFILE, &COpPanelDlg::OnBnClickedBtnLoadjobfile)
	ON_BN_CLICKED(IDC_BTN_SAVEJOBFILE, &COpPanelDlg::OnBnClickedBtnSavejobfile)
	ON_BN_CLICKED(IDC_CHKACTIVEALLCAM, &COpPanelDlg::OnBnClickedChkactiveallcam)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_AISTART, &COpPanelDlg::OnBnClickedAistart)
	ON_BN_CLICKED(IDC_BTN_SAVEAS, &COpPanelDlg::OnBnClickedBtnSaveas)
	ON_BN_CLICKED(IDC_BTNTRIAL, &COpPanelDlg::OnBnClickedBtntrial)
	ON_BN_CLICKED(IDC_BTN_OPTEST, &COpPanelDlg::OnBnClickedBtnOptest)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_XRAY_OFF, &COpPanelDlg::OnXrayOff)
	ON_BN_CLICKED(IDC_BTN_AIFIN, &COpPanelDlg::OnBnClickedBtnAifin)
	ON_BN_CLICKED(IDC_SELFOCALSPOT, &COpPanelDlg::OnBnClickedSelfocalspot)
	ON_BN_CLICKED(IDC_BTN_XRAYON, &COpPanelDlg::OnBnClickedBtnXrayon)
	ON_BN_CLICKED(IDC_BTN_XRAYOFF, &COpPanelDlg::OnBnClickedBtnXrayoff)
	ON_BN_CLICKED(IDC_ADDMARK, &COpPanelDlg::OnBnClickedAddmark)
	ON_BN_CLICKED(IDC_BTN_SAVCSV, &COpPanelDlg::OnBnClickedBtnSavcsv)
	ON_BN_CLICKED(IDC_AASTART, &COpPanelDlg::OnBnClickedAastart)
	ON_BN_CLICKED(IDC_BTN_AAFIN, &COpPanelDlg::OnBnClickedBtnAafin)
	ON_BN_CLICKED(IDC_SELFOCALSPOTLARGE, &COpPanelDlg::OnBnClickedSelfocalspotlarge)
END_MESSAGE_MAP()


// COpPanelDlg 메시지 처리기입니다.


BOOL COpPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;;
	CString strCamNo;
	for (int i = 0; i < m_nCountOfCam; i++)
	{
		strCamNo.Format(_T("%d"), i+1);
		m_ctrlSelCam.AddString(strCamNo);
	}
	m_ctrlSelCam.SetCurSel(0);


	LOGFONT log;
	m_cTempP1.GetFont()->GetLogFont(&log);
	log.lfHeight = 25;
	m_BigFont.CreateFontIndirect(&log);

	m_cTempP1.SetFont(&m_BigFont);
	m_cTempP2.SetFont(&m_BigFont);
	m_cTempP3.SetFont(&m_BigFont);
	m_cTempP4.SetFont(&m_BigFont);
	m_cTempP5.SetFont(&m_BigFont);
	m_cTempP6.SetFont(&m_BigFont);
	m_cLabelP1.SetFont(&m_BigFont);
	m_cLabelP2.SetFont(&m_BigFont);
	m_cLabelP3.SetFont(&m_BigFont);
	m_cLabelP4.SetFont(&m_BigFont);
	m_cLabelP5.SetFont(&m_BigFont);
	m_cLabelP6.SetFont(&m_BigFont);

	SetDlgItemText(IDC_LABELP1, CStatus::Instance()->m_strSensorNamePx[0]);
	SetDlgItemText(IDC_LABELP2, CStatus::Instance()->m_strSensorNamePx[1]);
	SetDlgItemText(IDC_LABELP3, CStatus::Instance()->m_strSensorNamePx[2]);
	SetDlgItemText(IDC_LABELP4, CStatus::Instance()->m_strSensorNamePx[3]);
	SetDlgItemText(IDC_LABELP5, CStatus::Instance()->m_strSensorNamePx[4]);
	SetDlgItemText(IDC_LABELP6, CStatus::Instance()->m_strSensorNamePx[5]);

	SetDlgItemText(IDC_TEMP_P1, _T("--"));
	SetDlgItemText(IDC_TEMP_P2, _T("--"));
	SetDlgItemText(IDC_TEMP_P3, _T("--"));
	SetDlgItemText(IDC_TEMP_P4, _T("--"));
	SetDlgItemText(IDC_TEMP_P5, _T("--"));
	SetDlgItemText(IDC_TEMP_P6, _T("--"));
	m_strMakCondition = _T("MakeCondition");
	m_strSampleName = _T("SampleName");
	SetDefault();
	switch (CStatus::Instance()->m_nCountOfSensors)
	{
	case 6:
		m_cLabelP6.ShowWindow(TRUE);
		m_cTempP6.ShowWindow(TRUE);
	case 5:
		m_cLabelP5.ShowWindow(TRUE);
		m_cTempP5.ShowWindow(TRUE);
	case 4:
		m_cLabelP4.ShowWindow(TRUE);
		m_cTempP4.ShowWindow(TRUE);
	case 3:
		m_cLabelP3.ShowWindow(TRUE);
		m_cTempP3.ShowWindow(TRUE);
	case 2:
		m_cLabelP2.ShowWindow(TRUE);
		m_cTempP2.ShowWindow(TRUE);
	case 1:
		m_cLabelP1.ShowWindow(TRUE);
		m_cTempP1.ShowWindow(TRUE);
		break;
	}

	SerialCheckNInit();
	m_selImgAST = 0;
	m_selAVIAST = 0;
	m_selFocalSpotSize = 0;
//	SetTimer(TIMEREVENT_DELOLDFILE, 3600*1000, NULL);
	SetTimer(TIMEEVENT_READTEMP, 1000, NULL);
	SetWindowTheme(GetDlgItem(IDC_CHKACTIVEALLCAM)->m_hWnd, _T(""), _T(""));
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void COpPanelDlg::OnCbnSelchangeSelcam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int CurSel = m_ctrlSelCam.GetCurSel();
	CStatus::Instance()->SetSelCam(CurSel);
}


void COpPanelDlg::OnBnClickedBtnLoadimage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog LoadFile(TRUE, _T(""), _TEXT("*.TIF"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _TEXT("TIF|*.TIF|BMP|*.BMP|"));
	if (LoadFile.DoModal()==IDOK)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
		int nCurSel = m_ctrlSelCam.GetCurSel();

		CString strExt;
		strExt = CStatus::Instance()->GetModelData()->strClip(LoadFile.GetPathName(), 2);
		strExt.MakeUpper();
		MIL_INT typeImage;
		if (strExt == _T(".BMP"))
		{
			typeImage = M_BMP;
		}else
		if (strExt == _T(".TIF"))
		{ 
			typeImage = M_TIFF;
		}
		CMilBuffer *pImage = CRepository::Instance()->m_MilLib.NewImage();
		pImage->Restore(LoadFile.GetPathName());
		pView->m_pCamDlg[nCurSel]->LoadBufImage(pImage);
//		pView->m_pCamDlg[nCurSel]->m_pMilCamView->BufImport(LoadFile.GetPathName(), typeImage);
		pView->m_pCamDlg[nCurSel]->m_pMilDisp->FilWindow();
		pImage->Close();
	}

}


void COpPanelDlg::OnBnClickedSetenv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEnvSettingDlg dlg;
	dlg.DoModal();
}

int COpPanelDlg::InitLight()
{
	return 0;
}


int COpPanelDlg::SerialCheckNInit()
{
	if (m_serial.IsOpen())
	{
		m_serial.Close();
	}

	m_serial.RegCallback(std::bind(&COpPanelDlg::ProcessCMD, this, std::placeholders::_1));
	m_serial.RegAnalizerCallback(std::bind(&COpPanelDlg::CheckoutPacket, this));

	CString  strSerialNo = CStatus::Instance()->GetComportNo();
	int iBaudRate = CStatus::Instance()->GetBaudRate();
	CSerial::EBaudrate BaudRate;
	switch (iBaudRate)
	{
	case 9600://9600
		BaudRate = CSerial::EBaud9600;
		break;
	case 19200://19200
		BaudRate = CSerial::EBaud19200;
		break;
	case 38400://38400
		BaudRate = CSerial::EBaud38400;
		break;
	case 115200://115200
		BaudRate = CSerial::EBaud115200;
		break;
	default:
		BaudRate = CSerial::EBaud9600;
	}

	strSerialNo = _T("\\\\.\\") + strSerialNo;

	// 필수 -
	if (m_serial.Open(strSerialNo.GetBuffer(), this, 0, 0, 16, 16) == ERROR_SUCCESS)
	{
		//		VERIFY(m_serial.SetupHandshaking(CSerial::EHandshake::EHandshakeOff) == ERROR_SUCCESS);
		VERIFY(m_serial.Setup(BaudRate, CSerial::EData8, CSerial::EParNone, CSerial::EStop1) == ERROR_SUCCESS);
		VERIFY(m_serial.SetMask(CSerial::EEventBreak | CSerial::EEventError | CSerial::EEventRecv) == ERROR_SUCCESS);
	}
	// - 필수

	if (!m_serial.IsOpen())
	{
		AfxMessageBox(_T("Unable to open COM-port"), MB_ICONSTOP | MB_OK);
	}

	CStatus::Instance()->SetComm(&m_XFMSPacket);
	return 0;
}


void COpPanelDlg::ProcessCMD(int nEvent)
{
	RcvSerialMsg((WPARAM)nEvent);
	SetFlagReceived();
}

int COpPanelDlg::RcvSerialMsg(WPARAM wParam)
{
	CSerial::EEvent eEvent = (CSerial::EEvent)wParam;

	static int prevTime = 0;
	static int prevNum = 0;
	static int Packet0Time = 0;
	static int Packet255Time = 0;
	USES_CONVERSION;
	CXFMSPacket::EPacketType eType = CXFMSPacket::EPacketType::None;

	if (eEvent == CSerial::EEvent::EEventRecv)
		eType = m_XFMSPacket.CheckoutPacket();
	else
		TRACE(_T("Serial Event: %d\n"), eEvent);

	if (eType == CXFMSPacket::None) return -1;

	//if (prevTime == 0)prevTime = timeGetTime();
	//int curTime = timeGetTime();
	//printf("\nInterval Packet %dms",curTime-prevTime);
	//prevTime = curTime;

	switch (eType)
	{
	case 0:
		break;
	default:
		break;
	}
	return 0;
}

void COpPanelDlg::SetFlagReceived()
{
	m_bReceived = TRUE;
}

BOOL COpPanelDlg::IsCMDReceived()
{
	BOOL bReturn;
	bReturn = m_bReceived;
	m_bReceived = FALSE;
	return bReturn;
}

int COpPanelDlg::CheckoutPacket()
{
	return m_XFMSPacket.CheckoutPacket();
}

void COpPanelDlg::OnBnClickedBtnTeaching()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (CStatus::Instance()->GetOpMode() == CStatus::OPAUTO)
		OnBnClickedAistart();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	pView->SetOPMode(CStatus::OPTEACHING);
}


void COpPanelDlg::OnBnClickedBtnLoadjobfile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COpenModelDataDlg dlg;
	dlg.DoModal();
}


void COpPanelDlg::OnBnClickedBtnSavejobfile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		AfxMessageBox(_T("COM ERROR\r\nCoInitialize Failed!"));
		return;
	}
	CStatus::Instance()->GetModelData()->SetModelPath(CStatus::Instance()->GetModelRepository().GetBuffer());
	CStatus::Instance()->GetModelData()->SetMDName(CStatus::Instance()->GetModelName().GetBuffer());
	CStatus::Instance()->GetModelData()->SaveModelData();
}

void COpPanelDlg::OnBnClickedChkactiveallcam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	if (IsDlgButtonChecked(IDC_CHKACTIVEALLCAM))
	{
		pView->m_pCamDlg[0]->m_pGrabChannel->GrabContinuous();
	}
	else
	{
		pView->m_pCamDlg[0]->m_pGrabChannel->GrabStop();
		//for (int i = 0; i < m_nCountOfCam; i++)
		//	pView->m_pCamDlg[i]->m_pEuresysChannel->GrabStop();
	}
}


void COpPanelDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;

	switch (nIDEvent)
	{
	case TIMEREVENT_GRAPCONTINUOUS:
		for (int i = 0; i < m_nCountOfCam; i++)
			pView->m_pCamDlg[i]->DoGrab();
		break;
	case TIMEREVENT_DELOLDFILE:
		if(CStatus::Instance()->m_bUseDelOldImg)DeleteOldFile();
		break;
	case TIMEEVENT_READTEMP:
		GetTempData();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void COpPanelDlg::OnBnClickedAistart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_bImgAuto)
	{
		AfxMessageBox(_T("Already started"));
		return;
	}

	if (!m_selImgAST)
	{
		TimeImgAutoSaveStarter();
	}
	else
	{
		TempImgAutoSaveStarter();
	}

	if(m_bImgAuto)
		m_cRunStop.SetBlink(TRUE);


}


void COpPanelDlg::OnBnClickedBtnSaveas()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSaveModelDataDlg dlg;
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		AfxMessageBox(_T("COM ERROR\r\nCoInitialize Failed!"));
		return;
	}

	dlg.DoModal();

//	CStatus::Instance()->GetModelData()->SaveModelData(SaveFile.GetPathName());

	//CFileDialog SaveFile(FALSE, _T(""), _T("*.INI"), OFN_OVERWRITEPROMPT, _TEXT("INI|*.INI|"));
	//if (SaveFile.DoModal()==IDOK)
	//{
	//}
}


void COpPanelDlg::OnBnClickedBtntrial()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	int nCurSel = m_ctrlSelCam.GetCurSel();
	pView->DoTrial();
}


void COpPanelDlg::OnBnClickedBtnOptest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	if(m_cAAStart.IsBlink())m_cAAStart.SetBlink(FALSE);
	else m_cAAStart.SetBlink(TRUE);
}


void COpPanelDlg::DeleteOldFile()
{
	CString strNGSaveFolder = CStatus::Instance()->m_strIMGSaveFolder;

	CFileFind NGFolderFinder;
	BOOL bFind = NGFolderFinder.FindFile(strNGSaveFolder + _T("\\*.*"));
	while (bFind)
	{
		bFind = NGFolderFinder.FindNextFileW();
		if (NGFolderFinder.IsDirectory())
		{
			CString strName = NGFolderFinder.GetFileName();
			if (strName.GetLength() == 6)
			{
				CTime tmDate;
				if (IsDate(strName, &tmDate))
				{
					if (DiffDate(&tmDate) > CStatus::Instance()->m_nDaysDel)
					{
						DelFolder(NGFolderFinder.GetFilePath());
					}
				}
			}
		}
	}
	

}

BOOL COpPanelDlg::IsDate(CString strName, CTime *pTime)
{
	int nLength = strName.GetLength();
	if (nLength != 6)return FALSE;
	for (int i = 0; i < nLength; i++)
	{
		if ( ('0' > strName.GetAt(i)) || ('9' < strName.GetAt(i)) )
			return FALSE;
	}
	
	CString strYear = strName.Left(2);
	CString strMon = strName.Mid(2, 2);
	CString strDay = strName.Right(2);
	
	int iYear = _wtoi(strYear)+2000;
	int iMon = _wtoi(strMon);
	int iDay = _wtoi(strDay);
	CTime tmDate(iYear, iMon, iDay, 0, 0, 0);

	*pTime = tmDate;

	return TRUE;
}


int COpPanelDlg::DiffDate(CTime * pDate)
{
	CTime tmCurrent = CTime::GetCurrentTime();
	CTimeSpan tmDiff = tmCurrent - *pDate;
	return tmDiff.GetDays();
}


void COpPanelDlg::DelFolder(CString strFolder)
{
	CFileInfo::SHDeleteFolder(strFolder);
}


BOOL COpPanelDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


//HBRUSH COpPanelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  여기서 DC의 특성을 변경합니다.
//	switch (nCtlColor)
//	{
//	case CTLCOLOR_STATIC:
//		hbr = CreateSolidBrush(RGB(255, 255, 0));
//		pDC->SetBkColor(RGB(255, 0, 0));
//		break;
//	}
//
//	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
//	return hbr;
//}


HBRUSH COpPanelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	//switch (nCtlColor)
	//{
	//case CTLCOLOR_STATIC:
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(RGB(90, 107, 135));  // static text 글자색 변경
	//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	//}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


UINT COpPanelDlg::ThreadTest(LPVOID pParam)
{
	CXFMSView *pView = (CXFMSView *)pParam;
	BOOL m_bRun = TRUE;
	for(int i=0;i<1000;i++)
	{
		pView->m_pCamDlg[0]->SaveImage();
		Sleep(200);
	}

	return 0;	return 0;
}


void COpPanelDlg::OnBnClickedBtnAifin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bImgAuto = FALSE;
	m_cRunStop.SetBlink(FALSE);
}



int COpPanelDlg::GetTempData()
{
	BYTE byAddr0, byAddr1, byAddr2, byAddr3;
	CString strAddress = CStatus::Instance()->GetTempSensorIPString();

	CModBusClient *pSockModBusRequest;

	REQ_READ_INPUTREG testp;
	testp.wID = 0x04;
	testp.wProtocolID = 0;
	testp.byDummy4 = 0;
	testp.byLenth = 6;
	testp.byUnit = 0;
	testp.byFuncCode = 0x04;
	testp.wStartAddr = 0x00;
	testp.byDummy10 = 0x00;
	testp.byQuanOfReg = 6;//6channel

	DATA_INPUTREG in;
	memset(&in, 0, sizeof(in));
	BYTE *testpoint;
	testpoint = (BYTE *)(&testp);

	pSockModBusRequest = new CModBusClient;
	pSockModBusRequest->Create(0);
	pSockModBusRequest->Connect(strAddress, 502);

	Sleep(100);
	int sent = pSockModBusRequest->Send(&testp, sizeof(testp));
	if (sent == SOCKET_ERROR)
	{
		int ierr = pSockModBusRequest->GetLastError();
		pSockModBusRequest->Close();
		delete pSockModBusRequest;
		return -1;
	}
	Sleep(100);
	int nCount = pSockModBusRequest->Receive(&in, sizeof(in));

	for (int i = 0; i < 6; i++)
	{
		in.wTemp[i] = ntohs(in.wTemp[i]);
		CStatus::Instance()->m_lTemperature[i] = in.wTemp[i];
	}
	CString strTempP1, strTempP2, strTempP3, strTempP4, strTempP5, strTempP6;
	int iTemperature = CStatus::Instance()->m_lTemperature[0] / 10.0;
//	strTempP1.Format(_T("%4.1f℃"), CStatus::Instance()->m_lTemperature[0] / 10.0);
	strTempP1.Format(_T("%4d"), iTemperature);
	iTemperature = CStatus::Instance()->m_lTemperature[1] / 10.0;
	strTempP2.Format(_T("%4d"), iTemperature);
	iTemperature = CStatus::Instance()->m_lTemperature[2] / 10.0;
	strTempP3.Format(_T("%4d"), iTemperature);
	iTemperature = CStatus::Instance()->m_lTemperature[3] / 10.0;
	strTempP4.Format(_T("%4d"), iTemperature);
	iTemperature = CStatus::Instance()->m_lTemperature[4] / 10.0;
	strTempP5.Format(_T("%4d"), iTemperature);
	iTemperature = CStatus::Instance()->m_lTemperature[5] / 10.0;
	strTempP6.Format(_T("%4d"), iTemperature);

	SetDlgItemText(IDC_TEMP_P1, strTempP1);
	SetDlgItemText(IDC_TEMP_P2, strTempP2);
	SetDlgItemText(IDC_TEMP_P3, strTempP3);
	SetDlgItemText(IDC_TEMP_P4, strTempP4);
	SetDlgItemText(IDC_TEMP_P5, strTempP5);
	SetDlgItemText(IDC_TEMP_P6, strTempP6);

	pSockModBusRequest->Close();

	delete pSockModBusRequest;
	if (m_bWriteCSV)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
		tXFMSRecord *pXFMSRecord;
		pXFMSRecord = new tXFMSRecord;
		for (int i = 0; i < 6; i++)
		{
			pXFMSRecord->Temperature[i] = CStatus::Instance()->m_lTemperature[i];
		}
		if (CRepository::Instance()->m_DataRecords.m_XFMSRecords.GetCount() >= CStatus::Instance()->m_ulGraphXRange)
		{
			OnBnClickedBtnSavcsv();
		}
		else
		{
			CRepository::Instance()->AppendData(pXFMSRecord, m_strFullCSVFileName);
			pView->m_pGraphDlg->Invalidate(FALSE);
		}
	}

	return 0;
}


void COpPanelDlg::OnBnClickedSelfocalspot()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Xray small focus"), RGB(50, 50, 50), FALSE);
	m_XFMSPacket.SendSetFocalSpot(FALSE);
}

void COpPanelDlg::OnBnClickedBtnXrayon()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Xray turn On!"), RGB(50, 50, 50), FALSE);
	m_XFMSPacket.SendSetXrayOnOff(TRUE);
}


void COpPanelDlg::OnBnClickedBtnXrayoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Xray turn Off!"), RGB(50, 50, 50), FALSE);
	m_XFMSPacket.SendSetXrayOnOff(FALSE);
}

void COpPanelDlg::OnBnClickedAddmark()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString strAddMark;
	CString strLogMsg;
	if (!m_strUserMarkName.IsEmpty())
	{
		strAddMark = m_strUserMarkName;
		strLogMsg.Format(_T("Mark Add!!(%s)"), strAddMark.GetBuffer());
		CTabDetailLogDlg::Instance()->SetTextLog(strLogMsg.GetBuffer(), RGB(50, 50, 50), FALSE);

	}
	else
	{
		//if (m_strAddMark.IsEmpty())
		//{
		//	m_strAddMark.Format(_T("Mark%d"), CStatus::Instance()->GetAutoMarkPos());
		//	CStatus::Instance()->IncAutoMarkPos();
		//}
	}
	m_strUserMarkName = _T("");
	CStatus::Instance()->SetMarkString(strAddMark);
	UpdateData(FALSE);

}


CString COpPanelDlg::GetManualFileName()
{
	CString strFileName;
	CString strSampleName;
	CString strMakCdt;
	GetDlgItemText(IDC_EDT_SMPNAME, strSampleName);
	GetDlgItemText(IDC_EDT_MAKCONDITION, strMakCdt);
	strFileName = strSampleName + strMakCdt;
	CTime curTime = CTime::GetCurrentTime();
	strFileName = strFileName + curTime.Format(_T("%y%m%d%H%M%S"));

	return strFileName;
}


BOOL COpPanelDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			
			UINT nID = ::GetDlgCtrlID(pMsg->hwnd);
			CString strNum;
			double dblNum;

			switch (nID)
			{
			case IDC_EDT_VOL:
				GetDlgItemText(IDC_EDT_VOL, strNum);
				dblNum = _tstof(strNum);
				m_XFMSPacket.SendSetKvCMD(dblNum);
				break;
			case IDC_EDT_CUR:
				GetDlgItemText(IDC_EDT_CUR, strNum);
				dblNum = _tstof(strNum);
				m_XFMSPacket.SendSetMaCmd(dblNum);
				break;
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void COpPanelDlg::OnBnClickedBtnSavcsv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	CString strCSVFolder = CStatus::Instance()->m_strModelRepository;

	CTime tmCurrent = CTime::GetCurrentTime();
	CString strDate = tmCurrent.Format(_T("%y%m%d"));
	CString strSaveFolder = strCSVFolder + _T("\\") + strDate;

	CString strFileName = GetManualFileName();

	CString strFullFileName = strSaveFolder + _T("\\") + strFileName + _T(".CSV");
	CRepository *pRepository = CStatus::Instance()->GetRepository();

	CString strSavBtn;
	GetDlgItemText(IDC_BTN_SAVCSV, strSavBtn);
	if (strSavBtn == _T("SAVE"))
	{
		m_cTempCSVSave.SetBlink(TRUE);
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Temperature value save Start"), RGB(50, 50, 50), FALSE);
		pRepository->EraseAllRecords();
		CFileInfo::CreateFolder(strSaveFolder.GetBuffer());
		m_bWriteCSV = TRUE;
		m_strFullCSVFileName = strFullFileName;
		SetDlgItemText(IDC_BTN_SAVCSV, _T("STOP"));
	}
	else
	{
		m_cTempCSVSave.SetBlink(FALSE);
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Temperature value save Stop"), RGB(50, 50, 50), FALSE);
		m_bWriteCSV = FALSE;
		SetDlgItemText(IDC_BTN_SAVCSV, _T("SAVE"));
	}
}


void COpPanelDlg::CloseProcess()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();

	OnBnClickedBtnXrayoff();
	if (IsDlgButtonChecked(IDC_CHKACTIVEALLCAM))
	{
		pView->m_pCamDlg[0]->m_pGrabChannel->GrabStop();
	}

}


void COpPanelDlg::OnBnClickedAastart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_bAVIAuto)
	{
		AfxMessageBox(_T("Already started"));
		return;
	}

	if (CStatus::Instance()->m_bIsAVIRecording)
	{
		AfxMessageBox(_T("Already, In recording !"));
		return;
	}


	if (!m_selAVIAST)
	{
		TimeAVIAutoSaveStarter();

	}
	else
	{
		TempAVIAutoSaveStarter();
	}
	if (m_bAVIAuto)
		m_cAAStart.SetBlink(TRUE);
}


void COpPanelDlg::OnBnClickedBtnAafin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	m_bAVIAuto = FALSE;
	if(!pView->m_pToolPanelDlg->m_bAVIMan)	
		CStatus::Instance()->m_bIsAVIRecording = FALSE;
	m_cAAStart.SetBlink(FALSE);

}


// 일정 시간간격으로 자동저장하는 자동 이미지 저장 루틴
void COpPanelDlg::TimeImgAutoSaveStarter()
{
	COleDateTime		oleTimeStart;
	COleDateTime		oleTimeEnd;
	COleDateTime		oleTimeCurrent;
	COleDateTimeSpan	oleTimeDiff;

	CTime tmStart, tmEnd;
	SYSTEMTIME systime;

	VERIFY(m_cImgTimeStart.GetTime(oleTimeStart));
	VERIFY(m_cImgTimeEnd.GetTime(oleTimeEnd));
	oleTimeCurrent = COleDateTime::GetCurrentTime();
	oleTimeDiff = oleTimeStart - oleTimeCurrent;
	if (oleTimeDiff.GetTotalSeconds() < 0)oleTimeStart = oleTimeCurrent;
	oleTimeDiff = oleTimeEnd - oleTimeStart;
	int iInterval = GetDlgItemInt(IDC_EDT_IAS_DELTATIME);
	if (oleTimeDiff.GetTotalSeconds() < iInterval * 60)
	{
		AfxMessageBox(_T("Interval is smaller than diffrnent time. Try again"));
		return;
	}

	int iTotalSecs = oleTimeDiff.GetTotalSeconds();
	if (iTotalSecs < 60)
	{
		AfxMessageBox(_T("Too small diffrent time. Try again"));
		return;
	}
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Image auto save Start(Time)"), RGB(50, 50, 50), FALSE);

	oleTimeStart.GetAsSystemTime(systime);
	tmStart = CTime(systime);

	oleTimeEnd.GetAsSystemTime(systime);
	tmEnd = CTime(systime);

	m_ImgAutoSaveTime.tmStart			= tmStart;
	m_ImgAutoSaveTime.tmEnd				= tmEnd;
	m_ImgAutoSaveTime.iSecTimeTotal		= iTotalSecs;
	m_ImgAutoSaveTime.iMinTimeInterval	= iInterval;
	m_bImgAuto = TRUE;

	AfxBeginThread(ThreadTimeImgAutoSave, this);





}


// 온도변화량으로 자동저장하는 자동 이미지 저장 루틴
void COpPanelDlg::TempImgAutoSaveStarter()
{
	m_ImgAutoSaveTemp.iTempLimitLow = GetDlgItemInt(IDC_EDT_IAS_TLMTLOW);
	m_ImgAutoSaveTemp.iTempLimitHigh = GetDlgItemInt(IDC_EDT_IAS_TLMTHIGH);
	m_ImgAutoSaveTemp.iTempDelta = GetDlgItemInt(IDC_IAS_TDELTA_TEMP);
	m_ImgAutoSaveTemp.iSensorNo = GetDlgItemInt(IDC_EDT_IAS_SENSORNO);
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Image auto save Start(Temperature)"), RGB(50, 50, 50), FALSE);
	if ((m_ImgAutoSaveTemp.iTempLimitLow + 100) > m_ImgAutoSaveTemp.iTempLimitHigh)
	{
		AfxMessageBox(_T("High temperature shall have a higher temperature low temperature than 100℃"));
		return;
	}
	m_bImgAuto = TRUE;
	m_cRunStop.SetBlink(TRUE);
	AfxBeginThread(ThreadTempImgAutoSave, this);
}


void COpPanelDlg::TimeAVIAutoSaveStarter()
{
	COleDateTime	oleTimeStart;
	COleDateTime	oleTimeEnd;
	COleDateTime	oleTimeCurrent;
	COleDateTimeSpan	oleTimeDiff;

	CTime tmStart, tmEnd;
	SYSTEMTIME systime;

	VERIFY(m_cAVITimeStart.GetTime(oleTimeStart));
	VERIFY(m_cAVITimeEnd.GetTime(oleTimeEnd));
	oleTimeCurrent = COleDateTime::GetCurrentTime();
	oleTimeDiff = oleTimeStart - oleTimeCurrent;
	if (oleTimeDiff.GetTotalSeconds() < 0)oleTimeStart = oleTimeCurrent;

	int iDuration = GetDlgItemInt(IDC_AAS_DURATION);
	if (iDuration <= 0)iDuration = 60;
	oleTimeDiff = oleTimeEnd - oleTimeStart;
	int iTotalSecs = oleTimeDiff.GetTotalSeconds();

	if (iTotalSecs < 60)
	{
		AfxMessageBox(_T("Too small diffrent time. Try again"));
		return;
	}

	oleTimeStart.GetAsSystemTime(systime);
	tmStart = CTime(systime);

	oleTimeEnd.GetAsSystemTime(systime);
	tmEnd = CTime(systime);

	CTabDetailLogDlg::Instance()->SetTextLog(_T("AVI auto save Start(Time)"), RGB(50, 50, 50), FALSE);

	m_AVIAutoSaveTime.tmStart = tmStart;
	m_AVIAutoSaveTime.tmEnd = tmEnd;
	m_AVIAutoSaveTime.iSecTimeTotal = iTotalSecs;//sec
	m_AVIAutoSaveTime.iMinTimeInterval = 0;
	m_AVIAutoSaveTime.iMinTimeDuration = iDuration;

	m_bAVIAuto = TRUE;
	CStatus::Instance()->m_bIsAVIRecording = TRUE;

	AfxBeginThread(ThreadTimeAVIAutoSave, this);
}


void COpPanelDlg::TempAVIAutoSaveStarter()
{
	m_AVIAutoSaveTemp.iTempLimitLow		= GetDlgItemInt(IDC_EDT_AAS_TLMTLOW);
	m_AVIAutoSaveTemp.iTempLimitHigh	= GetDlgItemInt(IDC_EDT_AAS_TLMTHIGH);
	m_AVIAutoSaveTemp.iTempDelta		= GetDlgItemInt(IDC_IAS_TDELTA_TEMP);
	m_AVIAutoSaveTemp.iSensorNo			= GetDlgItemInt(IDC_EDT_AAS_SENSORNO);
	m_AVIAutoSaveTemp.iMinTimeDuration		= GetDlgItemInt(IDC_AAS_DURATION);
	CTabDetailLogDlg::Instance()->SetTextLog(_T("AVI auto save Start(Temperature)"), RGB(50, 50, 50), FALSE);

	if ((m_AVIAutoSaveTemp.iTempLimitLow + 100) > m_AVIAutoSaveTemp.iTempLimitHigh)
	{
		AfxMessageBox(_T("High temperature shall have a higher temperature low temperature than 100℃"));
		return;
	}

	m_bAVIAuto = TRUE;
	CStatus::Instance()->m_bIsAVIRecording = TRUE;
	AfxBeginThread(ThreadTempAVIAutoSave, this);
}


UINT COpPanelDlg::ThreadTimeImgAutoSave(LPVOID pParam)
{
	COpPanelDlg * pOpPanelDlg = (COpPanelDlg *)pParam;
	CToolPanelDlg *pToolPanelDlg = pOpPanelDlg->m_pToolPanelDlg;
	tAutoSaveTime AutoSaveTime = pOpPanelDlg->m_ImgAutoSaveTime;

	CTime startTime = AutoSaveTime.tmStart;
	CTime curTime;
	int nextTime = 0;
	while (pOpPanelDlg->m_bImgAuto)
	{
		curTime = CTime::GetCurrentTime();
		CTimeSpan tmInterval = curTime - startTime;
		int iTotalSecs = tmInterval.GetTotalSeconds();
		if (iTotalSecs < 0)continue;

		int secInterval = tmInterval.GetTotalSeconds();
		if (secInterval > AutoSaveTime.iSecTimeTotal)
			pOpPanelDlg->m_bImgAuto = FALSE;
		if (secInterval >= nextTime*60)
		{
			nextTime += AutoSaveTime.iMinTimeInterval;
			pToolPanelDlg->SendMessage(WM_IMG_AS_TIME);

		}
		Sleep(100);
	}
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Image auto save End(Time)"), RGB(50, 50, 50), FALSE);
	pOpPanelDlg->SendMessage(WM_XRAY_OFF);
	return 0;
}

UINT COpPanelDlg::ThreadTempImgAutoSave(LPVOID pParam)
{
	COpPanelDlg * pOpPanelDlg = (COpPanelDlg *)pParam;
	CToolPanelDlg *pToolPanelDlg = pOpPanelDlg->m_pToolPanelDlg;
	tAutoSaveTemp AutoSaveTemp = pOpPanelDlg->m_ImgAutoSaveTemp;
	int iTempNext = AutoSaveTemp.iTempLimitLow;

	while (pOpPanelDlg->m_bImgAuto)
	{
		int iCurTemperature = CStatus::Instance()->m_lTemperature[AutoSaveTemp.iSensorNo - 1];
		iCurTemperature = iCurTemperature / 10;

		if (iCurTemperature >= iTempNext)
		{
			pToolPanelDlg->SendMessage(WM_IMG_AS_TEMP);
			iTempNext += AutoSaveTemp.iTempDelta;
		}

		if (iCurTemperature > AutoSaveTemp.iTempLimitHigh)pOpPanelDlg->m_bImgAuto = FALSE;
		Sleep(400);
	}
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Image auto save End(Temp)"), RGB(50, 50, 50), FALSE);
	pOpPanelDlg->SendMessage(WM_XRAY_OFF);

	return 0;
}

UINT COpPanelDlg::ThreadTimeAVIAutoSave(LPVOID pParam)
{
	COpPanelDlg * pOpPanelDlg = (COpPanelDlg *)pParam;
	CToolPanelDlg *pToolPanelDlg = pOpPanelDlg->m_pToolPanelDlg;
	tAutoSaveTime AutoSaveTime = pOpPanelDlg->m_AVIAutoSaveTime;

	CTime startTime = AutoSaveTime.tmStart;
	CTime curTime;

	int nextTime = AutoSaveTime.iMinTimeDuration;
	BOOL bIsStarted = FALSE;

	while (pOpPanelDlg->m_bAVIAuto)
	{
		curTime = CTime::GetCurrentTime();
		CTimeSpan tmInterval = curTime - startTime;
		int iTotalSecs = tmInterval.GetTotalSeconds();
		if (iTotalSecs < 0)continue;
		if (!bIsStarted)
		{
			pToolPanelDlg->SendMessage(WM_AVI_RECSTART);
			bIsStarted = TRUE;
		}
		int SecInterval = tmInterval.GetTotalSeconds();
		if ((SecInterval) >= AutoSaveTime.iSecTimeTotal)
		{
			pToolPanelDlg->SendMessage(WM_AVI_RECSTOP);
			pOpPanelDlg->m_bAVIAuto = FALSE;
			continue;
		}
		if( SecInterval > (nextTime*60)){
			bIsStarted = FALSE;
			pToolPanelDlg->SendMessage(WM_AVI_RECRESTART);
			nextTime += AutoSaveTime.iMinTimeDuration;
			bIsStarted = TRUE;
		//			Sleep(AVI_RESTART_DELAY);
		}
		Sleep(AVISAVE_POLLING_TIME);
	}

	CStatus::Instance()->m_bIsAVIRecording = FALSE;
	CTabDetailLogDlg::Instance()->SetTextLog(_T("AVI auto save End(Time)"), RGB(50, 50, 50), FALSE);
	pOpPanelDlg->SendMessage(WM_XRAY_OFF);
	return 0;
}

UINT COpPanelDlg::ThreadTempAVIAutoSave(LPVOID pParam)
{
	COpPanelDlg * pOpPanelDlg = (COpPanelDlg *)pParam;
	CToolPanelDlg *pToolPanelDlg = pOpPanelDlg->m_pToolPanelDlg;
	tAutoSaveTemp AutoSaveTemp = pOpPanelDlg->m_AVIAutoSaveTemp;
	int iTempStart = AutoSaveTemp.iTempLimitLow;
	int idurationNext = AutoSaveTemp.iMinTimeDuration;
	CTime curTime;
	CTime dtStartTime;
	BOOL bTempStarted = FALSE;
	dtStartTime = CTime::GetCurrentTime();
	CTimeSpan diffTime;
	BOOL bRecording = FALSE;
	while (pOpPanelDlg->m_bAVIAuto)
	{
		curTime = CTime::GetCurrentTime();
		int iCurTemperature = CStatus::Instance()->m_lTemperature[AutoSaveTemp.iSensorNo - 1];
		iCurTemperature = iCurTemperature / 10;

		if ( (iCurTemperature >= iTempStart) && (!bTempStarted))
		{
			bTempStarted = TRUE;
			dtStartTime = CTime::GetCurrentTime();
			pToolPanelDlg->SendMessage(WM_AVI_RECSTART);
			bRecording = TRUE;
		}

		if (bTempStarted)
		{
			diffTime = curTime - dtStartTime;

			if (diffTime.GetTotalMinutes() >= idurationNext)
			{
				bRecording = FALSE;
				pToolPanelDlg->SendMessage(WM_AVI_RECRESTART);
				dtStartTime = CTime::GetCurrentTime();
				bRecording = TRUE;
			}
		}

		if (iCurTemperature > AutoSaveTemp.iTempLimitHigh)
		{
			pOpPanelDlg->m_bAVIAuto = FALSE;
			if (bRecording)
			{
				pToolPanelDlg->SendMessage(WM_AVI_RECSTOP);
				bRecording = FALSE;
			}
		}

		Sleep(AVISAVE_POLLING_TIME);
	}

	if (bRecording)
	{
		pToolPanelDlg->SendMessage(WM_AVI_RECSTOP);
		bRecording = FALSE;
	}

	CStatus::Instance()->m_bIsAVIRecording = FALSE;
	CTabDetailLogDlg::Instance()->SetTextLog(_T("AVI auto save End(Temperature)"), RGB(50, 50, 50), FALSE);
	pOpPanelDlg->SendMessage(WM_XRAY_OFF);
	return 0;
}

void COpPanelDlg::OnBnClickedSelfocalspotlarge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Xray large focus"), RGB(50, 50, 50), FALSE);
	m_XFMSPacket.SendSetFocalSpot(TRUE);
}

LRESULT COpPanelDlg::OnXrayOff(WPARAM wparam, LPARAM lparam)
{
	if (m_bAVIAuto)return -1;
	if (m_bImgAuto)return -1;

	if (m_cRunStop.IsBlink())
		m_cRunStop.SetBlink(FALSE);

	if (m_cAAStart.IsBlink())
		m_cAAStart.SetBlink(FALSE);

	OnBnClickedBtnXrayoff();
	return 0;
}

void COpPanelDlg::SetDefault()
{
	SetDlgItemInt(IDC_EDT_IAS_TLMTLOW, 1000);
	SetDlgItemInt(IDC_EDT_IAS_TLMTHIGH, 1200);
	SetDlgItemInt(IDC_EDT_IAS_SENSORNO, 1);
	SetDlgItemInt(IDC_IAS_TDELTA_TEMP, 50);
	SetDlgItemInt(IDC_EDT_IAS_DELTATIME, 5);
	SetDlgItemInt(IDC_AAS_DURATION, 1);
	SetDlgItemInt(IDC_EDT_AAS_TLMTLOW, 1000);
	SetDlgItemInt(IDC_EDT_AAS_TLMTHIGH, 1200);
	SetDlgItemInt(IDC_EDT_AAS_SENSORNO, 1);
}
