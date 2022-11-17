#include "stdafx.h"
#include "Repository.h"
#include "Status.h"
#include "Strsafe.h"
#include "MainFrm.h"
#include <locale.h>

// 1,	 2,	   3,	     4,	       5,	           6,	            7,	      8,	    9,	         10,	       11,	         12,	         13,	 14,	                  15,	       16,	    17,	      18,	19,	     20
// Date, Time, Raw Roll, Raw Pitch, Diplayed Roll, Displayed Pitch, Roll Ref, Pitch Ref, Roll Stable, Pitch Stable, Stablization, Go/NoGo Value, Abs/Rel, Days since Calibration, Temperature, Battery, Operator, Tool, Station, Comment
#define CSV_HEADER_FORMAT _T("Date, Time, Raw Roll, Raw Pitch, Displayed X Value, Displayed Y Value, Abs/Rel, Roll Ref, Pitch Ref, Roll Stable, Pitch Stable, Stabilization, Go/NoGo Value, Days since Calibration, Temperature, Battery, Operator, Tool, Station, Comment\n")


CRepository::CRepository()
	: m_strCurLogName(_T(""))
	, m_bNewMeasureState(false)
	, m_strCurCSVName(_T(""))
	, m_iDispIndex(0)
{
	m_DataRecords.nCountOfRecords = 0;
}


CRepository::~CRepository()
{
}

int CRepository::TestFileWrite()
{
	tALMDRecord ALMDRecord;
	tALMDRecord *pNewRecord=&ALMDRecord;
	memset(pNewRecord, 0, sizeof(tALMDRecord));
	CString strTest;
	struct tm tm_ptr;
	time_t CalTime;
	tm_ptr.tm_year = 2016 - 1900;
	tm_ptr.tm_mon = 11 - 1;
	tm_ptr.tm_mday = 20;
	tm_ptr.tm_hour = 1;
	tm_ptr.tm_min = 0;
	tm_ptr.tm_sec = 0;
	tm_ptr.tm_isdst = 0;
	CalTime = mktime(&tm_ptr);
	
	for (int i = 0; i < 100; i++)
	{
		CString str;
		tMarkInfo MarkInfo;
		memset(&MarkInfo, 0, sizeof(tMarkInfo));
		wsprintf(MarkInfo.szOperator, TEXT("Operator"));
		wsprintf(MarkInfo.szStation, TEXT("A"));
		wsprintf(MarkInfo.szTool, TEXT("Seso"));
		StringCbPrintf(MarkInfo.szComment, 500, TEXT("Seso 테스트"));
		CStatus::Instance()->SetMarkToAdd(&MarkInfo);
		time_t now;
		time(&now);
		pNewRecord->byBattery=i;
		pNewRecord->byTemperature = 10;
		pNewRecord->CurTime = now;
		pNewRecord->dateCalibration = CalTime;
		pNewRecord->sRawPitch = i * 100;
		pNewRecord->sRawRoll = i * 15;
		pNewRecord->bRel = FALSE;
		AppendLogFile(&ALMDRecord);
		if (!strTest.IsEmpty())strTest = _T("");
	}

	return 0;
}


int CRepository::AppendLogFileCore(CFile * pFile, tALMDRecord * pNewRecord)
{
	if (!(PathFileExists(m_strCurLogName.GetBuffer())) | m_bNewMeasureState) {
		if (!pFile->Open(m_strCurLogName, CFile::modeCreate | CFile::modeWrite))
		{
			return -1;//파일생성불가
		}
	}
	else
	{
		if (!pFile->Open(m_strCurLogName, CFile::modeWrite))
		{
			return -1;//파일생성불가
		}
		pFile->SeekToEnd();
	}
	pNewRecord->MarkInfo = CStatus::Instance()->GetMarkToAdd();
	pFile->Write(pNewRecord, sizeof(tALMDRecord));

	pFile->Close();
	return 0;
}

