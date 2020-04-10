
// SvrNetGateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SvrNetGate.h"
#include "SvrNetGateDlg.h"
#include "afxdialogex.h"
#include "NetGateCenter.h"
#include "ZNetServer.h"

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


// CSvrNetGateDlg dialog




CSvrNetGateDlg::CSvrNetGateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSvrNetGateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSvrNetGateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_xCtrlList);
	DDX_Control(pDX, IDC_LIST2, m_xCListCtrlEx);
	DDX_Control(pDX, IDC_LIST3, xCListArea);
}

BEGIN_MESSAGE_MAP(CSvrNetGateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSvrNetGateDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_LISTREF, &CSvrNetGateDlg::OnBnClickedBtnListref)
	ON_BN_CLICKED(IDC_BUTTON1, &CSvrNetGateDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSvrNetGateDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSvrNetGateDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSvrNetGateDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BTN_LISTREF2, &CSvrNetGateDlg::OnBnClickedBtnListref2)
	ON_BN_CLICKED(IDC_BUTTON5, &CSvrNetGateDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CSvrNetGateDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CSvrNetGateDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BTN_LISTREF3, &CSvrNetGateDlg::OnBnClickedBtnListref3)
END_MESSAGE_MAP()


// CSvrNetGateDlg message handlers

BOOL CSvrNetGateDlg::OnInitDialog()
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

	g_pMainGate	= new NetGateCenter();
	g_pMainGate->InitNet();

	m_xCtrlList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);	//LVS_EX_HEADERDRAGDROP |LVS_EX_TRACKSELECT
	m_xCtrlList.InsertColumn(0,"Key",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(1,"Acc",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(2,"Uin",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(3,"IP",LVCFMT_CENTER,130);
	m_xCtrlList.InsertColumn(4,"RecvByte",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(5,"SendByte",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(6,"time",LVCFMT_CENTER,60);

	m_xCListCtrlEx.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_xCListCtrlEx.InsertColumn(0,"SvID",LVCFMT_CENTER,75);
	m_xCListCtrlEx.InsertColumn(1,"State",LVCFMT_CENTER,45);

	xCListArea.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	xCListArea.InsertColumn(0,"DefSvID",LVCFMT_CENTER,80);

	SetTimer(1,1000,NULL);
	SetTimer(9,3000,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSvrNetGateDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSvrNetGateDlg::OnPaint()
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
HCURSOR CSvrNetGateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSvrNetGateDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}


void CSvrNetGateDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		g_pMainGate->Update25();
	}
	else if(nIDEvent == 9)
		MfcUpdateView();

	CDialogEx::OnTimer(nIDEvent);
}

void CSvrNetGateDlg::MfcUpdateView()
{
	if(g_pNetCenter == NULL)
		return;
	if(g_pNetCenter->m_pSvrAccount->isActive())
		SetDlgItemText(IDC_EDIT3,"OK");
	else
		SetDlgItemText(IDC_EDIT3,"NO");
	if(g_pNetCenter->m_pSvrGame->isActive())
		SetDlgItemText(IDC_EDIT4,"OK");
	else
		SetDlgItemText(IDC_EDIT4,"NO");

	if(g_pNetCenter->m_ppSvrMap[0]->isActive())
		SetDlgItemText(IDC_EDIT8,"OK");
	else
		SetDlgItemText(IDC_EDIT8,"NO");

	SetDlgItemInt(IDC_EDIT1,g_pNetCenter->m_nLinkCnt);
	SetDlgItemInt(IDC_EDIT2,g_pNetCenter->m_nLinkTotal);

	SetDlgItemInt(IDC_EDIT5,g_pNetCenter->m_dwSendClient);
	SetDlgItemInt(IDC_EDIT6,g_pNetCenter->m_dwRecvClient);

	SetDlgItemInt(IDC_EDIT9,g_pNetCenter->m_nServerCnt);
}


void CSvrNetGateDlg::OnBnClickedBtnListref()
{
	// TODO: Add your control notification handler code here
	DWORD	dwTick	= GetTickCount();
	g_pNetCenter->RefreshUiView(&m_xCtrlList);
	SetDlgItemInt(IDC_EDIT7,GetTickCount()-dwTick);
}


void CSvrNetGateDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	g_pNetCenter->m_nServerCnt++;
	SetDlgItemInt(IDC_EDIT9,g_pNetCenter->m_nServerCnt);
}


void CSvrNetGateDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if(g_pNetCenter->m_nServerCnt > 0)
		g_pNetCenter->m_nServerCnt--;
	SetDlgItemInt(IDC_EDIT9,g_pNetCenter->m_nServerCnt);
}


void CSvrNetGateDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CString	strTmp;
	GetDlgItemText(IDC_EDIT10,strTmp);
	if(g_pNetCenter->m_szNotice)
		free(g_pNetCenter->m_szNotice);
	g_pNetCenter->m_szNotice	= strdup(strTmp.GetBuffer());
	strTmp.ReleaseBuffer();
}


void CSvrNetGateDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	int nSvrID	= GetDlgItemInt(IDC_EDIT12);
	g_pNetCenter->RefHotSvrList(nSvrID,&xCListArea,false);
}

void CSvrNetGateDlg::OnBnClickedBtnListref2()
{
	// TODO: Add your control notification handler code here
	int nSvID = GetDlgItemInt(IDC_EDIT16);
	int nState = GetDlgItemInt(IDC_EDIT17);

	g_pNetCenter->RefSeverState(nSvID,nState,&m_xCListCtrlEx);
}


void CSvrNetGateDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	g_pNetCenter->RefHotSvrList(0,&xCListArea,true);
}


void CSvrNetGateDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	g_pNetCenter->m_nDefCountry = GetDlgItemInt(IDC_EDIT18);
}


void CSvrNetGateDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	CString	strTmp;
	GetDlgItemText(IDC_EDIT14,strTmp);
	if(g_pNetCenter->m_szADNotice)
		free(g_pNetCenter->m_szADNotice);
	g_pNetCenter->m_szADNotice	= strdup(strTmp.GetBuffer());
	strTmp.ReleaseBuffer();
}


void CSvrNetGateDlg::OnBnClickedBtnListref3()
{
	// TODO: Add your control notification handler code here
	g_pNetCenter->RefSeverState(0,0,&m_xCListCtrlEx,true);
}
