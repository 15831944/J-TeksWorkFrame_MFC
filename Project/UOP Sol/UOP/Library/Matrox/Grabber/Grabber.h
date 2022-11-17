#pragma once
#include "..\MilBase.h"
#include <mil.h>

class CMilBuffer;
class CGrabChannel;

/*!
\brief 
* Grabber의 기초 Interface Class 
@author 전병근
*/
class CGrabber
{
public:
	enum tCameraType{CT_EOSENS, CT_ES51, CT_DT};
	CGrabber(void);
	~CGrabber(void);
	CGrabber * Create(TCHAR* szGrabber=_T("NONE"));
	int SetCamPath(TCHAR * pPath);
	CString m_strGrabberName;
//	static CGrabber *m_pInstance;
protected:
	CMilBuffer *m_pMilBuffer;
public:
	MIL_ID m_milIDSystem;
	int Alloc(void);
	virtual int Init(void);
	virtual CGrabChannel* NewChannel(TCHAR * szCamFile);
};
