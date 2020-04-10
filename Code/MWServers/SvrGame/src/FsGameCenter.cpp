#include "stdafx.h"
#include "FsGameCenter.h"
#include "EquipConfig.h"
#include "ItemConfig.h"
#include "MailCenter.h"
#include "FamilyCenter.h"
#include "UserList.h"
#include "ServerCfg.h"
#include "ios_ac_packet.h"
#include "P2PAskCenter.h"
#include "TimeControl.h"
#include "StateConfig.h"
#include "SkillConfig.h"
#include "UserConfig.h"
#include "MallConfig.h"
#include "QuestConfig.h"
#include "MapCopyConfig.h"
#include "EnemyDropConfig.h"
#include "UserFlag.h"
#include "debug_def.h"
#include "LuaStateManager.h"
#include "TeamData.h"
#include "ItemManage.h"
#include "MapStaticCfg.h"
#include "ZLogFile.h"
#include "VipConfig.h"
#include "FortCenter.h"
#include "CallMapEnemy.h"
#include "ActivedegreeCfg.h"
#include "WarRank.h"
#include "MapObjCenter.h"
#include "MapConfig.h"
#include "GameAwardCfg.h"
#include "TextConfig.h"
#include "StarSysConfig.h"
#include "NASaleCenter.h"
#include "..\include\svnversion.h"
#include "ExpMultipleCfg.h"
#include "DBLogControl.h"
#include "WorldFlag.h"
#include "MentoringCfg.h"
#include "AchievementCfg.h"
#include "MarkGameDB.h"
#include "..\MWServers\SvrGame\LuaDialog.h"
#include "GMActiveCfg.h"

#include "ios_system_packet.h"

ZLogFile*		g_pLogFile				= NULL;
ZLogFile*       g_pErrorFile            = NULL;
ZLogFile*       g_pSqlFile            = NULL;
bool            g_bSvrExpMult         = false;
FsGameCenter*	g_pGameCenter	= NULL;
#define  WM_SERVER_INIT	WM_USER + 1101
#define  WM_HEART_BEAT	WM_USER + 1102
#define  WM_DEAL_PACKET	WM_USER + 1103

CLuaStateManager * pLuaStateManager = 0;
BOOLEAN	FnMainThread(void * data, MSG * Msg)
{
	FsGameCenter*	pFCenter	= (FsGameCenter*)data;
	switch (Msg->message)
	{
	case	WM_SERVER_INIT:
		{
			pFCenter->MTInitCenter();
		}
		break;
	case	WM_HEART_BEAT:
		{
			pFCenter->HeartBeat();
		}
		break;
	case	WM_DEAL_PACKET:
		{
			pFCenter->DealPacket();
		}
		break;
	}
	return TRUE;
}

void OutMessage(int channel, char* lpszFormat, ...)
{
	char msg[4096] = {0};
	vsprintf(msg, lpszFormat, (char*) (&lpszFormat + 1));
	switch(channel)
	{
	case msg_type_normal:
		{
			if(g_pLogFile != NULL)
				g_pLogFile->OutPut(msg);
		}
		break;
	case msg_type_error:
		{
			if(g_pErrorFile != NULL)
				g_pErrorFile->OutPut(msg);
		}
		break;
	case msg_type_sql:
		{
			if(g_pSqlFile != NULL)
				g_pSqlFile->OutPut(msg);
		}
		break;
	}
}

FsGameCenter::FsGameCenter():m_nDay(0),m_nLockCountry(0),m_nDefaultCountry(0)
{
	InitializeCriticalSection(&m_xLockJoin);
	InitializeCriticalSection(&m_xLockLuaExecute);
	m_pListWaitJoin	= xnList::Create();
	m_pListWaitExit	= xnList::Create();
	m_pListLockNpcCmd	= xnList::Create();
	m_pListNotice		= xnList::Create();

	m_pTeamCenter = new TeamCenter;
	m_pMapCopyCenter	= new MapCopyCenter;
	pLuaStateManager = new CLuaStateManagerPlus();
	m_pFortCenter = new FortCenter();
	m_pItemManage = new ItemManage;
	m_pSysMailCenter = new SysMailCenter;
	SetLuaStateManager(pLuaStateManager);
	m_pCallEnemyCenter	= new CallEnemyCenter();
	m_pActivityCenter = new GameActivityCenter();
	m_pMapObjCenter = new MapObjCenter();
	m_pWarRankCenter = new WarRankCenter();
	m_pWorldFlag = new WorldFlag();
	m_pGMActiveCenter = new GMActiveCenter();
	m_pWorldLog = new WorldLog();

	g_pLogFile		= new ZLogFile(0);
	g_pLogFile->Open("OutLog");
	g_pErrorFile	= new ZLogFile(0);
	g_pErrorFile->Open("Error");
	g_pSqlFile	= new ZLogFile(0);
	g_pSqlFile->Open("sql");

	InstallMyExceptHandle();

	srand(GetTickCount());

	g_pGameCenter	= this;
	m_pMainThread = new xnThread(FnMainThread, this, "mainthread");
	m_pMainThread->SetPriority(THREAD_PRIORITY_HIGHEST);
	m_pMainThread->Start();
	m_pMainThread->SendMessage(WM_SERVER_INIT,0,0,TRUE);
	m_byShutDown	= 0;
	m_dwShutTick	= 0;
	g_pMarkGameDB	= new MarkGameDB();

	m_pLuaList = xnList::Create();
	m_nGMSvrExpMult = 0;

	m_dwShutWaitTick = 0;
}

void FsGameCenter::FireHeartBeat()
{
	m_pMainThread->SendMessage(WM_HEART_BEAT,0,0,TRUE);
}

void FsGameCenter::FireDealPacket()
{
	m_pMainThread->SendMessage(WM_DEAL_PACKET,0,0,TRUE);
}

