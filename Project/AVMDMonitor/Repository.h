#pragma once
#include "afxcoll.h"
#include <deque>
#include "..\Library\SerialLib\SerialMFC.h"	// 필수
#include "..\Library\SerialLib\AVMDPacket.h"	// 필수
#include <mutex>
using namespace std;


#define EXT_LOG _T(".LOG")
#define EXT_CSV _T(".CSV")
#define EXT_TAG _T(".TAG")
#define MAX_DEQUESIZE (4*3600*1000)

//! 데이터는 Raw로 받아들여진다.
struct tMarkInfo
{
	int	iIndex;
	CString strMark;
};

#pragma pack(push, 1)
struct tWorkEnvInfo
{
	int iIndex;
	WCHAR szOperator[20];
	WCHAR szTool[40];
	WCHAR szStation[40];
	WCHAR szComment[500];
};

struct tAVMDData
{
	ULONG	Index; //from 1~
	UINT	timeCurTime;  //time_t사용
	BYTE	byTemperature;	//-40~100
	BYTE	byBattery;		//0~100
	BYTE	nyNum;			//0~255
	DVC_ACCINFO accInfo;
	int		iXValue;		//-1962~1962
	int		iYValue;		//-1962~1962
	int		iZValue;		//-1962~1962
	int		iRMSValue;		//-1962~1962
};

//sizoof(tAVMDRecord) = 132byte
struct tAVMDRecord
{
	UINT	timeCurTime;  //time_t사용
	BYTE	byTemperature;	//-40~100
	BYTE	byBattery;		//0~100
	BYTE	byNum;			//0~255
	DVC_ACCINFO accInfo[VALUESET_COUNT];
};

#pragma pack(pop)

struct tRecordsSet {
	deque<tAVMDRecord *> m_AVMDRecords;
	int nCountOfRecords;
};


