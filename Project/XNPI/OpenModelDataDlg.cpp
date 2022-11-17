// OpenModelDataDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "OpenModelDataDlg.h"
#include "Status.h"
#include "resource.h"

CString COpenModelDataDlg::m_strModelName = _T("");
CString COpenModelDataDlg::m_strAllName = _T("");
static int g_imagecnt = 0;
CString g_settingPath;						   ///< 변경된 PCBDATA 경로 
COpenModelDataDlg* COpenModelDataDlg::m_pInstance=NULL;

// COpenModelDataDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COpenModelDataDlg, CDialog)

COpenModelDataDlg::COpenModelDataDlg(CWnd* pParent /*=NULL*/)
: CDialog(COpenModelDataDlg::IDD, pParent)
, m_imageWidth(0)
, m_imageHeight(0)
{
	m_pDisplayMap=NULL;
	m_pMilDisplay=NULL;

}

COpenModelDataDlg::~COpenModelDataDlg()
{
	g_imagecnt = 0;
}

void COpenModelDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODEL_LIST, m_list);
}


BEGIN_MESSAGE_MAP(COpenModelDataDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_MODEL_LIST, &COpenModelDataDlg::OnNMClickModelList)
	ON_BN_CLICKED(IDC_BTN_DELETE, &COpenModelDataDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_RENAME, &COpenModelDataDlg::OnBnClickedBtnRename)
	ON_BN_CLICKED(IDC_BTN_SELECT, &COpenModelDataDlg::OnBnClickedBtnSelect)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// COpenModelDataDlg 메시지 처리기입니다.

BOOL COpenModelDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list.InsertColumn( 0, _T("Model Name"), LVCFMT_LEFT,240);
//	m_list.InsertColumn( 1, _T("Last Update"), LVCFMT_LEFT,120);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	FindModelData();
    
	CStatus::Instance()->LoadCFG();
//    CString Dirstr = CStatus::Instance()->m_LengDir;
//	CChangeRes::Instance()->ChangeDialogCaption(Dirstr,this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void COpenModelDataDlg::FindModelData(void)
{
	m_list.DeleteAllItems();
	CString strFile;
	int i=0;

	WIN32_FIND_DATA findFileData;			// File Data 구조 Structure
	HANDLE hFileHandle;			// File Descriptor Handle

	strFile.Format(_T("%s\\*.*"), CStatus::Instance()->GetModelRepository().GetBuffer());

	hFileHandle = FindFirstFile( strFile, &findFileData);

	if ( hFileHandle != INVALID_HANDLE_VALUE)
	{
		do {
			CString strFileData = _T("");;
			strFileData.Format(findFileData.cFileName);

			if ( !(strFileData == _T(".") || strFileData == _T("..")) )
			{
				if ( (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					m_list.InsertItem( i, findFileData.cFileName );
					i++;
				}
			}
			strFileData.Empty();
		} while( FindNextFile( hFileHandle, &findFileData) );
	}
	else
	{
		AfxMessageBox(IDS_NO_MODELDATA);
	}
	FindClose(hFileHandle);
}

void COpenModelDataDlg::OnNMClickModelList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	UpdateData(true);
	int index = m_list.GetSelectionMark();
	if(index != -1)
	{
		CString strModelRepository = CStatus::Instance()->GetModelRepository();
		CString str;

		str = m_list.GetItemText(index, 0); //인덱스에서 폴더명을 받아옴     
		

		m_strAllName.Format(_T("%s\\%s"),strModelRepository, str);	

		m_strModelName = str;

		UpdateData(false);
	}

	*pResult = 0;
}

void COpenModelDataDlg::OnBnClickedBtnDelete()
{
	CString strModelName, strDelete,strMsg;
	CString strFullModelFolder;
	strMsg.LoadStringW(IDS_DELETE_DATA);
	strModelName = CStatus::Instance()->GetModelName();

	strDelete.Format(_T("[%s]%s"), strModelName, strMsg);
	strFullModelFolder = CStatus::Instance()->GetModelRepository() + _T("\\") + strModelName;
	switch(AfxMessageBox(strDelete, MB_YESNO||MB_ABORTRETRYIGNORE,0))
	{
		case IDOK : DeleteDirectory(strFullModelFolder, 1 ); break;
	}

	FindModelData();
}

#pragma warning(disable:4995)
BOOL COpenModelDataDlg::DeleteDirectory(CString szDir, int recur) // recur 가 0이면 자기는 지우지않음, 1이면 자기까지 삭제
{ 
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	int res = 1;

	TCHAR DelPath[MAX_PATH];
	TCHAR FullPath[MAX_PATH];
	TCHAR TempPath[MAX_PATH];

	lstrcpyW(DelPath, szDir);
	lstrcpyW(TempPath, szDir);
	if (lstrcmp(DelPath+lstrlen(DelPath)-4,_T("\\*.*")) != 0)
		lstrcat(DelPath, _T("\\*.*"));

	hSrch = FindFirstFile(DelPath, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE)
	{ 
		if (recur > 0) 
			RemoveDirectory(TempPath);

		return -1;
	}
	while(res)
	{
		wsprintf(FullPath, _T("%s\\%s"), TempPath, wfd.cFileName);

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
		{
			SetFileAttributes(FullPath, FILE_ATTRIBUTE_NORMAL);
		}

		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(lstrcmp(wfd.cFileName, _T(".")) && lstrcmp(wfd.cFileName, _T("..")))
			{
				recur++;
				DeleteDirectory(FullPath, recur);
				recur--;
			}
		}
		else
		{
			DeleteFile(FullPath);
		}
		res = FindNextFile(hSrch, &wfd);
	}
	FindClose(hSrch);

	if (recur > 0) 
		RemoveDirectory(TempPath);

	return 0;

} 
void COpenModelDataDlg::OnBnClickedBtnRename()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
     
	CRenameModelDataDlg dlg;
	dlg.DoModal();

	FindModelData();
}

void COpenModelDataDlg::OnBnClickedBtnSelect()
{
	int index = m_list.GetSelectionMark();

	if( index == -1 )
	{
		AfxMessageBox(IDS_NO_SELECTED_DATA);
		return;
	}

	CString m_strCaption;

	CString strFolderPath = CStatus::Instance()->GetModelRepository();

	CString strMsg;

	strMsg.LoadStringW(IDS_LOAD_MODEL_DATA);
	CModelData *pModelData = CStatus::Instance()->GetModelData();
	if(AfxMessageBox(strMsg,MB_OKCANCEL)==IDOK)
	{   
		pModelData->SetMDName(m_strModelName.GetBuffer());
		pModelData->LoadModelData();
		OnOK();
	}
	else
	{
		return;
	}
    
}
COpenModelDataDlg * COpenModelDataDlg::Instance(void)
{
	return m_pInstance;
}

void COpenModelDataDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDown(nFlags, point);
}