void FsGameCenter::MTInitCenter()
{	
	srand(GetTickCount());
	ItemConfig::GetInstance ().LoadFile ("data\\itemcfg.txt");
	EquipAttrConfig::GetInstance ().LoadFile ("data\\EquipAttachCfg.txt");
	ServerCfg::GetInstance().LoadFile("ServerCfg.txt");
	MapSelCfg::GetInstance().LoadFile("MapSelCfg.txt");
	HorseBaseCfg::GetInstance().LoadFile("data\\HorseBase.txt");
	HorseBaseCfg::GetInstance().LoadLvFile("data\\HorseLvConfig.txt");
	SlaveBaseCfg::GetInstance().LoadFile("data\\SlaveBase.txt");
	SlaveBaseCfg::GetInstance().LoadLvFile("data\\SlaveLvConfig.txt");
	SlaveStrengthenCfg::GetInstance().LoadFile("data\\SlaveStrengthen.txt");
	SlaveVariatCfg::GetInstance().LoadFile("data\\SlaveVariation.txt");
	EquipStarConfig::GetInstance().LoadFile("data\\EquipStarCfg.txt");
	EquipEatConfig::GetInstance().LoadFile("data\\EquipAbsorbCfg.txt");
	UFlagConfig::GetInstance().LoadFile("data\\rpgflag.txt");
	UserConfig::GetInstance().LoadFile("data\\playerCfg.txt");
	MallConfig::GetInstance().LoadFile("data\\mallCfg.txt");
	ShopConfig::GetInstance().LoadFile("data\\shopCfg.txt");
	ActiveConfig::GetInstance().LoadFile("data\\channel.txt");
	TransPointCfg::GetInstance().LoadFile("data\\MapTransCfg.txt");
	NpcPathLeadCfg::GetInstance().LoadFile("data\\NpcCfg.txt");
	QuestCfg::GetInstance().LoadFile("data\\questcfg.txt");
	SkillConfig::GetInstance().LoadFile("data\\SkillConfig.txt");
	SkillConfig::GetInstance().LoadFileJob("data\\jobskill.txt");
	StateConfig::GetInstance().LoadFile("data\\StateConfig.txt");
	MCopyConfig::GetInstance().LoadFile("data\\mapcopycfg.txt");
	DropGroupConfig2::GetInstance().LoadFile("data\\itemdropgroup.txt");
	EnemyKillConfig::GetInstance().LoadFile("data\\enemykill.txt");
	TestLibraryCfg::GetInstance().LoadFile("data\\TestLibrary.txt");
	TestAwardCfg::GetInstance().LoadFile("data\\TestAward.txt");
	WarRankItemConfig::GetInstance().LoadFile("data\\fightcfg.TXT");
	VipConfig::GetInstance().LoadFile("data\\VipConfig.txt");
	TitleConfig::GetInstance().LoadFile("data\\TitleConfig.txt");
	ActiveDegree::GetInstance().LoadFile("data\\ActivedegreeCfg.txt");
	MapViewConfig::GetInstance().LoadFile("data\\mapconfig.ini");
	GameActivityCfg::GetInstance().LoadFile("data\\GameActivityCfg.txt");
	TurntableCfg::GetInstance().LoadFile("data\\Turntablecfg.txt");
	GameAwardConfig::GetInstance().LoadFile("data\\GameAwardCfg.txt");
	StarSysCfg::GetInstance().LoadFile("data\\StarSysCfg.txt");
	RandNameConfig::GetInstance().LoadFile("data\\RandNameCfg.txt");
	ExpMultipleCfg::GetInstance().LoadFile("data\\ExpMultipleCfg.txt");
	ACBossCfg::GetInstance().LoadFile("data\\bossinfo.txt");
	MentoringCfg::GetInstance().LoadFile("data\\MentoringCfg.txt");
	AchievementCfg::GetInstance().LoadFile("data\\AchievementCfg.txt");
	AchievementCfg::GetInstance().LoadAwardFile("data\\AchieveAwardCfg.txt");
	MakeSkillConfig::GetInstance().LoadFile("data\\makeitem.txt");
	MarrySkillCfg::GetInstance().LoadFile("data\\MarrySkillCfg.txt");
	EquipGroupConfig::GetInstance().LoadFile("data\\EquipGroupCfg.txt");
	VipAwardConfig::GetInstance().LoadFile("data\\VipAwardcfg.txt");
	GMActiveConfig::GetInstance().LoadFile("data\\gmactivecfg.txt");
	MapLimitConfig::GetInstance().LoadFile("data\\MapLimitCfg.txt");

	m_nLockCountry = ServerCfg::GetInstance().nLockCountry;
	m_nDefaultCountry = ServerCfg::GetInstance().nDefaultCountry;

	g_pDBase	= new DBControl("mainDB");
	if(!g_pDBase->ConnectToDB(ServerCfg::GetInstance().szDBIP,ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd,ServerCfg::GetInstance().szDBName))
		AfxMessageBox("Link DB Error");
	g_pDBase->Start();

	for (int i = 0; i < 8; i++)
	{
		m_ppPlayerDB[i]	= new DBControl("PlayerDB");
		if(!m_ppPlayerDB[i]->ConnectToDB(ServerCfg::GetInstance().szDBIP,ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd,ServerCfg::GetInstance().szDBName))
			AfxMessageBox("Link DB Error");
		m_ppPlayerDB[i]->Start();
	}
	for (int i = 0; i < 2; i++)
	{
		m_ppLoginDB[i]	= new DBControl("LoginDB");
		if(!m_ppLoginDB[i]->ConnectToDB(ServerCfg::GetInstance().szDBIP,ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd,ServerCfg::GetInstance().szDBName))
			AfxMessageBox("Link DB Error");
		m_ppLoginDB[i]->Start();
	}

	g_pLogDB	= new DBLogControl("mainLog");
	if(g_pLogDB->ConnectToDB(ServerCfg::GetInstance().szDBIP,ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd,ServerCfg::GetInstance().szDBName))
		g_pLogDB->CreateLogDB(true);
	g_pLogDB->Start();

	g_pNetCenter	= new ZNetCenter();
	g_pNetCenter->StartNetCenter();

	//m_pWorldFlag->InitDB(g_pDBase);要放在靠前位置
	m_pWorldFlag->InitDB(g_pDBase);
	m_pMailCenter = new MailCenter();
	m_pMailCenter->DBInitMail(g_pDBase);
	m_pNASaleCenter	= new NASaleCenter();
	m_pNASaleCenter->DB_Init(g_pDBase);
	m_pHomeCenter	= new HomeCenter();
	m_pHomeCenter->DBInit(g_pDBase);
	m_pWorldTestCenter = new WorldTestCenter(g_pDBase);
	m_pWorldTestCenter->DBInit();
	m_pRankCenter = new RankCenter(g_pDBase);
	m_pRankCenter->DBInit();
	g_pUserList		= new UserList();
	g_pUserList->DBInit(g_pDBase->pDBServer);
	m_pWarRankCenter->DBInit(g_pDBase);
	m_pFortCenter->InitDB(g_pDBase);
	m_pItemManage->DBInit(g_pDBase);
	m_pSysMailCenter->DBInit(g_pDBase);
	
	g_pMentoringCenter	= new MentoringCenter(g_pDBase);
	g_pMentoringCenter->DBInit();
	g_pMarryCenter	= new MarryCenter(g_pDBase);
	g_pMarryCenter->DBInit();
	m_pAchievementCenter = new AchievementCenter(g_pDBase);
	m_pAchievementCenter->DBInit();
	OutMessage(msg_type_normal,"MTInitCenter OK SVN: %s",SVN_VERSION);
	OutMessage(msg_type_error,"MTInitCenter OK SVN: %s",SVN_VERSION);
	m_pRankCenter->Sort();

	m_pMapObjCenter->Init();
}

