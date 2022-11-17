#include "..\stdafx.h"
#ifdef _VIEWWORKS
#include "VWChannel.h"
#include "VWGrabber.h"
#include "ViVIX.T_def.h"
#include "ViVIX.T_API.h"
using namespace VWSDK;
using namespace VWSDK::T;
#include "..\Matrox.h"


/////////////////////////////////////////////////////////////////////////////
// GlobalCallback
void VWCallback(int nDetectorID, void * pUserClass, IMAGE_INFO_EX* pImageInfoEx)
{
	CVWChannel *pChannel = (CVWChannel *)pUserClass;
	pChannel->CallbackGrabEx(nDetectorID, pUserClass, pImageInfoEx);
}
/////////////////////////////////////////////////////////////////////////////
// Callback

CVWChannel::CVWChannel()
	: m_nChannel(0)
	, m_nDetectorID(0)
	, m_pMilBufDisp(NULL)
	, m_pMilBufProc(NULL)
	, m_bIsGrabDone(FALSE)
	, m_pMilBufView(NULL)
	, m_modeGrab(GRAB_STOP)
	, m_pRecMilBuffer(NULL)
	, m_pMilBufRec(NULL)
	, m_milIDBufRec(NULL)
	, m_wndLvlLow(0)
	, m_wnLvlHigh(65535)
	, m_bUseAutoScale(FALSE)
	, m_bImgEnhance(0)
	, m_bUseImgAvg(FALSE)
	, m_nAvgInputCnt(1)
	, m_bAvgIsDone(FALSE)
	, m_bPreAvgStatus(FALSE)
	, m_bSaveImg(FALSE)
{
}

CVWChannel::~CVWChannel()
{
}

int CVWChannel::Grab()
{
	m_modeGrab = GRAB_SINGLE;
	return 0;
}


int CVWChannel::GrabStop()
{
	m_modeGrab = GRAB_STOP;
	return 0;
}


int CVWChannel::SetMilBuffer(CMilBuffer * pMilBufProc, CMilBuffer * pMilBufDisp, CMilBuffer *pMilBufView, CMilBuffer *pMilBufRec)
{
	//! H/W --> m_pMilBufProc --> m_pMilBufDisp --> m_pMilBufView
	m_pMilBufProc = pMilBufProc;
	m_pMilBufDisp = pMilBufDisp;
	m_pMilBufView = pMilBufView;
	m_pMilBufRec = pMilBufRec;

	m_milIDApplication = m_pMilBufProc->m_milIDApplication;;
	m_milIDSystem = m_pMilBufProc->m_milIDSystem;
	return 0;
}

int CVWChannel::SetMilBufferWithAvg(CMilBuffer * pMilBufProc, CMilBuffer * pMilBufDisp, CMilBuffer *pMilBufView, CMilBuffer *pMilBufRec,
	CMilBuffer *pMilBufAvg, CMilBuffer *pMilBufAvgResult, CMilBuffer * pMilBufAvgResultHisto,
	CMilBuffer * pMilTempBufAvgResult, CMilBuffer * pMilTempBufAvgResultHisto)
{
	SetMilBuffer(pMilBufProc, pMilBufDisp, pMilBufView, pMilBufRec);

	m_pMilBufAvgAdd = pMilBufAvg;
	m_pMilBufAvgResult = pMilBufAvgResult;
	m_pMilCamAvgResult_Histo = pMilBufAvgResultHisto;

	m_pMilTempBufAvgResult = pMilTempBufAvgResult;
	m_pMilTempBufAvgResult_histo = pMilTempBufAvgResultHisto;

	return 0;
}

BOOL CVWChannel::IsGrabDone()
{
	return 0;
}


void CVWChannel::SetExpTime(int nExpTime)
{

}


