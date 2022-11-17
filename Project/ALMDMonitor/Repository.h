#pragma once
#include "afxcoll.h"

//struct DVC_PACKET_MEAINFO;

//! 데이터는 Raw로 받아들여진다.

#pragma pack(push, 1)
struct tMarkInfo
{
	WCHAR szOperator[20];
	WCHAR szTool[40];
	WCHAR szStation[40];
	WCHAR szComment[500];
};

struct tALMDRecord
{
	time_t CurTime;
	BYTE	byTemperature;	//-40~100
	BYTE	byBattery;		//0~100
	short	sRawRoll;		//-30000~30000
	short 	sRawPitch;		//-30000~30000
	double	dblDispRoll;
	double	dblDispPitch;
	double	dblRefRoll;		//-30~30
	double	dblRefPitch;    //-30~30
	BOOL	bRel;		//TRUE Rel, FALSE ABS
	BOOL	bStableRoll;
	BOOL	bStablePitch;
	BOOL	bStablization;
	double  dblGoNoGoValue;
	TCHAR	szSerial[20];
	time_t	dateCalibration;
	tMarkInfo MarkInfo;
};
#pragma pack(pop)

struct tRecordsSet {
	CPtrList m_ALMDRecords;
	int nCountOfRecords;
};


class CRepository
{
public:
	enum tLogType{LT_LOG, LT_CSV};
	CRepository();
	~CRepository();
	tRecordsSet m_DataRecords; 
	void ExportCSVFile(CString strFileName);
	int ReadFromLogFile(CFile* pLogFile, tRecordsSet * pRecordsSet, tLogType logType);
	int ReadFromLogFile(CString strFileName);
	int TestFileWrite();
	CString m_strCurLogName;
	CString m_strCurCSVName;
	int AppendData(tALMDRecord * pNewRecord);
	void EraseAllRecords();
	void EraseAllRecords(CPtrList *pRecords);
	void NewMeasure();
	bool m_bNewMeasureState;
	void DeleteMark(int nIndex);
	void UpdateMark(int nIndex, CString strMark);
protected:
	int AppendLogFile(tALMDRecord *pNewRecord);
	int AppendLogFileCore(CFile * pStdioFile, tALMDRecord * pNewRecord);
	int AppendCSVFileCore(CStdioFile * pStdioFile, tALMDRecord * pNewRecord);
public:
	void GetLogInfo(CString strCSVLine, CString & strMakeDate, CString & strSerialNo);
	int SetCurOpenFile(CString strCurFileName);
	void EraseAllRecords(tRecordsSet * pRecordsSet);
	void WriteCurStatus();
	void ClearData();
	tALMDRecord * GetMeasureInfo(int nPos);
	int m_iDispIndex;
	int SaveLogFile(CString strFileName);
	CString MakeCSVRecordString(struct tm * pDateTime, struct tm * pCalDateTime, tALMDRecord * pNewRecord);
};

