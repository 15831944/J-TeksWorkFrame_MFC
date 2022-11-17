
// MilTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MilTest.h"
#include "MilTestDlg.h"
#include "ScrnHistoDlg.h"
#include "atlimage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog) 
END_MESSAGE_MAP()


// CMilTestDlg 대화 상자




CMilTestDlg::CMilTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMilTestDlg::IDD, pParent)
	, m_pRawData(NULL)
	, m_pScrnHistoDlg(NULL)
	, m_bUseAutoScale(FALSE)
	, m_uiBinValue(0)
	, m_bFillWindow(FALSE)
	, m_SaveType(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_uiBinValue = 5800;
}

void CMilTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_VIEW, m_ImageView);
	DDX_Control(pDX, IDC_IMG_SPLMT, m_imgSpplement);
	DDX_Check(pDX, IDC_CHK_AUTOSCALE, m_bUseAutoScale);
	DDX_Control(pDX, IDC_EDT_BINV, m_ctlBinValue);
	DDX_Text(pDX, IDC_EDT_BINV, m_uiBinValue);
	DDX_Control(pDX, IDC_SP_BINV, m_ctlSPBinValue);
	DDX_Check(pDX, IDC_CHK_FILLWND, m_bFillWindow);
}

BEGIN_MESSAGE_MAP(CMilTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPEN, &CMilTestDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CMilTestDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_LOADIMAGE, &CMilTestDlg::OnBnClickedBtnLoadimage)
	ON_BN_CLICKED(IDC_BTN_SELECT, &CMilTestDlg::OnBnClickedBtnSelect)
	ON_BN_CLICKED(IDC_BTN_SELWND, &CMilTestDlg::OnBnClickedBtnSelwnd)
	ON_BN_CLICKED(IDC_BTN_BLOB, &CMilTestDlg::OnBnClickedBtnBlob)
	ON_BN_CLICKED(IDC_BTN_IMCLOSE, &CMilTestDlg::OnBnClickedBtnImclose)
	ON_BN_CLICKED(IDC_BTN_PROC, &CMilTestDlg::OnBnClickedBtnProc)
	ON_BN_CLICKED(IDC_BTN_BIN, &CMilTestDlg::OnBnClickedBtnBin)
	ON_BN_CLICKED(IDC_BTN_GETATTR, &CMilTestDlg::OnBnClickedBtnGetattr)
	ON_BN_CLICKED(IDC_BTN_RESIZE, &CMilTestDlg::OnBnClickedBtnResize)
	ON_BN_CLICKED(IDC_BTN_PUT, &CMilTestDlg::OnBnClickedBtnPut)
	ON_BN_CLICKED(IDC_BTN_HISTOGRAM, &CMilTestDlg::OnBnClickedBtnHistogram)
	ON_BN_CLICKED(IDC_BTN_GMF, &CMilTestDlg::OnBnClickedBtnGmf)
	ON_BN_CLICKED(IDC_BTN_CLIPIMG, &CMilTestDlg::OnBnClickedBtnClipimg)
	ON_BN_CLICKED(IDC_BTN_FIND, &CMilTestDlg::OnBnClickedBtnFind)
//	ON_BN_CLICKED(IDC_BTN_EXTRACTATTR, &CMilTestDlg::OnBnClickedBtnExtractattr)
	ON_BN_CLICKED(IDC_AVR, &CMilTestDlg::OnBnClickedAvr)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CMilTestDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTNLVLWND, &CMilTestDlg::OnBnClickedBtnlvlwnd)
	ON_BN_CLICKED(IDC_BTNHISTOTEST, &CMilTestDlg::OnBnClickedBtnhistotest)
	ON_BN_CLICKED(IDC_BTN_GETMAXMIN, &CMilTestDlg::OnBnClickedBtnGetmaxmin)
	ON_BN_CLICKED(IDC_GMF_SAVE, &CMilTestDlg::OnBnClickedGmfSave)
	ON_BN_CLICKED(IDC_IMPORT, &CMilTestDlg::OnBnClickedImport)
	ON_BN_CLICKED(IDC_ZOOM, &CMilTestDlg::OnBnClickedZoom)
	ON_BN_CLICKED(IDC_BTN_PAN, &CMilTestDlg::OnBnClickedBtnPan)
	ON_BN_CLICKED(IDC_BTN_SHOWCONTEXT, &CMilTestDlg::OnBnClickedBtnShowcontext)