int CVWChannel::Init()
{
	CWaitCursor oWaitCursor;
	int nDeviceIndex = m_nChannel;
	OutputDebugString(_T("\nViewWork Channel Init"));
	RESULT	result = RESULT_ERROR;
	result = VwOpenDetector(nDeviceIndex, &m_nDetectorID);
	if (result != RESULT_SUCCESS)
	{
		switch (result)
		{
		default:
		{
			AfxMessageBox(_T("ERROR : Default error code returned"));
		}
		break;
		case RESULT_ERROR_DEVCREATEDATASTREAM:
		{
			AfxMessageBox(_T("ERROR : RESULT_ERROR_DEVCREATESTREAM was returned"));
		}
		break;
		case RESULT_ERROR_NO_CAMERAS:
		{
			AfxMessageBox(_T("ERROR : RESULT_ERROR_NO_CAMERAS was returned. Please check the NIC properties"));
		}
		break;
		case RESULT_ERROR_VWCAMERA_CAMERAINDEX_OVER:
		{
			AfxMessageBox(_T("ERROR : RESULT_ERROR_VWCAMERA_CAMERAINDEX_OVER was returned. Please check Zero-based camera index."));
		}
		break;
		case RESULT_ERROR_DATASTREAM_MTU:
		{
			AfxMessageBox(_T("ERROR : RESULT_ERROR_STREAM_MTU was returned. Please check NIC MTU."));
		}
		break;
		case RESULT_ERROR_INSUFFICIENT_RESOURCES:
		{
			AfxMessageBox(_T("ERROR : RESULT_ERROR_BUFFER_TOO_SMALL was returned. Please check system resources."));
		}
		break;
		case RESULT_ERROR_MEMORY_ALLOCATION:
		{
			AfxMessageBox(_T("ERROR : RESULT_ERROR_MEMORY_ALLOCATION was returned. Please check system resources."));
		}
		break;
		}

		return -1;
	}

	int SizeX = 1280;
	int SizeY = 1280;
	MbufAlloc2d(m_milIDSystem, SizeX, SizeY, m_pMilBufProc->m_iDepth, m_pMilBufProc->m_BufAttr, &m_milIDBufProc);
	MbufAlloc2d(m_milIDSystem, SizeX, SizeY, m_pMilBufAvgAdd->m_iDepth, m_pMilBufAvgAdd->m_BufAttr, &m_milIDBufAvgAdd); //kiy
	MbufAlloc2d(m_milIDSystem, SizeX, SizeY, m_pMilBufAvgResult->m_iDepth, m_pMilBufAvgResult->m_BufAttr, &m_milIDBufAvgResult); //kiy
	MbufAlloc2d(m_milIDSystem, SizeX, SizeY, m_pMilCamAvgResult_Histo->m_iDepth, m_pMilCamAvgResult_Histo->m_BufAttr, &m_milIDBufAvgResult_Histo); //kiy

	MbufAlloc2d(m_milIDSystem, SizeX, SizeY, m_pMilTempBufAvgResult->m_iDepth, m_pMilTempBufAvgResult->m_BufAttr, &m_milIDBufTempAvtResult); //kiy
	MbufAlloc2d(m_milIDSystem, SizeX, SizeY, m_pMilTempBufAvgResult_histo->m_iDepth, m_pMilTempBufAvgResult_histo->m_BufAttr, &m_milIDBufTempAvtResult_histo); //kiy


	//kiy M_PROC
	MbufAllocColor(m_milIDSystem, 3, SizeX, SizeY, m_pMilBufDisp->GetDepth(), M_IMAGE + M_DISP + M_PROC, &m_milIDBufDisp);
	MbufAllocColor(m_milIDSystem, 3, SizeX, SizeY, m_pMilBufRec->GetDepth(), m_pMilBufRec->GetAttribute(), &m_milIDBufRec);

	m_pMilBufProc->SetIDBuffer((int)m_milIDBufProc);
	m_pMilBufDisp->SetIDBuffer((int)m_milIDBufDisp);
	m_pMilBufRec->SetIDBuffer((int)m_milIDBufRec);

	m_pMilBufAvgAdd->SetIDBuffer((int)m_milIDBufAvgAdd); //kiy
	m_pMilBufAvgResult->SetIDBuffer((int)m_milIDBufAvgResult); //kiy
	m_pMilCamAvgResult_Histo->SetIDBuffer((int)m_milIDBufAvgResult_Histo); //kiy

	m_pMilTempBufAvgResult->SetIDBuffer((int)m_milIDBufTempAvtResult); //kiy
	m_pMilTempBufAvgResult_histo->SetIDBuffer((int)m_milIDBufTempAvtResult_histo); //kiy

	UpdateDriveModeInfo();

	VwSetCallbackFnImageInEx(m_nDetectorID, VWCallback, this);

	return 0;
}


