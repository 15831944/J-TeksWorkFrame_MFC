#pragma once
class CLineEquation
{
public:
	CLineEquation();
	~CLineEquation();
	double m_coeA;
	double m_coeB;
	double m_coeC;
	double GetY(double dblX);
	void SetCoe(double coeA, double coeB, double coeC, int iDir);
	BOOL m_bIsSetCoe;
	BOOL IsSetCoe();
	int m_iDir;
};

