#include "stdafx.h"
#include "Repository.h"
#include "Status.h"
#include "Strsafe.h"
#include "MainFrm.h"
#include <locale.h>
#include <mutex>
#include "AVMDMonitorDoc.h"
#include "AVMDMonitorView.h"
#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib, "winmm.lib")
#include "Mmsystem.h"

// 1,		2,		3,		4,		5		6	
// Time,	X,		Y,		Z,		RMS,	Mark
#define CSV_HEADER_FORMAT _T("Time,X,Y,Z,RMS,Mark\n")
#define CSV_DATAGFORCE_FORMAT _T("%d,%5.3f,%5.3f,%5.3f,%5.3f,%s\n")
#define CSV_DATAGAL_FORMAT _T("%d,%5.3f,%5.3f,%5.3f,%5.3f,%s\n")
#define TAG_HEADER_FORMAT _T("Type,Index,Param1,Param2,Param3,Param4\n")
#define TAGNAME_WEI _T("WorkEnv")
#define TAGNAME_MARK _T("Mark")
#define TAGNAME_MINFO _T("MachineEnv")


CRepository::CRepository()
	: m_strCurLogName(_T(""))
	, m_bNewMeasureState(false)
	, m_strCurCSVName(_T(""))
	, m_iDispIndex(0)
	, m_strCurMarkName(_T(""))
	, m_sizePrepareQueue(0)
	, m_iLastWriteIndex(0)
	, m_pWFileWriteThread(NULL)
	, m_bTerminateThread(FALSE)
	, m_iLastWriteTime(0)
	//, m_CurDataPosition(0)
	, m_FileSaveStatus(SS_STOP)
//jubatemp	, m_iTargetIndex(0)
	, m_bIsSaveTaretIndex(FALSE)
	, m_iCSVDispIndexNo(0)
	//, m_strCurTagName(_T(""))
	, m_nToWriteMarkNo(0)
	, m_nCountOfWrite(0)
	, m_nPreRecordTime(0)
	, m_iTargetCount(0)
	, m_iPausePos(0)
{
	m_strCurTagName[0] = _T("");
	m_strCurTagName[1] = _T("");

	m_DataRecords[0].nCountOfRecords = 0;
	m_DataRecords[1].nCountOfRecords = 0;
	m_CurDataPosition[0] = 0;
	m_CurDataPosition[1] = 0;
	tWorkEnvInfo *pWEInfo;
	pWEInfo = new tWorkEnvInfo;
	memset(pWEInfo, 0, sizeof(tWorkEnvInfo));
	m_WorkEnvInfo[0].push_back(pWEInfo);
	pWEInfo = new tWorkEnvInfo;

	memset(pWEInfo, 0, sizeof(tWorkEnvInfo));
	m_WorkEnvInfo[1].push_back(pWEInfo);
}


CRepository::~CRepository()
{
}

int CRepository::TestFileWrite()
{
	tAVMDRecord *pNewRecord= NULL;

	CString strLogFile = CStatus::Instance()->GetDataSaveFolder() + _T("\\");
	CStatus::Instance()->m_strMakeDate[0] = "160404";
	CStatus::Instance()->m_strSerialNo[0] = "3P70016";

	MakeNewFileName();
	MakeFileThread();
	CString str;
	tWorkEnvInfo WorkEnvInfo;
	memset(&WorkEnvInfo, 0, sizeof(tWorkEnvInfo));
	wsprintf(WorkEnvInfo.szOperator, TEXT("Operator"));
	wsprintf(WorkEnvInfo.szStation, TEXT("A"));
	wsprintf(WorkEnvInfo.szTool, TEXT("Seso"));
	StringCbPrintf(WorkEnvInfo.szComment, 500, TEXT("Seso 테스트"));
	CStatus::Instance()->SetWorkEnvInfoToAdd(&WorkEnvInfo);

	int sizeAVMDRecord = sizeof(tAVMDRecord);

	int nStep = 0;
	for (int j = 0; j < 3600*4; j++)
	{
		printf(("\nThis is new line %d"), j);
		for (int nNum = 0; nNum < 20; nNum++)
		{
			pNewRecord = new tAVMDRecord;
			pNewRecord->timeCurTime = (UINT)time(NULL);
			pNewRecord->byNum = nNum;
			pNewRecord->byBattery = nNum % 100;
			pNewRecord->byTemperature = 20;

			for (int i = 0; i < VALUESET_COUNT; i++)
			{
				int XValue = (int)(900*sin((M_PI*nStep)/180));
				int YValue = (int)(900 * sin((M_PI*(nStep + 90)) / 180));
				int ZValue = (int)(900 * sin((M_PI*(nStep + 180)) / 180));
				CAVMDPacket::SetXYZ((BYTE *)(&(pNewRecord->accInfo[i])), XValue, YValue, ZValue);
				nStep++;
			}
			AppendData(pNewRecord);
		}
		CString strMark;
		strMark.Format(_T("%d"), nStep);
		AddMark(nStep, strMark);
	}

	AfxMessageBox(_T("BeginSave"));
	m_FileSaveStatus = SS_STOP;
	BeginSave(0, 500*3600*4);
	//!Wait File Save is terminated.
	if (!IsFileSaveDone())Sleep(1000);
	StopSave();
	TerminateFileSaveThread();

	return 0;
}


int CRepository::AppendLogFileCore(CFile * pFile, tAVMDRecord * pNewRecord)
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
	pFile->Write(pNewRecord, sizeof(tAVMDRecord));

	pFile->Close();
	return 0;
}



#pragma warning(disable: 4996)
int CRepository::AppendCSVFileCore(CStdioFile *pStdioFile, tAVMDRecord * pNewRecord)
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

	/*struct tm DateTime;
	struct tm CalDateTime;
	DateTime = *localtime(&(pNewRecord->CurTime));
	CalDateTime = *localtime(&(pNewRecord->dateCalibration));
	pNewRecord->MarkInfo = CStatus::Instance()->GetMarkToAdd();
	CString newRecord = MakeCSVRecordString(&DateTime, &CalDateTime, pNewRecord);

	pStdioFile->WriteString(newRecord);
	*/pStdioFile->Close();
	return 0;
}

/*! 파일이 없으면 만들고 추가, 없으면 추가*/
int CRepository::AppendLogFile(tAVMDRecord *pNewRecord)
{
	setlocale(LC_ALL, "Korean");
	CFile LogFile;
	CStdioFile CSVFile;

	AppendLogFileCore(&LogFile, pNewRecord);
	AppendCSVFileCore(&CSVFile, pNewRecord);

	if(m_bNewMeasureState)	m_bNewMeasureState = false;

	return 0;
}

