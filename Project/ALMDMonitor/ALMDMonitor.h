
// ALMDMonitor.h : main header file for the ALMDMonitor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CALMDMonitorApp:
// See ALMDMonitor.cpp for the implementation of this class
//

class CALMDMonitorApp : public CWinApp
{
public:
	CALMDMonitorApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	void InitEnvInstance();
};

extern CALMDMonitorApp theApp;
