#include "LoginScene.h"
#include "InterfaceLayer.h"
#include "ios_ac_packet.h"
#include "SoundCenter.h"
#include "MainMenu.h"
#include "ios_user_packet.h"
#include "xnExtstr.h"
#include "MapStaticCfg.h"
#include "FamilyCenter.h"
#include "MapControl.h"
#include "TextConfig.h"
#include "ReconnectGame.h"
#include "EffectSprite.h"
#include "MakeLayer.h"
#include <vector>
#include "MapTile.h"
#include "ClientLuaManager.h"
#ifdef USE_ANDROID
#include "JniUtil.h"
#endif

using namespace cocos2d;


int g_SelectedServer = 0;
char*	g_szLoginAcc	= NULL;
char*	g_szLoginPswd	= NULL;
char*	g_szBackKey		= NULL;
int		g_nServerID	= 0;
char*	g_szZoneName	= "";

void SelLineOKEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	LoginScene* pLoginS	= (LoginScene*)param;
	if (pLoginS)
	{
		pLoginS->m_pSelLineMenu->m_pForm->Close();
		pLoginS->m_pSelLineMenu->m_pFormEx->Open();
	}
}

void SelLineExOKEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	LoginScene* pLoginS	= (LoginScene*)param;

	if(pLoginS->m_pSelLineMenu->m_pSelLine)
	{
		g_packetCenter.ConnectServer (pLoginS->m_pSelLineMenu->m_pSelLine->szIP,pLoginS->m_pSelLineMenu->m_pSelLine->nPort);
		//ReconnectModel::GetInstance().SetServerInfo(pLoginS->m_pSelLineMenu->m_pSelLine->szIP,pLoginS->m_pSelLineMenu->m_pSelLine->nPort);
		pMainMenu->PopSystemMsg("正在连接服务器中，请稍候...",true);
		pLoginS->m_dwTickDealError	= xnGetTickCount();
	}
	else
		pMainMenu->PopSystemMsg("请先选择想要进入的服务器");
}

void SelLineOpenEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	LoginScene* pLoginS	= (LoginScene*)param;
	if (pLoginS)
	{
		if (g_nClientAreaLock == 1 && g_nServerID < 1100)
			pLoginS->m_pSelLineMenu->SetSelLine(1101);

		pLoginS->m_pSelLineMenu->m_pFormEx->Close();
		pLoginS->m_pSelLineMenu->m_pFormNotice->Close();
		pLoginS->m_pSelLineMenu->m_pForm->Open();
	}
}

void SelLineExCancelEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	LoginScene* pLoginS	= (LoginScene*)param;

	pLoginS->m_pSelLineMenu->m_pFormEx->Close();
	pLoginS->m_pSelLineMenu->m_pFormNotice->Close();
	if(0)//第三方
	{

	}
	else
	{
		pLoginS->m_pLoginMenu->Open();
	}
}

void SelLineCancelEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	LoginScene* pLoginS	= (LoginScene*)param;
	
	pLoginS->m_pSelLineMenu->m_pForm->Close();
	pLoginS->m_pSelLineMenu->m_pFormEx->Open();
}

void LoginExOkEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	LoginScene* pLoginS	= (LoginScene*)param;

	if(g_packetCenter.m_bActive == false)
	{
		if(pLoginS->m_pLineAcc)
			g_packetCenter.ConnectServer (pLoginS->m_pLineAcc->szIP,pLoginS->m_pLineAcc->nPort);
	}

	BuildPacketEx(pPacket, ACCOUNT, LOGINMAIN, buffer,1024);
	SET_DATA(loginData, ACCOUNT, LOGINMAIN, pPacket);

	strcpy(loginData->szLogin, pLoginS->m_pLoginMenu->m_pFormRegist->GetComponent("name")->GetEdit());
	strcpy(loginData->szPassword, pLoginS->m_pLoginMenu->m_pFormRegist->GetComponent("mima")->GetEdit());

	if(strcmp(loginData->szPassword, pLoginS->m_pLoginMenu->m_pFormRegist->GetComponent("ma")->GetEdit()) != 0)
	{
		pMainMenu->PopSystemMsg("密码输入不一致");
		return;
	}

	if(strlen(loginData->szLogin) < 6 || strlen(loginData->szLogin) > 16)
	{
		pMainMenu->PopSystemMsg("账号长度需要在6-16个字符内");
		return;
	}

	if(!((loginData->szLogin[0] >= 'A' && loginData->szLogin[0] <= 'Z') || (loginData->szLogin[0] >= 'a' && loginData->szLogin[0] <= 'z')))
	{
		pMainMenu->PopSystemMsg("账号首位必须是英文字母");
		return;
	}

	if(strlen(loginData->szPassword) < 6)
	{
		pMainMenu->PopSystemMsg("密码长度不得少于6个字符");
		return;
	}

	if(g_szLoginAcc)
		free(g_szLoginAcc);
	g_szLoginAcc	= strdup(loginData->szLogin);
	if(g_szLoginPswd)
		free(g_szLoginPswd);
	g_szLoginPswd	= strdup(loginData->szPassword);

	loginData->bSpeedReg	= 2;
	loginData->bJoin		= false;	//验证账号时不直接进
	memset(loginData->szMacAddress,0,32);
	strncpy(loginData->szMacAddress,g_pTPObj->TP_GetMacAddress(),30);
	loginData->nIOSVersion	= g_pTPObj->TP_GetSystemVersion();
	strncpy(loginData->szIDFA,g_pTPObj->TP_GetIDFA(),50);
	pLoginS->SendPacket(pPacket);
	pLoginS->m_dwTickDealError	= xnGetTickCount();	
	pMainMenu->PopSystemMsg("账号注册中，即将进入游戏...",true);	
	//pMainMenu->PopTipMsg("                注册成功");
}

void LoginMoveEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	LoginScene* pLoginS	= (LoginScene*)param;

	if(pLoginS->m_dwPlayMoveTick && xnGetTickCount()- pLoginS->m_dwPlayMoveTick < 7000)
	{
		pLoginS->m_dwPlayMoveTick = xnGetTickCount() - 7800;
	}
}

void OverMoveEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	LoginScene* pLoginS	= (LoginScene*)param;

	if(pLoginS->m_dwPlayMoveTick)
	{
		for (int i = 0; i < 4; i++)
		{
			if(pLoginS->m_pSpriteMove[i])
				pLoginS->m_pSpriteMove[i]->setVisible(false);
			if(pLoginS->m_pLableTxt[i])
				pLoginS->m_pLableTxt[i]->setVisible(false);
		}

		pLoginS->m_dwPlayMoveTick	= 0;
		pLoginS->m_pCreateRoleMenu->Open();
	}
}


void LoginOkEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	LoginScene* pLoginS	= (LoginScene*)param;

	if(g_packetCenter.m_bActive == false)
	{
		if(pLoginS->m_pLineAcc)
			g_packetCenter.ConnectServer (pLoginS->m_pLineAcc->szIP,pLoginS->m_pLineAcc->nPort);
	}

	BuildPacketEx(pPacket, ACCOUNT, LOGINMAIN, buffer,1024);
	SET_DATA(loginData, ACCOUNT, LOGINMAIN, pPacket);

	strcpy(loginData->szLogin, pLoginS->m_pLoginMenu->m_pForm->GetComponent("name")->GetEdit());
	strcpy(loginData->szPassword, pLoginS->m_pLoginMenu->m_pForm->GetComponent("mima")->GetEdit());

	if(strlen(loginData->szLogin) < 6)
	{
		pMainMenu->PopSystemMsg("账号至少6个字符");
		return;
	}

	if(strlen(loginData->szPassword) < 6)
	{
		pMainMenu->PopSystemMsg("密码至少6个字符");
		return;
	}

	if(g_szLoginAcc)
		free(g_szLoginAcc);
	g_szLoginAcc	= strdup(loginData->szLogin);
	if(g_szLoginPswd)
		free(g_szLoginPswd);
	g_szLoginPswd	= strdup(loginData->szPassword);

	loginData->bSpeedReg	= 0;
	loginData->bJoin		= false;	//账号时不直接进
	memset(loginData->szMacAddress,0,32);
	strncpy(loginData->szMacAddress,g_pTPObj->TP_GetMacAddress(),30);
	loginData->nIOSVersion	= g_pTPObj->TP_GetSystemVersion();
	strncpy(loginData->szIDFA,g_pTPObj->TP_GetIDFA(),50);
	pLoginS->SendPacket(pPacket);
	pLoginS->m_dwTickDealError	= xnGetTickCount();

	pMainMenu->PopSystemMsg("正在登录中，即将进入游戏...",true);	
}

void SpRegidtEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	LoginScene* pLoginS	= (LoginScene*)param;

	if(g_packetCenter.m_bActive == false)
	{
		if(pLoginS->m_pLineAcc)
			g_packetCenter.ConnectServer (pLoginS->m_pLineAcc->szIP,pLoginS->m_pLineAcc->nPort);
	}

	BuildPacketEx(pPacket, ACCOUNT, LOGINMAIN, buffer,1024);
	SET_DATA(loginData, ACCOUNT, LOGINMAIN, pPacket);

	//strcpy(loginData->szLogin, pLoginS->m_pLoginMenu->m_pForm->GetComponent("name")->GetEdit());
	//strcpy(loginData->szPassword, pLoginS->m_pLoginMenu->m_pForm->GetComponent("mima")->GetEdit());

	const char*	szDev	= g_pTPObj->TP_GetDevID();
	if(strlen(szDev) < 6)
	{
		pMainMenu->PopSystemMsg("广告追踪被关闭,请手动注册");
		return;
	}
	else if(strlen(szDev) < 18)
	{
		strcpy(loginData->szLogin,szDev);
		memset(loginData->szPassword,0,16);
		strncpy(loginData->szPassword,szDev,14);
	}
	else
	{
		strcpy(loginData->szLogin,szDev+strlen(szDev)-18);
		memset(loginData->szPassword,0,16);
		strncpy(loginData->szPassword,szDev,14);
	}
	
	if(strlen(loginData->szLogin) < 6)
	{
		pMainMenu->PopSystemMsg("账号至少6个字符");
		return;
	}

	if(strlen(loginData->szPassword) < 6)
	{
		pMainMenu->PopSystemMsg("密码至少6个字符");
		return;
	}

	if(g_szLoginAcc)
		free(g_szLoginAcc);
	g_szLoginAcc	= strdup(loginData->szLogin);
	if(g_szLoginPswd)
		free(g_szLoginPswd);
	g_szLoginPswd	= strdup(loginData->szPassword);

	loginData->bSpeedReg	= 1;
	loginData->bJoin		= false;	//账号时不直接进
	memset(loginData->szMacAddress,0,32);
	strncpy(loginData->szMacAddress,g_pTPObj->TP_GetMacAddress(),30);
	loginData->nIOSVersion	= g_pTPObj->TP_GetSystemVersion();
	strncpy(loginData->szIDFA,g_pTPObj->TP_GetIDFA(),50);
	pLoginS->SendPacket(pPacket);
	pLoginS->m_dwTickDealError	= xnGetTickCount();

	pMainMenu->PopSystemMsg("账号注册登录中...",true);	
}

void CreateRoleOKEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nJump = true;
	for (int i=0;i<CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCnt();i++)
	{
		int nColor = CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCompBase(i)->nR;
		if (nColor == 255)
		{
			nJump = false;
		}
		//CreateRoleMenu::pMenu->SetPosAction(i,"stand");
		//CreateRoleMenu::pMenu->SetPosDark(i,true);

	}

	if (nJump)
	{
		return;
	}
	LoginScene* pLoginS	= (LoginScene*)param;
	char	szNickName[64];
	char	szInvite[64];
	strcpy(szNickName,pLoginS->m_pCreateRoleMenu->m_pForm->GetComponent("name")->GetEdit());
	strcpy(szInvite,pLoginS->m_pCreateRoleMenu->m_pForm->GetComponent("yqm")->GetEdit());

	if(strlen(szNickName) < 4 ||strlen(szNickName) > 12)
	{
		pMainMenu->PopTipMsg("角色名字长度需要在4-12个字符之内，请重新输入");
		return;
	}
	if(strlen(szInvite) && strlen(szInvite) != 8)
	{
		pMainMenu->PopTipMsg("邀请码长度不对");
		return;
	}
	if (atoi(szNickName)&&szNickName[0]!='0')
	{
		pMainMenu->PopTipMsg("角色名第一位不能为数字，请重新输入");
		return;
	}
	if(RandNameConfig::GetInstance().CheckForbid(szNickName))
	{
		pMainMenu->PopTipMsg("名字中含有屏蔽词，请重新输入");
		return;
	}

	BuildPacketEx(pPacket,ACCOUNT,C_CREATE_CHARACTER,buf,1024);
	SET_DATA(pData,ACCOUNT,C_CREATE_CHARACTER,pPacket);
	strcpy(pData->szName,szNickName);
	pData->bJob		= pLoginS->m_pCreateRoleMenu->nJob;
	pData->bSex		= pLoginS->m_pCreateRoleMenu->nSex;
	pData->bCountry	= pLoginS->m_pCreateRoleMenu->m_nCountry;
	strcpy(pData->szInvite,szInvite);
	pLoginS->SendPacket(pPacket);

	pMainMenu->PopSystemMsg("即将进入游戏，请耐心等候..",true);
	pLoginS->m_dwTickDealError	= xnGetTickCount();
	
	g_pTPObj->TP_OtherEvent(5,"");
}

void CreateRoleCancelEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	LoginScene* pLoginS	= (LoginScene*)param;
	pLoginS->m_pCreateRoleMenu->m_pForm->Close();
	pLoginS->m_pChooseRoleMenu->Open(NULL);
}

void	Click2CreateRoleButton(InterfaceEvent * pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		LoginScene* pLogin = (LoginScene*)param;
		if(pLogin->m_pChooseRoleMenu->m_pRoleList->Count >= 4)
		{
			pMainMenu->PopSystemMsg("您的角色数量已经达到上限，无法创建");
			return;
		}
		pLogin->m_pChooseRoleMenu->Close();
		pLogin->m_pCreateRoleMenu->Open();
	}
}

void	DeleteRoleMsgEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		LoginScene* pLogin = (LoginScene*)param;
		USimpleInfo* pInfo = (USimpleInfo*)pLogin->m_pChooseRoleMenu->m_pRoleList->Items[pLogin->m_pChooseRoleMenu->m_nChoosen];
		BuildPacketEx(pNewPacket, ACCOUNT, C_DELETE_CHARACTER, buffer,256);
		SET_DATA(pSend,ACCOUNT, C_DELETE_CHARACTER,pNewPacket);
		pSend->dwUin	= pInfo->dwUin;
		pLogin->SendPacket(pNewPacket);

		pMainMenu->PopSystemMsg("正在删除角色...",true);
		pLogin->m_dwTickDealError	= xnGetTickCount();
	}
}

