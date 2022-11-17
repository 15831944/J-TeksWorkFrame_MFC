#include "StdAfx.h"
#include "MilVLib.h"
#include "MilClass.h"

CMilVLib* CMilVLib::m_pInstance=NULL;
CString		CMilVLib::m_strError;
int			CMilVLib::m_nPixelMapEntries;
CString		CMilVLib::m_strPixelMapFilePath;

int**	CMilVLib::m_pPointDefect;
int**	CMilVLib::m_pColDefect;
int**	CMilVLib::m_pRowDefect;

int			CMilVLib::m_nPointDefectCnt;
int			CMilVLib::m_nColDefectCnt;
int			CMilVLib::m_nRowDefectCnt;

CMilVLib::CMilVLib(void)
{
	m_pMilApplication = NULL;
	m_pMilDisplay=NULL;
	m_currDigiNum=0;
	m_nDigitizer=0;
	
}

CMilVLib::~CMilVLib(void)
{
	Close();
}

CMilVLib * CMilVLib::Instance(void)
{
	if(m_pInstance==NULL)m_pInstance=new CMilVLib;
	return m_pInstance;
}

/*!
	@fn int CMilVLib::DefaultSetting(void)
	@return 0:success 0>:fail -1:Cann't Create Handle -2:Internal Program -3:DongleKey not found  
*/
int CMilVLib::DefaultSetting(void)
{
	m_pMilApplication = new CMilApplication;
	if ( m_pMilApplication->Create() )return -1;

	if (CMilSystem::Instance()->Create(m_pMilApplication) ) return - 2;
	m_pMilDisplay = new CMilDisplay;
	if (m_pMilDisplay->Create(m_pMilApplication, CMilSystem::Instance()) == NULL)return -2;

	if(CMilSystem::Instance()->GetDongleType()==0)return -3;

	return 0;
}

int CMilVLib::Setting(TCHAR* szGrabber, TCHAR * camFile)
{
	int rv=0;
	m_pMilApplication = new CMilApplication;
	m_pMilApplication->Create();

	CMilSystem::Instance(szGrabber)->Create(m_pMilApplication);
	if(CMilSystem::Instance()->GetDongleType()==0)return -1;
	m_pMilDisplay = new CMilDisplay;
	m_pMilDisplay->Create(m_pMilApplication, CMilSystem::Instance());

	int setDigitizer=m_nDigitizer++;
	m_pMilDigitizer[setDigitizer] = CMilSystem::Instance(szGrabber)->NewDigitizer(camFile);
	m_pMilDigitizer[setDigitizer]->SetBuffer(NewImage());
	rv=m_pMilDigitizer[setDigitizer]->Create(m_pMilApplication, CMilSystem::Instance(szGrabber));
	if(rv)return rv;

	return 0;
}

int CMilVLib::AddCamera(TCHAR* szGrabber, TCHAR * camFile)
{
	///나중에 시스템에 따라 카메라를 추가한다 현재는 보정마크용카메라만 처리함
	int rv=0;
	CMilSystem::Instance(szGrabber)->Create(m_pMilApplication);

	int setDigitizer=m_nDigitizer++;
	m_pMilDigitizer[setDigitizer] = CMilSystem::Instance(szGrabber)->NewDigitizer(camFile);
	m_pMilDigitizer[setDigitizer]->SetBuffer(NewImage());
	rv=m_pMilDigitizer[setDigitizer]->Create(m_pMilApplication, CMilSystem::Instance(szGrabber));
	if(rv)return rv;
	return 0;
}

void CMilVLib::Close(void)
{
	if (m_pMilDisplay)
	{
		delete m_pMilDisplay;
		m_pMilDisplay=NULL;
	}

	CMilSystem::Instance()->Close();
	if (m_pMilApplication)
		delete m_pMilApplication;

	m_pMilApplication = NULL;
}


CMilBuffer* CMilVLib::NewImage(void)
{
	CMilBuffer *pMilBuffer=new CMilBuffer;
	pMilBuffer->SetDisplay(m_pMilDisplay);

	pMilBuffer->SetAttribute(M_IMAGE+M_PROC+M_DISP);
	pMilBuffer->m_milIDSystem = CMilSystem::Instance()->m_milIDSystem;
	pMilBuffer->m_milIDApplication = CMilSystem::Instance()->m_milIDApplication;
	return pMilBuffer;
}

