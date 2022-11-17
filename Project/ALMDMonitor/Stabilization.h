#pragma once
class CStabilization
{
public:
	CStabilization();
	~CStabilization();
	void Add(CPoint degreeValue);
	BOOL IsStable();
	static CStabilization * Instance();
	static CStabilization *m_pInstance;
	CPoint *m_pPointArray;
	int m_IndexToWrite;
	int m_IndexToRead;
	int m_sizeRingBuffer;
	void GetAverage(double *pDblAvrX, double *pDblAvrY);
	int m_nCount;
	void Init();
	void CalcStable();
	BOOL m_bIsStableX;
	BOOL m_bIsStableY;
	int m_iSensitivity;
	BOOL m_bIsStable;
	// iSensitivity값이 5이면 Sensitivity값이 0.005임
	void SetSensitivity(int iSensitivity);
	int GetDefaultSensitivity();
};

