#include "stdafx.h"
#include "Repository.h"
#include "Status.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <locale.h>


CRepository * CRepository::m_pInstance = NULL;
CRepository::CRepository()
	: m_strTempCSVFileName(_T(""))
{
	m_DataRecords.nCountOfRecords = 0;
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

int CRepository::WriteTemperatureData(long * pTempDatas)
{
	UpdateTemperatureFile(m_strTempCSVFileName, pTempDatas);
	return 0;
}


void CRepository::UpdateTemperatureFile(CString strFileName, long * pTempDatas)
{
	setlocale(LC_ALL, "Korean");
	CStdioFile csvFile;
	CFileException ex;
	CString newRecord;
	CTime timeCurrent;
	if (!PathFileExists(strFileName.GetBuffer())) {
		if (!csvFile.Open(strFileName.GetBuffer(), CFile::modeWrite | CFile::modeCreate, &ex)) {
			return;//Can't open file 
		}
		CString strLogInfo;

		timeCurrent = CTime::GetCurrentTime();
		CString strCurrent = timeCurrent.Format(_T("%y%m%d"));
		strLogInfo.Format(_T("DATE:,%s\n"), strCurrent);
		csvFile.WriteString(strLogInfo.GetBuffer());
		
		CString Header(_T("TimeInfo"));
		int nCountOfSensors = CStatus::Instance()->m_nCountOfSensors;
		CString strSensorName;

		for (int i = 0; i < nCountOfSensors; i++)
		{
			CString strSensor;
			strSensorName = CStatus::Instance()->m_strSensorNamePx[i];
			strSensor.Format(_T(",P%d(%s)"), i + 1, strSensorName);
			Header = Header + strSensor;
		}
		Header = Header + _T(",Mark\n");
		csvFile.WriteString(Header.GetBuffer());
	}
	else
	{
		if (!csvFile.Open(strFileName, CFile::modeWrite))
		{
			return;//파일생성불가
		}
		csvFile.SeekToEnd();
	}

	timeCurrent = CTime::GetCurrentTime();
	CString strCurrTime = timeCurrent.Format(_T("%H:%M:%S"));
	newRecord.Format(_T("%s"), strCurrTime.GetBuffer());
	int nCountOfSensors = CStatus::Instance()->m_nCountOfSensors;
	for (int i = 0; i < nCountOfSensors; i++)
	{
		CString strSensor;
		double dblTemperature = pTempDatas[i] / 10.0;
		int	iTemperature = (int)dblTemperature;
		strSensor.Format(_T(",%4d"), iTemperature);
		newRecord = newRecord + strSensor;
	}
	
	CString strMark;
	CString strMarkInfo = CStatus::Instance()->GetMarkString();
	if (strMarkInfo.IsEmpty())
		strMark.Format(_T(",\n"));
	else
		strMark.Format(_T(",%s\n"), strMarkInfo.GetBuffer());
	newRecord = newRecord + strMark;

	csvFile.WriteString(newRecord);
	csvFile.Close();
}


int CRepository::AppendData(tXFMSRecord * pNewRecord, CString strCSVFileName)
{
	m_strTempCSVFileName = strCSVFileName;
	pNewRecord->Index = m_DataRecords.nCountOfRecords;
	m_DataRecords.m_XFMSRecords.AddTail(pNewRecord);
	m_DataRecords.nCountOfRecords++;
	WriteTemperatureData(pNewRecord->Temperature);
	return 0;
}


void CRepository::EraseAllRecords()
{
	EraseAllRecords(&m_DataRecords);
}

void CRepository::EraseAllRecords(tRecordsSet * pRecordsSet)
{
	EraseAllRecords(&(pRecordsSet->m_XFMSRecords));
	pRecordsSet->nCountOfRecords = 0;
}

void CRepository::EraseAllRecords(CPtrList *pRecords)
{
	POSITION pos;
	tXFMSRecord *pNewRecord;

	if (pRecords->GetCount())
	{
		for (pos = pRecords->GetHeadPosition(); pos != NULL;)
		{
			pNewRecord = (tXFMSRecord *)pRecords->GetNext(pos);
			delete pNewRecord;
		}
		pRecords->RemoveAll();
	}
}


void CRepository::MakeTestData()
{
	tXFMSRecord XFMSRecord;
	tXFMSRecord *pNewRecord = new tXFMSRecord;

	for (int j = 0; j < (3600*8-2); j++)
	{
		tXFMSRecord *pNewRecord = new tXFMSRecord;
		pNewRecord->Temperature[0] = 4250;
		pNewRecord->Temperature[1] = 8500;
		pNewRecord->Temperature[2] = 12750;
		pNewRecord->Temperature[3] = 17000;
		//pNewRecord->Temperature[0] = 100 + 500 * sin(j*(M_PI / 180));
		//pNewRecord->Temperature[1] = 500 + 500 * sin(j*(M_PI / 180));
		//pNewRecord->Temperature[2] = 500 + 500 * cos(j*(M_PI / 180));
		//pNewRecord->Temperature[3] = 500 + 500 * cos(j*(M_PI / 180));
		pNewRecord->Index = m_DataRecords.nCountOfRecords;
		m_DataRecords.m_XFMSRecords.AddTail(pNewRecord);
		m_DataRecords.nCountOfRecords++;
//		AppendData(pNewRecord, );
	}
}
