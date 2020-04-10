
// MapServerDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CMapServerDlg dialog
class CMapServerDlg : public CDialogEx
{
// Construction
public:
	CMapServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MAPSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void	MfcUpdateView();
	afx_msg void OnBnClickedOk();
	CListCtrl m_xCtrlList;
	afx_msg void OnBnClickedButton1();
};