class CRepository
{
public:
	enum tTagType{TT_WORKENV, TT_MARK};
	enum tLogType{LT_LOG, LT_CSV};
	enum tSaveStatus{SS_STOP, SS_WRITE, SS_PAUSE};
	enum tData{ORG_DATA, COMPARE_DATA};
	CRepository();
	~CRepository();
	tRecordsSet m_DataRecords[2]; // 0 : View, 1: Review
	int ReadFromLogFile(CFile* pLogFile, tRecordsSet * pRecordsSet, tLogType logType, int nInputType=0);
	int ReadFromLogFile(CString strFileName, int nInputType=0);
	int TestFileWrite();
	CString m_strCurLogName;
	CString m_strCurCSVName;
	CString m_strCurTagName[2];
	int AppendData(tAVMDRecord * pNewRecord);
	void EraseAllRecords(int nInputType=0);
	void NewMeasure();
	bool m_bNewMeasureState;
	void DeleteMark(int nIndex);
	void UpdateMark(int nIndex, CString strMark);
private:
	tSaveStatus m_FileSaveStatus;
protected:
	std::mutex m_mtxLocker;
	int m_iLastWriteTime;
	int m_sizePrepareQueue;
	int AppendLogFile(tAVMDRecord *pNewRecord);
	int FlushLogFileCore(CFile * pFile, int iStartWriteIndex, int iEndWriteIndex);
	int FlushCSVFileCore(CStdioFile *pStdioFile, int iStartWriteIndex, int iEndWriteIndex, int nInputType =0);
	int FlushTagFileCore(CStdioFile * pFile, int nInputType);
	int AppendLogFileCore(CFile * pStdioFile, tAVMDRecord * pNewRecord);
	int AppendCSVFileCore(CStdioFile * pStdioFile, tAVMDRecord * pNewRecord);
public:
	int AppendData(DVC_PACKET_MEAINFO *pPacket);
	void GetLogInfo(CString strCSVLine, CString & strMakeDate, CString & strSerialNo);
	int SetCurOpenFile(CString strCurFileName);
	void EraseAllRecords(tRecordsSet * pRecordsSet);
	void WriteCurStatus();
	void ClearData(int nInputType=0);
	void SetDummyData();
	tAVMDRecord * GetMeasureInfo(int nPos, int nInputType=0);
	int m_iDispIndex;
	int SaveLogFile(CString strFileName);
	CString FlushCSVRecordString(int iIndexNum, tAVMDData * pAVMDData);
	CString MakeCSVRecordString(int iIndexNum, tAVMDData * pAVMDData, int nOffset=0);
	CString MakeFilteredCSVRecordString(int iIndexNum);
	int Flush();
	CString m_strCurMarkName;
	void MakeNewFileName(int nInputType=0);
	int GetPrepareQueueSize();
	int FlushData(int nInputType=0);
	int BeginSave(int iStartIndex, int nCount);
	int PauseSave();
	int ReStartSave();
	int StopSave();
	static UINT WriteFileThread(LPVOID pSendor);
	int MakeFileThread();
	// ! 0 부터 시작하여 마지막까지 저장된 파일위치이다. Write한 갯수가 없으면 -1 부터 시작함
	int m_iLastWriteIndex;//l
	CWinThread *m_pWFileWriteThread;
	BOOL m_bTerminateThread;
	int WaitForFileSaveThreadEnd();
	int TerminateFileSaveThread();
	BOOL IsFileSaveDone(int nInputType=0);
	int GetNextData(tAVMDData *pAVMDData, int idxAVMDData = -1, int nInputType=0);
	int GetPrevData(tAVMDData *pAVMDData, int idxAVMDData = -1, int nInputType=0);
	int GetAtData(tAVMDData *pAVMDData, int idxAVMDData, int nInputType=0);
	int m_CurDataPosition[2];
	int GetLastData(tAVMDData * pLastData, int nInputType=0);
	int GetDataCount(int nInputType=0);
	tSaveStatus GetFileSaveStatus();
//jubatemp	int  m_iTargetIndex;
	BOOL IsSaveTargetIndex();
	BOOL m_bIsSaveTaretIndex;
	int GetLastIndex(int nInputType=0);
	int m_iCSVDispIndexNo;
	int MakeTagInfoFile();
	int SetWorkEnv(int iIndex);
	tTagType ReadTagInfo();
	deque<tWorkEnvInfo *> m_WorkEnvInfo[2];
	deque<tMarkInfo *> m_MarkInfo[2];
		
//	int ReadTagFile(CString strTagFile, int nInputType);
//	int GetWorkEnv(CString strTag);
//	int ReadTagFile(CStdioFile* pTagFile);
	int AppendEnvInfo(CStdioFile * pTagFile, int nInputType=0);
	void MakeDefaultWorkEnvInfo(int nInputType=0);
	int ReadFromTagFile(CString strFileName, int nInputType=0);
	void GetWorkEnvInfo(CString strSubLine, int nInputType);
	void GetMarkInfo(CString strSubLine, int nInputType);
	void GetExtraInfo(CString strSubLine);
	int AppendMarkInfo(CStdioFile * pTagFile, int nInputType);
	int m_nToWriteMarkNo;
	void ResetMarkInfo(int nInputType=0);
	int AddPreRecordMark();
	int AddBeginMark(int nDataPos=0);
	int AddMachineInfo(CStdioFile * pTagFile, int nInputType=0);
	void GetMachineInfo(CString strSubLine, int nInputType);
	int ReSaveTagFile(int nInputType);
	void EraseAllTags(int nInputType);
	HANDLE m_hRecStart;
	int m_nCountOfWrite;
	BOOL ExportLogFile(CString strFileName, int nStartPos=-1, int nEndPos=0);
	BOOL ExportCSVFile(CString strFileName, int nStartPos=-1, int nEndPos=0);
	BOOL ExportFilteredCSVFile(CString strFileName, int nInputType =0);
	BOOL ExportTagFile(CString strFileName, int nStartPos=-1, int nEndPos=0);
	int ExportDataFile(CString strDataFile, int nStartPos=-1, int nEndPos=0);
	int m_nPreRecordTime;
	CString GetMark(int iIndexNum);
	int m_iTargetCount;
	int m_iPausePos;
	void AddMark(int iIndex, CString strMark);
};

