
// SvrGameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SvrGame.h"
#include "SvrGameDlg.h"
#include "afxdialogex.h"
#include "FsGameCenter.h"
#include <io.h>
#include <fcntl.h>
#include "LuaDialog.h"
#include "NoticeDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSvrGameDlg dialog




CSvrGameDlg::CSvrGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSvrGameDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pLuaDialog = new LuaDialog;
	g_pLuaDialog->Create(IDD_LUA_DIALOG,NULL);

	g_pNoticeDialog = new NoticeDialog;
	g_pNoticeDialog->Create(IDD_NOTICE_DIALOG,NULL);
}

void CSvrGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_xCtrlList);
}

BEGIN_MESSAGE_MAP(CSvrGameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CSvrGameDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSvrGameDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_LISTREF, &CSvrGameDlg::OnBnClickedBtnListref)
	ON_BN_CLICKED(IDC_BUTTON1, &CSvrGameDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSvrGameDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSvrGameDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CSvrGameDlg message handlers
//#define INIT_CONSOLE
#ifdef INIT_CONSOLE
void InitConsole()
{
	int nRet= 0;
	FILE* fp;
	AllocConsole();
	nRet= _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	fp = _fdopen(nRet, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
}
#endif

BOOL CSvrGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	SetWindowText(exeFullPath);


#ifdef INIT_CONSOLE
	InitConsole();
#endif
	g_pGameCenter	= new FsGameCenter();

	m_xCtrlList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);	//LVS_EX_HEADERDRAGDROP |LVS_EX_TRACKSELECT
	m_xCtrlList.InsertColumn(0,"Uin",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(1,"Lv",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(2,"Name",LVCFMT_CENTER,130);
	m_xCtrlList.InsertColumn(3,"MapIndex",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(4,"Gold",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(5,"RMB",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(6,"BindRMB",LVCFMT_CENTER,60);
	//m_xCtrlList.InsertColumn(4,"time",LVCFMT_CENTER,60);

	SetTimer(0,100,NULL);
	SetTimer(1,40,NULL);
	SetTimer(9,3000,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSvrGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSvrGameDlg::OnPaint()
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
HCURSOR CSvrGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSvrGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0)
	{
		g_pGameCenter->FireHeartBeat();
	}
	else if(nIDEvent == 1)
	{
		g_pGameCenter->FireDealPacket();
	}
	else if(nIDEvent == 9)
	{
		MfcUpdateView();
		if(g_pGameCenter->m_byShutDown == 99 && GetTickCount()-g_pGameCenter->m_dwShutTick > 10000)
		{
			CDialogEx::OnCancel();
			return;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CSvrGameDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}

void CSvrGameDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox("确定关闭游戏服务器?",MB_OKCANCEL) == IDOK)
		g_pGameCenter->m_byShutDown	= 1;
}


void CSvrGameDlg::MfcUpdateView()
{
	SetDlgItemInt(IDC_EDIT1,g_pGameCenter->GetUserCnt());
}


void CSvrGameDlg::OnBnClickedBtnListref()
{
	// TODO: Add your control notification handler code here
	DWORD	dwTick	= GetTickCount();
	//g_pGameCenter->RefreshUiView(&m_xCtrlList);	//可能有多线程问题
	//CString	str;
	//str.Format("耗费时间: %d",GetTickCount()-dwTick);
	//::MessageBox(NULL,str,"MSG",MB_OK);
	SetDlgItemInt(IDC_EDIT2,GetTickCount()-dwTick);
}


void CSvrGameDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if (g_pLuaDialog->IsWindowVisible())
	{
		g_pLuaDialog->ShowWindow(FALSE);
	}else{
		g_pLuaDialog->ShowWindow(TRUE);

	}
}


void CSvrGameDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	if (g_pNoticeDialog->IsWindowVisible())
	{
		g_pNoticeDialog->ShowWindow(FALSE);
	}else{
		g_pNoticeDialog->ShowWindow(TRUE);

	}
}


void CSvrGameDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox("游戏服务器将在60秒后关闭，是否确定?",MB_OKCANCEL) == IDOK)
		g_pGameCenter->SendGameSvrCloseTick(60);
}
