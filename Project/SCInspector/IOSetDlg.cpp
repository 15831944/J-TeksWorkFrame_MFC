// IOSetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Defines.h"
#include "SCInspector.h"
#include "IOSetDlg.h"
#include "SCInspIO.h"


// CIOSetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CIOSetDlg, CDialog)

CIOSetDlg::CIOSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIOSetDlg::IDD, pParent)
	, m_nOSpare0(0)
	, m_nOSpare31(0)
	, m_LableName0(_T(""))
	, m_LableName1(_T(""))
	, m_LableName2(_T(""))
	, m_LableName3(_T(""))
	, m_LableName4(_T(""))
	, m_LableName5(_T(""))
	, m_LableName6(_T(""))
	, m_LableName7(_T(""))
	, m_LableName8(_T(""))
	, m_LableName9(_T(""))
	, m_LableName10(_T(""))
	, m_LableName11(_T(""))
	, m_LableName12(_T(""))
	, m_LableName13(_T(""))
	, m_LableName14(_T(""))
	, m_LableName15(_T(""))
	, m_LableName16(_T(""))
	, m_LableName17(_T(""))
	, m_LableName18(_T(""))
	, m_LableName19(_T(""))
	, m_LableName20(_T(""))
	, m_LableName21(_T(""))
	, m_LableName22(_T(""))
	, m_LableName23(_T(""))
	, m_LableName24(_T(""))
	, m_LableName25(_T(""))
	, m_LableName26(_T(""))
	, m_LableName27(_T(""))
	, m_LableName28(_T(""))
	, m_LableName29(_T(""))
	, m_nISpare22(22)
	, m_nISpare23(23)
	, m_nISpare24(24)
	, m_nISpare25(25)
	, m_nISpare26(26)
	, m_nISpare27(27)
	, m_nISpare28(28)
	, m_nISpare29(29)
	, m_nISpare30(30)
	, m_nISpare31(31)
	, m_InputLableName0(_T(""))
	, m_InputLableName1(_T(""))
	, m_InputLableName2(_T(""))
	, m_InputLableName3(_T(""))
	, m_InputLableName4(_T(""))
	, m_InputLableName5(_T(""))
	, m_InputLableName6(_T(""))
	, m_InputLableName7(_T(""))
	, m_InputLableName8(_T(""))
	, m_InputLableName9(_T(""))
	, m_InputLableName10(_T(""))
	, m_InputLableName11(_T(""))
	, m_InputLableName12(_T(""))
	, m_InputLableName13(_T(""))
	, m_InputLableName14(_T(""))
	, m_InputLableName15(_T(""))
	, m_InputLableName16(_T(""))
	, m_InputLableName17(_T(""))
	, m_InputLableName18(_T(""))
	, m_InputLableName19(_T(""))
	, m_InputLableName20(_T(""))
	, m_InputLableName21(_T(""))
	, m_InputLableName22(_T(""))
	, m_InputLableName23(_T(""))
	, m_InputLableName24(_T(""))
	, m_InputLableName25(_T(""))
	, m_InputLableName26(_T(""))
	, m_InputLableName27(_T(""))
	, m_InputLableName28(_T(""))
	, m_InputLableName29(_T(""))
	, m_InputLableName30(_T(""))
	, m_InputLableName31(_T(""))
	, m_InputLableName32(_T(""))
	, m_bChkInput0(FALSE)
	, m_bChkInput1(FALSE)
	, m_bChkInput2(FALSE)
	, m_bChkInput3(FALSE)
	, m_bChkInput4(FALSE)
	, m_bChkInput5(FALSE)
	, m_bChkInput6(FALSE)
	, m_bChkInput7(FALSE)
	, m_bChkInput8(FALSE)
	, m_bChkInput9(FALSE)
	, m_bChkInput10(FALSE)
	, m_bChkInput11(FALSE)
	, m_bChkInput12(FALSE)
	, m_bChkInput13(FALSE)
	, m_bChkInput14(FALSE)
	, m_bChkInput15(FALSE)
	, m_bChkInput16(FALSE)
	, m_bChkInput17(FALSE)
	, m_bChkInput18(FALSE)
	, m_bChkInput19(FALSE)
	, m_bChkInput20(FALSE)
	, m_bChkInput21(FALSE)
	, m_bChkInput22(FALSE)
	, m_bChkInput23(FALSE)
	, m_bChkInput24(FALSE)
	, m_bChkInput25(FALSE)
	, m_bChkInput26(FALSE)
	, m_bChkInput27(FALSE)
	, m_bChkInput28(FALSE)
	, m_bChkInput29(FALSE)
	, m_bChkInput30(FALSE)
	, m_bChkInput31(FALSE)
	, m_bChkInput32(FALSE)
	, m_LableName30(_T(""))
	, m_LableName31(_T(""))
{

}

CIOSetDlg::~CIOSetDlg()
{
}

void CIOSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORTNUM0, m_nOSpare0);
	DDX_Text(pDX, IDC_PORTNUM1, m_nOStage[0].VisionReady);
	DDX_Text(pDX, IDC_PORTNUM2, m_nOStage[0].VisionNotReady);
	DDX_Text(pDX, IDC_PORTNUM3, m_nOStage[0].Seq.Pos[0].CapDone);
	DDX_Text(pDX, IDC_PORTNUM4, m_nOStage[0].Seq.Pos[0].NG);
	DDX_Text(pDX, IDC_PORTNUM5, m_nOStage[0].Seq.Pos[1].CapDone);
	DDX_Text(pDX, IDC_PORTNUM6, m_nOStage[0].Seq.Pos[1].NG);
	DDX_Text(pDX, IDC_PORTNUM7, m_nOStage[0].Seq.Pos[2].CapDone);
	DDX_Text(pDX, IDC_PORTNUM8, m_nOStage[0].Seq.Pos[2].NG);
	DDX_Text(pDX, IDC_PORTNUM9, m_nOStage[0].Seq.Pos[3].CapDone);
	DDX_Text(pDX, IDC_PORTNUM10, m_nOStage[0].Seq.Pos[3].NG);
	DDX_Text(pDX, IDC_PORTNUM11, m_nOStage[0].Seq.Pos[4].CapDone);
	DDX_Text(pDX, IDC_PORTNUM12, m_nOStage[0].Seq.Pos[4].NG);
	DDX_Text(pDX, IDC_PORTNUM13, m_nOStage[0].Seq.Pos[5].CapDone);
	DDX_Text(pDX, IDC_PORTNUM14, m_nOStage[0].Seq.Pos[5].NG);
	DDX_Text(pDX, IDC_PORTNUM15, m_nOStage[0].Seq.UnitInspDone);
	DDX_Text(pDX, IDC_PORTNUM16, m_nOStage[1].VisionReady);
	DDX_Text(pDX, IDC_PORTNUM17, m_nOStage[1].VisionNotReady);
	DDX_Text(pDX, IDC_PORTNUM18, m_nOStage[1].Seq.Pos[0].CapDone);
	DDX_Text(pDX, IDC_PORTNUM19, m_nOStage[1].Seq.Pos[0].NG);
	DDX_Text(pDX, IDC_PORTNUM20, m_nOStage[1].Seq.Pos[1].CapDone);
	DDX_Text(pDX, IDC_PORTNUM21, m_nOStage[1].Seq.Pos[1].NG);
	DDX_Text(pDX, IDC_PORTNUM22, m_nOStage[1].Seq.Pos[2].CapDone);
	DDX_Text(pDX, IDC_PORTNUM23, m_nOStage[1].Seq.Pos[2].NG);
	DDX_Text(pDX, IDC_PORTNUM24, m_nOStage[1].Seq.Pos[3].CapDone);
	DDX_Text(pDX, IDC_PORTNUM25, m_nOStage[1].Seq.Pos[3].NG);
	DDX_Text(pDX, IDC_PORTNUM26, m_nOStage[1].Seq.Pos[4].CapDone);
	DDX_Text(pDX, IDC_PORTNUM27, m_nOStage[1].Seq.Pos[4].NG);
	DDX_Text(pDX, IDC_PORTNUM28, m_nOStage[1].Seq.Pos[5].CapDone);
	DDX_Text(pDX, IDC_PORTNUM29, m_nOStage[1].Seq.Pos[5].NG);
	DDX_Text(pDX, IDC_PORTNUM30, m_nOStage[1].Seq.UnitInspDone);
	DDX_Text(pDX, IDC_PORTNUM31, m_nOSpare31);
	DDX_Text(pDX, IDC_PORTNAME0, m_LableName0);
	DDX_Text(pDX, IDC_PORTNAME1, m_LableName1);
	DDX_Text(pDX, IDC_PORTNAME2, m_LableName2);
	DDX_Text(pDX, IDC_PORTNAME3, m_LableName3);
	DDX_Text(pDX, IDC_PORTNAME4, m_LableName4);
	DDX_Text(pDX, IDC_PORTNAME5, m_LableName5);
	DDX_Text(pDX, IDC_PORTNAME6, m_LableName6);
	DDX_Text(pDX, IDC_PORTNAME7, m_LableName7);
	DDX_Text(pDX, IDC_PORTNAME8, m_LableName8);
	DDX_Text(pDX, IDC_PORTNAME9, m_LableName9);
	DDX_Text(pDX, IDC_PORTNAME10, m_LableName10);
	DDX_Text(pDX, IDC_PORTNAME11, m_LableName11);
	DDX_Text(pDX, IDC_PORTNAME12, m_LableName12);
	DDX_Text(pDX, IDC_PORTNAME13, m_LableName13);
	DDX_Text(pDX, IDC_PORTNAME14, m_LableName14);
	DDX_Text(pDX, IDC_PORTNAME15, m_LableName15);
	DDX_Text(pDX, IDC_PORTNAME16, m_LableName16);
	DDX_Text(pDX, IDC_PORTNAME17, m_LableName17);
	DDX_Text(pDX, IDC_PORTNAME18, m_LableName18);
	DDX_Text(pDX, IDC_PORTNAME19, m_LableName19);
	DDX_Text(pDX, IDC_PORTNAME20, m_LableName20);
	DDX_Text(pDX, IDC_PORTNAME21, m_LableName21);
	DDX_Text(pDX, IDC_PORTNAME22, m_LableName22);
	DDX_Text(pDX, IDC_PORTNAME23, m_LableName23);
	DDX_Text(pDX, IDC_PORTNAME24, m_LableName24);
	DDX_Text(pDX, IDC_PORTNAME25, m_LableName25);
	DDX_Text(pDX, IDC_PORTNAME26, m_LableName26);
	DDX_Text(pDX, IDC_PORTNAME27, m_LableName27);
	DDX_Text(pDX, IDC_PORTNAME28, m_LableName28);
	DDX_Text(pDX, IDC_PORTNAME29, m_LableName29);
	DDX_Text(pDX, IDC_PORTNAME30, m_LableName30);
	DDX_Text(pDX, IDC_PORTNAME31, m_LableName31);

	DDX_Text(pDX, IDC_INPUTPORTNAME0, m_InputLableName0);
	DDX_Text(pDX, IDC_INPUTPORTNAME1, m_InputLableName1);
	DDX_Text(pDX, IDC_INPUTPORTNAME2, m_InputLableName2);
	DDX_Text(pDX, IDC_INPUTPORTNAME3, m_InputLableName3);
	DDX_Text(pDX, IDC_INPUTPORTNAME4, m_InputLableName4);
	DDX_Text(pDX, IDC_INPUTPORTNAME5, m_InputLableName5);
	DDX_Text(pDX, IDC_INPUTPORTNAME6, m_InputLableName6);
	DDX_Text(pDX, IDC_INPUTPORTNAME7, m_InputLableName7);
	DDX_Text(pDX, IDC_INPUTPORTNAME8, m_InputLableName8);
	DDX_Text(pDX, IDC_INPUTPORTNAME9, m_InputLableName9);
	DDX_Text(pDX, IDC_INPUTPORTNAME10, m_InputLableName10);
	DDX_Text(pDX, IDC_INPUTPORTNAME11, m_InputLableName11);
	DDX_Text(pDX, IDC_INPUTPORTNAME12, m_InputLableName12);
	DDX_Text(pDX, IDC_INPUTPORTNAME13, m_InputLableName13);
	DDX_Text(pDX, IDC_INPUTPORTNAME14, m_InputLableName14);
	DDX_Text(pDX, IDC_INPUTPORTNAME15, m_InputLableName15);
	DDX_Text(pDX, IDC_INPUTPORTNAME16, m_InputLableName16);
	DDX_Text(pDX, IDC_INPUTPORTNAME17, m_InputLableName17);
	DDX_Text(pDX, IDC_INPUTPORTNAME18, m_InputLableName18);
	DDX_Text(pDX, IDC_INPUTPORTNAME19, m_InputLableName19);
	DDX_Text(pDX, IDC_INPUTPORTNAME20, m_InputLableName20);
	DDX_Text(pDX, IDC_INPUTPORTNAME21, m_InputLableName21);
	DDX_Text(pDX, IDC_INPUTPORTNAME22, m_InputLableName22);
	DDX_Text(pDX, IDC_INPUTPORTNAME23, m_InputLableName23);
	DDX_Text(pDX, IDC_INPUTPORTNAME24, m_InputLableName24);
	DDX_Text(pDX, IDC_INPUTPORTNAME25, m_InputLableName25);
	DDX_Text(pDX, IDC_INPUTPORTNAME26, m_InputLableName26);
	DDX_Text(pDX, IDC_INPUTPORTNAME27, m_InputLableName27);
	DDX_Text(pDX, IDC_INPUTPORTNAME28, m_InputLableName28);
	DDX_Text(pDX, IDC_INPUTPORTNAME29, m_InputLableName29);
	DDX_Text(pDX, IDC_INPUTPORTNAME30, m_InputLableName30);
	DDX_Text(pDX, IDC_INPUTPORTNAME31, m_InputLableName31);
	DDX_Text(pDX, IDC_PORTNUMINPUT0, m_nIStage[0].MANUAL);
	//	DDX_Text(pDX, IDC_PORTNUMINPUT1, m_nIStage[0].ERR);
	//	DDX_Text(pDX, IDC_PORTNUMINPUT2, m_nIStage[0].RUN);
	DDX_Text(pDX, IDC_PORTNUMINPUT3, m_nIStage[0].IdleReset);
	DDX_Text(pDX, IDC_PORTNUMINPUT4, m_nIStage[0].UnitStart);
	DDX_Text(pDX, IDC_PORTNUMINPUT5, m_nIStage[0].MotionDone[0]);
	DDX_Text(pDX, IDC_PORTNUMINPUT6, m_nIStage[0].MotionDone[1]);
	DDX_Text(pDX, IDC_PORTNUMINPUT7, m_nIStage[0].MotionDone[2]);
	DDX_Text(pDX, IDC_PORTNUMINPUT8, m_nIStage[0].MotionDone[3]);
	DDX_Text(pDX, IDC_PORTNUMINPUT9, m_nIStage[0].MotionDone[4]);
	DDX_Text(pDX, IDC_PORTNUMINPUT10, m_nIStage[0].MotionDone[5]);
	DDX_Text(pDX, IDC_PORTNUMINPUT11, m_nIStage[1].MANUAL);
	//DDX_Text(pDX, IDC_PORTNUMINPUT11, m_nIStage[1].EMG);
	//DDX_Text(pDX, IDC_PORTNUMINPUT12, m_nIStage[1].ERR);
	DDX_Text(pDX, IDC_PORTNUMINPUT13, m_nIStage[1].RUN);
	DDX_Text(pDX, IDC_PORTNUMINPUT14, m_nIStage[1].IdleReset);
	DDX_Text(pDX, IDC_PORTNUMINPUT15, m_nIStage[1].UnitStart);
	DDX_Text(pDX, IDC_PORTNUMINPUT16, m_nIStage[1].MotionDone[0]);
	DDX_Text(pDX, IDC_PORTNUMINPUT17, m_nIStage[1].MotionDone[1]);
	DDX_Text(pDX, IDC_PORTNUMINPUT18, m_nIStage[1].MotionDone[2]);
	DDX_Text(pDX, IDC_PORTNUMINPUT19, m_nIStage[1].MotionDone[3]);
	DDX_Text(pDX, IDC_PORTNUMINPUT20, m_nIStage[1].MotionDone[4]);
	DDX_Text(pDX, IDC_PORTNUMINPUT21, m_nIStage[1].MotionDone[5]);
	DDX_Text(pDX, IDC_PORTNUMINPUT22, m_nISpare22);
	DDX_Text(pDX, IDC_PORTNUMINPUT23, m_nISpare23);
	DDX_Text(pDX, IDC_PORTNUMINPUT24, m_nISpare24);
	DDX_Text(pDX, IDC_PORTNUMINPUT25, m_nISpare25);
	DDX_Text(pDX, IDC_PORTNUMINPUT26, m_nISpare26);
	DDX_Text(pDX, IDC_PORTNUMINPUT27, m_nISpare27);
	DDX_Text(pDX, IDC_PORTNUMINPUT28, m_nISpare28);
	DDX_Text(pDX, IDC_PORTNUMINPUT29, m_nISpare29);
	DDX_Text(pDX, IDC_PORTNUMINPUT30, m_nISpare30);
	DDX_Text(pDX, IDC_PORTNUMINPUT31, m_nISpare31);

	DDX_Control(pDX, IDC_OUTPUTIOCHECK0, m_OutputLed0);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK1, m_OutputLed1);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK2, m_OutputLed2);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK3, m_OutputLed3);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK4, m_OutputLed4);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK5, m_OutputLed5);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK6, m_OutputLed6);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK7, m_OutputLed7);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK8, m_OutputLed8);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK9, m_OutputLed9);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK10, m_OutputLed10);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK11, m_OutputLed11);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK12, m_OutputLed12);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK13, m_OutputLed13);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK14, m_OutputLed14);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK15, m_OutputLed15);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK16, m_OutputLed16);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK17, m_OutputLed17);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK18, m_OutputLed18);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK19, m_OutputLed19);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK20, m_OutputLed20);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK21, m_OutputLed21);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK22, m_OutputLed22);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK23, m_OutputLed23);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK24, m_OutputLed24);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK25, m_OutputLed25);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK26, m_OutputLed26);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK27, m_OutputLed27);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK28, m_OutputLed28);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK29, m_OutputLed29);
	DDX_Control(pDX, IDC_INPUTIOCHECK0, m_InputLed0);
	DDX_Control(pDX, IDC_INPUTIOCHECK1, m_InputLed1);
	DDX_Control(pDX, IDC_INPUTIOCHECK2, m_InputLed2);
	DDX_Control(pDX, IDC_INPUTIOCHECK3, m_InputLed3);
	DDX_Control(pDX, IDC_INPUTIOCHECK4, m_InputLed4);
	DDX_Control(pDX, IDC_INPUTIOCHECK5, m_InputLed5);
	DDX_Control(pDX, IDC_INPUTIOCHECK6, m_InputLed6);
	DDX_Control(pDX, IDC_INPUTIOCHECK7, m_InputLed7);
	DDX_Control(pDX, IDC_INPUTIOCHECK8, m_InputLed8);
	DDX_Control(pDX, IDC_INPUTIOCHECK9, m_InputLed9);
	DDX_Control(pDX, IDC_INPUTIOCHECK10, m_InputLed10);
	DDX_Control(pDX, IDC_INPUTIOCHECK11, m_InputLed11);
	DDX_Control(pDX, IDC_INPUTIOCHECK12, m_InputLed12);
	DDX_Control(pDX, IDC_INPUTIOCHECK13, m_InputLed13);
	DDX_Control(pDX, IDC_INPUTIOCHECK14, m_InputLed14);
	DDX_Control(pDX, IDC_INPUTIOCHECK15, m_InputLed15);
	DDX_Control(pDX, IDC_INPUTIOCHECK16, m_InputLed16);
	DDX_Control(pDX, IDC_INPUTIOCHECK17, m_InputLed17);
	DDX_Control(pDX, IDC_INPUTIOCHECK18, m_InputLed18);
	DDX_Control(pDX, IDC_INPUTIOCHECK19, m_InputLed19);
	DDX_Control(pDX, IDC_INPUTIOCHECK20, m_InputLed20);
	DDX_Control(pDX, IDC_INPUTIOCHECK21, m_InputLed21);
	DDX_Control(pDX, IDC_INPUTIOCHECK22, m_InputLed22);
	DDX_Control(pDX, IDC_INPUTIOCHECK23, m_InputLed23);
	DDX_Control(pDX, IDC_INPUTIOCHECK24, m_InputLed24);
	DDX_Control(pDX, IDC_INPUTIOCHECK25, m_InputLed25);
	DDX_Control(pDX, IDC_INPUTIOCHECK26, m_InputLed26);
	DDX_Control(pDX, IDC_INPUTIOCHECK27, m_InputLed27);
	DDX_Control(pDX, IDC_INPUTIOCHECK28, m_InputLed28);
	DDX_Control(pDX, IDC_INPUTIOCHECK29, m_InputLed29);
	DDX_Control(pDX, IDC_INPUTIOCHECK30, m_InputLed30);
	DDX_Control(pDX, IDC_INPUTIOCHECK31, m_InputLed31);

	DDX_Check(pDX, IDC_INPUTIOCHECK0, m_bChkInput0);
	DDX_Check(pDX, IDC_INPUTIOCHECK1, m_bChkInput1);
	DDX_Check(pDX, IDC_INPUTIOCHECK2, m_bChkInput2);
	DDX_Check(pDX, IDC_INPUTIOCHECK3, m_bChkInput3);
	DDX_Check(pDX, IDC_INPUTIOCHECK4, m_bChkInput4);
	DDX_Check(pDX, IDC_INPUTIOCHECK5, m_bChkInput5);
	DDX_Check(pDX, IDC_INPUTIOCHECK6, m_bChkInput6);
	DDX_Check(pDX, IDC_INPUTIOCHECK7, m_bChkInput7);
	DDX_Check(pDX, IDC_INPUTIOCHECK8, m_bChkInput8);
	DDX_Check(pDX, IDC_INPUTIOCHECK9, m_bChkInput9);
	DDX_Check(pDX, IDC_INPUTIOCHECK10, m_bChkInput10);
	DDX_Check(pDX, IDC_INPUTIOCHECK11, m_bChkInput11);
	DDX_Check(pDX, IDC_INPUTIOCHECK12, m_bChkInput12);
	DDX_Check(pDX, IDC_INPUTIOCHECK13, m_bChkInput13);
	DDX_Check(pDX, IDC_INPUTIOCHECK14, m_bChkInput14);
	DDX_Check(pDX, IDC_INPUTIOCHECK15, m_bChkInput15);
	DDX_Check(pDX, IDC_INPUTIOCHECK16, m_bChkInput16);
	DDX_Check(pDX, IDC_INPUTIOCHECK17, m_bChkInput17);
	DDX_Check(pDX, IDC_INPUTIOCHECK18, m_bChkInput18);
	DDX_Check(pDX, IDC_INPUTIOCHECK19, m_bChkInput19);
	DDX_Check(pDX, IDC_INPUTIOCHECK20, m_bChkInput20);
	DDX_Check(pDX, IDC_INPUTIOCHECK21, m_bChkInput21);
	DDX_Check(pDX, IDC_INPUTIOCHECK22, m_bChkInput22);
	DDX_Check(pDX, IDC_INPUTIOCHECK23, m_bChkInput23);
	DDX_Check(pDX, IDC_INPUTIOCHECK24, m_bChkInput24);
	DDX_Check(pDX, IDC_INPUTIOCHECK25, m_bChkInput25);
	DDX_Check(pDX, IDC_INPUTIOCHECK26, m_bChkInput26);
	DDX_Check(pDX, IDC_INPUTIOCHECK27, m_bChkInput27);
	DDX_Check(pDX, IDC_INPUTIOCHECK28, m_bChkInput28);
	DDX_Check(pDX, IDC_INPUTIOCHECK29, m_bChkInput29);
	DDX_Check(pDX, IDC_INPUTIOCHECK30, m_bChkInput30);
	DDX_Check(pDX, IDC_INPUTIOCHECK31, m_bChkInput31);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK30, m_OutputLed30);
	DDX_Control(pDX, IDC_OUTPUTIOCHECK31, m_OutputLed31);
	DDX_Control(pDX, IDOK, m_cSave);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
}


