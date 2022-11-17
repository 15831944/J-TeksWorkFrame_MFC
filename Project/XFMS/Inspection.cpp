#include "StdAfx.h"
#include "Frame.h"
#include "XFMS.h"
#include "Inspection.h"
#include "Status.h"
//#include "ChipAlgo.h"
//#include "BridgeAlgo.h"
#include "AlgoDialog.h"


CInspection * CInspection::m_pInstance =NULL;
CInspection::CInspection(void)
:m_pCurrFrame(NULL)
, m_pInputImg(NULL)
{
}

CInspection::~CInspection(void)
{
}

int CInspection::Inspect(void)
{
	POSITION pos;
	CAlgoDialog *pAlgoDialog;
	if(!m_regUIAlgo.GetCount())return -1;

	pos = m_regUIAlgo.GetHeadPosition();
	pAlgoDialog=(CAlgoDialog *)m_regUIAlgo.GetHead();

	if(pAlgoDialog->IsChecked())
	{
		pAlgoDialog->SetInput(m_pInputImg);
		pAlgoDialog->Inspect();
	}

	return 0;

}

int CInspection::Inspect(CFrame* pFrame)
{
	CModelBase *pObject;
	USES_CONVERSION;
	int retv=0;

	int nCount=pFrame->GetList()->GetCount();

	POSITION pos;
	pos=pFrame->GetList()->GetHeadPosition();

	if(!pFrame->GetList()->GetCount())return 0;

	pObject=(CModelBase *)pFrame->GetList()->GetHead();
	for(int i=0;i<nCount;i++)
	{
		if(pObject->IsKindOf(RUNTIME_CLASS(CAlgorithmBase)))
		{
			((CAlgorithmBase *)pObject)->SetInput(m_pInputImg);
			retv=((CAlgorithmBase *)pObject)->Inspect();

			((CAlgorithmBase *)pObject)->DrawFeature(CStatus::Instance()->GetRVOption());
		}
		pObject=(CModelBase *)pFrame->GetList()->GetNext(pos);
	}
	
	return retv;
}

int CInspection::LoadAlgorithm(CFrame * pFrame)
{
	int nCount=m_regUIAlgo.GetCount();
	if(!nCount)return -1;

	CAlgorithmBase *pAlgorithm=NULL;
	POSITION pos;
	CAlgoDialog *pAlgoDialog = (CAlgoDialog *)m_regUIAlgo.GetHead();
	pos = m_regUIAlgo.GetHeadPosition();
	for(int i=0;i<nCount;i++)
	{
		if(!FindAlgo(pFrame, pAlgoDialog->GetAlgoName(), &pAlgorithm))
		{
			pAlgoDialog->SetParam(pAlgorithm);
			pAlgoDialog->m_pAlgorithm = pAlgorithm;
		}

		pAlgoDialog=(CAlgoDialog *)m_regUIAlgo.GetNext(pos);
	}
	return 0;
}


int CInspection::AddAlgo2Frame(CFrame * pFrame)
{
	int nCount=m_regUIAlgo.GetCount();
	if(!nCount)return -1;

	CAlgorithmBase *pAlgorithm=NULL;
	POSITION pos;
	CAlgoDialog *pAlgoDialog = (CAlgoDialog *)m_regUIAlgo.GetHead();
	pos = m_regUIAlgo.GetHeadPosition();
	for(int i=0;i<nCount;i++)
	{
		if(pAlgoDialog->IsChecked())
		{
			pAlgorithm=pAlgoDialog->NewAlgoObject();
			pAlgoDialog->GetParam(pAlgorithm);
			pFrame->GetList()->AddTail(pAlgorithm);
			pAlgoDialog=(CAlgoDialog *)m_regUIAlgo.GetNext(pos);
		}
	}
	return 0;
}

CInspection * CInspection::Instance(void)
{
	if(!m_pInstance)m_pInstance=new CInspection;
	return m_pInstance;
}

int CInspection::UpdateAlgo2Frame(CFrame * pFrame)
{
	CModelBase* CurElement;
	POSITION pos;
	int nCount=pFrame->GetList()->GetCount();
	for(pos=pFrame->GetList()->GetHeadPosition();pos!=NULL;)
	{
		CurElement=(CModelBase *)pFrame->GetList()->GetNext(pos);
//		if(CurElement->IsKindOf(pElementType)) return TRUE;
	}

	return 0;
}