FsGameCenter::~FsGameCenter()
{
	HMapUserDataPtr	itor	= m_mapUsers.begin();
	while(itor != m_mapUsers.end())
	{
		delete itor->second;
		m_mapUsers.erase(itor++);
	}
	SAFE_DELETE(g_pLogFile);
	SAFE_DELETE(g_pErrorFile);
	SAFE_DELETE(g_pSqlFile);
	m_pListWaitJoin->Free();
	m_pListWaitExit->Free();
	m_pListLockNpcCmd->Free();
	m_pListNotice->Free();
	DeleteCriticalSection(&m_xLockJoin);
	DeleteCriticalSection(&m_xLockLuaExecute);
	m_pMainThread->Stop();
	delete m_pMainThread;
	delete m_pWorldTestCenter;
	delete m_pRankCenter;
	delete m_pHomeCenter;
	delete m_pMailCenter;
	delete m_pNASaleCenter;
	delete m_pTeamCenter;
	delete m_pMapCopyCenter;
	delete m_pFortCenter;
	delete m_pCallEnemyCenter;
	delete m_pWorldFlag;
	delete m_pMapObjCenter;
	delete m_pAchievementCenter;
	delete g_pMentoringCenter;
	delete g_pMarryCenter;
	delete m_pGMActiveCenter;
}

void FsGameCenter::DBT_PlayerJoinWait( UserData* pUser )
{
	EnterCriticalSection(&m_xLockJoin);
	m_pListWaitJoin->Add(pUser);
	LeaveCriticalSection(&m_xLockJoin);
}

