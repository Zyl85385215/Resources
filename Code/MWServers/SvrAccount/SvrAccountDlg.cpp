
// SvrAccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SvrAccount.h"
#include "SvrAccountDlg.h"
#include "afxdialogex.h"
#include "AccountCenter.h"
#include "ZNetServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSvrAccountDlg dialog




CSvrAccountDlg::CSvrAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSvrAccountDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSvrAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_xCtrlList);
}

BEGIN_MESSAGE_MAP(CSvrAccountDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSvrAccountDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CSvrAccountDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSvrAccountDlg message handlers

BOOL CSvrAccountDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	SetWindowText(exeFullPath);

	g_pMainAccount	= new AccountCenter();
	g_pMainAccount->InitDB();
	g_pMainAccount->InitNet();

	m_xCtrlList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);	//LVS_EX_HEADERDRAGDROP |LVS_EX_TRACKSELECT
	m_xCtrlList.InsertColumn(0,"ID",LVCFMT_CENTER,40);
	m_xCtrlList.InsertColumn(1,"类型",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(2,"IP",LVCFMT_CENTER,150);
	m_xCtrlList.InsertColumn(3,"PORT",LVCFMT_CENTER,60);
	

	CString	str;
	for (int i = 0; i < MAX_NETGATE; i++)
	{
		str.Format("%d",i);
		m_xCtrlList.InsertItem(i,str);
		m_xCtrlList.SetItemText(i,1,"网关");
	}
	for (int i = 0; i < MAX_GAMESVR; i++)
	{
		str.Format("%d",i+10);
		m_xCtrlList.InsertItem(i+MAX_NETGATE,str);
		m_xCtrlList.SetItemText(i+MAX_NETGATE,1,"游戏");
	}
	SetTimer(1,40,NULL);
	SetTimer(9,1000,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSvrAccountDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSvrAccountDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSvrAccountDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}


void CSvrAccountDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		{
			g_pMainAccount->Update();
		}
		break;
	case 9:
		{
			MfcUpdateView();
		}
		break;
	}

	


	CDialogEx::OnTimer(nIDEvent);
}

void CSvrAccountDlg::MfcUpdateView()
{
	g_pMainAccount->Update25();
	CString	strFormat;
	SetDlgItemInt(IDC_EDIT1,g_pMainAccount->m_nCreateCnt);
	SetDlgItemInt(IDC_EDIT2,g_pMainAccount->m_nLoginCnt);

	for (int i = 0; i < MAX_NETGATE; i++)
	{
		if(g_pNetCenter->m_szpNetGates[i] && g_pNetCenter->m_szpNetGates[i]->IsActive())
		{
			strFormat	= inet_ntoa(g_pNetCenter->m_szpNetGates[i]->GetAddress()->sin_addr);
			if(m_xCtrlList.GetItemText(i,2) != strFormat)
				m_xCtrlList.SetItemText(i,2,strFormat);
			strFormat.Format("%d",g_pNetCenter->m_szpNetGates[i]->GetAddress()->sin_port);
			if(m_xCtrlList.GetItemText(i,3) != strFormat)
				m_xCtrlList.SetItemText(i,3,strFormat);
		}
		else
		{
			if(m_xCtrlList.GetItemText(i,2) != "未连接")
				m_xCtrlList.SetItemText(i,2,"未连接");
			if(m_xCtrlList.GetItemText(i,3) != "0")
				m_xCtrlList.SetItemText(i,3,"0");
		}
	}
	for (int i = 0; i < MAX_GAMESVR; i++)
	{
		if(g_pNetCenter->m_szpGameSvr[i] && g_pNetCenter->m_szpGameSvr[i]->IsActive())
		{
			strFormat	= inet_ntoa(g_pNetCenter->m_szpGameSvr[i]->GetAddress()->sin_addr);
			if(m_xCtrlList.GetItemText(i,2) != strFormat)
				m_xCtrlList.SetItemText(i+MAX_NETGATE,2,strFormat);
			strFormat.Format("%d",g_pNetCenter->m_szpGameSvr[i]->GetAddress()->sin_port);
			if(m_xCtrlList.GetItemText(i,3) != strFormat)
				m_xCtrlList.SetItemText(i+MAX_NETGATE,3,strFormat);
		}
		else
		{
			if(m_xCtrlList.GetItemText(i+MAX_NETGATE,2) != "未连接")
				m_xCtrlList.SetItemText(i+MAX_NETGATE,2,"未连接");
			if(m_xCtrlList.GetItemText(i+MAX_NETGATE,3) != "0")
				m_xCtrlList.SetItemText(i+MAX_NETGATE,3,"0");
		}
	}
}


void CSvrAccountDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	g_pMainAccount->m_bIsLockNewAcc	= !g_pMainAccount->m_bIsLockNewAcc;
	if(g_pMainAccount->m_bIsLockNewAcc)
	{
		SetDlgItemText(IDC_BUTTON1,"已限制");
	}
	else
		SetDlgItemText(IDC_BUTTON1,"不限制");

}
