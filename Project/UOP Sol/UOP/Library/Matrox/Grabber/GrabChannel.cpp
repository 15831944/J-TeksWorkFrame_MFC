#include "StdAfx.h"
#include "GrabChannel.h"

int CGrabChannel::m_bUseManHisto=FALSE;
int CGrabChannel::m_bUseHFilltero=FALSE;
long *CGrabChannel::m_pOrgHistoData=NULL;
long *CGrabChannel::m_pDestHistoData=NULL;
tWndLv CGrabChannel::m_WndLv;
BOOL CGrabChannel::m_bWarpCal=FALSE;
BOOL CGrabChannel::m_bScrnNormalize=FALSE;
CMilBuffer * CGrabChannel::m_pGrabBuffer = NULL;
CMilBuffer * CGrabChannel::m_pfillterBuffer1 = NULL;
CMilBuffer * CGrabChannel::m_pfillterBuffer2 = NULL;
CMilBuffer * CGrabChannel::m_pfillterBuffer3 = NULL;
CMilBuffer * CGrabChannel::m_pfillterBuffer4 = NULL;
CMilBuffer * CGrabChannel::m_pfillterBuffer5 = NULL;
CMilBuffer * CGrabChannel::m_pfillterBuffer6 = NULL;
CMilBuffer * CGrabChannel::m_pfillterBuffer7 = NULL;
CMilBuffer * CGrabChannel::m_pfillterBuffer8 = NULL;
CMilBuffer * CGrabChannel::m_pfillterBuffer9 = NULL;
CMilBuffer * CGrabChannel::m_pfillterBuffer10 = NULL;


long CGrabChannel::m_lExposeTime=0;
CMilCal	*CGrabChannel::m_pMilWarpCal=NULL;
BOOL CGrabChannel::m_bInlineAvgEnable =FALSE;

CGrabChannel::CGrabChannel(void)
: m_strCameraFile(_T(""))
, m_nChannel(0)
, m_iAverageCounter(3)
, m_bImgEnhance(FALSE)
{
	m_bnState.bBrightNormalize=FALSE;
	m_bnState.bCutHigh=TRUE;
	m_bnState.bCutLow=TRUE;
	m_pMilWarpCal = new CMilCal;
}

CGrabChannel::~CGrabChannel(void)
{
	delete m_pMilWarpCal;
}


int CGrabChannel::Create(void)
{
	m_pMilBuffer->SetDepth(8+M_UNSIGNED);
	m_pMilBuffer->SetAttribute(M_IMAGE+M_DISP+M_PROC+M_DIB);
//jubatemp 4/22
//	m_pMilBuffer->Alloc2D(CSize(640,480));
	m_pMilBuffer->Alloc2D(CSize(1000,1000));
	m_pImageDark = new CMilBuffer;
	m_pImageWhite = new CMilBuffer;
	m_WndLv.OrgStart=0;
	m_WndLv.OrgEnd=255;
	m_WndLv.DestStart=0;
	m_WndLv.DestEnd=255;
	m_bUseManHisto=FALSE;
	m_bUseHFilltero=FALSE;
	m_bnState.bBrightNormalize=FALSE;
	m_bnState.bCutHigh=TRUE;
	m_bnState.bCutLow=TRUE;
	m_bnState.uiBNLimit=900;
	return 0;
}

int CGrabChannel::GetResolution(CSize& Rect)
{
	//jubatemp
	Rect.cx=1000;
	Rect.cy=1000;
	return 0;
}

int CGrabChannel::SetExposeTime(long epTime)
{
	m_lExposeTime=epTime;
	return 0;
}

int CGrabChannel::GetExposeTime()
{
    return m_lExposeTime;
}
int CGrabChannel::SetBuffer(CMilBuffer * pMilBuffer)
{
	m_pMilBuffer=pMilBuffer;
	return 0;
}

int CGrabChannel::StopGrab(void)
{
	return 0;
}

int CGrabChannel::Grab(void)
{
	return 0;
}

int CGrabChannel::GrabContinuousStop(void)
{
   return 0;
}

