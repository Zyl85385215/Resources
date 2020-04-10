// LuaDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SvrGame.h"
#include "LuaDialog.h"
#include "afxdialogex.h"
#include "FsGameCenter.h"
#include <string>
using namespace std;

// LuaDialog dialog
LuaDialog*	g_pLuaDialog	= NULL;

IMPLEMENT_DYNAMIC(LuaDialog, CDialogEx)

LuaDialog::LuaDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(LuaDialog::IDD, pParent)
	, LuaOutPut(_T(""))
{
}

LuaDialog::~LuaDialog()
{
}

void LuaDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, LuaString);
	DDX_Control(pDX, IDC_COMBO1, LuaTemplate);
	DDX_Text(pDX, IDC_EDIT2, LuaOutPut);
}


BEGIN_MESSAGE_MAP(LuaDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &LuaDialog::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &LuaDialog::OnCbnSelchangeCombo1)
	ON_MESSAGE(WM_LUAOUTPUT,&LuaDialog::OnUpdateOutput)
END_MESSAGE_MAP()


// LuaDialog message handlers


void LuaDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	static DWORD nTime = 0;
	DWORD dwNowTime = CMTime::GetCurTickCount();
	if(dwNowTime - nTime < 10)
	{
		char sztxt[32];
		sprintf(sztxt,"请等待%d秒再试",(10+ nTime-dwNowTime ));
		AddOutput(sztxt);
		return;
	}
	nTime = dwNowTime;
	CString szstr;
	LuaString.GetWindowText(szstr);
	luaStringTag* pTag = new luaStringTag;
	pTag->szLua = szstr.GetBuffer();
	szstr.ReleaseBuffer();
	g_pGameCenter->PushLuaExecute(pTag);
	//CDialogEx::OnOK();
}

BOOL LuaDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LuaTemplate.AddString("发送系统公告");
	LuaTemplate.AddString("发送系统邮件");
	LuaTemplate.AddString("禁言某人");
	LuaTemplate.AddString("踢出某人");
	LuaTemplate.AddString("封号某人");
	return TRUE;
}


void LuaDialog::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	switch(LuaTemplate.GetCurSel())
	{
	case 0:
		{
			LuaString.SetWindowText("CommonModule.SendGameMessage(\"这里输入公告内容\",0,0,1)");
		}
		break;
	case 1:
		{
			LuaString.SetWindowText("CommonModule.AddSysMail(\"系统\",\"这里输入正文\",86400--[[时间:秒]],0--[[钱]],0--[[物品ID]],1--[[物品堆叠]],0--[[物品绑定]])");
		}
		break;
	case 2:
		{
			LuaString.SetWindowText("CommonModule.ForbidPlayer(0--[[玩家UIN]],0--[[时间:秒]])");
		}
		break;
	case 3:
		{
			LuaString.SetWindowText("CommonModule.KickPlayer(0--[[玩家UIN]],0--[[时间:秒,这项暂时无效]])");
		}
		break;
	case 4:
		{
			LuaString.SetWindowText("CommonModule.UnablePlayer(0--[[玩家账号UIN,注意是账号UIN]],true--[[封是true,解封是false]])");
		}
		break;
	}
}

void LuaDialog::AddOutput(const char* szTxt )
{
	LuaOutPut += szTxt;
	LuaOutPut += "\r\n";
	PostMessage(WM_LUAOUTPUT);
}

LRESULT LuaDialog::OnUpdateOutput( WPARAM wParam, LPARAM lParam )
{
	UpdateData(FALSE);
	return 0;
}
