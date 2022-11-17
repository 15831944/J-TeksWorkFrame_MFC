#pragma once
#include "MilClass.h"
#define CAL_FILE _T("CAL.SET")
#define WHITE_FILE _T("White")
#define DARK_FILE _T("Dark.tif")

/*!
\brief
* Mil의 Calibration관련 함수를 Class화함
@author 전병근
*/
class MIL_DLLDECL CMilCal :public CMilBase
{
public:
	CMilCal(void);
	~CMilCal(void);
	MIL_ID m_milIDCal;
	int TeachCalParam(CMilBuffer *pCalImage, int RowCount, int ColCount);
	int SaveCalData(CString strFileName);
	int RestoreCalData(CString strFileName);
	int TransformImage(CMilBuffer * pImageDest, CMilBuffer * pImageSrc);
	int Alloc(MIL_ID * pMilIDCal);
	int Alloc(void);
	int Free(void);
	void Associate(CMilBuffer * pImage);
	int TransformCoordinate(double posX, double PosY, double *pGetPosX, double *pGetPosY, int Dir = M_PIXEL_TO_WORLD);
	int Grid(CMilBuffer * pImage, double OffsetX, double OffsetY, double OffsetZ, int RowCount, int ColumnCount, double RowSpacing, double ColumnSpacing, int OperateMode = M_DEFAULT, int GridType = M_CIRCLE_GRID);
	int Inquire(MIL_ID * pRetValue, int InquireType = M_CALIBRATION_STATUS + M_TYPE_MIL_INT);
};