int CRepository::FlushData(int nInputType)// 저장
{
	setlocale(LC_ALL, "Korean");
	CFile LogFile;
	CStdioFile CSVFile;
	CStdioFile TagFile;
	//! 쓸 데이타 겟수가 BLOCKCOUNT_TOSAVE이하면 다음에 Write 다만 MAXTIME_TOCHARGE시간이 지나면 나머지 모두 쓰기를 한다.
	int nCurTime = timeGetTime();
	int nElapseTime = abs(nCurTime - m_iLastWriteTime);
	if (!m_DataRecords[0].m_AVMDRecords.size())return -1;
	int nBlockToSave = GetDataCount() - m_iLastWriteIndex;
	if (nBlockToSave <= 0)return 0;
	int iEndWriteIndex = GetDataCount();
//jubatemp	printf("\nLastWriteIndex %d, iEndWirteIndex %d", m_iLastWriteIndex, iEndWriteIndex);
	int nRestCount = m_iTargetCount - m_nCountOfWrite;
	int nLastToEnd = iEndWriteIndex - m_iLastWriteIndex;//시작할 인덱스부터 마지막 인덱스까지의 갯수 
	if (nLastToEnd >= nRestCount)
	{
		iEndWriteIndex = m_iLastWriteIndex + nRestCount;
	}
	else
	{
		iEndWriteIndex = m_iLastWriteIndex + nLastToEnd;
	}

	FlushLogFileCore(&LogFile, m_iLastWriteIndex, iEndWriteIndex);
	FlushTagFileCore(&TagFile, nInputType);
	FlushCSVFileCore(&CSVFile, m_iLastWriteIndex, iEndWriteIndex);
	m_iLastWriteTime = timeGetTime();
	m_iLastWriteIndex = iEndWriteIndex + 1;
	if (m_nCountOfWrite >= m_iTargetCount)
	{
		m_bIsSaveTaretIndex = TRUE;
		m_FileSaveStatus = SS_STOP;
	}
	return 0;
}

int CRepository::FlushLogFileCore(CFile * pFile, int iStartWriteIndex, int iEndWriteIndex)
{
	int iStartWriteRecord = iStartWriteIndex / VALUESET_COUNT;
	int iEndwriteRecord = iEndWriteIndex / VALUESET_COUNT;

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

	int nCount = m_DataRecords[0].m_AVMDRecords.size();
	if (iEndwriteRecord > nCount)iEndwriteRecord = nCount;
	for (int i = iStartWriteRecord; i < iEndwriteRecord; i++)
	{
		m_mtxLocker.lock();
		tAVMDRecord *pNewRecord = m_DataRecords[0].m_AVMDRecords.at(i);
		m_mtxLocker.unlock();
		ASSERT(pNewRecord != NULL);
		pFile->Write(pNewRecord, sizeof(tAVMDRecord));
		m_nCountOfWrite += VALUESET_COUNT;
	}

	pFile->Close();
	return 0;
}

#pragma warning(disable: 4996)
int CRepository::FlushCSVFileCore(CStdioFile *pStdioFile, int iStartWriteIndex, int iEndWriteIndex, int nInputType)
{
	int iStartWriteRecord = iStartWriteIndex / VALUESET_COUNT;
	int iEndwriteRecord = iEndWriteIndex / VALUESET_COUNT;
	setlocale(LC_ALL, "Korean");
	if (!(PathFileExists(m_strCurCSVName.GetBuffer())) | m_bNewMeasureState) {
		if (!pStdioFile->Open(m_strCurCSVName, CFile::modeCreate | CFile::modeWrite))
		{
			return -1;//파일생성불가
		}
		//USHORT uBOM = 0xfeff;
		//pStdioFile->Write(&uBOM, sizeof(uBOM)); // 파일의 첫부분에 BOM저장 
		CString strLogInfo;
		strLogInfo.Format(_T("Serial Number:,%s, Unit:, %s\n"), CStatus::Instance()->m_strSerialNo[nInputType].GetBuffer(), CStatus::Instance()->m_strTimeDomainUnit.GetBuffer());
		pStdioFile->WriteString(strLogInfo);
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

	int nCount = m_DataRecords[0].m_AVMDRecords.size();
	if (iEndwriteRecord > nCount)iEndwriteRecord = nCount;
	for (int i = iStartWriteRecord; i < iEndwriteRecord; i++)
	{
		m_mtxLocker.lock();
		tAVMDRecord *pNewRecord = m_DataRecords[0].m_AVMDRecords.at(i);
		m_mtxLocker.unlock();
		ASSERT(pNewRecord != NULL);
		for (int j = 0; j < VALUESET_COUNT; j++)
		{
			tAVMDData dataTemp;
			CAVMDPacket::GetXYZ(dataTemp.iXValue, dataTemp.iYValue, dataTemp.iZValue, (BYTE *)(&(pNewRecord->accInfo[j])));
			CString strData = FlushCSVRecordString(m_iCSVDispIndexNo++, &dataTemp);
			pStdioFile->WriteString(strData);
		}
	}
	pStdioFile->Close();
	return 0;
}


int CRepository::FlushTagFileCore(CStdioFile * pTagFile, int nInputType)
{

	setlocale(LC_ALL, "Korean");
	if (!(PathFileExists(m_strCurTagName[nInputType].GetBuffer())) | m_bNewMeasureState) {
		if (!pTagFile->Open(m_strCurTagName[nInputType], CFile::modeCreate | CFile::modeWrite))
		{
			return -1;//파일생성불가
		}
		CString Header(TAG_HEADER_FORMAT);
		pTagFile->WriteString(Header.GetBuffer());
		if (m_WorkEnvInfo[nInputType].size())
		{
			AppendEnvInfo(pTagFile);
		}
		if ( (CStatus::Instance()->m_bSavePreRecData) && (CStatus::Instance()->GetPreRecordTime()) )
		{
			AddPreRecordMark();
			if(CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::ONE_THOUSAND)
				AddBeginMark(CStatus::Instance()->GetPreRecordTime());
			if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::Five_HUNDRED)
				AddBeginMark(CStatus::Instance()->GetPreRecordTime() / 2);
		}
		else
			AddBeginMark();
		AddMachineInfo(pTagFile);
	}
	else
	{
		if (!pTagFile->Open(m_strCurTagName[nInputType], CFile::modeWrite))
		{
			return -1;//파일생성불가
		}
		pTagFile->SeekToEnd();
	}

	if (m_MarkInfo[nInputType].size())
	{
		AppendMarkInfo(pTagFile, nInputType);
	}

	pTagFile->Close();
	return 0;
}

int CRepository::AppendData(tAVMDRecord * pNewRecord)
{
	m_mtxLocker.lock();
	m_DataRecords[0].m_AVMDRecords.push_back(pNewRecord);
	//! 저장하기전에는 Queue관리를 하여 초과하면 버린다
	//if(!m_FileSaveStatus == SS_WRITE)
	//	while ((int)(m_DataRecords[0].m_AVMDRecords.size()) >= m_sizeQueue)
	//	{
	//		delete m_DataRecords[0].m_AVMDRecords.front();
	//		m_DataRecords[0].m_AVMDRecords.pop_front();
	//	}
	m_mtxLocker.unlock();
	int nCount = m_DataRecords[0].m_AVMDRecords.size();
	return 0;
}