void CVWChannel::UpdateDriveModeInfo()
{
	RESULT res;
	int nDriveModeNum;
	DRIVE_MODE_NAME_SET* pDriveModeNameSet;
	CString strDriveModeName;

	// Get list of Drive mode's name 
	res = VwGetDriveModeNameSet(m_nDetectorID, &nDriveModeNum, &pDriveModeNameSet);
	if (RESULT_SUCCESS != res)
	{
		//AfxMessageBox( _T("Fail to get Drive mode info.") );
		return;
	}

	//	m_cbDriveMode.ResetContent();
	for (int i = 0; i < nDriveModeNum; i++)
	{
		strDriveModeName.Format(_T("%d - "), i);
		strDriveModeName += CString(pDriveModeNameSet[i].szName, 20);
		//		m_cbDriveMode.InsertString(i, strDriveModeName);
	}

	// Get current drive mode
	int nDriveMode;
	res = VwGetDriveMode(m_nDetectorID, &nDriveMode);
	if (RESULT_SUCCESS != res)
	{
		//AfxMessageBox( _T("Fail to get current Drive mode.") );
		return;
	}

	//	m_cbDriveMode.SetCurSel(nDriveMode);

	return;
}


int CVWChannel::GrabContinuous()
{
	RESULT res;
	m_modeGrab = GRAB_CONTINUOUS;

	res = VwAcquisitionStart(m_nDetectorID);
	if (res != RESULT_SUCCESS)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("Fail to start acquisition : %s"), VwGetLastErrorMsg());
	}
	else
	{
		return -1;
		//GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		//m_cbDriveMode.EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_OFFSET_CAL)->EnableWindow(FALSE);
	}

	return 0;
}


void CVWChannel::Close()
{
	CWaitCursor oWaitCursor;
	RESULT res;

	if (VwIsGrabbing(m_nDetectorID))
	{
		VwAcquisitionStop(m_nDetectorID);
	}

	res = VwCloseDetector(m_nDetectorID);
	if (res != RESULT_SUCCESS)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("Fail to close the Detector : %s"), VwGetLastErrorMsg());
	}

	return;
}

