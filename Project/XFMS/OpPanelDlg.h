#pragma once
#include "afxwin.h"
#include "..\Library\SerialLib\SerialMFC.h"	// 필수
#include "..\Library\SerialLib\XFMSPacket.h"	// 필수
#include "SCButton.h"
#include "SCGroupBox.h"
#include "NumEdit.h"
#include "afxdtctl.h"

// COpPanelDlg 대화 상자입니다.
struct tXFMSRecord;
class CToolPanelDlg;


//시간관련 자동저장에 필용한 옵션
struct tAutoSaveTime
{
	CTime	tmStart;
	CTime	tmEnd;
	int iSecTimeTotal;
	int iMinTimeInterval;
	int iMinTimeDuration;
};

// 온도관련 자동저장에 필요한 옵션
struct tAutoSaveTemp
{
	int iTempLimitLow;
	int iTempLimitHigh;
	int iSensorNo;
	int iTempDelta;
	int iMinTimeDuration;
};

class COpPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpPanelDlg)

public:
	COpPanelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COpPanelDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPPANEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CSerialMFC		m_serial;				// 필수
	CXFMSPacket 	m_XFMSPacket;			// 필수
	CComboBox m_ctrlSelCam;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeSelcam();
	afx_msg void OnBnClickedBtnLoadimage();
	afx_msg void OnBnClickedSetenv();
	int InitLight();
	BOOL m_bReceived;
	int SerialCheckNInit();
	void ProcessCMD(int nEvent);
	void SetFlagReceived();
	BOOL IsCMDReceived();
	int RcvSerialMsg(WPARAM wParam);
	int CheckoutPacket();
	afx_msg void OnBnClickedBtnTeaching();
	afx_msg void OnBnClickedBtnLoadjobfile();
	afx_msg void OnBnClickedBtnSavejobfile();
	afx_msg void OnBnClickedChkactiveallcam();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedAistart();
	afx_msg void OnBnClickedBtnSaveas();
	afx_msg void OnBnClickedBtntrial();
	afx_msg void OnBnClickedBtnOptest();
	void DeleteOldFile();
	BOOL IsDate(CString strName, CTime *pTime);
	int DiffDate(CTime * pDate);
	void DelFolder(CString strFolder);
	CSCButton m_cRunStop;
	CSCButton m_cSaveImage;
	CSCButton m_cLoadImage;
	CSCButton m_cLoadJobFile;
	CSCButton m_cSaveJobFile;
	CSCButton m_cSaveAs;
	CSCButton m_cTeaching;
	CSCButton m_cTrial;
	CSCButton m_cSetEnv;
	CSCButton m_cFinish;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	static UINT ThreadTest(LPVOID pParam);
	CEdit m_cTempP1;
	CEdit m_cTempP2;
	CEdit m_cTempP3;
	CEdit m_cTempP4;
	CEdit m_cTempP5;
	CEdit m_cTempP6;
	CStatic m_cLabelP1;
	CStatic m_cLabelP2;
	CStatic m_cLabelP3;
	CStatic m_cLabelP4;
	CStatic m_cLabelP5;
	CStatic m_cLabelP6;
	CFont m_BigFont;
	afx_msg void OnBnClickedBtnAifin();
	int GetTempData();
	int m_selFocalSpotSize;
	//! AVI Auto Select Type
	int m_selAVIAST;
	//! Image auto Select Type
	int m_selImgAST;
	afx_msg void OnBnClickedSelfocalspot();
	afx_msg void OnBnClickedBtnXrayon();
	afx_msg void OnBnClickedBtnXrayoff();
	CSCButton m_cBtnXrayON;
	CSCButton m_cBtnXrayOFF;
	afx_msg void OnBnClickedAddmark();
	CString m_strUserMarkName;
//	CString m_strAddMark;
	CString m_strSampleName;
	CString m_strMakCondition;
	CSCButton m_cMarkAdd;
	CString GetManualFileName();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSCButton m_cTempSave;
	afx_msg void OnBnClickedBtnSavcsv();
	CSCButton m_cTempCSVSave;
	BOOL m_bWriteCSV;
	CString m_strFullCSVFileName;
	CNumEdit m_cVol;
	CNumEdit m_cCur;
	CSCButton m_cAAFin;
	CSCButton m_cAAStart;
	void CloseProcess();
	afx_msg void OnBnClickedAastart();
	afx_msg void OnBnClickedBtnAafin();
	void TimeImgAutoSaveStarter();
	// !온도변화량으로 자동저장하는 자동 이미지 저장 루틴
	void TempImgAutoSaveStarter();
	void TimeAVIAutoSaveStarter();
	void TempAVIAutoSaveStarter();
	static UINT ThreadTempImgAutoSave(LPVOID pParam);
	static UINT ThreadTimeImgAutoSave(LPVOID pParam);
	static UINT ThreadTimeAVIAutoSave(LPVOID pParam);
	static UINT ThreadTempAVIAutoSave(LPVOID pParam);


	// 자동 이미지 저장로직 온도변화에 따른
	tAutoSaveTemp m_ImgAutoSaveTemp;
	tAutoSaveTime m_ImgAutoSaveTime;
	tAutoSaveTemp m_AVIAutoSaveTemp;
	tAutoSaveTime m_AVIAutoSaveTime;
	CDateTimeCtrl m_cImgTimeStart;
	CDateTimeCtrl m_cImgTimeEnd;
	CDateTimeCtrl m_cAVITimeStart;
	CDateTimeCtrl m_cAVITimeEnd;
	BOOL m_bImgAuto;
	CToolPanelDlg *m_pToolPanelDlg;
	LRESULT OnXrayOff(WPARAM wparam, LPARAM lparam);
	afx_msg void OnBnClickedSelfocalspotlarge();
	void SetDefault();
	BOOL m_bAVIAuto;
};
