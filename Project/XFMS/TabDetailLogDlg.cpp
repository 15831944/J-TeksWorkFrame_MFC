// TabDetailLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XFMS.h"
#include "TabDetailLogDlg.h"
#include <UniUtility.h>

static BOOL Settext=false;

COLORREF m_color;

#define COLOR_BLACK RGB( 0, 0, 0)
#define COLOR_BLUE RGB( 0, 0,128)
#define COLOR_GREEN RGB( 0,128, 0)
#define COLOR_CYAN RGB( 0,128,128)
#define COLOR_RED RGB(128, 0, 0)
#define COLOR_MAGENTA RGB(128, 0,128)
#define COLOR_BROWN RGB(128,128, 0)
#define COLOR_DARKGRAY RGB(128,128,128)
#define COLOR_LIGHTGRAY RGB(192,192,192)
#define COLOR_LIGHTBLUE RGB( 0, 0,255)
#define COLOR_LIGHTGREEN RGB( 0,255, 0)
#define COLOR_LIGHTCYAN RGB( 0,255,255)
#define COLOR_LIGHTRED RGB(255, 0, 0)
#define COLOR_LIGHTMAGENTA RGB(255, 0,255)
#define COLOR_YELLOW RGB(255,255, 0)
#define COLOR_WHITE RGB(255,255,255)


CTabDetailLogDlg* CTabDetailLogDlg::m_pInstance=NULL;
// CTabDetailLogDlg dialog

IMPLEMENT_DYNAMIC(CTabDetailLogDlg, CPropertyPage)

CTabDetailLogDlg::CTabDetailLogDlg()
	: CPropertyPage(CTabDetailLogDlg::IDD)

	, m_StringErr(_T(""))
{
}

CTabDetailLogDlg::~CTabDetailLogDlg()
{
	
}

void CTabDetailLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_BTNLOGTEXTCEAR, m_ClearBtn);

	DDX_Text(pDX, IDC_EDITLOG, m_StringErr);
	DDX_Control(pDX, IDC_EDITLOG, m_LogdataEditControl);
}


BEGIN_MESSAGE_MAP(CTabDetailLogDlg, CPropertyPage)
//	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTNLOGTEXTCEAR, &CTabDetailLogDlg::OnBnClickedTextLogClear)
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTabDetailLogDlg message handlers


/** \brief 여기서 Log 창과 배경과 Text 색상을 지정합니다.*/
//HBRUSH CTabDetailLogDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  여기서 DC의 특성을 변경합니다.
//
//
//   
//			HBRUSH brush;
//			if (pWnd->GetDlgCtrlID()==IDC_EDITLOG)
//			{
//			brush=CreateSolidBrush( COLOR_BLACK); // 에디트창 의 배경
//			pDC->SetBkMode(TRANSPARENT);
//					 // 텍스트의 색깔을 흰색으로 한다. 
//			pDC->SetTextColor(m_color);
//			
//			hbr=brush;
//			}
//			else
//			{
//			//brush=CreateSolidBrush(COLOR_WHITE);
//			//hbr=brush; // 클라이언트 창의 배경
//			}
//
//            //DeleteObject(brush);    // 브러쉬 개체를 삭제한다.
//
//
// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
//	return hbr;
//}



/** \brief 다른 클래스에서 LOG 창을 이용할 수 있도록 하는 instance 함수*/
CTabDetailLogDlg * CTabDetailLogDlg::Instance(void)
{
	if(!m_pInstance)m_pInstance=new CTabDetailLogDlg; 

	return m_pInstance;
}



/* \brief 로그창에 TEXT 를 출력 하기 위한 함수
   한줄이 써질때마다 자동으로 다음줄로 계행 하도록 하였다.

   추가되는 Text 라인을 카운터하여 200 이되면 자동으로 Clear 되어 재시작되도록 하였다.

   \param CString Text  Log 창에 쓰여질 CString
*/
int len=0; // 지우기용텍스트창 (메모리늘어나기때문에)
void CTabDetailLogDlg::SetTextLog(CString Text,COLORREF RGBcolor,BOOL ERR)

