#pragma once
#include "MilClass.h"
#include "MilBase.h"
#include <vector>
#include ".\Grabber\Grabber.h"

class CGrabber;
class CMilDigitizer;
class CMilSystem;
using namespace std;
/*!
\brief 
* Mil의 System관련 함수를 Class화함, 기본적으로 1개는 있어야 하며, 추가되는것은 Grabber라 생각하면 됨.
@author 전병근
*/
class MIL_DLLDECL CMilSystem:public CMilBase
{
public:
	CMilSystem(void);
	CMilSystem(CMilApplication *pMilapplication);
	~CMilSystem(void);
	int Close(void);
	static CMilSystem * Instance(TCHAR* szGrabber);
	static CMilSystem * Instance(void);

	int Create(TCHAR *szGrabber=_T("NONE"));
	int Create(CMilApplication *pMilApplication);

	static CMilApplication *m_pMilApplication;
	MIL_ID m_milIDApplication;
	MIL_ID m_milIDSystem;
	static vector<CMilSystem *> m_vMilSystem;
	CMilSystem *m_pMilSystem;
	MIL_ID GetSystemID(void);
	void SetSystemID(MIL_ID idSystem);

	CGrabber *m_pGrabber;
	int GetDongleType(void);
	CString m_strGrabberName;
	CMilDigitizer* NewDigitizer(TCHAR *pCamFile);
};
