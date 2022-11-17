#include "StdAfx.h"
#include "AjinIOPort.h"
#include <AXL.h>
#include <AXD.h>

CAjinIOPort::CAjinIOPort(void)
	: m_nInputModule(0)
	, m_nOutputModule(1)
{
}

CAjinIOPort::~CAjinIOPort(void)
{
}

int CAjinIOPort::SetPort(int nPort, long value)
{
	if (AXT_RT_SUCCESS == AxdoWriteOutport(m_nOutputModule, value)) return 0;
	else return -1;
	return 0;
}

int CAjinIOPort::GetPort(int nPort, long * pValue)
{
	if (AXT_RT_SUCCESS == AxdiReadInport(m_nInputModule, (DWORD *)pValue)) return 0;
	else return -1;
}

int CAjinIOPort::GetOutPort(int nPort, long * pValue)
{
	if (AXT_RT_SUCCESS == AxdoReadOutport(m_nOutputModule, (DWORD *)pValue)) return 0;
	else return -1;
}

int CAjinIOPort::SetBit(int bitNo, int Value)
{
	if (AXT_RT_SUCCESS == AxdoWriteOutportBit(m_nOutputModule, bitNo, Value)) return 0;
	else return -1;
}

BOOL CAjinIOPort::GetBit(int nBitNo)
{
	DWORD dwValue;
	if (AXT_RT_SUCCESS != AxdiReadInportBit(m_nInputModule, nBitNo, &dwValue)) TRACE(_T("Error Input"));
	if (dwValue)
		return TRUE;
	else
		return FALSE;
}

BOOL CAjinIOPort::GetOutBit(int nBitNo)
{
	DWORD dwValue;
	if (AXT_RT_SUCCESS != AxdoReadOutportBit(m_nOutputModule, nBitNo, &dwValue)) TRACE(_T("Get OutBit Error"));
	if (dwValue)
		return TRUE;
	else
		return FALSE;
}


//! return -1 error
int CAjinIOPort::Init()
{
	//IRQ는 싱글보드에서는 무시(즉 자동)
	if (AxlOpen(7) == AXT_RT_SUCCESS)TRACE(_T("라이브러리 초기화 성공"));

	DWORD dwStatus;
	AxdInfoIsDIOModule(&dwStatus);
	if (dwStatus != STATUS_EXIST)
	{
		return -1;
	}
	
	//아진 PCI-DB64R은 0번이 Input 1번이 Output
	long lCount; 
	AxdInfoGetModuleCount(&lCount);//모듈카운트
	if (lCount < 2)return -2; //인풋아웃풋 2개여야 함.
	AxdInfoGetInputCount(m_nInputModule, &lCount);//0번모듈의 입력 채널 개수 가져우기
//	if (lCount < 32)return -3; //0번모듈을 입력 채널 개수가 32개 미만이면 에러리턴
	AxdInfoGetOutputCount(m_nOutputModule, &lCount);
//	if (lCount < 32)return -4; //0번모듈을 출력 채널 개수가 32개 미만이면 에러리턴

	return 0;
}


void CAjinIOPort::Close()
{
	if (AxlClose())TRACE("DIO card is closed");
}


int CAjinIOPort::SetOffset()
{
	for(int i=0;i<32;i++)
		AxdiLevelSetInportBit(m_nInputModule, i, 1);  //0번모듈의 i번째비트가 1이면 실제 신호 High일때 1로 입력들어옴
	for (int i = 0; i<32; i++)
		AxdoLevelSetOutportBit(m_nOutputModule, i, 1);  //1번모듈의 i번째비트가 1이면 실제 설정 1일 때 출력 High음
	return 0;
}