int CRepository::AppendData(DVC_PACKET_MEAINFO *pPacket)
{
	tAVMDRecord *pAVMDRecord;
	pAVMDRecord = new tAVMDRecord;
	pAVMDRecord->timeCurTime = (UINT)time(NULL);
	pAVMDRecord->byBattery = pPacket->byBattery;
	pAVMDRecord->byTemperature = pPacket->byTemperatue;
	memcpy(pAVMDRecord->accInfo, pPacket->accInfo, sizeof(pPacket->accInfo));
	AppendData(pAVMDRecord);
	return 0;
}


//nStartPos이 -1면 전체저장
int CRepository::ExportDataFile(CString strDataFile, int nStartPos, int nEndPos)
{
	CString strLogFileName, strTagFileName, strCSVFileName;
	strLogFileName = strDataFile;

	//! 확장자를 제외한 이름을 같기 때문에 통일화시킴
	strLogFileName = strLogFileName;
	strCSVFileName = strLogFileName;
	strTagFileName = strLogFileName;

	strLogFileName += EXT_LOG;
	strCSVFileName += EXT_CSV;
	strTagFileName += EXT_TAG;

	ExportLogFile(strLogFileName, nStartPos, nEndPos);
	ExportTagFile(strTagFileName, nStartPos, nEndPos);
	ExportCSVFile(strCSVFileName, nStartPos, nEndPos);
	return 0;
}

BOOL CRepository::ExportLogFile(CString strFileName, int nStartPos, int nEndPos)
{
	CFile logFile;
	if (!logFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		return -1;//파일생성불가
	}

	tAVMDData AVMDData;
	tAVMDRecord AVMDRecord;
	if (nStartPos == -1)
	{
		int nCount = m_DataRecords[0].m_AVMDRecords.size();

		for (int i = 0; i < nCount; i++)
		{
			m_mtxLocker.lock();
			tAVMDRecord *pNewRecord = m_DataRecords[0].m_AVMDRecords.at(i);
			m_mtxLocker.unlock();
			ASSERT(pNewRecord != NULL);
			logFile.Write(pNewRecord, sizeof(tAVMDRecord));
		}
	}
	else
	{
		memset(&AVMDRecord, 0, sizeof(tAVMDRecord));
		int j = 0;
		for (int i = nStartPos; i < nEndPos; i++)
		{
			GetAtData(&AVMDData, i);
			if (j == 0)
			{
				AVMDRecord.byBattery = AVMDData.byBattery;
				AVMDRecord.byTemperature = AVMDData.byTemperature;
			}
			AVMDRecord.accInfo[j] = AVMDData.accInfo;
			if (j == (VALUESET_COUNT - 1))
			{
				logFile.Write(&AVMDRecord, sizeof(tAVMDRecord));
				j = 0;
			}
			else j++;
		}

		if (j != 0 ) logFile.Write(&AVMDRecord, sizeof(tAVMDRecord));
	}

	logFile.Close();
	return 0;

}


BOOL CRepository::ExportCSVFile(CString strFileName, int nStartPos, int nEndPos)
{
	CStdioFile csvFile;
	BOOL bIsPartial = FALSE;
	setlocale(LC_ALL, "Korean");
	if (!csvFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		return -1;//파일생성불가
	}
	if (nStartPos == -1)
	{
		nStartPos = 0;
		nEndPos = GetDataCount();
	}
	else
	{
		bIsPartial = TRUE;
	}


	CString strLogInfo;
	strLogInfo.Format(_T("Serial Number:,%s, Unit:, %s\n"), CStatus::Instance()->m_strSerialNo[0].GetBuffer(), CStatus::Instance()->m_strTimeDomainUnit.GetBuffer());
	csvFile.WriteString(strLogInfo);
	CString Header(CSV_HEADER_FORMAT);
	csvFile.WriteString(Header.GetBuffer());

	tAVMDData AVMDData;
	tAVMDRecord AVMDRecord;
	memset(&AVMDRecord, 0, sizeof(tAVMDRecord));
	int j = 0;
	for (int i = nStartPos; i < nEndPos; i++, j++)
	{
		GetAtData(&AVMDData, i);
		CString strData;
		if(bIsPartial)
			strData = MakeCSVRecordString(i, &AVMDData, nStartPos);
		else
			strData = MakeCSVRecordString(i, &AVMDData);
		csvFile.WriteString(strData);
		CStatus::Instance()->m_nProgressBarCount++;
		if (CStatus::Instance()->m_bIsThreadStop == TRUE)
		{
			csvFile.Close();
			return 0;
		}
	}

	csvFile.Close();
	return 0;
}

BOOL CRepository::ExportFilteredCSVFile(CString strFileName, int nInputType)
{
	CStdioFile csvFile;
	BOOL bIsPartial = FALSE;
	setlocale(LC_ALL, "Korean");
	if (!csvFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		return -1;//파일생성불가
	}

	CString strLogInfo;
	strLogInfo.Format(_T("Serial Number:,%s, Unit:, %s\n"), CStatus::Instance()->m_strSerialNo[0].GetBuffer(), CStatus::Instance()->m_strTimeDomainUnit.GetBuffer());
	csvFile.WriteString(strLogInfo);
	CString Header(CSV_HEADER_FORMAT);
	csvFile.WriteString(Header.GetBuffer());


	int j = 0;
	int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(nInputType);
	CAVMDMonitorView *pView =  CStatus::Instance()->GetAVMDView();

	for (int i = 0; i < nDataCount; i++)
	{
		CString strData;
			strData = MakeFilteredCSVRecordString(i);
		csvFile.WriteString(strData);
		CStatus::Instance()->m_nProgressBarCount++;
		if (CStatus::Instance()->m_bIsThreadStop == TRUE)
		{
			csvFile.Close();
			return 0;
		}
			
	}

	csvFile.Close();
	return 0;
}