int CGrabChannel::GrabContinuous(void)
{
	return 0;
}

int CGrabChannel::GrabTrigger(void)
{
	return 0;
}


int CGrabChannel::SingleGrab(void)
{
	return 0;
}

int CGrabChannel::SetupTrigger(BOOL OnOff)
{
 return 0;
}


int CGrabChannel::StartAvreage(BOOL GRAB)
{
  
	return 0 ;
}
int CGrabChannel::StopAvreage(BOOL GRAB)
{
   return 0;
}

int CGrabChannel::Close(void)
{
	return 0;
}

CMilBuffer* CGrabChannel::GetBuffer(void)
{
	return m_pMilBuffer;
}


int CGrabChannel::SetAverageCounter(int nCounter)
{
	m_iAverageCounter=nCounter;
	
	return 0;
}

int  CGrabChannel::SetAverageCounterSoft(int nCounter)
{
		m_iAverageCounterSoft = nCounter;
	return 0;
}

int  CGrabChannel::SetHRSCLevel(int nLevel)
{
		m_iHRSCLevel = nLevel;
	return 0;
}


int CGrabChannel::GetAverageCounter(void)
{    

	return m_iAverageCounter;
}

int CGrabChannel::GetAverageCounterSoft(void)
{
   return  m_iAverageCounterSoft;
}
int CGrabChannel::GetHRSCLevel(void)
{
   return  m_iHRSCLevel;
}

void CGrabChannel::SetImgEnhance(BOOL bStatus)
{
	m_bImgEnhance = bStatus;
}

BOOL CGrabChannel::GetImgEnhance(void)
{
	return m_bImgEnhance;
}

void CGrabChannel::SetInlineAvgEnable(BOOL bValue)
{
	m_bInlineAvgEnable=bValue;
}
BOOL CGrabChannel::GetInlineAvgEnable(void)
{
	return m_bInlineAvgEnable;
}


void CGrabChannel::SetBrightNormalize(tBNState bnStatus)
{
	m_bnState = bnStatus;
}

tBNState CGrabChannel::GetBrightNormalize(void)
{
	return m_bnState;
}

void CGrabChannel::SetWarpCal(BOOL bState)
{
	if(bState)m_pMilWarpCal->RestoreCalData(CAppInfo::GetAppDir()+_T("\\..\\Setup\\")+CAL_FILE);
	m_bWarpCal = bState;
}

BOOL CGrabChannel::GetWarpCal(void)
{
	return m_bWarpCal;
}

void CGrabChannel::SetScrnNormalize(BOOL bState)
{
	m_bScrnNormalize = bState;
}

BOOL CGrabChannel::GetScrnNormalize(void)
{
	return m_bScrnNormalize;
}

int CGrabChannel::GetDepth(void)
{
	return 8;
}

long * CGrabChannel::GetOrgHistoBuff(void)
{
	return m_pOrgHistoData;
}

long * CGrabChannel::GetDestHistoBuff(void)
{
	return m_pDestHistoData;
}

BOOL CGrabChannel::IsManHistogram(void)
{
	return m_bUseManHisto;
}
BOOL CGrabChannel::IsManHFiltero(void)
{
	return m_bUseHFilltero;
}

void CGrabChannel::SetManHistoMode(BOOL state)
{
	m_bUseManHisto=state;
}

void CGrabChannel::SetHFiltero(BOOL state)
{
	m_bUseHFilltero=state;
}

void CGrabChannel::SetInputRange(long lStart, long lEnd)
{
	m_WndLv.OrgStart=lStart;
	m_WndLv.OrgEnd=lEnd;
}

int CGrabChannel::OnTrigger(BOOL On)
{
  return 0;
}

int CGrabChannel::RotateImage(BOOL value)
{
  return 0;

}

int CGrabChannel::HorizonMirr(BOOL value)
{
 return 0;
}
int CGrabChannel::VerticalMirr(BOOL value)
{
 return 0;
}
BOOL CGrabChannel::IsGrabDone(void)
{
	return TRUE;
}




