// MviArchive.cpp: implementation of the CModelFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCInspector.h"
#include "ModelFile.h"
#include "ModelData.h"

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

#define TEMP_MDLFILEEXT	_T("ModelData.~NI")
#define MDLFILEEXT		_T("ModelData.INI")
#define BAK_MDLFILEEXT	_T("ModelData.BAK")

BOOL CModelFile::Create(CModelData* pModelData)
{
	m_pModelData = pModelData;
	if (!CXmlBrowser::Create())
		return FALSE;

	return TRUE;
}

BOOL CModelFile::Open(CModelData* pModelData, CString strFileName)
{
	CString fileName;
	if (strFileName.IsEmpty())
		fileName.Format(_T("%s\\%s"), pModelData->GetModelPath() + _T("\\") + pModelData->GetMDName(), MDLFILEEXT);
	else
		fileName = strFileName;
	if (!CXmlBrowser::Open(fileName.GetBuffer()))
		return FALSE;

	m_pModelData = pModelData;

	return TRUE;
}

BOOL CModelFile::IsSavable()
{
	CString fileName;

	fileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath()+_T("\\")+m_pModelData->GetMDName(), MDLFILEEXT);
	if (CFileInfo::IsReadonly(fileName)) {
		return FALSE;
	}

	fileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath()+_T("\\")+m_pModelData->GetMDName(), BAK_MDLFILEEXT);
	if (CFileInfo::IsReadonly(fileName)) {
		return FALSE;
	}
	return TRUE;
}

BOOL CModelFile::RemoveReadonly()
{
	CString fileName;

	fileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath()+_T("\\")+m_pModelData->GetMDName(), MDLFILEEXT);
	CFileInfo::RemoveReadonly(fileName);

	fileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath()+_T("\\")+m_pModelData->GetMDName(), BAK_MDLFILEEXT);
	CFileInfo::RemoveReadonly(fileName);
	return TRUE;
}

void CModelFile::Save(CString strFileNameNoExt)
{
	if (!IsSavable()) {
		if (AfxMessageBox(IDS_SAVE_ERR, MB_YESNO) == IDNO) {
			return;
		}
		RemoveReadonly();
	}

	BOOL fResult = FALSE;
	CString modelTempFileName, modelFileName, modelBakFileName;
	if (strFileNameNoExt.IsEmpty())
	{
		modelBakFileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath() + _T("\\") + m_pModelData->GetMDName(), BAK_MDLFILEEXT);
		modelTempFileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath() + _T("\\") + m_pModelData->GetMDName(), TEMP_MDLFILEEXT);
		modelFileName.Format(_T("%s\\%s"), m_pModelData->GetModelPath() + _T("\\") + m_pModelData->GetMDName(), MDLFILEEXT);
	}
	else
	{
		modelBakFileName.Format(_T("%s\\%s"), strFileNameNoExt, BAK_MDLFILEEXT);
		modelTempFileName.Format(_T("%s\\%s"), strFileNameNoExt, TEMP_MDLFILEEXT);
		modelFileName.Format(_T("%s\\%s"), strFileNameNoExt, MDLFILEEXT);
	}

	if (CXmlBrowser::Save(modelFileName)) {

		fResult = TRUE;

	} 

	if (!fResult) AfxMessageBox(IDS_SAVE_ERR);
}

