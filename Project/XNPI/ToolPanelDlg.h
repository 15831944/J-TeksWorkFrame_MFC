#pragma once
#include "SCButton.h"
#include <Matrox.h>
#include "afxwin.h"
#include "TiffTagManager.h"

// CToolPanelDlg 대화 상자입니다.
struct tManSaveDuration
{
	CTime	tmStart;
	int iMinTimeDuration;
};

class CToolPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CToolPanelDlg)

public:
	CToolPanelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CToolPanelDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_TOOLPANEL
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnMansaveimg();
	afx_msg void OnBnClickedBtnMansavavi();
	CSCButton m_cManSavImg;
	CSCButton m_cManSavAVI;
	tExportAVI m_ExportAVITag;
	tExportAVI m_ExportAutoAVITag;
	afx_msg void OnBnClickedBtnDevsave();
	CSCButton m_cTiffSave;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	LRESULT OnImgAutoSaveTime(WPARAM wparam, LPARAM lparam);
	LRESULT OnImgAutoSaveTemp(WPARAM wparam, LPARAM lparam);
	LRESULT OnAVIAutoSaveStart(WPARAM wparam, LPARAM lparam);
	LRESULT OnAVIAutoSaveStop(WPARAM wparam, LPARAM lparam);
	LRESULT OnAVIManSaveStart(WPARAM wparam, LPARAM lparam);
	LRESULT OnAVIManSaveStop(WPARAM wparam, LPARAM lparam);
	static UINT ThreadAVIManSave(LPVOID pParam);
	void AVIManSaveStarter();
	virtual BOOL OnInitDialog();
	BOOL m_bAVIMan;
	tManSaveDuration m_AVIManSaveDuration;

	//kiy --------------------------------
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnAverageAccept(); //kiy - 0212
	afx_msg void OnBnClickedBtnLoadImg(); //kiy - 0212
	bool testFlag;
	int testNum;
	CSCButton m_cAvrApply;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_sAvgProc;
	CString m_sAvgDone;
	//CString m_sAvgNotUse;
	CStatic m_avgIsDoneStatic;
	bool m_bIsAvgDone;
	CView* m_pView;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	TiffTagManager m_tiffTag;	//
	void SaveImage(CString path, CMilBuffer* milBuf, MIL_INT fileType = M_TIFF);
	CString m_strAvgVal;
	CSCButton m_btnLoadimg;

	int m_nSectionNum;
	BOOL m_bTriggered;
	void Triggered(int sectionNum);
	SYSTEMTIME m_tCur_time; //190327 - 현재시간 밀리초 단위로 구하기(로그 접두사에 붙일 용도)
	void OnReceiveAvgStatus(bool isAvgDone); //190327

	//------------------------------------
	afx_msg void OnBnClickedButtonLoadInspFolder();
	CSCButton m_btnLoadFolder;
};