//	ON_BN_CLICKED(IDC_BTN_PROP, &CMilTestDlg::OnBnClickedBtnProp)
ON_BN_CLICKED(IDC_CHK_AUTOSCALE, &CMilTestDlg::OnBnClickedChkAutoscale)
ON_BN_CLICKED(IDC_CHK_FILLWND, &CMilTestDlg::OnBnClickedChkFillwnd)
ON_BN_CLICKED(IDC_CALCORD, &CMilTestDlg::OnBnClickedCalcord)
ON_BN_CLICKED(IDC_BTNJPGSAVE, &CMilTestDlg::OnBnClickedBtnjpgsave)
ON_BN_CLICKED(IDC_BTN_LOADRAWIMAGE, &CMilTestDlg::OnBnClickedBtnLoadrawimage)
END_MESSAGE_MAP()


// CMilTestDlg 메시지 처리기

BOOL CMilTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);		// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
//	m_pScrnHistoDlg = new CScrnHistoDlg;
//	m_pScrnHistoDlg->Create(CScrnHistoDlg::IDD, this);
	m_SaveType = 0;
	m_ctlSPBinValue.SetRange32(0, 65535);
	UpdateData(FALSE);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMilTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMilTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMilTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMilTestDlg::OnBnClickedBtnOpen()
{
	// TODO: Add your control notification handler code here
	if (myMilTest.DefaultSetting())
	{
		AfxMessageBox(_T("Cann't open default setting"));
		return;
	}
	m_pImage = myMilTest.NewImage();
	m_pMyDisplay = myMilTest.GetDefaultDisp();
	m_pMySecondDisplay = myMilTest.NewDisplay();
}

void CMilTestDlg::OnBnClickedBtnClose()
{
	// TODO: Add your control notification handler code here
	m_pImage->Close();
	m_pMyDisplay->Close();
	m_pMySecondDisplay->Close();
	myMilTest.Close();
}

void CMilTestDlg::OnBnClickedBtnLoadimage()
{
	// TODO: Add your control notification handler code here
	CFileDialog LoadFile(TRUE, _TEXT("*.bmp;*.jpg"));
	if(LoadFile.DoModal())
	{
		m_pImage->Restore((WCHAR *)(LoadFile.GetPathName().GetString()));
//		pMyImageProcess->ArithConst(m_pImage, m_pImage, 10, M_SUB_CONST);
	}
}

void CMilTestDlg::OnBnClickedImport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog LoadFile(TRUE, _TEXT("*.tiff;*.tif"));
	if (LoadFile.DoModal())
	{
		m_pImage->BufImport((WCHAR *)(LoadFile.GetPathName().GetString()), M_TIFF);
	}
}

void CMilTestDlg::OnBnClickedBtnSelect()
{
	// TODO: Add your control notification handler code here
	m_pImage->Select(m_pMyDisplay);
	m_pImage->GetDisplay()->EnableOverlay();
	m_pMyDisplay->EnableOverlay();
}

void CMilTestDlg::OnBnClickedBtnSelwnd()
{
	// TODO: Add your control notification handler code here
	m_pImage->SelectWindow(m_pMyDisplay, &m_ImageView);
	m_pImage->SelectWindow(m_pMySecondDisplay, &m_imgSpplement);
}


void CMilTestDlg::OnBnClickedBtnImclose()
{
	// TODO: Add your control notification handler code here
	CMilIm::Close(m_pImage, m_pImage);
}

