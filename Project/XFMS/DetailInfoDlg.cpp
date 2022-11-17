// DetailInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XFMS.h"
#include "DetailInfoDlg.h"


// CDetailInfoDlg dialog
CDetailInfoDlg* CDetailInfoDlg::m_pInstance=NULL;

IMPLEMENT_DYNAMIC(CDetailInfoDlg, CDialog)

CDetailInfoDlg::CDetailInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailInfoDlg::IDD, pParent)
{

}

CDetailInfoDlg::~CDetailInfoDlg()
{
}

void CDetailInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CDetailInfoDlg, CDialog)
	
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDetailInfoDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CDetailInfoDlg message handlers

BOOL CDetailInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    

	::SetWindowLong( this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 0, LWA_ALPHA);  // 투명윈도우
	// TODO:  Add extra initialization here
	initTabContain();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


/*int CDetailInfoDlg::DisplayAllMapInfo()
{
	 CString DIRIamge;

     CStarTestApp *pApp=(CStarTestApp *)AfxGetApp();
	 CSize BufSize;
	 DIRIamge.Format(_T("..\\Setup\\PCBImage\\AllImage.bmp"));
 	 m_DisplayAllMap=pApp->m_pMilVLib->NewImage();
	 m_DisplayAllMap->Restore((LPWSTR)(LPCTSTR)DIRIamge);

	//m_DisplayAllMap->BufImport(DIRIamge.GetBuffer(), M_BMP, M_LOAD);
	 m_DisplayAllMap->Restore((LPWSTR)(LPCTSTR)DIRIamge);
	m_DisplayAllMap->SelectWindow(this);
	m_DisplayAllMap->GetDisplay()->Control(M_FILL_DISPLAY,M_ENABLE);



	return 0;
}
*/

void CDetailInfoDlg::initTabContain()

{
   CTabDetailLogDlg *m_pTabDetailLogDlg;

    CString nameTab;
	CRect WinRect;
//	m_ImageList.Create(IDB_TAB_SIDEINFO, 11, 0, RGB(0, 0, 0));
//	m_ImageList.Create(IDB_TAB_SIDEINFO, 11, 0, RGB(0, 0, 0));
//	m_TabCtrl.SetImageList(&m_ImageList);
	m_TabCtrl.GetWindowRect(&WinRect);
	nameTab.Format(_T("Log"));
	m_pTabDetailLogDlg = new CTabDetailLogDlg;
	m_pTabDetailLogDlg->m_pInstance=m_pTabDetailLogDlg;
	m_pTabDetailLogDlg->Create(CTabDetailLogDlg::IDD, &m_TabCtrl);
	m_pTabDetailLogDlg->MoveWindow(50,0,WinRect.Width()-10,WinRect.Height()-10);
	m_TabCtrl.AddTab(m_pTabDetailLogDlg, nameTab.GetBuffer(), 7);

	//nameTab.Format((_T("Result")));
	//m_pTabDetailResultDlg = new CTabDetailResultDlg;
	//m_pTabDetailResultDlg->m_pInstance=m_pTabDetailResultDlg;
	//m_pTabDetailResultDlg->Create(CTabDetailResultDlg::IDD, &m_TabCtrl);
	//m_pTabDetailResultDlg->MoveWindow(50,0,WinRect.Width()-10,WinRect.Height()-10);
	//m_TabCtrl.AddTab(m_pTabDetailResultDlg, nameTab.GetBuffer(), 5);

	//m_TabCtrl.SetSelectedColor(RGB(0,0,0));
	//m_TabCtrl.SetMouseOverColor((RGB(0,0,0)));


	CBitmap bmp;

    bmp.LoadBitmap(IDB_BITMAIN);
    static CImageList imgList;
    imgList.Create(16, 16, ILC_COLOR24 | ILC_MASK, 0,0);
    imgList.Add(&bmp, RGB(192,192,192));
    m_TabCtrl.SetImageList(&imgList);

}


CDetailInfoDlg * CDetailInfoDlg::Instance(void)
{
	if(!m_pInstance)m_pInstance= new CDetailInfoDlg; 

	return m_pInstance;
}

void CDetailInfoDlg::ShowResultPage(void)
{
	m_TabCtrl.SelectTab(1);
//	m_pDetailPropDlg->SetActivePage(1);
}

BOOL CDetailInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDetailInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
    
	/*CBrush m_Brush;
    m_Brush.CreateSolidBrush(RGB(255,0,0));
	int nRet = pWnd->GetDlgCtrlID();
	if(nRet == IDC_TAB1)
	{
		pDC->SetBkColor(RGB(0,0,0)); // 글자가 선택된 곳의 색 
		//pDC->SetTextColor(RGB(0,255,0)); // 글자색
		   //배경색
		return m_Brush;
	}*/


	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CDetailInfoDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
