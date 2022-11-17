#include "stdafx.h"
#include "Repository.h"
#include "Status.h"
#include <locale.h>
#include "MainFrm.h"

// 1,	 2,	   3,	 4,	   5,	  6,	   7,	    8,	     9,	   10,	   11,	     12,	  13,         14,        15,          16,                     17,       18,   19,     20
// Date, Time, Unit, Gap1, Gap2, Gap3 , Offset1, Offset2, Offset3, Target1, Target2, Target3, Tolerance1, Tolerance2, Tolerance3, Days since Calibration, Operator, Tool, Station, Comment
#define CSV_HEADER_FORMAT _T("Date, Time, Unit, Gap1, Gap2, Gap3, Target1, Target2, Target3, Tolerance1, Tolerance2, Tolerance3, Offset, Offset1, Offset2, Offset3, Days since Calibration, Operator, Tool, Station, Comment\n")

CRepository::CRepository()
	: m_strCurLogName(_T(""))
	, m_strCurCSVName(_T(""))
	, m_bNewMeasureState(FALSE)
	, m_iDispIndex(0)
{
	
}

CRepository::~CRepository()
{
}

void CRepository::WriteCurStatus()
{
	AppendLogFile(CStatus::Instance()->GetCurMeasureData());
}

int CRepository::AppendData(tAGMDRecord * pNewRecord)
{
	m_DataRecords.m_AGMDRecords.AddTail(pNewRecord);
	AppendLogFile(pNewRecord);
	m_DataRecords.nCountOfRecords++;
	return 0;
}

int CRepository::AppendLogFile(tAGMDRecord *pNewRecord)
{
	setlocale(LC_ALL, "Korean");
	CFile LogFile;
	CStdioFile CSVFile;

	CString strLogFile = CStatus::Instance()->GetDataSaveFolder() + _T("\\");
	strLogFile += CStatus::Instance()->GetDataFileName();
	m_strCurLogName = strLogFile + _T(".LOG");
	m_strCurCSVName = strLogFile + _T(".CSV");

	pNewRecord->workEnvInfo = CStatus::Instance()->GetWorkEnvInfo();
	AppendLogFileCore(&LogFile, pNewRecord);
	AppendCSVFileCore(&CSVFile, pNewRecord);

	if (m_bNewMeasureState)	m_bNewMeasureState = false;

	return 0;
}

