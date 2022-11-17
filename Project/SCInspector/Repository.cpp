#include "stdafx.h"
#include "Repository.h"
#include "Status.h"

CRepository * CRepository::m_pInstance = NULL;
CRepository::CRepository()
{
}


CRepository::~CRepository()
{
}


CRepository * CRepository::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CRepository;
	}
	return m_pInstance;
}


int CRepository::WriteInspResult(CSCResult *pResult)
{
	CString strCurModelFolder;
	CString strFolderPath = CStatus::Instance()->GetModelRepository();
	strCurModelFolder = strFolderPath +_T("\\") + CStatus::Instance()->GetModelName();

	CString strResultFolder = strCurModelFolder + _T("\\") + RESULT_FOLDER;
	CTime timeCur = CTime::GetCurrentTime();
	CString strDate;
	strDate = timeCur.Format(_T("%y%m%d"));
	CString strResultFileName = strResultFolder + _T("\\") + strDate + _T(".CSV");

	//弃歹积己?
	CFileInfo::CreateFolder(strResultFolder.GetBuffer());
	WriteResultFile(strResultFileName, pResult);

	return 0;
}

void CRepository::WriteResultFile(CString strFileName, CSCResult *pResult)
{
	CStdioFile csvFile;
	CFileException ex;
	CString newRecord;
	CTime timeCurrent;
	if ( !PathFileExists( strFileName.GetBuffer() ) ) {
		if (!csvFile.Open(strFileName.GetBuffer(), CFile::modeWrite | CFile::modeCreate, &ex)) {
			return;//Can't open file 
		}
		CString strLogInfo;

		timeCurrent = CTime::GetCurrentTime();
		CString strCurrent = timeCurrent.Format(_T("%y%m%d"));
		strLogInfo.Format(_T("DATE:,%s\n"), strCurrent);
		csvFile.WriteString(strLogInfo.GetBuffer());
		CString Header(_T("TimeInfo,CamNo, InspType, Result, Mean, Min, Max, Mean(um), Min(mm), Max(mm)\n"));
		csvFile.WriteString(Header.GetBuffer());
	}
	else
	{
		if (!csvFile.Open(strFileName, CFile::modeWrite))
		{
			return;//颇老积己阂啊
		}
		csvFile.SeekToEnd();
	}

	timeCurrent = CTime::GetCurrentTime();
	CString strCurrTime = timeCurrent.Format(_T("%Hhour %Mmin %Ssec"));
	CString strNGGood;
	if (!(pResult->iResult))
		strNGGood = _T("Good");
	else
		strNGGood = _T("NG");
	newRecord.Format(_T("%s, %d, %d, %s, %6.2f, %6.2f, %6.2f, %6.2f, %6.2f, %6.2f\n"), 
		      strCurrTime.GetBuffer(), pResult->CamNo, pResult->iInspType, strNGGood.GetBuffer(), pResult->dblMean, pResult->dblMin, pResult->dblMax, pResult->umMean, pResult->umMin, pResult->umMax);
	csvFile.WriteString(newRecord);
	csvFile.Close();

}