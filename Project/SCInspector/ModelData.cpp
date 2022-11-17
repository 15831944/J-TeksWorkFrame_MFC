// ModelData.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "ModelFile.h"
#include "ModelData.h"
#include "Compo.h"
#include "Frame.h"
//#include "BridgeAlgo.h"
//#include "BallAlgo.h"
#include "Inspection.h"
#include "Module.h"
#include "FidAlgo.h"
//#include "ChipAlgo.h"
#include "CamPart.h"
#include "MainFrm.h"
#include "Repository.h"
#include "Status.h"

// CModelData

#define MODEL_CFGFILE _T("ModelCFG.ini")

IMPLEMENT_DYNAMIC(CModelData, CBroadcaster)
CModelData::CModelData()
: m_strModelPath(_T(""))
{
	m_nSeqNum=1;
	m_dblLeftPos=0.0;
	m_dblRightPos=0.0;
	m_dblLeftPos=0.0;
	m_dblLeftPos=0.0;

}

CModelData::~CModelData()
{
}


// CModelData 멤버 함수

int CModelData::AddElement(CModelBase* pElement)
{
	m_ListElement.AddTail(pElement);
	return 0;
}

int CModelData::AddElementHead(CModelBase* pElement)
{
	m_ListElement.AddHead(pElement);
	return 0;
}

CFrame* CModelData::FindElmtWindow(CdblPoint dblPosition)
{
//	m_ListCompo.GetHead()
	return 0;
}

int CModelData::SaveModelData(CString strFileName)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	CString nameElement=szTemp;
	MakeModelFolder();
//jubatemp	SaveModelInfo();
	CModelBase* pElement;

	CModelFile hFile;
	if (!hFile.Create(this))return -1;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->SetTitleCaption(GetMDName());

	CXmlNode * pXmlRootNode = hFile.GetRootNode(nameElement.GetBuffer());

	SaveAttr(pXmlRootNode);

	POSITION pos;
	if(m_ListElement.GetCount())
	{
		for(pos=m_ListElement.GetHeadPosition();pos!=NULL;)
		{
			pElement=(CModelBase *)m_ListElement.GetNext(pos);
			pElement->SaveParam(pXmlRootNode);
		}
	}
	pXmlRootNode->EndNode();
    
	if(strFileName.IsEmpty())
		hFile.Save();
	else
	{
		CString strFileNameNoExt;
		strFileNameNoExt = strClip(strFileName, 3);
		hFile.Save(strFileNameNoExt);
	}

	return 0;
}

int CModelData::ReturnDIR()
{
	 m_strModelPath.Format(_T("D:\\SCModel"));
	 return 0;
}
int CModelData::SaveModelDataTemp()
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	CString nameElement=szTemp;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

	SaveModelInfoforTemp();
	CModelBase* pElement;
	

	CModelFile hFile;
	if(!hFile.Create(this))return -1;
	pMainFrame->SetTitleCaption(GetMDName());

	CXmlNode * pXmlRootNode = hFile.GetRootNode(nameElement.GetBuffer());

	SaveAttr(pXmlRootNode);

	POSITION pos;
	if(m_ListElement.GetCount())
	{
		for(pos=m_ListElement.GetHeadPosition();pos!=NULL;)
		{
			pElement=(CModelBase *)m_ListElement.GetNext(pos);
			pElement->SaveParam(pXmlRootNode);
		}
	}
	pXmlRootNode->EndNode();
    
	hFile.Save();
  
	return 0;
}


int CModelData::LoadModelData(CString strFileName)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	CString nameCModelData=szTemp;
	CString nameElement = _T("");

//jubatemp	LoadModelInfo();
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	
	pMainFrame->SetTitleCaption(GetMDName());
    
 	CModelBase* pElement;
	
	CModelFile hFile;
	if (strFileName.IsEmpty())
	{
		if (!hFile.Open(this))return -1;
	}
	else
	{
		if (!hFile.Open(this, strFileName))return -1;
	}

	RemoveAll();

	CXmlNode * pXmlRootNode = hFile.GetRootNode(nameElement.GetBuffer());
	nameElement.Format(_T("CModelData"));
	if(nameElement.Compare(nameCModelData))return -1;

	m_ListElement.RemoveAll();
	CXmlNode * pXmlChildNode = pXmlRootNode->GetFirstChildNode();
   
	do
	{	
		nameElement = pXmlChildNode->GetNodeName();
		pElement = GetModelClass(nameElement.GetBuffer());
		pElement->LoadParam(pXmlChildNode);
		m_ListElement.AddTail(pElement);

		pXmlChildNode = pXmlChildNode->GetNextNode();
	}while(pXmlChildNode);
	
	return 0;
}

