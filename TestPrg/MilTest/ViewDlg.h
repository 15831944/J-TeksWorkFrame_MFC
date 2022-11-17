#pragma once


// CViewDlg dialog

class CViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CViewDlg)

public:
	CViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CViewDlg();

// Dialog Data
	enum { IDD = IDD_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
