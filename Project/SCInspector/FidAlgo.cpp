#include "StdAfx.h"
#include "FidAlgo.h"
#include "ConfigInfo.h"
#define _USE_MATH_DEFINES
#include <math.h>

IMPLEMENT_DYNAMIC(CFidAlgo, CAlgorithmBase)
CFidAlgo::CFidAlgo(void)
{
	Init();
}

CFidAlgo::~CFidAlgo(void)
{
	if(pGetMainDlg->m_pFidsetDlg->m_pRegFid == this)
	{
		pGetMainDlg->m_pFidsetDlg->RemoveAll();
	}
	Free();
}

int CFidAlgo::SaveParam(CXmlNode* pNode)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(this->GetRuntimeClass()->m_lpszClassName);
	CXmlNode *pChildNode = pNode->AddNode(szTemp);
    
	pChildNode->SetLong(_T("CamNo"), m_nCamNo);
	pChildNode->SetLong(_T("FidCnt"), m_nFidCnt);
    pChildNode->SetLong(_T("MatRate"),m_iMatRate);
	pChildNode->SetLong(_T("UseScale"),m_iUseScale);
	pChildNode->SetLong(_T("ScaleRate"),m_iScaleRate);
	pChildNode->SetLong(_T("UseAngle"),m_iUseAngle);
	pChildNode->SetLong(_T("AngleRate"),m_iAngleRate);
    pChildNode->SetLong(_T("FidKind"),m_iFidKind); ///Normal, Hal
    pChildNode->SetLong(_T("FidType"),m_iFidType); /// circle, rectangle
	pChildNode->SetDouble(_T("Fid1x"),m_lFidPos[0].x); /// circle, rectangle
	pChildNode->SetDouble(_T("Fid1y"),m_lFidPos[0].y); /// circle, rectangle
	pChildNode->SetDouble(_T("Fid1z"),m_lFidPos[0].z); /// circle, rectangle
	pChildNode->SetDouble(_T("Fid1t"),m_lFidPos[0].t); /// circle, rectangle
	pChildNode->SetDouble(_T("Fid1r"),m_lFidPos[0].r); /// circle, rectangle
	pChildNode->SetDouble(_T("Fid2x"),m_lFidPos[1].x); /// circle, rectangle
	pChildNode->SetDouble(_T("Fid2y"),m_lFidPos[1].y); /// circle, rectangle
	pChildNode->SetDouble(_T("Fid2z"),m_lFidPos[1].z); /// circle, rectangle
	pChildNode->SetDouble(_T("Fid2t"),m_lFidPos[1].t); /// circle, rectangle
	pChildNode->SetDouble(_T("Fid2r"),m_lFidPos[1].r); /// circle, rectangle

	pChildNode->SetLong(_T("Fid1ROIWidth"),m_upROI[0].cx);
	pChildNode->SetLong(_T("Fid1ROIHeight"),m_upROI[0].cy);
	pChildNode->SetLong(_T("Fid2ROIWidth"),m_upROI[1].cx);
	pChildNode->SetLong(_T("Fid2ROIHeight"),m_upROI[1].cy);

	pChildNode->SetLong(_T("Fid1Width"),m_upFid[0].cx);
	pChildNode->SetLong(_T("Fid1Height"),m_upFid[0].cy);
	pChildNode->SetLong(_T("Fid2Width"),m_upFid[1].cx);
	pChildNode->SetLong(_T("Fid2Height"),m_upFid[1].cy);

	pChildNode->SetLong(_T("xrayVol1"),m_xrayCdt[0].m_dblVol);
	pChildNode->SetLong(_T("xrayCur1"),m_xrayCdt[0].m_dblCur);

	pChildNode->SetLong(_T("xrayVol2"),m_xrayCdt[1].m_dblVol);
	pChildNode->SetLong(_T("xrayCur2"),m_xrayCdt[1].m_dblCur);

	pChildNode->SetLong(_T("ImgEnhance1"), m_CamStatus[0].bImgEnhance);
	pChildNode->SetLong(_T("ScrnNormalize1"), m_CamStatus[0].bScrnNormalize);
	pChildNode->SetLong(_T("ImgNormalize1"), m_CamStatus[0].bnState.bBrightNormalize);
	pChildNode->SetLong(_T("BNCutHigh1"), m_CamStatus[0].bnState.bCutHigh);
	pChildNode->SetLong(_T("BNCutLow1"), m_CamStatus[0].bnState.bCutLow);
	pChildNode->SetLong(_T("BNLimit1"), m_CamStatus[0].bnState.uiBNLimit);

	pChildNode->SetLong(_T("ImgEnhance2"), m_CamStatus[1].bImgEnhance);
	pChildNode->SetLong(_T("ScrnNormalize2"), m_CamStatus[1].bScrnNormalize);
	pChildNode->SetLong(_T("ImgNormalize2"), m_CamStatus[1].bnState.bBrightNormalize);
	pChildNode->SetLong(_T("BNCutHigh2"), m_CamStatus[1].bnState.bCutHigh);
	pChildNode->SetLong(_T("BNCutLow2"), m_CamStatus[1].bnState.bCutLow);
	pChildNode->SetLong(_T("BNLimit2"), m_CamStatus[1].bnState.uiBNLimit);

	CAlgorithmBase::SaveParam(pChildNode);

	pChildNode->EndNode();
	CString ModelFolder=CConfigInfo::Instance()->GetModelRepository()+'\\'+CMachineInfo::Instance()->GetModelName();
	SaveFid(ModelFolder.GetBuffer());
	return 0;
}