int CModelData::LoadModelDataTemp()
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	CString nameCModelData=szTemp;
	CString nameElement = _T("");

	
    LoadModelInfoTemp();
 	CModelBase* pElement;
	
	CModelFile hFile;
	if(!hFile.Open(this))return -1;
	
	CXmlNode * pXmlRootNode = hFile.GetRootNode(nameElement.GetBuffer());
	nameElement.Format(_T("CModelData"));
	if(nameElement.Compare(nameCModelData))return -1;

	m_ListElement.RemoveAll();
	CXmlNode * pXmlChildNode = pXmlRootNode->GetFirstChildNode();
   
	do
	{	
		nameElement = pXmlChildNode->GetNodeName();
		pElement = GetModelClass(nameElement.GetBuffer());
		pElement->LoadParam(pXmlChildNode);
		m_ListElement.AddTail(pElement);

		pXmlChildNode = pXmlChildNode->GetNextNode();
	}while(pXmlChildNode);
	
	return 0;
}

void CModelData::SetModelPath(TCHAR * szModelPath)
{
	m_strModelPath=szModelPath;
}

int CModelData::SaveModelInfo(void)
{   
	USES_CONVERSION;
	//마지막 업데이트 시간을 알기 위한 함수///
	//struct tm *t;
	//time_t timer;

	//timer = time(NULL);    // 현재 시각을 초 단위로 얻기
	//t = localtime(&timer); // 초 단위의 시간을 분리하여 구조체에 넣기

//	m_dbUpdateTime = timeToString(t);

//	m_chUpdateTime = (TCHAR*)(LPCTSTR)(m_dbUpdateTime);  //Cstring -> TCHAR* 로 변환

	//////////////

	CCfgFile Info;
	CString strSettingFile=m_strModelPath+_T("\\")+m_strMDName;
	strSettingFile=strSettingFile+_T("\\");
	strSettingFile+=MODEL_CFGFILE;
	Info.SetPath(strSettingFile);
	if(!Info.Open())return -1;
	Info.Set((long)m_nSeqNum, _T("Description"), _T("SequenceNumber"));
	Info.Set(m_dblLeftPos, _T("Description"), _T("LEFTPos"));
	Info.Set(m_dblRightPos, _T("Description"), _T("RIGHTPos"));
	Info.Set(m_dblTopPos, _T("Description"), _T("Topos"));
	Info.Set(m_dblBottomPos, _T("Description"), _T("BottomPos"));
     
	Info.Set(m_dblXrayPIPZ, _T("XrayPIP"), _T("PosX"));

	Info.Set(m_chUpdateTime, _T("UpdateTime"), _T("LastUpdate")); //매개변수 1 에 CString 을 넣어봤지만 되지않음

	Info.Flush();
	return 0;
}

int CModelData::SaveModelInfoforTemp(void)
{   
	USES_CONVERSION;
	//마지막 업데이트 시간을 알기 위한 함수///
	//struct tm *t;
	//time_t timer;

	//timer = time(NULL);    // 현재 시각을 초 단위로 얻기
	//t = localtime(&timer); // 초 단위의 시간을 분리하여 구조체에 넣기

//	m_dbUpdateTime = timeToString(t);

//	m_chUpdateTime = (TCHAR*)(LPCTSTR)(m_dbUpdateTime);  //Cstring -> TCHAR* 로 변환

	//////////////
	CCfgFile Info;
	m_strModelPath.Format(_T("..\\Setup"));
	CString strSettingFile=m_strModelPath+_T("\\")+m_strMDName;
	strSettingFile=strSettingFile+_T("\\");
	strSettingFile+=MODEL_CFGFILE;
	Info.SetPath(strSettingFile);
	if(!Info.Open())return -1;
	Info.Set((long)m_nSeqNum, _T("Description"), _T("SequenceNumber"));
	Info.Set(m_dblLeftPos, _T("Description"), _T("LEFTPos"));
	Info.Set(m_dblRightPos, _T("Description"), _T("RIGHTPos"));
	Info.Set(m_dblTopPos, _T("Description"), _T("Topos"));
	Info.Set(m_dblBottomPos, _T("Description"), _T("BottomPos"));
     
	Info.Set(m_dblXrayPIPZ, _T("XrayPIP"), _T("PosX"));

	Info.Set(m_chUpdateTime, _T("UpdateTime"), _T("LastUpdate")); //매개변수 1 에 CString 을 넣어봤지만 되지않음 되지않음
	Info.Flush();
	
	return 0;
}

