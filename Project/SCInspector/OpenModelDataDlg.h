#pragma once
#include "afxcmn.h"
#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <string.h>
#include "afxwin.h"
#include "RenameModelDataDlg.h"
#include "SCInspector.h"
#include "Status.h"


// COpenModelDataDlg 대화 상자입니다.

class COpenModelDataDlg : public CDialog
{
	DECLARE_DYNAMIC(COpenModelDataDlg)

public:
	COpenModelDataDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COpenModelDataDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OPEN_MODELDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CStatic m_image;
	static CString m_strModelName;
	static CString m_strAllName;

	CMilBuffer *SavePCBModelBuff;

	virtual BOOL OnInitDialog();

	CMilDisplay *m_pMilDisplay; //Mil 관련 함수
	CMilBuffer *m_pDisplayMap; //Mil 관련 함수

	void FindModelData(void);
	BOOL DeleteDirectory(CString szDir, int recur);		// recur 가 0이면 자기는 지우지않음, 1이면 자기까지 삭제
	afx_msg void OnNMClickModelList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDelete();

	afx_msg void OnBnClickedBtnRename();
	afx_msg void OnBnClickedBtnSelect();

	CStatus m_pStatus;
	
    static COpenModelDataDlg* Instance(void);                 //<싱글톤 인스턴스 부분(.)
  	static COpenModelDataDlg* m_pInstance;
	int m_imageWidth;
	int m_imageHeight;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CStatic m_static_NoImage;
};