int CFidAlgo::LoadParam(CXmlNode* pNode)
{
	if(!pNode)return -1;

	m_nCamNo	=   pNode->ReadLong(1, _T("CamNo"));
	m_nFidCnt	=	pNode->ReadLong(0, _T("FidCnt"));
	m_iMatRate	=	pNode->ReadLong(70, _T("MatRate"));
	m_iUseScale	=	pNode->ReadLong(0, _T("UseScale"));
	m_iScaleRate=	pNode->ReadLong(5, _T("ScaleRate"));
	m_iUseAngle	=	pNode->ReadLong(0, _T("UseAngle"));
	m_iAngleRate=	pNode->ReadLong(5, _T("AngleRate"));
	m_iFidKind	=	pNode->ReadLong(0, _T("FidKind"));
	m_iFidType	=	pNode->ReadLong(0, _T("FidType"));
	m_lFidPos[0].x = 	pNode->ReadLong(0, _T("Fid1x"));
	m_lFidPos[0].y = 	pNode->ReadLong(0, _T("Fid1y"));
	m_lFidPos[0].z = 	pNode->ReadLong(0, _T("Fid1z"));  
	m_lFidPos[0].t = 	pNode->ReadLong(0, _T("Fid1t"));  
	m_lFidPos[0].r = 	pNode->ReadLong(0, _T("Fid1r"));  
	m_lFidPos[1].x = 	pNode->ReadLong(0, _T("Fid2x"));
	m_lFidPos[1].y = 	pNode->ReadLong(0, _T("Fid2y"));
	m_lFidPos[1].z = 	pNode->ReadLong(0, _T("Fid2z"));
	m_lFidPos[1].t = 	pNode->ReadLong(0, _T("Fid2t"));
	m_lFidPos[1].r = 	pNode->ReadLong(0, _T("Fid2r"));

	m_upROI[0].cx = pNode->ReadLong(180, _T("Fid1ROIWidth"));
	m_upROI[0].cy = pNode->ReadLong(180, _T("Fid1ROIHeight"));
	m_upROI[1].cx = pNode->ReadLong(180, _T("Fid2ROIWidth"));
	m_upROI[1].cy = pNode->ReadLong(180, _T("Fid2ROIHeight"));

	m_upFid[0].cx = pNode->ReadLong(60, _T("Fid1Width"));
	m_upFid[0].cy = pNode->ReadLong(60, _T("Fid1Height"));
	m_upFid[1].cx = pNode->ReadLong(60, _T("Fid2Width"));
	m_upFid[1].cy = pNode->ReadLong(60, _T("Fid2Height"));

	m_xrayCdt[0].m_dblVol = pNode->ReadLong(90, _T("xrayVol1"));
	m_xrayCdt[0].m_dblCur = pNode->ReadLong(60, _T("xrayCur1"));
	m_xrayCdt[1].m_dblVol = pNode->ReadLong(90, _T("xrayVol2"));
	m_xrayCdt[1].m_dblCur = pNode->ReadLong(60, _T("xrayCur2"));


	m_CamStatus[0].bImgEnhance			= pNode->ReadLong(0, _T("ImgEnhance1"));
	m_CamStatus[0].bScrnNormalize		= pNode->ReadLong(0, _T("ScrnNormalize1"));
	m_CamStatus[0].bnState.bBrightNormalize	= pNode->ReadLong(0, _T("ImgNormalize1"));
	m_CamStatus[0].bnState.bCutHigh		= pNode->ReadLong(1, _T("BNCutHigh1"));
	m_CamStatus[0].bnState.bCutLow		= pNode->ReadLong(1, _T("BNCutLow1"));
	m_CamStatus[0].bnState.uiBNLimit	= pNode->ReadLong(500, _T("BNLimit1"));

	m_CamStatus[1].bImgEnhance			= pNode->ReadLong(0, _T("ImgEnhance2"));
	m_CamStatus[1].bScrnNormalize		= pNode->ReadLong(0, _T("ScrnNormalize2"));
	m_CamStatus[1].bnState.bBrightNormalize	= pNode->ReadLong(0, _T("ImgNormalize2"));
	m_CamStatus[1].bnState.bCutHigh		= pNode->ReadLong(1, _T("BNCutHigh2"));
	m_CamStatus[1].bnState.bCutLow		= pNode->ReadLong(1, _T("BNCutLow2"));
	m_CamStatus[1].bnState.uiBNLimit	= pNode->ReadLong(500, _T("BNLimit2"));

	CAlgorithmBase::LoadParam(pNode);

	CString ModelFolder=CConfigInfo::Instance()->GetModelRepository()+'\\'+CMachineInfo::Instance()->GetModelName();
	LoadFid(ModelFolder.GetBuffer());

//	if(pGetMainDlg->m_pFidsetDlg->m_pRegFid)
//		delete pGetMainDlg->m_pFidsetDlg->m_pRegFid;
	pGetMainDlg->m_pFidsetDlg->m_pRegFid = this;
	return 0;
}

