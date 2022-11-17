#include "stdafx.h"
#include "ExcelReport.h"
#include "Status.h"
#include "UnitConversion.h"
//#include <UniUtility.h>
#include "..\..\Library\UniUtility\UniUtility.h"
#include <Tlhelp32.h>
CExcelReport::CExcelReport()
	: m_strFileName(_T(""))
	, m_LastNumber(0)
{
}


CExcelReport::~CExcelReport()
{
}


void CExcelReport::SetFileName(CString strFileName)
{
	m_strFileName = strFileName;
}


int CExcelReport::OpenExcelFile()
{
	CString strThisPath;

	CFileDialog SaveExelReport(TRUE);
	m_pXL = new CXLEzAutomation(FALSE);

	CString strLogFile = CStatus::Instance()->GetDataSaveFolder() + _T("\\");
	strLogFile += CStatus::Instance()->GetDataFileName();
	CString exlReportForm = CAppInfo::GetAppDir() + EXL_FORM;
	m_pXL->OpenExcelFile(exlReportForm);             // 엑셀 파일 열기
	return 0;
}


int CExcelReport::CloseExcelFile()
{
	m_pXL->SaveFileAs(m_strFileName);                  //  엑셀 파일 저장 
	m_pXL->ReleaseExcel();                             // 엑셀 파일 해제
	delete m_pXL;
	KillProcess();
	return 0;
}


int CExcelReport::CloseExcelNoSave()
{
	CString strTempFile = CAppInfo::GetAppDir();
	strTempFile += _T("\\Temp.xlsx");
	::DeleteFile(strTempFile);
	m_pXL->SaveFileAs(strTempFile);                  //  엑셀 파일 저장 
	m_pXL->ReleaseExcel();                             // 엑셀 파일 해제
	delete m_pXL;	
	::DeleteFile(strTempFile);
	KillProcess();
	return 0;
}