BOOL CRepository::ExportTagFile(CString strFileName, int nStartPos, int nEndPos)
{
	CStdioFile tagFile;
	CString strMarkInfo;
	setlocale(LC_ALL, "Korean");
	if (!tagFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		return -1;//파일생성불가
	}
	CString Header(TAG_HEADER_FORMAT);
	tagFile.WriteString(Header.GetBuffer());

	if (nStartPos == -1)
	{
		if (m_WorkEnvInfo[0].size())
		{
			AppendEnvInfo(&tagFile);
		}
		if (!nStartPos)
			AddBeginMark();
		AddMachineInfo(&tagFile);

		int nCount = m_MarkInfo[0].size();
		for (int i = 0; i < nCount; i++)
		{
			tMarkInfo *pMarkInfo = m_MarkInfo[0].at(i);
			strMarkInfo.Format(_T("%s,%d,%s\n"),
				TAGNAME_MARK, pMarkInfo->iIndex, pMarkInfo->strMark.GetBuffer());
			tagFile.WriteString(strMarkInfo);
		}
	}
	else
	{
		if (m_WorkEnvInfo[0].size())
		{
			AppendEnvInfo(&tagFile);
		}
		AddMachineInfo(&tagFile);

		int nCount = m_MarkInfo[0].size();
		if (!nCount)AddBeginMark();
		int nNewCount = 0;
		for (int i = 0; i < nCount; i++)
		{
			tMarkInfo *pMarkInfo = m_MarkInfo[0].at(i);
			if ((pMarkInfo->iIndex >= nStartPos) && (pMarkInfo->iIndex <= nEndPos))
			{
				if ((nNewCount == 0) && (pMarkInfo->iIndex != nStartPos))
					AddBeginMark();
				strMarkInfo.Format(_T("%s,%d,%s\n"),
					TAGNAME_MARK, pMarkInfo->iIndex - nStartPos, pMarkInfo->strMark.GetBuffer());
				tagFile.WriteString(strMarkInfo);
				nNewCount++;
			}
		}
		if (!nNewCount)AddBeginMark();
	}


	tagFile.Close();
	return 0;
}



int CRepository::ReadFromLogFile(CFile* pLogFile, tRecordsSet * pRecordsSet, tLogType logType, int nInpuType)
{
	CString csvLine, subLine;
	bool exceptFirstLine = false;
	bool exceptInfoLine = false;
	EraseAllRecords(pRecordsSet);

	tAVMDRecord NewRecord;

	pRecordsSet->nCountOfRecords = 0;
	int nCount;
	while(sizeof(tAVMDRecord) == (nCount = pLogFile->Read(&NewRecord, sizeof(tAVMDRecord))))
	{
		tAVMDRecord *pNewRecord = new tAVMDRecord;
		memcpy(pNewRecord, &NewRecord, sizeof(tAVMDRecord));
		pRecordsSet->m_AVMDRecords.push_back(pNewRecord);
		pRecordsSet->nCountOfRecords++;
	}

	return 0;
}


int CRepository::ReadFromTagFile(CString strFileName, int nInputType)
{
	setlocale(LC_ALL, "Korean");
	CStdioFile TagFile;
	CFileException ex;
	CString strTagLine;
	CString strTagType;
	CString strSubLine;

	if (!TagFile.Open(strFileName.GetBuffer(), CStdioFile::modeRead, &ex)) {
		return -1;//Can't open file 
	}
	m_strCurTagName[nInputType] = strFileName;

	if (!TagFile.ReadString(strTagLine))return -1; //Skip Header;


	while (TagFile.ReadString(strTagLine))
	{
		if(AfxExtractSubString(strTagType, strTagLine.GetBuffer(), 0, _T(',')))
		{
			strTagType.Trim();
			strSubLine = strTagLine.Right(strTagLine.GetLength() - strTagLine.Find(',') -1);
			strSubLine.Trim();
			(strTagType == TAGNAME_WEI)		? GetWorkEnvInfo(strSubLine, nInputType):
			(strTagType == TAGNAME_MARK)	? GetMarkInfo(strSubLine, nInputType)	: 
			(strTagType == TAGNAME_MINFO)	? GetMachineInfo(strSubLine , nInputType)	:	GetExtraInfo(strSubLine);
		}

	}

	TagFile.Close();
	int iDataCount = GetDataCount();
	
	tMarkInfo *pMarkInfo;
	int nMarkCount = m_MarkInfo[nInputType].size();
	if (nMarkCount)
	{
		pMarkInfo = m_MarkInfo[nInputType].at(nMarkCount - 1);
		if (pMarkInfo->strMark != _T("End"))
		{//마지막이 마크가 "End"가 아닐경우
			if (pMarkInfo->iIndex != (iDataCount))
			{
				pMarkInfo = new tMarkInfo;
				pMarkInfo->iIndex = iDataCount;
				pMarkInfo->strMark = _T("End");
				m_MarkInfo[nInputType].push_back(pMarkInfo);
			}
		}
	}
	else
	{//마크가 아무것도 없을때 Begin과 End를 만들어 준다.
		pMarkInfo = new tMarkInfo;
		pMarkInfo->iIndex = 0;
		pMarkInfo->strMark = _T("Begin");
		m_MarkInfo[nInputType].push_back(pMarkInfo);

		pMarkInfo = new tMarkInfo;
		pMarkInfo->iIndex = iDataCount;
		pMarkInfo->strMark = _T("End");
		m_MarkInfo[nInputType].push_back(pMarkInfo);
	}
	return 0;
}

int CRepository::ReadFromLogFile(CString strFileName, int nInputType) // 
{
	CFile LogFile;
	CFileException ex;
	tLogType logType=LT_LOG;
	CString strExtName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('.') - 1);
	CString strTagFileName = strFileName.Left(strFileName.Find('.'))+ EXT_TAG;


	if ( (strExtName.MakeUpper() != _T("LOG")) )
	{
		AfxMessageBox(_T("Please enter the LOG File"), MB_OK);
		return -1;
	}

	m_iDispIndex = 0;
	if (!LogFile.Open(strFileName.GetBuffer(), CFile::modeRead, &ex)) {
		return -1;//Can't open file 
	}
	CStatus::Instance()->m_strLastOpenFileName = strFileName;
	if (nInputType == 0)
		SetCurOpenFile(strFileName);
		


	if (m_DataRecords[nInputType].m_AVMDRecords.size() != 0)
	{
		EraseAllRecords(nInputType);//!이미사용하고 있는 것이 있으면 날린다.
	}
	EraseAllTags(nInputType);
	ReadFromLogFile(&LogFile, &m_DataRecords[nInputType], logType);
	ReadFromTagFile(strTagFileName, nInputType);
	LogFile.Close();

	return 0;
}

//CRepository::tTagType CRepository::ReadTagInfo(CString strTag)
//{
//	CString subLine;
//	CStdioFile tagFile;
//	CFileException ex;
//	tTagType tagType;
//	CString strTagType;
//	tagType =	(strTagType == _T("WorkEnv"))	? TT_WORKENV :
//				(strTagType == _T("MarkInfo"))	? TT_MARK : ;
//
//	int i = 0;
//	while (AfxExtractSubString(subLine, strTag.GetBuffer(), i, _T(',')))
//	{
//		subLine.Trim();
//		switch (i)
//		{
//		case 0:
//			break;
//		}
//	}
//
//	return;
//}

//
//int CRepository::ReadTagFile(CStdioFile* pTagFile)
//{
//
//	return 0;
//}

//int CRepository::ReadTagFile(CString strFileName, int nInputType)
//{
//	CStdioFile TagFile;
//	CFileException ex;
//	CString strExtName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('.') - 1);
//
//	if ((strExtName.MakeUpper() != EXT_TAG))
//	{
//		AfxMessageBox(_T("Please enter the TAG File"), MB_OK);
//		return -1;
//	}
//
//	if (!TagFile.Open(strFileName.GetBuffer(), CFile::modeRead, &ex)) {
//		return -1;//Can't open file 
//	}
//	ReadTagFile(&TagFile);
//	TagFile.Close();
//	return 0;
//}