/*! @fn int CFidAlgo::RemoveAllModels(int FidIndex)
	@brief FindIndex의 보정마크의 Model을 모두 지운다
	@param FidIndex 보정마크 번호 */
int CFidAlgo::RemoveAllModels(int FidIndex)
{
	return m_MilContext[FidIndex].DelAllModel();
}

/*! @fn int CFidAlgo::Reg(int FidIndex, CMilBuffer* pMilBuffer, CAxisPos *pPos)
	@brief FindIndex 보정마크에 pMilBuffer의 영상을 등록시킨다.
	@param FidIndex 보정마크 번호 
	@param pMilBuffer 등록할 MilBuffer
	@param pPos	등록할 보정마크 위치 */
int CFidAlgo::Reg(int FidIndex, CMilBuffer* pMilBuffer, CAxisPos *pPos)
{
	CMilBuffer newImage;
	tImageProperties imgProperty;
	m_nFidCnt=FidIndex+1;
	
	pMilBuffer->ConvertToBMPFmt(&newImage);
	m_modFinder.SetBuffer(&newImage);
	m_modFinder.SetContext(&m_MilContext[FidIndex]);
	m_modFinder.SetResult(&m_milResult);
	m_modFinder.Alloc();
	m_modFinder.GetFeature();
	m_modFinder.PreProcess();
	m_lFidPos[FidIndex].x = pPos->x;
	m_lFidPos[FidIndex].y = pPos->y;
	m_lFidPos[FidIndex].t = pPos->t;
	m_lFidPos[FidIndex].r = pPos->r;
	m_lFidPos[FidIndex].z = pPos->z;
	newImage.Close();

	return 0;
}

