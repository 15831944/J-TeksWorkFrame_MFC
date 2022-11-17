// ComsetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "ComsetDlg.h"
#include "afxdialogex.h"
#include "Status.h"

// CComSetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CComSetDlg, CDialogEx)

CPtrList * CComSetDlg::m_pLstComsName = NULL;
CPtrList * CComSetDlg::m_pLstComsNo = NULL;

CComSetDlg::CComSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMSET, pParent)
{
	m_pLstComsName = new(CPtrList);
	m_pLstComsNo = new(CPtrList);
}

CComSetDlg::~CComSetDlg()
{
}

void CComSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_CBPORTNO, m_strPortNo);
	DDX_CBIndex(pDX, IDC_CBBAUDRATE, m_iBaudRate);
	DDX_Control(pDX, IDC_CBPORTNO, m_cPortNo);
	DDX_Control(pDX, IDC_CBBAUDRATE, m_cBaudRate);
}


BEGIN_MESSAGE_MAP(CComSetDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_CBPORTNO, &CComSetDlg::OnCbnSelchangeCbportno)
	ON_BN_CLICKED(IDOK, &CComSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CComSetDlg 메시지 처리기입니다.


void CComSetDlg::OnCbnSelchangeCbportno()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CComSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	int i = 0;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetComPortEnum();

	m_cBaudRate.AddString(_T("9600"));
	m_cBaudRate.AddString(_T("19200"));
	m_cBaudRate.AddString(_T("38400"));
	m_cBaudRate.AddString(_T("115200"));

	CString strComportNo = CStatus::Instance()->GetComportNo();
	if ((i = FindIndex(strComportNo.GetBuffer(), &m_cPortNo)) >= 0)
	{
		m_cPortNo.SetCurSel(i);
	}


	CString strBaudRate;
	strBaudRate.Format(_T("%d"), CStatus::Instance()->GetBaudRate());
	if((i = FindIndex(strBaudRate.GetBuffer(), &m_cBaudRate))>=0)
	{
		m_cBaudRate.SetCurSel(i);
	}
	UpdateData(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CALLBACK CComSetDlg::SerialEnum(LPVOID lpCallbackValue, LISTPORTS_PORTINFO* lpPortInfo)
{
	CString *pComString;
	pComString = new(CString);
	*(pComString) = lpPortInfo->lpPortName;
	m_pLstComsNo->AddTail(pComString);
	pComString = new(CString);
	*(pComString) = lpPortInfo->lpFriendlyName;
	m_pLstComsName->AddTail(pComString);
	return TRUE;
}

CString CComSetDlg::GetComPortEnum()
{
	POSITION pos;
	CString *pString;
	ListPorts(SerialEnum, NULL);
	Sleep(10);

	pos = m_pLstComsNo->GetHeadPosition();
	int i = 0;
	do {
		pString = (CString *)m_pLstComsNo->GetAt(pos);
		m_cPortNo.AddString(*pString);
		m_pLstComsNo->GetNext(pos);

	} while (pos != NULL);

	return CString();
}


void CComSetDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strBaudRate;
	long iBaudRate;
	CString strComportNo;
	m_cBaudRate.GetLBText(m_cBaudRate.GetCurSel(), strBaudRate);
	m_cPortNo.GetLBText(m_cPortNo.GetCurSel(), strComportNo);

	iBaudRate = _wtoi(strBaudRate.GetBuffer());
	CStatus::Instance()->SetBaudRate(iBaudRate);
	CStatus::Instance()->SetComportNo(strComportNo.GetBuffer());
	CStatus::Instance()->SetDefault();
	CDialogEx::OnOK();
}


int CComSetDlg::FindIndex(TCHAR *szBaudRate, CComboBox * pComboBox)
{
	int i=pComboBox->FindString(0, szBaudRate);
	return i;
}