#pragma warning(disable: 4996)
int CRepository::AppendCSVFileCore(CStdioFile *pStdioFile, tALMDRecord * pNewRecord)
{
	setlocale(LC_ALL, "Korean");
	if (!(PathFileExists(m_strCurCSVName.GetBuffer())) | m_bNewMeasureState) {
		if (!pStdioFile->Open(m_strCurCSVName, CFile::modeCreate | CFile::modeWrite))
		{
			return -1;//파일생성불가
		}
		//USHORT uBOM = 0xfeff;
		//pStdioFile->Write(&uBOM, sizeof(uBOM)); // 파일의 첫부분에 BOM저장 
		CString strLogInfo;
		CString Header(CSV_HEADER_FORMAT);
		pStdioFile->WriteString(Header.GetBuffer());
	}
	else
	{
		if (!pStdioFile->Open(m_strCurCSVName, CFile::modeWrite))
		{
			return -1;//파일생성불가
		}
		pStdioFile->SeekToEnd();
	}

	struct tm DateTime;
	struct tm CalDateTime;
	DateTime = *localtime(&(pNewRecord->CurTime));
	CalDateTime = *localtime(&(pNewRecord->dateCalibration));
	pNewRecord->MarkInfo = CStatus::Instance()->GetMarkToAdd();
	CString newRecord = MakeCSVRecordString(&DateTime, &CalDateTime, pNewRecord);

	pStdioFile->WriteString(newRecord);
	pStdioFile->Close();
	return 0;
}

/*! 파일이 없으면 만들고 추가, 없으면 추가*/
int CRepository::AppendLogFile(tALMDRecord *pNewRecord)
{
	setlocale(LC_ALL, "Korean");
	CFile LogFile;
	CStdioFile CSVFile;

	CString strLogFile = CStatus::Instance()->GetDataSaveFolder() + _T("\\");
	strLogFile += CStatus::Instance()->GetDataFileName();
	m_strCurLogName = strLogFile + _T(".LOG");
	m_strCurCSVName = strLogFile + _T(".CSV");

	AppendLogFileCore(&LogFile, pNewRecord);
	AppendCSVFileCore(&CSVFile, pNewRecord);
	CStatus::Instance()->ClearMarkToAdd();

	if(m_bNewMeasureState)	m_bNewMeasureState = false;

	return 0;
}

void CRepository::ExportCSVFile(CString strFileName)
{
	CStdioFile csvFile;
	CFileException ex;
	if (!csvFile.Open(strFileName.GetBuffer(), CFile::modeWrite | CFile::modeCreate, &ex)) {
		return;//Can't open file 
	}
	CString strLogInfo;
	CString Header(CSV_HEADER_FORMAT);
	csvFile.WriteString(Header.GetBuffer());

	POSITION pos;
	tALMDRecord *pNewRecord;
	for (pos = m_DataRecords.m_ALMDRecords.GetHeadPosition(); pos != NULL;)
	{
		struct tm DateTime;
		struct tm CalDateTime;
		pNewRecord = (tALMDRecord *)m_DataRecords.m_ALMDRecords.GetNext(pos);
		DateTime = *localtime(&(pNewRecord->CurTime));
		CalDateTime = *localtime(&(pNewRecord->dateCalibration));
		CString newRecord = MakeCSVRecordString(&DateTime, &CalDateTime, pNewRecord);
		csvFile.WriteString(newRecord);
	}
	csvFile.Close();

}

int CRepository::ReadFromLogFile(CFile* pLogFile, tRecordsSet * pRecordsSet, tLogType logType)
{
	CString csvLine, subLine;
	bool exceptFirstLine = false;
	bool exceptInfoLine = false;
	EraseAllRecords(pRecordsSet);

	tALMDRecord NewRecord;

	pRecordsSet->nCountOfRecords = 0;
	int nCount;
	while(sizeof(tALMDRecord) == (nCount = pLogFile->Read(&NewRecord, sizeof(tALMDRecord))))
	{
		tALMDRecord *pNewRecord = new tALMDRecord;
		memcpy(pNewRecord, &NewRecord, sizeof(tALMDRecord));
		pRecordsSet->m_ALMDRecords.AddTail(pNewRecord);
		pRecordsSet->nCountOfRecords++;
	}

	return 0;
}

int CRepository::ReadFromLogFile(CString strFileName)
{
	CFile LogFile;
	CFileException ex;
	tLogType logType=LT_LOG;
	CString strExtName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('.') - 1);

	if ( (strExtName.MakeUpper() != _T("LOG")) )
	{
		AfxMessageBox(_T("Please enter the LOG File"), MB_OK);
		return -1;
	}

	m_iDispIndex = 0;
	if (!LogFile.Open(strFileName.GetBuffer(), CFile::modeRead, &ex)) {
		return -1;//Can't open file 
	}
	CStatus::Instance()->strLastOpenFileName = strFileName;
	SetCurOpenFile(strFileName);
	if (m_DataRecords.m_ALMDRecords.GetCount() != 0)EraseAllRecords();//!이미사용하고 있는 것이 있으면 날린다.
	ReadFromLogFile(&LogFile, &m_DataRecords, logType);
	LogFile.Close();
	return 0;
}


