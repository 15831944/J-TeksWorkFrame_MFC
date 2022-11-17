#include "StdAfx.h"
#include <Mil.h>
#include "MilContext.h"

CMilContext::CMilContext(void)
{
	m_milIDContext=M_NULL;
}

CMilContext::~CMilContext(void)
{
}

/*! @fn int CMilContext::GetIDContext(void)
*	@brief Class에서 사용하는 실제 Context ID를 반환한다
*	@return Context ID를 반환한다.
*/
int CMilContext::GetIDContext(void)
{
	return m_milIDContext;
}


/*! @fn int CMilContext::RestoreContext(TCHAR * szLoadFile)
*	@brief Context를 파일로부터 읽어온다
*	@return 0이 아니면 Error
*/
int CMilContext::RestoreContext(TCHAR * szLoadFile)
{
	m_pMilSystem=CMilSystem::Instance();
	return MmodRestore(szLoadFile, m_pMilSystem->GetSystemID(), M_DEFAULT, &m_milIDContext);

}

/*! @fn int CMilContext::GetImage(CMilBuffer *pBuffer)
*	@brief Context로부터 이미지를 가져온다
*   @n 정확히 말하면 pBuffer에 이미지를 그리는것이다.
*	@param pBuffer 이미지를 그릴 Mil Buffer(Image 속성이 있어야 함)
*/
int CMilContext::GetImage(CMilBuffer *pBuffer)
{
	m_pMilSystem=CMilSystem::Instance();
	MmodDraw(M_DEFAULT, m_milIDContext, pBuffer->GetIDBuffer(), M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);
	return 0;
}

/*! @fn void CMilContext::SaveContext(TCHAR * szSaveFile)
*	@brief Context를 파일로 저장한다
*	@param szSaveFile 저장할 파일 이름(경로포함)
*/
void CMilContext::SaveContext(TCHAR * szSaveFile)
{
	MmodSave(szSaveFile, m_milIDContext, M_DEFAULT);
}

void CMilContext::SetMilSystem(CMilSystem * pMilSystem)
{
	m_pMilSystem=pMilSystem;
}

/*! @fn int CMilContext::Alloc(int MFType)
*	@brief 사용할수 있도록 Alloc한다
*/
int CMilContext::Alloc(int MFType)
{
	if(MFType==CMilContext::MFT_GEOMETRIC)
		return MmodAlloc(m_pMilSystem->GetSystemID(), M_GEOMETRIC, M_DEFAULT, &m_milIDContext);
	else
		return MmodAlloc(m_pMilSystem->GetSystemID(), M_GEOMETRIC_CONTROLLED, M_DEFAULT, &m_milIDContext);
}

int CMilContext::Control(int Index, int Type, int Value)
{
	MmodControl(m_milIDContext, Index, Type, Value);
	return 0;
}

int CMilContext::Close(void)
{
	if(m_milIDContext)MmodFree(m_milIDContext);
	m_milIDContext=M_NULL;
	return 0;
}

/*! @fn int CMilContext::PreProcess(void)
*	@brief Context에 대한 적용 여기서 이루어진다
*   @n 예를 들어 Context중 일부삭제혹은 추가가 이루어지면 꼭 PreProcess를 하여야함
*/
int CMilContext::PreProcess(void)
{
	MmodPreprocess(GetIDContext(), M_DEFAULT);
	return 0;
}

/*! @fn int CMilContext::GetModelNumber(void)
*	@brief Context내에 들어 있는 모델의 갯수을 얻는다.
*	@return Context내의 모델갯수를 반환한다.혹 Context가 할당되지 않았을경우에는 -1을 반환
*/
int CMilContext::GetModelNumber(void)
{
	MIL_DOUBLE rv=0;
	if(!GetIDContext())return -1;
	MmodInquire(GetIDContext(), M_CONTEXT, M_NUMBER_MODELS, &rv);
	return (int)rv;
}

/*! @fn int CMilContext::DelContext(int Index)
*	@brief Context중 Index모델을 삭제한다
*/
int CMilContext::DelModel(int Index)
{
	MmodDefine(GetIDContext(), M_DELETE, Index, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
	return 0;
}


/*! @fn int CMilContext::DelAllContext(void)
*	@brief Context내의 모든 모델을 삭제한다
*/
int CMilContext::DelAllModel(void)
{
	int nCount=GetModelNumber();

	if(nCount>0)
	{
		for(int i=0;i<nCount;i++)
		{
			DelModel(0);
		}
	}

	return 0;
}
