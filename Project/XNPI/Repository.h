#pragma once
#include "ModelData.h"
#include "SCResult.h"
#include <Matrox.h>
#include "afxcoll.h"

struct tXFMSRecord
{
	long Index; //from 1~
	long Temperature[6];	//ea
	CString strMark;    //Mark가 있으면 설정
};

struct tRecordsSet {
	CPtrList m_XFMSRecords;
	int nCountOfRecords;
};
class CRepository
{
public:
	CRepository();
	~CRepository();
	static CRepository * Instance();
	static CRepository *m_pInstance;
	CMilVLib m_MilLib;
	CVWGrabber m_Grabber;
	int WriteTemperatureData(long * pTempDatas);
	void UpdateTemperatureFile(CString strFileName, long * pTempDatas);
	tRecordsSet m_DataRecords;
	int AppendData(tXFMSRecord * pNewRecord, CString strCSVFileName);
	void EraseAllRecords();
	void EraseAllRecords(CPtrList *pRecords);
	void EraseAllRecords(tRecordsSet * pRecordsSet);
	void MakeTestData();
	CString m_strTempCSVFileName;
};

