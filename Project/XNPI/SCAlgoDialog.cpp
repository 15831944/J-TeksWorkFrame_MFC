// SCAlgoDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XNPI.h"
#include "SCAlgoBase.h"
#include "SCAlgoDialog.h"


// CSCAlgoDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSCAlgoDialog, CDialog)

CSCAlgoDialog::CSCAlgoDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SCALGODIALOG, pParent)
	, m_pSCAlgo(NULL)
{

}

CSCAlgoDialog::~CSCAlgoDialog()
{
}

void CSCAlgoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSCAlgoDialog, CDialog)
END_MESSAGE_MAP()


// CSCAlgoDialog 메시지 처리기입니다.


void CSCAlgoDialog::SetAlgo(CSCAlgoBase * pAlgo)
{
	if (!pAlgo)return;
	m_pSCAlgo = pAlgo;
	LoadParam(pAlgo);
}


void CSCAlgoDialog::LoadParam(CSCAlgoBase * pAlgo)
{
}
