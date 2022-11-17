#include "stdafx.h"
#include "Repository.h"
#include "Status.h"

#include "MainFrm.h"
CRepository::CRepository()
	: m_strCurLogName(_T(""))
	, m_bNewMeasureState(false)
	, m_strCurCSVName(_T(""))
{
	m_DataRecords[0].nCountOfRecords = 0;
	m_DataRecords[1].nCountOfRecords = 0;
}


CRepository::~CRepository()
{
}

int CRepository::TestFileWrite()
{
	tAPMDRecord APMDRecord;
	tAPMDRecord *pNewRecord=&APMDRecord;
	CString strTest;
	for (int j = 0; j < 36 * 4; j++)
	{
		strTest.Format(_T("Mark j:%d"), j);
		for (int i = 0; i < 100; i++)
		{
			pNewRecord->Index = i + j * 100; pNewRecord->TimeInfo = i; pNewRecord->Particle01 = i; pNewRecord->Particle03 = 100 - i; pNewRecord->Particle05 = i * 2;
			pNewRecord->Temperature = 2500 + i * 5; pNewRecord->Pressure = 0; pNewRecord->Battery = 100 - i / 10; pNewRecord->Humidity = 0; pNewRecord->strMark = strTest;
			AppendLogFile(&APMDRecord);
			if (!strTest.IsEmpty())strTest = _T("");
		}
	}

	return 0;
}