/*! @fn void CFidAlgo::GetAxisPos(int FidIndex, CdblAxisPos *pPos)
	@brief 보정마크가 있는 모션위치를 가져온다
	@param FidIndex 보정마크 번호 
	@param pPos 보정마크 frame의 위치 */
void CFidAlgo::GetAxisPos(int FidIndex, CdblAxisPos *pPos)
{
	pPos->x = m_lFidPos[FidIndex].x;
	pPos->y = m_lFidPos[FidIndex].y;
	pPos->z = m_lFidPos[FidIndex].z;
	pPos->t = m_lFidPos[FidIndex].t;
	pPos->r = m_lFidPos[FidIndex].r;
}

/*! @fn void CFidAlgo::GetAxisOffset(int FidIndex, tModResult *pResult)
	@brief 보정마크로 틀어진 위치를 um단위로 가져온다
	@param FidIndex 보정마크 번호 
	@param pResult 보정마크 검사를 한 값이 들어있음(pixel 단위) */
void CFidAlgo::GetAxisOffset(int FidIndex, tModResult *pResult)
{
	double centerX=pResult->ImageSize.cx/2;
	double centerY=pResult->ImageSize.cy/2;
	m_upOffset[FidIndex].x = pResult->Pos.dblPosX-centerX;
	m_upOffset[FidIndex].y = pResult->Pos.dblPosY-centerY;

	m_umOffset[FidIndex].x = (m_upOffset[FidIndex].x*m_dblPixelScale.dblCX)*1000;
	if(abs(m_umOffset[FidIndex].x)>22000)m_umOffset[FidIndex].x=0;///보정값이 촤대 FOV보다 클경우 오류로 판단하여 0으로 변경
	m_umOffset[FidIndex].y = (m_upOffset[FidIndex].y*m_dblPixelScale.dblCY)*1000;
	if(abs(m_umOffset[FidIndex].y)>22000)m_umOffset[FidIndex].y=0;///보정값이 촤대 FOV보다 클경우 오류로 판단하여 0으로 변경
#ifdef _DEBUG
	CDbgFile::Instance()->Write(_T("Fid:Pixel x=%f, y%f, um x=%f, y=%f"), m_upOffset[FidIndex].x, m_upOffset[FidIndex].y, m_umOffset[FidIndex].x, m_umOffset[FidIndex].y);
#endif
}

/*! @fn void CFidAlgo::SetPixelScale(CdblSize dblSize)
	@brief 보정마크로 틀어진 위치를 pixel단위에서 um단위로 바꾸기위한 단위값을 설정한다.
	@param dblSize 1pixel의 x,y 크기값 */
void CFidAlgo::SetPixelScale(CdblSize dblSize)
{       
	  
	m_dblPixelScale.dblCX = dblSize.dblCX;
	m_dblPixelScale.dblCY = dblSize.dblCY;
	m_umOffset[0].x = (m_upOffset[0].x*m_dblPixelScale.dblCX)*1000;
	if(abs(m_umOffset[0].x)>22000)m_umOffset[0].x=0;///보정값이 촤대 FOV보다 클경우 오류로 판단하여 0으로 변경
	m_umOffset[0].y = (m_upOffset[0].y*m_dblPixelScale.dblCY)*1000;
	if(abs(m_umOffset[0].y)>22000)m_umOffset[0].y=0;///보정값이 촤대 FOV보다 클경우 오류로 판단하여 0으로 변경
#ifdef _DEBUG
		CDbgFile::Instance()->Write(_T("Fid 0x um=%f up=%f"), m_umOffset[0].x, m_upOffset[0].x);
		CDbgFile::Instance()->Write(_T("Fid 0y um=%f up=%f"), m_umOffset[0].y, m_upOffset[0].y);
#endif
}

