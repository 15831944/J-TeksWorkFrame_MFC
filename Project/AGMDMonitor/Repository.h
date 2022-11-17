#pragma once

#pragma pack(push, 1)
struct tWorkEnvInfo
{
	int iIndex;
	WCHAR szOperator[20];
	WCHAR szTool[40];
	WCHAR szStation[40];
	WCHAR szComment[500];
};

struct tAGMDRecord
{
	//CTime CurDate;
	time_t CurTime;
	BYTE	byTemperature;	//-40~100
	BYTE	byBattery;		//0~100
	unsigned short	usGap1;		//0~65535
	unsigned short 	usGap2;		//
	unsigned short	usGap3;
	double dblTarget_Gap1;
	double dblTarget_Gap2;
	double dblTarget_Gap3;
	double dblTolerance_Gap1;
	double dblTolerance_Gap2;
	double dblTolerance_Gap3;
	double dblOffset_Gap1;
	double dblOffset_Gap2;
	double dblOffset_Gap3;
	double dblTarget_AllGap;
	double dblTolerance_AllGap;
	BOOL bIsAllGapSame;
	int iNotchMode;
	//TCHAR	szSerial[20];
	BOOL bIsOffsetMode;
	int iUnitMode; // 0 : Inch, 1:mm
	double dblGapViewRange;
	time_t	dateCalibration;
	tWorkEnvInfo workEnvInfo;
	WCHAR szMark[100];
};
#pragma pack(pop)

struct tRecordsSet 
{
	CPtrList m_AGMDRecords;
	int nCountOfRecords;
};

class CRepository
{
public:
	enum tLogType { LT_LOG, LT_CSV };
	CRepository();
	~CRepository();
	tRecordsSet m_DataRecords;

	void WriteCurStatus();
	int AppendData(tAGMDRecord * pNewRecord);
	int NewMakeLogFile(CString strFileName);
private:
	int AppendLogFile(tAGMDRecord *pNewRecord);
protected:
	int AppendLogFileCore(CFile * pFile, tAGMDRecord * pNewRecord);
	int NewLogFileCore(CFile * pFile);
	int AppendCSVFileCore(CStdioFile *pStdioFile, tAGMDRecord * pNewRecord);
	int NewCSVFileCore(CStdioFile *pStdioFile);
public:
	CString m_strCurLogName;
	CString m_strCurCSVName;
	BOOL m_bNewMeasureState;
	CString MakeCSVRecordString(struct tm * pDateTime, struct tm * pCalDateTime, tAGMDRecord * pNewRecord);
	CString MakeReviewCSVRecordString(struct tm * pDateTime, struct tm * pCalDateTime, tAGMDRecord * pNewRecord);
	int ReadFromLogFile(CString strFileName);
	int SetCurOpenFile(CString strCurFileName);
	void EraseAllRecords();
	void EraseAllRecords(tRecordsSet * pRecordsSet);
	void EraseAllRecords(CPtrList *pRecords);
	int ReadFromLogFile(CFile* pLogFile, tRecordsSet * pRecordsSet, tLogType logType);
	int m_iDispIndex;
	void ClearData();
	tAGMDRecord * GetMeasureInfo(int nPos);
	void ExportCSVFile(CString strFileName);
};