int CRepository::NewMakeLogFile(CString strFileName)
{
	setlocale(LC_ALL, "Korean");
	CFile LogFile;
	CStdioFile CSVFile;

	CString strLogFile = strFileName.Left(strFileName.GetLength() -4);
	//CString strLogFile = strFileName;
	m_strCurLogName = strLogFile + _T(".LOG");
	m_strCurCSVName = strLogFile + _T(".CSV");

	NewLogFileCore(&LogFile);
	NewCSVFileCore(&CSVFile);

	return 0;
}
#pragma warning(disable: 4996)
int CRepository::AppendCSVFileCore(CStdioFile *pStdioFile, tAGMDRecord * pNewRecord)
{
	// 향후 Calibration 데이터 저장
	setlocale(LC_ALL, "Korean");
	if (!(PathFileExists(m_strCurCSVName.GetBuffer())) | m_bNewMeasureState)
	{
		if (!pStdioFile->Open(m_strCurCSVName, CFile::modeCreate | CFile::modeWrite))
		{
			return -1;//파일생성불가
		}
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
	//pNewRecord->workEnvInfo = CStatus::Instance()->GetWorkEnvInfo();
	CString newRecord;
	if(CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
		newRecord =MakeCSVRecordString(&DateTime, &CalDateTime, pNewRecord);
	else
		newRecord = MakeReviewCSVRecordString(&DateTime, &CalDateTime, pNewRecord);
	pStdioFile->WriteString(newRecord);
	pStdioFile->Close();
	return 0;
}

int CRepository::NewCSVFileCore(CStdioFile *pStdioFile)
{
	int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
	setlocale(LC_ALL, "Korean");
	if (!pStdioFile->Open(m_strCurCSVName, CFile::modeCreate | CFile::modeWrite))
	{
		return -1;//파일생성불가
	}
	CString strLogInfo;
	CString Header(CSV_HEADER_FORMAT);
	pStdioFile->WriteString(Header.GetBuffer());

	for (int i = 0; i < nCount; i++)
	{
		tAGMDRecord *pNewRecord = CStatus::Instance()->GetRepository()->GetMeasureInfo(i);
			
		struct tm DateTime;
		struct tm CalDateTime;
		DateTime = *localtime(&(pNewRecord->CurTime));
		CalDateTime = *localtime(&(pNewRecord->dateCalibration));
		CString newRecord;
		if (CStatus::Instance()->GetProgramMode() == CStatus::PM_MONITORING)
			newRecord = MakeCSVRecordString(&DateTime, &CalDateTime, pNewRecord);
		else
			newRecord = MakeReviewCSVRecordString(&DateTime, &CalDateTime, pNewRecord);
		pStdioFile->WriteString(newRecord);
	}	
	pStdioFile->Close();
	return 0;
}

int CRepository::AppendLogFileCore(CFile * pFile, tAGMDRecord * pNewRecord)
{
	if (!(PathFileExists(m_strCurLogName.GetBuffer())) | m_bNewMeasureState) 
	{
		if (!pFile->Open(m_strCurLogName, CFile::modeCreate | CFile::modeWrite))
			return -1;//파일생성불가
	}
	else
	{
		if (!pFile->Open(m_strCurLogName, CFile::modeWrite))
			return -1;//파일생성불가

		pFile->SeekToEnd();
	}
	//pNewRecord->workEnvInfo = CStatus::Instance()->GetWorkEnvInfo();
	
	//CTime CurTime = CTime::GetCurrentTime();
	//	pNewRecord->CurDate = CurTime;
	pFile->Write(pNewRecord, sizeof(tAGMDRecord));

	pFile->Close();
	return 0;
}

int CRepository::NewLogFileCore(CFile * pFile)
{
	if (!pFile->Open(m_strCurLogName, CFile::modeCreate | CFile::modeWrite))
	{
		return -1;//파일생성불가
	}
	int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		tAGMDRecord *pNewRecord = CStatus::Instance()->GetRepository()->GetMeasureInfo(i);
		pFile->Write(pNewRecord, sizeof(tAGMDRecord));
	}
	pFile->Close();
	return 0;
}

CString CRepository::MakeCSVRecordString(struct tm * pDateTime, struct tm * pCalDateTime, tAGMDRecord * pNewRecord)
{
	// 향후 Calibration 데이터 저장
	CString newRecord;
	CString strDate, strTime, strUnit;
	CString strGap1, strGap2, strGap3;
	CString strOffset_Gap1, strOffset_Gap2, strOffset_Gap3, strIsOffsetMode;
	CString strTarget_Gap1, strTarget_Gap2, strTarget_Gap3;
	CString strTolerance_Gap1, strTolerance_Gap2, strTolerance_Gap3;
	//Date, Time, Unit, Gap1, Gap2, Gap3, Target1, Target2, Target3, Tolerance1, Tolerance2, Tolerance3, Offset1, Offset2, Offset3, Days since Calibration, Operator, Tool, Station, Comment
	CString strCalibrationDate, strOperator, strTool, strSation, strComment;
	CString strRecord;

	strDate.Format(_T("%04d-%02d-%02d"), pDateTime->tm_year + 1900, pDateTime->tm_mon + 1, pDateTime->tm_mday); // 현재 일 반환
	strTime.Format(_T("%02d:%02d:%02d"), pDateTime->tm_hour, pDateTime->tm_min, pDateTime->tm_sec);

	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
	{
		strUnit.Format(_T("mm"));
		strGap1.Format(_T("%.3f"), pNewRecord->usGap1 / UNIT_MM_VALUE);
		strGap2.Format(_T("%.3f"), pNewRecord->usGap2 / UNIT_MM_VALUE);
		strGap3.Format(_T("%.3f"), pNewRecord->usGap3 / UNIT_MM_VALUE);
	}

	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
	{
		strUnit.Format(_T("inches"));
		strGap1.Format(_T("%.3f"), pNewRecord->usGap1 / UNIT_MM_VALUE * UNIT_INCH_VALUE);
		strGap2.Format(_T("%.3f"), pNewRecord->usGap2 / UNIT_MM_VALUE * UNIT_INCH_VALUE);
		strGap3.Format(_T("%.3f"), pNewRecord->usGap3 / UNIT_MM_VALUE * UNIT_INCH_VALUE);
	}
	if (CStatus::Instance()->GetIsOffsetMode())
	{
		strIsOffsetMode.Format(_T("O"));
		strOffset_Gap1.Format(_T("%.3f"), CStatus::Instance()->GetOffset(0));
		strOffset_Gap2.Format(_T("%.3f"), CStatus::Instance()->GetOffset(1));
		strOffset_Gap3.Format(_T("%.3f"), CStatus::Instance()->GetOffset(2));
	}
	else
	{
		strIsOffsetMode.Format(_T("X"));
		strOffset_Gap1.Format(_T(""));
		strOffset_Gap2.Format(_T(""));
		strOffset_Gap3.Format(_T(""));
	}
	if (CStatus::Instance()->GetIsGapSameSettings())
	{
		strTarget_Gap1.Format(_T("%.3f"), CStatus::Instance()->GetTarget(3));
		strTarget_Gap2.Format(_T("%.3f"), CStatus::Instance()->GetTarget(3));
		strTarget_Gap3.Format(_T("%.3f"), CStatus::Instance()->GetTarget(3));
		strTolerance_Gap1.Format(_T("%.3f"), CStatus::Instance()->GetTolerance(3));
		strTolerance_Gap2.Format(_T("%.3f"), CStatus::Instance()->GetTolerance(3));
		strTolerance_Gap3.Format(_T("%.3f"), CStatus::Instance()->GetTolerance(3));
	}
	else
	{
		strTarget_Gap1.Format(_T("%.3f"), CStatus::Instance()->GetTarget(0));
		strTarget_Gap2.Format(_T("%.3f"), CStatus::Instance()->GetTarget(1));
		strTarget_Gap3.Format(_T("%.3f"), CStatus::Instance()->GetTarget(2));
		strTolerance_Gap1.Format(_T("%.3f"), CStatus::Instance()->GetTolerance(0));
		strTolerance_Gap2.Format(_T("%.3f"), CStatus::Instance()->GetTolerance(1));
		strTolerance_Gap3.Format(_T("%.3f"), CStatus::Instance()->GetTolerance(2));
	}
	strOperator = pNewRecord->workEnvInfo.szOperator;
	strTool = pNewRecord->workEnvInfo.szTool;
	strSation = pNewRecord->workEnvInfo.szStation;
	strComment = pNewRecord->workEnvInfo.szComment;


	struct tm CalibrationDateTime = *localtime(&(pNewRecord->dateCalibration));
	CString strClibrationDate;

	strClibrationDate.Format(_T("%04d%02d%02d"), CalibrationDateTime.tm_year + 1900, CalibrationDateTime.tm_mon + 1, CalibrationDateTime.tm_mday); // 현재 일 반환

	CString strYear = strClibrationDate.Mid(0, 4);
	CString strMon = strClibrationDate.Mid(4, 2);
	CString strDay = strClibrationDate.Mid(6, 2);
	strCalibrationDate = strYear + _T("-") + strMon + _T("-") + strDay;

	newRecord.Format(_T("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s, %s\n"),
		strDate, strTime, strUnit,
		strGap1, strGap2, strGap3,
		strTarget_Gap1, strTarget_Gap2, strTarget_Gap3,
		strTolerance_Gap1, strTolerance_Gap2, strTolerance_Gap3,
		strIsOffsetMode, strOffset_Gap1, strOffset_Gap2, strOffset_Gap3,
		strCalibrationDate, strOperator, strTool, strSation, strComment
	);


	return newRecord;
}

CString CRepository::MakeReviewCSVRecordString(struct tm * pDateTime, struct tm * pCalDateTime, tAGMDRecord * pNewRecord)
{// 향후 Calibration 데이터 저장
	CString newRecord;
	CString strDate, strTime, strUnit;
	CString strGap1, strGap2, strGap3;
	CString strOffset_Gap1, strOffset_Gap2, strOffset_Gap3, strIsOffsetMode;
	CString strTarget_Gap1, strTarget_Gap2, strTarget_Gap3;
	CString strTolerance_Gap1, strTolerance_Gap2, strTolerance_Gap3;
	//Date, Time, Unit, Gap1, Gap2, Gap3, Target1, Target2, Target3, Tolerance1, Tolerance2, Tolerance3, Offset1, Offset2, Offset3, Days since Calibration, Operator, Tool, Station, Comment
	CString strCalibrationDate, strOperator, strTool, strSation, strComment;
	CString strRecord;

	strDate.Format(_T("%04d-%02d-%02d"), pDateTime->tm_year + 1900, pDateTime->tm_mon + 1, pDateTime->tm_mday); // 현재 일 반환
	strTime.Format(_T("%02d:%02d:%02d"), pDateTime->tm_hour, pDateTime->tm_min, pDateTime->tm_sec);

	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_MM)
	{
		strUnit.Format(_T("mm"));
		strGap1.Format(_T("%.3f"), pNewRecord->usGap1 / UNIT_MM_VALUE);
		strGap2.Format(_T("%.3f"), pNewRecord->usGap2 / UNIT_MM_VALUE);
		strGap3.Format(_T("%.3f"), pNewRecord->usGap3 / UNIT_MM_VALUE);
	}

	if (CStatus::Instance()->GetUnitMode() == CStatus::tUnitMode::UNIT_INCH)
	{
		strUnit.Format(_T("inches"));
		strGap1.Format(_T("%.3f"), pNewRecord->usGap1 / UNIT_MM_VALUE * UNIT_INCH_VALUE);
		strGap2.Format(_T("%.3f"), pNewRecord->usGap2 / UNIT_MM_VALUE * UNIT_INCH_VALUE);
		strGap3.Format(_T("%.3f"), pNewRecord->usGap3 / UNIT_MM_VALUE * UNIT_INCH_VALUE);
	}
	if (pNewRecord->bIsOffsetMode)
	{
		strIsOffsetMode.Format(_T("O"));
		strOffset_Gap1.Format(_T("%.3f"), pNewRecord->dblOffset_Gap1);
		strOffset_Gap2.Format(_T("%.3f"), pNewRecord->dblOffset_Gap2);
		strOffset_Gap3.Format(_T("%.3f"), pNewRecord->dblOffset_Gap3);
	}
	else
	{
		strIsOffsetMode.Format(_T("X"));
		strOffset_Gap1.Format(_T(""));
		strOffset_Gap2.Format(_T(""));
		strOffset_Gap3.Format(_T(""));
	}
	if (pNewRecord->bIsAllGapSame)
	{
		strTarget_Gap1.Format(_T("%.3f"),pNewRecord->dblTarget_AllGap);
		strTarget_Gap2.Format(_T("%.3f"), pNewRecord->dblTarget_AllGap);
		strTarget_Gap3.Format(_T("%.3f"), pNewRecord->dblTarget_AllGap);
		strTolerance_Gap1.Format(_T("%.3f"), pNewRecord->dblTolerance_AllGap);
		strTolerance_Gap2.Format(_T("%.3f"), pNewRecord->dblTolerance_AllGap);
		strTolerance_Gap3.Format(_T("%.3f"), pNewRecord->dblTolerance_AllGap);
	}
	else
	{
		strTarget_Gap1.Format(_T("%.3f"), pNewRecord->dblTarget_Gap1);
		strTarget_Gap2.Format(_T("%.3f"), pNewRecord->dblTarget_Gap2);
		strTarget_Gap3.Format(_T("%.3f"), pNewRecord->dblTarget_Gap3);
		strTolerance_Gap1.Format(_T("%.3f"), pNewRecord->dblTolerance_Gap1);
		strTolerance_Gap2.Format(_T("%.3f"), pNewRecord->dblTolerance_Gap2);
		strTolerance_Gap3.Format(_T("%.3f"), pNewRecord->dblTolerance_Gap3);
	}
	strOperator = pNewRecord->workEnvInfo.szOperator;
	strTool = pNewRecord->workEnvInfo.szTool;
	strSation = pNewRecord->workEnvInfo.szStation;
	strComment = pNewRecord->workEnvInfo.szComment;


	struct tm CalibrationDateTime = *localtime(&(pNewRecord->dateCalibration));
	CString strClibrationDate;

	strClibrationDate.Format(_T("%04d%02d%02d"), CalibrationDateTime.tm_year + 1900, CalibrationDateTime.tm_mon + 1, CalibrationDateTime.tm_mday); // 현재 일 반환

	CString strYear = strClibrationDate.Mid(0, 4);
	CString strMon = strClibrationDate.Mid(4, 2);
	CString strDay = strClibrationDate.Mid(6, 2);
	strCalibrationDate = strYear + _T("-") + strMon + _T("-") + strDay;

	newRecord.Format(_T("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s, %s\n"),
		strDate, strTime, strUnit,
		strGap1, strGap2, strGap3,
		strTarget_Gap1, strTarget_Gap2, strTarget_Gap3,
		strTolerance_Gap1, strTolerance_Gap2, strTolerance_Gap3,
		strIsOffsetMode, strOffset_Gap1, strOffset_Gap2, strOffset_Gap3,
		strCalibrationDate, strOperator, strTool, strSation, strComment
	);


	return newRecord;

}

