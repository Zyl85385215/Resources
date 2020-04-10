#pragma once
#include "afxwin.h"


// NoticeDialog dialog

class NoticeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(NoticeDialog)

public:
	NoticeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~NoticeDialog();

// Dialog Data
	enum { IDD = IDD_NOTICE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedOk();
	CEdit NoticeString;
};

extern	NoticeDialog* g_pNoticeDialog;