void CMilTestDlg::OnBnClickedBtnProc()
{
	// TODO: Add your control notification handler code here
	CMilIm::Convolve(m_pImage, m_pImage, M_DERICHE_FILTER( M_SHARPEN, M_DEFAULT));
}

void CMilTestDlg::OnBnClickedBtnBin()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	CMilIm::Open(m_pImage, m_pImage, 1);
	CMilIm::Binarize(m_pImage, m_pImage, m_uiBinValue);
}

void CMilTestDlg::OnBnClickedBtnGetattr()
{
	// TODO: Add your control notification handler code here
	tImageProperties ImageProperty;
	m_pImage->GetImageProperties(&ImageProperty);
	CString strMsg;

	strMsg.Format(_TEXT("Image Size x:%d, y:%d BandSize:%d Depth:%d"),ImageProperty.ImageSize.cx, ImageProperty.ImageSize.cy, ImageProperty.BandSize, ImageProperty.Depth);
	AfxMessageBox(strMsg);
}

void CMilTestDlg::OnBnClickedBtnResize()
{
	// TODO: Add your control notification handler code here
	CMilIm::Resize(m_pImage, m_pImage, 0.5, 0.5);
	
}

void CMilTestDlg::OnBnClickedBtnPut()
{
	// TODO: Add your control notification handler code here
	CFileDialog LoadFile(TRUE, _TEXT("*.raw"));
	if(LoadFile.DoModal())
	{
	}else return;

	ULONGLONG  FileSize;
	CFile RawImageFile;
	if (RawImageFile.Open(LoadFile.GetPathName(), CFile::modeRead))
	{
		FileSize=RawImageFile.GetLength();
	}else return;
	m_pRawData=(short *)malloc(FileSize);
	memset(m_pRawData,1, FileSize);
	
	RawImageFile.Read(m_pRawData, FileSize);
	RawImageFile.Close();
/*	for(int i=0;i<FileSize;i+=2)
	{
		BYTE temp=((char *)m_pRawData)[i];
		((char *)m_pRawData)[i]=((char *)m_pRawData)[i+1];
		((char *)m_pRawData)[i+1]=temp;
	}*/

	m_pImage->SetDepth(16);
	m_pImage->Alloc2D(CSize(1000, 1000));
	m_pImage->SelectWindow(m_pMyDisplay, &m_ImageView);
	m_pImage->Put2D(CSize(0,0), CSize(1000,1000), m_pRawData);
	m_pImage->SelectWindow(m_pMyDisplay, &m_ImageView);
//	m_pMyDisplay->Control(M_FILL_DISPLAY, M_ENABLE);
	m_pMyDisplay->FilWindow();
	//	m_pMyDisplay->Control(M_INTERPOLATION_MODE,M_DEFAULT);
//	m_pImage->Put2D(CSize(0,0), CSize(1000,1000), m_pRawData);
}

void CMilTestDlg::OnBnClickedBtnHistogram()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	long temp[65536];
	m_pOrgHistoData = new double[65536];
	CMilIm::Histogram(m_pImage, 65535, temp);
	for(int i=0;i<65536;i++)m_pOrgHistoData[i]=temp[i];
	m_pScrnHistoDlg->m_pHistoValueOrg=m_pOrgHistoData;
	m_pScrnHistoDlg->ShowWindow(SW_SHOW);
}