/*! @fn BOOL CFidAlgo::IsEmpty(void)
	@brief 티칭된 보정마크가 없는지 확인
	@return TRUE:있음 FALSE:없음 */
BOOL CFidAlgo::IsEmpty(void)
{       
	if(!m_nFidCnt)return TRUE;else return FALSE;
}

void CFidAlgo::ApplyParamToAlgo(void)
{       
	if(m_iUseAngle)m_modFinder.SetSearchAngleRange(m_iAngleRate); else m_modFinder.SetSearchAngleRange(); 
	if(m_iUseScale)m_modFinder.SetSearchScaleRange(m_iScaleRate); else m_modFinder.SetSearchScaleRange();
	m_modFinder.SetAccptanceScore(m_iMatRate);
}

/*! @fn int CFidAlgo::SetDrawOffset(CSize Offset)
	@brief 보정마크의 찾을 결과를 그려줄때 그려줄 좌표를 보정한다
	@param Offset 이동할 Offset(이 위치많큼 이동하여 그려줌 단위:Pixel)	 */
int CFidAlgo::SetDrawOffset(CSize Offset)
{
	return m_modFinder.SetDrawOffset(Offset);
}

/*! @fn int CFidAlgo::Find(int FidIndex, CMilBuffer* pMilBuffer)
	@brief 보정마크의 찾을 결과를 그려줄때 그려줄 좌표를 보정한다
	@param FidIndex 보정마크 번호 
	@param pMilBuffer 보정마크를 찾을 이미지 Mil Buffer	
	@return -1:can't find fid -2:fid is not registrated */
int CFidAlgo::Find(int FidIndex, CMilBuffer* pMilBuffer)
{
	CMilBuffer newImage;
	tImageProperties imgProperty;
	tModResult myResult;
	if(IsEmpty())return -2;

	pMilBuffer->ConvertToBMPFmt(&newImage,M_IMAGE+M_PROC+M_DISP);

	m_modFinder.SetBuffer(&newImage);
	m_modFinder.SetContext(&m_MilContext[FidIndex]);
	ApplyParamToAlgo();
	m_modFinder.SetResult(&m_milResult);
	m_modFinder.Alloc();
	m_modFinder.PreProcess();
	m_modFinder.Find(&myResult);
	GetAxisOffset(FidIndex, &myResult);
#ifdef _DEBUG
	CDbgFile::Instance()->Write(_T("Fid Result:%ld"), myResult.NumResults); 
#endif
	///Result를 그릴 버퍼로 치환함
	m_modFinder.SetBuffer(pMilBuffer);
	if(!myResult.NumResults)return -1;
	newImage.Close();
	return 0;
}

/*! @fn double CFidAlgo::CalcTheta(CPoint origin, CPoint x, CPoint y)
*/
double CFidAlgo::CalcTheta()
{
	double Realpos1x= m_lFidPos[0].x + m_umOffset[0].x;
	double Realpos1y= m_lFidPos[0].y + m_umOffset[0].y;
	double Estpos2x = m_lFidPos[1].x + m_umOffset[0].x;
	double Estpos2y = m_lFidPos[1].y + m_umOffset[0].y;
	double Realpos2x = m_lFidPos[1].x + m_umOffset[1].x;
	double Realpos2y = m_lFidPos[1].y + m_umOffset[1].y;

	
	m_theta = CalcVectorTheta(CPoint((int)Realpos1x, (int)Realpos1y), CPoint((int)Estpos2x, (int)Estpos2y), CPoint((int)Realpos2x,(int)Realpos2y));
#ifdef _DEBUG
	CDbgFile::Instance()->Write(_T("FidPos[0]  x:%f y:%f"), m_lFidPos[0].x, m_lFidPos[0].y);
	CDbgFile::Instance()->Write(_T("FidPos[1]  x:%f y:%f"), m_lFidPos[1].x, m_lFidPos[1].y);
	CDbgFile::Instance()->Write(_T("umOffset[0]x:%f y:%f"), m_umOffset[0].x, m_umOffset[0].y);
	CDbgFile::Instance()->Write(_T("umOffset[1]x:%f y:%f"), m_umOffset[1].x, m_umOffset[1].y);
	CDbgFile::Instance()->Write(_T("RealPos[0] x:%f y:%f"), Realpos1x, Realpos1y);
	CDbgFile::Instance()->Write(_T("RealPos[1] x:%f y:%f"), Realpos2x, Realpos2y);
	CDbgFile::Instance()->Write(_T("EstPos[1]  x:%f y:%f"), Estpos2x, Estpos2y);
	CDbgFile::Instance()->Write(_T("Calc Theta:%f"), m_theta);
#endif
	return m_theta;
}


