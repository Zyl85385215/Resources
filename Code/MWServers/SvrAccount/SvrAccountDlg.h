
// SvrAccountDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CSvrAccountDlg dialog
class CSvrAccountDlg : public CDialogEx
{
// Construction
public:
	CSvrAccountDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SVRACCOUNT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_xCtrlList;
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void	MfcUpdateView();
	afx_msg void OnBnClickedButton1();
};
