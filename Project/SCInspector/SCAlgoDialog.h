#pragma once

class CSCAlgoBase;
// CSCAlgoDialog 대화 상자입니다.

class CSCAlgoDialog : public CDialog
{
	DECLARE_DYNAMIC(CSCAlgoDialog)

public:
	CSCAlgoDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSCAlgoDialog();

// 대화 상자 데이터입니다.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void SetAlgo(CSCAlgoBase * pAlgo);
	CSCAlgoBase *m_pSCAlgo;
	virtual void LoadParam(CSCAlgoBase * pAlgo);
};