int CRepository::AppendLogFileCore(CStdioFile * pStdioFile, tAPMDRecord * pNewRecord)
{
	if (!(PathFileExists(m_strCurLogName.GetBuffer())) | m_bNewMeasureState) {
		if (!pStdioFile->Open(m_strCurLogName, CFile::modeCreate | CFile::modeWrite))
		{
			return -1;//파일생성불가
		}
		CString strLogInfo;
		CString strDate;
		CTime cTime = CTime::GetCurrentTime(); // 현재 시스템으로부터 날짜 및 시간을 얻어 온다.
		strDate.Format(_T("%04d-%02d-%02d"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay()); // 현재 일 반환
		CStatus::Instance()->m_strMakeDate = strDate;
		strLogInfo.Format(_T("DATE:,%s,SerialNo:,%s\n"),
			strDate.GetBuffer(), CStatus::Instance()->m_strSerialNo.GetBuffer());
		pStdioFile->WriteString(strLogInfo.GetBuffer());

		CString Header(_T("Index,TimeInfo,Particle01,Particle03,Particle05,Temp,Pressure,Battery,Humidity,Mark\n"));
		CString newRecord;
		pStdioFile->WriteString(Header.GetBuffer());
	}
	else
	{
		if (!pStdioFile->Open(m_strCurLogName, CFile::modeWrite))
		{
			return -1;//파일생성불가
		}
		pStdioFile->SeekToEnd();
	}

	CString newRecord;
	newRecord.Format(_T("%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n"), pNewRecord->Index, pNewRecord->TimeInfo, pNewRecord->Particle01, pNewRecord->Particle03, pNewRecord->Particle05, pNewRecord->Temperature, pNewRecord->Pressure, pNewRecord->Battery, pNewRecord->Humidity, pNewRecord->strMark.GetBuffer());
	pStdioFile->WriteString(newRecord);

	pStdioFile->Close();
	return 0;
}

int CRepository::AppendCSVFileCore(CStdioFile * pStdioFile, tAPMDRecord * pNewRecord)
{
	if (!(PathFileExists(m_strCurCSVName.GetBuffer())) | m_bNewMeasureState) {
		if (!pStdioFile->Open(m_strCurCSVName, CFile::modeCreate | CFile::modeWrite))
		{
			return -1;//파일생성불가
		}
		CString strLogInfo;
		CString strDate;
		CTime cTime = CTime::GetCurrentTime(); // 현재 시스템으로부터 날짜 및 시간을 얻어 온다.
		strDate.Format(_T("%04d-%02d-%02d"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay()); // 현재 일 반환
		strLogInfo.Format(_T("DATE:,%s,SerialNo:,%s\n"),
			strDate.GetBuffer(), CStatus::Instance()->m_strSerialNo.GetBuffer());
		pStdioFile->WriteString(strLogInfo.GetBuffer());

		CString Header(_T("Index,TimeInfo,Particle 0.1um,Particle 0.3um,Particle 0.5um,Temp,Pressure,Battery,Humidity,Mark\n"));
		CString newRecord;
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

	CString newRecord;
	double dblTemperature, dblPressure;
	dblTemperature = pNewRecord->Temperature / 100.;
	dblPressure = pNewRecord->Pressure / 10.;
	newRecord.Format(_T("%d,%d,%d,%d,%d,%.2f,%.1f,%d,%d,%s\n"), pNewRecord->Index, pNewRecord->TimeInfo, pNewRecord->Particle01, pNewRecord->Particle03, pNewRecord->Particle05, dblTemperature, dblPressure, pNewRecord->Battery, pNewRecord->Humidity, pNewRecord->strMark.GetBuffer());
	pStdioFile->WriteString(newRecord);

	pStdioFile->Close();
	return 0;
}

/*! 파일이 없으면 만들고 추가, 없으면 추가*/
int CRepository::AppendLogFile(tAPMDRecord *pNewRecord)
{
	CStdioFile LogFile;
	CStdioFile CSVFile;

	CString strLogFile = CStatus::Instance()->GetDataSaveFolder() + _T("\\");
	strLogFile += CStatus::Instance()->GetDataFileName();
	m_strCurLogName = strLogFile + _T(".LOG");
	m_strCurCSVName = strLogFile + _T(".CSV");

	AppendLogFileCore(&LogFile, pNewRecord);
	AppendCSVFileCore(&CSVFile, pNewRecord);

	if(m_bNewMeasureState)	m_bNewMeasureState = false;

	return 0;
}

void CRepository::ExportCSVFile(CString strFileName)
{
	CStdioFile csvFile;
	CFileException ex;
	if (!csvFile.Open(strFileName.GetBuffer(), CFile::modeWrite|CFile::modeCreate, &ex)) {
		return;//Can't open file 
	}
	CString strLogInfo;
	
	strLogInfo.Format(_T("DATE:,%s,SerialNo:,%s\n"),
		CStatus::Instance()->m_strMakeDate.GetBuffer(), CStatus::Instance()->m_strSerialNo.GetBuffer());
	csvFile.WriteString(strLogInfo.GetBuffer());
	CString Header(_T("Index,TimeInfo,Particle 0.1um,Particle 0.3um,Particle 0.5um,Temp,Pressure,Battery,Humidity,Mark\n"));
	CString newRecord;
	csvFile.WriteString(Header.GetBuffer());


	POSITION pos;
	tAPMDRecord *pNewRecord;
	float fTemperature, fPressure;
	for (pos = m_DataRecords[0].m_APMDRecords.GetHeadPosition(); pos != NULL;)
	{
		pNewRecord = (tAPMDRecord *)m_DataRecords[0].m_APMDRecords.GetNext(pos);
		fTemperature = (float)(pNewRecord->Temperature / 100.);
		fPressure = (float)(pNewRecord->Pressure / 10.);
		newRecord.Format(_T("%d,%d,%d,%d,%d,%.2f,%.1f,%d,%d,%s\n"), pNewRecord->Index, pNewRecord->TimeInfo, pNewRecord->Particle01, pNewRecord->Particle03, pNewRecord->Particle05, fTemperature, fPressure, pNewRecord->Battery, pNewRecord->Humidity, pNewRecord->strMark.GetBuffer());
		csvFile.WriteString(newRecord);
	}
	csvFile.Close();
}

int CRepository::ReadFromLogFile(CStdioFile* pLogFile, tRecordsSet * pRecordsSet, tLogType logType)
{
	CString csvLine, subLine;
	bool exceptFirstLine = false;
	bool exceptInfoLine = false;
	EraseAllRecords(pRecordsSet);

	tAPMDRecord *pNewRecord=NULL;
	while (pLogFile->ReadString(csvLine))
	{
		if (!exceptInfoLine)
		{
			GetLogInfo(csvLine, CStatus::Instance()->m_strMakeDate, CStatus::Instance()->m_strSerialNo);
			exceptInfoLine = true;
			pLogFile->ReadString(csvLine);//Skip Title Header
			continue;
		}

		csvLine.Trim();
		
		pNewRecord = new tAPMDRecord;
		pNewRecord->Index = 0; pNewRecord->TimeInfo = 0; pNewRecord->Particle01 = 0; pNewRecord->Particle03 = 0; pNewRecord->Particle05 = 0;
		pNewRecord->Temperature = 0; pNewRecord->Pressure = 0; pNewRecord->Battery = 0; pNewRecord->Humidity = 0; pNewRecord->strMark = _T("");

		int i = 0;
		while (AfxExtractSubString(subLine, csvLine.GetBuffer(), i, _T(',')))
		{
			subLine.Trim();
			switch (i)
			{
			case 0://Index
				pNewRecord->Index = _wtol(subLine.GetBuffer());
				break;
			case 1://TimeInfo
				pNewRecord->TimeInfo = _wtol(subLine.GetBuffer());
				break;
			case 2://Particle01
				pNewRecord->Particle01 = _wtol(subLine.GetBuffer());
				break;
			case 3://Particle03
				pNewRecord->Particle03 = _wtol(subLine.GetBuffer());
				break;
			case 4://Particle05
				pNewRecord->Particle05 = _wtol(subLine.GetBuffer());
				break;
			case 5://Temperature
				if(logType == LT_LOG)
					pNewRecord->Temperature = _wtol(subLine.GetBuffer());
				else
					pNewRecord->Temperature = (int)(100*_wtof(subLine.GetBuffer()));
				break;
			case 6://Pressure
				if (logType == LT_LOG)
					pNewRecord->Pressure = _wtol(subLine.GetBuffer());
				else
					pNewRecord->Pressure = (int)(10*_wtof(subLine.GetBuffer()));
				break;
			case 7://Battery
				pNewRecord->Battery = _wtol(subLine.GetBuffer());
				break;
			case 8://Humidity
				pNewRecord->Humidity = _wtol(subLine.GetBuffer());
				break;
			case 9://Mark
				pNewRecord->strMark = subLine;
				break;
			}
			i++;
			if (i == 10)
			{
				pRecordsSet->m_APMDRecords.AddTail(pNewRecord);
				pRecordsSet->nCountOfRecords++;
			}

		}
	}
	if(pNewRecord)pNewRecord->strMark = MARK_END;
	return 0;
}

int CRepository::ReadFromLogFile(CString strFileName)
{
	CStdioFile LogFile;
	CFileException ex;
	tLogType logType=LT_LOG;
	CString strExtName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('.') - 1);

	if ( (strExtName.MakeUpper() != _T("LOG")) && (strExtName.MakeUpper() != _T("CSV")) )
	{
		AfxMessageBox(_T("Please enter the LOG File"), MB_OK);
		return -1;
	}
	if ( (strExtName.MakeUpper() == _T("LOG")) )
		logType = LT_LOG;
	else
		logType = LT_CSV;

	if (!LogFile.Open(strFileName.GetBuffer(), CFile::modeRead, &ex)) {
		return -1;//Can't open file 
	}
	CStatus::Instance()->strLastOpenFileName = strFileName;
	SetCurOpenFile(strFileName);
	if (m_DataRecords[0].m_APMDRecords.GetCount() != 0)EraseAllRecords();//!이미사용하고 있는 것이 있으면 날린다.
	ReadFromLogFile(&LogFile, &m_DataRecords[0], logType);
	LogFile.Close();
	MakeMarkInfos();
	Update();
	return 0;
}

int CRepository::MarkNameSet(int Index, CString strName)
{
	return 0;
}

//전체가 한꺼번에 업데이트됨
int CRepository::Update()
{
	return 0;
}

//하나씩 업데이트됨
int CRepository::UpdateRecord()
{
	return 0;
}

int CRepository::Register()
{
	return 0;
}


int CRepository::AppendData(tAPMDRecord * pNewRecord)
{
	m_DataRecords[0].m_APMDRecords.AddTail(pNewRecord);

	tMarkInfo *pMarkInfo;
	pMarkInfo = new tMarkInfo;
	pNewRecord->Index = m_DataRecords[0].nCountOfRecords;
	if (!pNewRecord->strMark.IsEmpty())
	{
		pMarkInfo->nIndex = m_DataRecords[0].nCountOfRecords;
		pMarkInfo->strMarkName = pNewRecord->strMark;
	}

	AppendLogFile(pNewRecord);
	UpdateRecord();
	m_DataRecords[0].nCountOfRecords++;
	return 0;
}

int CRepository::CalculateSum(int &iAccum01, int &iAccum03, int &iAccum05)
{
	POSITION pos;
	tAPMDRecord *pNewRecord;
	iAccum01=0, iAccum03=0, iAccum05=0;

	for (pos = m_DataRecords[0].m_APMDRecords.GetHeadPosition(); pos != NULL;)
	{
		pNewRecord = (tAPMDRecord *)m_DataRecords[0].m_APMDRecords.GetNext(pos);
		iAccum01 += pNewRecord->Particle01;
		iAccum03 += pNewRecord->Particle03;
		iAccum05 += pNewRecord->Particle05;
	}
	return 0;
}


int CRepository::CalculateSum(int nIndex, int & iAccum01, int & iAccum03, int & iAccum05, int KindOfData)
{
	POSITION pos;
	tAPMDRecord *pNewRecord;
	iAccum01 = 0, iAccum03 = 0, iAccum05 = 0;

	int nCount = 0;

	for (pos = m_DataRecords[KindOfData].m_APMDRecords.GetHeadPosition(); (pos != NULL)&&(nIndex>=nCount);)
	{
		pNewRecord = (tAPMDRecord *)m_DataRecords[0].m_APMDRecords.GetNext(pos);
		nCount++;
		iAccum01 += pNewRecord->Particle01;
		iAccum03 += pNewRecord->Particle03;
		iAccum05 += pNewRecord->Particle05;
	}
	return 0;
}



int CRepository::CheckWarning(bool & bUseWarning, bool &bIsError, CString & strErrMsg)
{
	bUseWarning = 0 != CStatus::Instance()->m_iUseWarning;

	POSITION pos;
	tAPMDRecord *pRecord=NULL;
	int iAccum01 = 0, iAccum03 = 0, iAccum05 = 0;
	double recTemp = 0.0;
	int nCount = 0;
	int recCount01 = 0, recCount03 = 0, recCount05 = 0;
	
	if ((nCount=m_DataRecords[0].m_APMDRecords.GetCount()) == 0) return -2;

	//	strErrMsg
	if(CStatus::Instance()->m_iTimeWarningSel == 0)
	{//! Second
		pRecord = (tAPMDRecord *)m_DataRecords[0].m_APMDRecords.GetTail();
		recTemp = pRecord->Temperature / 100.0;
		iAccum01 = pRecord->Particle01;
		iAccum03 = pRecord->Particle03;
		iAccum05 = pRecord->Particle05;
	}else
	{
	//! Minute
		int sRecNo = 0;
		if (nCount >= 60) sRecNo = nCount - 60;else sRecNo = 0;
	
		for (pos = m_DataRecords[0].m_APMDRecords.FindIndex(sRecNo); pos != NULL;)
		{
			pRecord = (tAPMDRecord *)m_DataRecords[0].m_APMDRecords.GetNext(pos);
			iAccum01 += pRecord->Particle01;
			iAccum03 += pRecord->Particle03;
			iAccum05 += pRecord->Particle05;
		}
		if(pRecord)
			recTemp = pRecord->Temperature / 100.0;
	}

	if (CStatus::Instance()->m_iUseP01)
	{
		CString str;
		if (CStatus::Instance()->m_iThresP01 <= iAccum01)
		{
			str.Format(_T("Particle Error 0.1um Count:%d\r\n"), iAccum01);
			strErrMsg += str;
			bIsError = true;
		}
	}

	if (CStatus::Instance()->m_iUseP03)
	{
		if (CStatus::Instance()->m_iThresP03 <= iAccum03)
		{
			CString str;
			str.Format(_T("Particle 0.3um Count:%d\r\n"), iAccum03);
			strErrMsg += str;
			bIsError = true;
		}
	}

	if (CStatus::Instance()->m_iUseP05)
	{
		if (CStatus::Instance()->m_iThresP05 <= iAccum05)
		{
			CString str;
			str.Format(_T("Particle 0.5um Count:%d\r\n"), iAccum05);
			strErrMsg += str;
			bIsError = true;
		}
	}

	if (CStatus::Instance()->m_iUseTemp)
	{
		if (CStatus::Instance()->m_dblThresTemp <= recTemp)
		{
			CString str;
			str.Format(_T("Temp:%.2f℃\r\n"), (float)recTemp);
			strErrMsg += str;
			bIsError = true;
		}
	}

	if(!bIsError)
	{
		CString str;
		str.Format(_T("Particle 0.1um:%d, 0.3um:%d, 0.5um:%d\r\nTemp %.2f℃"), iAccum01, iAccum03, iAccum05, (float)recTemp);
		strErrMsg = str;
	}
	return 0;
}


void CRepository::EraseAllTags(CPtrList * pLstMarkInfo)
{
	POSITION pos;
	tMarkInfo *pMarkInfo;
	if (pLstMarkInfo->GetCount())
	{
		for (pos = pLstMarkInfo->GetHeadPosition(); pos != NULL; )
		{
			pMarkInfo = (tMarkInfo *)pLstMarkInfo->GetNext(pos);
			delete pMarkInfo;
		}
		pLstMarkInfo->RemoveAll();
	}
}


void CRepository::EraseAllRecords()
{
	EraseAllRecords(&m_DataRecords[0]);
}

void CRepository::EraseAllRecords(tRecordsSet * pRecordsSet)
{
	EraseAllRecords(&(pRecordsSet->m_APMDRecords));
	EraseAllTags(&(pRecordsSet->lstMarkInfos));
	pRecordsSet->nCountOfRecords = 0;
}

void CRepository::EraseAllRecords(CPtrList *pRecords)
{
	POSITION pos;
	tAPMDRecord *pNewRecord;

	if (pRecords->GetCount())
	{
		for (pos = pRecords->GetHeadPosition(); pos != NULL;)
		{
			pNewRecord = (tAPMDRecord *)pRecords->GetNext(pos);
			delete pNewRecord;
		}
		pRecords->RemoveAll();
	}
}


//Mark Browser용 자료를 작성
int CRepository::MakeMarkInfos(int nKindOfData)
{
	tMarkInfo *pMarkInfo = NULL;

	POSITION pos;

	tAPMDRecord *pNewRecord;

	for (pos = m_DataRecords[nKindOfData].lstMarkInfos.GetHeadPosition(); pos != NULL; )
	{
		pMarkInfo = (tMarkInfo *)m_DataRecords[nKindOfData].lstMarkInfos.GetNext(pos);
		delete pMarkInfo;
	}
	
	m_DataRecords[nKindOfData].lstMarkInfos.RemoveAll();

	int nIndex = 0;
	for (pos = m_DataRecords[nKindOfData].m_APMDRecords.GetHeadPosition(); pos != NULL;nIndex++)
	{
		pNewRecord = (tAPMDRecord *)m_DataRecords[nKindOfData].m_APMDRecords.GetNext(pos);
		if (pNewRecord->strMark.IsEmpty())continue;
		pMarkInfo = new tMarkInfo;
		pMarkInfo->nIndex = nIndex;
		pMarkInfo->strMarkName = pNewRecord->strMark;
		m_DataRecords[nKindOfData].lstMarkInfos.AddTail(pMarkInfo);
	}

	return 0;
}


void CRepository::NewMeasure()
{
	m_bNewMeasureState = true;
}


void CRepository::DeleteMark(int nIndex)
{
	tAPMDRecord *pNewRecord;
	POSITION pos;
	pos = m_DataRecords[0].m_APMDRecords.FindIndex(nIndex);
	if (!pos)return;
	pNewRecord = (tAPMDRecord *)m_DataRecords[0].m_APMDRecords.GetAt(pos);
	pNewRecord->strMark = _T("");
	MakeMarkInfos();
}


void CRepository::UpdateMark(int nIndex, CString strMark)
{
	tAPMDRecord *pNewRecord;
	POSITION pos;
	pos = m_DataRecords[0].m_APMDRecords.FindIndex(nIndex);
	pNewRecord = (tAPMDRecord *)m_DataRecords[0].m_APMDRecords.GetAt(pos);
	pNewRecord->strMark = strMark;
	MakeMarkInfos();
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



int CRepository::CalcParticleCount(int &n01Count, int &n03Count, int &n05Count)
{
	POSITION pos;
	tAPMDRecord *pNewRecord;
	n01Count = 0;
	n03Count = 0;
	n05Count = 0;

	if (m_DataRecords[0].m_APMDRecords.GetCount())
	{
		for (pos = m_DataRecords[0].m_APMDRecords.GetHeadPosition(); pos != NULL;)
		{
			pNewRecord = (tAPMDRecord *)m_DataRecords[0].m_APMDRecords.GetNext(pos);
			n01Count += pNewRecord->Particle01;
			n03Count += pNewRecord->Particle03;
			n05Count += pNewRecord->Particle05;
		}
	}
	else return - 1;

	return 0;
}

//! strMark가 나타는데까지의 결과리턴
int CRepository::ScanMarkSection(POSITION & curPos, tMarkSectionInfo & MarkSectionInfo, int KindOfata)
{
	tAPMDRecord *pNewRecord;
	int AccHumi = 0;
	
	MarkSectionInfo.particle01.iMax = 0;
	MarkSectionInfo.particle01.iMin = 0;
	MarkSectionInfo.particle01.iSum = 0;

	MarkSectionInfo.particle03.iMax = 0;
	MarkSectionInfo.particle03.iMin = 0;
	MarkSectionInfo.particle03.iSum = 0;

	MarkSectionInfo.particle05.iMax = 0;
	MarkSectionInfo.particle05.iMin = 0;
	MarkSectionInfo.particle05.iSum = 0;

	MarkSectionInfo.Humi.iMax = 0;
	MarkSectionInfo.Humi.iMin = 0;
	MarkSectionInfo.Humi.dblAvg = 0.0;
	MarkSectionInfo.iCumulativeTime = 0;
	int nDisconnected = 0;
	bool IsFirstMark = false;
	for (int i=0; curPos != NULL;i++)
	{
		pNewRecord = (tAPMDRecord *)m_DataRecords[KindOfata].m_APMDRecords.GetNext(curPos);
		if (!pNewRecord->strMark.IsEmpty())
		{
			if(!IsFirstMark)
			{
				IsFirstMark = true;
				MarkSectionInfo.strStartMark = pNewRecord->strMark;
				MarkSectionInfo.particle01.iMin = pNewRecord->Particle01;
				MarkSectionInfo.particle03.iMin = pNewRecord->Particle03;
				MarkSectionInfo.particle05.iMin = pNewRecord->Particle05;
				MarkSectionInfo.Humi.iMin = pNewRecord->Humidity;
			}
			else
			{
				MarkSectionInfo.strEndMark = pNewRecord->strMark;
				if (pNewRecord->strMark != MARK_END)
				{
					if (curPos)m_DataRecords[0].m_APMDRecords.GetPrev(curPos);
					return 0;
				}
			}
		}
		if (IsFirstMark)
		{
			MarkSectionInfo.particle01.iSum += pNewRecord->Particle01;
			MarkSectionInfo.particle03.iSum += pNewRecord->Particle03;
			MarkSectionInfo.particle05.iSum += pNewRecord->Particle05;
			AccHumi += pNewRecord->Humidity;
			if (!pNewRecord->Humidity)nDisconnected++;

			int newCounted = (i + 1 - nDisconnected);
			if(newCounted>0) MarkSectionInfo.Humi.dblAvg = ((double)AccHumi) / newCounted;
			else MarkSectionInfo.Humi.dblAvg = 0;

			MarkSectionInfo.particle01.iMax = (MarkSectionInfo.particle01.iMax < pNewRecord->Particle01 ? pNewRecord->Particle01 : MarkSectionInfo.particle01.iMax);
			MarkSectionInfo.particle03.iMax = (MarkSectionInfo.particle03.iMax < pNewRecord->Particle03 ? pNewRecord->Particle03 : MarkSectionInfo.particle03.iMax);
			MarkSectionInfo.particle05.iMax = (MarkSectionInfo.particle05.iMax < pNewRecord->Particle05 ? pNewRecord->Particle05 : MarkSectionInfo.particle05.iMax);
			MarkSectionInfo.Humi.iMax = (MarkSectionInfo.Humi.iMax < pNewRecord->Humidity ? pNewRecord->Humidity : MarkSectionInfo.Humi.iMax);

			MarkSectionInfo.particle01.iMin = (MarkSectionInfo.particle01.iMin > pNewRecord->Particle01 ? pNewRecord->Particle01 : MarkSectionInfo.particle01.iMin);
			MarkSectionInfo.particle03.iMin = (MarkSectionInfo.particle03.iMin > pNewRecord->Particle03 ? pNewRecord->Particle03 : MarkSectionInfo.particle03.iMin);
			MarkSectionInfo.particle05.iMin = (MarkSectionInfo.particle05.iMin > pNewRecord->Particle05 ? pNewRecord->Particle05 : MarkSectionInfo.particle05.iMin);
			MarkSectionInfo.Humi.iMin = (MarkSectionInfo.Humi.iMin > pNewRecord->Humidity ? pNewRecord->Humidity : MarkSectionInfo.Humi.iMin);
			MarkSectionInfo.iCumulativeTime++;
		}
	}

	return -1;
}


int CRepository::ScanMarkFirst(POSITION & headPos)
{
	headPos = m_DataRecords[0].m_APMDRecords.GetHeadPosition();
	return 0;
}


int CRepository::FindPrevMark(int nIndex, tMarkInfo& MarkInfo)
{
	tAPMDRecord *pNewRecord;
	POSITION pos = m_DataRecords[0].m_APMDRecords.FindIndex(nIndex);
	int curIndex = nIndex;
	if (!pos)return -1;
	do {
		pNewRecord = (tAPMDRecord *)m_DataRecords[0].m_APMDRecords.GetPrev(pos);
		if (!pNewRecord->strMark.IsEmpty())
		{
			MarkInfo.nIndex = curIndex;
			MarkInfo.strMarkName = pNewRecord->strMark;

			return 0;
		}
		if (pos)
		{
			if (curIndex)curIndex--;
		}
	} while (pos);
	return -1;
}

int CRepository::FindNextMark(int nIndex, tMarkInfo &MarkInfo)
{
	tAPMDRecord *pNewRecord;
	if (m_DataRecords[0].m_APMDRecords.IsEmpty())return -2;
	POSITION pos = m_DataRecords[0].m_APMDRecords.FindIndex(nIndex);
	int curIndex = nIndex;
	if (!pos)return -1;
	do {
		pNewRecord = (tAPMDRecord *)m_DataRecords[0].m_APMDRecords.GetNext(pos);
		if (!pNewRecord->strMark.IsEmpty())
		{
			MarkInfo.nIndex = curIndex;
			MarkInfo.strMarkName = pNewRecord->strMark;

			return 0;
		}
		if (pos)curIndex++;
	} while (pos);
	return -1;
}

int CRepository::AnalizeMarkToMark(int nIndex, tMarkSectionInfo & MarkSectionInfo, int KindOfData)
{
	tAPMDRecord *pNewRecord;
	if (m_DataRecords[KindOfData].m_APMDRecords.IsEmpty())return -2;
	POSITION pos = m_DataRecords[KindOfData].m_APMDRecords.FindIndex(nIndex);
	if (!pos)return -1;
	do {
		pNewRecord = (tAPMDRecord *)m_DataRecords[KindOfData].m_APMDRecords.GetPrev(pos);
		if (!pNewRecord->strMark.IsEmpty())
		{
			MarkSectionInfo.strStartMark = pNewRecord->strMark;
			break;
		}
	} while (pos);

	if (pos) m_DataRecords[KindOfData].m_APMDRecords.GetNext(pos);
	else
		pos = m_DataRecords[KindOfData].m_APMDRecords.GetHeadPosition();

	ScanMarkSection(pos, MarkSectionInfo);

	return 0;
}


int CRepository::LoadCompareData(CString strCompareFile)
{
	CStdioFile LogFile;
	CFileException ex;
	tLogType logType = LT_LOG;

	CString strExtName = strCompareFile.Right(strCompareFile.GetLength() - strCompareFile.ReverseFind('.') - 1);

	if ((strExtName.MakeUpper() != _T("LOG")) && (strExtName.MakeUpper() != _T("CSV")))
	{
		AfxMessageBox(_T("Please enter the LOG File"), MB_OK);
		return -1;
	}
	if ((strExtName.MakeUpper() == _T("LOG")))
		logType = LT_LOG;
	else
		logType = LT_CSV;

	if (!LogFile.Open(strCompareFile.GetBuffer(), CFile::modeRead, &ex)) {
		return -1;//Can't open file 
	}

	
	if (m_DataRecords[1].m_APMDRecords.GetCount() != 0)EraseAllRecords(&m_DataRecords[1]);//!이미사용하고 있는 것이 있으면 날린다.
	ReadFromLogFile(&LogFile, &m_DataRecords[1], logType);
	LogFile.Close();
	MakeMarkInfos(1);
	Update();
	return 0;
}

void CRepository::AddEndMark()
{
	tAPMDRecord *pNewRecord;
	POSITION pos = m_DataRecords[0].m_APMDRecords.GetTailPosition();
	if (pos)
		pNewRecord = (tAPMDRecord *)(m_DataRecords[0].m_APMDRecords.GetAt(pos));
	else return;

	pNewRecord->strMark = MARK_END;
}


int CRepository::SetCurOpenFile(CString strCurFileName)
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->SetTitleCaption(strCurFileName);
	return 0;
}


