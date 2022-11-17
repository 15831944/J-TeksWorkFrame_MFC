#pragma once
#include "afxwin.h"
#include "SCInspIO.h"

// CIOSetDlg 대화 상자입니다.
#include "SCInspIO.h"
#include "SCButton.h"

class CIOSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CIOSetDlg)

public:
	CIOSetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CIOSetDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IOSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:


	// output 변수들
	tOStage m_nOStage[2];
	int m_nOSpare0;
	int m_nOSpare31;

	CString m_LableName0;
	CString m_LableName1;
	CString m_LableName2;
	CString m_LableName3;
	CString m_LableName4;
	CString m_LableName5;
	CString m_LableName6;
	CString m_LableName7;
	CString m_LableName8;
	CString m_LableName9;
	CString m_LableName10;
	CString m_LableName11;
	CString m_LableName12;
	CString m_LableName13;
	CString m_LableName14;
	CString m_LableName15;
	CString m_LableName16;
	CString m_LableName17;
	CString m_LableName18;
	CString m_LableName19;
	CString m_LableName20;
	CString m_LableName21;
	CString m_LableName22;
	CString m_LableName23;
	CString m_LableName24;
	CString m_LableName25;
	CString m_LableName26;
	CString m_LableName27;
	CString m_LableName28;
	CString m_LableName29;
	CString m_LableName30;
	CString m_LableName31;



	
	afx_msg void OnBnClickedOk();


	void init_IOset(void);

	virtual BOOL OnInitDialog();

    //input 관련 변수들 
	tIStage m_nIStage[2];
	int m_nISpare22;
	int m_nISpare23;
	int m_nISpare24;
	int m_nISpare25;
	int m_nISpare26;
	int m_nISpare27;
	int m_nISpare28;
	int m_nISpare29;
	int m_nISpare30;
	int m_nISpare31;


	CString m_InputLableName0;
	CString m_InputLableName1;
	CString m_InputLableName2;
	CString m_InputLableName3;
	CString m_InputLableName4;
	CString m_InputLableName5;
	CString m_InputLableName6;
	CString m_InputLableName7;
	CString m_InputLableName8;
	CString m_InputLableName9;
	CString m_InputLableName10;
	CString m_InputLableName11;
	CString m_InputLableName12;
	CString m_InputLableName13;
	CString m_InputLableName14;
	CString m_InputLableName15;
	CString m_InputLableName16;
	CString m_InputLableName17;
	CString m_InputLableName18;
	CString m_InputLableName19;
	CString m_InputLableName20;
	CString m_InputLableName21;
	CString m_InputLableName22;
	CString m_InputLableName23;
	CString m_InputLableName24;
	CString m_InputLableName25;
	CString m_InputLableName26;
	CString m_InputLableName27;
	CString m_InputLableName28;
	CString m_InputLableName29;
	CString m_InputLableName30;
	CString m_InputLableName31;
	CString m_InputLableName32;
	CString m_InputLableName33;
	CString m_InputLableName34;
	CString m_InputLableName35;
	CString m_InputLableName36;
	CString m_InputLableName37;
	CString m_InputLableName38;


	void SetButton(void);
	CButtonST m_OutputLed0;
	CButtonST m_OutputLed1;
	CButtonST m_OutputLed2;
	CButtonST m_OutputLed3;
	CButtonST m_OutputLed4;
	CButtonST m_OutputLed5;
	CButtonST m_OutputLed6;
	CButtonST m_OutputLed7;
	CButtonST m_OutputLed8;
	CButtonST m_OutputLed9;
	CButtonST m_OutputLed10;
	CButtonST m_OutputLed11;
	CButtonST m_OutputLed12;
	CButtonST m_OutputLed13;
	CButtonST m_OutputLed14;
	CButtonST m_OutputLed15;
	CButtonST m_OutputLed16;
	CButtonST m_OutputLed17;
	CButtonST m_OutputLed18;
	CButtonST m_OutputLed19;
	CButtonST m_OutputLed20;
	CButtonST m_OutputLed21;
	CButtonST m_OutputLed22;
	CButtonST m_OutputLed23;
	CButtonST m_OutputLed24;
	CButtonST m_OutputLed25;
	CButtonST m_OutputLed26;
	CButtonST m_OutputLed27;
	CButtonST m_OutputLed28;
	CButtonST m_OutputLed29;
	
	CButtonST m_OutputLed31;

	

	CToolTipCtrl m_ToolTip;
	CLedButton m_InputLed0;
	CLedButton m_InputLed1;
	CLedButton m_InputLed2;
	CLedButton m_InputLed3;
	CLedButton m_InputLed4;
	CLedButton m_InputLed5;
	CLedButton m_InputLed6;
	CLedButton m_InputLed7;
	CLedButton m_InputLed8;
	CLedButton m_InputLed9;
	CLedButton m_InputLed10;
	CLedButton m_InputLed11;
	CLedButton m_InputLed12;
	CLedButton m_InputLed13;
	CLedButton m_InputLed14;
	CLedButton m_InputLed15;
	CLedButton m_InputLed16;
	CLedButton m_InputLed17;
	CLedButton m_InputLed18;
	CLedButton m_InputLed19;
	CLedButton m_InputLed20;
	CLedButton m_InputLed21;
	CLedButton m_InputLed22;
	CLedButton m_InputLed23;
	CLedButton m_InputLed24;
	CLedButton m_InputLed25;
	CLedButton m_InputLed26;
	CLedButton m_InputLed27;
	CLedButton m_InputLed28;
	CLedButton m_InputLed29;
	CLedButton m_InputLed30;
	CLedButton m_InputLed31;
	CLedButton m_InputLed32;
	CLedButton m_InputLed33;
	CLedButton m_InputLed34;
	CLedButton m_InputLed35;
	CLedButton m_InputLed36;
	CLedButton m_InputLed37;
	CLedButton m_InputLed38;


	afx_msg void OnBnClickedOutputiocheck0();
	afx_msg void OnBnClickedOutputiocheck1();
	afx_msg void OnBnClickedOutputiocheck2();
	afx_msg void OnBnClickedOutputiocheck3();
	afx_msg void OnBnClickedOutputiocheck4();
	afx_msg void OnBnClickedOutputiocheck5();
	afx_msg void OnBnClickedOutputiocheck6();
	afx_msg void OnBnClickedOutputiocheck7();
	afx_msg void OnBnClickedOutputiocheck8();
	afx_msg void OnBnClickedOutputiocheck9();
	afx_msg void OnBnClickedOutputiocheck10();
	afx_msg void OnBnClickedOutputiocheck11();
	afx_msg void OnBnClickedOutputiocheck12();
	afx_msg void OnBnClickedOutputiocheck13();
	afx_msg void OnBnClickedOutputiocheck14();
	afx_msg void OnBnClickedOutputiocheck15();
	afx_msg void OnBnClickedOutputiocheck16();
	afx_msg void OnBnClickedOutputiocheck17();
	afx_msg void OnBnClickedOutputiocheck18();
	afx_msg void OnBnClickedOutputiocheck19();
	afx_msg void OnBnClickedOutputiocheck20();
	afx_msg void OnBnClickedOutputiocheck21();
	afx_msg void OnBnClickedOutputiocheck22();
	afx_msg void OnBnClickedOutputiocheck23();
	afx_msg void OnBnClickedOutputiocheck24();
	afx_msg void OnBnClickedOutputiocheck25();
	afx_msg void OnBnClickedOutputiocheck26();
	afx_msg void OnBnClickedOutputiocheck27();
	afx_msg void OnBnClickedOutputiocheck28();
	afx_msg void OnBnClickedOutputiocheck29();
    afx_msg void OnBnClickedOutputiocheck31();


	void DoInputScan(void);
	BOOL m_bChkInput0;
	BOOL m_bChkInput1;
	BOOL m_bChkInput2;
	BOOL m_bChkInput3;
	BOOL m_bChkInput4;
	BOOL m_bChkInput5;
	BOOL m_bChkInput6;
	BOOL m_bChkInput7;
	BOOL m_bChkInput8;
	BOOL m_bChkInput9;
	BOOL m_bChkInput10;
	BOOL m_bChkInput11;
	BOOL m_bChkInput12;
	BOOL m_bChkInput13;
	BOOL m_bChkInput14;
	BOOL m_bChkInput15;
	BOOL m_bChkInput16;
	BOOL m_bChkInput17;
	BOOL m_bChkInput18;
	BOOL m_bChkInput19;
	BOOL m_bChkInput20;
	BOOL m_bChkInput21;
	BOOL m_bChkInput22;
	BOOL m_bChkInput23;
	BOOL m_bChkInput24;
	BOOL m_bChkInput25;
	BOOL m_bChkInput26;
	BOOL m_bChkInput27;
	BOOL m_bChkInput28;
	BOOL m_bChkInput29;
	BOOL m_bChkInput30;
	BOOL m_bChkInput31;
	BOOL m_bChkInput32;
	BOOL m_bChkInput33;
	BOOL m_bChkInput34;
	BOOL m_bChkInput35;
	BOOL m_bChkInput36;
	BOOL m_bChkInput37;
	BOOL m_bChkInput38;

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	
	CButtonST m_OutputLed30;
	afx_msg void OnBnClickedOutputiocheck30();
	CSCButton m_cSave;
	CSCButton m_cCancel;
};
