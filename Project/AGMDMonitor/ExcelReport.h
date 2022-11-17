#pragma once
#include "XLEzAutomation.h"
#include "Repository.h"

struct tExcelRecordInfo
{
	tAGMDRecord AGMDRecord;
	CString strFileName;
};

class CExcelReport
{
public:
	CExcelReport();
	~CExcelReport();
	void SetFileName(CString strFileName=_T(""));
	CString m_strFileName;
	CXLEzAutomation *m_pXL;
	void OpenExcelFile();
	void CloseExcelFile();
	void MakeHeader();
	tExcelRecordInfo m_excelRecords[10];
	void InseartNewRecord(int nIndex);
	int m_iLastNumber;
	void RemoveDummy();
	void DataReport();
private:
	CString CheckResult(double dblTargetValue, double dblTolerance, double dblGap);
	double OriginalValueToCalcValue(double dblGapValue, int iGapUnit);
public:
	void CloseExcelNoSave();
	void KillProcess();
};