void	Click2DeleteRoleButton(InterfaceEvent * pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		LoginScene* pLogin = (LoginScene*)param;
		if(pLogin->m_pChooseRoleMenu->m_nChoosen < 0 || pLogin->m_pChooseRoleMenu->m_nChoosen >= pLogin->m_pChooseRoleMenu->m_pRoleList->Count)
		{
			pMainMenu->PopSystemMsg("请先选择角色");
			return;
		}
		USimpleInfo* pInfo = (USimpleInfo*)pLogin->m_pChooseRoleMenu->m_pRoleList->Items[pLogin->m_pChooseRoleMenu->m_nChoosen];
		if(pInfo->byLv >= 20)
		{
			pMainMenu->PopSystemMsg("角色等级大于20级，无法删除");
			return;
		}
		char szMsg[128];
		sprintf(szMsg,"是否确认删除角色[%s],删除角色后将无法再恢复",pInfo->szNick);
		pMainMenu->PopAskMsgBox(szMsg,DeleteRoleMsgEvent,param);
	}
}

void	Click2EnterGameButton(InterfaceEvent * pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		LoginScene* pLogin = (LoginScene*)param;
		if(pLogin->m_pChooseRoleMenu->m_nChoosen < 0 || pLogin->m_pChooseRoleMenu->m_nChoosen >= pLogin->m_pChooseRoleMenu->m_pRoleList->Count)
		{
			pMainMenu->PopSystemMsg("请先选择角色");
			return;
		}
		USimpleInfo* pInfo = (USimpleInfo*)pLogin->m_pChooseRoleMenu->m_pRoleList->Items[pLogin->m_pChooseRoleMenu->m_nChoosen];
		BuildPacketEx(pNewPacket, ACCOUNT, C_REQUEST_JOIN, buffer,256);
		SET_DATA(pSend,ACCOUNT, C_REQUEST_JOIN,pNewPacket);
		pSend->dwUin	= pInfo->dwUin;
		pLogin->SendPacket(pNewPacket);
		pChar->m_byLvl	= pInfo->byLv;
		pMainMenu->PopSystemMsg("即将进入游戏，请耐心等候...",true);
		pLogin->m_dwTickDealError	= xnGetTickCount();
		SaveIntegerToXML("role_choosen",pLogin->m_pChooseRoleMenu->m_nChoosen);
	}
}
#ifdef TEST_LOGIN
xnList*		g_pNetList	= xnList::Create();
#endif
LoginScene::LoginScene():m_dwErrorCode(LOGIN_SUCCESSFUL)
{
	m_dwTickDealError	= 0;
	m_pLineAcc		= NULL;
	m_dwPlayMoveTick	= 0;
	m_byMoveStep		= 0;
	m_dwTickInit		= 0;
	memset(m_pSpriteMove,0,16);
	memset(m_pLableTxt,0,16);

	strcpy(m_szMoveTxt[0],"        距离那场上古之战已经过去了万年之久，被永恒之井的大爆炸所分裂的艾泽拉斯世界逐渐归于平静。");
	strcpy(m_szMoveTxt[1],"        人类因为理念的不同而分为了两股势力——联盟与部落，长久以来两国之间的冲突从未停止。");
	strcpy(m_szMoveTxt[2],"        然而谁也没有注意到，在遥远的外域，一支强大的种族在邪恶力量的操纵下正对他们的世界虎视眈眈。");
	strcpy(m_szMoveTxt[3],"        终于有一天，黑暗之门的开启，打破了长久以来的宁静。与此同时，一个传说也开始踏上了征程……");

#ifdef TEST_LOGIN
	for (int i = 0; i < 100; i++)
	{
		PacketCenter*	pPkC	= new PacketCenter();
		g_pNetList->Add(pPkC);
	}
#endif
}

LoginScene::~LoginScene()
{
	//delete g_pIosTp;
}

void LoginScene::onEnter()
{
	CCScene::onEnter ();

	scheduleUpdate ();

	CCLog("1111111111ONENter");
	srand(xnGetTickCount());
	ReconnectModel::GetInstance().SetModelWorked(false);
	RandNameConfig::GetInstance().LoadFile("cfg/RandNameCfg.txt");
	RandNameConfig::GetInstance().LoadFBFile("cfg/Forbid.txt");
	LoadingTxtConfig::GetInstance().LoadFile("cfg/loadingcfg.txt");
#ifdef USE_ANDROID
	ResControl::GetInstance().Load("cfg/ResControl.txt");
#endif
	ServerLineCfg::GetInstance().LoadFile();
	//HelloWorld* pLayer	= HelloWorld::create ();

	if(pMainInterface)
		pMainInterface->CloseAllForm ();
	if(g_pLayerIF == NULL)
	{
		g_pLayerIF	= InterfaceLayer::create ();
		g_pLayerIF->retain();
	}
	addChild(g_pLayerIF,10,2);

	if(g_pSoundCenter == NULL)
		g_pSoundCenter	= new CSoundCenter();

	if(pChar == NULL)
		pChar	= new UserData();
	//if(pLuaStateManager == NULL)
	{
// 		pLuaStateManager = new CLuaStateManagerPlus();
// 		SetLuaStateManager(pLuaStateManager);
	}
	if (pLeague == NULL)
	{
		pLeague = new FamilyCenter;
	}
	if(pMainMenu	== NULL)
		pMainMenu		= new MainMenu();
	if(g_pMapCenter	== NULL)
		g_pMapCenter	= new MapControl();

	m_pLoginMenu	= new LoginMenu();
	m_pSelLineMenu	= new SelLineMenu();

	m_pCreateRoleMenu	= new CreateRoleMenu();
	m_pChooseRoleMenu	= new ChooseRoleMenu();


	//m_pSelLineMenu->m_pForm->Open();
	//m_pLoginMenu->m_pForm->Open();
	//if(g_pTPObj->TP_GetPlat() != 14)		//同步推开启声音会导致ios5不能自动登录
	CCLog("1111111111STEP2");
	g_pSoundCenter->PlayerBackMp3("music/entry.mp3",true);
	CCLog("1111111111STEP3");
	InitBackSprite();

	if(g_pTPObj == NULL)
		g_pTPObj	= new IosThirdPlat();
	
	if(g_pTPObj->TP_GetPlat()/10%2)
		m_pSelLineMenu->m_pFormEx->GetComponent("fh")->SetVisible(false);
	m_pSelLineMenu->m_pFormEx->GetComponent("jr")->SetEvent(SelLineExOKEvent, this);
	m_pSelLineMenu->m_pFormEx->GetComponent("fh")->SetEvent(SelLineExCancelEvent, this);
	m_pSelLineMenu->m_pFormEx->GetComponent("xuan")->SetEvent(SelLineOpenEvent, this);
	m_pSelLineMenu->m_pForm->GetComponent("ok")->SetEvent(SelLineOKEvent, this);
	m_pSelLineMenu->m_pForm->GetComponent("qx")->SetEvent(SelLineCancelEvent, this);
	m_pLoginMenu->m_pForm->GetComponent("denglu")->SetEvent(LoginOkEvent,this);
	m_pLoginMenu->m_pForm->GetComponent("kuaisu")->SetEvent(SpRegidtEvent,this);
	m_pLoginMenu->m_pFormRegist->GetComponent("zhuce")->SetEvent(LoginExOkEvent,this);
	m_pCreateRoleMenu->m_pForm->GetComponent("ok")->SetEvent(CreateRoleOKEvent,this);
	m_pCreateRoleMenu->m_pForm->GetComponent("qx")->SetEvent(CreateRoleCancelEvent,this);
	m_pChooseRoleMenu->m_pForm->GetComponent("cj")->SetEvent(Click2CreateRoleButton,this);
	m_pChooseRoleMenu->m_pForm->GetComponent("ok")->SetEvent(Click2EnterGameButton,this);
	m_pChooseRoleMenu->m_pForm->GetComponent("sc")->SetEvent(Click2DeleteRoleButton,this);
	pMainInterface->GetDForm("moving")->GetComponent("xx")->SetEvent(LoginMoveEvent,this);
	pMainInterface->GetDForm("moving")->GetComponent("tiaoguo")->SetEvent(OverMoveEvent,this);
	pMainInterface->GetDForm("moving")->GetComponent("tiaoguo")->byRunAction = 16;

	m_dwTickDealError	= 0;

// 
// 	MakeLayer* pMake = new MakeLayer;
// 	pMake->retain();
// 	addChild(pMake,99,3);

#ifdef TEST_LOGIN
	return;
#endif

	if(g_pTPObj->TP_GetPlat()/10%2 && g_pTPObj->nLoginStep >= 2)
	{//跳过账号登陆部分
		m_pSelLineMenu->SetSelLine(g_nServerID);
		m_pSelLineMenu->m_pFormEx->Open();
	}
	else
	{
		//一开始都要连接服务器获取是否更新以及线路数量
		int	nRandAccLine	= rand()%3;
		m_pLineAcc	= ServerLineCfg::GetInstance().GetLine(0,nRandAccLine);
		if(m_pLineAcc == NULL && nRandAccLine != 0)
			m_pLineAcc	= ServerLineCfg::GetInstance().GetLine(0,0);
		if(!m_pLineAcc)
		{
			pMainMenu->PopSystemMsg("客户端版本过低，请更新至最新版本",true);
			return;
		}

		g_packetCenter.ConnectServer (m_pLineAcc->szIP,m_pLineAcc->nPort);
		pMainMenu->PopSystemMsg("正在连接网络，请确保网络设置正常",true);
		m_dwTickInit	= xnGetTickCount();
		//m_dwPlayMoveTick	= xnGetTickCount();
	}
	CCLog("1111111111STEP4");
}