BEGIN_MESSAGE_MAP(CIOSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CIOSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK0, &CIOSetDlg::OnBnClickedOutputiocheck0)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK1, &CIOSetDlg::OnBnClickedOutputiocheck1)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK2, &CIOSetDlg::OnBnClickedOutputiocheck2)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK3, &CIOSetDlg::OnBnClickedOutputiocheck3)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK4, &CIOSetDlg::OnBnClickedOutputiocheck4)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK5, &CIOSetDlg::OnBnClickedOutputiocheck5)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK6, &CIOSetDlg::OnBnClickedOutputiocheck6)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK7, &CIOSetDlg::OnBnClickedOutputiocheck7)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK8, &CIOSetDlg::OnBnClickedOutputiocheck8)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK9, &CIOSetDlg::OnBnClickedOutputiocheck9)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK10, &CIOSetDlg::OnBnClickedOutputiocheck10)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK11, &CIOSetDlg::OnBnClickedOutputiocheck11)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK12, &CIOSetDlg::OnBnClickedOutputiocheck12)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK13, &CIOSetDlg::OnBnClickedOutputiocheck13)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK14, &CIOSetDlg::OnBnClickedOutputiocheck14)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK15, &CIOSetDlg::OnBnClickedOutputiocheck15)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK16, &CIOSetDlg::OnBnClickedOutputiocheck16)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK17, &CIOSetDlg::OnBnClickedOutputiocheck17)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK18, &CIOSetDlg::OnBnClickedOutputiocheck18)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK19, &CIOSetDlg::OnBnClickedOutputiocheck19)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK20, &CIOSetDlg::OnBnClickedOutputiocheck20)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK21, &CIOSetDlg::OnBnClickedOutputiocheck21)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK22, &CIOSetDlg::OnBnClickedOutputiocheck22)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK23, &CIOSetDlg::OnBnClickedOutputiocheck23)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK24, &CIOSetDlg::OnBnClickedOutputiocheck24)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK25, &CIOSetDlg::OnBnClickedOutputiocheck25)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK26, &CIOSetDlg::OnBnClickedOutputiocheck26)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK27, &CIOSetDlg::OnBnClickedOutputiocheck27)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK28, &CIOSetDlg::OnBnClickedOutputiocheck28)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK29, &CIOSetDlg::OnBnClickedOutputiocheck29)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK30, &CIOSetDlg::OnBnClickedOutputiocheck30)
	ON_BN_CLICKED(IDC_OUTPUTIOCHECK31, &CIOSetDlg::OnBnClickedOutputiocheck31)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CIOSetDlg 메시지 처리기입니다.
