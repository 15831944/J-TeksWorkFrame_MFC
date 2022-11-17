// CamRemoteDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "SCSocket.h"
#include "CamRemoteDlg.h"
#include "afxdialogex.h"
#include "Repository.h"
#include "Status.h"
#include "SCClientSocket.h"

// CCamRemoteDlg 대화 상자입니다.


IMPLEMENT_DYNAMIC(CCamRemoteDlg, CDialogEx)

int CCamRemoteDlg::m_iRemoteIndex = 0;

CCamRemoteDlg::CCamRemoteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAMREMOTE, pParent)
	, m_pMilCamView(NULL)
	, m_pMilDisp(NULL)
	, m_iCamBuildNo(0)
	, m_strCamName(_T(""))
	, m_pSockContainer(NULL)
{

}

CCamRemoteDlg::~CCamRemoteDlg()
{
}

void CCamRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCamRemoteDlg, CDialogEx)
	ON_MESSAGE(WM_CAM_REMOTE, &CCamRemoteDlg::OnReceiveCMD)
	ON_MESSAGE(WM_SOCK_LISTEN, &CCamRemoteDlg::OnSockInitListen)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CCamRemoteDlg 메시지 처리기입니다.


BOOL CCamRemoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if (!m_pMilCamView)
	{
		m_pMilCamView = CRepository::Instance()->m_MilLib.NewImage();
	}

	m_pMilDisp = CRepository::Instance()->m_MilLib.NewDisplay();
	InitRemoteCam();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


LRESULT CCamRemoteDlg::OnReceiveCMD(WPARAM wparam, LPARAM lparam)
{
	USES_CONVERSION;
	char szBuffer[1024];
	int nCountOfRcv = ((CSCSocket *)wparam)->Receive(szBuffer, sizeof(szBuffer));
	szBuffer[nCountOfRcv] = 0;
	CString strFileName = A2CT(szBuffer);
	LoadFile(strFileName);
	return 0;
}


void CCamRemoteDlg::LoadFile(CString strRemoteFileName)
{
	CString strRemoteCamFolder = CStatus::Instance()->m_strRemoteCamFolder;
	CString strFileName = strRemoteCamFolder + strRemoteFileName;
	if (!PathFileExists(strFileName))
	{
		return;//there is no file.
	}

	if (m_pMilCamView)
	{
		if (m_pMilCamView->IsSelectDisp())m_pMilCamView->UnSelectDisp();
		m_pMilCamView->Close();
	}

	m_pMilCamView->Restore(strFileName);
	m_pMilCamView->SelectWindow(m_pMilDisp, this);
	m_pMilDisp->FilWindow();
}


BOOL CCamRemoteDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CCamRemoteDlg::InitRemoteCam()
{
	InitSockListen();
	m_iRemoteIndex++;
}


LRESULT CCamRemoteDlg::OnSockInitListen(WPARAM wparam, LPARAM lparam)
{
	m_pSockContainer->CloseClient();
	return LRESULT();
}


void CCamRemoteDlg::InitSockListen()
{
	CString strIP = CStatus::Instance()->GetRemoteIPString();
	
	if (m_pSockContainer) {
		m_pSockContainer->ShutDown();
		m_pSockContainer->CloseClient();
		m_pSockContainer->Close();
		delete m_pSockContainer;
	}
	m_pSockContainer = new CSCSocket;
	m_pSockContainer->Init(this, BASE_SCREMOTEPORTNO + m_iRemoteIndex);
	m_pSockContainer->Listen();
}


void CCamRemoteDlg::CreateSockListen()
{
}