double CFidAlgo::CalcVectorTheta(CPoint origin, CPoint Pos1, CPoint Pos2)
{
/*	double vector1Scala=sqrt((double)Pos1.x*Pos1.x+(double)Pos1.y*Pos1.y);
	double vector2Scala=sqrt((double)Pos2.x*Pos2.x+(double)Pos2.y*Pos2.y);

	double vector1x=(Pos1.x-origin.x);
	double vector1y=(Pos1.y-origin.y);
	double vector2x=(Pos2.x-origin.x);
	double vector2y=(Pos2.y-origin.y);

	double costheta = (vector1x*vector2x+vector1y*vector2y)/(vector1Scala*vector2Scala);
	double thetaRadian =acos(costheta);
	double thetaDegree = (180*thetaRadian)/M_PI;
	*/

	// 벡터로 변환
	double vector1x = (Pos1.x-origin.x);
	double vector1y = (Pos1.y-origin.y);
	double vector2x = (Pos2.x-origin.x);
	double vector2y = (Pos2.y-origin.y);

	// 두 벡터를 단위 벡터로 만듬.
	double vector1Scala = sqrt( (double)vector1x*vector1x + (double)vector1y*vector1y );
	vector1x /= vector1Scala;
	vector1y /= vector1Scala;

	double vector2Scala = sqrt( (double)vector2x*vector2x + (double)vector2y*vector2y );
	vector2x /= vector2Scala;
	vector2y /= vector2Scala;

	// 내적 구하기
	double costheta = ( vector1x*vector2x + vector1y*vector2y );
	double thetaRadian = acos(costheta);
	double thetaDegree = thetaRadian * (180/M_PI);

	// 각도 양수, 음수 구분
	vector1x*vector2y - vector1y*vector2x >= 0 ? thetaDegree : thetaDegree = -thetaDegree;

	return thetaDegree;
}

/*! @fn int CFidAlgo::AdjustPos(CdblAxisPos *pModified, CdblAxisPos *pOriginal)
	@brief 보정마크를 찾은 결과로 알게된 Offset을 적용하여 위치를 변경시켜준다
	@n (x,y)를 (x',y')을 중심으로 @회전하면 (X,Y)가나옴
	@n X=(x-x')cos@-(y-y')sin@+x'
	@n Y=(x-x')sin@+(y-y')cos@+y'
	@param pOriginal 보정전의 위치값	
	@param pModified 보정후의 위치값 */
int CFidAlgo::AdjustPos(CdblAxisPos *pModified, CdblAxisPos *pOriginal)
{   
	CString CheckFidModifiedXY;

	switch(m_nFidCnt)
	{
	case 0:
		*pModified = *pOriginal;
		break;
	case 1:
		*pModified = *pOriginal;
		pModified->Offset(&m_umOffset[0]);
		break;
	case 2:
		CalcTheta();
		*pModified = *pOriginal;
		pModified->Offset(&m_umOffset[0]);
		pModified->x = (pOriginal->x-m_lFidPos[0].x)*cos(m_theta) - (pOriginal->y-m_lFidPos[0].y)*sin(m_theta)+m_lFidPos[0].x;
		pModified->y = (pOriginal->x-m_lFidPos[0].x)*sin(m_theta) + (pOriginal->y-m_lFidPos[0].y)*cos(m_theta)+m_lFidPos[0].y;
		
		CheckFidModifiedXY.Format(_T("ORGPOS - X:%f  Y:%f"),pOriginal->x,pOriginal->y);
		CTabDetailLogDlg::Instance()->SetTextLog(CheckFidModifiedXY,RGB(0,255,0),false);
		CheckFidModifiedXY.Empty();
		CheckFidModifiedXY.Format(_T("MODIFIEDPOS - X:%f  Y:%f"),pModified->x,pModified->y);
		CTabDetailLogDlg::Instance()->SetTextLog(CheckFidModifiedXY,RGB(0,255,0),false);
		CheckFidModifiedXY.Empty();

		break;
	default:
		break;
	}

	return 0;
}

