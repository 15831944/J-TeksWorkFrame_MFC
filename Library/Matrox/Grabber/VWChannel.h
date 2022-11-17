#pragma once
#ifdef _VIEWWORKS
#include "..\MilBase.h"
class CMilBuffer;
#include <functional>

typedef std::function<void(bool)> tfOnAvgStatus;

class MIL_DLLDECL CVWChannel
{
public:
	typedef std::function<void()> tFunction;
	enum GRAB_MODE { GRAB_STOP, GRAB_SINGLE, GRAB_CONTINUOUS };
	CVWChannel();
	~CVWChannel();
	tFunction m_LegendUpdate;
	void SetExpTime(int nExpTime);
	int Init();
	int Grab();
	int GrabContinuous();
	int GrabStop();
	int SetMilBuffer(CMilBuffer * pMilBufProc, CMilBuffer * pMilBufDisp, CMilBuffer *pMilBufView = NULL, CMilBuffer *pMilBufRec = NULL);
	void CallbackGrabEx(int nDetectorID, void * pUserClass, void* pInfoEx);
	void UpdateDriveModeInfo();
	BOOL IsGrabDone();
	int m_nChannel;
	void Close();
	int m_nDetectorID;
	
	CMilBuffer *m_pMilBufDisp;	//! 가장 최종단으로 화면에 보이는 버퍼이다.
	CMilBuffer *m_pMilBufProc;	//! Grab하여 받아오는 최초의 버퍼로 Raw데이터에 상응한다.
	//! 
	CMilBuffer *m_pMilBufView;
	MIL_ID m_milIDApplication;
	MIL_ID m_milIDSystem;
	MIL_ID m_milIDBufProc;
	MIL_ID m_milIDBufDisp;
	MIL_ID m_milIDBufRec;
	BOOL m_bIsGrabDone;
	GRAB_MODE m_modeGrab;
	CMilBuffer *m_pRecMilBuffer;
	void SetRecMilBuffer(CMilBuffer *pRecMilBuffer);
	CMilBuffer *m_pMilBufRec;
	MIL_ID m_miLIDBufRec;
	int m_wndLvlLow;
	int m_wnLvlHigh;
	BOOL m_bUseAutoScale;
	void SetAutoScale();
	void SetWndLvl(int iWndLvlLow, int iWndLvlHigh);
	int m_bImgEnhance;
	void SetImgEnhance(bool bUseImgEnhance);
	void SetLegendCallBack(tFunction funcLegendUpdate);
	//kiy---------------------
	BOOL m_bUseImgAvg;	//Average 사용 여부
	BOOL m_bSetAvg;		//Average 장수 변경 - TooPanel에서 Avg apply버튼 누를때
	int m_nAvgCurCnt;
	int m_nAvgInputCnt;
	CMilBuffer *m_pMilBufAvgAdd;
	CMilBuffer *m_pMilBufAvgResult;
	CMilBuffer *m_pMilCamAvgResult_Histo;
	int SetMilBufferWithAvg(CMilBuffer * pMilBufProc, CMilBuffer * pMilBufDisp, CMilBuffer *pMilBufView, CMilBuffer *pMilBufRec,
							CMilBuffer * pMilBufAvgAdd, CMilBuffer * pMilBufAvgResult, CMilBuffer * pMilBufAvgResultHisto,
							CMilBuffer * pMilTempBufAvgResult, CMilBuffer * pMilTempBufAvgResultHisto);
	MIL_ID m_milIDBufAvgAdd;
	MIL_ID m_milIDBufAvgResult;
	MIL_ID m_milIDBufAvgResult_Histo;

	BOOL m_bAvgIsDone;
	void Stop();

	BOOL m_bPreAvgStatus;
	BOOL m_bCurAvgStatus;
	tfOnAvgStatus m_pfOnAvgStatus;
	void SetOnAvgStatusCallback(tfOnAvgStatus funcOnAvgStatus);

	BOOL m_bSaveImg;

	CMilBuffer *m_pMilTempBufAvgResult;	//! Avg 완료된 버퍼 저장전 임시 보관 용도
	MIL_ID m_milIDBufTempAvtResult;
	CMilBuffer *m_pMilTempBufAvgResult_histo;	//! Avg 완료된 버퍼 저장전 임시 보관 용도
	MIL_ID m_milIDBufTempAvtResult_histo;
	//------------------------
};
#endif

