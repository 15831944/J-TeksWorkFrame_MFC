#include "stdafx.h"
#include "ExcelReport.h"
#include "Status.h"
#include <Tlhelp32.h>
CExcelReport::CExcelReport()
	: m_strFileName(_T(""))
	, m_iLastNumber(0)
{
}


CExcelReport::~CExcelReport()
{
}

void CExcelReport::SetFileName(CString strFileName)
{
	m_strFileName = strFileName;
}

void CExcelReport::OpenExcelFile()
{
	CString strThisPath;
	m_iLastNumber = 0;
	CFileDialog SaveExelReport(TRUE);
	m_pXL = new CXLEzAutomation(FALSE);

	CString strLogFile = CStatus::Instance()->GetDataSaveFolder() + _T("\\");
	strLogFile += CStatus::Instance()->GetDataFileName();
	CString exlReportForm = CAppInfo::GetAppDir() + EXL_FORM;
	m_pXL->OpenExcelFile(exlReportForm);             // 엑셀 파일 열기
}

void CExcelReport::CloseExcelFile()
{
	m_pXL->SaveFileAs(m_strFileName);                  //  엑셀 파일 저장 
	m_pXL->ReleaseExcel();                             // 엑셀 파일 해제
	delete m_pXL;
	KillProcess();
}