{   
	//m_color=RGBcolor;
	
	if(ERR)
	{
	   ///////////////////LOG 저장하기 ////////////////////////

		CString strDir;
		strDir.Format(_T("..\\Setup\\LOGDATA.txt"));
		
		CFile* pcFile = new CFile(strDir,CFile::modeCreate | CFile::modeWrite);
		if(pcFile == NULL)
		{}
		else{
				CStringA strTemp, strBuff;
				strBuff = _T("LogData:\r\n");
				char buff[100000] = {NULL,};

				strBuff +=_T("\r\n");        

				strTemp.Format("%S",m_StringErr);  
				strBuff +=  strTemp; 

				sprintf_s(buff, strBuff.GetLength()+1, "%s", strBuff);
				pcFile->Write(buff,(UINT)strlen(buff));
				pcFile->Close();
			}
			delete pcFile;
   
	
	}

	else if(len>500)                          ///500자 이상이면 자동 클리어
	{
     ///////////////////LOG 저장하기 ////////////////////////
/*
    CString strDir;
	strDir.Format(_T("..\\Setup\\LOGDATA.txt"));
	
	CFile* pcFile = new CFile(strDir,CFile::modeCreate | CFile::modeWrite);
	if(pcFile == NULL)
	{}
	else{
		CStringA strTemp, strBuff;
		strBuff = _T("LogData:\r\n");
		char buff[100000] = {NULL,};

		strBuff +=_T("\r\n");;       // 모드 Laminography

		strTemp.Format("%S",m_StringErr);  // Last Angle
		strBuff +=  strTemp; 

		sprintf_s(buff, strBuff.GetLength()+1, "%s", strBuff);
		pcFile->Write(buff,(UINT)strlen(buff));
		pcFile->Close();
	}
	delete pcFile;
*/

  ///////////////////////////////////////////
	m_StringErr.Empty();
	Settext=true;
	len=0;
	}

	else
	{
     m_StringErr+=Text;
     m_StringErr+=_T("\r\n");
	 Settext=true;
     len=len+1;
	}
 
  
 }


BOOL CTabDetailLogDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
   	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	 
      brush1 = new CBrush(RGB(255,0,0));
   
	SetTimer(0,50,NULL);
  
	m_ClearBtn.SetIcon(IDI_CLEAR);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

/** \brief 강제적으로 Log 창에 출력된 Text 데이터를 삭제한다.*/
void CTabDetailLogDlg::OnBnClickedTextLogClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
   
    m_StringErr.Empty();
	Settext=true;
	
}


BOOL CTabDetailLogDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return CPropertyPage::OnCommand(wParam, lParam);
}

void CTabDetailLogDlg::OnRButtonDown(UINT nFlags, CPoint point)
{

	CPropertyPage::OnRButtonDown(nFlags, point);
}

void CTabDetailLogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    
	if(Settext)
	{
		UpdateData(false);
		m_LogdataEditControl.SetSel(-2, -1);
		Settext = false;
	}
	CPropertyPage::OnTimer(nIDEvent);
}

HBRUSH CTabDetailLogDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.


	switch (nCtlColor) 
	{
 
 		case CTLCOLOR_EDIT:
 		pDC->SetTextColor(RGB(255, 255, 255));
 		pDC->SetBkColor(RGB(0, 0, 0));
 		return 0;

		case CTLCOLOR_STATIC:
 		pDC->SetTextColor(RGB(255, 255, 255));
 		pDC->SetBkColor(RGB(0, 0, 0));
 	 	return 0;
     
		
		default:
 		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
 	}

      return (HBRUSH)(brush1->GetSafeHandle());

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
}

BOOL CTabDetailLogDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
 
	CRect winRC;
	GetClientRect(winRC);
	
	CBrush bkBrush(RGB(0, 0, 0));
	CBrush* pOldBrush = pDC->SelectObject(&bkBrush);
	pDC->Rectangle(winRC);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}
