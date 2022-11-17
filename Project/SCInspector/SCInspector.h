
// SCInspector.h : SCInspector 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
/**
@mainpage SCInspector Project
@section intro 소개
- 소개      :   Shield Cap검사를 위한 App.으로 개발@n
@section   Program SCInspector
- 프로그램명		:  SCInspector.EXE
- 프로그램내용	:  4개의 카메라 인터페이스를 가진 프로그램이다.
Area Camera를 사용하는 높이 검사와 Align검사를 행한다.
2개 PC에서 설치되어 No1PC 높이검사 2개, Align 1개 No2PC높이검사 3개
@section  INOUTPUT I/O자료
- INPUT          	:	아진보드 PCI-DB64R Description.xls참조
- OUTPUT      		:   아진보드 PCI-DB64R Description.xls참조
- Camera			:   Sentec STC-ANB521PCL
- Grabber			:	Euresys Multicam
@section  CREATEINFO 작성정보
- 작성자     		:	전병근
- 작성일		 	:	09/08/17
- 수정일			:	23/08/17
@section  MODIFYINFO    수정정보
- 수정자/수정일  	:	수정내역
- 전병근/13/08/17	:	Commit Git Server
- 
*/
class CRepository;

// CSCInspectorApp:
// 이 클래스의 구현에 대해서는 SCInspector.cpp을 참조하십시오.
//
#include "ModelData.h"

class CSCInspectorApp : public CWinAppEx
{
public:
	CSCInspectorApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	ULONG gdiplusToken;

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	int InitVision();
	virtual void OnClosingMainFrame(CFrameImpl* pFrameImpl);
	CRepository * m_pRepository;
	CModelData m_ModelData;
	int InitEnvInstance();
	BOOL InitGDIPlus();
};

extern CSCInspectorApp theApp;
