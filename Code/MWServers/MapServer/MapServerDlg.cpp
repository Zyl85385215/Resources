
// MapServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MapServer.h"
#include "MapServerDlg.h"
#include "afxdialogex.h"

#include "ServerCenter.h"
#include "MapCenter.h"
#include "ControlCenter.h"
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


// CMapServerDlg dialog




CMapServerDlg::CMapServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMapServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMapServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_xCtrlList);
}

BEGIN_MESSAGE_MAP(CMapServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CMapServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapServerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMapServerDlg message handlers

BOOL CMapServerDlg::OnInitDialog()
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


	g_pMSvrCenter	= new MapServerCenter();

	m_xCtrlList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);	//LVS_EX_HEADERDRAGDROP |LVS_EX_TRACKSELECT
	m_xCtrlList.InsertColumn(0,"Index",LVCFMT_CENTER,50);
	m_xCtrlList.InsertColumn(1,"BaseIndex",LVCFMT_CENTER,60);
	m_xCtrlList.InsertColumn(2,"UserNum",LVCFMT_CENTER,50);
	m_xCtrlList.InsertColumn(3,"ActiveCnt",LVCFMT_CENTER,60);

	SetTimer(0,100,NULL);
	SetTimer(1,40,NULL);
	SetTimer(9,3000,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMapServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMapServerDlg::OnPaint()
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
HCURSOR CMapServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMapServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0)
	{
		g_pMSvrCenter->MTFireHeartBeat();
	}
	else if(nIDEvent == 1)
	{
		g_pMSvrCenter->MTFireDealPacket();
	}
	else if(nIDEvent == 9)
	{
		MfcUpdateView();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CMapServerDlg::MfcUpdateView()
{
	if(g_pNetCenter == NULL)
		return;
	if(g_pNetCenter->m_pSvrGame->isActive())
		SetDlgItemText(IDC_EDIT4,"OK");
	else
		SetDlgItemText(IDC_EDIT4,"NO");

	if(g_pMapList)
		SetDlgItemInt(IDC_EDIT5,g_pMapList->m_mapMaps.size());
	if(g_pCtrlList)
		SetDlgItemInt(IDC_EDIT6,g_pCtrlList->mapControlers.size());

}

void CMapServerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}


void CMapServerDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	m_xCtrlList.DeleteAllItems();
	char	szTmp[256];
	std::map<DWORD,MapManager*>::iterator itor	= g_pMapList->m_mapMaps.begin();
	while(itor != g_pMapList->m_mapMaps.end())
	{
		MapManager* pND	= (MapManager*)itor->second;

		sprintf(szTmp,"%d", pND->m_dwMapIndex);
		int nPos	= m_xCtrlList.InsertItem(m_xCtrlList.GetItemCount(),szTmp);
		sprintf(szTmp,"%d", pND->m_pMCfgObj->m_wMapIndex);
		m_xCtrlList.SetItemText(nPos,1,szTmp);
		sprintf(szTmp,"%d", pND->m_nUserCnt);
		m_xCtrlList.SetItemText(nPos,2,szTmp);
		sprintf(szTmp,"%d", pND->m_nActiveCnt);
		m_xCtrlList.SetItemText(nPos,3,szTmp);
		
		itor++;
	}
}