void LoginScene::onExit()
{
	delete m_pSelLineMenu;
	delete m_pLoginMenu;
	delete m_pCreateRoleMenu;
	delete m_pChooseRoleMenu;
	CCScene::onExit ();
}

bool LoginScene::PacketProcess( ZPacket* recvPacket )
{
	if(recvPacket->bCmdGroup == ACCOUNT)
	{
		switch (recvPacket->bCmd)
		{
		case	INIT_CLIENT:
			{
				m_dwTickInit	= 0;
				CCLog("SET_SOCKETKEY");
				SET_DATA(pKey, ACCOUNT, INIT_CLIENT, recvPacket);
				if(pKey->wVersion > CLIENT_VERSION)
				{
					pMainMenu->PopSystemMsg("请下载最新客户端",true);
					g_packetCenter.DisConnectServer();
					return true;
				}
				if(recvPacket->wSize >= sizeof(ACCOUNT_INIT_CLIENT_Data) && pKey->bRfSvrCnt)
				{
					for (int i = 0; i < pKey->bRfSvrCnt; i++)
					{
						ServerLineCfg::GetInstance().RfSvrLine(pKey->rfSvrs[i].wIndex,pKey->rfSvrs[i].szIP,pKey->rfSvrs[i].wPort);
					}
				}

				//DWORD dwClientVer	= cocos2d::CCUserDefault::sharedUserDefault ()->getIntegerForKey ("clientversion");
				//if(dwClientVer == 0)
				//{
				//	cocos2d::CCUserDefault::sharedUserDefault ()->setIntegerForKey ("clientversion",pKey->dwGameVersion);
				//	cocos2d::CCUserDefault::sharedUserDefault ()->flush ();
				//}
				//else if(pKey->dwGameVersion != dwClientVer)
				//{
				//	pMainMenu->PopSystemMsg("客户端已有更新,请手动下载");
				//}
				if (m_dwErrorCode == LOGIN_SUCCESSFUL)
				{
					pMainMenu->CloseMessageBox();
				}
				if(pKey->bServerCnt	== 0)
				{
					pMainMenu->PopSystemMsg("账号异常,正在重新设置网络");
					//g_packetCenter.DisConnectServer();
					//第三方登录不需要 重新连接账号登录服务器
					if(0)
					{

					}
					else
					{
						g_packetCenter.ConnectServer(m_pLineAcc->szIP,m_pLineAcc->nPort);
						m_pSelLineMenu->Close();
					}
					return true;
				}
				m_dwTickDealError	= 0;
				if(m_pSelLineMenu->m_pFormEx->bOpened)
				{//正式进入游戏服务器组
					BuildPacketEx(pPacket, ACCOUNT, LOGIN, buffer,1024);
					SET_DATA(loginData, ACCOUNT, LOGIN, pPacket);

					strcpy(loginData->szLogin, g_szLoginAcc);
					strcpy(loginData->szPassword, g_szLoginPswd);

					loginData->bSpeedReg	= 1;
					loginData->bJoin		= true;	//账号时不直接进
					memset(loginData->szMacAddress,0,32);
					strncpy(loginData->szMacAddress,g_pTPObj->TP_GetMacAddress(),30);
					loginData->bTPID	= g_pTPObj->TP_GetPlat();
					memset(loginData->szAPNToken,0,80);
					if(g_szAPNToken)
						strcpy(loginData->szAPNToken,g_szAPNToken);
					SendPacket(pPacket);
					g_nServerID		= m_pSelLineMenu->m_pSelLine->nSvrID;
					g_szZoneName	= m_pSelLineMenu->m_pSelLine->szName;
					m_pLoginMenu->SaveConfig();
					pMainMenu->PopSystemMsg("即将进入游戏，请耐心等候..",true);
					m_dwTickDealError	= xnGetTickCount();
					return true;	//正式进入游戏时
				}
				else
					m_pSelLineMenu->SetMaxLine(pKey->bServerCnt);
				
				if(g_pTPObj->TP_Login())	//是否第三方登录
				{//打开第三方登录界面
					
				}
				else
				{
					//连接成功
					m_pLoginMenu->Open();
					
				}
				int nActive = LoadIntegerFromXML("active",0);
				if (!nActive)
				{
					BuildPacketEx(pPacket, ACCOUNT, ACTIVE_CNT, buffer,1024);
					SET_DATA(loginData, ACCOUNT, ACTIVE_CNT, pPacket);
					loginData->nPVal = g_pTPObj->TP_GetPlat();
					SendPacket(pPacket);
					SaveIntegerToXML("active",1);
				}

			}
			return true;
		case INIT_NOTICE:
			{
				SET_DATA(pRecv,ACCOUNT,INIT_NOTICE,recvPacket);
				CCLog("11111INIT_NOTICE(%d:%d)",g_nServerID,pRecv->wGameSvr);
				if(g_nServerID	== 0)	//设定默认服务器
					g_nServerID	= pRecv->wGameSvr;

				if(strlen(pRecv->szMsg) == 0)
				{
					m_pSelLineMenu->m_pFormNotice->Close();
					m_pSelLineMenu->strNotice.assign("");
				}
				else
				{
					m_pSelLineMenu->strNotice.assign(pRecv->szMsg);
					std::vector<std::string> vecTemp;
					StringSplit(m_pSelLineMenu->strNotice,"|",vecTemp);
					DComponent* pComp = m_pSelLineMenu->m_pFormNotice->GetComponent("txt");
					pComp->pSTextCenter->ClearSText();
					for(int i = 0; i < vecTemp.size(); i ++)
					{
						pComp->pSTextCenter->AddSText(i,0,true,(char*)vecTemp[i].c_str(),255,255,255);
					}
					//StringReplace(m_pSelLineMenu->strNotice,'|','\n');
					//m_pSelLineMenu->m_pFormNotice->Open();
					//m_pSelLineMenu->m_pFormNotice->GetComponent("txt")->SetCaptionEx("%s",0,m_pSelLineMenu->strNotice.c_str());
				}
			}
			return true;
		case SVRLINE_STATE:
			{
				SET_DATA(pRecv,ACCOUNT,SVRLINE_STATE,recvPacket);
				
				while(m_pSelLineMenu->m_pStateList->Count)
					m_pSelLineMenu->m_pStateList->Delete(0);

				for (int i = 0; i < pRecv->wCnt; i++)
				{
					m_pSelLineMenu->m_pStateList->Add((void*)pRecv->sznState[i]);
				}

			}
			return true;
		case SVRLINE_AREA_NAME:
			{
				SET_DATA(pRecv,ACCOUNT,SVRLINE_AREA_NAME,recvPacket);

				//while(m_pSelLineMenu->m_pAreaNameList->Count)
				//	m_pSelLineMenu->m_pAreaNameList->Delete(0);

				if (m_pCreateRoleMenu->m_nDefCountry == 0)
					m_pCreateRoleMenu->m_nDefCountry = pRecv->byCountry;

				//std::string strTemp1(pRecv->szAreaName);
				//std::vector<std::string> vec1;
				//StringSplit(strTemp1,"|",vec1);
				//for(int i = 0; i < vec1.size(); i ++)
				//{
				//	char* szAreaName = strdup(vec1[i].c_str());
				//	m_pSelLineMenu->m_pAreaNameList->Add((void*)szAreaName);
				//}
			}
			return true;
		case	SIMPLEINFOS:
			{
				SET_DATA(pData, ACCOUNT, SIMPLEINFOS, recvPacket);
				pMainMenu->CloseMessageBox();
				m_dwTickDealError	= 0;
				if(pData->bCnt%100 == 0)
				{//没有角色就新建
					m_pLoginMenu->Close();
					m_pSelLineMenu->Close();
					m_pChooseRoleMenu->Close();
					m_dwPlayMoveTick	= xnGetTickCount();
					//m_pCreateRoleMenu->Open();
					
				}
				else
				{//选角色界面暂缺
					if(pData->bCnt == 1)
					{
						BuildPacketEx(pNewPacket, ACCOUNT, C_REQUEST_JOIN, buffer,256);
						SET_DATA(pSend,ACCOUNT, C_REQUEST_JOIN,pNewPacket);
						pSend->dwUin	= pData->szUSInfos[0].dwUin;
						SendPacket(pNewPacket);
						pChar->m_byLvl	= pData->szUSInfos[0].byLv;

						pMainMenu->PopSystemMsg("即将进入游戏，请耐心等候...",true);
						m_dwTickDealError	= xnGetTickCount();
					}
					else
					{
						m_pLoginMenu->Close();
						m_pSelLineMenu->Close();
						m_pCreateRoleMenu->m_pForm->Close();
						m_pChooseRoleMenu->Open(pData);
					}

				}
			}
			return true;
		case S_DELETE_CHARACTER:
			{
				SET_DATA(pRecv, ACCOUNT, S_DELETE_CHARACTER, recvPacket);
				m_pChooseRoleMenu->Delete(pRecv->dwUin);
			}
			return true;
		case COUNTRY_CHOOSE:
			{
				SET_DATA(pRecv, ACCOUNT, COUNTRY_CHOOSE, recvPacket);
				if (pRecv->byCountry)
				{
					m_pCreateRoleMenu->m_nCountry = pRecv->byCountry;
				}
			}
			return true;
		case	TP_LOGINRTL:
			{
				SET_DATA(pErr, ACCOUNT, TP_LOGINRTL, recvPacket);
				m_dwTickDealError	= 0;
				pMainMenu->CloseMessageBox();
				if(pErr->byErrorCode == LOGIN_SUCCESSFUL)
				{//这边直接发送角色信息,少一次来回
					g_szLoginAcc	= strdup(pErr->szAcc);
					g_szLoginPswd	= strdup(pErr->szPswd);
					g_szBackKey		= strdup(pErr->szBackKey);
					m_pSelLineMenu->SetSelLine(g_nServerID);
					m_pSelLineMenu->m_pFormEx->Open();
					m_pSelLineMenu->ShowNotice();
				}
				else
				{
					switch(pErr->byErrorCode)
					{
					case LOGIN_ERROR_ACOUNT_ERROR:
						pMainMenu->PopSystemMsg ("账号不存在，请重新登录或注册新账号");
						break;
					case LOGIN_ERROR_INVITE_ERROR:
						pMainMenu->PopSystemMsg ("邀请码不存在");
						break;
					case LOGIN_ERROR_PSWD_ERROR:
						pMainMenu->PopSystemMsg ("账号或密码错误，请重新输入");
						break;
					case LOGIN_ERROR_ACOUNT_ISBEREG:
						pMainMenu->PopSystemMsg ("该账号已被使用，请选择其它账号注册");
						break;
					case LOGIN_ERROR_SERVER_USER_FULL:
						pMainMenu->PopSystemMsg ("服务器爆满, 请选择其他服务器或稍后重试");
						break;
					case LOGIN_ERROR_OUT_SERVER:
						pMainMenu->PopSystemMsg ("服务器临时维护,请联系GM");
						break;
					case LOGIN_ERROR_UNENABLE:
						pMainMenu->PopSystemMsg ("该账号已经被禁用,请联系GM");
						break;

					}
					m_dwErrorCode = pErr->byErrorCode;
					m_pSelLineMenu->Close();
					g_pTPObj->TP_Login();
				}
			}
			return true;
		case	LOGIN_RLT:
			{
				SET_DATA(pErr, ACCOUNT, LOGIN_RLT, recvPacket);

				if(g_bySvrLockClient == 0)
					g_bySvrLockClient	= pErr->bSvrLock;

				if (g_nClientAreaLock == -1)
					g_nClientAreaLock   = pErr->bSvrIsAD;
#ifdef USE_ANDROID
				g_bySvrLockClient	= 1;
#endif
//#ifdef WIN32
//				g_bySvrLockClient	= 1;	//加载资源是否多线程
//#endif
				//pMainMenu->CloseMessageBox ();
				m_dwTickDealError	= 0;
				pMainMenu->CloseMessageBox();
				if(pErr->byErrorCode == LOGIN_SUCCESSFUL)
				{//这边直接发送角色信息,少一次来回
					if(g_szLoginAcc)
						m_pLoginMenu->m_pForm->GetComponent("name")->SetEdit(g_szLoginAcc);
					if(g_szLoginPswd)
						m_pLoginMenu->m_pForm->GetComponent("mima")->SetEdit(g_szLoginPswd);
					if(pErr->byJoin == false)
					{
						m_pLoginMenu->Close();
						m_pSelLineMenu->SetSelLine(g_nServerID);
						m_pSelLineMenu->m_pFormEx->Open();
						m_pSelLineMenu->ShowNotice();
					}
					else
					{
						if(m_pSelLineMenu->m_pSelLine)
						{
							ReconnectModel::GetInstance().SetServerInfo(m_pSelLineMenu->m_pSelLine->szIP,m_pSelLineMenu->m_pSelLine->nPort);
						}
					}
				}
				else
				{
					switch(pErr->byErrorCode)
					{
					case LOGIN_ERROR_ACOUNT_ERROR:
						pMainMenu->PopSystemMsg ("账号不存在，请重新登录或注册新账号");
						break;
					case LOGIN_ERROR_INVITE_ERROR:
						pMainMenu->PopSystemMsg ("邀请码不存在");
						break;
					case LOGIN_ERROR_PSWD_ERROR:
						pMainMenu->PopSystemMsg ("账号或密码错误，请重新输入");
						break;
					case LOGIN_ERROR_ACOUNT_ISBEREG:
						pMainMenu->PopSystemMsg ("该账号已被使用，请选择其它账号注册");
						break;
					case LOGIN_ERROR_SERVER_USER_FULL:
						pMainMenu->PopSystemMsg ("服务器爆满, 请选择其他服务器或稍后重试");
						break;
					case LOGIN_ERROR_UNENABLE:
						pMainMenu->PopSystemMsg ("该账号已经被禁用,请联系GM");
						break;
					}
					m_dwErrorCode = pErr->byErrorCode;
					if(0)	//第三方
					{

					}
					else
					{
						g_packetCenter.ConnectServer(m_pLineAcc->szIP,m_pLineAcc->nPort);
						m_pSelLineMenu->Close();
					}
				}
			}
			return true;
		}
	}
	return false;
}


