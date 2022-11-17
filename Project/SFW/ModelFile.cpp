// MviArchive.cpp: implementation of the CModelFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModelFile.h"
#include "ModelData.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModelFile::CModelFile()
{
	m_pModelData = NULL;
}

CModelFile::~CModelFile()
{
}

#define TEMP_MDLFILE	_T("ModelData.~NI")
#define MDLFILE			_T("ModelData.INI")
#define BAK_MDLFILE		_T("ModelData.BAK")


BOOL CModelFile::Create(CModelData* pModelData)
{
	m_pModelData = pModelData;
	if (!CXmlBrowser::Create())
		return FALSE;

	return TRUE;
}

BOOL CModelFile::Open(CModelData* pModelData)
{
	CString fileName;
	fileName.Format(_T("%s\\%s"), pModelData->GetModelPath()+_T("\\")+pModelData->GetMDName(), MDLFILE);
//	fileName.Format(_T("D:\\Model9800\\NewModeltest\\ModelData.INI"));           
	if (!CXmlBrowser::Open(fileName.GetBuffer()))
		return FALSE;

	m_pModelData = pModelData;

	return TRUE;
}

BOOL CModelFile::IsSavable()
{
	CString fileName;

	fileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath()+_T("\\")+m_pModelData->GetMDName(), MDLFILE);
	if (CFileInfo::IsReadonly(fileName)) {
		return FALSE;
	}

	fileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath()+_T("\\")+m_pModelData->GetMDName(), BAK_MDLFILE);
	if (CFileInfo::IsReadonly(fileName)) {
		return FALSE;
	}
	return TRUE;
}

BOOL CModelFile::RemoveReadonly()
{
	CString fileName;

	fileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath()+_T("\\")+m_pModelData->GetMDName(), MDLFILE);
	CFileInfo::RemoveReadonly(fileName);

	fileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath()+_T("\\")+m_pModelData->GetMDName(), BAK_MDLFILE);
	CFileInfo::RemoveReadonly(fileName);
	return TRUE;
}

void CModelFile::Save()
{
	if (!IsSavable()) {
		if (AfxMessageBox(IDS_SAVE_ERR, MB_YESNO) == IDNO) {
			return;
		}
		RemoveReadonly();
	}

	BOOL fResult = FALSE;
	CString modelTempFileName, modelFileName, modelBakFileName;
	modelBakFileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath()+_T("\\")+m_pModelData->GetMDName(), BAK_MDLFILE);
	modelTempFileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath()+_T("\\")+m_pModelData->GetMDName(), TEMP_MDLFILE);
	modelFileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath()+_T("\\")+m_pModelData->GetMDName(), MDLFILE);

	if (CXmlBrowser::Save(modelFileName)) {

		fResult = TRUE;

	} 

	if (!fResult) AfxMessageBox(IDS_SAVE_ERR);
}