//void FsGameCenter::PlayerExitGame( DWORD dwUin )
//{
//	UserData*	pUser	= GetPlayer(dwUin);
//	if (pUser)
//	{
//		pUser->LeaveGame();
//	}
//
//}
extern ZPacket* PacketDup(ZPacket * pPacket);
void FsGameCenter::RecvPacket( ZPacket* pPacket, DWORD dwUin )
{
	switch(pPacket->bCmdGroup)
	{
	case ACCOUNT:
		{
			switch(pPacket->bCmd)
			{
			case C_REQUEST_JOIN:
				{
					m_ppLoginDB[rand()%2]->UserJoinGame(PacketDup(pPacket),false);
					//g_pDBase->UserJoinGame(pPacket,true);
				}
				return;
			case C_CREATE_CHARACTER:
				{
					m_ppLoginDB[rand()%2]->CreateUser(PacketDup(pPacket),false);
				}
				return;
			case C_DELETE_CHARACTER:
				{
					SET_DATA(pRecv,ACCOUNT,C_DELETE_CHARACTER,pPacket);
					_deletePlayer(pRecv->dwUin);
					BuildPacketEx(pSendPacket,ACCOUNT,S_DELETE_CHARACTER,buf,1024);
					SET_DATA(pSend,ACCOUNT,S_DELETE_CHARACTER,pSendPacket);
					pSend->dwUin = pRecv->dwUin;
					TCP_SendPacketToKey(pPacket->dwKey,pSendPacket);
				}
				return;
			case LOGOUT:
				{
					SET_DATA(pRecv,ACCOUNT,LOGOUT,pPacket);
					_playerExitGame(pRecv->dwUin);
				}
				return;
			case GA_RECV:
				{
					SET_DATA(pRecv,ACCOUNT,GA_RECV,pPacket);
					UserData* pUser	= GetPlayer(pRecv->nUin);
					if(pUser)
					{
						pUser->RecvPacket(pPacket);
					}
				}
				return;
			case IAP_CHECK_FAIL:
				{
					SET_DATA(pRecv,ACCOUNT,IAP_CHECK_FAIL,pPacket);
					g_pLogDB->ExecuteSQL(false,"insert into "RPG_PAYERROR_LOG" (tpid,ctime,logdesc) values (10,now(),'%s')",pRecv->szAccount);
				}
				return;
			case TPCOMM_PAY:
				{//充值
					SET_DATA(pRecv,ACCOUNT,TPCOMM_PAY,pPacket);
					//UserData* pUser	= NULL;
					//EnterCriticalSection(&m_xLockJoin);
					//for (int i = 0; i < m_pListWaitJoin->Count; i++)
					//{//在等待队列的直接让他进入游戏
					//	UserData* pTmp	= (UserData*)m_pListWaitJoin->Items[i];
					//	if(pTmp->m_dwAccount	== pRecv->nAccUin)
					//	{
					//		m_pListWaitJoin->Delete(i);
					//		_playerJoinGame(pTmp);
					//		pUser	= pTmp;
					//		break;
					//	}
					//}
					//LeaveCriticalSection(&m_xLockJoin);

					//if(pUser == NULL)
					UserData* pUser	= g_pGameCenter->GetPlayerByAcc(pRecv->nAccUin);
					if(pUser)
					{
						pUser->RecvPacket(pPacket);
					}
					else	
					{//改数据库
						UserSimple* pUS = NULL;
						if(pRecv->nAccUin > 0)
							pUS = g_pUserList->GetUserByAcc(pRecv->nAccUin);

						if(pUS && (pUS->dwTotalRmb == 0 || pUS->nMuliPay))
						{
							g_pUserList->SetTotalRmb(pRecv->nAccUin,pUS->dwTotalRmb+pRecv->nRealVal);
							g_pDBase->ExecuteSQL(FALSE,"update "RPG_ACCOUNT_TABLE" set rmbmoney=rmbmoney+%d,rechargetemp=rechargetemp+%d,rmbtotal=rmbtotal+%d,temprmbpay=temprmbpay+%d where accountUin=%d",pRecv->nRealVal*2,pRecv->nRealVal,pRecv->nRealVal,pRecv->nRealVal,pRecv->nAccUin);
							g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_RMB_LOG" (uin,accuin,leftrmb,addval,type,ctime,logdesc) values (%d,%d,%d,%d,%d,now(),'%s')",0,pRecv->nAccUin,0,pRecv->nRealVal,RMB_SAVE_TPCOMMPAY,pRecv->szOrder);
							if(pUS->nMuliPay)
							{
								g_pDBase->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set mulipay=0 where Uin=%d",pUS->dwUin);
								pUS->nMuliPay	= 0;
							}
						}
						else
						{
							if(pUS)
								g_pUserList->SetTotalRmb(pRecv->nAccUin,pUS->dwTotalRmb+pRecv->nRealVal);
							g_pDBase->ExecuteSQL(FALSE,"update "RPG_ACCOUNT_TABLE" set rmbmoney=rmbmoney+%d,rechargetemp=rechargetemp+%d,rmbtotal=rmbtotal+%d,temprmbpay=temprmbpay+%d where accountUin=%d",pRecv->nRealVal,pRecv->nRealVal,pRecv->nRealVal,pRecv->nRealVal,pRecv->nAccUin);
							g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_RMB_LOG" (uin,accuin,leftrmb,addval,type,ctime,logdesc) values (%d,%d,%d,%d,%d,now(),'%s')",0,pRecv->nAccUin,0,pRecv->nRealVal,RMB_SAVE_TPCOMMPAY,pRecv->szOrder);
						}
					}
					AddMarkPayTable(pRecv->nAccUin, pRecv->nRealVal);
				}
				return;
			}
		}
		break;
	case GMTOOL:
		{
			switch(pPacket->bCmd)
			{
			case C_LUA:
				{
					SET_DATA(pRecv,GMTOOL,C_LUA,pPacket);
					LockSection tLock(g_pGameCenter->m_xLockLuaExecute);
					std::string szTxt = pRecv->szLua;
					std::string szResult;
					OutMessage(msg_type_normal,"GMTOOL INPUT: acc(%s) lua(%s)",pRecv->szAccount,pRecv->szLua);
					QueryRlt xRlt;
					if(!g_pDBase->pDBServer->Query(&xRlt,"SELECT * FROM mw_rpg_gmtable.gmtooluser WHERE accname='%s' and accpswd='%s'",pRecv->szAccount,pRecv->szPassword)) 
						return;
					MySqlQuery* pCmd = xRlt.GetRlt();
					if (pCmd == NULL||!pCmd->FetchRow())
					{
						szResult = "账号或密码不正确";
					}else{
						bool	bLuaSuccess = true;
						LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
						if (pLuaState)
						{
							int nret = pLuaState->DoBuffer(szTxt.c_str(),szTxt.size(),"temp_lua_execute");
							if (nret)
							{
								printf(lua_tostring(pLuaState->GetCState(),-1));

								szResult = lua_tostring(pLuaState->GetCState(),-1);
								pLuaState->Pop();
								bLuaSuccess = false;
							}else{
								szResult = "LuaExecute:" + szTxt;
								szResult += " success.";
							}
						}
						g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_LUA_LOG" (id,logdesc,bsuccess,ctime) values ('%s','%s',%d,now())",
							pRecv->szAccount,pRecv->szLua,bLuaSuccess);
					}
					BuildPacketEx(pPacket1,GMTOOL,C_LUA,buf, 1024);
					SET_DATA(pSend,GMTOOL,C_LUA,pPacket1 );
					strncpy(pSend->szLua,szResult.c_str(),511);
					pPacket1->dwKey = pPacket->dwKey;
					g_pNetCenter->SendNGPacket(pPacket1,pPacket->dwKey);
				}
				return;
			}

			//pSvr->pLinkServer->SendPacket(pPacket);
		}
		break;
	}
	if(m_pHomeCenter->RecvPacket(pPacket,dwUin))
		return;
	if(m_pNASaleCenter->RecvPacket(pPacket,dwUin))
		return;

	if(dwUin)
	{
		HMapUserDataPtr	itor	= m_mapUsers.find(dwUin);
		if(itor != m_mapUsers.end())
		{
			itor->second->RecvPacket(pPacket);
		}
	}
	else
	{
		if(m_pCallEnemyCenter->RecvPacket(pPacket))
			return;
	}

}

void FsGameCenter::SendGameSvrCloseTick(DWORD dwTick)
{
	m_dwShutWaitTick = GetTickCount() + dwTick*1000;

	HMapUserDataPtr itor	= m_mapUsers.begin();
	while(itor != m_mapUsers.end())
	{
		itor->second->SendGSCloseWait(dwTick);
		itor++;
	}
}