void CMilTestDlg::OnBnClickedBtnBlob()
{
	// TODO: Add your control notification handler code here
	CMilBlob BlobTest(m_pImage);
	BlobTest.SetBuffer(m_pImage);
	BlobTest.m_milIDSystem = m_pImage->m_milIDSystem;
	BlobTest.AllocFeatureList();
	BlobTest.SelectFeature(M_AREA);
	BlobTest.SelectFeature(M_CENTER_OF_GRAVITY);
	BlobTest.SelectFeature(M_BOX_X_MIN);
	BlobTest.SelectFeature(M_BOX_Y_MIN);
	BlobTest.SelectFeature(M_BOX_X_MAX);
	BlobTest.SelectFeature(M_BOX_Y_MAX);
	BlobTest.AllocResult();
	BlobTest.Caculate();
	BlobTest.m_pMilBuffer=m_pImage;
	BlobTest.Draw(m_pMyDisplay);
	//	BlobTest.Select(M_EXCLUDE, M_AREA, M_LESS_OR_EQUAL, 128, M_NULL);
	int i=BlobTest.GetNumber();
	CString msg;
	msg.Format(_TEXT("Count:%d"),i);
	AfxMessageBox(msg);
}

void CMilTestDlg::OnBnClickedBtnGmf()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMilModFinder Finder;
	CMilContext ctxtFirst;
	CMilResult milResult;
	tModResult myResult;

	
	ctxtFirst.RestoreContext(_T("D:\\first.ctx"));
	
	Finder.SetBuffer(m_pImage);
	Finder.SetContext(&ctxtFirst);
	Finder.SetResult(&milResult);
	Finder.Alloc();
	Finder.PreProcess();
	Finder.Find(&myResult);
	Finder.DrawResult();

	//Finder.SetContext(&ctxtFirst);
	//Finder.SetBuffer(m_pImage);
	//Finder.PreProcess();
	//Finder.Find();
	//	Finder

}

void CMilTestDlg::OnBnClickedBtnClipimg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pMyImageProcess = new CMilIm;

	CFileDialog LoadFile(TRUE, _TEXT("*.bmp;*.jpg;*.tif"));
	if(LoadFile.DoModal())
	{
		m_pImage->Restore(LoadFile.GetPathName());
		CMilBuffer *testImg = myMilTest.NewImage(m_pImage);
		testImg->Alloc2D(CSize(500, 500));
		m_pImage->CopyColor2D(testImg, CRect(0, 0, 500, 500));
		testImg->Export(_T("D:\\test.tif"));
	}
}

void CMilTestDlg::OnBnClickedBtnFind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMilTestDlg::OnBnClickedAvr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileFind AtomImageFile;
	CString nameFolder;
	CString nameImg;
	CPtrList ImgList;
	int cntImg=0;

	m_pImage = myMilTest.NewImage();
//	m_pAddImages = myMilTest.NewImage();
	pImgData = myMilTest.NewImage();

	m_pAddImages = new CMilBuffer;
	m_pAddImages->SetDepth(32+M_UNSIGNED);
	m_pAddImages->SetAttribute(M_IMAGE+M_PROC);
	m_pAddImages->Alloc2D(CSize(1024,1024));
	m_pAddImages->Clear();
	m_pAddImages->Export(_T("d:\\clear.tif"));
	CMilBuffer *pDarkImg;
	pDarkImg =myMilTest.NewImage();
	pDarkImg->Restore(_T("D:\\DarkImg.tif"));

	CFolderDialog ImgFolder;
	if(ImgFolder.DoModal())
	{
		nameFolder=ImgFolder.GetPathName();
		nameFolder+=_T("\\");
		if(AtomImageFile.FindFile(nameFolder+_T("*.tif")))
		{
			if(AtomImageFile.FindNextFile())
			{
				nameImg=AtomImageFile.GetFileName();
				nameImg=nameFolder+nameImg;
				pImgData->Restore(nameImg.GetBuffer());	
				CMilIm::Arith(pImgData, pImgData, pDarkImg, M_SUB);
				m_pAddImages->Copy(pImgData);
				m_pAddImages->Export(_T("d:\\first.tif"));
				cntImg++;

				while(AtomImageFile.FindNextFile())
				{
					nameImg=AtomImageFile.GetFileName();
					nameImg=nameFolder+nameImg;
					pImgData->Restore(nameImg.GetBuffer());	
					CMilIm::Arith(pImgData, pImgData, pDarkImg, M_SUB);
					cntImg++;
					CMilIm::Arith(m_pAddImages, m_pAddImages, pImgData,  M_ADD);

				}
				nameImg=AtomImageFile.GetFileName();
				nameImg=nameFolder+nameImg;
				pImgData->Restore(nameImg.GetBuffer());	
				CMilIm::Arith(pImgData, pImgData, pDarkImg, M_SUB);
				cntImg++;
				CMilIm::Arith(m_pAddImages, m_pAddImages, pImgData,  M_ADD);

			}
			m_pAddImages->Export(_T("D:\\Added.tif"));

			CString msg;
			msg.Format(_T("Image Count:%d"), cntImg);
			AfxMessageBox(msg);
			CMilIm::ArithConst(pImgData, m_pAddImages, cntImg,  M_DIV_CONST);
			
			pImgData->Export(_T("D:\\AvgImg.tif"));
			pImgData->Select(m_pMyDisplay);
		}

	}
}

