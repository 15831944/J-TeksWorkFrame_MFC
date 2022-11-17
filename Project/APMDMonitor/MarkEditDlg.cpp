// MarkEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "MarkEditDlg.h"
#include "afxdialogex.h"
#include "ViewOptionDlg.h"
// CMarkEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMarkEditDlg, CDialogEx)

CMarkEditDlg::CMarkEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MARKEDIT, pParent)
	, m_strMarkEdit(_T(""))
	, m_pViewOptionDlg(NULL)
{

}

CMarkEditDlg::~CMarkEditDlg()
{
}

void CMarkEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_MARKNAME, m_strMarkEdit);
	DDX_Control(pDX, IDC_LSTB_MARK, m_lstbMarks);
}


BEGIN_MESSAGE_MAP(CMarkEditDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD, &CMarkEditDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_ITEMDEL, &CMarkEditDlg::OnBnClickedBtnItemdel)
	ON_BN_CLICKED(IDC_BTN_ITEMINS, &CMarkEditDlg::OnBnClickedBtnItemins)
	ON_BN_CLICKED(IDC_BTN_ITEMUPDATE, &CMarkEditDlg::OnBnClickedBtnItemupdate)
	ON_LBN_SELCHANGE(IDC_LSTB_MARK, &CMarkEditDlg::OnLbnSelchangeLstbMark)
	ON_BN_CLICKED(IDC_BTN_ITEMUP, &CMarkEditDlg::OnBnClickedBtnItemup)
	ON_BN_CLICKED(IDC_BTN_ITEMPDOWN, &CMarkEditDlg::OnBnClickedBtnItempdown)
	ON_BN_CLICKED(IDC_BTN_ITEMLOAD, &CMarkEditDlg::OnBnClickedBtnItemload)
	ON_BN_CLICKED(IDC_BTN_ITEMSAVE, &CMarkEditDlg::OnBnClickedBtnItemsave)
	ON_BN_CLICKED(IDOK, &CMarkEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMarkEditDlg 메시지 처리기입니다.


BOOL CMarkEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadMark();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMarkEditDlg::OnBnClickedBtnAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_strMarkEdit.Trim();
	if (m_strMarkEdit.IsEmpty())return;
	m_lstbMarks.AddString(m_strMarkEdit.GetBuffer());
	m_strMarkEdit = "";
	UpdateData(FALSE);
}


void CMarkEditDlg::OnBnClickedBtnItemdel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_lstbMarks.DeleteString(m_lstbMarks.GetCurSel());
}


void CMarkEditDlg::OnBnClickedBtnItemins()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_strMarkEdit.Trim();
	m_lstbMarks.InsertString(m_lstbMarks.GetCurSel(), m_strMarkEdit.GetBuffer());
}


void CMarkEditDlg::OnBnClickedBtnItemupdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int selNo = m_lstbMarks.GetCurSel();
	m_lstbMarks.DeleteString(selNo);
	m_lstbMarks.InsertString(selNo, m_strMarkEdit.GetBuffer());
}


void CMarkEditDlg::OnLbnSelchangeLstbMark()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int selNo = m_lstbMarks.GetCurSel();
	m_lstbMarks.GetText(selNo, m_strMarkEdit);
	UpdateData(FALSE);

}


void CMarkEditDlg::OnBnClickedBtnItemup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int selNo = m_lstbMarks.GetCurSel();
	if (selNo == 0)return;

	CString strToMove;
	m_lstbMarks.GetText(selNo, strToMove);
	m_lstbMarks.DeleteString(selNo);
	m_lstbMarks.InsertString(selNo - 1, strToMove);
	m_lstbMarks.SetCurSel(selNo - 1);
}


void CMarkEditDlg::OnBnClickedBtnItempdown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int selNo = m_lstbMarks.GetCurSel();
	int countNo = m_lstbMarks.GetCount();
	if ((countNo - 1) <= selNo)return;

	CString strToMove;
	m_lstbMarks.GetText(selNo, strToMove);
	m_lstbMarks.DeleteString(selNo);
	m_lstbMarks.InsertString(selNo + 1, strToMove);
	m_lstbMarks.SetCurSel(selNo + 1);
}


void CMarkEditDlg::OnBnClickedBtnItemload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog LoadDlg(TRUE);

	CString fn;
	m_lstbMarks.ResetContent();
	if (LoadDlg.DoModal())
	{
		fn = LoadDlg.GetPathName();

		CStdioFile hFile;
		CString strTemp;
		if (!hFile.Open(fn, CFile::modeRead))return;
		while(hFile.ReadString(strTemp))
		{
			m_lstbMarks.AddString(strTemp);
		}
		hFile.Close();
	}
	UpdateData(FALSE);
}


void CMarkEditDlg::OnBnClickedBtnItemsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCount = m_lstbMarks.GetCount();
	if (nCount == 0)return;

	CFileDialog SaveDlg(FALSE);
	CString fn;

	if (SaveDlg.DoModal() == IDOK)
	{
		fn=SaveDlg.GetPathName();

		CStdioFile hFile;
		CString strTemp;
		hFile.Open(fn, CFile::modeCreate | CFile::modeWrite);
		for (int i = 0; i < nCount; i++)
		{
			m_lstbMarks.GetText(i, strTemp);
			hFile.WriteString(strTemp+_T("\n"));
		}
		hFile.Close();
	}
}


BOOL CMarkEditDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedBtnAdd();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMarkEditDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strMark;
	m_pViewOptionDlg->m_lstMark.RemoveAll();
	if (!m_lstbMarks.GetCount())return;
	for (int i = 0; i < m_lstbMarks.GetCount(); i++)
	{
		m_lstbMarks.GetText(i, strMark);
		m_pViewOptionDlg->m_lstMark.Add(strMark);
	}
	m_lstbMarks.GetText(0, strMark);
	m_pViewOptionDlg->m_strNextMarkName = strMark;
	m_pViewOptionDlg->UpdateData(FALSE);
	CStatus::Instance()->SetMarkPos(0);

	CDialogEx::OnOK();
}


void CMarkEditDlg::SetViewOptionDlg(CViewOptionDlg * pViewOptionDlg)
{
	m_pViewOptionDlg = pViewOptionDlg;
}


void CMarkEditDlg::LoadMark()
{
	CString strMark;
	m_lstbMarks.ResetContent();
	int nCount = m_pViewOptionDlg->m_lstMark.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		m_lstbMarks.AddString(m_pViewOptionDlg->m_lstMark.GetAt(i));
	}
}
