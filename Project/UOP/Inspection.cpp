#include "StdAfx.h"
#include "Frame.h"
#include "Inspection.h"
#include <ItfModelBase.h>
#include "MachineInfo.h"

CInspection * CInspection::m_pInstance =NULL;
CInspection::CInspection(void)
:m_pInputImg(NULL)
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

int CInspection::Inspect(CAOIWin * pCompo)
{
	CModelBase *pObject;
	USES_CONVERSION;
	int retv=0;

	int nCount=pCompo->GetList()->GetCount();

	POSITION pos;
	pos=pCompo->GetList()->GetHeadPosition();

	if(!pCompo->GetList()->GetCount())return 0;

	pObject=(CModelBase *)pCompo->GetList()->GetHead();
	for(int i=0;i<nCount;i++)
	{
		if(pObject->IsKindOf(RUNTIME_CLASS(CAlgorithmBase)))
		{
			((CAlgorithmBase *)pObject)->SetInput(m_pInputImg);
			CRect CompoRect((int)pCompo->m_rectPLeft, (int)pCompo->m_rectPTop, 
							(int)pCompo->m_rectPRight, (int)pCompo->m_rectPBottom);
			CInspResult compoResult;
			retv=((CAlgorithmBase *)pObject)->Inspect(CompoRect, &compoResult);

			((CAlgorithmBase *)pObject)->DrawFeature(CMachineInfo::Instance()->GetRVOption());
		}
		pObject=(CModelBase *)pCompo->GetList()->GetNext(pos);
	}

	return retv;
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

			((CAlgorithmBase *)pObject)->DrawFeature(CMachineInfo::Instance()->GetRVOption());
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
	CAlgoDialog *pAlgoDialog = NULL;
	for(pos=m_regUIAlgo.GetHeadPosition();pos!=NULL;)
	{
		pAlgoDialog=(CAlgoDialog *)m_regUIAlgo.GetNext(pos);
		if(!(pAlgoDialog->GetAlgoType() & ALGO_FRAME))continue;
		if(!FindAlgo(pFrame, pAlgoDialog->GetAlgoName(), &pAlgorithm))
		{
			pAlgoDialog->SetParam(pAlgorithm);
			pAlgoDialog->m_pAlgorithm = pAlgorithm;
		}else
		{
			pAlgoDialog->SetCheck(FALSE);
		}
	}
	return 0;
}

int CInspection::LoadAlgorithm(CAOIWin * pCompo)
{
	int nCount=m_regUIAlgo.GetCount();
	if(!nCount)return -1;

	CAlgorithmBase *pAlgorithm=NULL;
	POSITION pos;
	CAlgoDialog *pAlgoDialog = NULL;
	for(pos = m_regUIAlgo.GetHeadPosition();pos!=NULL;)
	{
		pAlgoDialog=(CAlgoDialog *)m_regUIAlgo.GetNext(pos);
		if(!FindAlgo(pCompo, pAlgoDialog->GetAlgoName(), &pAlgorithm))
		{
			pAlgoDialog->SetParam(pAlgorithm);
			pAlgoDialog->m_pAlgorithm = pAlgorithm;
		}else
		{
			pAlgoDialog->SetCheck(FALSE);
		}
	}
	return 0;
}

void CInspection::SetMainApp(CWinApp * pMainApp)
{
}

int CInspection::AddAlgo2Compo(CAOIWin * pCompo)
{
	int nCount=m_regUIAlgo.GetCount();
	if(!nCount)return -1;

	CAlgorithmBase *pAlgorithm=NULL;
	POSITION pos;
	CAlgoDialog *pAlgoDialog = NULL;
	for(pos = m_regUIAlgo.GetHeadPosition();pos!=NULL;)
	{
		pAlgoDialog=(CAlgoDialog *)m_regUIAlgo.GetNext(pos);
		if(pAlgoDialog->IsChecked())
		{
			if(pAlgoDialog->GetAlgoType() & ALGO_COMPO)
			{
				pAlgorithm=pAlgoDialog->NewAlgoObject();
				pAlgorithm->SetParentModel(pCompo);
				pAlgoDialog->GetParam(pAlgorithm);
				pCompo->GetList()->AddTail(pAlgorithm);
			}
		}
	}
	return 0;
}