void CExcelReport::MakeHeader()
{
	CString strTime;
	CTime curTime = CTime::GetCurrentTime();
	strTime.Format(_T("%04d-%02d-%02d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());

	strTime.Format(_T(" »  Date : %s"), strTime);
	m_pXL->SetCellValue(1, 3, strTime);
}

#pragma warning(disable:4996)
void CExcelReport::InseartNewRecord(int nIndex)
{
	//CClipBoard::CopyWndImgToClipboard((CWnd *)(CStatus::Instance()->m_pSensorViewDlg));
	CClipBoard::CopyWndImgToResizeClipboard((CWnd *)(CStatus::Instance()->m_pSensorViewDlg), 640, 640);
	m_excelRecords[nIndex - 1].AGMDRecord = *CStatus::Instance()->GetCurMeasureData();
	CString strFileName = CStatus::Instance()->GetLastOpenFileName();
	strFileName = CFileInfo::ExtractFileName(strFileName);
	CStatus::tNotchMode tNotch = CStatus::Instance()->GetNotchMode();
	
	CString strNotch;
	if (tNotch == CStatus::tNotchMode::TWO_CLOCK)
		strNotch = _T("2");
	if (tNotch == CStatus::tNotchMode::FOUR_CLOCK)
		strNotch = _T("4");
	if (tNotch == CStatus::tNotchMode::SIX_CLOCK)
		strNotch = _T("6");
	if (tNotch == CStatus::tNotchMode::EIGHT_CLOCK)
		strNotch = _T("8");
	if (tNotch == CStatus::tNotchMode::TEN_CLOCK)
		strNotch = _T("10");
	if (tNotch == CStatus::tNotchMode::TWELVE_CLOCK)
		strNotch = _T("12");
	
	m_excelRecords[nIndex - 1].strFileName = strFileName;
	m_pXL->InsertPictureFromClipboard(6, 7 + (nIndex - 1) * 3);
	struct tm DateTime;
	DateTime = *localtime(&(m_excelRecords[nIndex - 1].AGMDRecord.CurTime));
	CString strDate;
	strDate.Format(_T("%04d-%02d-%02d"), DateTime.tm_year + 1900, DateTime.tm_mon + 1, DateTime.tm_mday);
	CString strTime;
	strTime.Format(_T("%02d:%02d:%02d"), DateTime.tm_hour, DateTime.tm_min, DateTime.tm_sec);

	m_pXL->SetCellValue(3, 8 + (nIndex - 1) * 3, strFileName);
	m_pXL->SetCellValue(8, 8 + (nIndex - 1) * 3, strDate);
	m_pXL->SetCellValue(12, 8 + (nIndex - 1) * 3, strTime);
	m_pXL->SetCellValue(17, 8 + (nIndex - 1) * 3, strNotch);
	m_pXL->SetCellValue(3, 9 + (nIndex - 1) * 3, m_excelRecords[nIndex - 1].AGMDRecord.workEnvInfo.szComment);
	m_iLastNumber = nIndex;
}

void CExcelReport::RemoveDummy()
{
	int toDeleteLine = 7 - m_iLastNumber;
	for (int i = 0; i < toDeleteLine; i++)
	{
		m_pXL->DeleteRow(32 + m_iLastNumber);
	}

	for (int i = 0; i < toDeleteLine * 3; i++)
	{
		m_pXL->DeleteRow(7 + m_iLastNumber * 3);
	}
}

void CExcelReport::DataReport()
{
	CString strUnit;
	CString strTarget_Gap1, strTarget_Gap2, strTarget_Gap3;
	CString strTolerance_Gap1, strTolerance_Gap2, strTolerance_Gap3;
	CString strGap1, strGap2, strGap3;
	CString strOffset_Gap1, strOffset_Gap2, strOffset_Gap3;
	CString strResult_Gap1, strResult_Gap2, strResult_Gap3;
	for (int nIndex = 1; nIndex <= m_iLastNumber; nIndex++)
	{
		int iUnit = m_excelRecords[nIndex - 1].AGMDRecord.iUnitMode;
		if (iUnit == (int)CStatus::tUnitMode::UNIT_INCH)	strUnit = _T("inches");
		if (iUnit == (int)CStatus::tUnitMode::UNIT_MM)		strUnit = _T("mm");

		m_pXL->SetCellValue(2, 31 + nIndex, strUnit);

		double dblTarget_Gap1, dblTarget_Gap2, dblTarget_Gap3;

		if (m_excelRecords[nIndex - 1].AGMDRecord.bIsAllGapSame)
		{
			dblTarget_Gap1 = m_excelRecords[nIndex - 1].AGMDRecord.dblTarget_AllGap;
			dblTarget_Gap2 = m_excelRecords[nIndex - 1].AGMDRecord.dblTarget_AllGap;
			dblTarget_Gap3 = m_excelRecords[nIndex - 1].AGMDRecord.dblTarget_AllGap;
		}
		else
		{
			dblTarget_Gap1 = m_excelRecords[nIndex - 1].AGMDRecord.dblTarget_Gap1;
			dblTarget_Gap2 = m_excelRecords[nIndex - 1].AGMDRecord.dblTarget_Gap2;
			dblTarget_Gap3 = m_excelRecords[nIndex - 1].AGMDRecord.dblTarget_Gap3;
		}

		strTarget_Gap1.Format(_T("%.3f"), dblTarget_Gap1);
		strTarget_Gap2.Format(_T("%.3f"), dblTarget_Gap2);
		strTarget_Gap3.Format(_T("%.3f"), dblTarget_Gap3);

		m_pXL->SetCellValue(3, 31 + nIndex, strTarget_Gap1);
		m_pXL->SetCellValue(7, 31 + nIndex, strTarget_Gap2);
		m_pXL->SetCellValue(11, 31 + nIndex, strTarget_Gap3);


		double dblTolerance_Gap1, dblTolerance_Gap2, dblTolerance_Gap3;

		if (m_excelRecords[nIndex - 1].AGMDRecord.bIsAllGapSame)
		{
			dblTolerance_Gap1 = m_excelRecords[nIndex - 1].AGMDRecord.dblTolerance_AllGap;
			dblTolerance_Gap2 = m_excelRecords[nIndex - 1].AGMDRecord.dblTolerance_AllGap;
			dblTolerance_Gap3 = m_excelRecords[nIndex - 1].AGMDRecord.dblTolerance_AllGap;
		}
		else
		{
			dblTolerance_Gap1 = m_excelRecords[nIndex - 1].AGMDRecord.dblTolerance_Gap1;
			dblTolerance_Gap2 = m_excelRecords[nIndex - 1].AGMDRecord.dblTolerance_Gap2;
			dblTolerance_Gap3 = m_excelRecords[nIndex - 1].AGMDRecord.dblTolerance_Gap3;
		}
		
		strTolerance_Gap1.Format(_T("%.3f"), dblTolerance_Gap1);
		strTolerance_Gap2.Format(_T("%.3f"), dblTolerance_Gap2);
		strTolerance_Gap3.Format(_T("%.3f"), dblTolerance_Gap3);
		
		m_pXL->SetCellValue(4, 31 + nIndex, strTolerance_Gap1);
		m_pXL->SetCellValue(8, 31 + nIndex, strTolerance_Gap2);
		m_pXL->SetCellValue(12, 31 + nIndex, strTolerance_Gap3);

		double dblCalcGap1 = OriginalValueToCalcValue(m_excelRecords[nIndex - 1].AGMDRecord.usGap1, iUnit);
		double dblCalcGap2 = OriginalValueToCalcValue(m_excelRecords[nIndex - 1].AGMDRecord.usGap2, iUnit);
		double dblCalcGap3 = OriginalValueToCalcValue(m_excelRecords[nIndex - 1].AGMDRecord.usGap3, iUnit);

		if (m_excelRecords[nIndex - 1].AGMDRecord.bIsOffsetMode)
		{
			m_pXL->SetCellValue(15, 31 + nIndex, _T("On"));

			strOffset_Gap1.Format(_T("%.3f"), m_excelRecords[nIndex - 1].AGMDRecord.dblOffset_Gap1);
			strOffset_Gap2.Format(_T("%.3f"), m_excelRecords[nIndex - 1].AGMDRecord.dblOffset_Gap2);
			strOffset_Gap3.Format(_T("%.3f"), m_excelRecords[nIndex - 1].AGMDRecord.dblOffset_Gap3);

			m_pXL->SetCellValue(16, 31 + nIndex, strOffset_Gap1);
			m_pXL->SetCellValue(17, 31 + nIndex, strOffset_Gap2);
			m_pXL->SetCellValue(18, 31 + nIndex, strOffset_Gap3);


			dblCalcGap1 += m_excelRecords[nIndex - 1].AGMDRecord.dblOffset_Gap1;
			dblCalcGap2 += m_excelRecords[nIndex - 1].AGMDRecord.dblOffset_Gap2;
			dblCalcGap3 += m_excelRecords[nIndex - 1].AGMDRecord.dblOffset_Gap3;
		}
		else
			m_pXL->SetCellValue(15, 31 + nIndex, _T("Off"));

		strGap1.Format(_T("%.3f"), dblCalcGap1);
		strGap2.Format(_T("%.3f"), dblCalcGap2);
		strGap3.Format(_T("%.3f"), dblCalcGap3);

		m_pXL->SetCellValue(5, 31 + nIndex, strGap1);
		m_pXL->SetCellValue(9, 31 + nIndex, strGap2);
		m_pXL->SetCellValue(13, 31 + nIndex, strGap3);

		strResult_Gap1 = CheckResult(dblTarget_Gap1, dblTolerance_Gap1, dblCalcGap1);
		strResult_Gap2 = CheckResult(dblTarget_Gap2, dblTolerance_Gap2, dblCalcGap2);
		strResult_Gap3 = CheckResult(dblTarget_Gap3, dblTolerance_Gap3, dblCalcGap3);

		m_pXL->SetCellValue(6, 31 + nIndex, strResult_Gap1);
		m_pXL->SetCellValue(10, 31 + nIndex, strResult_Gap2);
		m_pXL->SetCellValue(14, 31 + nIndex, strResult_Gap3);
	}
}

CString CExcelReport::CheckResult(double dblTargetValue, double dblTolerance, double dblGap)
{
	double dblMaxTarget, dblMinTarget;
	CString strResult;
	dblMaxTarget = dblTargetValue + dblTolerance;
	dblMinTarget = dblTargetValue - dblTolerance;

	if (dblGap <= dblMaxTarget && dblGap >= dblMinTarget)
		strResult = _T("In");
	else
		strResult = _T("Out");
	return strResult;
}

double CExcelReport::OriginalValueToCalcValue(double dblGapValue, int iGapUnit)
{
	double dbCalcValue;
	if (iGapUnit == CStatus::tUnitMode::UNIT_MM)
		dbCalcValue = dblGapValue / UNIT_MM_VALUE;
	if (iGapUnit == CStatus::tUnitMode::UNIT_INCH)
		dbCalcValue = dblGapValue / UNIT_MM_VALUE * UNIT_INCH_VALUE; //  0.03937 : mm -> inche
	return dbCalcValue;
}

void CExcelReport::CloseExcelNoSave()
{
	CString strTempFile = CAppInfo::GetAppDir();
	strTempFile += _T("\\Temp.xlsx");
	::DeleteFile(strTempFile);
	m_pXL->SaveFileAs(strTempFile);                  //  엑셀 파일 저장 
	m_pXL->ReleaseExcel();                             // 엑셀 파일 해제

	delete m_pXL;
	::DeleteFile(strTempFile);
	KillProcess();
}

void CExcelReport::KillProcess()
{
	DWORD dwSize = 250;
	HANDLE hSnapShot;
	PROCESSENTRY32 pEntry;
	BOOL bCrrent = FALSE;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	pEntry.dwSize = sizeof(pEntry);
	Process32First(hSnapShot, &pEntry);
	//
	while (1)
	{
		// 다음번 프로세스의 정보를 가져온다.
		BOOL hRes = Process32Next(hSnapShot, &pEntry);

		if (hRes == FALSE)
			break;
		CString strTemp(pEntry.szExeFile);
		if(strTemp == _T("EXCEL.EXE"))
		{
			bCrrent = TRUE;
		}
		if (bCrrent)
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEntry.th32ProcessID);
			if (hProcess)
			{
				if (TerminateProcess(hProcess, 0))
				{
					unsigned long nCode; //프로세스 종료 상태
					GetExitCodeProcess(hProcess, &nCode);
				}
				CloseHandle(hProcess);
			}
			break;
		}
	}
}
