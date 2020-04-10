// NoticeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SvrGame.h"
#include "NoticeDialog.h"
#include "afxdialogex.h"
#include "FsGameCenter.h"

// NoticeDialog dialog

NoticeDialog*	g_pNoticeDialog	= NULL;

IMPLEMENT_DYNAMIC(NoticeDialog, CDialogEx)

NoticeDialog::NoticeDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(NoticeDialog::IDD, pParent)
{

}

NoticeDialog::~NoticeDialog()
{
}

void NoticeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, NoticeString);
}


BEGIN_MESSAGE_MAP(NoticeDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &NoticeDialog::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &NoticeDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// NoticeDialog message handlers


void NoticeDialog::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void NoticeDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	CString szstr;
	NoticeString.GetWindowText(szstr);
	NoticeStringTag* pTag = new NoticeStringTag;
	pTag->szNotice = szstr.GetBuffer();
	szstr.ReleaseBuffer();
	g_pGameCenter->PushNoticeExecute(pTag);

	//CDialogEx::OnOK();
}