void CMilTestDlg::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	switch (m_SaveType)
	{
	case 0:
		m_pImage->Export(_T("d:\\test.bmp"), M_BMP);
		break;
	case 1:
		m_pImage->Export(_T("d:\\test.jpg"), M_JPEG2000_LOSSY);
		break;
	case 2:
		m_pImage->Export(_T("d:\\test.tif"), M_TIFF);
		break;
	}
}


void CMilTestDlg::OnBnClickedBtnlvlwnd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pImage->WndLvlTranform(m_pImage, m_pScrnHistoDlg->m_SliderLeft, m_pScrnHistoDlg->m_SliderRight,0,4095);
}

void CMilTestDlg::OnBnClickedBtnhistotest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	long temp[65536];
	m_pOrgHistoData = new double[65536];
	m_pDestHistoData = new double[65536];
	CMilIm::Histogram(m_pImage, 65535, temp);
	for(int i=0;i<65536;i++)m_pOrgHistoData[i]=temp[i];
	m_pScrnHistoDlg->m_pHistoValueOrg=m_pOrgHistoData;

//	m_pImage->WndLvlTranform(m_pImage, m_pScrnHistoDlg->m_SliderLeft, m_pScrnHistoDlg->m_SliderRight,0,4095);
	m_pImage->WndLvlTranform(m_pImage, 1523, 3601, 0,65535);

	CMilIm::Histogram(m_pImage, 65535, temp);
	for(int i=0;i<65536;i++)m_pDestHistoData[i]=temp[i]; 
	m_pScrnHistoDlg->m_pHistoValueModified=m_pDestHistoData;
	m_pScrnHistoDlg->ShowWindow(SW_SHOW);
}

void CMilTestDlg::OnBnClickedBtnGetmaxmin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	long lMax, lMin;
	//CMilBuffer *pNewChild = m_pImage->Child2D(10, 10, 100, 100);
	//CMilIm::GetMinMax(pNewChild, &lMin, &lMax);
	CMilIm::GetMinMax(m_pImage, &lMin, &lMax);

	CString strMsg;

	strMsg.Format(_TEXT("Max:%d, Min:%d"), lMax, lMin);
	AfxMessageBox(strMsg);

}

void CMilTestDlg::OnBnClickedGmfSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMilModFinder Finder;
	CMilContext ctxtFirst;
	CMilResult milResult;


	Finder.SetBuffer(m_pImage);
	Finder.SetContext(&ctxtFirst);
	Finder.SetResult(&milResult);
	Finder.Alloc();
	Finder.GetFeature(CRect(396,408, 464, 490));
	//	Finder.GetFeature();
	ctxtFirst.SaveContext(_T("D:\\first.ctx"));
	m_pImage->GetDisplay()->EnableOverlay();
	m_pImage->GetDisplay()->EnableGraph();

	Finder.DrawContext();

}


void CMilTestDlg::OnBnClickedZoom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pImage->GetDisplay()->SetZoomFactor(0.5,0.5);
}


