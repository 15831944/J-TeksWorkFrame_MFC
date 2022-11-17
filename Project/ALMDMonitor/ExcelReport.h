#pragma once
#include "XLEzAutomation.h"
#include "Repository.h"

struct tExcelRecordInfo
{
	tALMDRecord ALMDRecord;
	CString strFileName;
};
class CExcelReport
{
public:
	CExcelReport();
	~CExcelReport();
	void SetFileName(CString strFileName);
	CString m_strFileName;
	int OpenExcelFile();
	int CloseExcelFile();
	int MakeHeader();
	CXLEzAutomation *m_pXL;
	int InseartNewRecord(int nIndex);
	int MakeChart();
	int RemoveDummy();
	tExcelRecordInfo m_excelRecords[10];
	int m_LastNumber;
	int CloseExcelNoSave();
private:
	void KillProcess();
};

