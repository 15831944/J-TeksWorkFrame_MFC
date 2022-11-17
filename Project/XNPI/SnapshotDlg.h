#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include "SCButton.h"
#include "XNPI.h"
#include "XNPIDoc.h"
#include "XNPIView.h"
#include "ToolPanelDlg.h"
#include "OpPanelDlg.h"
#include "TiffTagManager.h"

// CSnapshotDlg 대화 상자입니다.

class CSnapshotDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSnapshotDlg)

public:
	CSnapshotDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSnapshotDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_SNAPSHOT_PANEL
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonRetake();
	afx_msg void OnBnClickedButtonNewpipe();
	afx_msg void OnEnSetfocusEditNumer();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonTakeImage();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonTriggerRepeat();
	afx_msg void OnNMClickListctrlSnapshot(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void DrawThumbnail(CString orgPath, CString itemName);
	void ReplaceThumbnail(CString orgPath, int index);

	int pipeImgNum = 1;
	int m_nItemNum = 0;
	int m_nSelectedItem;
	BOOL m_bIsNewPipe;
	CImageList m_ImageListThumb;
	ULONG_PTR m_gdiPlusToken;
	CListCtrl m_ListCtrlSnapShot;
	TiffTagManager m_tiffTag;
	CString m_strImgSaveFolder;
	CSCButton m_btnRetake;
	CSCButton m_btnNewPipe;
	CSCButton m_btnTakeImage;
	CSCButton m_btnReset;
	CSCButton m_btnTriggerRepeat;

	//! 아래 3 함수는 차례대로 호출해야 한다.
	void CreateInspectionImgFolder(CString time);
	void CreatePipeForlder(CString pipeNo, CString time);
	CString CreateImgName(CString sectionName, CString time, CString format);

	void SaveInspectionImg(CString path, CMilBuffer* img, CString format);
	void AttachTag(CString tifPath, CString tag);
	CXNPIView *m_pView;
	vector<CString> m_vtImgNameList;
	CString m_strPipeSectorNum;

	void SendProcDondCmd();
	void SendResetCmd();
	void SendTriggerRepeatCmd();
	void SendGoToSection(int sectionIndex);
	CMenu m_Menu; //스냅샷 우클릭 팝업메뉴

	int GetIndexBySectionName(CString sectionName);
	int FindSnapshotIndexByPartOfString(CString sectionName);

	CModbusManager* m_mdManager;
	vector<CString> m_vLastOpendFolderFileList;

	void DrawInfosMgraText(CMilBuffer* srcBuf, int num); //181016 MgraText로 정보등 쓴다음 저장

	SYSTEMTIME m_tCur_time; //190327 - 현재시간 밀리초 단위로 구하기(로그 접두사에 붙일 용도)
public:
	void NewPipeInspectionStart();
	void TakeImage();
	void TakeImage(int sectionNum);
	void RetakeImage(int snapshotIndex);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton2();
	void LoadInspectionFolder(CString path);
	vector<CString> GetFileNamesInDirectory(CString directory);
};