static DWORD s_dwCheckUin	= 0;
static int	s_nErrorCnt	= 0;
void FsGameCenter::Update()
{
	if(m_byShutDown == 99)
		return;
	if(m_byShutDown == 1)
	{
		HMapUserDataPtr itor	= m_mapUsers.begin();
		while(itor != m_mapUsers.end())
		{
			itor->second->LeaveGame();
			itor++;
		}
		m_byShutDown	= 99;
		m_dwShutTick	= GetTickCount();
		return;
	}

	if (m_dwShutWaitTick && GetTickCount() > m_dwShutWaitTick )
	{
		m_dwShutWaitTick = 0;

		HMapUserDataPtr itor	= m_mapUsers.begin();
		while(itor != m_mapUsers.end())
		{
			itor->second->LeaveGame();
			itor++;
		}
		m_byShutDown	= 99;
		m_dwShutTick	= GetTickCount();
		return;
	}

	EnterCriticalSection(&m_xLockJoin);
	for (int i = m_pListWaitJoin->Count-1; i>= 0; i--)
	{//可能出现玩家数据被2个线程操作的情况,延迟200ms
		UserData* pUser	= (UserData*)m_pListWaitJoin->Items[i];
		if(pUser->m_wJoinWaitTick == 0 || pUser->m_wJoinWaitTick > 50)
		{
			_playerJoinGame(pUser);
			m_pListWaitJoin->Delete(i);
		}
		else
			pUser->m_wJoinWaitTick--;
	}
	//while(m_pListWaitJoin->Count)
	//{
	//	UserData* pUser	= (UserData*)m_pListWaitJoin->Delete(0);
	//	_playerJoinGame(pUser);
	//}
	LeaveCriticalSection(&m_xLockJoin);
	//g_pWarPkCenter->Update();
	if (!m_nDay)
	{
		m_nDay = CMTime::GetCurTime().tm_yday;
	}
	if (m_nDay != CMTime::GetCurTime().tm_yday)
	{
		m_nDay = CMTime::GetCurTime().tm_yday;
		ResetDailyData();
		memset(m_pActivityCenter->m_bActivityTime,true,24);
	}
	if(s_dwCheckUin)
	{//只有被try  catch过了才
		OutMessage(msg_type_error,"USER ERROR DUMP: %d",s_dwCheckUin);
		if(s_nErrorCnt > 10)
		{//连续10次失败就把玩家T下线
			OutMessage(msg_type_error,"USER ERROR DUMP KICKOUT: %d",s_dwCheckUin);
			//_playerExitGame(s_dwCheckUin);
			HMapUserDataPtr	itor	= m_mapUsers.find(s_dwCheckUin);
			if(itor != m_mapUsers.end())
			{
				UserData* pUser		= itor->second;
				//pUser->LeaveGame();
				m_mapUsers.erase(itor);
				std::string strName(pUser->m_szNick);
				HMapUserNameDataPtr itor	= m_mapNameUsers.find(strName);
				if(itor != m_mapNameUsers.end())
					m_mapNameUsers.erase(itor);
				//pUser->m_dwTickLogout	= GetTickCount();
				//m_pListWaitExit->Add(pUser);
			}


			s_dwCheckUin	= 0;
		}
	}

	s_nErrorCnt++;
	HMapUserDataPtr itor	= m_mapUsers.begin();
	while(itor != m_mapUsers.end())
	{
		s_dwCheckUin	= itor->second->m_dwUin;
		itor->second->TryUpdate();
		s_dwCheckUin	= 0;
		itor++;
	}
	s_nErrorCnt	= 0;
	static DWORD	s_dwCenterUpdateCnt	= 0;
	s_dwCenterUpdateCnt++;
	if(s_dwCenterUpdateCnt % 10 == 0)
		UpdateSec();
	m_pTeamCenter->Update();
	m_pRankCenter->Update();
	m_pWorldTestCenter->Update();
	g_pMarryCenter->Update();
	g_pMentoringCenter->Update();
	m_pGMActiveCenter->Update();
}

void FsGameCenter::UpdateSec()
{
	//数据库断线重连
	if(g_pDBase->pDBServer->IsConnected() == false)
		g_pDBase->ConnectToDB(ServerCfg::GetInstance().szDBIP,ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd,ServerCfg::GetInstance().szDBName);
	for (int i = 0; i < 8; i++)
	{
		if(m_ppPlayerDB[i]->pDBServer->IsConnected() == false)
			m_ppPlayerDB[i]->ConnectToDB(ServerCfg::GetInstance().szDBIP,ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd,ServerCfg::GetInstance().szDBName);
	}
	for (int i = 0; i < 2; i++)
	{
		if(m_ppLoginDB[i]->pDBServer->IsConnected() == false)
			m_ppLoginDB[i]->ConnectToDB(ServerCfg::GetInstance().szDBIP,ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd,ServerCfg::GetInstance().szDBName);
	}
	if(g_pLogDB->pDBServer->IsConnected() == false)
		g_pLogDB->ConnectToDB(ServerCfg::GetInstance().szDBIP,ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd,ServerCfg::GetInstance().szDBName);


	CheckLuaExecute();
	HMapUserDataPtr itor	= m_mapUsers.begin();
	while(itor != m_mapUsers.end())
	{
		itor->second->Update25();
		itor++;
	}

	DWORD	dwCurTick	= GetTickCount();
	for (int i = m_pListWaitExit->Count-1; i >= 0; i--)
	{
		UserData*	pUser	= (UserData*)m_pListWaitExit->Items[i];
		if(dwCurTick - pUser->m_dwTickLogout > 15000)
		{//15秒后对象销毁
			m_pListWaitExit->Delete(i);
			delete pUser;
		}
	}
	P2pAskCenter::GetInstance().UpdateCheck();
	static DWORD	s_dwUpdateTick	= 0;

	if(s_dwUpdateTick%60 == 0)
	{
		m_pMailCenter->UpdateMintue();
		m_pSysMailCenter->Update();
		m_pNASaleCenter->UpdateMinute();
		m_pHomeCenter->UpdateMintue();
	}
	m_pHomeCenter->UpdateSec();
		
	if(s_dwUpdateTick%600 == 0)
		m_pWarRankCenter->Update10Mintue();

	s_dwUpdateTick++;
	m_pWorldTestCenter->Update25();
	m_pMapCopyCenter->UpdateSec();
	m_pMapObjCenter->UpdateSec();
	m_pActivityCenter->UpdateSec();

	g_pMarkGameDB->CheckTimeEvent();
	
	m_pWorldLog->UpdateSec();
	static DWORD s_LastTime = 0;
	time_t nNow = CMTime::GetCurTickCount();
	static int nDiffTime = 300;
#ifdef _DEBUG
	nDiffTime = 5;
#endif // _DEBUG
	if (nNow%nDiffTime == 0&&s_LastTime/nDiffTime!=nNow/nDiffTime)
	{
		s_LastTime = nNow;
		LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
		LuaObject loFunction = pLuaState->GetGlobal("extend_update");
		if(loFunction.IsFunction()){
			LuaFunction<void> lf(loFunction);

			CLuaPlusSafeFunction_1<void> func;
			if(!func(pLuaState, lf,NULL))
			{
				g_pLogFile->OutPut("脚本函数extend_update执行错误");
			}
		}
	}

	for (int i = m_pListNotice->Count-1; i >= 0; i--)
	{
		GMMsgObj* pMsg	= (GMMsgObj*)m_pListNotice->Items[i];
		if(pMsg->dwTickNextShow <= GetTickCount())
		{
			ChatCenter::GetInstance().SendSystemMsg(pMsg->szMsg,pMsg->bChannel,0,1);
			pMsg->nTime--;
			pMsg->dwTickNextShow	= GetTickCount()+pMsg->dwSplitSec*1000;

			if(pMsg->nTime <= 0)
			{
				m_pListNotice->Delete(i);
				free(pMsg->szMsg);
				delete pMsg;
			}
		}
	}
	

}