//
//int CRepository::GetWorkEnv(CString strTag)
//{
//	return 0;
//}


int CRepository::SaveLogFile(CString strFileName)
{
//	CFile LogFile;
//	CFileException ex;
//	tLogType logType = LT_LOG;
//	CString strExtName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('.') - 1);
//
//	if ((strExtName.MakeUpper() != _T("LOG")))
//	{
//		AfxMessageBox(_T("Please enter the LOG File"), MB_OK);
//		return -1;
//	}
//
//	m_iDispIndex = 0;
//	if (!LogFile.Open(strFileName.GetBuffer(), CFile::modeWrite | CFile::modeCreate, &ex)) {
//		return -1;//Can't open file 
//	}
////	CStatus::Instance()->strLastOpenFileName = strFileName;
//	SetCurOpenFile(strFileName);
//
//	POSITION pos;
//	tAVMDRecord *pNewRecord=NULL;
//
//	if (m_DataRecords.m_AVMDRecords.size())
//	{
////		for (pos = m_DataRecords.m_ALMDRecords.GetHeadPosition(); pos != NULL;)
//		{
////			pNewRecord = (tAVMDData *)m_DataRecords.m_ALMDRecords.GetNext(pos);
//			LogFile.Write(pNewRecord, sizeof(tAVMDRecord));
//		}
//	}
//	LogFile.Close();
	return 0;
}

void CRepository::EraseAllRecords(int nInputType)
{
	EraseAllRecords(&m_DataRecords[nInputType]);
}

void CRepository::EraseAllRecords(tRecordsSet * pRecordsSet)
{
	pRecordsSet->nCountOfRecords = 0;

	int nCount = pRecordsSet->m_AVMDRecords.size();
	for (int i = 0; i < nCount; i++)
	{
		tAVMDRecord * pAVMDRecord = pRecordsSet->m_AVMDRecords.at(0);
		pRecordsSet->m_AVMDRecords.pop_front();
		if(pAVMDRecord)	delete pAVMDRecord;
	}
}

void CRepository::NewMeasure()
{
	m_bNewMeasureState = true;
}


void CRepository::DeleteMark(int nIndex)
{
	//tAVMDData *pNewRecord;

	//pNewRecord = (tAVMDData *)m_DataRecords.m_AVMDRecords.at(nIndex);
	////pNewRecord->strMark = _T("");
}


void CRepository::UpdateMark(int nIndex, CString strMark)
{
	/*tAVMDData *pNewRecord;
	pNewRecord = (tAVMDData *)m_DataRecords.m_AVMDRecords.at(nIndex);*/
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
//	AppendLogFile(CStatus::Instance()->GetCurMeasureData());
}


void CRepository::ClearData(int nInputType)
{
	EraseAllRecords(&m_DataRecords[nInputType]);
	m_DataRecords[nInputType].nCountOfRecords = 0;
	m_iDispIndex = 0;
}

void CRepository::SetDummyData()
{
	tAVMDRecord * pNewRecord;

	for (int i = 0; i < 768; i++)
	{
		pNewRecord = new tAVMDRecord;
		memset(pNewRecord, 0, sizeof(tAVMDRecord));
		pNewRecord->byNum = i;
		m_DataRecords[0].m_AVMDRecords.push_back(pNewRecord);
	}
}

tAVMDRecord * CRepository::GetMeasureInfo(int nPos, int nInputType)
{
	return  m_DataRecords[nInputType].m_AVMDRecords.at(nPos);
}



CString CRepository::FlushCSVRecordString(int iIndexNum, tAVMDData * pAVMDData)
{
	CString strMark;
	CString strRecord;

	strMark = GetMark(iIndexNum);
	if (!strMark.IsEmpty())
	{
		wprintf(_T("\n%d, %s\n"), iIndexNum, strMark.GetBuffer());
	}
//jubatemp	printf("\nWrite IndexNo:%d", iIndexNum);
	///			gal, gforce = gal/981
	double tempRMS = 0;
	tempRMS = ((pAVMDData->iXValue * pAVMDData->iXValue)
		+ (pAVMDData->iYValue * pAVMDData->iYValue)
		+ (pAVMDData->iZValue * pAVMDData->iZValue)) / (double)3;

	pAVMDData->iRMSValue = (int)(sqrt(tempRMS));
	if (CStatus::Instance()->m_strTimeDomainUnit == _T("g-Force"))
	{
		//double dblRMS = sqrt((pow(pAVMDData->iXValue / 981, 2) + pow(pAVMDData->iYValue / 981, 2) + pow(pAVMDData->iZValue / 981, 2)) / 3);
		//strRecord.Format(CSV_DATAGFORCE_FORMAT, iIndexNum*2, pAVMDData->iXValue / 981., pAVMDData->iYValue / 981., pAVMDData->iZValue / 981., dblRMS, strMark.GetBuffer());
		if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::ONE_THOUSAND)
			strRecord.Format(CSV_DATAGFORCE_FORMAT, iIndexNum, pAVMDData->iXValue / 981., pAVMDData->iYValue / 981., pAVMDData->iZValue / 981., pAVMDData->iRMSValue / 981., strMark.GetBuffer());
		if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::Five_HUNDRED)
			strRecord.Format(CSV_DATAGFORCE_FORMAT, iIndexNum * 2, pAVMDData->iXValue / 981., pAVMDData->iYValue / 981., pAVMDData->iZValue / 981., pAVMDData->iRMSValue / 981., strMark.GetBuffer());
	}
	else
	{
		//double dblRMS = sqrt((pow(pAVMDData->iXValue, 2) + pow(pAVMDData->iYValue, 2) + pow(pAVMDData->iZValue, 2)) / 3);
		//strRecord.Format(CSV_DATAGAL_FORMAT, iIndexNum*2, pAVMDData->iXValue, pAVMDData->iYValue, pAVMDData->iZValue, dblRMS, strMark.GetBuffer());
		if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::ONE_THOUSAND)
			strRecord.Format(CSV_DATAGAL_FORMAT, iIndexNum, (double)pAVMDData->iXValue, (double)pAVMDData->iYValue, (double)pAVMDData->iZValue, (double)pAVMDData->iRMSValue, strMark.GetBuffer());
		if (CStatus::Instance()->m_DeviceFrequency == CStatus::tDevice_Frequency::Five_HUNDRED)
			strRecord.Format(CSV_DATAGAL_FORMAT, iIndexNum * 2, (double)pAVMDData->iXValue, (double)pAVMDData->iYValue, (double)pAVMDData->iZValue, (double)pAVMDData->iRMSValue, strMark.GetBuffer());
	}



	return strRecord;
}

