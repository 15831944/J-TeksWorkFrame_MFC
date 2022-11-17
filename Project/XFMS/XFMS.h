
// XFMS.h : SCInspector 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
/**
@mainpage XFMS Project
@section intro 소개
- 소개      :   X-Ray를 이용하여 로의 용융상태를 관찰하기 위해 App.으로 개발@n
@section   Program XFMS
- 프로그램명		:  XFMS.EXE
- 프로그램내용	:  1개의 네트웍타입의 Xray 편판디텍터 인페이스로 동영상의 저장을 하고 이때 측정온도를 그래프로 보인다.
@section  INOUTPUT I/O자료
- INPUT(네트웍)   	:	Modulbus 2개 포인트 온도측정 R7E-TS4_INST
- 통신(Serial)		:	튜브
- Camera			:   Viewworks
- Grabber			:	network type
@section  CREATEINFO 작성정보
- 작성자     		:	전병근
- 작성일		 	:	20/10/17
- 수정일			:	23/11/17
@section  MODIFYINFO    수정정보
- 수정자/수정일  	:	수정내역
- 전병근/13/11/17	:	Commit Git Server
- 
*/
class CRepository;

// CXFMSApp:
// 이 클래스의 구현에 대해서는 SCInspector.cpp을 참조하십시오.
//
#include "ModelData.h"

class CXFMSApp : public CWinAppEx
{
public:
	CXFMSApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	ULONG gdiplusToken;
	CRepository * m_pRepository;

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	int InitVision();
	virtual void OnClosingMainFrame(CFrameImpl* pFrameImpl);
	CModelData m_ModelData;
	int InitEnvInstance();
	BOOL InitGDIPlus();
};

extern CXFMSApp theApp;