int CExcelReport::MakeHeader()
{
	CString strTime;
	CTime curTime = CTime::GetCurrentTime();
	strTime.Format(_T("%04d-%02d-%02d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());
	
	strTime.Format(_T(" »  Date : %s"), strTime);
	m_pXL->SetCellValue(1, 3, strTime);

	return 0;
}

#pragma warning(disable:4996)
int CExcelReport::InseartNewRecord(int nIndex)
{
	CClipBoard::CopyWndImgToClipboard((CWnd *)(CStatus::Instance()->m_pViewDlg));
	m_excelRecords[nIndex - 1].ALMDRecord = *CStatus::Instance()->GetCurMeasureData();
	CString strFileName = CStatus::Instance()->GetLastOpenFileName();
	strFileName = CFileInfo::ExtractFileName(strFileName);

	m_excelRecords[nIndex - 1].strFileName = strFileName;
	m_pXL->InsertPictureFromClipboard(1, 7+(nIndex-1)*3);
	struct tm DateTime;
	DateTime = *localtime(&(m_excelRecords[nIndex - 1].ALMDRecord.CurTime));
	CString strDate;
	strDate.Format(_T("%04d-%02d-%02d"), DateTime.tm_year + 1900, DateTime.tm_mon + 1, DateTime.tm_mday);
	CString strTime;
	strTime.Format(_T("%02d:%02d:%02d"), DateTime.tm_hour, DateTime.tm_min, DateTime.tm_sec);
	m_pXL->SetCellValue(3, 8 + (nIndex - 1) * 3, strFileName);
	m_pXL->SetCellValue(6, 8 + (nIndex - 1) * 3, strDate);
	m_pXL->SetCellValue(9, 8 + (nIndex - 1) * 3, strTime);
	m_pXL->SetCellValue(3, 9 + (nIndex - 1) * 3, m_excelRecords[nIndex - 1].ALMDRecord.MarkInfo.szComment);
	m_LastNumber = nIndex;
	return 0;
}


int CExcelReport::MakeChart()
{
	CString strRollAndPitch;

	CString strXUnit, strYUnit;
	CUnitConversion::Instance()->GetUnitString(strXUnit, strYUnit);
	if (strXUnit == _T("°"))strXUnit = _T("Degree");
	if (strYUnit == _T("°"))strYUnit = _T("Degree");
	m_pXL->SetCellValue(2, 31, strXUnit);
	m_pXL->SetCellValue(4, 31, strYUnit);
	m_pXL->SetCellValue(8, 31, strXUnit);

	if (CStatus::Instance()->GetCoordinateSystem() == CStatus::CS_CARTESIAN)
	{
		m_pXL->SetCellValue(2, 30, _T("Roll"));
		m_pXL->SetCellValue(4, 30, _T("Pitch"));
		//m_pXL->SetCellValue(8, 29, _T("Go/No Go"));
	}
	else
	{
		m_pXL->SetCellValue(2, 30, _T("Magnitude"));
		m_pXL->SetCellValue(5, 30, _T("Direction"));
		//m_pXL->SetCellValue(8, 29, _T("Go/No Go"));
	}
	for (int i = 0; i < m_LastNumber; i++)
	{
		BOOL bRel = false;
		BOOL bIsStable = false;
		double dblRawRoll = m_excelRecords[i].ALMDRecord.sRawRoll / 1000.;
		double dblRawPitch = m_excelRecords[i].ALMDRecord.sRawPitch / 1000.;
		if (m_excelRecords[i].ALMDRecord.bRel)
		{
			bRel = true;
			dblRawRoll -= m_excelRecords[i].ALMDRecord.dblRefRoll;
			dblRawPitch -= m_excelRecords[i].ALMDRecord.dblRefPitch;
		}

		bIsStable = m_excelRecords[i].ALMDRecord.bStablization;
		CUnitConversion::Instance()->SetRollAndPitch(dblRawRoll, dblRawPitch);
		double dblStateX, dblStateY, dblGoNoGo= m_excelRecords[i].ALMDRecord.dblGoNoGoValue;
		CString strStateX, strStateY, strGoNoGo;
		CString strIsRelative, strIsStable;
		CString strGoNoGoUnit;
		double dblDispTolerance;
		CUnitConversion::Instance()->GetStateValue(dblStateX, dblStateY);
		CUnitConversion::Instance()->ConvertToDest(CStatus::Instance()->GetCoordinateSystem(), CStatus::Instance()->GetAngleType(), (CStatus::tIncType)(CStatus::Instance()->GetInclinationType()), CStatus::Instance()->GetRORType(), &strGoNoGoUnit, &dblDispTolerance, dblGoNoGo);
		if (CStatus::Instance()->IsDigit2Mode())
		{
			strStateX.Format(_T("%5.2f"), dblStateX);
			strStateY.Format(_T("%5.2f"), dblStateY);
			strGoNoGo.Format(_T("%5.2f"), dblDispTolerance);
		}
		else
		{
			strStateX.Format(_T("%5.3f"), dblStateX);
			strStateY.Format(_T("%5.3f"), dblStateY);
			strGoNoGo.Format(_T("%5.3f"), dblDispTolerance);
		}
		if (bRel)strIsRelative.Format(_T("Relative"));
		else strIsRelative.Format(_T("Absolute"));
		if (bIsStable)strIsStable.Format(_T("Stable"));
		else strIsStable.Format(_T("Unstable"));


		m_pXL->SetCellValue(2, 32 + i, strStateX);
		m_pXL->SetCellValue(4, 32 + i, strStateY);
		m_pXL->SetCellValue(6, 32 + i, strIsRelative);
		m_pXL->SetCellValue(7, 32 + i, strIsStable);
		m_pXL->SetCellValue(8, 32 + i, strGoNoGo);
	}

	return 0;
}


int CExcelReport::RemoveDummy()
{

	int toDeleteLine = 7 - m_LastNumber;
	for (int i = 0; i < toDeleteLine; i++)
	{
		m_pXL->DeleteRow(33 + m_LastNumber);
	}

	for (int i = 0; i < toDeleteLine*3; i++)
	{
		m_pXL->DeleteRow(7 + m_LastNumber*3);
	}

	return 0;
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
		if (strTemp == _T("EXCEL.EXE"))
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