int FsGameCenter::RandGetPlayerByLv(DWORD dwOutUin, int nLvS, int nLvE, xnList* pList,int nCnt )
{
	HMapUserDataPtr	itor	= m_mapUsers.begin();
	while(itor != m_mapUsers.end())
	{
		if(itor->second->m_xUWealth[UW_LV] >= nLvS && itor->second->m_xUWealth[UW_LV] <= nLvE && itor->second->m_dwUin != dwOutUin)
		{
			pList->Add(itor->second);
		}


		if(pList->Count >= nCnt)
			break;
		itor++;
	}
	return pList->Count;
}

void FsGameCenter::GMAddSvrExpMult(int nID,bool bRemove)
{
	if (bRemove)
		m_nGMSvrExpMult = 0;
	else
		m_nGMSvrExpMult = nID;

	HMapUserDataPtr	itor	= m_mapUsers.begin();
	while(itor != m_mapUsers.end())
	{
		UserData* pUser = itor->second;
		if (pUser)
		{
			if (bRemove)
				pUser->m_pUserExpCenter->RemoveObj(nID);
			else
				pUser->m_pUserExpCenter->AddObj(nID);
		}
		itor++;
	}
}

UserData* FsGameCenter::GetPlayer( DWORD dwUin )
{
	HMapUserDataPtr	itor	= m_mapUsers.find(dwUin);
	if(itor != m_mapUsers.end())
	{
		return	itor->second;
	}
	return NULL;
}

void FsGameCenter::GetUinList(xnList* pList)
{
	HMapUserDataPtr	itor	= m_mapUsers.begin();
	while(itor != m_mapUsers.end())
	{
		UserData* pUser = itor->second;
		pList->Add((void*)pUser->m_dwUin);
		itor++;
	}
}

void FsGameCenter::GetUinListByCountry(xnList* pList,BYTE byCountry)
{
	HMapUserDataPtr	itor	= m_mapUsers.begin();
	while(itor != m_mapUsers.end())
	{
		UserData* pUser = itor->second;
		if (byCountry&&pUser->m_byCountry == byCountry)
		{
			pList->Add((void*)pUser->m_dwUin);
		}
		itor++;
	}
}

void	FsGameCenter::GetPlayerByLv(int nMine,int nMin,int nMax,xnList* pList)
{
	HMapUserDataPtr	itor	= m_mapUsers.begin();
	while(itor != m_mapUsers.end())
	{
		UserData* pUser = itor->second;
		if(pUser->m_dwUin != nMine &&pUser->m_xUWealth[UW_LV] >= nMin && pUser->m_xUWealth[UW_LV] <= nMax)
			pList->Add((void*)pUser);
		itor++;
	}
}

UserData* FsGameCenter::GetPlayerByname(char* szName)
{
	if(szName == NULL)
		return NULL;
	std::string strName(szName);
	HMapUserNameDataPtr itor	= m_mapNameUsers.find(strName);
	if(itor != m_mapNameUsers.end())
		return itor->second;
	return NULL;
}

void FsGameCenter::_playerJoinGame( UserData* pUser )
{
	HMapUserDataPtr	itor	= m_mapUsers.find(pUser->m_dwUin);
	if(itor == m_mapUsers.end())
	{
		m_mapUsers[pUser->m_dwUin]	= pUser;
		if (g_bSvrExpMult)
		{
			pUser->m_pUserExpCenter->AddObj(m_nGMSvrExpMult);
		}
	}

	std::string strName(pUser->m_szNick);
	HMapUserNameDataPtr itor2	= m_mapNameUsers.find(strName);
	if(itor2 == m_mapNameUsers.end())
		m_mapNameUsers[strName]	= pUser;

	//发送玩家登录后的信息并同步amzdata
	//需要网关确认完登陆信息后才正式进入游戏
	pUser->NoticeLoginOk();
}

void FsGameCenter::_playerExitGame( DWORD dwUin )
{
	HMapUserDataPtr	itor	= m_mapUsers.find(dwUin);
	if(itor != m_mapUsers.end())
	{
		UserData* pUser		= itor->second;
		m_mapUsers.erase(itor);
		pUser->LeaveGame();
		std::string strName(pUser->m_szNick);
		HMapUserNameDataPtr itor	= m_mapNameUsers.find(strName);
		if(itor != m_mapNameUsers.end())
			m_mapNameUsers.erase(itor);
		pUser->m_dwTickLogout	= GetTickCount();
		m_pListWaitExit->Add(pUser);
	}
}