CString CRepository::MakeCSVRecordString(int iIndexNum, tAVMDData * pAVMDData, int nOffset)
{
	CString strMark;
	CString strRecord;

	strMark = GetMark(iIndexNum);
	if (!strMark.IsEmpty())
	{
		wprintf(_T("\n%d, %s\n"), iIndexNum - nOffset, strMark.GetBuffer());
	}

	if (nOffset)
	{
		if (!(iIndexNum - nOffset))
		{
			if (strMark.IsEmpty())
			{
				strMark = _T("Begin");
			}
		}
	}
	//jubatemp	printf("\nWrite IndexNo:%d", iIndexNum);
	///			gal, gforce = gal/981
	if (CStatus::Instance()->m_strTimeDomainUnit == _T("g-Force"))
	{
		double dblRMS = sqrt((pow((double)pAVMDData->iXValue / 981., 2.) + pow((double)pAVMDData->iYValue / 981., 2.) + pow((double)pAVMDData->iZValue / 981, 2.)) / 3.);
		strRecord.Format(CSV_DATAGFORCE_FORMAT, (iIndexNum - nOffset), pAVMDData->iXValue / 981., pAVMDData->iYValue / 981., pAVMDData->iZValue / 981., dblRMS, strMark.GetBuffer());
	}
	else
	{
		double dblRMS = sqrt((pow((double)pAVMDData->iXValue, 2.) + pow((double)pAVMDData->iYValue, 2.) + pow((double)pAVMDData->iZValue, 2.)) / 3.);
		strRecord.Format(CSV_DATAGAL_FORMAT, (iIndexNum - nOffset), (double)pAVMDData->iXValue, (double)pAVMDData->iYValue, (double)pAVMDData->iZValue, dblRMS, strMark.GetBuffer());
	}

	return strRecord;
}

CString CRepository::MakeFilteredCSVRecordString(int iIndexNum)
{
	CAVMDMonitorView *pView = CStatus::Instance()->GetAVMDView();
	double dblXvalue = pView->m_pGraphXDlg->m_drawHelper.GetFilterData(iIndexNum);
	double dblYvalue = pView->m_pGraphYDlg->m_drawHelper.GetFilterData(iIndexNum);
	double dblZvalue = pView->m_pGraphZDlg->m_drawHelper.GetFilterData(iIndexNum);
	CString strRecord;
	CString strMark;
	strMark = GetMark(iIndexNum);
	if (CStatus::Instance()->m_strTimeDomainUnit == _T("g-Force"))
	{
		double dblRMS = sqrt((pow(dblXvalue / 981., 2) + pow(dblYvalue / 981., 2) + pow(dblZvalue / 981., 2)) / 3.);
		strRecord.Format(CSV_DATAGFORCE_FORMAT, iIndexNum, dblXvalue / 981., dblYvalue / 981., dblZvalue / 981., dblRMS, strMark.GetBuffer());
	}
	else
	{
		double dblRMS = sqrt((pow(dblXvalue, 2) + pow(dblYvalue, 2) + pow(dblZvalue, 2)) / 3.);
		strRecord.Format(CSV_DATAGAL_FORMAT, iIndexNum, dblXvalue, dblYvalue, dblZvalue, dblRMS, strMark.GetBuffer());
	}

	return strRecord;
}
int CRepository::GetPrepareQueueSize()
{
	return m_sizePrepareQueue;
}

int CRepository::Flush()
{
	return 0;
}

void CRepository::MakeNewFileName(int nInputType)
{
	CTime curTime = CTime::GetCurrentTime();
	CString strCurDateTime = curTime.Format(_T("%y%m%d%H%M%S"));
	CStatus::Instance()->m_strMakeDate[nInputType] = strCurDateTime;
	m_strCurLogName = CStatus::Instance()->GetDataSaveFolder() + _T("\\");
	m_strCurLogName += CStatus::Instance()->GetDataFileName();
	m_strCurLogName += strCurDateTime;
	//! 확장자를 제외한 이름을 같기 때문에 통일화시킴
	m_strCurCSVName = m_strCurLogName;
	m_strCurMarkName = m_strCurLogName;  
	m_strCurTagName[nInputType] = m_strCurLogName;

	m_strCurLogName		+= EXT_LOG;
	m_strCurCSVName		+= EXT_CSV;
	m_strCurTagName[nInputType] += EXT_TAG;
}


int CRepository::MakeTagInfoFile()
{
	return 0;
}

int CRepository::BeginSave(int iStartIndex, int nCount)
{
	printf("\nBeginSave %d, %d", iStartIndex, nCount);

	switch (m_FileSaveStatus)
	{
	case SS_STOP:
	{
		m_FileSaveStatus = SS_WRITE;
		m_iLastWriteIndex = iStartIndex;
		m_iTargetCount = nCount;
//		m_iTargetIndex = nCount + m_iLastWriteIndex;
		m_bIsSaveTaretIndex = FALSE;
		ResetMarkInfo();
		MakeNewFileName();
		MakeFileThread();
		printf("\nBeginSave SS_STOP_END");
	}
	break;
	case SS_PAUSE:
		m_FileSaveStatus = SS_WRITE;
		SetEvent(m_hRecStart);
		break;
	case SS_WRITE:
		break;
	}
	m_iLastWriteTime = timeGetTime();
	return 0;
}

int CRepository::PauseSave()
{
	ResetEvent(m_hRecStart);
	m_FileSaveStatus = SS_PAUSE;
	m_iPausePos = GetDataCount();
	return 0;
}

int CRepository::ReStartSave()
{
	m_iLastWriteIndex = GetDataCount();
	SetEvent(m_hRecStart);
	m_FileSaveStatus = SS_WRITE;
	return 0;
}

int CRepository::StopSave()
{
	SetEvent(m_hRecStart);
	m_FileSaveStatus = SS_STOP;
	CStatus::Instance()->m_bSavePreRecData = FALSE;
	return 0;
}

CRepository::tSaveStatus CRepository::GetFileSaveStatus()
{
	return m_FileSaveStatus;
}


UINT CRepository::WriteFileThread(LPVOID pSender)
{
	CRepository *pRepository = (CRepository *)pSender;

	while (!pRepository->m_bTerminateThread)
	{
		WaitForSingleObject(pRepository->m_hRecStart, INFINITE);
		if(pRepository->m_FileSaveStatus == SS_WRITE)
			pRepository->FlushData();
	}

	return 0;
}


int CRepository::MakeFileThread()
{
	m_bTerminateThread = FALSE;
	m_hRecStart = CreateEvent(0, TRUE, FALSE, 0);
	SetEvent(m_hRecStart);
	m_pWFileWriteThread = AfxBeginThread(WriteFileThread, this);
	if (!m_pWFileWriteThread)
		return 0;
	return -1;
}

DWORD WaitThreadEndHandle(HANDLE hThread, int nWaitMilSec)
{
	if (!hThread) return -1;

	DWORD dwExit = 0;
	DWORD dwRetCode = WaitForSingleObject(hThread, nWaitMilSec);
	if (dwRetCode == WAIT_OBJECT_0) // 정상종료
	{
		return -2;
	}
	else if (dwRetCode == WAIT_TIMEOUT) // 타임아웃
	{
		return -3;
	}

	return 0;
}