#define m_nOStageSetDef(stage, n) \
pMyCfg->Set(m_nOStage[stage].Seq.Pos[n].CapDone, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-"#stage"-.Seq.Pos-"#n"-.CapDone"));\
pMyCfg->Set(m_nOStage[stage].Seq.Pos[n].NG, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-"#stage"-.Seq.Pos-"#n"-.NG"));

#define m_nIStageSetDef(stage, n) \
pMyCfg->Set(m_nIStage[stage].MotionDone[n], (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-"#stage"-.MotionDone-"#n"-"));
void CIOSetDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString MachineInputName, MachineOutputName, MachineInputLableName, MachineOutputLabelName, strProductName;
	CCfgFile* pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pMyCfg->Open();
	pMyCfg->SetDepth(2);

	strProductName = PRODUCT_NAME;
	MachineInputName.Format(_T("%s%s"), strProductName, _T("_INPUTSET"));
	MachineOutputName.Format(_T("%s%s"), strProductName, _T("_OUTPUTSET"));
	MachineInputLableName = MachineInputName + _T("_NAME");
	MachineOutputLabelName = MachineOutputName + _T("_NAME");


	//! Input port number setting
#pragma region Input port number setting
	pMyCfg->Set(m_nIStage[0].MANUAL, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.MANUAL"));
	//pMyCfg->Set(m_nIStage[0].ERR, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.ERR"));
	pMyCfg->Set(m_nIStage[0].RUN, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.RUN"));
	pMyCfg->Set(m_nIStage[0].IdleReset, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.IdleReset"));//default Input number 3
	pMyCfg->Set(m_nIStage[0].UnitStart, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.UnitStart"));//default Input number 4

	m_nIStageSetDef(0, 0);
	m_nIStageSetDef(0, 1);
	m_nIStageSetDef(0, 2);
	m_nIStageSetDef(0, 3);
	m_nIStageSetDef(0, 4);
	m_nIStageSetDef(0, 5);


	pMyCfg->Set(m_nIStage[1].MANUAL, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.MANUAL"));
	//pMyCfg->Set(m_nIStage[1].ERR, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.ERR"));
	pMyCfg->Set(m_nIStage[1].RUN, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.RUN"));
	pMyCfg->Set(m_nIStage[1].IdleReset, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.IdleReset"));//default Input number 3
	pMyCfg->Set(m_nIStage[1].UnitStart, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.UnitStart"));//default Input number 4

	m_nIStageSetDef(1, 0);
	m_nIStageSetDef(1, 1);
	m_nIStageSetDef(1, 2);
	m_nIStageSetDef(1, 3);
	m_nIStageSetDef(1, 4);
	m_nIStageSetDef(1, 5);
#pragma endregion

	//! Ouput port number settting
#pragma region Output port number setting 
	pMyCfg->Set(m_nOStage[0].VisionReady, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-0-.VisionReady"));
	pMyCfg->Set(m_nOStage[0].VisionNotReady, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-0-.VisionNotReady"));
	m_nOStageSetDef(0, 0);
	m_nOStageSetDef(0, 1);
	m_nOStageSetDef(0, 2);
	m_nOStageSetDef(0, 3);
	m_nOStageSetDef(0, 4);
	m_nOStageSetDef(0, 5);
	pMyCfg->Set(m_nOStage[0].Seq.UnitInspDone, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-0-.Seq.UnitInspDone"));

	pMyCfg->Set(m_nOStage[1].VisionReady, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-1-.VisionReady"));
	pMyCfg->Set(m_nOStage[1].VisionNotReady, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-1-.VisionNotReady"));
	m_nOStageSetDef(1, 0);
	m_nOStageSetDef(1, 1);
	m_nOStageSetDef(1, 2);
	m_nOStageSetDef(1, 3);
	m_nOStageSetDef(1, 4);
	m_nOStageSetDef(1, 5);
	pMyCfg->Set(m_nOStage[1].Seq.UnitInspDone, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-1-.Seq.UnitInspDone"));
#pragma endregion

	/************Input***************/
#pragma region InputLabelName 정의
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName0, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName0"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName1, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName1"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName2, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName2"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName3, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName3"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName4, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName4"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName5, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName5"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName6, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName6"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName7, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName7"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName8, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName8"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName9, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName9"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName10, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName10"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName11, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName11"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName12, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName12"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName13, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName13"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName14, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName14"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName15, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName15"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName16, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName16"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName17, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName17"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName18, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName18"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName19, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName19"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName20, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName20"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName21, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName21"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName22, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName22"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName23, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName23"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName24, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName24"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName25, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName25"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName26, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName26"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName27, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName27"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName28, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName28"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName29, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName29"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName30, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName30"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_InputLableName31, (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName31"));
#pragma endregion
	
	/************output***************/
#pragma region LabelName 설정
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName0, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName0"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName1, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName1"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName2, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName2"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName3, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName3"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName4, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName4"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName5, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName5"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName6, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName6"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName7, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName7"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName8, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName8"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName9, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName9"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName10, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName10"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName11, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName11"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName12, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName12"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName13, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName13"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName14, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName14"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName15, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName15"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName16, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName16"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName17, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName17"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName18, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName18"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName19, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName19"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName20, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName20"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName21, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName21"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName22, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName22"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName23, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName23"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName24, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName24"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName25, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName25"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName26, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName26"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName27, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName27"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName28, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName28"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName29, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName29"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName30, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName30"));
	pMyCfg->Set((TCHAR*)(LPCTSTR)m_LableName31, (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName31"));
#pragma endregion
	pMyCfg->Flush();
	CSCInspIO::Instance()->LoadPortNo();
	OnOK();
}

#define m_nOStageGetDef(stage, base, n) \
m_nOStage[stage].Seq.Pos[n].CapDone	= pMyCfg->Get(base + (long)n*2, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-"#stage"-.Seq.Pos-"#n"-.CapDone"));\
m_nOStage[stage].Seq.Pos[n].NG		= pMyCfg->Get(base+ 1 + (long)n*2, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-"#stage"-.Seq.Pos-"#n"-.NG"));

#define m_nIStageGetDef(stage, base, n) \
m_nIStage[stage].MotionDone[n]		= pMyCfg->Get(base+(long)n, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-"#stage"-.MotionDone-"#n"-"));

void CIOSetDlg::init_IOset(void)
{ 
	CString MachineInputName, MachineOutputName, MachineInputLableName, MachineOutputLabelName, ProductName;

    CCfgFile* pMyCfg=CCfgFile::Instance();
    pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
    pMyCfg->Open(); 
	pMyCfg->SetDepth(2);
   
	ProductName = PRODUCT_NAME;
    MachineInputName=ProductName+_T("_INPUTSET");
    MachineOutputName=ProductName+_T("_OUTPUTSET");
    MachineInputLableName=MachineInputName+_T("_NAME");
    MachineOutputLabelName=MachineOutputName+_T("_NAME");
 	
   // Output 관련 내용
	m_nOSpare0 = pMyCfg->Get(0L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOSpare0"));
	m_nOStage[0].VisionReady		= pMyCfg->Get(1L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-0-.VisionReady"));
	m_nOStage[0].VisionNotReady		= pMyCfg->Get(2L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-0-.VisionNotReady"));

	m_nOStageGetDef(0,3,0);
	m_nOStageGetDef(0,3,1);
	m_nOStageGetDef(0,3,2);
	m_nOStageGetDef(0,3,3);
	m_nOStageGetDef(0,3,4);
	m_nOStageGetDef(0,3,5);

	m_nOStage[0].Seq.UnitInspDone		= pMyCfg->Get(15L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-0-.Seq.UnitInspDone"));

	m_nOStage[1].VisionReady = pMyCfg->Get(16L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-1-.VisionReady"));
	m_nOStage[1].VisionNotReady = pMyCfg->Get(17L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-1-.VisionNotReady"));

	m_nOStageGetDef(1, 18, 0);
	m_nOStageGetDef(1, 18, 1);
	m_nOStageGetDef(1, 18, 2);
	m_nOStageGetDef(1, 18, 3);
	m_nOStageGetDef(1, 18, 4);
	m_nOStageGetDef(1, 18, 5);

	m_nOStage[1].Seq.UnitInspDone = pMyCfg->Get(30L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOStage-1-.Seq.UnitInspDone"));
	m_nOSpare31 = pMyCfg->Get(31L, (TCHAR*)(LPCTSTR)MachineOutputName, _T("m_nOSpare31"));

#pragma region Get LabelName 정의 
	m_LableName0 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName0"));
	m_LableName1 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName1"));
	m_LableName2 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName2"));
	m_LableName3 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName3"));
	m_LableName4 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName4"));
	m_LableName5 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName5"));
	m_LableName6 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName6"));
	m_LableName7 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName7"));
	m_LableName8 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName8"));
	m_LableName9 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName9"));
	m_LableName10 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName10"));
	m_LableName11 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName11"));
	m_LableName12 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName12"));
	m_LableName13 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName13"));
	m_LableName14 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName14"));
	m_LableName15 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName15"));
	m_LableName16 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName16"));
	m_LableName17 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName17"));
	m_LableName18 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName18"));
	m_LableName19 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName19"));
	m_LableName20 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName20"));
	m_LableName21 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName21"));
	m_LableName22 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName22"));
	m_LableName23 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName23"));
	m_LableName24 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName24"));
	m_LableName25 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName25"));
	m_LableName26 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName26"));
	m_LableName27 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName27"));
	m_LableName28 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName28"));
	m_LableName29 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName29"));
	m_LableName30 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName30"));
	m_LableName31 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineOutputLabelName, _T("m_LableName31"));
#pragma endregion

	//Input 관련내용
	m_nIStage[0].MANUAL = pMyCfg->Get(1L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.MANUAL"));
	//m_nIStage[0].ERR = pMyCfg->Get(2L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.ERR"));
	m_nIStage[0].RUN = pMyCfg->Get(3L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.RUN"));
	m_nIStage[0].IdleReset = pMyCfg->Get(4L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.IdleReset"));

	m_nIStage[0].UnitStart = pMyCfg->Get(5L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-0-.UnitStart"));

	m_nIStageGetDef(0, 6, 0);
	m_nIStageGetDef(0, 6, 1);
	m_nIStageGetDef(0, 6, 2);
	m_nIStageGetDef(0, 6, 3);
	m_nIStageGetDef(0, 6, 4);
	m_nIStageGetDef(0, 6, 5);

	m_nIStage[1].MANUAL = pMyCfg->Get(12L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.MANUAL"));
	//m_nIStage[1].ERR = pMyCfg->Get(13L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.ERR"));
	m_nIStage[1].RUN = pMyCfg->Get(14L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.RUN"));
	m_nIStage[1].IdleReset = pMyCfg->Get(15L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.IdleReset"));

	m_nIStage[1].UnitStart = pMyCfg->Get(16L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIStage-1-.UnitStart"));

	m_nIStageGetDef(1, 17, 0);
	m_nIStageGetDef(1, 17, 1);
	m_nIStageGetDef(1, 17, 2);
	m_nIStageGetDef(1, 17, 3);
	m_nIStageGetDef(1, 17, 4);
	m_nIStageGetDef(1, 17, 5);

#pragma region Get Output Label   
	m_InputLableName0 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName0"));
	m_InputLableName1 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName1"));
	m_InputLableName2 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName2"));
	m_InputLableName3 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName3"));
	m_InputLableName4 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName4"));
	m_InputLableName5 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName5"));
	m_InputLableName6 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName6"));
	m_InputLableName7 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName7"));
	m_InputLableName8 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName8"));
	m_InputLableName9 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName9"));
	m_InputLableName10 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName10"));
	m_InputLableName11 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName11"));
	m_InputLableName12 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName12"));
	m_InputLableName13 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName13"));
	m_InputLableName14 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName14"));
	m_InputLableName15 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName15"));
	m_InputLableName16 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName16"));
	m_InputLableName17 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName17"));
	m_InputLableName18 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName18"));
	m_InputLableName19 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName19"));
	m_InputLableName20 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName20"));
	m_InputLableName21 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName21"));
	m_InputLableName22 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName22"));
	m_InputLableName23 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName23"));
	m_InputLableName24 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName24"));
	m_InputLableName25 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName25"));
	m_InputLableName26 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName26"));
	m_InputLableName27 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName27"));
	m_InputLableName28 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName28"));
	m_InputLableName29 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName29"));
	m_InputLableName30 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName30"));
	m_InputLableName31 = pMyCfg->Get(_T("SPEAR"), (TCHAR*)(LPCTSTR)MachineInputLableName, _T("m_InputLableName31"));
#pragma endregion

	UpdateData(false);
}
BOOL CIOSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    
	SetTimer(0,50,NULL);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
    SetButton();
	init_IOset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CIOSetDlg::SetButton(void)
{

    short	shBtnColor = 30;

	////////////////////////// INPUT //////////////////////////////////

	m_OutputLed0.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed0.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed1.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed2.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed3.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed4.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed5.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed5.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed6.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed6.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed7.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed7.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed8.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed8.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed9.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed9.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed10.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed10.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed11.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed11.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed12.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed12.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed13.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed13.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
    m_OutputLed14.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed14.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed15.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed15.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed16.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed16.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed17.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed17.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed18.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed18.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed19.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed19.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed20.SetIcon(IDI_LEDON, IDI_LEDOFF);
	m_OutputLed20.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed21.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed21.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed22.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed22.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed23.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed23.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed24.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed24.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
    m_OutputLed25.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed25.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed26.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed26.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed27.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed27.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed28.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed28.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed29.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed29.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_OutputLed30.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed30.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);

	m_OutputLed31.SetIcon(IDI_LEDON, IDI_LEDOFF);
    m_OutputLed31.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);


    //m_InputLed0

    m_ToolTip.Create(this); 
	m_InputLed0.SetImage(IDB_GREENBUTTON,15);
	m_InputLed1.SetImage(IDB_GREENBUTTON,15);
	m_InputLed2.SetImage(IDB_GREENBUTTON,15);
	m_InputLed3.SetImage(IDB_GREENBUTTON,15);
	m_InputLed4.SetImage(IDB_GREENBUTTON,15);
	m_InputLed5.SetImage(IDB_GREENBUTTON,15);
	m_InputLed6.SetImage(IDB_GREENBUTTON,15);
	m_InputLed7.SetImage(IDB_GREENBUTTON,15);
	m_InputLed8.SetImage(IDB_GREENBUTTON,15);
	m_InputLed9.SetImage(IDB_GREENBUTTON,15);
	m_InputLed10.SetImage(IDB_GREENBUTTON,15);
	m_InputLed11.SetImage(IDB_GREENBUTTON,15);
	m_InputLed12.SetImage(IDB_GREENBUTTON,15);
	m_InputLed13.SetImage(IDB_GREENBUTTON,15);
	m_InputLed14.SetImage(IDB_GREENBUTTON,15);
	m_InputLed15.SetImage(IDB_GREENBUTTON,15);
	m_InputLed16.SetImage(IDB_GREENBUTTON,15);
	m_InputLed17.SetImage(IDB_GREENBUTTON,15);
	m_InputLed18.SetImage(IDB_GREENBUTTON,15);
	m_InputLed19.SetImage(IDB_GREENBUTTON,15);
	m_InputLed20.SetImage(IDB_GREENBUTTON,15);
	m_InputLed21.SetImage(IDB_GREENBUTTON,15);
	m_InputLed22.SetImage(IDB_GREENBUTTON,15);
	m_InputLed23.SetImage(IDB_GREENBUTTON,15);
	m_InputLed24.SetImage(IDB_GREENBUTTON,15);
	m_InputLed25.SetImage(IDB_GREENBUTTON,15);
	m_InputLed26.SetImage(IDB_GREENBUTTON,15);
	m_InputLed27.SetImage(IDB_GREENBUTTON,15);
	m_InputLed28.SetImage(IDB_GREENBUTTON,15);
	m_InputLed29.SetImage(IDB_GREENBUTTON,15);
	m_InputLed30.SetImage(IDB_GREENBUTTON,15);
	m_InputLed31.SetImage(IDB_GREENBUTTON,15);
	m_InputLed32.SetImage(IDB_GREENBUTTON,15);
	m_InputLed33.SetImage(IDB_GREENBUTTON,15);
	m_InputLed34.SetImage(IDB_GREENBUTTON,15);
	m_InputLed35.SetImage(IDB_GREENBUTTON,15);
	m_InputLed36.SetImage(IDB_GREENBUTTON,15);
	m_InputLed37.SetImage(IDB_GREENBUTTON,15);
	m_InputLed38.SetImage(IDB_GREENBUTTON,15);
}


#pragma region Output check box 처리
void CIOSetDlg::OnBnClickedOutputiocheck0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CIOPort * pIOPort = CSCInspIO::Instance()->m_pBoard;
	int nPortBit = m_nOSpare0;
	pIOPort->SetBit(nPortBit, !pIOPort->GetOutBit(nPortBit));
}

void CIOSetDlg::OnBnClickedOutputiocheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CIOPort * pIOPort = CSCInspIO::Instance()->m_pBoard;
	int nPortBit = m_nOStage[0].VisionReady;
	pIOPort->SetBit(nPortBit, !pIOPort->GetOutBit(nPortBit));
}

void CIOSetDlg::OnBnClickedOutputiocheck2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다. 
	CIOPort * pIOPort = CSCInspIO::Instance()->m_pBoard;
	int nPortBit = m_nOStage[0].VisionNotReady;
	  pIOPort->SetBit(nPortBit, !pIOPort->GetOutBit(nPortBit));
}

#define ChkBoxCapDoneEventHandler(stage, n) \
	 CIOPort * pIOPort=CSCInspIO::Instance()->m_pBoard;\
	int nPortBit = m_nOStage[stage].Seq.Pos[##n].CapDone;\
	 pIOPort->SetBit(nPortBit, !pIOPort->GetOutBit(nPortBit));

#define ChkBoxNGEventHandler(stage, n) \
	 CIOPort * pIOPort=CSCInspIO::Instance()->m_pBoard;\
	 int nPortBit = m_nOStage[stage].Seq.Pos[##n].NG;\
	 pIOPort->SetBit(nPortBit, !pIOPort->GetOutBit(nPortBit));

void CIOSetDlg::OnBnClickedOutputiocheck3()
{
	ChkBoxCapDoneEventHandler(0, 0);
}
void CIOSetDlg::OnBnClickedOutputiocheck4()
{
	ChkBoxNGEventHandler(0, 0);
}

void CIOSetDlg::OnBnClickedOutputiocheck5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxCapDoneEventHandler(0, 1);
}

void CIOSetDlg::OnBnClickedOutputiocheck6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxNGEventHandler(0, 1);
}

void CIOSetDlg::OnBnClickedOutputiocheck7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxCapDoneEventHandler(0, 2);
}

void CIOSetDlg::OnBnClickedOutputiocheck8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxNGEventHandler(0, 2);
}

void CIOSetDlg::OnBnClickedOutputiocheck9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxCapDoneEventHandler(0, 3);	
}

void CIOSetDlg::OnBnClickedOutputiocheck10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxNGEventHandler(0, 3);
}

void CIOSetDlg::OnBnClickedOutputiocheck11()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxCapDoneEventHandler(0, 4);
}

void CIOSetDlg::OnBnClickedOutputiocheck12()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxNGEventHandler(0, 4);

}

void CIOSetDlg::OnBnClickedOutputiocheck13()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxCapDoneEventHandler(0, 5);	
}


void CIOSetDlg::OnBnClickedOutputiocheck14()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxNGEventHandler(0, 5);	
}

void CIOSetDlg::OnBnClickedOutputiocheck15()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CIOPort * pIOPort = CSCInspIO::Instance()->m_pBoard;
	pIOPort->SetBit(m_nOStage[0].Seq.UnitInspDone, !pIOPort->GetOutBit(m_nOStage[0].Seq.UnitInspDone));
		
}

void CIOSetDlg::OnBnClickedOutputiocheck16()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CIOPort * pIOPort = CSCInspIO::Instance()->m_pBoard;
	int nPortBit = m_nOStage[1].VisionReady;
	pIOPort->SetBit(nPortBit, !pIOPort->GetOutBit(nPortBit));
}


void CIOSetDlg::OnBnClickedOutputiocheck17()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CIOPort * pIOPort = CSCInspIO::Instance()->m_pBoard;
	int nPortBit = m_nOStage[1].VisionNotReady;
	pIOPort->SetBit(nPortBit, !pIOPort->GetOutBit(nPortBit));
}

void CIOSetDlg::OnBnClickedOutputiocheck18()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxCapDoneEventHandler(1, 0);
}

void CIOSetDlg::OnBnClickedOutputiocheck19()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxNGEventHandler(1, 0);
}

void CIOSetDlg::OnBnClickedOutputiocheck20()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxCapDoneEventHandler(1, 1);
}

void CIOSetDlg::OnBnClickedOutputiocheck21()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxNGEventHandler(1, 1);
}

void CIOSetDlg::OnBnClickedOutputiocheck22()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxCapDoneEventHandler(1, 2);
}

void CIOSetDlg::OnBnClickedOutputiocheck23()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxNGEventHandler(1, 2);
}

void CIOSetDlg::OnBnClickedOutputiocheck24()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxCapDoneEventHandler(1, 3);
}

void CIOSetDlg::OnBnClickedOutputiocheck25()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxNGEventHandler(1, 3);
}

void CIOSetDlg::OnBnClickedOutputiocheck26()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxCapDoneEventHandler(1, 4);
}

void CIOSetDlg::OnBnClickedOutputiocheck27()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxNGEventHandler(1, 4);
}

void CIOSetDlg::OnBnClickedOutputiocheck28()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxCapDoneEventHandler(1, 5);
}

void CIOSetDlg::OnBnClickedOutputiocheck29()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ChkBoxNGEventHandler(1, 5);
}

void CIOSetDlg::OnBnClickedOutputiocheck30()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CIOPort * pIOPort = CSCInspIO::Instance()->m_pBoard;
	int nPortBit = m_nOStage[1].Seq.UnitInspDone;
	pIOPort->SetBit(nPortBit, !pIOPort->GetOutBit(nPortBit));
}

void CIOSetDlg::OnBnClickedOutputiocheck31()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CIOPort * pIOPort = CSCInspIO::Instance()->m_pBoard;
	int nPortBit = m_nOSpare31;
	pIOPort->SetBit(nPortBit, !pIOPort->GetOutBit(nPortBit));
}

#pragma endregion%


void CIOSetDlg::DoInputScan(void)
{
	CIOPort * pIOPort = CSCInspIO::Instance()->m_pBoard;

	m_bChkInput0 = pIOPort->GetBit(0);	//Spare port 0
	if (m_bChkInput0 != 0)
	{
		m_InputLed0.Depress(true);
	}
	else { m_InputLed0.Depress(false); }

	m_bChkInput1 = pIOPort->GetBit(m_nIStage[0].MANUAL);	//Manual 체크 port 1
	if (m_bChkInput1 != 0)
	{
		m_InputLed1.Depress(true);
	}
	else { m_InputLed1.Depress(false); }

	m_bChkInput1=pIOPort->GetBit(2);	//Spare port 2
	if(m_bChkInput1!=0)
	{m_InputLed1.Depress(true);}
	else{m_InputLed1.Depress(false);}

	m_bChkInput3=pIOPort->GetBit(m_nIStage[0].RUN);	// Run상태 체크  port 3
	if(m_bChkInput3!=0)
	{m_InputLed3.Depress(true);}
	else{m_InputLed3.Depress(false);}

	m_bChkInput4=pIOPort->GetBit(m_nIStage[0].IdleReset);		// idle/Reset 상태 port 4
	if(m_bChkInput4!=0)
	{m_InputLed4.Depress(true);}
	else{m_InputLed4.Depress(false);}

	m_bChkInput5=pIOPort->GetBit(m_nIStage[0].UnitStart);		// Unit Start port 5
	if(m_bChkInput5!=0)
	{m_InputLed5.Depress(true);}
	else{m_InputLed5.Depress(false);}

	m_bChkInput6=pIOPort->GetBit(m_nIStage[0].MotionDone[0]);	//Sequence done  port 6
	if(m_bChkInput6!=0)
	{m_InputLed6.Depress(true);}
	else{m_InputLed6.Depress(false);}

	m_bChkInput7=pIOPort->GetBit(m_nIStage[0].MotionDone[1]);	//Sequence done  port 7
	if(m_bChkInput7!=0)
	{m_InputLed7.Depress(true);}
	else{m_InputLed7.Depress(false);}

	m_bChkInput8=pIOPort->GetBit(m_nIStage[0].MotionDone[2]);	//Sequence done  port 8
	if(m_bChkInput8!=0)
	{m_InputLed8.Depress(true);}
	else{m_InputLed8.Depress(false);}

    m_bChkInput9=pIOPort->GetBit(m_nIStage[0].MotionDone[3]);	//Sequence done  port 9
	if(m_bChkInput9!=0)
	{m_InputLed9.Depress(true);}
	else{m_InputLed9.Depress(false);}

	m_bChkInput10=pIOPort->GetBit(m_nIStage[0].MotionDone[4]);	//Sequence done  port 10
	if(m_bChkInput10!=0)
	{m_InputLed10.Depress(true);}
	else{m_InputLed10.Depress(false);}

    m_bChkInput11=pIOPort->GetBit(m_nIStage[0].MotionDone[5]);	//Sequence done  port 11
	if(m_bChkInput11!=0)
	{m_InputLed11.Depress(true);}
	else{m_InputLed11.Depress(false);}

	m_bChkInput12=pIOPort->GetBit(m_nIStage[1].MANUAL);	//Stage 1 MANUAL port 12
	if(m_bChkInput12!=0)
	{m_InputLed12.Depress(true);}
	else{m_InputLed12.Depress(false);}

	m_bChkInput13=pIOPort->GetBit(13);	//Spare port 13
	if(m_bChkInput13!=0)
	{m_InputLed13.Depress(true);}
	else{m_InputLed13.Depress(false);}

	m_bChkInput14=pIOPort->GetBit(m_nIStage[1].RUN);	//Stage 1 Run Status port 14
	if(m_bChkInput14!=0)
	{m_InputLed14.Depress(true);}
	else{m_InputLed14.Depress(false);}

	m_bChkInput15=pIOPort->GetBit(m_nIStage[1].IdleReset);	//Stage1 idle/Reset Status port 15
	if(m_bChkInput15!=0)
	{m_InputLed15.Depress(true);}
	else{m_InputLed15.Depress(false);}

	m_bChkInput16=pIOPort->GetBit(m_nIStage[1].UnitStart);	//Stage1 UnitStart port 16
	if(m_bChkInput16!=0)
	{m_InputLed16.Depress(true);}
	else{m_InputLed16.Depress(false);}

	m_bChkInput17=pIOPort->GetBit(m_nIStage[1].MotionDone[0]);	//Stage1 Sequence Done Port 17
	if(m_bChkInput17!=0)
	{m_InputLed17.Depress(true);}
	else{m_InputLed17.Depress(false);}


	m_bChkInput18=pIOPort->GetBit(m_nIStage[1].MotionDone[1]);	//Stage1 Sequence Done Port 18
	if(m_bChkInput18!=0)
	{m_InputLed18.Depress(true);}
	else{m_InputLed18.Depress(false);}


	m_bChkInput19=pIOPort->GetBit(m_nIStage[1].MotionDone[2]);	//Stage1 Sequence Done Port 19
	if(m_bChkInput19!=0)
	{m_InputLed19.Depress(true);}
	else{m_InputLed19.Depress(false);}


   m_bChkInput20=pIOPort->GetBit(m_nIStage[1].MotionDone[3]);	//Stage1 Sequence Done Port 20
	if(m_bChkInput20!=0)
	{m_InputLed20.Depress(true);}
	else{m_InputLed20.Depress(false);}


	  m_bChkInput21=pIOPort->GetBit(m_nIStage[1].MotionDone[4]);	//Stage1 Sequence Done Port 21
	if(m_bChkInput21!=0)
	{m_InputLed21.Depress(true);}
	else{m_InputLed21.Depress(false);}


	  m_bChkInput22=pIOPort->GetBit(m_nIStage[1].MotionDone[5]);	//Stage1 Sequence Done Port 22
	if(m_bChkInput22!=0)
	{m_InputLed22.Depress(true);}
	else{m_InputLed22.Depress(false);}

	m_bChkInput23 = pIOPort->GetBit(23);	//Spare Port 23
	if (m_bChkInput23 != 0)
	{
		m_InputLed23.Depress(true);
	}
	else { m_InputLed23.Depress(false); }

	m_bChkInput24 = pIOPort->GetBit(24);	//Spare Port 24
	if (m_bChkInput24 != 0)
	{
		m_InputLed24.Depress(true);
	}
	else { m_InputLed24.Depress(false); }

	m_bChkInput25 = pIOPort->GetBit(25);	//Spare Port 25
	if (m_bChkInput25 != 0)
	{
		m_InputLed25.Depress(true);
	}
	else { m_InputLed25.Depress(false); }

}
void CIOSetDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    
	CDialog::OnTimer(nIDEvent);
	//if(!CMainDlg::Instance()->InspectionOnMode)
	//{
		
		DoInputScan();
	//}
}