/*! @fn int CFidAlgo::LoadFid(TCHAR * szModelPath)
	@brief 보정마크를 모델데이터로부터 읽어온다
	@param szModelPath data파일의 폴더 */
int CFidAlgo::LoadFid(TCHAR * szModelPath)
{
	CString FidName;
	for(int i=0;i<m_nFidCnt;i++)
	{
		FidName.Format(_T("%s\\%d.ctx"),szModelPath,i);
		m_MilContext[i].RestoreContext(FidName.GetBuffer());
		m_MilContext[i].PreProcess();
		
		if(m_pMilBuffer[i])
		{
			m_pMilBuffer[i]->Close();
			delete m_pMilBuffer[i];
			m_pMilBuffer[i]=NULL;
		}
		m_pMilBuffer[i] = new CMilBuffer;
		m_pMilBuffer[i]->SetDepth(8);
		m_pMilBuffer[i]->SetAttribute(M_IMAGE+M_PROC+M_DISP);
		m_pMilBuffer[i]->Alloc2D(CSize(m_upFid[i].cx, m_upFid[i].cy));
		m_MilContext[i].GetImage(m_pMilBuffer[i]);
	}
	return 0;
}


void CFidAlgo::UnloadFid(void)
{
	for(int i=0;i<m_nFidCnt;i++)
	{

		if(m_pMilBuffer[i])
		{
			m_pMilBuffer[i]->Close();
			delete m_pMilBuffer[i];
			m_pMilBuffer[i]=NULL;
		}
	}
}

/*! @fn int CFidAlgo::LoadFid(TCHAR * szModelPath)
	@brief 보정마크데이터를 모델데이터파일폴더로 출력한다
	@param szModelPath  모델데이터 폴더 */
int CFidAlgo::SaveFid(TCHAR * szModelPath)
{
	CString FidName;
	for(int i=0;i<m_nFidCnt;i++)
	{
		FidName.Format(_T("%s\\%d.ctx"),szModelPath,i);
		m_MilContext[i].SaveContext(FidName.GetBuffer());
	}

	return 0;
}

int CFidAlgo::Init(void)
{
	m_nCamNo=0;
	m_nFidCnt=0;
	m_iMatRate=70;
	m_iFidKind=0;
	m_iFidType=0;
	m_iUseScale=0;
	m_iUseAngle=0;
	m_iScaleRate=5;
	m_iAngleRate=10;
	m_dblPixelScale.dblCX=10;
	m_dblPixelScale.dblCY=10;

	m_xrayCdt[0].m_dblCur=100;
	m_xrayCdt[0].m_dblVol=100;
	m_xrayCdt[1].m_dblCur=100;
	m_xrayCdt[1].m_dblVol=100;

	memset(&m_lFidPos, 0, sizeof(m_lFidPos));
	memset(&m_CamStatus, 0, sizeof(m_CamStatus));
	for(int i=0;i<4;i++)
		m_pMilBuffer[i]=NULL;
	return 0;
}

/*! @fn int CFidAlgo::Draw()
	@brief 보정마크를 찾은 결과를 그려줌 */
int CFidAlgo::Draw()
{
	m_modFinder.DrawResult();
	return 0;
}
int CFidAlgo::Free(void)
{
	UnloadFid();
	Init();
	return 0;
}
