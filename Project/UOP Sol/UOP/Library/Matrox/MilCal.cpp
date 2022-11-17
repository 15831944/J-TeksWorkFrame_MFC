#include "StdAfx.h"
#include "MilCal.h"

CMilCal::CMilCal(void)
{
	m_milIDCal=NULL;
}

CMilCal::~CMilCal(void)
{
	Free();
}

/*! @fn int CMilCal::TeachCalParam(CMilBuffer *pCalImage, int RowCount, int ColCount)
	@brief 영상을 받아 Calibration Data를 만들어 낸다.
	@param pCalImage Calibration을 할 영상(ball 영상)
	@param RowCount 가로 방향의 볼갯수
	@param ColCount 세로 방향의 볼갯수
	@return 0:Calibration이 정상처리됨 -1:문제발생
*/
int CMilCal::TeachCalParam(CMilBuffer *pCalImage, int RowCount, int ColCount)
{
	MIL_INT CalStatus;
	Alloc();
	McalGrid(m_milIDCal, pCalImage->GetIDBuffer(), 0, 0, 0, RowCount, ColCount, 1, 1, M_DEFAULT, M_DEFAULT);

	McalInquire(m_milIDCal, M_CALIBRATION_STATUS + M_TYPE_MIL_INT, &CalStatus);

	if(CalStatus==M_CALIBRATED)
	{
		return 0;
	}else
	{
		return -1;
	}
}

/*! @fn int CMilCal::SaveCalData(WCHAR * strFileName)
	@brief Calibration 데이터를 저장한다.
	@param strFileName 데이터가 저장될 폴더를 포함한 파일이름
*/
int CMilCal::SaveCalData(CString strFileName)
{
	Alloc();
	McalSave(strFileName.GetBuffer(), m_milIDCal, M_DEFAULT);
	return 0;
}

int CMilCal::RestoreCalData(CString strFileName)
{
	if(m_milIDCal)Free();
	return McalRestore(strFileName.GetBuffer(), CMilSystem::Instance()->GetSystemID(), M_DEFAULT, &m_milIDCal);
}

int CMilCal::TransformImage(CMilBuffer * pImageDest, CMilBuffer * pImageSrc)
{
	if(!m_milIDCal)return -1;
	McalTransformImage(pImageSrc->GetIDBuffer(), pImageDest->GetIDBuffer(), m_milIDCal, M_BILINEAR + M_OVERSCAN_CLEAR, M_DEFAULT, M_DEFAULT);
	return 0;
}

int CMilCal::Alloc(MIL_ID * pMilIDCal)
{
	return McalAlloc(CMilSystem::Instance()->GetSystemID(), M_DEFAULT, M_DEFAULT, pMilIDCal);
}


int CMilCal::Alloc(void)
{
	if(!m_milIDCal)
		return McalAlloc(CMilSystem::Instance()->GetSystemID(), M_DEFAULT, M_DEFAULT, &m_milIDCal);
	else
		return -1;
}

int CMilCal::Free(void)
{
	if(m_milIDCal)
	{
		McalFree(m_milIDCal);
		m_milIDCal=NULL;
		return 0;
	}else return -1;
}

void CMilCal::Associate(CMilBuffer * pImage)
{
	McalAssociate(m_milIDCal, pImage->GetIDBuffer(), M_DEFAULT);
}


int CMilCal::TransformCoordinate(double posX, double PosY, double *pGetPosX, double *pGetPosY, int Dir)
{
	McalTransformCoordinate(m_milIDCal, Dir, posX, PosY, pGetPosX, pGetPosY);
	return 0;
}


int CMilCal::Grid(CMilBuffer * pImage, double OffsetX, double OffsetY, double OffsetZ, int RowCount, int ColumnCount, double RowSpacing, double ColumnSpacing, int OperateMode, int GridType)
{
	McalGrid(m_milIDCal, pImage->GetIDBuffer(), OffsetX, OffsetY, OffsetZ, RowCount, ColumnCount, RowSpacing, ColumnSpacing, OperateMode, GridType);
	return 0;
}


int CMilCal::Inquire(MIL_ID * pRetValue, int InquireType)
{
	return McalInquire(m_milIDCal, InquireType, pRetValue);
}