int CModelData::LoadModelInfo(void)
{
	CCfgFile Info;
	CString strSettingFile=m_strModelPath+_T("\\")+m_strMDName;
	strSettingFile=strSettingFile+_T("\\");
	strSettingFile+=MODEL_CFGFILE;
	Info.SetPath(strSettingFile);
	if(!Info.Open())return -1;
	Info.Get((long)m_nSeqNum, _T("Description"), _T("SequenceNumber"));
	m_dblLeftPos	=Info.Get(0.0, _T("Description"), _T("LEFTPos"));
	m_dblRightPos	=Info.Get(0.0, _T("Description"), _T("RIGHTPos"));
	m_dblTopPos		=Info.Get(0.0, _T("Description"), _T("Topos"));
	m_dblBottomPos	=Info.Get(0.0, _T("Description"), _T("BottomPos"));

	return 0;
}

int CModelData::LoadModelInfoTemp(void)
{
	CCfgFile Info;
	m_strModelPath.Format(_T("..\\Setup"));
	CString strSettingFile=m_strModelPath+_T("\\")+m_strMDName;
	strSettingFile=strSettingFile+_T("\\");
	strSettingFile+=MODEL_CFGFILE;
	Info.SetPath(strSettingFile);
	if(!Info.Open())return -1;
	Info.Get((long)m_nSeqNum, _T("Description"), _T("SequenceNumber"));
	m_dblLeftPos	=Info.Get(0.0, _T("Description"), _T("LEFTPos"));
	m_dblRightPos	=Info.Get(0.0, _T("Description"), _T("RIGHTPos"));
	m_dblTopPos		=Info.Get(0.0, _T("Description"), _T("Topos"));
	m_dblBottomPos	=Info.Get(0.0, _T("Description"), _T("BottomPos"));

	m_dblXrayPIPZ	=Info.Get(0.0, _T("XrayPIP"), _T("PosX"));
	return 0;
}


int CModelData::Create(void)
{
	if(!m_ModelFile.Create())return 0;
	return 1;
}

CString CModelData::GetModelPath(void)
{
	return m_strModelPath;
}


CModelBase * CModelData::GetModelClass(TCHAR * szModelName)
{
	CString strModelName=szModelName;
	CAlgorithmBase *pModelBase;
	if(!CInspection::Instance()->NewObject(szModelName, &pModelBase))return pModelBase;
	if(!strModelName.Compare(_T("CCompo")))return new CCompo;
	if (!strModelName.Compare(_T("CFrame")))return new CFrame;
	if (!strModelName.Compare(_T("CCamPart")))return new CCamPart;

//	if(!strModelName.Compare(_T("CBridgeAlgo")))return new CBridgeAlgo;
	if(!strModelName.Compare(_T("CModule")))return new CModule;
//	if(!strModelName.Compare(_T("CBallAlgo")))return new CBallAlgo;
//	if(!strModelName.Compare(_T("CFidAlgo")))return new CFidAlgo;
//	if(!strModelName.Compare(_T("CChipAlgo")))return new CChipAlgo;
//	if(!strModelName.Compare(_T("CBatteryAlgo")))return new CChipAlgo;
	//CString strErrMsg;
	//strErrMsg.Format(_T("%s is not registrated"), strModelName.GetBuffer());
	//AfxMessageBox(strErrMsg, MB_OK);
	return NULL;
}

void CModelData::RemoveAll(void)
{
	CModelData();

	int nCount=m_ListElement.GetCount();
	if(!nCount)return;

	CModelBase* pElement;

	POSITION pos;
	pos=m_ListElement.GetHeadPosition();

	int i=0;
	do{
		pElement = (CModelBase *)m_ListElement.GetAt(pos);
		pElement->Free();
		delete pElement;
		m_ListElement.GetNext(pos);
	}while((i++<nCount) && (pos !=NULL));

	m_ListElement.RemoveAll();
}