int CInspection::AddAlgDlg(CAlgoDialog * pNewAlgoDlg)
{
	pNewAlgoDlg->RegCaller(this);
	m_regUIAlgo.AddTail(pNewAlgoDlg);
	return 0;
}

int CInspection::DoTrial(CFrame *pFrame)
{
	CModelBase *pObject;
	USES_CONVERSION;
	int nCount=pFrame->GetList()->GetCount();

	POSITION pos;
	pos=pFrame->GetList()->GetHeadPosition();

	pObject=(CModelBase *)pFrame->GetList()->GetHead();
	for(int i=0;i<nCount;i++)
	{
		if(pObject->IsKindOf(RUNTIME_CLASS(CAlgorithmBase)))
		{
			((CAlgorithmBase *)pObject)->SetInput(m_pInputImg);
			((CAlgorithmBase *)pObject)->Inspect();
		}
		pObject=(CModelBase *)pFrame->GetList()->GetNext(pos);
	}

	return 0;
}

int CInspection::DoTrial(void)
{
	POSITION pos;
	CAlgoDialog *pAlgoDialog;
	if(!m_regUIAlgo.GetCount())return -1;

	pos = m_regUIAlgo.GetHeadPosition();
	pAlgoDialog=(CAlgoDialog *)m_regUIAlgo.GetHead();

	do 
	{
		if(pAlgoDialog->IsChecked())
		{
			pAlgoDialog->SetInput(m_pInputImg);
			pAlgoDialog->DoTrial();
		}
		m_regUIAlgo.GetNext(pos);
	} while (pos);

	return 0;
}

int CInspection::DoApply(CAlgoDialog * pAlgoDialog)
{
	pAlgoDialog->GetParam(pAlgoDialog->m_pAlgorithm);
	return 0;
}

CFrame * CInspection::GetCurrFrame(void)
{
	return m_pCurrFrame;
}

void CInspection::SetInput(CMilBuffer * pMilBuffer)
{
	m_pInputImg = pMilBuffer;
}

/*!
	@fn int CInspection::NewObject(TCHAR * szModelName, CModelBase ** ppAlgorithm)
	@param szModelName 찾을 알고리즘의 String값 ModelData에 있는 String값임
	@param ppAlgorithm 찾을 알고리즘의 값을 반환한다
	@return 0:정상 0이외의 값:알고리즘을 찾지 못하여 알고리즘을 생성하지 못함 -1:등록된 알고리즘이 없음 -2:스트링에 해당하는것을 찾지 못함
*/
int CInspection::NewObject(TCHAR * szModelName, CAlgorithmBase ** ppAlgorithm)
{
	POSITION pos;
	CAlgoDialog *pAlgoDialog;
	if(!m_regUIAlgo.GetCount())return -1;

	pos = m_regUIAlgo.GetHeadPosition();
	pAlgoDialog=(CAlgoDialog *)m_regUIAlgo.GetHead();

	do{
		if(!pAlgoDialog->CompareAlgoName(szModelName))
		{
			*ppAlgorithm = pAlgoDialog->NewAlgoObject();
			return 0;
		}
		pAlgoDialog = (CAlgoDialog *)m_regUIAlgo.GetNext(pos);
	}while(pos);

	return -2;
}

int CInspection::FindAlgo(CFrame * pFrame, CString strAlgoName, CAlgorithmBase ** ppAlgorithm)
{
	int nCount = pFrame->GetList()->GetCount();
	if(!nCount)return -1;
	CAlgorithmBase *pAlgo=(CAlgorithmBase *)pFrame->GetList()->GetHead();

	POSITION pos;
	pos=pFrame->GetList()->GetHeadPosition();

	for(int i=0;i<nCount;i++)
	{
		if(!strAlgoName.Compare(pAlgo->GetAlgoName()))
		{
			*ppAlgorithm = pAlgo;
			return 0;
		}
		pFrame->GetList()->GetNext(pos);	
	}

	return -2;
}
