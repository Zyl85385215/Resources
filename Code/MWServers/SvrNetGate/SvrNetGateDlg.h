
// SvrNetGateDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CSvrNetGateDlg dialog
class CSvrNetGateDlg : public CDialogEx
{
// Construction
public:
	CSvrNetGateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SVRNETGATE_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void	MfcUpdateView();
	CListCtrl m_xCtrlList;
	afx_msg void OnBnClickedBtnListref();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	CListCtrl m_xCListCtrlEx;
	afx_msg void OnBnClickedBtnListref2();
	CListCtrl xCListArea;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedBtnListref3();
};
