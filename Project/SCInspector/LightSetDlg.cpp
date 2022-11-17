// LightSetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "LightSetDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "OpPanelDlg.h"

// CLightSetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLightSetDlg, CDialogEx)

CLightSetDlg::CLightSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LIGHTSET, pParent)
{
	for (int i = 0; i < 4; i++)
		m_ChBrightness[i] = 1023;
}

CLightSetDlg::~CLightSetDlg()
{
}

void CLightSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPL0, m_spL0);
	DDX_Control(pDX, IDC_SPL1, m_spL1);
	DDX_Control(pDX, IDC_SPL2, m_spL2);
	DDX_Control(pDX, IDC_SPL3, m_spL3);
	DDX_Text(pDX, IDC_L0V, m_ChBrightness[0]);
	DDX_Text(pDX, IDC_L1V, m_ChBrightness[1]);
	DDX_Text(pDX, IDC_L2V, m_ChBrightness[2]);
	DDX_Text(pDX, IDC_L3V, m_ChBrightness[3]);
	DDX_Control(pDX, IDOK, m_cOK);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
}


BEGIN_MESSAGE_MAP(CLightSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLightSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHKL0, &CLightSetDlg::OnBnClickedChkl0)
	ON_BN_CLICKED(IDC_CHKL1, &CLightSetDlg::OnBnClickedChkl1)
	ON_BN_CLICKED(IDC_CHKL2, &CLightSetDlg::OnBnClickedChkl2)
	ON_BN_CLICKED(IDC_CHKL3, &CLightSetDlg::OnBnClickedChkl3)
	ON_BN_CLICKED(IDCANCEL, &CLightSetDlg::OnBnClickedCancel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPL0, &CLightSetDlg::OnDeltaposSpl0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPL1, &CLightSetDlg::OnDeltaposSpl1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPL2, &CLightSetDlg::OnDeltaposSpl2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPL3, &CLightSetDlg::OnDeltaposSpl3)
END_MESSAGE_MAP()


// CLightSetDlg 메시지 처리기입니다.


BOOL CLightSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CStatus::Instance()->LoadCFG();
	m_spL0.SetRange(0, 1023);
	m_spL1.SetRange(0, 1023);
	m_spL2.SetRange(0, 1023);
	m_spL3.SetRange(0, 1023);

	m_ChBrightness[0] = CStatus::Instance()->m_LightBrightness[0];
	m_ChBrightness[1] = CStatus::Instance()->m_LightBrightness[1];
	m_ChBrightness[2] = CStatus::Instance()->m_LightBrightness[2];
	m_ChBrightness[3] = CStatus::Instance()->m_LightBrightness[3];

	CheckDlgButton(IDC_CHKL0, TRUE);
	CheckDlgButton(IDC_CHKL1, TRUE);
	CheckDlgButton(IDC_CHKL2, TRUE);
	CheckDlgButton(IDC_CHKL3, TRUE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CLightSetDlg::OnBnClickedChkl0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LightSet(0, IsDlgButtonChecked(IDC_CHKL0));
} 


void CLightSetDlg::OnBnClickedChkl1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LightSet(1, IsDlgButtonChecked(IDC_CHKL1));
}


void CLightSetDlg::OnBnClickedChkl2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LightSet(2, IsDlgButtonChecked(IDC_CHKL2));

}


void CLightSetDlg::OnBnClickedChkl3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LightSet(3, IsDlgButtonChecked(IDC_CHKL3));
}


void CLightSetDlg::LightSet(int ch, bool bStatus)
{
	UpdateData(TRUE);
	if (bStatus)
	{
		CStatus::Instance()->GetComm()->SendLightBrightness(ch, m_ChBrightness[ch]);
		CStatus::Instance()->GetComm()->LightOnOff(ch, CSCInspPacket::LIGHT_ON);
	}
	else
	{
		CStatus::Instance()->GetComm()->LightOnOff(ch, CSCInspPacket::LIGHT_OFF);
	}
}

void CLightSetDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CStatus::Instance()->m_LightBrightness[0] = m_ChBrightness[0];
	CStatus::Instance()->m_LightBrightness[1] = m_ChBrightness[1];
	CStatus::Instance()->m_LightBrightness[2] = m_ChBrightness[2];
	CStatus::Instance()->m_LightBrightness[3] = m_ChBrightness[3];
	CStatus::Instance()->SaveCFG();
	CStatus::Instance()->GetComm()->LightOnOff(CSCInspPacket::LIGHT_ON, m_ChBrightness);

	CDialogEx::OnOK();
}


void CLightSetDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ChBrightness[0] = CStatus::Instance()->m_LightBrightness[0];
	m_ChBrightness[1] = CStatus::Instance()->m_LightBrightness[1];
	m_ChBrightness[2] = CStatus::Instance()->m_LightBrightness[2];
	m_ChBrightness[3] = CStatus::Instance()->m_LightBrightness[3];
	UpdateData(FALSE);
	for(int i=0;i<4;i++)
		LightSet(i, IsDlgButtonChecked(IDC_CHKL0));
	CDialogEx::OnCancel();
}

void CLightSetDlg::OnDeltaposSpl0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LightSet(0, true);
	*pResult = 0;
}


void CLightSetDlg::OnDeltaposSpl1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LightSet(1, true);
	*pResult = 0;
}


void CLightSetDlg::OnDeltaposSpl2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LightSet(2, true);
	*pResult = 0;
}


void CLightSetDlg::OnDeltaposSpl3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LightSet(3, true);
	*pResult = 0;
}
