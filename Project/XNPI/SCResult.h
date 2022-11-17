#pragma once
class CSCResult
{
public:
	CSCResult();
	~CSCResult();
	CString strRefID;
	CString strTime;
	int CamNo;
	int iInspType;
	int iResult;
	BOOL bIsPixelCal;
	double dblMean;
	double dblMin;
	double dblMax;
	double umMean;
	double umMin;
	double umMax;
};