int CInspection::AddAlgo2Frame(CFrame * pFrame)
{
	int nCount=m_regUIAlgo.GetCount();
	if(!nCount)return -1;

	CAlgorithmBase *pAlgorithm=NULL;
	POSITION pos;
	CAlgoDialog *pAlgoDialog = NULL;
	for(pos = m_regUIAlgo.GetHeadPosition();pos!=NULL;)
	{
		pAlgoDialog=(CAlgoDialog *)m_regUIAlgo.GetNext(pos);
		if(pAlgoDialog->IsChecked())
		{
			if(pAlgoDialog->GetAlgoType() & ALGO_FRAME)
			{
				pAlgorithm=pAlgoDialog->NewAlgoObject();
				pAlgorithm->SetParentModel(pFrame);
				pAlgoDialog->GetParam(pAlgorithm);
				pFrame->GetList()->AddTail(pAlgorithm);
			}
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

/*! @fn int CInspection::DoTrial(void)
	@brief 현재 Frame에 등록된 전체 알고리즘을 실행한다. frame단위검사 및 component단위검사모두 포함
*/
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

/*! @fn int CInspection::DoTrial(CAlgoDialog * pSender)
	@param pSender Trial을 실행하는 UI의 포인터, 이함수는 Frame알고리즘검사를 하기 위한것이다.
	@brief 해당알고리즘만 테스트를 한다.
*/
int CInspection::DoTrial(CAlgoDialog * pSender)
{
	CInspResult Result;
	pSender->SetInput(m_pInputImg);
	if(pSender->GetAlgoType() & ALGO_COMBI)///Combination Type일 경우
	{
		CFrame *pSelectedFrame=GetCurrFrame();
		if(!pSelectedFrame)
		{
			AfxMessageBox(_T("None frame is selected"));
			return -1;
		}
		FindAlgo(pSelectedFrame, pSender->GetAlgoName(), &pSender->m_lstTemplete);
		pSender->DoTrial();
	}else
		pSender->DoTrial();
	if(pSender->m_pAlgorithm->GetResults(&Result))return 0;
	if(Result.GetCount())
		pSender->ReportResult(&Result);
	return 0;
}

/*! @fn int CInspection::DoCompoTrial(CAlgoDialog * pSender)
	@param pSender Trial요청하는 UI의 포인터, 이함수는 Component알고리즘검사를 하기 위한것이다. 
	@brief 해당알고리즘만 Component단위로 검사한다.
*/
int CInspection::DoCompoTrial(CAlgoDialog * pSender)
{
	return 0;
}

/*! @fn int CInspection::DoApply(CAlgoDialog * pSender)
	@param pSender Apply요청하는 UI의 포인터, 이함수는 Frame단위로 알고리즘을 적용하기 위해 사용된다. 
*/
int CInspection::DoApply(CAlgoDialog * pSender)
{
	pSender->GetParam(pSender->m_pAlgorithm);
	return 0;
}

/*! @fn int CInspection::DoCompoApply(CAlgoDialog * pSender)
	@param pSender Apply요청하는 UI의 포인터, 이함수는 Component단위로 알고리즘을 적용하기 위해 사용된다. 
*/
int CInspection::DoCompoApply(CAlgoDialog * pSender)
{
	return 0;
}

CFrame * CInspection::GetCurrFrame(void)
{

	return NULL;
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
	pAlgoDialog=NULL;
	while(pos)
	{
		pAlgoDialog = (CAlgoDialog *)m_regUIAlgo.GetNext(pos);
		if(pAlgoDialog)
		if(!pAlgoDialog->CompareAlgoName(szModelName))
		{
			*ppAlgorithm = pAlgoDialog->NewAlgoObject();
			return 0;
		}
	}

	return -2;
}

int CInspection::DeleteAlgo(CFrame * pFrame, CString strAlgoName)
{
	int nCount = pFrame->GetList()->GetCount();
	if(!nCount)return -1;
	USES_CONVERSION;

	CModelBase *pModel=(CModelBase *)pFrame->GetList()->GetHead();

	POSITION pos;
	pos=pFrame->GetList()->GetHeadPosition();

	CString ClssName;
	while(pos)
	{
		pModel=(CModelBase *)pFrame->GetList()->GetNext(pos);
		ClssName = A2W(pModel->GetRuntimeClass()->m_lpszClassName);
		if(!strAlgoName.Compare(ClssName))
		{
			delete pModel;
			pFrame->GetList()->RemoveAt(pos);
			return 0;
		}

	}

	return -2;
}

int CInspection::DeleteAlgo(CAOIWin * pCompo, CString strAlgoName)
{
	int nCount = pCompo->GetList()->GetCount();
	if(!nCount)return -1;
	CAlgorithmBase *pAlgo=(CAlgorithmBase *)pCompo->GetList()->GetHead();

	POSITION pos;
	pos=pCompo->GetList()->GetHeadPosition();

	for(int i=0;i<nCount;i++)
	{
		if(!strAlgoName.Compare(pAlgo->GetAlgoName()))
		{
			delete pAlgo;
			pCompo->GetList()->RemoveAt(pos);
			return 0;
		}
		pCompo->GetList()->GetNext(pos);	
	}

	return -2;
}

int CInspection::FindAlgo(CAOIWin * pCompo, CString strAlgoName, CAlgorithmBase ** ppAlgorithm)
{
	int nCount = pCompo->GetList()->GetCount();
	if(!nCount)return -1;
	CAlgorithmBase *pAlgo=(CAlgorithmBase *)pCompo->GetList()->GetHead();

	POSITION pos;
	pos=pCompo->GetList()->GetHeadPosition();

	for(int i=0;i<nCount;i++)
	{
		if(!strAlgoName.Compare(pAlgo->GetAlgoName()))
		{
			*ppAlgorithm = pAlgo;
			return 0;
		}
		pCompo->GetList()->GetNext(pos);	
	}

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

int CInspection::FindAlgo(CFrame * pFrame, CString strAlgoName, CPtrList *pLstAlgo)
{
	int nCount = pFrame->GetList()->GetCount();
	if(!nCount)return -1;
	pLstAlgo->RemoveAll();

	CString ClassNameCheck;
	CModelBase *pModelBase, *pModelBaseIn;
	POSITION pos;
	pos=pFrame->GetList()->GetHeadPosition();
	
	USES_CONVERSION;
	while(pos)
	{
		pModelBase=(CModelBase *)pFrame->GetList()->GetNext(pos);
		ClassNameCheck = A2W(pModelBase->GetRuntimeClass()->m_lpszClassName);

		if(ClassNameCheck==(_T("CAOIWin")))
		{
			POSITION posIn;
			CdblRect dblRectTemp(((CAOIWin *)pModelBase)->m_rectPLeft, ((CAOIWin *)pModelBase)->m_rectPTop,
				((CAOIWin *)pModelBase)->m_rectPRight, ((CAOIWin *)pModelBase)->m_rectPBottom);
			posIn=pModelBase->GetList()->GetHeadPosition();
			while(posIn)
			{
				pModelBaseIn = (CModelBase *)pModelBase->GetList()->GetNext(posIn);
				ClassNameCheck = A2W(pModelBaseIn->GetRuntimeClass()->m_lpszClassName);
				if(ClassNameCheck==strAlgoName)
				{
					pModelBaseIn->m_dblRect = dblRectTemp; 
					pLstAlgo->AddTail(pModelBaseIn);
				}
			}
		}
	}

	return 0;
}

int CInspection::GetUIParam(CAOIWin * pAOIWin)
{
	int nCount=m_regUIAlgo.GetCount();
	if(!nCount)return -1;

	CAlgorithmBase *pAlgorithm=NULL;
	POSITION pos;
	CAlgoDialog *pAlgoDialog = NULL;
	for(pos = m_regUIAlgo.GetHeadPosition();pos!=NULL;)
	{
		pAlgoDialog=(CAlgoDialog *)m_regUIAlgo.GetNext(pos);
		if (!FindAlgo(pAOIWin, pAlgoDialog->GetAlgoName(), &pAlgorithm))
		{
			pAlgoDialog->GetParam(pAlgorithm);
			pAlgoDialog->m_pAlgorithm = pAlgorithm;
		}
	}
	return 0;
}

int CInspection::SetFrameAlgo(CAlgoDialog * pSender, bool bStatus)
{
	CFrame *nSelectedFrame = GetCurrFrame();
	if(!nSelectedFrame)return -1;
	
	DeleteAlgo(nSelectedFrame, pSender->GetAlgoName());
	return 0;
}

int CInspection::SetCompoAlgo(CAlgoDialog * pSender, bool bStatus)
{

	return 0;
}


int CInspection::AddNewCompo(CPtrList *plstCompoRect, CPtrList *plstAlgo)
{
	return 0;
}

int CInspection::DoAutoTeaching(CAlgoDialog * pSender)
{
	pSender->SetInput(m_pInputImg);
	pSender->DoAutoTeaching();
	return 0;
}