CMilBuffer* CMilVLib::NewImage(CMilBuffer *pOrgBuff)
{
	tImageProperties orgProperty;
	CMilBuffer *pMilBuffer = new CMilBuffer;
	try
	{
		if (!pOrgBuff->GetIDBuffer())throw ;
	}catch(...)
	{
		AfxMessageBox(_T("pOrgBuffer is not allocated"));
		return NULL;
	}
	pMilBuffer->SetDisplay(m_pMilDisplay);
	pOrgBuff->GetImageProperties(&orgProperty);
	pMilBuffer->SetImageProperties(&orgProperty);
	pMilBuffer->SetAttribute(M_IMAGE + M_PROC + M_DISP);
	pMilBuffer->m_milIDSystem = CMilSystem::Instance()->m_milIDSystem;
	pMilBuffer->m_milIDApplication = CMilSystem::Instance()->m_milIDApplication;
	return pMilBuffer;
}

CMilDisplay* CMilVLib::NewDisplay(void)
{
	CMilDisplay *pMilDisplay;
	pMilDisplay = new CMilDisplay;
	pMilDisplay->Create(m_pMilApplication, CMilSystem::Instance());
	return pMilDisplay;
}

CMilDigitizer* CMilVLib::NewDigitizer(int tGrabber)
{
	CMilDigitizer *pMilDigitizer;
	pMilDigitizer = new CMilDigitizer;
	pMilDigitizer->Create(m_pMilApplication, CMilSystem::Instance());
	return NULL;
}

CMilBlob* CMilVLib::NewBlobTool(CMilBuffer *pMilBuffer)
{
	return new CMilBlob(pMilBuffer);
}

CMilModFinder* CMilVLib::NewModFinderTool(void)
{
	return new CMilModFinder;
}

CMilEdgeFinder* CMilVLib::NewEdgeFinderTool(void)
{
	return new CMilEdgeFinder;
}

CMilPat* CMilVLib::NewPatTool(void)
{
	return new CMilPat;
}

CMilDisplay * CMilVLib::GetDefaultDisp(void)
{

	return m_pMilDisplay;
}

void CMilVLib::SetDefDigitizer(int nDigitizer)
{
	m_currDigiNum=nDigitizer;
}

int CMilVLib::GrabContinuous(void)
{
	return m_pMilDigitizer[m_currDigiNum]->SetContinuous();
}

int CMilVLib::SingleGrabTrigger(void)
{

	return m_pMilDigitizer[m_currDigiNum]->GrabTrigger();

}
int CMilVLib::SingleGrab(void)
{
	return m_pMilDigitizer[m_currDigiNum]->Grab();
}

int CMilVLib::SetupTrigger(BOOL OnOff)
{

   return m_pMilDigitizer[m_currDigiNum]->SetupTrigger(OnOff);
}

int CMilVLib::StopGrab(void)
{
	return m_pMilDigitizer[m_currDigiNum]->StopGrab();

}

int CMilVLib::StopGrabContinuous(void)
{
  
   return  m_pMilDigitizer[m_currDigiNum]->StopGrabContinuous();
}

void CMilVLib::MilSaveImage(CString DIR)
{    
	wchar_t *WDIR = (wchar_t *)DIR.GetBuffer();
	m_pMilBuffer->Export(WDIR);
}

CMilDigitizer* CMilVLib::GetDigitizer(int iDigitizer)
{
	return m_pMilDigitizer[iDigitizer];
}


int CMilVLib::SetAverageCounter(int nCount)
{
	GetDigitizer(0)->m_pGrabChannel->SetAverageCounter(nCount);
	return 0;
}

int CMilVLib::GetAverageCounter(void)
{
	return GetDigitizer(0)->m_pGrabChannel->GetAverageCounter();
}



int CMilVLib::SetAverageCounterSoft(int nCount)
{
	GetDigitizer(0)->m_pGrabChannel->SetAverageCounter(nCount);
	return 0;
}


int CMilVLib::GetHRSCLevel(int nLevel)
{
	GetDigitizer(0)->m_pGrabChannel->GetHRSCLevel();
	return 0;
}


int CMilVLib::GetAverageCounterSoft(void)
{
	return GetDigitizer(0)->m_pGrabChannel->GetAverageCounterSoft();
}
int CMilVLib::GetHRSCLevel(void)
{
	return GetDigitizer(0)->m_pGrabChannel->GetHRSCLevel();
   
}


