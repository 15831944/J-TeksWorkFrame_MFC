#pragma once

class CUnitConversion
{
public:
	CUnitConversion();
	~CUnitConversion();
	void SetRollAndPitch(double dblRoll, double dblPitch);//! degree단위로 설정한다.
	void SetWaferSize(int iSize);
	int m_iWaferSize;//! Diamenter 지름값
	double m_dblRoll;//! Degree단위의 Roll값
	double m_dblPitch;  //! Degree단위의 Pitch값
	double CalcMagnitude();
	int CheckQuadrant(double dblRoll, double dblPitch);
	double CalcDirection();
	void GetUnitString(CString &strXUnit, CString &strYUnit);
	static CUnitConversion * Instance();
	static CUnitConversion *m_pUnitConversion;
	int m_iCoordinateSystem;
	int m_iInclinationType;
	void GetStateValue(double &dblXValue, double &dblYValue);
	void ConvertToDest(CStatus::tCoordinateSystem iCrdiSystem, CStatus::tAngleType iAngleType, CStatus::tIncType iIncType, CStatus::tRORType iRORType, CString *pStrUnit, double *pDestValue, double orgDegree);
	void ConvertToDegree(CStatus::tCoordinateSystem iCrdiSystem, CStatus::tAngleType iAngleType, CStatus::tIncType iIncType, CStatus::tRORType iRORType, double *pDestDegree, double orgValue);

};