void LoginScene::update( float dt )
{
#ifdef TEST_LOGIN
	static	DWORD	s_dwCntLogin	= 0;
	static	DWORD	s_dwTickRun	= xnGetTickCount();
	static	DWORD	s_dwRunTime	= 0;
	char	szTmpName[256];
	bool bLimit	= false;
	if(xnGetTickCount() - s_dwTickRun < 300)
	{
		bLimit	= true;
	}
	else
		s_dwTickRun	= xnGetTickCount();
	s_dwRunTime++;
	for (int i = 0; i < g_pNetList->Count; i++)
	{
		PacketCenter* pPkC	= (PacketCenter*)g_pNetList->Items[i];
		if(pPkC->m_SocketManager.nConnectState <= 0)
		{
			if(bLimit == false && i/10==s_dwRunTime%10)
				pPkC->ConnectServer("218.244.148.29",8800);
		}
		else
		{
			pPkC->m_SocketManager.Filp();
			ZPacket* pPacket	= NULL;
			while(pPacket	= pPkC->m_SocketManager.GetPacket ())
			{
				if(pPacket->bCmdGroup == ACCOUNT)
				{
					if(pPacket->bCmd	== INIT_CLIENT)
					{
						BuildPacketEx(pSendPacket, ACCOUNT, LOGINMAIN, buffer,1024);
						SET_DATA(loginData, ACCOUNT, LOGINMAIN, pSendPacket);
						sprintf(szTmpName,"test2%d",rand()%10000);
						strcpy(loginData->szLogin, szTmpName);
						strcpy(loginData->szPassword, "123456");
						s_dwCntLogin++;
						loginData->bSpeedReg	= 1;
						loginData->bJoin		= false;	//验证账号时不直接进
						memset(loginData->szMacAddress,0,32);
						strncpy(loginData->szMacAddress,g_pTPObj->TP_GetMacAddress(),30);
						loginData->nIOSVersion	= g_pTPObj->TP_GetSystemVersion();
						strncpy(loginData->szIDFA,g_pTPObj->TP_GetIDFA(),50);
						pPkC->m_SocketManager.SendPacket(pSendPacket);
					}
					else if(pPacket->bCmd	== LOGIN_RLT)
					{
						SET_DATA(pErr, ACCOUNT, LOGIN_RLT, pPacket);
						if(pErr->byErrorCode == LOGIN_SUCCESSFUL)
						{
							
						}
						pPkC->m_SocketManager.nConnectState	= 0;
						//pPkC->DisConnectServer();
					}
				}



				free(pPacket);
			}
		}
	}

	return;
#endif



	g_packetCenter.Update ();
	ASyncPListCenter::GetInstance().Update();
	if(m_dwTickDealError && xnGetTickCount() - m_dwTickDealError	> 12000)
	{
		pMainMenu->PopSystemMsg ("网络连接异常，请检查网络设置是否正确后再重新尝试");
		//g_packetCenter.DisConnectServer();
		m_dwTickDealError	= 0;
		//if(g_pTPObj->nLoginStep && g_pTPObj->TP_GetPlat() != 12)
			//g_pTPObj->TP_Login();
	}

	if(m_dwTickInit && xnGetTickCount() - m_dwTickInit > 8000 && g_packetCenter.m_bActive == false)
	{
		int	nRandAccLine	= rand()%3;
		m_pLineAcc	= ServerLineCfg::GetInstance().GetLine(0,nRandAccLine);
		if(m_pLineAcc == NULL)
			m_pLineAcc	= ServerLineCfg::GetInstance().GetLine(0,0);
		g_packetCenter.ConnectServer (m_pLineAcc->szIP,m_pLineAcc->nPort);
		m_dwTickInit	= xnGetTickCount();
	}

	g_pTPObj->TP_Update();
	if(g_pTPObj->nLoginStep == 2)
	{//第三方登录成功
		if(strlen(g_pTPObj->szToken) > 75)
		{
			BuildPacketEx(pPacket, ACCOUNT, TP_LOGIN_SPTOKEN, buffer,1024);
			SET_DATA(loginData, ACCOUNT, TP_LOGIN_SPTOKEN, pPacket);

			loginData->byTPID	= g_pTPObj->TP_GetPlat();
			memset(loginData->szMacAddress,0,32);
			strncpy(loginData->szMacAddress,g_pTPObj->TP_GetMacAddress(),30);
			strncpy(loginData->szToken, g_pTPObj->szToken,255);
			sprintf(loginData->szUin,"%d",g_pTPObj->nLoginMID);
			loginData->nIOSVersion	= g_pTPObj->TP_GetSystemVersion();
			strncpy(loginData->szIDFA,g_pTPObj->TP_GetIDFA(),50);
			SendPacket(pPacket);
		}
		else
		{
			BuildPacketEx(pPacket, ACCOUNT, TP_LOGINMAIN, buffer,1024);
			SET_DATA(loginData, ACCOUNT, TP_LOGINMAIN, pPacket);

			loginData->byTPID	= g_pTPObj->TP_GetPlat();
			memset(loginData->szMacAddress,0,32);
			strncpy(loginData->szMacAddress,g_pTPObj->TP_GetMacAddress(),30);
			strncpy(loginData->szToken, g_pTPObj->szToken,80);
			sprintf(loginData->szUin,"%d",g_pTPObj->nLoginMID);
			loginData->nIOSVersion	= g_pTPObj->TP_GetSystemVersion();
			strncpy(loginData->szIDFA,g_pTPObj->TP_GetIDFA(),50);
			SendPacket(pPacket);
		}
		
		m_dwTickDealError	= xnGetTickCount();
		pMainMenu->PopSystemMsg("正在登录中，即将进入游戏...",true);
		g_pTPObj->nLoginStep	= 3;
	}

	if(m_pLoginMenu->m_pForm->bOpened)
	{
		if(strlen(m_pLoginMenu->m_pForm->GetComponent("name")->GetEdit()) == 0)
			m_pLoginMenu->m_pForm->GetComponent("denglu")->SetVisible(false);
		else
			m_pLoginMenu->m_pForm->GetComponent("denglu")->SetVisible(true);
	}

	m_pCreateRoleMenu->update(dt);
	m_pChooseRoleMenu->Update();
	UpdateMovePlay();
	UpdateBackView();
	//if(g_packetCenter.Update () == false && m_dwTickDealError && xnGetTickCount () - m_dwTickDealError > 2000)
	//{
	//	pMainMenu->PopSystemMsg ("网络连接异常中断");
	//	//g_pIosTp->TP_Login ();
	//}
}