CPtrList * CModelData::GetList(void)
{
	return &m_ListElement;
}

void CModelData::SetMDName(TCHAR * szMDName)
{
	CStatus::Instance()->SetModelName(szMDName);
	m_strMDName = szMDName;
}

CString CModelData::GetMDName(void)
{
	return m_strMDName;
}

#pragma warning(disable:4995)
char* CModelData::timeToString(struct tm *t) 
{
	static char s[20];

	sprintf_s(s, sizeof(s), "%04d-%02d-%02d %02d:%02d:%02d",
		t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec );

	return s;
}


int CModelData::SaveAttr(CXmlNode * pNode)
{
	return 0;
}

CdblSize CModelData::GetModelScaleFactor()
{

	return CdblSize(10,10);
}

int CModelData::GetOLTTImg(double posx, double posy, double posz)
{
	CdblSize frameScale = GetModelScaleFactor();
	long frameX = (long)(posx / frameScale.dblCX );
	long frameY = (long)(posy / frameScale.dblCY );

//	CSize sizeImage = GetDigitizer()->GetImageSize();

	return 0;
}

int CModelData::GetOLTTImg(int iFrameNumber)
{
	CRepository::Instance()->m_MilLib.NewImage();
	return 0;
}

/// Return Value 0:Find 해당객체가 있는지 리턴한다.
///              -1:Not Founded
BOOL CModelData::IsExist(CModelBase* pElement)
{
	CModelBase* CurElement;
	POSITION pos;

	for(pos=m_ListElement.GetHeadPosition();pos!=NULL;)
	{
		CurElement=(CModelBase *)m_ListElement.GetNext(pos);
		if(CurElement==pElement)return TRUE;
	}
	return FALSE;
}


/// 해당객체의 형태가 있는지 리턴한다.
BOOL CModelData::IsExistKindOf(CRuntimeClass* pElementType)
{
	CModelBase* CurElement;
	POSITION pos;

	for(pos=m_ListElement.GetHeadPosition();pos!=NULL;)
	{
		CurElement=(CModelBase *)m_ListElement.GetNext(pos);
		if(CurElement->IsKindOf(pElementType)) return TRUE;
	}
	return FALSE;
}
BOOL CModelData::IsEmpty(void)
{
	return m_ListElement.IsEmpty();
}

// Type
// 0: 파일 경로만 복사
// 1: 파일 이름만 복사
// 2: 파일 확장자 복사
// 3: 확장자를 뺀 파일명 복사
// 4: Path도 제거 확장자를 뺀 파일명 복사
// 5: 2번케이스의 파일 확장자에서 .을 뺌.
CString CModelData::strClip(CString str, int nType)
{
	//파일 Full Path를 복사
	TCHAR szTmp[4096];
	StrCpy(szTmp, str);
	CString strTmp;
	CString strResult = _T("");
	switch (nType)
	{
	case 0:
		//파일의 경로만 복사.
		PathRemoveFileSpec(szTmp);
		strResult = szTmp;
		break;
	case 1:
		// 1: 파일 이름만 복사
		strResult = PathFindFileName(szTmp);
		//strResult = szTmp;
		break;
	case 2:
		// 2: 파일 확장자 복사
		strResult = PathFindExtension(szTmp);
		break;
	case 3:
		// 3: 확장자를 뺀 파일명 복사
		StrCpy(szTmp, str);
		PathRemoveExtension(szTmp);
		strResult = szTmp;
		break;
	case 4:
		// 4: Path도 제거 확장자를 뺀 파일명 복사
		strTmp = PathFindFileName(szTmp);
		ZeroMemory(szTmp, 4096);
		StrCpy(szTmp, strTmp);
		PathRemoveExtension(szTmp);
		strResult = szTmp;
		break;
	case 5:
		// 5: 2번케이스의 파일 확장자에서 .을 뺌.
		strResult = PathFindExtension(szTmp);
		strResult = strResult.Right(strResult.GetLength() - 1);
		break;
	}

	return strResult;
}

int CModelData::MakeModelFolder()
{
	CString strModelRepository = GetModelPath();
	CString strModelName = GetMDName();
	CString strFullPath = strModelRepository + _T("\\") + strModelName;
//	COpenModelDataDlg::Instance()->m_strAllName = strFullPath;
	CFileInfo::CreateFolder(strFullPath.GetBuffer());
	GetModelPath();
	return 0;
}
