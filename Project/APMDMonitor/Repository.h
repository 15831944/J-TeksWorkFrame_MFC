#pragma once
#include "afxcoll.h"

//! 데이터는 Raw로 받아들여진다.
struct tAPMDRecord
{
	long Index; //from 1~
	long TimeInfo;		//sec
	long Particle01;	//ea
	long Particle03;	//ea
	long Particle05;	//ea
	long Temperature;	//-4000~8500
	long Pressure;		//0~2400
	long Battery;		//0~100
	long Humidity;		//0~100
	CString strMark;    //Mark가 있으면 설정
};

struct tMarkRecord
{
	long Index;
	TCHAR *MarkName;
};

struct tParticleStaticInfo
{
	int iMax;
	int iMin;
	int iSum;
};

struct tHumiStaticInfo
{
	int iMax;
	int iMin;
	double dblAvg;
};

struct tMarkSectionInfo
{
	CString strStartMark;
	CString strEndMark;
	tParticleStaticInfo particle01;
	tParticleStaticInfo particle03;
	tParticleStaticInfo particle05;
	tHumiStaticInfo Humi;
	int iCumulativeTime;
};

struct tMarkInfo
{
	int nIndex;
	tAPMDRecord *pNewRecord;
	CString strMarkName;
};

struct tRecordsSet {
	CPtrList m_APMDRecords;
	CPtrList lstMarkInfos;
	int nCountOfRecords;
};

class CRepository
{
public:
	enum tDataKind{DK_MAIN, DK_COMPARE};
	enum tLogType{LT_LOG, LT_CSV};
	CRepository();
	~CRepository();
	tRecordsSet m_DataRecords[2]; //! 0:Original 1:Compare
//	CPtrList m_APMDRecords;
//	CPtrList m_CompareRecords;
	void ExportCSVFile(CString strFileName);
	int ReadFromLogFile(CStdioFile* pLogFile, tRecordsSet * pRecordsSet, tLogType logType);
	int ReadFromLogFile(CString strFileName);
	int TestFileWrite();
	int MarkNameSet(int Index, CString strName);
	CString m_strCurLogName;
	int Update();
	int Register();
	int AppendData(tAPMDRecord * pNewRecord);
	int UpdateRecord();
	int CalculateSum(int &iAccum01, int &iAccum03, int &iAccum05);
	int CalculateSum(int nIndex, int & nPat01, int & nPat03, int & nPat05, int KindOfData=0);
	int CheckWarning(bool & bUseWarning, bool & bIsError, CString & strErrMsg);
//	int m_nCountOfRecords;
	void EraseAllRecords();
	void EraseAllRecords(CPtrList *pRecords);
//	CPtrList m_lstMarkInfos;
//	CPtrList m_lstCompareMarkInfos;
	int MakeMarkInfos(int nKindOfData=0);
	void NewMeasure();
	bool m_bNewMeasureState;
	void DeleteMark(int nIndex);
	void UpdateMark(int nIndex, CString strMark);
	CString m_strCurCSVName;
protected:
	int AppendLogFile(tAPMDRecord *pNewRecord);
	int AppendLogFileCore(CStdioFile * pStdioFile, tAPMDRecord * pNewRecord);
	int AppendCSVFileCore(CStdioFile * pStdioFile, tAPMDRecord * pNewRecord);
public:
	void GetLogInfo(CString strCSVLine, CString & strMakeDate, CString & strSerialNo);
	int CalcParticleCount(int &n01Count, int &n03Count, int &n05Count);
	int ScanMarkSection(POSITION & curPos, tMarkSectionInfo & MarkSectionInfo, int KindOfata=0);
	int ScanMarkFirst(POSITION & headPos);
	int AnalizeMarkToMark(int nIndex, tMarkSectionInfo & MarkSectionInfo, int KindOfData=0);
	int LoadCompareData(CString strCompareFile);
	int FindNextMark(int nIndex, tMarkInfo &MarkInfo);
	int FindPrevMark(int nIndex, tMarkInfo& MarkInfo);
	void AddEndMark();
	int SetCurOpenFile(CString strCurFileName);
	void EraseAllRecords(tRecordsSet * pRecordsSet);
	void EraseAllTags(CPtrList * pLstMarkInfo);
};