void LoginScene::UpdateMovePlay()
{
	if(m_dwPlayMoveTick == 0)
	{
		pMainInterface->GetDForm("moving")->Close();
		return;
	}
	else
	{
		pMainInterface->GetDForm("moving")->Open();
	}

	if(m_byMoveStep >= 4)
	{

		for (int i = 0; i < 4; i++)
		{
			if(m_pSpriteMove[i])
				m_pSpriteMove[i]->setVisible(false);
			if(m_pLableTxt[i])
				m_pLableTxt[i]->setVisible(false);
		}

		m_dwPlayMoveTick	= 0;
		m_pCreateRoleMenu->Open();
		return;
	}

	int		nMoveWidth	= nowScreenWidth/POINT_PIX_CNT;
	int		nMoveHeight	= nowScreenHeight/POINT_PIX_CNT;
	if(m_pSpriteMove[0] == NULL)
	{
		m_byMoveStep	= 0;
		m_pSpriteMove[0]	= GFun_InitSpriteByJPG("ui/YD1.jpg");
		m_pSpriteMove[0]->setPosition(ccp(nMoveWidth/2,nMoveHeight/2));
		m_pSpriteMove[0]->setScaleX(nowScreenWidth/960.0);
		m_pSpriteMove[0]->setScaleY(nowScreenHeight/640.0);
		addChild(m_pSpriteMove[0],7);
		m_pSpriteMove[1]	= GFun_InitSpriteByJPG("ui/YD2.jpg");
		m_pSpriteMove[1]->setScaleX(nowScreenWidth/960.0);
		m_pSpriteMove[1]->setScaleY(nowScreenHeight/640.0);
		m_pSpriteMove[1]->setPosition(ccp(nMoveWidth/2,nMoveHeight/2));
		addChild(m_pSpriteMove[1],5);
		m_pSpriteMove[2]	= GFun_InitSpriteByJPG("ui/YD3.jpg");
		m_pSpriteMove[2]->setScaleX(nowScreenWidth/960.0);
		m_pSpriteMove[2]->setScaleY(nowScreenHeight/640.0);
		m_pSpriteMove[2]->setPosition(ccp(nMoveWidth/2,nMoveHeight/2));
		addChild(m_pSpriteMove[2],3);
		m_pSpriteMove[3]	= GFun_InitSpriteByJPG("ui/YD4.jpg");
		m_pSpriteMove[3]->setScaleX(nowScreenWidth/960.0);
		m_pSpriteMove[3]->setScaleY(nowScreenHeight/640.0);
		m_pSpriteMove[3]->setPosition(ccp(nMoveWidth/2,nMoveHeight/2));
		addChild(m_pSpriteMove[3],1);
	}
	if(m_pLableTxt[0] == NULL)
	{
		m_pLableTxt[0]		= CCLabelTTF::create(UTEXT_CHINE(""),DEFAULT_FONT,30/POINT_PIX_CNT);
		m_pLableTxt[0]->setColor(ccc3(222,222,222));
		m_pLableTxt[0]->setHorizontalAlignment(kCCTextAlignmentLeft);
		m_pLableTxt[0]->setVerticalAlignment(kCCVerticalTextAlignmentBottom);
		m_pLableTxt[0]->setPosition(ccp(nMoveWidth/2,nMoveHeight/2));
		m_pLableTxt[0]->setDimensions(CCSize(nMoveWidth*7/10,nMoveHeight*3/4));
		addChild(m_pLableTxt[0],8);
		m_pLableTxt[1]		= CCLabelTTF::create(UTEXT_CHINE(""),DEFAULT_FONT,30/POINT_PIX_CNT);
		m_pLableTxt[1]->setColor(ccc3(222,222,222));
		m_pLableTxt[1]->setHorizontalAlignment(kCCTextAlignmentLeft);
		m_pLableTxt[1]->setVerticalAlignment(kCCVerticalTextAlignmentBottom);
		m_pLableTxt[1]->setPosition(ccp(nMoveWidth/2,nMoveHeight/2));
		m_pLableTxt[1]->setDimensions(CCSize(nMoveWidth*3/4,nMoveHeight*3/4));
		addChild(m_pLableTxt[1],6);
		m_pLableTxt[2]		= CCLabelTTF::create(UTEXT_CHINE(""),DEFAULT_FONT,30/POINT_PIX_CNT);
		m_pLableTxt[2]->setColor(ccc3(222,222,222));
		m_pLableTxt[2]->setHorizontalAlignment(kCCTextAlignmentLeft);
		m_pLableTxt[2]->setVerticalAlignment(kCCVerticalTextAlignmentBottom);
		m_pLableTxt[2]->setPosition(ccp(nMoveWidth/2,nMoveHeight/2));
		m_pLableTxt[2]->setDimensions(CCSize(nMoveWidth*3/4,nMoveHeight*3/4));
		addChild(m_pLableTxt[2],4);
		m_pLableTxt[3]		= CCLabelTTF::create(UTEXT_CHINE(""),DEFAULT_FONT,30/POINT_PIX_CNT);
		m_pLableTxt[3]->setColor(ccc3(222,222,222));
		m_pLableTxt[3]->setHorizontalAlignment(kCCTextAlignmentLeft);
		m_pLableTxt[3]->setVerticalAlignment(kCCVerticalTextAlignmentBottom);
		m_pLableTxt[3]->setPosition(ccp(nMoveWidth/2,nMoveHeight/2));
		m_pLableTxt[3]->setDimensions(CCSize(nMoveWidth*3/4,nMoveHeight*3/4));
		addChild(m_pLableTxt[3],2);
	}

	char	szTmp[256]	= {0};
	DWORD	dwTimePass	= xnGetTickCount() - m_dwPlayMoveTick;
	int		nPos	= dwTimePass/150;
	if(nPos > 100)
		nPos	= 100;
	memcpy(szTmp,m_szMoveTxt[m_byMoveStep],nPos*2);
	m_pLableTxt[m_byMoveStep]->setString(UTEXT_CHINE(szTmp));

	static int	s_markSound	= -1;
	static	int	s_nSoundID	= -1;
	if(dwTimePass > 8000)
	{
		s_markSound	= -1;
		if(s_nSoundID >= 0)
		{
			g_pSoundCenter->StopEffect(s_nSoundID);
			s_nSoundID	= -1;
		}
		if(m_pSpriteMove[m_byMoveStep]->numberOfRunningActions() == 0)
		{
			m_pSpriteMove[m_byMoveStep]->runAction(CCFadeOut::create(2.0f));
			m_pLableTxt[m_byMoveStep]->runAction(CCFadeOut::create(1.5f));
		}
	}
	else
	{
		if(dwTimePass < 7000 && dwTimePass /1800 != s_markSound)
		{
			s_markSound	= dwTimePass/1800;
			
			s_nSoundID	= g_pSoundCenter->PlayerEffect("Sound/typing");
		}
	}

	if(m_pSpriteMove[m_byMoveStep]->getOpacity() < 10)
	{
		m_pSpriteMove[m_byMoveStep]->setVisible(false);
		m_pLableTxt[m_byMoveStep]->setVisible(false);
		m_byMoveStep++;
		m_dwPlayMoveTick	= xnGetTickCount();
	}
}

