#pragma once
#include "AlgorithmBase.h"
#include "ParamsInterface.h"

// CAlgoDialog 대화 상자입니다.

/*!
	@brief 알고리즘작업자가 같이 작업할 다이얼로그의 조상클래스로 등록등을 사용자가 인지 하지 않는 상황에서 할 수 있도록 한다.
*/
class ITFBASEDLLDECL CAlgoDialog : public CDialog
{
	DECLARE_DYNAMIC(CAlgoDialog)

public:
	CAlgoDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAlgoDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_resID;
	CParamsInterface * m_pParamsInterface;
	CPtrList m_lstTemplete;
	virtual LRESULT OnScrnUpdate(WPARAM wparam, LPARAM lparam);
	virtual int GetParam(CAlgorithmBase * pAlgorithm);
	virtual int SetParam(CAlgorithmBase * pAlgorithm);
	virtual CAlgorithmBase * NewAlgoObject(void);
	void SetResID(int resID);
	virtual BOOL IsChecked(void);
	virtual int RegCaller(CParamsInterface * pParamsInterface);
	virtual int CompareAlgoName(TCHAR * szAlgoName);
	virtual int DoTrial(void);
	virtual int DoCompoTrial(CRect inspRect, CInspResult * pResult);
	virtual int Inspect(void);
	virtual void SetInput(CMilBuffer * pInputImage);
	CMilBuffer *m_pInputImg;
	CAlgorithmBase *m_pAlgorithm;
	virtual CString GetAlgoName(void);
	virtual int GetAlgoType(void);
	virtual void SetCheck(bool bStatus);
	virtual int DoAutoTeaching(void);
	virtual int ReportResult(CInspResult * pInspResult);
};