int CRepository::SaveLogFile(CString strFileName)
{
	CFile LogFile;
	CFileException ex;
	tLogType logType = LT_LOG;
	CString strExtName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('.') - 1);

	if ((strExtName.MakeUpper() != _T("LOG")))
	{
		AfxMessageBox(_T("Please enter the LOG File"), MB_OK);
		return -1;
	}

	m_iDispIndex = 0;
	if (!LogFile.Open(strFileName.GetBuffer(), CFile::modeWrite | CFile::modeCreate, &ex)) {
		return -1;//Can't open file 
	}
	CStatus::Instance()->strLastOpenFileName = strFileName;
	SetCurOpenFile(strFileName);

	POSITION pos;
	tALMDRecord *pNewRecord;

	if (m_DataRecords.m_ALMDRecords.GetCount())
	{
		for (pos = m_DataRecords.m_ALMDRecords.GetHeadPosition(); pos != NULL;)
		{
			pNewRecord = (tALMDRecord *)m_DataRecords.m_ALMDRecords.GetNext(pos);
			LogFile.Write(pNewRecord, sizeof(tALMDRecord));
		}
	}
	LogFile.Close();
	return 0;
}


int CRepository::AppendData(tALMDRecord * pNewRecord)
{
	m_DataRecords.m_ALMDRecords.AddTail(pNewRecord);

	tMarkInfo *pMarkInfo;
	pMarkInfo = new tMarkInfo;
	//pNewRecord->Index = m_DataRecords[0].nCountOfRecords;
	//if (!pNewRecord->strMark.IsEmpty())
	//{
	//	pMarkInfo->nIndex = m_DataRecords[0].nCountOfRecords;
	//	pMarkInfo->strMarkName = pNewRecord->strMark;
	//}

	AppendLogFile(pNewRecord);
	//UpdateRecord();
	m_DataRecords.nCountOfRecords++;
	return 0;
}

void CRepository::EraseAllRecords()
{
	EraseAllRecords(&m_DataRecords);
}

void CRepository::EraseAllRecords(tRecordsSet * pRecordsSet)
{
	EraseAllRecords(&(pRecordsSet->m_ALMDRecords));
	pRecordsSet->nCountOfRecords = 0;
}

void CRepository::EraseAllRecords(CPtrList *pRecords)
{
	POSITION pos;
	tALMDRecord *pNewRecord;

	if (pRecords->GetCount())
	{
		for (pos = pRecords->GetHeadPosition(); pos != NULL;)
		{
			pNewRecord = (tALMDRecord *)pRecords->GetNext(pos);
			delete pNewRecord;
		}
		pRecords->RemoveAll();
	}
}


void CRepository::NewMeasure()
{
	m_bNewMeasureState = true;
}


void CRepository::DeleteMark(int nIndex)
{
	tALMDRecord *pNewRecord;
	POSITION pos;
	pos = m_DataRecords.m_ALMDRecords.FindIndex(nIndex);
	if (!pos)return;
	pNewRecord = (tALMDRecord *)m_DataRecords.m_ALMDRecords.GetAt(pos);
	//pNewRecord->strMark = _T("");
}


void CRepository::UpdateMark(int nIndex, CString strMark)
{
	tALMDRecord *pNewRecord;
	POSITION pos;
	pos = m_DataRecords.m_ALMDRecords.FindIndex(nIndex);
	pNewRecord = (tALMDRecord *)m_DataRecords.m_ALMDRecords.GetAt(pos);
}


void CRepository::GetLogInfo(CString strCSVLine, CString & strMakeDate, CString & strSerialNo)
{
	CString subLine;
	int i=0;
	while (AfxExtractSubString(subLine, strCSVLine.GetBuffer(), i, _T(',')))
	{
		subLine.Trim();
		switch (i)
		{
		case 0://Skip "Date:" mark
			break;
		case 1://Get Date Info
			strMakeDate = subLine;
			break;
		case 2://Skip "Serial No:" mark
			break;
		case 3://Get SerialNo info
			strSerialNo = subLine;
			break;
		}
		i++;
	}

}




int CRepository::SetCurOpenFile(CString strCurFileName)
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->SetTitleCaption(strCurFileName);
	return 0;
}




void CRepository::WriteCurStatus()
{
	AppendLogFile(CStatus::Instance()->GetCurMeasureData());
}


void CRepository::ClearData()
{
	EraseAllRecords(&m_DataRecords);
	m_DataRecords.nCountOfRecords = 0;
	m_iDispIndex = 0;
}


