#include "StdAfx.h"
#include "MMCIOPortH100.h"

CMMCIOPortH100::CMMCIOPortH100(void)
{
}

CMMCIOPortH100::~CMMCIOPortH100(void)
{
}


int CMMCIOPortH100::SetPort(int nPort, long value)
{
#ifdef BIT_REV
	return set_io(nPort, ~value);
#else
	return set_io(nPort, value);
#endif
}

int CMMCIOPortH100::GetPort(int nPort, long * pValue)
{
	long getv=0;
	int rv=get_io(nPort, &getv);
#ifdef BIT_REV
	*pValue=~getv;
#else
	*pValue=getv;
#endif
	return rv;
}

int CMMCIOPortH100::GetOutPort(int nPort, long * pValue)
{
	long getv=0;
	int rv=get_out_io(nPort, &getv);
#ifdef BIT_REV
	*pValue=~getv;
#else
	*pValue=getv;
#endif
	return rv;
}

int CMMCIOPortH100::SetBit(int bitNo, int Value)
{
	int rv;
#ifdef BIT_REV
	if(!Value)
		rv=set_bit(bitNo);
	else
		rv=reset_bit(bitNo);
#else
	if(Value)
		rv=set_bit(bitNo);
	else
		rv=reset_bit(bitNo);
#endif
	return rv;
}

BOOL CMMCIOPortH100::GetBit(int nBitNo)
{
	long PortNo=nBitNo/32;
	long Bit=0x01<<(nBitNo%32);
	long rv=0;
	GetPort(PortNo, &rv);
	if(rv&Bit)return TRUE;
	else return FALSE;
}

BOOL CMMCIOPortH100::GetOutBit(int nBitNo)
{
	long PortNo=nBitNo/32;
	long Bit=0x01<<(nBitNo%32);
	long rv=0;
	GetOutPort(PortNo, &rv);
	if(rv&Bit)return TRUE;
	else return FALSE;
}

void CMMCIOPortH100::LoadEnv(void)
{

	  // input set 
	    m_nIMainPowerCheck		=0;
		m_nIEmergencyCheck		=1;
		m_nILoadSwitch			=2;
		m_nIUnLoadSwitch		=3;
		m_nIFrontDoorSensor1	=4;
		m_nIFrontDoorSensor2	=5;
		m_nIFrontDoorSensor3	=6;
		m_nIFrontDoorSensor4	=7;
		m_nIRearDoorSensor1		=99;
		m_nIRearDoorSensor2		=9;
		m_nIKeySwitch			=99;
		m_nIConvInSensor		=99;
		m_nIConvScanPos1		=99;
		m_nIConvOutSensor		=99;
		m_nIConvPCBReleaseSensor=99;
		m_nIConvPCBHoldingSensor=99;
		m_nIStopperUpSensor		=99;
		m_nIStopperDownSensor	=99;
		m_nIConvPushSensor1		=99;
		m_nIConvPushSensor2		=99;
		m_nIJog_x_up			=16;
		m_nIJog_x_down=17;
		m_nIJog_y_up=18;
	    m_nIJog_y_down=19;
		m_nIJog_dx_up=24;
		m_nIJog_dx_down=25;
		m_nIJog_z_up=22;
		m_nIJog_z_down=23;
		m_nINextMC=99;
		m_nIPRIVIOUS=99;
		m_nIInShutterUpSensor=99;
		m_nIInShutterDownSensor=99;
		m_nIOutShutterUpSensor=99;
		m_nIOutShutterDownSensor=99;
		m_nILoadUnloadCheck=99;
		m_nIWarningLampCheck=99;
		m_nIConvScanPos2=99;
		m_nIJog_r_up=20;
		m_nIJog_r_down=21;
		m_LimtiR = 12;
		//m_nIStartStopSwitch=99;
       

		//output set
		m_nODetectorSelect1=0;
		m_nODetectorSelect2=99;
		m_nOWarningLamp=99;
		m_nOInsideLight=1;
		m_nODetectorSelectToshiba=99;
		m_nOLaserPointer=3;
		m_nOsidePusher=99;
		m_nOConvHoldingPCB=99;
		m_nOStopperUp=99;
		m_nOInShutterDown=99;
		m_nOInShutterUp=99;
		m_nOOutShutterDown=99;
		m_nOOutShutterUp=99;
		m_nONextMC=99;
		m_nOPRIVIOUS_1_PCB_AVA=99;
		m_nOGoodNG=99;
		m_nOConvFwdBwd=99;
		m_nOConvGo=99;
		m_nOBuzzer=99;
		m_nOTLampR=99;
		m_nOTLampG=99;
		m_nOTLampB=99;
		m_nOStartStopLamp=99;
	//	m_nOSpaer6=99;
	//	m_nOSpaer9=99;
	//	m_nOSpaer16=99;
	//	m_nOSpaer20=99;
	//	m_nOSpaer21=99;
	//	m_nOSpaer22=99;
	//	m_nOSpaer11=99;
		m_RotateGoHome=4;
		m_RotateHomeSavePos=5;
        m_nOAOILamp=16;
}