void	FsGameCenter::_deletePlayer(DWORD dwUin)	//删除角色
{
	g_pGameCenter->m_pRankCenter->DeleteUser(dwUin);	//排行榜
	g_pMentoringCenter->DeleteUser(dwUin);				//师徒
	g_pMarryCenter->DeleteUser(dwUin);					//婚姻
	m_pAchievementCenter->DeleteUser(dwUin);			//成就

	g_pUserList->DeleteUser(dwUin);						//角色
	g_pDBase->ExecuteSQL(FALSE,"delete from "RPG_USER_TABLE" where uin = %d",dwUin);		
	g_pDBase->ExecuteSQL(FALSE,"delete from "RPG_NEWSLAVE_TABLE" where uin = %d",dwUin);	//宠物
	g_pDBase->ExecuteSQL(FALSE,"delete from "RPG_STATE_TABLE" where uin = %d",dwUin);	//状态
	g_pDBase->ExecuteSQL(FALSE,"delete from "RPG_SKILL_TABLE" where uin = %d",dwUin);	//技能
	g_pDBase->ExecuteSQL(FALSE,"delete from "RPG_ITEM_TABLE" where uin = %d",dwUin);	//物品
	g_pDBase->ExecuteSQL(FALSE,"delete from "RPG_EQUIPEX_TABLE" where uin = %d",dwUin);	//装备
}

void FsGameCenter::_dealClientPackets()
{
	xnList*	pLPackets	= g_pNetCenter->SwitchDealPList();
	while(pLPackets->Count)
	{
		ZPacket*	pPacket	= (ZPacket*)pLPackets->Delete(0);
#ifndef _DEBUG
		__try
#endif
		{
			RecvPacket(pPacket,pPacket->dwKey);
		}
#ifndef _DEBUG
		__except(DUMP_CALLSTACK())
		{
			char	szDmpMsg[256];
			sprintf(szDmpMsg,"FsGameCenter _dealClientPackets Error: %d--%d---%d\n",pPacket->bCmdGroup,pPacket->bCmd,pPacket->dwKey);
			DUMP_STRING(szDmpMsg);
		}
#endif
		free(pPacket);
	}
}

void FsGameCenter::DealPacket()
{
#ifndef _DEBUG
	__try
#endif
	{
		_dealClientPackets();
	}
#ifndef _DEBUG
	__except(DUMP_CALLSTACK())
	{
		DUMP_STRING("FsGameCenter DealPacket Error!\n");
	}
#endif
}

void FsGameCenter::HeartBeat()
{
#ifndef _DEBUG
	__try
#endif
	{
		Update();
	}
#ifndef _DEBUG
	__except(DUMP_CALLSTACK())
	{
		DUMP_STRING("FsGameCenter HeartBeat Error!\n");
	}
#endif
}

void FsGameCenter::RefreshUiView( CListCtrl* pCtrl )
{//多线程原因废弃
	//char	szTmp[256];
	//pCtrl->DeleteAllItems();
	//HMapUserDataPtr itor	= m_mapUsers.begin();
	//while(itor != m_mapUsers.end())
	//{
	//	sprintf(szTmp,"%d",itor->second->m_dwUin);
	//	int nPos	= pCtrl->InsertItem(pCtrl->GetItemCount(),szTmp);
	//	sprintf(szTmp,"%d",itor->second->m_xUWealth[UW_LV]);
	//	pCtrl->SetItemText(nPos,1,szTmp);
	//	pCtrl->SetItemText(nPos,2,itor->second->m_szNick);
	//	sprintf(szTmp,"%d",itor->second->m_nMIndex);
	//	pCtrl->SetItemText(nPos,3,szTmp);
	//	sprintf(szTmp,"%d",itor->second->m_xUWealth[UW_GOLD]);
	//	pCtrl->SetItemText(nPos,4,szTmp);
	//	sprintf(szTmp,"%d",itor->second->m_xUWealth[UW_RMB]);
	//	pCtrl->SetItemText(nPos,5,szTmp);
	//	sprintf(szTmp,"%d",itor->second->m_xUWealth[UW_BINDRMB]);
	//	pCtrl->SetItemText(nPos,6,szTmp);
	//	itor++;
	//}
}

void FsGameCenter::ResetDailyData()
{
	m_pHomeCenter->SendBattleDailyReward();
	if(g_pLogDB->ChangeLogDB(true))
		g_pLogDB->CreateLogDB(true);
	g_pMentoringCenter->ResetDataFlag();
	m_pMapCopyCenter->ReFreshDailyLmt();
	//m_pHomeCenter->CheckChangeLeader();
	m_pWorldLog->SaveAll();
}

UserData* FsGameCenter::GetPlayerByAcc( DWORD dwAcc )
{
	HMapUserDataPtr itor	= m_mapUsers.begin();
	while(itor != m_mapUsers.end())
	{
		if(itor->second->m_dwAccount == dwAcc)
			return itor->second;
		itor++;
	}
	return NULL;
}

void FsGameCenter::SendPValInfo( int nPVal,int nType,int nValEx /*= 0*/ )
{
	BuildPacketEx(pSendPack,ACCOUNT,GA_GMDATA,buf,256);
	SET_DATA(pSend, ACCOUNT,GA_GMDATA,pSendPack);
	pSend->nPVal	= nPVal;
	pSend->nType	= nType;
	pSend->nExVal	= nValEx;

	g_pNetCenter->SendNGPacket(pSendPack,0);
}

void FsGameCenter::GetUserPayData( int nVal1,int nVal2,map<int,int>& mapData )
{
 	QueryRlt xRlt;
	char sztxt[64];
	sprintf(sztxt,"and temprmbpay<%d",nVal2);
 	if(!g_pDBase->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_ACCOUNT_TABLE" WHERE temprmbpay>%d %s ",nVal1,nVal2 == 0?" ":sztxt)) 
 		return;
 
 	MySqlQuery* pCmd = xRlt.GetRlt();
 	if (pCmd != NULL)
 	{
 		while(pCmd->FetchRow())
 		{	
			int nPay = pCmd->GetInt("temprmbpay");
			int nUin = pCmd->GetInt("AccountUin");
			if (nPay<nVal1)
			{
				continue;
			}
			if (nVal2 !=0&&nPay>nVal2)
			{
				continue;
			}
			mapData[nUin] = nPay;
 		}
 	}
}

