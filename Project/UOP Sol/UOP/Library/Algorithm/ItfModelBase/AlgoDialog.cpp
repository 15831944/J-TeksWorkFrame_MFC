// AlgoDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AlgoDialog.h"
#include "StarMessage.h"


// CAlgoDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAlgoDialog, CDialog)

CAlgoDialog::CAlgoDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
	, m_resID(0)
{
	
}

CAlgoDialog::~CAlgoDialog()
{
}

void CAlgoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAlgoDialog, CDialog)
	ON_MESSAGE(WM_SCRN_UPDATE, &CAlgoDialog::OnScrnUpdate)
END_MESSAGE_MAP()


// CAlgoDialog 메시지 처리기입니다.

void CAlgoDialog::SetResID(int resID)
{
	m_resID=resID;
}

int CAlgoDialog::GetParam(CAlgorithmBase * pAlgorithm)
{
	return 0;
}

int CAlgoDialog::SetParam(CAlgorithmBase * pAlgorithm)
{
	return 0;
}

/*!
	@fn CAlgorithmBase * CAlgoDialog::NewAlgoObject(void)
	@brief 알고리즘을 객체를 생성시켜 줄때 사용하게 된다. 알고리즘의 이름을 모르기때문에 자동생성에 필요하다
*/
CAlgorithmBase * CAlgoDialog::NewAlgoObject(void)
{
	return NULL;
}
BOOL CAlgoDialog::IsChecked(void)
{
	return 0;
}

int CAlgoDialog::RegCaller(CParamsInterface * pParamsInterface)
{
	m_pParamsInterface = pParamsInterface;
	return 0;
}

/*!
	@fn int CAlgoDialog::CompareAlgoName(TCHAR * szAlgoName)
	@brief 모델을 Loading할때 알고리즘이름이 있는지 확인할 수 있는 방법을 제공해준다
*/
int CAlgoDialog::CompareAlgoName(TCHAR * szAlgoName)
{
	return GetAlgoName().Compare(szAlgoName);
}

int CAlgoDialog::DoTrial(void)
{
	return 0;
}

int CAlgoDialog::DoCompoTrial(CRect inspRect, CInspResult * pResult)
{
	return 0;
}

int CAlgoDialog::Inspect(void)
{
	return 0;
}

/*!
	@fn void CAlgoDialog::SetInput(CMilBuffer * pInputImage)
	@brief 검사하게될 이미지를 지정하게 된다.
*/
void CAlgoDialog::SetInput(CMilBuffer * pInputImage)
{
	m_pInputImg = pInputImage;
}

/*!
	@fn CString CAlgoDialog::GetAlgoName(void)
	@brief 알고리즘의 이름을 반환한다. 모델로딩등에서 사용하게 된다.
*/
CString CAlgoDialog::GetAlgoName(void)
{
	return CString();
}

/*!
	@fn LRESULT CAlgoDialog::OnScrnUpdate(WPARAM wparam, LPARAM lparam)
	@brief 화면갱신이 요청되면 파레메터등의 값을 화면으로 Update해준다
*/
LRESULT CAlgoDialog::OnScrnUpdate(WPARAM wparam, LPARAM lparam)
{
	UpdateData(FALSE);
	return 0;
}

/*!	@fn int CAlgoDialog::GetAlgoType(void)
	@brief 알고리즘의 형태를 반환한다
*/
int CAlgoDialog::GetAlgoType(void)
{
	return 0;
}

void CAlgoDialog::SetCheck(bool bStatus)
{
}

int CAlgoDialog::DoAutoTeaching(void)
{
	return 0;
}

int CAlgoDialog::ReportResult(CInspResult * pInspResult)
{
	return 0;
}