int CMilVLib::SetImgEnhance(BOOL bStatus)
{
	GetDigitizer(0)->m_pGrabChannel->SetImgEnhance(bStatus);

	return 0;
}

int CMilVLib::GetImgEnhance(void)
{
	return 	GetDigitizer(0)->m_pGrabChannel->GetImgEnhance();
}

BOOL CMilVLib::IsGrabDone(void)
{
	return 	GetDigitizer(0)->m_pGrabChannel->IsGrabDone();
}

int CMilVLib::ScrnNormalize(CMilBuffer *pMilResult, CMilBuffer *pMilOrg, CMilBuffer *pMilWhite, CMilBuffer *pMilDark, int MaxWhiteValue)
{
	CMilBuffer *pTemp1;
	CMilBuffer  *pTemp2;

	int OrgDepth=0;
	tImageProperties ImageProperty;
	pMilOrg->GetImageProperties(&ImageProperty);

	OrgDepth=ImageProperty.Depth;

	if(pMilWhite!=NULL)
		if(OrgDepth!=pMilWhite->GetDepth())
		{
			return -1;
		}

		if(OrgDepth!=pMilDark->GetDepth())
		{
			return -2;
		}

		pTemp1 = new CMilBuffer;
		pTemp1->SetDepth(32);
		pTemp1->SetAttribute(M_IMAGE+M_DISP+M_PROC);
		pTemp1->Alloc2D(ImageProperty.ImageSize);

		pTemp2 = new CMilBuffer;
		pTemp2->SetDepth(32);
		pTemp2->SetAttribute(M_IMAGE+M_DISP+M_PROC);
		pTemp2->Alloc2D(ImageProperty.ImageSize);

		/// Imax*((Io-Id)/(Iw-id))
		if(pMilWhite!=NULL)
		{
			CMilIm::Arith(pTemp1, pMilOrg, pMilDark, M_SUB+M_SATURATION);
			CMilIm::ArithConst(pTemp1, pTemp1, MaxWhiteValue, M_MULT_CONST);
			CMilIm::Arith(pTemp2, pMilWhite, pMilDark, M_SUB+M_SATURATION);
			CMilIm::Arith(pMilResult, pTemp1, pTemp2, M_DIV);
		}else
			CMilIm::Arith(pMilResult, pMilOrg, pMilDark, M_SUB+M_SATURATION);

		pTemp1->Close();
		pTemp2->Close();
		return 0;
}

int CMilVLib::SetAverageStatus(BOOL status)
{
 	return m_pMilDigitizer[0]->m_pGrabChannel->StopAvreage(status);
}
CMilBuffer* CMilVLib::GetGrabBuffer(void)
{
	return m_pMilDigitizer[m_currDigiNum]->GetGrabBuffer();
}

int CMilVLib::MakeCombineImage(CString strFileName, CMilBuffer *pMilSrc, int FileType)
{
	MIL_ID colorBuffer;
	MIL_UINT32 PixelToGet[1];

	tImageProperties ImgProperty;
	pMilSrc->GetImageProperties(&ImgProperty);
	CSize ImgSize=ImgProperty.ImageSize;
	
	CMilBuffer newImage;
	pMilSrc->ConvertToBMPFmt(&newImage);

	MbufAllocColor(newImage.m_milIDSystem, 3, ImgSize.cx,ImgSize.cy, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &colorBuffer);
	MbufCopy(newImage.GetIDBuffer(), colorBuffer);
	MIL_ID milOverlay=pMilSrc->GetDisplay()->GetIDOverlay();

	UINT TranColor= pMilSrc->GetDisplay()->GetTransparencyColor();
	for(int i=0;i<ImgSize.cy;i++)
		for(int j=0;j<ImgSize.cx;j++)
		{
			MbufGetColor2d(milOverlay, M_PLANAR, M_ALL_BANDS,i,j,1,1, PixelToGet);
			PixelToGet[0]&=0x0fff;
			if(PixelToGet[0]==TranColor)continue;
			MbufPutColor2d(colorBuffer, M_PLANAR, M_ALL_BANDS, i,j, 1,1, PixelToGet);
		}

	MbufExport(strFileName.GetBuffer(), FileType, colorBuffer);
	newImage.Close();
	MbufFree(colorBuffer);
	return 0;
}