int CRepository::WaitForFileSaveThreadEnd()
{
	if (!m_pWFileWriteThread)return -1;
	SetEvent(m_hRecStart);
	printf("\nWaitForFileSaveThreadEnd 1");
	DWORD dwRetCode = WaitThreadEndHandle(m_pWFileWriteThread->m_hThread, 3000);
	printf("\nWaitForFileSaveThreadEnd 2");
	return 0;
}


int CRepository::TerminateFileSaveThread()
{
	m_bTerminateThread = TRUE;
	WaitForFileSaveThreadEnd();
	return 0;
}


BOOL CRepository::IsFileSaveDone(int nInputType)
{
	if (m_iLastWriteIndex == -1)return FALSE;
	if ((m_iLastWriteIndex + 1) >= (int)(m_DataRecords[nInputType].m_AVMDRecords.size()))return TRUE;
	return FALSE;
}


int  CRepository::GetNextData(tAVMDData *pAVMDData, int idxAVMDData, int nInputType)
{
	int rv = 0;
	if (idxAVMDData == -1)
	{
		rv = GetAtData(pAVMDData, ++m_CurDataPosition[nInputType], nInputType);
		if (rv == -1)m_CurDataPosition[nInputType]--;
		return rv;
	}
	else
	{
		rv = GetAtData(pAVMDData, ++idxAVMDData, nInputType);
		return rv;
	}
}


int CRepository::GetPrevData(tAVMDData *pAVMDData, int idxAVMDData, int nInputType)
{
	int rv = 0;
	int MoveTo = 0;
	if (idxAVMDData == -1)
	{
		if (!m_CurDataPosition[nInputType]) return -1;
		rv = GetAtData(pAVMDData, --m_CurDataPosition[nInputType], nInputType);
		return rv;
	}
	else
	{
		if(idxAVMDData)
			--idxAVMDData;
		return GetAtData(pAVMDData, MoveTo, nInputType);
	}
}

int CRepository::GetAtData(tAVMDData *pAVMDData, int idxAVMDData, int nInputType)
{

	UINT RecordPos = idxAVMDData / VALUESET_COUNT;
	int PosInRecord = idxAVMDData % VALUESET_COUNT;
	tAVMDRecord *pAVMDRecord = NULL;
	m_mtxLocker.lock();
	if (RecordPos < m_DataRecords[nInputType].m_AVMDRecords.size())
	{
		pAVMDRecord = m_DataRecords[nInputType].m_AVMDRecords.at(RecordPos);
	}
	m_mtxLocker.unlock();
	if (!pAVMDRecord)
	{
		return -1;
	}
	m_CurDataPosition[nInputType] = idxAVMDData;

	pAVMDData->byTemperature = pAVMDRecord->byTemperature;
	pAVMDData->byBattery = pAVMDRecord->byBattery;
	pAVMDData->timeCurTime = pAVMDRecord->timeCurTime;
	pAVMDData->accInfo = pAVMDRecord->accInfo[PosInRecord];
	CAVMDPacket::GetXYZ(pAVMDData->iXValue, pAVMDData->iYValue, pAVMDData->iZValue, (BYTE *)(&(pAVMDRecord->accInfo[PosInRecord])));
	//jubatemp
	return 0;
}



int CRepository::GetLastData(tAVMDData *pLastData, int nInputType)
{
	int RecordPos = m_DataRecords[nInputType].m_AVMDRecords.size();
	m_CurDataPosition[nInputType] = RecordPos*VALUESET_COUNT;

	tAVMDRecord *pAVMDRecord = m_DataRecords[nInputType].m_AVMDRecords.at(RecordPos-1);
	pLastData->byTemperature = pAVMDRecord->byTemperature;
	pLastData->byBattery = pAVMDRecord->byBattery;
	CAVMDPacket::GetXYZ(pLastData->iXValue, pLastData->iYValue, pLastData->iZValue, (BYTE *)(&(pAVMDRecord->accInfo[VALUESET_COUNT - 1])));

	return 0;
}

int CRepository::GetDataCount(int nInputType)
{
	int RecordPos = m_DataRecords[nInputType].m_AVMDRecords.size();
	return RecordPos*VALUESET_COUNT;
}

BOOL CRepository::IsSaveTargetIndex()
{
	return m_bIsSaveTaretIndex;
}


int CRepository::GetLastIndex(int nInputType)
{
	return GetDataCount(nInputType);
}


int CRepository::SetWorkEnv(int iIndex)
{
	return 0;
}


int CRepository::AppendMarkInfo(CStdioFile * pTagFile, int nInputType)
{
	CString strMarkInfo;
	int nCount = m_MarkInfo[nInputType].size();
	if (!nCount)return -1;

	for (;nCount > m_nToWriteMarkNo; m_nToWriteMarkNo++)
	{
		tMarkInfo *pMarkInfo = m_MarkInfo[nInputType].at(m_nToWriteMarkNo);
		strMarkInfo.Format(_T("%s,%d,%s\n"),
			TAGNAME_MARK, pMarkInfo->iIndex, pMarkInfo->strMark.GetBuffer());
		pTagFile->WriteString(strMarkInfo);
	}
	return 0;
}

int CRepository::AddPreRecordMark()
{
	CString strBeginEvent;
	tMarkInfo *pMarkInfo = new tMarkInfo;
	pMarkInfo->iIndex = 0;
	pMarkInfo->strMark = _T("PreRecord");
	m_MarkInfo[0].push_back(pMarkInfo);

	return 0;
}

int CRepository::AddBeginMark(int nDataPos)
{
	CString strMarkInfo;
	CString strBeginEvent;

	int iEventMode = CStatus::Instance()->GetEventMode();
	if (CStatus::Instance()->m_bSavePreRecData)iEventMode = CStatus::Evt_NONE;
	switch (iEventMode)
	{
	case CStatus::Evt_NONE:
		strBeginEvent = _T("Begin");
		break;
	case CStatus::Evt_GONOGO:
		strBeginEvent = _T("Begin(Go) : ") + CStatus::Instance()->m_strEventText;
		break;
	case CStatus::Evt_TRIGGER:
		strBeginEvent = _T("Begin(Trigger) : ") + CStatus::Instance()->m_strEventText;
		break;
	}
	AddMark(nDataPos, strBeginEvent.GetBuffer());

	return 0;
}

int CRepository::AddMachineInfo(CStdioFile * pTagFile, int nInputType)
{
	CString strMchInfo;
	strMchInfo.Format(_T("%s,%s,%s\n"), TAGNAME_MINFO, CStatus::Instance()->m_strMakeDate[nInputType], CStatus::Instance()->m_strSerialNo[nInputType].GetBuffer());
	pTagFile->WriteString(strMchInfo);
	return 0;
}