void LoginScene::InitBackSprite()
{
	m_pSpriteBack[0]		= GFun_InitSpriteByJPG("other/SKY.jpg");
	m_pSpriteBack[0]->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/2));
	m_pSpriteBack[0]->setScaleX(nowScreenWidth/1024.0);
	m_pSpriteBack[0]->setScaleY(nowScreenHeight/768.0);
	addChild(m_pSpriteBack[0],-2);
	m_pSpriteBack[1]		= CCSprite::create("other/TREE.png");
	m_pSpriteBack[1]->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/2));
	m_pSpriteBack[1]->setScaleX(nowScreenWidth/1024.0);
	m_pSpriteBack[1]->setScaleY(nowScreenHeight/768.0);
	addChild(m_pSpriteBack[1],0);
	CCLog("1111111111STEP30");
	m_pSpriteCloud[0]	= CCSprite::create("other/YUN.png");
	m_pSpriteCloud[0]->setScaleY(nowScreenHeight/768.0);
	m_pSpriteCloud[0]->setAnchorPoint(ccp(0,1));
	m_pSpriteCloud[0]->setPosition(ccp(0,SCREEN_SIZE_H));
	addChild(m_pSpriteCloud[0],-1);

	{
		CCActionInterval* go = CCMoveTo::create (30,ccp(-1922/POINT_PIX_CNT,SCREEN_SIZE_H));
		CCActionInterval* hide	= CCMoveTo::create (0.1f,ccp(-1922/POINT_PIX_CNT,0));
		CCActionInterval* goback	= CCMoveTo::create (0.1f,ccp(1922/POINT_PIX_CNT,0));
		CCActionInterval* show	= CCMoveTo::create (0.1f,ccp(1922/POINT_PIX_CNT,SCREEN_SIZE_H));
		CCActionInterval* go1	= CCMoveTo::create (30,ccp(0,SCREEN_SIZE_H));

		CCFiniteTimeAction* seq = CCSequence::create(go, hide, goback, show, go1,NULL);

		m_pSpriteCloud[0]->runAction( (CCRepeatForever::create((CCActionInterval*) seq) ));
		m_pSpriteCloud[0]->getTexture ()->setAliasTexParameters ();
	}

	m_pSpriteCloud[1]	= CCSprite::create("other/YUN.png");
	m_pSpriteCloud[1]->setScaleY(nowScreenHeight/768.0);
	m_pSpriteCloud[1]->setAnchorPoint(ccp(0,1));
	m_pSpriteCloud[1]->setPosition(ccp(1922/POINT_PIX_CNT,SCREEN_SIZE_H));
	addChild(m_pSpriteCloud[1],-1);

	{
		CCActionInterval* go = CCMoveTo::create (60,ccp(-1922/POINT_PIX_CNT,SCREEN_SIZE_H));
		CCActionInterval* hide	= CCMoveTo::create (0.1f,ccp(-1922/POINT_PIX_CNT,0));
		CCActionInterval* goback	= CCMoveTo::create (0.1f,ccp(1922/POINT_PIX_CNT,0));
		CCActionInterval* show	= CCMoveTo::create (0.1f,ccp(1922/POINT_PIX_CNT,SCREEN_SIZE_H));

		CCFiniteTimeAction* seq = CCSequence::create(go, hide, goback, show, NULL);

		m_pSpriteCloud[1]->runAction( (CCRepeatForever::create((CCActionInterval*) seq) ));
		m_pSpriteCloud[1]->getTexture ()->setAliasTexParameters ();
	}
	CCLog("1111111111STEP31");
	m_pEffLoginBack[0]	= CCEffectSprite::create(EffectCenter::GetInstance().GetEffObj(217));
	m_pEffLoginBack[0]->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT*36/100,nowScreenHeight/POINT_PIX_CNT*20/100));
	m_pEffLoginBack[0]->setScaleX(nowScreenWidth/1024.0);
	m_pEffLoginBack[0]->setScaleY(nowScreenHeight/768.0);
	addChild(m_pEffLoginBack[0],0);
	CCLog("1111111111STEP32");
	m_pEffLoginBack[1]	= CCEffectSprite::create(EffectCenter::GetInstance().GetEffObj(218));
	m_pEffLoginBack[1]->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT*56/100,nowScreenHeight/POINT_PIX_CNT*16/100));
	m_pEffLoginBack[1]->setScaleX(nowScreenWidth/1024.0);
	m_pEffLoginBack[1]->setScaleY(nowScreenHeight/768.0);
	addChild(m_pEffLoginBack[1],0);
	
	m_pEffLoginBack[2]	= CCEffectSprite::create(EffectCenter::GetInstance().GetEffObj(214));
	m_pEffLoginBack[2]->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT*40/100,nowScreenHeight/POINT_PIX_CNT*15/100));
	addChild(m_pEffLoginBack[2],0);
	
	m_pEffLoginBack[3]	= CCEffectSprite::create(EffectCenter::GetInstance().GetEffObj(215));
	m_pEffLoginBack[3]->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT*20/100,nowScreenHeight/POINT_PIX_CNT*18/100));
	addChild(m_pEffLoginBack[3],0);

	m_pEffLoginBack[4]	= CCEffectSprite::create(EffectCenter::GetInstance().GetEffObj(216));
	m_pEffLoginBack[4]->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT*10/100,nowScreenHeight/POINT_PIX_CNT*18/100));
	addChild(m_pEffLoginBack[4],0);
}

void LoginScene::UpdateBackView()
{
	int bVisable	= !m_pCreateRoleMenu->m_pForm->bOpened;
	for (int i = 0; i < 5; i++)
	{
		if(m_pEffLoginBack[i] && m_pEffLoginBack[i]->CheckEnd())
			m_pEffLoginBack[i]	= NULL;
		if(m_pEffLoginBack[i])
			m_pEffLoginBack[i]->setVisible(bVisable);
	}

	m_pSpriteBack[0]->setVisible(bVisable);
	m_pSpriteBack[1]->setVisible(bVisable);
	m_pSpriteCloud[0]->setVisible(bVisable);
	m_pSpriteCloud[1]->setVisible(bVisable);
}