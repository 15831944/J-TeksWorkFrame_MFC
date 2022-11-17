#pragma once

typedef struct dblXRAYSET
{
	long m_dblVol;
	long m_dblCur;
}tXraySet;

class CXraySet : public tXraySet
{
public:
	CXraySet(void);
	~CXraySet(void);
	void operator=(CXraySet& xraySet);
	BOOL operator==(CXraySet& xraySet);
	CXraySet operator+(CXraySet& xraySet);
	CXraySet operator-(CXraySet& xraySet);
	CXraySet(double setVol, double setCur);
};