tALMDRecord * CRepository::GetMeasureInfo(int nPos)
{
	POSITION pos;
	pos = m_DataRecords.m_ALMDRecords.FindIndex(nPos);
	if (!pos) return NULL;
	return  (tALMDRecord *)m_DataRecords.m_ALMDRecords.GetAt(pos);

}



CString CRepository::MakeCSVRecordString(struct tm * pDateTime, struct tm * pCalDateTime, tALMDRecord * pNewRecord)
{
	CString newRecord;
	CString strDate, strTime;
	CString strRawRoll, strRawPitch;
	CString strDispRoll, strDispPitch;
	CString strRefRoll, strRefPitch;
	CString strStableRoll, strStablePitch;
	CString strStablization;
	CString strGoNoGoValue;
	CString strAbsRel;
	CString	strElapseDay;
	CString strTemp;
	CString strCalElapseTime;
	CString strBattery;


	CString strRecord;
	strDate.Format(_T("%04d-%02d-%02d"), pDateTime->tm_year + 1900, pDateTime->tm_mon + 1, pDateTime->tm_mday); // 현재 일 반환
	strTime.Format(_T("%02d:%02d:%02d"), pDateTime->tm_hour, pDateTime->tm_min, pDateTime->tm_sec);
	int iCalElapseTime = (int)(difftime(pNewRecord->CurTime, pNewRecord->dateCalibration) / (60 * 60 * 24));
	strCalElapseTime.Format(_T("%d"), iCalElapseTime);
	if (CStatus::Instance()->IsDigit2Mode())
	{
		strRawRoll.Format(_T("%2.2f"), (pNewRecord->sRawRoll) / 1000.);
		strRawPitch.Format(_T("%2.2f"), (pNewRecord->sRawPitch) / 1000.);
		strDispRoll.Format(_T("%2.2f"), pNewRecord->dblDispRoll);
		strDispPitch.Format(_T("%2.2f"), pNewRecord->dblDispPitch);
		strRefRoll.Format(_T("%2.2f"), pNewRecord->dblRefRoll);
		strRefPitch.Format(_T("%2.2f"), pNewRecord->dblRefPitch);
		strGoNoGoValue.Format(_T("%2.2f"), pNewRecord->dblGoNoGoValue);
	}
	else
	{
		strRawRoll.Format(_T("%2.3f"), (pNewRecord->sRawRoll) / 1000.);
		strRawPitch.Format(_T("%2.3f"), (pNewRecord->sRawPitch) / 1000.);
		strDispRoll.Format(_T("%2.3f"), pNewRecord->dblDispRoll);
		strDispPitch.Format(_T("%2.3f"), pNewRecord->dblDispPitch);
		strRefRoll.Format(_T("%2.3f"), pNewRecord->dblRefRoll);
		strRefPitch.Format(_T("%2.3f"), pNewRecord->dblRefPitch);
		strGoNoGoValue.Format(_T("%2.3f"), pNewRecord->dblGoNoGoValue);
	}

	if (pNewRecord->bStableRoll)
		strStableRoll.Format(_T("Stable"));
	else
		strStableRoll.Format(_T("Unstable"));
	if (pNewRecord->bStablePitch)
		strStablePitch.Format(_T("Stable"));
	else
		strStablePitch.Format(_T("Unstable"));
	if (pNewRecord->bStablization)
		strStablization.Format(_T("Stable"));
	else
		strStablization.Format(_T("Unstable"));

	if (pNewRecord->bRel)
		strAbsRel.Format(_T("Relative"));
	else
		strAbsRel.Format(_T("Absolute"));
	strTemp.Format(_T("%d"), (int)((signed char)(pNewRecord->byTemperature)));
	strBattery.Format(_T("%d"), (int)(pNewRecord->byBattery));

	newRecord.Format(_T("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n"),
		strDate.GetBuffer(), strTime.GetBuffer(),
		strRawRoll.GetBuffer(), strRawPitch.GetBuffer(),
		strDispRoll.GetBuffer(), strDispPitch.GetBuffer(),
		strAbsRel.GetBuffer(),
		strRefRoll.GetBuffer(), strRefPitch.GetBuffer(),
		strStableRoll.GetBuffer(), strStablePitch.GetBuffer(),
		strStablization.GetBuffer(),
		strGoNoGoValue.GetBuffer(),
		strCalElapseTime.GetBuffer(),
		strTemp.GetBuffer(),
		strBattery.GetBuffer(),
		pNewRecord->MarkInfo.szOperator,
		pNewRecord->MarkInfo.szTool,
		pNewRecord->MarkInfo.szStation,
		pNewRecord->MarkInfo.szComment
		);

	return newRecord;
}