int CRepository::ReadFromLogFile(CString strFileName)
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
	if (!LogFile.Open(strFileName.GetBuffer(), CFile::modeRead, &ex)) {
		return -1;//Can't open file 
	}
	CStatus::Instance()->m_strLastOpenFileName = strFileName;
	SetCurOpenFile(strFileName);
	if (m_DataRecords.m_AGMDRecords.GetCount() != 0)
		EraseAllRecords();//!이미사용하고 있는 것이 있으면 날린다.

	ReadFromLogFile(&LogFile, &m_DataRecords, logType);
	LogFile.Close();
	return 0;
}

int CRepository::SetCurOpenFile(CString strCurFileName)
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->SetTitleCaption(strCurFileName);
	return 0;
}

void CRepository::EraseAllRecords()
{
	EraseAllRecords(&m_DataRecords);
}

void CRepository::EraseAllRecords(tRecordsSet * pRecordsSet)
{
	EraseAllRecords(&(pRecordsSet->m_AGMDRecords));
	pRecordsSet->nCountOfRecords = 0;
}

void CRepository::EraseAllRecords(CPtrList *pRecords)
{
	POSITION pos;
	tAGMDRecord *pNewRecord;

	if (pRecords->GetCount())
	{
		for (pos = pRecords->GetHeadPosition(); pos != NULL;)
		{
			pNewRecord = (tAGMDRecord *)pRecords->GetNext(pos);
			delete pNewRecord;
		}
		pRecords->RemoveAll();
	}
}

