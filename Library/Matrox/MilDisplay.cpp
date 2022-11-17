#include "StdAfx.h"
#include "MilClass.h"
#include "MilDisplay.h"


CMilDisplay::CMilDisplay(void)
{
	m_milIDDisplay=NULL;
}


CMilDisplay::CMilDisplay(CMilApplication * pMilApplication, CMilSystem * pMilSystem)
{
	m_milIDApplication = pMilApplication->m_milIDApplication;
	m_milIDSystem = pMilSystem->m_milIDSystem;
}

CMilDisplay::~CMilDisplay(void)
{
	Close();
}

int CMilDisplay::Create(void)
{
	return (int)MdispAlloc(m_milIDSystem, M_DEFAULT, L"M_DEFAULT", M_DEFAULT, &m_milIDDisplay);
}

int CMilDisplay::Create(CMilApplication * pMilApplication, CMilSystem *pMilSystem)
{
	m_milIDApplication = pMilApplication->m_milIDApplication;
	m_milIDSystem = pMilSystem->m_milIDSystem;

	return Create();
}

int CMilDisplay::Close(void)
{
	if (m_milIDGraph != M_NULL)
	{
		CMilGraph::Free();
	}
	if(m_milIDDisplay!=M_NULL)
	{
		MdispFree(m_milIDDisplay);
		m_milIDDisplay=M_NULL;
	}
	return 0;
}

void CMilDisplay::Control(MIL_INT ctrlType, MIL_DOUBLE ctrlValue)
{
	MdispControl(m_milIDDisplay, ctrlType, ctrlValue);
}

int CMilDisplay::GetDisplayID(void)
{
	return (int)m_milIDDisplay;
}

int CMilDisplay::EnableOverlay(BOOL bState)
{
	if (bState)
	{
		MdispControl(m_milIDDisplay, M_OVERLAY, M_ENABLE);
		return (int)MdispInquire(m_milIDDisplay, M_OVERLAY_ID, &m_milIDBuffer);
	}
	else
	{
		MdispControl(m_milIDDisplay, M_OVERLAY, M_DISABLE);
		return (int)MdispInquire(m_milIDDisplay, M_OVERLAY_ID, M_NULL);
	}
}

int CMilDisplay::GetIDOverlay(void)
{
	return (int)m_milIDBuffer;
}

BOOL CMilDisplay::IsSelected(void)
{
	return MdispInquire(m_milIDDisplay, M_SELECTED, M_NULL);
}

void CMilDisplay::UnSelectDisp(void)
{
	MdispSelect(m_milIDDisplay, NULL);
}

int CMilDisplay::ClearOverlay(CMilDisplay* pMilDisplay)
{
	MdispControl(pMilDisplay->m_milIDDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
	return 0;
}

int CMilDisplay::ClearOverlay(void)
{
	MdispControl(m_milIDDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
	return 0;
}

int CMilDisplay::SetViewShift(int vShift)
{
	MdispControl(m_milIDDisplay, M_VIEW_MODE, M_BIT_SHIFT);
	MdispControl(m_milIDDisplay, M_VIEW_BIT_SHIFT, vShift);
	return 0;
}

int CMilDisplay::SetViewAutoScale(BOOL bState)
{
	if (bState)	MdispControl(m_milIDDisplay, M_VIEW_MODE, M_AUTO_SCALE);
	else MdispControl(m_milIDDisplay, M_VIEW_MODE, M_TRANSPARENT);
	return 0;
}

int CMilDisplay::SetZoomFactor(double XFactor, double YFactor)
{
	MdispZoom(m_milIDDisplay, XFactor, YFactor);
	return 0;
}

int CMilDisplay::SetPanFactor(double XOffset, double YOffset)
{
	MdispZoom(m_milIDDisplay, XOffset, YOffset);
	return 0;
}

UINT CMilDisplay::GetTransparencyColor(void)
{
	MIL_INT nColor=0;
	MdispInquire(m_milIDDisplay, M_TRANSPARENT_COLOR, &nColor);
	return nColor & 0x0ffffff;
}

void CMilDisplay::FilWindow(bool bStatus)
{
	if (bStatus)
		Control(M_SCALE_DISPLAY, M_ENABLE);
	else
		Control(M_SCALE_DISPLAY, M_DISABLE);
}

void CMilDisplay::DispCenter(bool bStatus)
{
	if (bStatus)
		Control(M_CENTER_DISPLAY, M_ENABLE);
	else
		Control(M_CENTER_DISPLAY, M_DISABLE);
}



CdblSize CMilDisplay::GetZoomFactor()
{
	double dblZoomFactorX = 1.0;
	double dblZoomFactorY = 1.0;
	MdispInquire(m_milIDDisplay, M_ZOOM_FACTOR_X, &dblZoomFactorX);
	MdispInquire(m_milIDDisplay, M_ZOOM_FACTOR_Y, &dblZoomFactorY);

	if (dblZoomFactorX < 0)dblZoomFactorX = 1.0;
	if (dblZoomFactorY < 0)dblZoomFactorY = 1.0;
	return CdblSize(dblZoomFactorY, dblZoomFactorY);
}


int CMilDisplay::GetMousePosImgXY(CdblPoint * pDblPoint)
{
	//Hook해야만 사용가능
	MIL_DOUBLE posX, posY;
	MdispGetHookInfo(m_milIDDisplay, M_MOUSE_POSITION_BUFFER_X, &posX);
	MdispGetHookInfo(m_milIDDisplay, M_MOUSE_POSITION_BUFFER_Y, &posY);
	pDblPoint->dblPosX = posX;
	pDblPoint->dblPosY = posY;
	return 0;
}