int CRepository::AppendEnvInfo(CStdioFile * pTagFile, int nInputType)
{
	CString strWorkEnvInfo;
	tWorkEnvInfo *pWorkEnvInfo = m_WorkEnvInfo[nInputType].at(0);
	strWorkEnvInfo.Format( _T("%s,%s,%s,%s,%s\n"), 
		TAGNAME_WEI, pWorkEnvInfo->szOperator, pWorkEnvInfo->szTool, pWorkEnvInfo->szStation, pWorkEnvInfo->szComment);

	pTagFile->WriteString(strWorkEnvInfo);

	return 0;
}

void CRepository::MakeDefaultWorkEnvInfo(int nInputType)
{
	CString str;
	tWorkEnvInfo *pWorkEnvInfo;
	if (m_WorkEnvInfo[nInputType].size())
	{
		pWorkEnvInfo = m_WorkEnvInfo[nInputType].at(0);
	}
	else
	{
		pWorkEnvInfo = new tWorkEnvInfo;
	}
	memset(pWorkEnvInfo, 0, sizeof(tWorkEnvInfo));
	//wsprintf(WorkEnvInfo.szOperator, TEXT("Operator"));
	//wsprintf(WorkEnvInfo.szStation, TEXT("A"));
	//wsprintf(WorkEnvInfo.szTool, TEXT("Seso"));
	//StringCbPrintf(WorkEnvInfo.szComment, 500, TEXT("Seso 테스트"));
}



void CRepository::GetWorkEnvInfo(CString strSubLine, int nInputType)
{
	CString strParam;
	int i = 0;
	tWorkEnvInfo *pWorkEnvInfo = new tWorkEnvInfo;

	while (AfxExtractSubString(strParam, strSubLine.GetBuffer(), i, _T(',')))
	{
		strParam.Trim();
		switch (i)
		{
		case 0://Operator
			wsprintf(pWorkEnvInfo->szOperator, strParam.GetBuffer());
			break;
		case 1://Tool
			wsprintf(pWorkEnvInfo->szTool, strParam.GetBuffer());
			break;
		case 2://Station
			wsprintf(pWorkEnvInfo->szStation, strParam.GetBuffer());
			break;
		case 3://Comment
			wsprintf(pWorkEnvInfo->szComment, strParam.GetBuffer());
			break;
		}
		i++;
	}
	
	m_WorkEnvInfo[nInputType].push_back(pWorkEnvInfo);
}


void CRepository::GetMarkInfo(CString strSubLine, int nInputType)
{
	CString strParam;
	tMarkInfo *pMarkInfo = new tMarkInfo;

	int i = 0;
	while (AfxExtractSubString(strParam, strSubLine.GetBuffer(), i, _T(',')))
	{
		strParam.Trim();
		switch (i)
		{
		case 0://Operator
			pMarkInfo->iIndex = _wtol(strParam.GetBuffer());
			break;
		case 1://Mark
			pMarkInfo->strMark = strParam;
			break;
		}
		i++;
	}

	m_MarkInfo[nInputType].push_back(pMarkInfo);
}

void CRepository::GetMachineInfo(CString strSubLine, int nInputType)
{
	CString strParam;
	tMarkInfo *pMarkInfo = new tMarkInfo;

	int i = 0;
	while (AfxExtractSubString(strParam, strSubLine.GetBuffer(), i, _T(',')))
	{
		strParam.Trim();
		switch (i)
		{
		case 0://Date
			CStatus::Instance()->m_strMakeDate[nInputType] = strParam;
			break;
		case 1://Sensor S/N
			CStatus::Instance()->m_strSerialNo[nInputType] = strParam;
			break;
		}
		i++;
	}
}


void CRepository::GetExtraInfo(CString strSubLine)
{
}


void CRepository::ResetMarkInfo(int nInputType)
{
	int nCount = m_MarkInfo[nInputType].size();
	if (!nCount)return;
	tMarkInfo *pMarkInfo;
	for (int i = 0; i < nCount; i++)
	{
		pMarkInfo = m_MarkInfo[nInputType].at(0);
		delete pMarkInfo;
		m_MarkInfo[nInputType].pop_front();
	}
	m_nToWriteMarkNo = 0;
}

int CRepository::ReSaveTagFile(int nInputType)
{
	setlocale(LC_ALL, "Korean");
	CStdioFile TagFile;

	if (!TagFile.Open(m_strCurTagName[nInputType], CFile::modeCreate | CFile::modeWrite))
	{
		return -1;//파일생성불가
	}
	CString Header(TAG_HEADER_FORMAT);
	TagFile.WriteString(Header.GetBuffer());
	if (m_WorkEnvInfo[nInputType].size())
	{
		AppendEnvInfo(&TagFile);
	}
	AddMachineInfo(&TagFile);


	if (m_MarkInfo[nInputType].size())
	{
		CString strMarkInfo;
		int nCount = m_MarkInfo[nInputType].size();
		if (!nCount)return -1;

		for (int i = 0; nCount > i; i++)
		{
			tMarkInfo *pMarkInfo = m_MarkInfo[nInputType].at(i);
			strMarkInfo.Format(_T("%s,%d,%s\n"),
				TAGNAME_MARK, pMarkInfo->iIndex, pMarkInfo->strMark.GetBuffer());
			TagFile.WriteString(strMarkInfo);
		}
	}

	TagFile.Close();

	return 0;
}

void CRepository::EraseAllTags(int nInputType)
{
	tWorkEnvInfo *pWorkEnvInfo;
	int nCount = m_WorkEnvInfo[nInputType].size();
	if (nCount)
	{
		for (int i = 0; i < nCount; i++)
		{
			pWorkEnvInfo = m_WorkEnvInfo[nInputType].at(0);
			delete pWorkEnvInfo;
			m_WorkEnvInfo[nInputType].pop_front();
		}
	}


	tMarkInfo *pMarkInfo;
	nCount = m_MarkInfo[nInputType].size();
	if (nCount)
	{
		for (int i = 0; i < nCount; i++)
		{
			pMarkInfo = m_MarkInfo[nInputType].at(0);
			delete pMarkInfo;
			m_MarkInfo[nInputType].pop_front();
		}
	}
}




CString CRepository::GetMark(int iIndexNum)
{
	tMarkInfo *pMarkInfo;
	int nCount;
	nCount = m_MarkInfo[0].size();
	if (nCount)
	{
		for (int i = 0; i < nCount; i++)
		{
			pMarkInfo = m_MarkInfo[0].at(i);
			if (pMarkInfo->iIndex == iIndexNum)
			{
				return pMarkInfo->strMark;
			}
		}
	}

	return CString();
}


void CRepository::AddMark(int iIndex, CString strMark)
{
	tMarkInfo *pMarkInfo = new tMarkInfo;
	pMarkInfo->iIndex = iIndex;
	pMarkInfo->strMark = strMark;
	m_MarkInfo[0].push_back(pMarkInfo);
}