int CRepository::ReadFromLogFile(CFile* pLogFile, tRecordsSet * pRecordsSet, tLogType logType)
{
	CString csvLine, subLine;
	bool exceptFirstLine = false;
	bool exceptInfoLine = false;
	EraseAllRecords(pRecordsSet);

	tAGMDRecord NewRecord;

	pRecordsSet->nCountOfRecords = 0;
	int nCount;
	while (sizeof(tAGMDRecord) == (nCount = pLogFile->Read(&NewRecord, sizeof(tAGMDRecord))))
	{
		tAGMDRecord *pNewRecord = new tAGMDRecord;
		memcpy(pNewRecord, &NewRecord, sizeof(tAGMDRecord));
		pRecordsSet->m_AGMDRecords.AddTail(pNewRecord);
		pRecordsSet->nCountOfRecords++;
	}

	return 0;
}

void CRepository::ClearData()
{
	EraseAllRecords(&m_DataRecords);
	m_DataRecords.nCountOfRecords = 0;
	m_iDispIndex = 0;
}

tAGMDRecord * CRepository::GetMeasureInfo(int nPos)
{
	POSITION pos;
	pos = m_DataRecords.m_AGMDRecords.FindIndex(nPos);
	if (!pos) return NULL;
	return  (tAGMDRecord *)m_DataRecords.m_AGMDRecords.GetAt(pos);
}

void CRepository::ExportCSVFile(CString strFileName)
{
	setlocale(LC_ALL, "Korean");
	CStdioFile csvFile;
	CFileException ex;
	CString strCSVFileName = strFileName + _T(".CSV");

	if (!csvFile.Open(strCSVFileName.GetBuffer(), CFile::modeWrite | CFile::modeCreate, &ex)) {
		return;//Can't open file 
	}
	CString strLogInfo;
	CString Header(CSV_HEADER_FORMAT);
	csvFile.WriteString(Header.GetBuffer());

	POSITION pos;
	tAGMDRecord *pNewRecord;
	for (pos = m_DataRecords.m_AGMDRecords.GetHeadPosition(); pos != NULL;)
	{
		struct tm DateTime;
		struct tm CalDateTime;
		pNewRecord = (tAGMDRecord *)m_DataRecords.m_AGMDRecords.GetNext(pos);
		DateTime = *localtime(&(pNewRecord->CurTime));
		CalDateTime = *localtime(&(pNewRecord->dateCalibration));
		CString newRecord = MakeCSVRecordString(&DateTime, &CalDateTime, pNewRecord);
		csvFile.WriteString(newRecord.GetBuffer());
	}
	csvFile.Close();
}