// SaveModelData.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XFMS.h"
#include "Status.h"
#include "SaveModelDataDlg.h"
#include "OpenModelDataDlg.h"

// CSaveModelDataDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSaveModelDataDlg, CDialog)

CSaveModelDataDlg::CSaveModelDataDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSaveModelDataDlg::IDD, pParent)
, m_strModelName(_T(""))
{

}

CSaveModelDataDlg::~CSaveModelDataDlg()
{
}

void CSaveModelDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_MODELNAME, m_strModelName);
	DDX_Control(pDX, IDOK, m_cOK);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
}


BEGIN_MESSAGE_MAP(CSaveModelDataDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSaveModelDataDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_BROWSER, &CSaveModelDataDlg::OnBnClickedBtnBrowser)
END_MESSAGE_MAP()


// CSaveModelDataDlg 메시지 처리기입니다.

void CSaveModelDataDlg::OnBnClickedOk()
{  
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CModelData *pModelData = CStatus::Instance()->GetModelData();
	int nCount = pModelData->GetList()->GetCount();
	if(nCount)
	{
		UpdateData();

		CString strFolderPath= pModelData->GetModelPath();
		pModelData->SetMDName(m_strModelName.GetBuffer());
		CStatus::Instance()->SetModelName(m_strModelName.GetBuffer());
		pModelData->SaveModelData();

		/////////////////// 지정된 폴더와 파일의 모든 내용을 지정된 경로안으로 통째로 옮김
		OnOK();
	}
	else
	{
		AfxMessageBox(IDS_NOT_FOUND_TEACHING_DATA);
	}
}

void CSaveModelDataDlg::CopyfileSetuptoModelFloder(CString OriginPath,CString NewPath)
{    
	
	if(AfxMessageBox(IDS_MODEL_SAVE_START,MB_OKCANCEL)==IDOK)
	{   
		CopyFile(OriginPath+_T("\\CameraPIP_other.ini"),NewPath+_T("\\CameraPIP_other.ini"),FALSE);  // 파일복사 
				
		CopyFile(OriginPath+_T("\\PCBImage\\AllImage.tif"),NewPath+_T("\\AllImage.tif"),FALSE);
		SHFILEOPSTRUCT fo;
		fo.wFunc=FO_COPY;
		fo.pFrom=_T("D:\\StarPrj\\Runtime\\Setup\\PCBImage\\ImageList"); // 폴더전체 복사 
		fo.pTo=NewPath;
		fo.fFlags=FOF_NOCONFIRMMKDIR;
		SHFileOperation(&fo);
	}
	else 
		return;

	AfxMessageBox(IDS_SAVE_OK);

}

void CSaveModelDataDlg::OnBnClickedBtnBrowser()
{
	CFolderDialog FloderDlg;
	CString strModelName = _T("");
	CString strFullName = _T("");

	switch(FloderDlg.DoModal())
	{
	case IDOK:
		{
		strFullName = FloderDlg.GetFolderPath();

			strFullName.MakeReverse();

			int i = strFullName.Find('\\');
			strModelName = strFullName.Left(i);

			strModelName.MakeReverse();

			m_strModelName = strModelName;
		};
		break;
	case IDCANCEL:
		m_strModelName = _T("NewModel");
		break;
	}

	UpdateData(FALSE);
}
BOOL CSaveModelDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if( CStatus::Instance()->GetModelName() == _T("") )
	{
		m_strModelName =_T("NewModel");
	}
	else
	{
		m_strModelName = CStatus::Instance()->GetModelName();
	}	

	UpdateData(FALSE);
	CStatus::Instance()->LoadCFG();
//    CString Dirstr = CStatus::Instance()->m_LengDir;
//	CChangeRes::Instance()->ChangeDialogCaption(Dirstr,this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