void FsGameCenter::AddUserRmbByScript( int nUin,int nVal )
{
	UserData* pUser	= g_pGameCenter->GetPlayerByAcc(nUin);
	if(pUser)
	{
		pUser->m_xUWealth.AddWealth(UW_RMB,nVal,RMB_SAVE_SCRPIT,"活动脚本加");
	}
	else	
	{//改数据库
		g_pDBase->ExecuteSQL(FALSE,"update "RPG_ACCOUNT_TABLE" set rmbmoney=rmbmoney+%d where accountUin=%d",nVal,nUin);
		g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_RMB_LOG" (uin,accuin,leftrmb,addval,type,ctime,logdesc) values (%d,%d,%d,%d,%d,now(),'%s')",0,nUin,0,nVal,RMB_SAVE_SCRPIT,"活动脚本加");
	}
}

void FsGameCenter::ClearTempPay()
{
	g_pDBase->ExecuteSQL(FALSE,"update "RPG_ACCOUNT_TABLE" set temprmbpay=0 ");

}

void FsGameCenter::GetUserByLeague( int nLeague,set<int>& setData )
{
	HomeObj* pHome = m_pHomeCenter->_getHomeObj(nLeague);
	if (!pHome)
	{
		return;
	}
	for (int i=0;i<pHome->m_pList->Count;i++)
	{
		UserSimple* pInfo = (UserSimple*)pHome->m_pList->Items[i];
		setData.insert(pInfo->dwUin);
	}
	
}

int FsGameCenter::GetOnlineCnt()
{
	return m_mapUsers.size();
}

void FsGameCenter::PushNoticeExecute(NoticeStringTag* pTag)
{
	m_strNotice.assign(pTag->szNotice.c_str());
}

void FsGameCenter::PushLuaExecute( luaStringTag* pTag )
{
	LockSection tLock(m_xLockLuaExecute);
	//EnterCriticalSection(&m_xLockLuaExecute);
	m_pLuaList->Add(pTag);
	//LeaveCriticalSection(&m_xLockLuaExecute);
}

void FsGameCenter::CheckLuaExecute()
{
	LockSection tLock(m_xLockLuaExecute);
	//EnterCriticalSection(&m_xLockLuaExecute);
	if (!m_pLuaList->Count)
	{
		//LeaveCriticalSection(&m_xLockLuaExecute);
		return;
	}
	for (int i=m_pLuaList->Count-1;i>=0;i--)
	{
		luaStringTag* pTag = (luaStringTag*)m_pLuaList->Items[i];
		printf("LuaExecute:%s\r\n",pTag->szLua.c_str());
		LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
		if (pLuaState)
		{
			int nret = pLuaState->DoBuffer(pTag->szLua.c_str(),pTag->szLua.size(),"temp_lua_execute");
			if (nret)
			{
				printf(lua_tostring(pLuaState->GetCState(),-1));
				g_pLuaDialog->AddOutput(lua_tostring(pLuaState->GetCState(),-1));
				pLuaState->Pop();
			}else{
				string temps = "LuaExecute:" + pTag->szLua;
				temps += " success.";
				g_pLuaDialog->AddOutput(temps.c_str());
			}
		}
		delete (luaStringTag*)m_pLuaList->Delete(i);
	}
	
	//LeaveCriticalSection(&m_xLockLuaExecute);
}

int FsGameCenter::CheckForbid(int nID)
{
	std::map<int,DWORD>::iterator itor = m_mapForbid.find(nID);
	if (itor!=m_mapForbid.end())
	{
		DWORD dwTick = CMTime::GetCurTickCount();
		if (itor->second<dwTick)
		{
			m_mapForbid.erase(itor);
			return 0;
		}else{
			return itor->second-dwTick;
		}
	}else{
		return 0;
	}
}

void FsGameCenter::ForbidPlayer( int nID,DWORD dwTime )
{
	m_mapForbid[nID] = CMTime::GetCurTickCount()+dwTime;
}

bool FsGameCenter::CheckEverForbid(int nID)
{
	std::map<int,bool>::iterator itor = m_mapEverForbid.find(nID);
	if (itor!=m_mapEverForbid.end())
	{		
		if (itor->second == false)
		{
			m_mapEverForbid.erase(itor);
			return false;
		}
		else
			return itor->second;
	}else
	{
		return false;
	}
}

void FsGameCenter::EverForbidPlayer( int nID,bool byType )
{
	m_mapEverForbid[nID] = byType;
	g_pDBase->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set everforbid=%d where uin=%d",byType,nID);
}

void FsGameCenter::KickPlayer( int nUin,DWORD dwTime )
{
	TCP_SendCmdToUin(nUin,ACCOUNT,OUT_BY_KICK);
	//_playerExitGame(nUin);
}

void FsGameCenter::UnablePlayer( int nUin ,bool bForbid)
{
	//KickPlayer(nUin,0);
	g_pDBase->ExecuteSQL(FALSE,"update "RPG_ACCOUNT_TABLE" set accstate=%d where accountUin=%d",bForbid?1:0,nUin);

}

void FsGameCenter::AddMarkPayTable(int nAccUin, int nRealVal)
{
	bool	bFind	= false;
	QueryRlt xRlt;
	MySqlQuery* pCmd = NULL;
	if(g_pDBase->pDBServer->Query(&xRlt,"SELECT * FROM "MARK_PAY_TABLE" where accuin = %d",nAccUin))
	{
		pCmd = xRlt.GetRlt();
		if(pCmd != NULL && pCmd->FetchRow())
		{
			bFind	= true;
		}
	}
	if(!bFind)
	{
		g_pDBase->ExecuteSQL(FALSE, "insert into "MARK_PAY_TABLE" (accuin,totalpay,paytime) values(%d,%d,now()",
			nAccUin, nRealVal);
	}else
	{
		g_pDBase->ExecuteSQL(FALSE, "update "MARK_PAY_TABLE" set totalpay = totalpay+%d,paytime = now() where accuin = %d",
			nRealVal, nAccUin);
	}
}

LockSection::LockSection( CRITICAL_SECTION& cLock ):csLock(cLock)
{
	EnterCriticalSection(&csLock);

}

LockSection::~LockSection()
{
	LeaveCriticalSection(&csLock);

}