void CVWChannel::CallbackGrabEx(int nDetectorID, void * pUserClass, void* pInfoEx)
{
	IMAGE_INFO_EX* pImageInfoEx = (IMAGE_INFO_EX*)pInfoEx;
	m_bIsGrabDone = FALSE;
	int nUsingBit = 16;

	// Check image error
	if (pImageInfoEx->callbackResult != RESULT_SUCCESS)
	{
		CString strError = VwGetLastErrorMsg();
		return;
	}

	// Validation check : Image size info
	switch (pImageInfoEx->pixelFormat)
	{
	case PIXEL_FORMAT_MONO8:
		nUsingBit = 8;
		break;
	case PIXEL_FORMAT_MONO12:
		nUsingBit = 12;
		break;
	default:
		nUsingBit = 16;
		break;
	}

	m_pMilBufProc->Put(pImageInfoEx->pImage);

	if (m_bImgEnhance)
	{
		CMilIm::Median(m_pMilBufProc, m_pMilBufProc);
		CMilIm::UserDefine1(m_pMilBufProc, m_pMilBufProc);
	}

	//kiy avg -----------------------------------------------
	//m_bUseImgAvg = FALSE;
	if (m_bUseImgAvg)
	{
		if (m_bSetAvg)
		{
			OutputDebugString(_T("\nm_bSetAvg"));
			m_bSetAvg = FALSE;
			m_nAvgCurCnt = 0;
			m_pMilBufAvgAdd->Clear(0L);
			m_pMilBufAvgResult->Clear(0L);
		}

		if (m_nAvgInputCnt > m_nAvgCurCnt)
		{
			m_nAvgCurCnt++;

			CMilIm::Arith(m_pMilBufAvgAdd, m_pMilBufProc, m_pMilBufAvgAdd, M_ADD + M_SATURATION);
			CMilIm::ArithConst(m_pMilBufAvgResult, m_pMilBufAvgAdd, (double)m_nAvgCurCnt, M_DIV_CONST + M_FLOAT_PROC);

			m_bCurAvgStatus = false;
		}
		else
		{
			double ratio = (m_nAvgInputCnt - 1) / (double)m_nAvgInputCnt;

			CMilIm::ArithConst(m_pMilBufAvgAdd, m_pMilBufAvgAdd, ratio, M_MULT_CONST + M_FLOAT_PROC);
			CMilIm::Arith(m_pMilBufAvgAdd, m_pMilBufProc, m_pMilBufAvgAdd, M_ADD + M_SATURATION);
			CMilIm::ArithConst(m_pMilBufAvgResult, m_pMilBufAvgAdd, (double)m_nAvgInputCnt, M_DIV_CONST + M_FLOAT_PROC);

			m_bCurAvgStatus = true;
			m_bSaveImg = true;
		}
	}
	//-------------------------------------------------------

	//AutoScale, manual 프로그램 재부팅 해야 작동
	if (m_bUseAutoScale)
	{
		long lGrayMax = 0, lGrayMin = 0;
		//CMilIm::GetMinMax(m_pMilBufProc, &lGrayMin, &lGrayMax);
		CMilIm::GetLimitedMinMax(m_pMilBufProc, 100, &lGrayMin, &lGrayMax, TRUE, FALSE);

		//kiy
		if (m_bUseImgAvg)
		{
			m_pMilBufAvgResult->WndLvlTranform(m_pMilCamAvgResult_Histo, lGrayMin, lGrayMax, 0, 65535);
			m_pMilBufAvgResult->WndLvlTranform(m_pMilBufDisp, lGrayMin, lGrayMax, 0, 255);

			m_pMilTempBufAvgResult_histo->Copy(m_pMilCamAvgResult_Histo);
			if (m_bSaveImg)
			{
				//OutputDebugString(_T("\nSaveImage"));
				m_pMilTempBufAvgResult->Copy(m_pMilBufAvgResult);
				m_pMilTempBufAvgResult_histo->Copy(m_pMilCamAvgResult_Histo);
				m_bSaveImg = false;
			}

			//190328 - Avg 완료 여부가 전과 다를때만 콜백 호출
			if (m_bPreAvgStatus != m_bCurAvgStatus)
				m_pfOnAvgStatus(m_bCurAvgStatus);

			m_bPreAvgStatus = m_bCurAvgStatus;
		}
		else
			m_pMilBufProc->WndLvlTranform(m_pMilBufDisp, lGrayMin, lGrayMax, 0, 255);
	}
	else
	{
		//kiy
		if (m_bUseImgAvg)
		{
			m_pMilBufAvgResult->WndLvlTranform(m_pMilCamAvgResult_Histo, m_wndLvlLow, m_wnLvlHigh, 0, 65535);
			m_pMilBufAvgResult->WndLvlTranform(m_pMilBufDisp, m_wndLvlLow, m_wnLvlHigh, 0, 255);

			if (m_bSaveImg)
			{
				//OutputDebugString(_T("\nm_bSaveImg"));
				m_pMilTempBufAvgResult->Copy(m_pMilBufAvgResult);
				m_pMilTempBufAvgResult_histo->Copy(m_pMilCamAvgResult_Histo);
				m_bSaveImg = false;
			}

			//190328 - Avg 완료 여부가 전과 다를때만 콜백 호출
			if (m_bPreAvgStatus != m_bCurAvgStatus)
				m_pfOnAvgStatus(m_bCurAvgStatus);

			m_bPreAvgStatus = m_bCurAvgStatus;
		}
		else
			m_pMilBufProc->WndLvlTranform(m_pMilBufDisp, m_wndLvlLow, m_wnLvlHigh, 0, 255);
	}

	if (m_modeGrab == GRAB_CONTINUOUS)
	{
		m_LegendUpdate();
		m_pMilBufView->Copy(m_pMilBufDisp);
		m_pMilBufRec->Copy(m_pMilBufDisp); //여기서 오래걸림, 28 프레임 안나오는 가장 큰 원인

		if (m_pRecMilBuffer)
			m_pRecMilBuffer->ExportAVI();
	}

	m_bIsGrabDone = TRUE;

	return;
}

void CVWChannel::SetRecMilBuffer(CMilBuffer *pRecMilBuffer)
{
	m_pRecMilBuffer = pRecMilBuffer;
}

void CVWChannel::SetAutoScale()
{
	m_bUseAutoScale = TRUE;
}

void CVWChannel::SetWndLvl(int iWndLvlLow, int iWndLvlHigh)
{
	m_wndLvlLow = iWndLvlLow;
	m_wnLvlHigh = iWndLvlHigh;
}

void CVWChannel::SetImgEnhance(bool bUseImgEnhance)
{
	m_bImgEnhance = bUseImgEnhance;
}

void CVWChannel::SetLegendCallBack(tFunction funcLegendUpdate)
{
	m_LegendUpdate = std::move(funcLegendUpdate);
}

//kiy - OPPANEL Select Device 선택용. GrabStop()쓰면 오류나서 따로 만듦
void CVWChannel::Stop()
{
	RESULT res;

	res = VwAcquisitionStop(m_nDetectorID);

	if (res != RESULT_SUCCESS)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("Fail to stop acquisition : %s"), VwGetLastErrorMsg());
	}

	return;
}

void CVWChannel::SetOnAvgStatusCallback(tfOnAvgStatus funcOnAvgStatus)
{
	m_pfOnAvgStatus = std::move(funcOnAvgStatus);
}


#endif