void CMilTestDlg::OnBnClickedBtnPan()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pViewDlg = new CViewDlg;
	m_pViewDlg->Create(IDD_VIEW);
	m_pViewDlg->ShowWindow(SW_SHOW);
	m_pViewDlg->SetScrollInfo(SB_CTL, FALSE);

}

void CMilTestDlg::OnBnClickedBtnShowcontext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMilModFinder Finder;
	CMilResult milResult;
	tModResult myResult;

	ctxtFirst.RestoreContext(_T("D:\\0.ctx"));

	ctxtFirst.GetImage(m_pImage);

}


void CMilTestDlg::OnBnClickedChkAutoscale()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if (m_bUseAutoScale)
	{
		m_pMyDisplay->SetViewAutoScale();
		m_pMySecondDisplay->SetViewAutoScale();
		m_pImage->Modified();
	}
	else
	{
		m_pMyDisplay->SetViewAutoScale(FALSE);
		m_pMySecondDisplay->SetViewAutoScale(FALSE);
		m_pImage->Modified();
	}
}


void CMilTestDlg::OnBnClickedChkFillwnd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if (m_bFillWindow)m_pMyDisplay->FilWindow();
	else m_pMyDisplay->FilWindow(FALSE);
}


void CMilTestDlg::OnBnClickedCalcord()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMilCal CalTest;
	double dblX, dblY;
	MIL_ID CalibrationStatus;
	CalTest.Alloc();
	CalTest.Grid(m_pImage, 0, 0, 0, 13, 12, 100, 100);
	CalTest.Inquire(&CalibrationStatus);
	if (CalibrationStatus == M_CALIBRATED)
	{
		CalTest.TransformCoordinate(41, 377, &dblX, &dblY);
		AfxMessageBox(_T("Wait"));
	}
	CalTest.Free();

	//MIL_ID ImageID = m_pImage->GetIDBuffer();
	//MIL_ID SystemID = m_pImage->m_milIDSystem;
	//MIL_ID CalID;
	//MIL_ID CalibrationStatus;
	//int XPos = 0;
	//int YPos = 0;
	//McalAlloc(SystemID, M_DEFAULT, M_DEFAULT, &CalID);
	//McalGrid(CalID, ImageID, 0, 0, 0, 13, 12, 100, 100, M_DEFAULT, M_DEFAULT);

	//int rv=McalInquire(CalID, M_CALIBRATION_STATUS + M_TYPE_MIL_INT, &CalibrationStatus);
	//if (CalibrationStatus == M_CALIBRATED)
	//{
	//	XPos = 100; YPos = 100;

	//	double dblX, dblY;
	//	McalTransformCoordinate(CalID, M_PIXEL_TO_WORLD, 41, 377, &dblX, &dblY);
	//	AfxMessageBox(_T("Wait"));
	//}
}


void CMilTestDlg::OnBnClickedBtnjpgsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CImage tempImg;
	COLORREF color;
	
	tImageProperties ImageProperty;
	m_pImage->GetImageProperties(&ImageProperty);
	int cx = ImageProperty.ImageSize.cx;
	int cy = ImageProperty.ImageSize.cy;
	int x, y;
	tempImg.Create(cx, cy, 24);
	for (y = 0; y < cy; y++)
		for (x = 0; x < cx; x++)
		{
			m_pImage->GetPixelColor(x, y, &color);
			tempImg.SetPixel(x, y, color);

		}
	tempImg.Save(_T("Save.jpg"));
}


void CMilTestDlg::OnBnClickedBtnLoadrawimage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog LoadFile(TRUE, _TEXT("*.raw"));
	m_pImage->SetDepth(16);
	m_pImage->SetAttribute(M_IMAGE+M_DISP+M_PROC);
	if (LoadFile.DoModal())
	{
		CSize rawFileSize(1316, 1312);
		m_pImage->LoadRawFile((WCHAR *)(LoadFile.GetPathName().GetString()), rawFileSize);
	}
}
