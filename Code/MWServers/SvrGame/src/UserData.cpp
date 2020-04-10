#include "stdafx.h"
#include "UserData.h"
#include "EquipConfig.h"
#include "ios_user_packet.h"
#include "FriendCenter.h"
#include "MailCenter.h"
#include "ItemConfig.h"
#include "ios_ac_packet.h"
#include "FamilyObject.h"
#include "ios_home_packet.h"
#include <stack>
#include "FsGameCenter.h"
#include "TimeControl.h"
#include "DebugMsg.h"
#include "P2PAskCenter.h"
#include "ios_muli_packet.h"
#include "FamilyCenter.h"
#include "ios_yjgslave_packet.h"
#include "SlaveOpCenter.h"
#include "ios_rpgchat_packet.h"
#include "UserFlag.h"
#include "UserConfig.h"
#include "MapStaticCfg.h"
#include "UserQuest.h"
#include "TeamData.h"
#include "EnemyDropConfig.h"
#include "QuestConfig.h"
#include "UserList.h"
#include "FortCenter.h"
#include "ItemManage.h"
#include "MapObjCenter.h"
#include "CallMapEnemy.h"
#include "MapConfig.h"
#include "ZLogFile.h"
#include "ios_award_packet.h"
#include "StarSysConfig.h"
#include "WarRank.h"
#include "GameActivityCfg.h"
#include "ios_relation_packet.h"
#include "MakeSkillData.h"
#include "MentoringCfg.h"
#include "GameAwardCfg.h"
#include <algorithm>
#include <iterator>
#include "ServerCfg.h"
#include "MarkGameDB.h"
#include "DBLogControl.h"
#include "WorldFlag.h"
#include "debug_def.h"

using namespace std;
extern DBLogControl* g_pLogDB;
extern ZLogFile* g_pLogFile;
extern CLuaStateManager * pLuaStateManager;

static int nConvoyReward[4][9] = {
	{10000,15000,22500,33750,50625,75938,113906,170859,256289},
	{16000,24000,36000,54000,81000,121500,182250,273375,410063},
	{25600,38400,57600,86400,129600,194400,291600,437400,656100},
	{40960,61440,92160,138240,207360,311040,466560,699840,1049760},
};

//---------------------------------DB--------------------------
UserData*	UserData::CreateOpUserByDB(DWORD dwUin,DBControl* pDB)
{
	UserData*	pUser	= new UserData(dwUin,pDB);

	if(pUser->DBInit())
	{
		return pUser;
	}

	delete pUser;
	return	NULL;
}


int CalcDistance(int nX1,int nY1,int nX2,int nY2)
{
	int	nRtl	= (int)sqrt((float)((nX1-nX2)*(nX1-nX2)+(nY1-nY2)*(nY1-nY2)));
	if(nRtl <= 0)
		nRtl	= 1;
	return nRtl;
}

int CalcDistance2(int nX1,int nY1,int nX2,int nY2)
{
	return (nX1-nX2)*(nX1-nX2)+(nY1-nY2)*(nY1-nY2);
}
//---------------------------------DB--------------------------
UserData::UserData(DWORD dwUin,DBControl* pDB):m_xUWealth(this),m_pTeamData(NULL),m_nContribute(0),m_pFortData(NULL)
	,m_nSysMailID(0),m_nFastItemID(0),m_pWarRankFight(NULL),m_nScore(0),m_bIfEquipState(true),m_pFightTarget(NULL),m_nTPID(0),m_dwAccCreateTime(0),m_bIsRecharge(true),m_dwResetTime(0)
	,m_nMaxRankPos(99999)
{
	m_dwKey		= 0;
	m_pDB		= pDB;
	m_dwUin		= dwUin;
	m_dwAccount	= 0;
	m_bLogOut	= false;
	m_bEnterGame	= false;
	m_pAmzDManager	= new AmzDataManager(dwUin,pDB);

	m_pFriendList = new FriendList(this);
	m_pMailBox = new MailBox(dwUin);
	for (int i=0;i<CHAT_ROOM_MAX;i++)
	{
		m_pChatRoom[i] = NULL;
	}
	m_pDegreeList = xnList::Create();
	m_byDegreeLevel = 0;
	m_byMCopyAwardNum	= 0;
	m_pMySimple	= NULL;
	m_pLuckData = new UserLuckData(this);
	m_pUserRecord	= new UserRecord(this);
	m_pUserFlag		= new UserFlag(this);
	m_pQuestCenter	= new UserQuestCenter(this);
	m_pUserCD		= new UserCoolDown(this);
	m_pActivityMsgCenter = new NAMsgCenter(this);
	m_pUserMarry = new UserMarry(this);
	m_pSlaveCenter = new SlaveCenter(this);
	m_pHorseCenter = new HorseCenter(this);
	m_pMObjCenter		= new UserMObjCenter(this);
	m_pItemCenter = new ItemCenter(this);
	m_pSkillCenter	= new SkillCenter(this);
	m_pVIPCenter	= new VIPCenter(this);
	m_pTitleCenter	= new TitleCenter(this);
	m_pOnlineAwardCenter = new OnlineAwardCenter(this);
	m_pRechargeAwardCenter = new RechargeAwardCenter(this);
	m_pLvupAwardCenter = new LvupAwardCenter(this);
	m_pLoginAwardCenter = new LoginAwardCenter(this);
	m_pMounthAwardCenter = new MounthAwardCenter(this);
	m_pUserExpCenter = new UserExpCenter(this);
	m_pUserMentoring = new UserMentoring(this);
	m_pUserAchievement = new UserAchievement(this);
	m_pMakeSkillData = new MakeSkillData(this);
	m_pMapCopy		= NULL;
	m_pMCopyTeam	= NULL;
	m_timeRun	= CMTime::GetCurTime();
	m_bSendUserParam	= false;
	m_dwDaylyOnline	= 0;
	m_byGMLv		= 0;
	m_wJoinWaitTick	= 2;
	m_dwSCCheckTime	= 0;
	m_pTurntableCenter = new TurntableCenter(this);
	CLuaClassObject<UserData>::Init(this);

	memset(&m_xHomeSkill,0,sizeof(UserHomeSkill));
	m_dwCopyStartTick = 0;

	m_nStarState = 0;
	m_nStoneState = 0;
	m_nRechargeTemp = 0;

	memset(m_szInviteCode,0,10);
	memset(m_szBeInvCode,0,10);
	m_wCheckInvNum	= 0;
	m_wReachInvNum	= 0;
	m_bFirstOpenCard = false;
	m_dwTotalRmb = 0;
	m_nMuliPay	 = 0;
	m_nLoginDays = 0;
	m_bChangeCountry = false;
	m_nMCopyCardCnt	= 0;
	m_bySlaveAct = 1;
	m_bCheckChgCountry = true;

	m_bLoginAwarded = false;
	m_bWarnJJCTip = true;
}

UserData::~UserData()
{
	delete	m_pAmzDManager;
	delete	m_pUserRecord;
	delete	m_pUserFlag;
	delete	m_pUserCD;
	delete	m_pQuestCenter;
	delete	m_pLuckData;
	delete	m_pActivityMsgCenter;
	delete	m_pUserMarry;
	delete	m_pFriendList;
	delete	m_pMailBox;
	delete	m_pMObjCenter;
	delete	m_pItemCenter;
	delete	m_pVIPCenter;
	delete	m_pTitleCenter;
	delete	m_pTurntableCenter;
	delete	m_pOnlineAwardCenter;
	delete	m_pRechargeAwardCenter;
	delete	m_pLvupAwardCenter;
	delete  m_pLoginAwardCenter;
	delete	m_pMounthAwardCenter;
	delete	m_pUserMentoring;
	delete	m_pUserAchievement;
	if(m_pDegreeList)
	{
		while(m_pDegreeList->Count)
			delete (UserDegreePart*)m_pDegreeList->Delete(0);
		m_pDegreeList->Free();
	}
	delete	m_pUserExpCenter;
	delete m_pMakeSkillData;
	delete m_pSlaveCenter;
	delete m_pHorseCenter;
	delete m_pSkillCenter;
}

bool UserData::_dbFillDataWith()
{
	QueryRlt xRlt;

	if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_USER_TABLE","RPG_ACCOUNT_TABLE" WHERE "RPG_USER_TABLE".uin = %d and "RPG_USER_TABLE".AccountUin = "RPG_ACCOUNT_TABLE".AccountUin", 	m_dwUin)) 
		return false;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd != NULL)
	{
		if(pCmd->FetchRow())
		{	
			m_dwAccount		= pCmd->GetInt("AccountUin");
			strcpy(m_szNick, pCmd->GetStr("Nickname"));
			m_dwNextItemID	= pCmd->GetInt("nextitemid");

			m_byJob			= pCmd->GetInt("job");
			m_byCountry		= pCmd->GetInt("country");
			m_bySex			= pCmd->GetInt("sex");

			m_wReviveMap	= pCmd->GetInt("rmindex");
			m_wRevivePosX	= pCmd->GetInt("rmposx");
			m_wRevivePosY	= pCmd->GetInt("rmposy");

			m_nMIndex		= pCmd->GetInt("mapindex");

			m_nContribute	= pCmd->GetInt("homecontribution");
			m_nMaxContribute = pCmd->GetInt("homemaxcontribution");
			m_bLoginAwarded  = pCmd->GetInt("loginawarded");

			m_nSysMailID	= pCmd->GetInt("sysmailid");
			m_byDegreeLevel = pCmd->GetInt("degreelevel");
			m_nFastItemID	= pCmd->GetInt("fastitem");
			m_dwDaylyOnline	= pCmd->GetInt("daylyonline");
			m_nRechargeTemp = pCmd->GetInt("rechargetemp");

			m_nMaxRankPos = pCmd->GetInt("maxrankpos");

			m_nLoginDays  = pCmd->GetInt("logindays");
			m_nTPID = pCmd->GetInt("tpid");
			m_dwAccCreateTime = CMTime(pCmd->GetStr("createtime")).GetTickCount();

			m_dwResetTime = CMTime(pCmd->GetStr("ResetTime")).GetTickCount();

			int nPoxX		= pCmd->GetInt("mapx");
			int nPoxY		= pCmd->GetInt("mapy");

			char*	szTmp	= pCmd->GetStr("beinvitecode");
			if(szTmp[0]=='$')
				memset(m_szBeInvCode,0,10);
			else
				strncpy(m_szBeInvCode,szTmp,10);

			if(m_nMIndex == 0 || m_nMIndex >= 1000)
			{
				if(m_wReviveMap < 1000)
				{
					m_nMIndex	= m_wReviveMap;
					nPoxX		= m_wRevivePosX;
					nPoxY		= m_wRevivePosY;
				}
				else
				{
					if(m_byCountry == 1)
					{
						m_nMIndex	= 101;
						nPoxX		= 1146;
						nPoxY		= 1310;
					}
					else
					{
						m_nMIndex	= 102;
						nPoxX		= 1146;
						nPoxY		= 1310;
					}
				}
			}

			m_nRealMIndex	= m_nMIndex;
			m_byGMLv		= pCmd->GetInt("gmlv");

			m_xUWealth.DBInitWealth(UW_POW,pCmd->GetInt("power"));
			m_xUWealth.DBInitWealth(UW_LV,pCmd->GetInt("lvl"));
			m_xUWealth.DBInitWealth(UW_EXP,pCmd->GetInt("exp"));
			m_xUWealth.DBInitWealth(UW_EXP_CNT,pCmd->GetInt("expcnt"));
			m_xUWealth.DBInitWealth(UW_GOLD,pCmd->GetInt("gold"));
			m_xUWealth.DBInitWealth(UW_RMB,pCmd->GetInt("rmbmoney"));
			m_xUWealth.DBInitWealth(UW_BINDRMB,pCmd->GetInt("rmbbind"));
			m_xUWealth.DBInitWealth(UW_VIPLV,pCmd->GetInt("viplvl"));
			m_xUWealth.DBInitWealth(UW_VIPEXP,pCmd->GetInt("vipexp"));
			m_xUWealth.DBInitWealth(UW_SKILLPT,pCmd->GetInt("skillpt"));
			m_xUWealth.DBInitWealth(UW_STARLV,pCmd->GetInt("starlv"));
			m_xUWealth.DBInitWealth(UW_LIFE_STARLV,pCmd->GetInt("lifestarlv"));
			m_xUWealth.DBInitWealth(UW_GSLAVELMT,pCmd->GetInt("getslavelimit"));
			m_xUWealth.DBInitWealth(UW_HONOUR,pCmd->GetInt("honour"));
			m_dwTotalRmb	= pCmd->GetInt("rmbtotal");
			m_nMuliPay		= pCmd->GetInt("mulipay");
			m_dwHomeID		= pCmd->GetInt("homeid");
			m_dwHomeDuty		= pCmd->GetInt("homeduty");
			m_pLuckData->m_dwVipLv = pCmd->GetInt("viplvl");
			m_pHorseCenter->m_dwEquipID = pCmd->GetInt("equiphorse");
			m_pSlaveCenter->m_dwActiveID= pCmd->GetInt("activepet");
			m_pUserMarry->m_ifShow = pCmd->GetInt("showmarrytitle");
			m_pOnlineAwardCenter->m_dwOnlineSec = pCmd->GetInt("onlineawardsec");
			m_pOnlineAwardCenter->m_dwStart = GetTickCount()/1000 - m_pOnlineAwardCenter->m_dwOnlineSec;

			m_pItemCenter->SetBagSize(pCmd->GetInt("bagsize"));

			m_pMObjCenter->InitPlayer(m_dwUin,m_szNick,m_nMIndex,nPoxX,nPoxY,m_byJob,m_bySex);
			m_pMObjCenter->m_pPlayer->xParam.nHp	= pCmd->GetInt("hp");
			m_pMObjCenter->m_pPlayer->xParam.nMp	= pCmd->GetInt("mp");
			m_pMObjCenter->m_pPlayer->byLv	= m_xUWealth.GetWealth(UW_LV);
			//m_nDay = ;
			m_timeRun		= CMTime(pCmd->GetStr("LogoutTime"));


			CMTime tmCreat	= CMTime(pCmd->GetStr("createtime"));
			if(tmCreat[time_yeardays] == CMTime::GetCurTime().tm_yday)
			{
				m_bIsNewAccount	= true;
				g_pMarkGameDB->ChgOnline(true,true);
			}
			else
			{
				m_bIsNewAccount	= false;
				g_pMarkGameDB->ChgOnline(false,true);
			}

			int nLen = 0;
			BYTE* pData = pCmd->GetBLOB("flag",&nLen);
			m_pUserFlag->DBInitFlag(pData,nLen);

			pData = pCmd->GetBLOB("slimitflag",&nLen);
			m_pUserFlag->DBInitSLimit(pData,nLen);

			pData = pCmd->GetBLOB("questflag",&nLen);
			m_pUserFlag->DBInitQuest(pData,nLen);
			//{"cfgflag",			"longBLOB"},	做成就用吧
			pData = pCmd->GetBLOB("cdflag",&nLen);
			m_pUserCD->DBInitCD(pData,nLen);

			pData = pCmd->GetBLOB("activedegree",&nLen);
			if(pData && nLen)
			{
				g_pGameCenter->m_pActivityCenter->LoadActiveDegree(this,pData,nLen);
			}

			pData = pCmd->GetBLOB("cfgflag",&nLen);
			m_pUserFlag->DBInitMark(pData,nLen);

			pData = pCmd->GetBLOB("homeskill",&nLen);
			_loadHomeSkill(pData,nLen);

			pData = pCmd->GetBLOB("expmultiple",&nLen);
			m_pUserExpCenter->SetData(pData,nLen);

			pData = pCmd->GetBLOB("makeskill",&nLen);
			m_pMakeSkillData->LoadMakeSkillInfo(pData,nLen);
			_getPowerByTime(CMTime::GetCurTickCount(),m_timeRun.GetTickCount());

			m_pVIPCenter->Reflesh(m_xUWealth[UW_VIPLV]);

			if(m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_INVITE_TABLE" WHERE uinsvr=%d", 	m_dwUin)) 
			{
				pCmd = xRlt.GetRlt();
				if (pCmd != NULL && pCmd->FetchRow())
				{
					strncpy(m_szInviteCode,pCmd->GetStr("invitecode"),10);

					m_wCheckInvNum	= pCmd->GetInt("checknum");
					m_wReachInvNum	= pCmd->GetInt("reachnum");
				}
			}

			return true;
		}
	}
	return false;
}

void UserData::Update()
{
	if(m_bEnterGame == false)
		return;
	m_pAmzDManager->Update();
	m_pUserMentoring->Update();
	m_pUserMarry->Update();
	m_pUserRecord->Update();
	if(m_pActivityMsgCenter)
		m_pActivityMsgCenter->Update();
	if(m_pSlaveCenter)
		m_pSlaveCenter->Update();
	if(m_pHorseCenter)
		m_pHorseCenter->Update();
	if (m_bSendUserParam)
	{
		m_bSendUserParam = false;
		MapObjChange();

		_sendUserParam();
		//g_pGameCenter->m_pRankCenter->CheckRank(RANK_ATTACK,(void*)m_dwUin,true);
		//g_pGameCenter->m_pRankCenter->UpdateUser(this);
	}
	m_pTurntableCenter->Update();
	m_pUserExpCenter->Update();

	if(m_nRechargeTemp > 0)
	{
		if(m_pVIPCenter)
			m_pVIPCenter->AddVIPRMB(m_nRechargeTemp);
		m_pRechargeAwardCenter->RefreshFlag();
		m_pRechargeAwardCenter->RefreshFlagEx();
		m_nRechargeTemp = 0;
		m_pDB->ExecuteSQL(FALSE,"update "RPG_ACCOUNT_TABLE" set rechargetemp=0 where accountUin=%d",m_dwAccount);
	}
	if (m_pFriendList)
		m_pFriendList->UpdateCheckFrdOnline();

	m_pMObjCenter->Update();
	//if (!m_nDay)
	//{
	//	m_nDay = CMTime::GetCurTime().tm_yday;
	//}
	//if (m_nDay != CMTime::GetCurTime().tm_yday)
	//{
	//	int nTimeCount = m_pCByteData->GetData(BYTE_DATA_LOGINTIMES)<<8>>8;
	//	int nGetFlag = m_pCByteData->GetData(BYTE_DATA_LOGINTIMES)>>24;
	//	if (m_nDay == CMTime::GetCurTime().tm_yday - 1)
	//	{
	//		nTimeCount++;
	//	}else{
	//		nTimeCount = 0;
	//	}
	//	nGetFlag = 0;
	//	m_pCByteData->SetData(BYTE_DATA_LOGINTIMES,nTimeCount|(nGetFlag<<24));

	//	m_nDay = CMTime::GetCurTime().tm_yday;
	//	ResetDailyData();
	//}
}

bool UserData::CheckResetLoginDays()
{
	int nTmpDays = m_nLoginDays>1000?m_nLoginDays-1000:m_nLoginDays;
	if (m_nLoginDays < 1000 && nTmpDays < 30)
		return false;

	if (m_nLoginDays > 1000 && nTmpDays < 7)
		return false;

	int nGotDays = 0;
	int nLoginFlag = m_pUserFlag->GetFlag(50);
	int nCheck = 0x01;
	bool bResetD = false;
	for(int i = 0; i < 30; i++)
	{
		if(i > 0)
			nCheck = (nCheck<<1);
		if((nLoginFlag&nCheck) == nCheck)
			nGotDays++;
	}

	if (m_nLoginDays < 1000 && nGotDays < 30)
		return false;

	if (m_nLoginDays > 1000 && nGotDays < 7)
		return false;

	return true;
}

void UserData::CheckSendLoginDays(bool bAdd)
{
	if (bAdd || m_nLoginDays == 0)
	{
		if (CheckResetLoginDays())
		{
			m_nLoginDays = 1000;
			m_pUserFlag->SetFlag(50,0);
		}

		SetLoginAwarded(false);
		m_nLoginDays++;
		m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set logindays=%d where uin=%d",m_nLoginDays,m_dwUin);
	}

	BuildPacketEx(pPacket,USER, S_SEND_LOGIN_DAYS, buf, 1024);
	SET_DATA(pData, USER, S_SEND_LOGIN_DAYS, pPacket);
	pData->nLoginD = m_nLoginDays;
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::SendUserInfo()
{
	BuildPacketEx(pPacket,USER, S_USER_INFO, buf, 1024);
	SET_DATA(pData, USER, S_USER_INFO, pPacket);
	pData->dwUin	= m_dwUin;
	// 	pData->wHomeID	= m_dwHomeID;
	// 	pData->byHomeDuty = m_dwHomeDuty;
	//	pData->byVipLvl	= m_pLuckData->m_dwVipLv;
	pData->byCountry	= m_byCountry;
	strcpy(pData->szNick,m_szNick);
	pData->byJob	= m_byJob;
	pData->bySex	= m_bySex;
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::LeaveGame()
{
	m_pAmzDManager->SaveLeave();
	m_bLogOut	= true;
	if(m_bIsNewAccount)
		g_pMarkGameDB->ChgOnline(true,false);
	else
		g_pMarkGameDB->ChgOnline(false,false);
	OutMessage(msg_type_normal,"User %d LeaveGame:  %d(%d)",m_dwUin,m_nMIndex,m_nRealMIndex);
	if(m_pMCopyTeam)
		m_pMCopyTeam->pLRqMCopy->Remove((void*)m_dwUin);
	m_pMCopyTeam	= NULL;
	m_pMObjCenter->ObjLeaveMap(m_pMObjCenter->m_pSlave);
	m_pMObjCenter->ObjLeaveMap(m_pMObjCenter->m_pPlayer);
	m_pMObjCenter->OffCtrl();
	g_pGameCenter->m_pMapObjCenter->Leave(this,m_nMIndex);
	g_pGameCenter->m_pRankCenter->UpdateUser(this);
	m_pUserRecord->SaveAllLog();

	if (m_pFightTarget)
	{
		_onFightResult(false);
	}

	if(m_nMIndex >= 1000)
	{
		g_pGameCenter->m_pMapCopyCenter->RequestLeaveMCopy(m_dwUin,m_nMIndex);
		m_pMapCopy	= NULL;
		m_nMIndex	= m_wOldMIndex;
		if(m_pMObjCenter->m_pPlayer)
		{
			m_pMObjCenter->m_pPlayer->nPosX	= m_wOldPosX;
			m_pMObjCenter->m_pPlayer->nPosY	= m_wOldPosY;
		}
	}

	for (int i=0;i<CHAT_ROOM_MAX;i++)
	{
		ChatCenter::GetInstance().UnRegChatObj(m_pChatRoom[i],this);
	}

	m_pQuestCenter->UpdateMinuteOrLeave();
	m_xUWealth.SaveToDB(m_pDB,m_dwUin,m_dwAccount);
	m_pSlaveCenter->SaveAll();
	m_pHorseCenter->SaveAll();
	m_pUserAchievement->SaveAll();
	if(m_pMObjCenter && m_pMObjCenter->m_pPlayer)
	{
		m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set mapindex=%d,mapx=%d,mapy=%d,hp=%d,mp=%d, LogoutTime = now() where uin=%d",
			m_nMIndex,m_pMObjCenter->m_pPlayer->nPosX,m_pMObjCenter->m_pPlayer->nPosY,m_pMObjCenter->m_pPlayer->xParam.nHp,m_pMObjCenter->m_pPlayer->xParam.nMp,m_dwUin);
	}
	else
		m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set LogoutTime = now() where uin=%d",m_dwUin);
	if (m_dwHomeID)
	{
		HomeObj* pHome = g_pGameCenter->m_pHomeCenter->_getHomeObj(m_dwHomeID);
		if (pHome)
		{
			pHome->RemoveActiveMember(m_dwUin);
		}
	}
// 	if (m_pTeamData)
// 	{
// 		m_pTeamData->OnlineChg(m_pTeamData->GetPosByUin(m_dwUin),false,this);
// 	}
	LeaveTeam();
	m_pVIPCenter->SendWorldMessage(false);
	g_pGameCenter->m_pRankCenter->CheckAllUserRank(this);

	if (m_pMySimple)
	{
		m_pMySimple->bOnline = false;
	}

	m_pOnlineAwardCenter->_SaveOnlineSec();
	RecordLoginOut(m_dwUin,1);
}

void UserData::JoinGame()
{
	m_bEnterGame	= true;
	m_bLogOut	= false;
	m_pMObjCenter->m_dwKey		= m_dwKey;
	m_pMObjCenter->m_dwUin		= m_dwUin;

	m_pMObjCenter->SetMapIndex(m_nMIndex,m_nRealMIndex);

	OutMessage(msg_type_normal,"User %d JoinGame:  %d(%d)",m_dwUin,m_nMIndex,m_nRealMIndex);

	CMTime	tmNow	= CMTime::GetCurTime();
	//GetPowerByTime(tmNow.GetTickCount(),m_timeRun.GetTickCount());
	bool ifDfWeek = false;
	bool ifDfMounth = false;
	CMTime m_timeRun1 = CMTime(m_dwResetTime);
	if(tmNow.GetTime().tm_mon != m_timeRun1.GetTime().tm_mon)
	{//不周月
		ifDfMounth = true;
	}
	if(tmNow.GetTime().tm_wday != m_timeRun1.GetTime().tm_wday)
	{//不同周
		//ResetDailyData(true,true);
		ifDfWeek = true;
	}
	if(tmNow.GetTime().tm_yday != m_timeRun1.GetTime().tm_yday)
	{//不同天
		ResetDailyData(ifDfWeek,true,ifDfMounth);
	}
	m_dwResetTime	= tmNow.GetCurTickCount();

	SendUserInfo();	//进入前就已经发送相关信息NoticeLoginOk
	CheckSendLoginDays(false);
	m_xUWealth.SetTotalSend();

	m_pOnlineAwardCenter->SendInfo(true);
	m_pMounthAwardCenter->SendInfo();
	m_pRechargeAwardCenter->RefreshFlag();
	m_pRechargeAwardCenter->RefreshFlagEx();

	g_pGameCenter->m_pSysMailCenter->GetSystemMail(this);
	g_pGameCenter->m_pMailCenter->GetMyMails(this,m_pMailBox);
	g_pMentoringCenter->RequestTotalInfo(this);

	SendMailList();

	RecalcParam();
	MapObjChange();

	for (int i=0;i<=CHAT_ROOM_P2P;i++)
	{
		ChatCenter::GetInstance().RegChatObj(i,0,this);	//加入各个公共频道，私聊也放在公共频道
	}
	ChatCenter::GetInstance().RegChatObj(CHAT_ROOM_COUNTRY,m_byCountry,this);		//加入公会频道
	ChatCenter::GetInstance().RegChatObj(CHAT_ROOM_MAP,m_nMIndex,this);		//加入地图频道
	m_pUserRecord->AddRecord(UA_ENTER,UO_MAP,m_nRealMIndex,m_nMIndex);
	if(m_dwHomeID > 0)
		ChatCenter::GetInstance().RegChatObj(CHAT_ROOM_HOME,m_dwHomeID,this);		//加入公会频道
	m_pFriendList->SendFriendList();
	m_pFriendList->SendFrdOnlineList();
	if (m_dwHomeID)
	{
		HomeObj* pHome = g_pGameCenter->m_pHomeCenter->_getHomeObj(m_dwHomeID);
		if (pHome)
		{
			pHome->AddActiveMember(m_dwUin);
		}
	}

	m_pUserFlag->SyncClient();
	m_pUserFlag->SyncSLimitClient();
	m_pUserFlag->SyncQuestClient();
	m_pUserFlag->SyncCopyLmtClient();

	m_pUserCD->SyncClient();
	if(m_pDegreeList->Count == 0)
		g_pGameCenter->m_pActivityCenter->ResetActiveDegree(this);
	else
		g_pGameCenter->m_pActivityCenter->SendActiveDegree(this,-1);

	SendFastItemID();

// 	m_pTeamData = g_pGameCenter->m_pTeamCenter->GetTeamData(m_dwUin);
// 	if (m_pTeamData)
// 	{
// 		m_pTeamData->OnlineChg(m_pTeamData->GetPosByUin(m_dwUin),true,this);
// 	}
	m_pQuestCenter->SyncClient();
	m_pSkillCenter->SendSyncAllSkill();

	m_pItemCenter->SendToClient();
	m_pVIPCenter->SendWorldMessage(true);

	SendMaxContribute();
	_sendHomeSkill();
	SendFamilyQuest();
	//g_pGameCenter->m_pRankCenter->SendRankList(this);
	SendServerTime();
	SendInviteInfo();
	SendWorldInfo();
	m_pMakeSkillData->SendMakeSkillInfo();
	m_pItemCenter->SendMallItem();
	m_pItemCenter->CheckEquipRank();

	m_pMObjCenter->InitCtrl();

	m_pMObjCenter->ObjEnterMap(m_pMObjCenter->m_pPlayer);

	m_pTitleCenter->ResetObj();

	g_pMarryCenter->RequestTotalInfo(this);
	GetMarryQuest();
	m_pUserMarry->ResetEff();
	m_pUserMarry->ResetObj();

	if(m_pSlaveCenter)
	{
		m_pSlaveCenter->SendSlave2C();
		m_pSlaveCenter->CheckActiveWhenJoin();
	}
	if(m_pHorseCenter)
	{
		m_pHorseCenter->SendHorse2C();
		m_pHorseCenter->SendEquip();
	}
	m_pUserAchievement->SendAll();
	m_pUserAchievement->CheckShow();

	//这边过快进入地图服务器,地图服务器发包的时候客户端还没登陆成功
	m_pMObjCenter->SetCtrl(m_pMObjCenter->m_pPlayer,10);
	g_pGameCenter->m_pMapObjCenter->Join(this,m_nMIndex,m_nRealMIndex);
	for (int i = 0;i<m_pQuestCenter->m_pLQuest->Count;i++)
	{
		UQuestObj*	pUQ	= (UQuestObj*)m_pQuestCenter->m_pLQuest->Items[i];
		if(pUQ->pQBase && pUQ->pQBase->wStateID)
			m_pMObjCenter->AddState(m_pMObjCenter->m_pPlayer,pUQ->pQBase->wStateID,false);
	}
	if (m_pMySimple)
	{
		m_pMySimple->bOnline = true;
		m_pMySimple->dwTotalRmb = m_dwTotalRmb;
		m_pMySimple->nMuliPay	= m_nMuliPay;
		m_pMySimple->nAccountUin = m_dwAccount;
	}
	if (m_bIfEquipState)
	{
		m_bIfEquipState = false;
		RecalcParam();
	}
	if (m_pVIPCenter)
		m_pVIPCenter->CheckAddVipState();

	if (g_pGameCenter->m_dwShutWaitTick)
	{
		int nCloseWaiteSec = (g_pGameCenter->m_dwShutWaitTick - GetTickCount())/1000;
		if (nCloseWaiteSec > 0)
			SendGSCloseWait(nCloseWaiteSec);
	}

	if(m_dwHomeID && g_pGameCenter->m_pWorldFlag->GetFlagInt(1) == m_dwHomeID)
		AddState(5100,false);

	g_pGameCenter->m_pRankCenter->CheckRank(RANK_LEVEL,(void*)m_dwUin,true);
	g_pGameCenter->m_pRankCenter->CheckRank(RANK_HORNOUR,(void*)m_dwUin,true);
	g_pGameCenter->m_pActivityCenter->RequestForInfo(this,0);
	g_pGameCenter->m_pActivityCenter->JoinSendActive(this);
	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set logintime=now() where uin=%d",m_dwUin);
	RecordLoginOut(m_dwUin,0);
}

void UserData::RecvPacket( ZPacket* pPacket )
{
	if(_AccountPacketProcess(pPacket))
		return;
	if(_UserPacketProcess(pPacket))
		return;
	if(_SlaveePacketProcess(pPacket))
		return;
	if(_ChatPacketProcess(pPacket))
		return;
	if(_ActivityPacketProcess(pPacket))
		return;
	if(_RankPacketProcess(pPacket))
		return;
	if(_AwardPacketProcess(pPacket))
		return;
	if(_RelationPacketProcess(pPacket))
		return;
	if(m_pMObjCenter && m_pMObjCenter->PacketProcess(pPacket))
		return;
	if(m_pItemCenter&&m_pItemCenter->ItemPacketProcess(pPacket))
		return;
	if(m_pFriendList && m_pFriendList->RecvPacket(pPacket))
		return;
}

void UserData::AddItemEx(int nType,int nCnt,BYTE byOrigin,BYTE byGoldType,BYTE byItemType)
{
	switch(nType)
	{
	case 333:
		{
			m_xUWealth.AddWealth(UW_BINDRMB,nCnt,byOrigin);
		}
		break;
	case 444:
		{
			m_xUWealth.AddWealth(UW_HONOUR,nCnt);
		}
		break;
	case 555:
		{
			m_xUWealth.AddWealth(UW_EXP,nCnt);
		}
		break;
	case 666:
		{
			m_xUWealth.AddWealth(UW_GOLD,nCnt,1,NULL,byGoldType);
		}
		break;
	case 777:
		{
			m_xUWealth.AddWealth(UW_POW,nCnt);
		}
		break;
	case 888:
		{
			m_xUWealth.AddWealth(UW_RMB,nCnt,byOrigin);
		}
		break;
	case 999:
		{
			m_pSlaveCenter->AddNewSlave(nCnt);
		}
		break;
	default:
		{
			m_pItemCenter->AddItemByID(nType,nCnt,true,byItemType);
		}
		break;
	}
}

void UserData::Update25()
{
	if(m_bEnterGame == false)
		return;
	CMTime	tmNow	= CMTime::GetCurTime();
	if(tmNow.GetTickCount()%600 == 0){
		m_pMakeSkillData->AddPower(1);
		if (GetFortData()&&m_pFortData->m_pWarRankData->nRankPos!=-1)
		{
			int nPos = m_pFortData->m_pWarRankData->nRankPos>500?500:m_pFortData->m_pWarRankData->nRankPos;
			int nScore	= (600-nPos) * (650-nPos)/5000;
			m_pFortData->AddWarScore(nScore);
		}
	}
	m_pItemCenter->UpdateSec();
	m_pOnlineAwardCenter->UpdateSec();
	m_pUserExpCenter->UpdateSec();
	DaylyRun();
	if(tmNow.GetTickCount()%180 == 0)
	{
		g_pGameCenter->m_pRankCenter->CheckRank(RANK_MONEY,(void*)m_dwUin,true);
		g_pGameCenter->m_pRankCenter->CheckRank(RANK_ATTACK,(void*)m_dwUin,true);

		m_dwSCCheckTime	= GetTickCount();	//客户端30秒后发回来, 时间在30秒以内就说明客户端加速了
		TCP_SendCmdToUin(m_dwUin,USER,SC_CHECKTIME);
	}

	if (GetFortData()->CheckHarvest(true))
	{
		TCP_SendCmdToUin(m_dwUin,USER,C_FORT_HARVEST);
	}

	m_xUWealth.CheckSaveDB(m_pDB,m_dwUin,m_dwAccount);
	if(tmNow.GetTickCount()%60==0)
	{
		//装备状态检测
		m_pQuestCenter->UpdateMinuteOrLeave();
		CheckEquipState();
		//V9 V10 加光效
		if (m_pVIPCenter)
			m_pVIPCenter->CheckAddVipState();

		int nOldMail	= m_pMailBox->pListMails->Count;
		g_pGameCenter->m_pSysMailCenter->GetSystemMail(this);
		g_pGameCenter->m_pMailCenter->GetMyMails(this,m_pMailBox);
		if(nOldMail	!= m_pMailBox->pListMails->Count ||  m_pMailBox->m_nMailCount < 30)
			SendMailList();

		bool ifDfWeek = false;
		bool ifDfMounth = false;

		CMTime m_timeRun1 = CMTime(m_dwResetTime);

		if(tmNow.GetTime().tm_mon != m_timeRun1.GetTime().tm_mon)
		{//不周月
			ifDfMounth = true;
		}
		if(tmNow.GetTime().tm_wday != m_timeRun1.GetTime().tm_wday)
		{//不同周
			//ResetDailyData(true,false);
			ifDfWeek = true;
		}
		if(tmNow.GetTime().tm_yday != m_timeRun1.GetTime().tm_yday)
		{//不同天
			ResetDailyData(ifDfWeek,false,ifDfMounth);
		}
		m_dwResetTime	= tmNow.GetCurTickCount();
	}

	P2pAskObj* pPA	= P2pAskCenter::GetInstance().GetMyPAObj(m_dwUin);
	if(pPA && pPA->bStep	== PAS_ASK)
	{
		BuildPacketEx(pSendPacket, FRIEND,S_P2PASK,buf, 256);
		SET_DATA(pSend, FRIEND,S_P2PASK,pSendPacket);
		pSend->frd.dwUinB	= m_dwUin;
		pSend->frd.byLv		= m_xUWealth[UW_LV];
		strcpy(pSend->frd.szName,pPA->szName);
		pSend->bType		= pPA->bType;
		pSend->nIndex		= pPA->nIndex;
		TCP_SendPacketToUin(m_dwUin,pSendPacket);
		pPA->bStep	= PAS_RECV;
	}
	m_pMObjCenter->Update25();

	if(m_pMapCopy && m_nMIndex >= 1000)
	{
		for (int h = 0; h < m_pMapCopy->m_pListHelper->Count; h++)
		{
			MapHelpObj* pHelp	= (MapHelpObj*)m_pMapCopy->m_pListHelper->Items[h];
			if(pHelp->dwHelpID == 0)
			{
				if(pHelp->dwHelpTick && GetTickCount() - pHelp->dwHelpTick > 10000)
				{
					pHelp->dwHelpID		= g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(pHelp->wHelpBase,m_nMIndex,m_pMObjCenter->m_pPlayer->nPosX,m_pMObjCenter->m_pPlayer->nPosY,m_byCountry);
					pHelp->dwHelpTick	= 0;
					if(pHelp->dwHelpID)
						m_pMObjCenter->SetCtrl(pHelp->dwHelpID,MAPOBJ_ENEMY,2);

				}
			}
		}
	}
}

bool UserData::DBInit()
{

	if(_dbFillDataWith() == false)
		return false;
	if(m_dwNextItemID	== 1)
		CreateInit();
	_checkLogDB();
	SetSimpleInfo();
	m_pAmzDManager->DBInit();
	m_pFriendList->DBInit();
	m_pQuestCenter->DBInit();
	m_pSkillCenter->DBInit();

	m_pMailBox->DBInitMBox(m_pDB);

	m_pSlaveCenter->DBInit();
	m_pHorseCenter->DBInit();
	m_pItemCenter->Init();
	return true;
}

bool	UserData::SetSimpleInfo()
{
	m_pMySimple = g_pUserList->GetUser(m_dwUin);
	if(m_pMySimple == NULL)
	{
		UserSimple* pUsp = new UserSimple;
		pUsp->bCountry = m_byCountry;
		pUsp->bJob = m_byJob;
		pUsp->bLv = m_xUWealth[UW_LV];
		pUsp->bSex = m_bySex;
		pUsp->dwUin = m_dwUin;
		pUsp->szName = strdup(m_szNick);
		pUsp->bOnline = true;
		pUsp->dwTotalRmb = m_dwTotalRmb;
		pUsp->nAccountUin = m_dwAccount;
		pUsp->bVipLv		= m_xUWealth[UW_VIPLV];
		g_pUserList->AddUser(pUsp);

		m_pMySimple = pUsp;
	}
	return true;
}

bool UserData::UpdateSimpleInfo()
{
	UserSimple* pUsp = g_pUserList->GetUser(m_dwUin);
	if (pUsp)
	{
		pUsp->bCountry = m_byCountry;
		pUsp->bJob = m_byJob;
		pUsp->bLv = m_xUWealth[UW_LV];
		pUsp->bSex = m_bySex;
		pUsp->dwUin = m_dwUin;
		pUsp->szName = strdup(m_szNick);
		pUsp->bOnline = true;
		pUsp->dwTotalRmb = m_dwTotalRmb;
		pUsp->nAccountUin = m_dwAccount;
	}

	return true;
}

void UserData::DaylyRun()
{
	m_dwDaylyOnline ++;
	if(m_dwDaylyOnline % 300 == 0)
	{
		m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set daylyonline=%d where uin=%d",m_dwDaylyOnline,m_dwUin);
		m_pUserRecord->AddRecord(UA_ENTER,UO_TIME,0,m_dwDaylyOnline/60);
	}
}

bool UserData::_RelationPacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGRELATION)
		return false;
	switch(pPacket->bCmd)
	{
	case C_REQUEST_TEACHERLIST:
		{
			SET_DATA(pRecv,RPGRELATION,C_REQUEST_TEACHERLIST,pPacket);
			g_pMentoringCenter->RequestTeacherList(this,pRecv->nFlag);
		}
		return true;
	case C_MARRY_QUEST_ACCEPT:
		{
			int nFlag = m_pUserFlag->GetFlag(27);
			if(nFlag >= 100)
			{
				SendWarnMsg("今日已做完10次，明天再做吧。");
				return true;
			}
			int nQid = m_pUserFlag->GetFlag(28);
			if(nQid)
			{
				if(m_pQuestCenter->RequestAcceptQuest(nQid) == false)
				{
					SendWarnMsg("接受任务失败");
				}
			}
		}
		return true;
	case C_MARRY_QUEST_COMPLETE:
		{
			int nQid = m_pUserFlag->GetFlag(28);
			if(nQid)
			{
				if(m_pQuestCenter->RequestCompleteQuest(nQid) == false)
				{
					SendWarnMsg("完成任务失败");
				}
			}

		}
		return true;
	case C_MARRY_QUEST_GIVEUP:
		{
			int nQid = m_pUserFlag->GetFlag(28);
			if(nQid)
			{
				if(m_pQuestCenter->RequestGiveUpQuest(nQid) == false)
				{
					SendWarnMsg("放弃任务失败");
				}
			}

		}
		return true;
	case C_REQUEST_INSTRUCT:
		{
			g_pMentoringCenter->RequestInstruct(this);
		}
		return true;
	case C_REQUEST_STUDY:
		{
			SET_DATA(pRecv,RPGRELATION,C_REQUEST_STUDY,pPacket);
			g_pMentoringCenter->RequestStudy(this,pRecv->nUin);
		}
		return true;
	case C_REQUEST_SENDITEM:
		{
			SET_DATA(pRecv,RPGRELATION,C_REQUEST_SENDITEM,pPacket);
			g_pMentoringCenter->RequestSendItem(this,pRecv->nID);
		}
		return true;
	case C_REQUEST_GETITEM:
		{
			SET_DATA(pRecv,RPGRELATION,C_REQUEST_GETITEM,pPacket);
			g_pMentoringCenter->RequestGetItem(this,pRecv->nID);
		}
		return true;
	case C_REQUEST_BREAKRELATION:
		{
			SET_DATA(pRecv,RPGRELATION,C_REQUEST_BREAKRELATION,pPacket);
			g_pMentoringCenter->BreakRelationship(this,pRecv->nID);
		}
		return true;
	case C_SEND_MENTOR_MSG:
		{
			SET_DATA(pRecv,RPGRELATION,C_SEND_MENTOR_MSG,pPacket);
			g_pMentoringCenter->DealWithMsg(this, pRecv->dwStuUin );
		}
		return true;
	case C_REQUEST_DEALWITH_ST:
		{
			SET_DATA(pRecv,RPGRELATION,C_REQUEST_DEALWITH_ST,pPacket);
			g_pMentoringCenter->DealWithRequestP(this,pRecv->nUin , pRecv->ifOk);
		}
		return true;
	case C_REQUEST_DEALWITH:
		{
			SET_DATA(pRecv,RPGRELATION,C_REQUEST_DEALWITH,pPacket);
			g_pMentoringCenter->DealWithRequest(this,pRecv->nUin,pRecv->ifOk);
		}
		return true;
	case C_REQUEST_MARRY_LIST:
		{
			SET_DATA(pRecv,RPGRELATION,C_REQUEST_MARRY_LIST,pPacket);
			g_pMarryCenter->GetRequestList(this,pRecv->nFlag);
		}
		return true;
	case C_REQUEST_MARRY_INSTRUCT:
		{
			if(g_pMarryCenter->RequestInstruct(this))
				g_pMarryCenter->GetRequestList(this,0);
		}
		return true;
	case C_REQUEST_MARRY_BREAKE:
		{
			MarryData* pData = m_pUserMarry->GetRelation();
			if(pData)
				g_pMarryCenter->BreakRelationship(this,pData->m_dwID);
		}
		return true;
	case C_REQUEST_MARRY_SKILL:
		{
			SET_DATA(pRecv,RPGRELATION,C_REQUEST_MARRY_SKILL,pPacket);
			MarryData* pData = m_pUserMarry->GetRelation();
			if(pData)
			{
				g_pMarryCenter->RequestUpSkill(this,pData->m_dwID,pRecv->byPos);
			}
		}
		return true;
	case C_MARRY_SENDANSWER:
		{
			SET_DATA(pRecv,RPGRELATION,C_MARRY_SENDANSWER,pPacket);
			if(pRecv->bRlt)
			{
				if(g_pMarryCenter->GetEngaged(pRecv->nUin,m_dwUin))
					m_pUserRecord->RunLuaMarryFunc(1,pRecv->nUin,0,0);
			}
			else
			{
				char szMsg[128];
				sprintf(szMsg,"%s拒绝了你的求婚",m_szNick);
				UserData* pUser = g_pGameCenter->GetPlayer(pRecv->nUin);
				if(pUser)
					pUser->PopMarryMessage("求婚",szMsg);
			}
		}
		return true;
	case C_ANSWER_GETMARRIED:
		{
			SET_DATA(pRecv,RPGRELATION,C_ANSWER_GETMARRIED,pPacket);
			if(pRecv->bRlt)
			{
				MarryData* pData = m_pUserMarry->GetRelation();
				if(pData)
				{
					pData->m_nMarryCnt ++;
					if(pData->m_nMarryCnt == 2)
					{
						pData->m_nMarryCnt = 0;
						if(pData->m_pMan == m_pMySimple)
							g_pMarryCenter->RequestMarry(this,pData->m_dwID);
						else
						{
							UserData* pUser = g_pGameCenter->GetPlayer(pData->m_pMan->dwUin);
							if(pUser)
								g_pMarryCenter->RequestMarry(pUser,pData->m_dwID);
						}
					}
				}
			}
			else
			{
				MarryData* pData = m_pUserMarry->GetRelation();
				if(pData)
				{
					pData->m_nMarryCnt = 0;
					char szMsg[128];
					sprintf(szMsg,"你似乎还没准备好步入婚姻殿堂");
					PopMarryMessage("结婚",szMsg);
					UserData* pUser = NULL;
					if(pData->m_pMan == m_pMySimple)
						pUser = g_pGameCenter->GetPlayer(pData->m_pWoman->dwUin);
					else
						pUser = g_pGameCenter->GetPlayer(pData->m_pMan->dwUin);
					if(pUser)
					{
						sprintf(szMsg,"你的伴侣还没准备好步入婚姻殿堂");
						pUser->PopMarryMessage("结婚",szMsg);
					}
				}
			}
		}
		return true;
	case C_MARRY_SENDRING:
		{
			SET_DATA(pData,RPGRELATION,C_MARRY_SENDRING,pPacket);
			MarryData* pMarry = m_pUserMarry->GetRelation();
			if(pMarry)
				g_pMarryCenter->SendRing(this,pMarry->m_dwID,pData->byPos);
		}
		return true;
	case C_REQUEST_MARRY_SHOW_TITLE:
		{
			SET_DATA(pData,RPGRELATION,C_REQUEST_MARRY_SHOW_TITLE,pPacket);
			if(m_pUserMarry->m_ifShow != pData->ifShow)
			{
				m_pUserMarry->m_ifShow = pData->ifShow;
				m_pUserMarry->ResetObj();
				m_pDB->ExecuteSQL(false,"update "RPG_USER_TABLE" set showmarrytitle=%d where uin=%d",m_pUserMarry->m_ifShow,m_dwUin);
			}
		}
		return true;
	case C_MARRY_ACTION_EVENT:
		{
			SET_DATA(pData,RPGRELATION,C_MARRY_ACTION_EVENT,pPacket);
			DealWithMarryAction(pData->byAction);
		}
		return true;
	case C_REQUEST_CANCEL_MINSTRUCT:
		{
			if(g_pMarryCenter->ClearRequest(this->m_dwUin))
				g_pMarryCenter->GetRequestList(this,0);
		}
		return true;
	case MARRY_SELT_TEST:
		{
			// 			SET_DATA(pData,RPGRELATION,MARRY_SELT_TEST,pPacket);
		}
		return true;
	}
	return true;
}

void	UserData::DealWithMarryAction(BYTE byAction)
{
	int nUin = 0;
	if(byAction < 20)
	{
		if(m_pTeamData == NULL || m_pTeamData->GetTeamUserCount(false) != 2)
		{
			SendWarnMsg("请带你的意中人前来。");
			return;
		}

		for(int i = 0; i < TEAM_MAXUSER; i ++)
		{
			if(m_pTeamData->m_xUser[i].pUser && m_pTeamData->m_xUser[i].pUser != this)
			{
				nUin = m_pTeamData->m_xUser[i].pUser->m_dwUin;
				break;
			}
		}
	}
	switch(byAction)
	{
	case 0:	//订婚
		{
			if(nUin > 0)
				AskEngage(nUin);
		}
		break;
	case 1:	//结婚
		{
			if(CheckMarryConditions(nUin))
				GetMarried();
		}
		break;
	case 4:	//周年庆
		break;
	case 10://巡游
	case 11:
	case 12:
		{
			MarryData* pData = m_pUserMarry->GetRelation();
			if(pData)
				g_pMarryCenter->StreetMarry(this,pData->m_dwID,byAction-10);
		}
		break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
		{
			MarryData* pData = m_pUserMarry->GetRelation();
			if(pData)
				g_pMarryCenter->RequestAward(this,pData->m_dwID,byAction-19);
		}
		break;
	}
}

bool UserData::_AwardPacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGAWARD)
		return false;
	switch(pPacket->bCmd)
	{
	case C_REQUEST_ONLINEGIFT:
		{
			SET_DATA(pRecv,RPGAWARD,C_REQUEST_ONLINEGIFT,pPacket);
			m_pOnlineAwardCenter->RequestForGift(pRecv->byPos);
		}
		return true;
	case C_REQUEST_RECHARGEGIFT:
		{
			m_pRechargeAwardCenter->RequestGift();
		}
		return true;
	case C_REQUEST_RECHARGEGIFT_EX:
		{
			SET_DATA(pRecv,RPGAWARD,C_REQUEST_RECHARGEGIFT_EX,pPacket);
			m_pRechargeAwardCenter->RequestGiftEx(pRecv->nPos);
		}
		return true;
	case C_REQUEST_LVUPGIFT:
		{
			SET_DATA(pRecv,RPGAWARD,C_REQUEST_LVUPGIFT,pPacket);
			m_pLvupAwardCenter->RequestGift(pRecv->nPos);
		}
		return true;
	case C_REQUEST_LOGINGIFT:
		{
			SET_DATA(pRecv,RPGAWARD,C_REQUEST_LOGINGIFT,pPacket);
			m_pLoginAwardCenter->RequestGift(pRecv->nPos);
		}
		return true;
	case C_REQUEST_MOUNTH_SIGN:
		{
			SET_DATA(pRecv,RPGAWARD,C_REQUEST_MOUNTH_SIGN,pPacket);
			m_pMounthAwardCenter->RequestSign(pRecv->ifToday);
		}
		return true;
	case C_REQUEST_MOUNTH_GIFT:
		{
			SET_DATA(pRecv,RPGAWARD,C_REQUEST_MOUNTH_GIFT,pPacket);
			m_pMounthAwardCenter->RequestForGift(pRecv->nPos);
		}
		return true;
	}
	return true;
}

bool UserData::_RankPacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGRANK)
		return false;
	switch(pPacket->bCmd)
	{
	case C_REQUEST_RANKINFO:
		{
			SET_DATA(pRecv,RPGRANK,C_REQUEST_RANKINFO,pPacket);
			g_pGameCenter->m_pRankCenter->RequestInfo(this,pRecv->dwRankFlag);
			g_pGameCenter->m_pRankCenter->RequestLastDamageInfo(this);
		}
		return true;
	case C_REQUEST_GET_USERINFO:
		{
			SET_DATA(pRecv,RPGRANK,C_REQUEST_GET_USERINFO,pPacket);
			SvRankUserInfo* pInfo = g_pGameCenter->m_pRankCenter->GetUserInfo(pRecv->nUin);
			if(pInfo)
			{
				BuildPacketEx(pSendPacket,RPGRANK,S_SEND_USERINFO,buf,2048);
				SET_DATA(pSendData,RPGRANK,S_SEND_USERINFO,pSendPacket);
				memcpy(&pSendData->xInfo,pInfo,sizeof(RankUserInfo));
				TCP_SendPacketToUin(m_dwUin,pSendPacket);
			}
		}
		return true;
	case C_REQUEST_GET_WATCHDATA:
		{
			SET_DATA(pRecv,RPGRANK,C_REQUEST_GET_WATCHDATA,pPacket);
			RankUWatchInfo* pInfo = g_pGameCenter->m_pRankCenter->GetWatchData(pRecv->nUin,pRecv->nID,pRecv->byType);
			if(pInfo)
			{
				BuildPacketEx(pSendPacket,RPGRANK,S_SEND_WATCHDATA,buf,2048);
				SET_DATA(pSendData,RPGRANK,S_SEND_WATCHDATA,pSendPacket);
				memcpy(&pSendData->xInfo,pInfo,sizeof(RankUWatchInfo));
				TCP_SendPacketToUin(m_dwUin,pSendPacket);
			}
		}
		return true;
	}
	return true;
}

bool UserData::_ActivityPacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGACTIVITY)
		return false;
	switch(pPacket->bCmd)
	{
	case C_REQUEST_START:
		{
			//	g_pGameCenter->m_pWorldTestCenter->RequestStart(this);
		}
		return true;
	case C_REQUEST_ACHIEVEMENT_GIFT:
		{
			SET_DATA(pData,RPGACTIVITY,C_REQUEST_ACHIEVEMENT_GIFT,pPacket);
			m_pUserAchievement->RequestForGift(pData->nPos);
		}
		return true;
	case C_REQUEST_GIVEUP:
		{
			g_pGameCenter->m_pWorldTestCenter->GiveUpTest(this);
		}
		return true;
	case C_ANSWER_QUESTION:
		{
			SET_DATA(pData,RPGACTIVITY,C_ANSWER_QUESTION,pPacket);
			g_pGameCenter->m_pWorldTestCenter->AnswerQuestion(this,pData->byPos);
		}
		return true;
	case C_REQUEST_FOR_GIFT:
		{
			g_pGameCenter->m_pWorldTestCenter->RequestForGift(this);
		}
		return true;
	case C_REQUEST_DEGREEAWARD:
		{
			g_pGameCenter->m_pActivityCenter->RequestDegreeAward(this);
		}
		return true;
	case C_REQUEST_ACINFO:
		{
			SET_DATA(pData,RPGACTIVITY,C_REQUEST_ACINFO,pPacket);
			g_pGameCenter->m_pActivityCenter->RequestForInfo(this,pData->nFlag);
		}
		return true;
	case C_REQUEST_ENTERAC:
		{
			SET_DATA(pData,RPGACTIVITY,C_REQUEST_ENTERAC,pPacket);
			g_pGameCenter->m_pActivityCenter->RequestEnter(this,pData->nID);
		}
		return true;
	case C_REQUEST_TURNTABLE_START:
		{
			g_pGameCenter->m_pActivityCenter->RequestTurntableStart(this);
		}
		return true;
	case C_REQUEST_TURNTABLE_FORTEN:
		{
			g_pGameCenter->m_pActivityCenter->RequestTurntableStartForTen(this);
		}
		return true;
	case C_REQUEST_TURNTABLE_GIFT:
		{
			SET_DATA(pData,RPGACTIVITY,C_REQUEST_TURNTABLE_GIFT,pPacket);
			g_pGameCenter->m_pActivityCenter->RequestTurntableGift(this,pData->nPos);
		}
		return true;
	case C_REQUEST_BOSSINFO:
		{
			g_pGameCenter->m_pActivityCenter->RequestBossState(this);
		}
		return true;
	case C_SEND_JUMPTONPC:
		{
			SET_DATA(pData,RPGACTIVITY,C_SEND_JUMPTONPC,pPacket);
			PathObj* pNpc = NpcPathLeadCfg::GetInstance().GetNpcPath(pData->nID);
			if (pNpc)
			{
				int nToMap	= pNpc->nMapindex;
				if(MapViewConfig::GetInstance().GetMapTileData(nToMap) == NULL)
				{
					if(m_byCountry == 1)
						nToMap	+= 100;
					else
						nToMap	+= 200;
				}
				JumpTo(nToMap,pNpc->nPosX,pNpc->nPosY);
				m_pItemCenter->RemoveItemByID(30009);
			}
		}
		return true;
	case C_OPEN_MAPCOPY_RLT:
		{
			SET_DATA(pData,RPGACTIVITY,C_OPEN_MAPCOPY_RLT,pPacket);
			RequestCreateMapCopy(pData->wMCID);
		}
		return true;
	}
	return true;
}

bool UserData::_ChatPacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGCHAT)
		return false;
	switch(pPacket->bCmd)
	{
	case C_CHAT_MSG:
		{
			if (g_pGameCenter->CheckEverForbid(m_dwUin))
			{
				SendWarnMsg("您已经被永久禁言。");
				return true;
			}
			int nTime = g_pGameCenter->CheckForbid(m_dwUin);
			if (nTime)
			{
				SendWarnMsg("您当前已经被禁言,剩余%d小时%d分钟%d秒",nTime/3600,nTime%3600/60,nTime%60);
				return true;
			}
			SET_DATA(pRec,RPGCHAT,C_CHAT_MSG,pPacket);
			if(!DebugCmd::GetInstance().DealMsg(this, pRec->szMsg,m_byGMLv))
				ChatCenter::GetInstance().AddMsg(m_pChatRoom[pRec->byCRType],this,pRec->nRecvUin,pRec->szMsg,pRec->szID,pRec->nCnt);
		}
		return true;
	case C_REQUEST_ITEMINFO:
		{
			SET_DATA(pRec,RPGCHAT,C_REQUEST_ITEMINFO,pPacket);
			ChatCenter::GetInstance().GetItemInfo(this,pRec->nID);
		}
		return true;
	}
	return true;
}

bool UserData::_SlaveePacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != JGSLAVE)
		return false;
	switch(pPacket->bCmd)
	{
	case C_REQUEST_ACTIVE:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_ACTIVE,pPacket);

			if (MapLimitConfig::GetInstance().CheckMapSlaveLimit(m_nRealMIndex) == true)
			{
				SendWarnMsg("当前地图无法召唤宠物！");
				return true;
			}else
				SlaveOperation::GetInstance().SetActiveSlave(m_pSlaveCenter,pData->dwID);
		}
		return true;
	case C_REQUEST_CHANGENAME:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_CHANGENAME,pPacket);
			SlaveOperation::GetInstance().ChangeSlaveName(m_pSlaveCenter,pData->dwID,pData->szName);
		}
		return true;
	case C_REQUEST_RELEASE:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_RELEASE,pPacket);
			SlaveOperation::GetInstance().ReleaseSlave(m_pSlaveCenter,pData->dwID);
		}
		return true;
	case C_REQUEST_LEARN_SKILL:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_LEARN_SKILL,pPacket);
			SlaveOperation::GetInstance().LearnSlaveSkill(m_pSlaveCenter,pData->dwPetID,pData->nItemID);
		}
		return true;
	case C_REQUEST_REVIVEPET:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_REVIVEPET,pPacket);
			SlaveOperation::GetInstance().ReviveSlave(m_pSlaveCenter,pData->dwID);
		}
		return true;
	case C_REQUEST_STRENGTHEN:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_STRENGTHEN,pPacket);
			SlaveOperation::GetInstance().StrengthenSlave(m_pSlaveCenter,pData->dwID,pData->bySel);
		}
		return true;
	case C_REQUEST_EXCHANGE:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_EXCHANGE,pPacket);
			SlaveOperation::GetInstance().ExchangeSlave(m_pSlaveCenter,pData->dwID);
		}
		return true;
	case C_REQUEST_VARIATION:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_VARIATION,pPacket);
			SlaveOperation::GetInstance().VariatSlave(m_pSlaveCenter,pData->dwID,pData->szID,pData->nCnt);
		}
		return true;
	case C_REQUEST_FEED:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_FEED,pPacket);
			HorseOperation::GetInstance().FeedHorse(m_pHorseCenter,pData->dwID,pData->ifSenior,pData->nCnt);
		}
		return true;
	case C_REQUEST_CULTIVATE:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_CULTIVATE,pPacket);
			if (pData->byType < 2)
				HorseOperation::GetInstance().CultivateHorse(m_pHorseCenter,pData->dwID,pData->byType);
			else if (pData->byType > 1)
				HorseOperation::GetInstance().CultivateHorseEx(m_pHorseCenter,pData->dwID,pData->byType);
		}
		return true;
	case C_REQUEST_CULTIVATE_RLT:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_CULTIVATE_RLT,pPacket);
			HorseOperation::GetInstance().CultivateHorseRlt(m_pHorseCenter,pData->dwID,pData->ifSure);
		}
		return true;
	case C_REQUEST_EAT:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_EAT,pPacket);
			HorseOperation::GetInstance().EatHorse(m_pHorseCenter,pData->dwID,pData->szID,pData->nCnt);
		}
		return true;
	case C_REQUEST_RELEASE_HORSE:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_RELEASE_HORSE,pPacket);
			HorseOperation::GetInstance().ReleaseHorse(m_pHorseCenter,pData->dwID);
		}
		return true;
	case C_REQUEST_RIDE:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_RIDE,pPacket);

			if (pData->ifOn == true && MapLimitConfig::GetInstance().CheckMapHorseLimit(m_nRealMIndex) == true)
			{
				m_pMObjCenter->ReSetMapObjSkin(m_pMObjCenter->m_pPlayer);
				SendWarnMsg("当前地图无法骑乘坐骑！");
				return true;
			}else
				HorseOperation::GetInstance().RideHorse(m_pHorseCenter,pData->ifOn);
		}
		return true;
	case C_REQUEST_EQUIP:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_EQUIP,pPacket);
			HorseOperation::GetInstance().EquipHorse(m_pHorseCenter,pData->dwID);
		}
		return true;
	case C_REQUEST_GET:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_GET,pPacket);
			SlaveOperation::GetInstance().GetSlave(m_pSlaveCenter,pData->nGetType);
		}
		return true;
	case C_REQUEST_QUICKGET:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_QUICKGET,pPacket);
			SlaveOperation::GetInstance().QuickGetSlave(m_pSlaveCenter,pData->nGetType);
		}
		return true;
	case C_REQUEST_REVIVEPET_BYGOLD:
		{
			SET_DATA(pData,JGSLAVE,C_REQUEST_REVIVEPET_BYGOLD,pPacket);
			SlaveOperation::GetInstance().ReviveSlave(m_pSlaveCenter,pData->dwID,false);
		}
		return true;
	}
	return true;
}
bool UserData::_AccountPacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup != ACCOUNT)
		return false;

	switch(pPacket->bCmd)
	{
	case TPCOMM_PAY:
		{
			SET_DATA(pRecv,ACCOUNT,TPCOMM_PAY,pPacket);
			if(pRecv->nRmbVal == 0)
				return true;
			if(m_dwTotalRmb == 0 || m_nMuliPay)
			{
				//if(pRecv->nRealVal > 10000)
				//	m_xUWealth.AddWealth(UW_RMB,pRecv->nRealVal+10000,RMB_SAVE_TPCOMMPAY,pRecv->szOrder);
				//else
				m_xUWealth.AddWealth(UW_RMB,pRecv->nRealVal*2,RMB_SAVE_TPCOMMPAY,pRecv->szOrder);
				if(m_pVIPCenter)
					m_pVIPCenter->AddVIPRMB(pRecv->nRealVal);
				if (m_dwAccCreateTime/86400 == CMTime::GetCurTickCount()/86400)
				{
					g_pGameCenter->SendPValInfo(m_nTPID,2);
				}
				if(m_nMuliPay)
				{
					m_pDB->ExecuteSQL(false,"update "RPG_USER_TABLE" set mulipay=0 where Uin=%d",m_dwUin);
					m_nMuliPay	= 0;
					if(m_pMySimple)
						m_pMySimple->nMuliPay	= 0;
				}
			}
			else{
				m_xUWealth.AddWealth(UW_RMB,pRecv->nRealVal,RMB_SAVE_TPCOMMPAY,pRecv->szOrder);
				if(m_pVIPCenter)
					m_pVIPCenter->AddVIPRMB(pRecv->nRealVal);
			}
			if (m_bIsRecharge)
			{
				g_pGameCenter->SendPValInfo(m_nTPID,4);
				m_bIsRecharge = false;
			}
			g_pGameCenter->SendPValInfo(m_nTPID,5);
			m_dwTotalRmb	+= pRecv->nRealVal;
			//m_pMySimple->dwTotalRmb = m_dwTotalRmb;
			g_pUserList->SetTotalRmb(m_dwAccount,m_dwTotalRmb);
			m_pRechargeAwardCenter->RefreshFlag();
			m_pRechargeAwardCenter->RefreshFlagEx();
			m_pDB->ExecuteSQL(false,"update "RPG_ACCOUNT_TABLE" set rmbtotal=%d,temprmbpay=temprmbpay+%d where AccountUin=%d",m_dwTotalRmb,pRecv->nRealVal,m_dwAccount);
			if (m_dwAccCreateTime/86400 == CMTime::GetCurTickCount()/86400)
			{
				g_pGameCenter->SendPValInfo(m_nTPID,3,pRecv->nRealVal);
			}
			g_pGameCenter->SendPValInfo(m_nTPID,6,pRecv->nRealVal);

		}
		return true;
	case GA_RECV:
		{
			SET_DATA(pRecv,ACCOUNT,GA_RECV,pPacket);
			m_wReachInvNum++;
			SendInviteInfo();
		}
		return true;
	}
	return false;
}

bool UserData::_UserPacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup != USER)
		return false;

	switch(pPacket->bCmd)
	{
	case N_NETGATEOK:
		{
			JoinGame();
		}
		return true;
	case C_REQUEST_VIPTEST:
		{
#ifdef _DEBUG
			m_pVIPCenter->AddVIPRMB(20);
#endif
		}
		return true;
	case C_GET_FORTDATA:
		{
			SendFortData(true);
		}
		return true;
	case C_GET_FORTDATA_NEW:
		{
			SendFortData(false);
		}
		return true;
	case C_ADD_WARRANKTIMES:
		{
			g_pGameCenter->m_pWarRankCenter->AddWarRankTimes(this);
		}
		return true;
	case C_GET_RANK_ENAMY:
		{
			g_pGameCenter->m_pWarRankCenter->RequestWRObjs(this);
		}
		return true;
	case CHECK_GAMESVR_ALIVE:
		{
			TCP_SendCmdToUin(m_dwUin,USER,CHECK_GAMESVR_ALIVE);
		}
		return true;
	case C_REQUEST_VIPGIFT:
		{
			SET_DATA(pData,USER,C_REQUEST_VIPGIFT,pPacket);
			m_pVIPCenter->RequestForGift(pData->nLv);
		}
		return true;
	case C_REQUEST_VIPPOWER:
		{
			m_pVIPCenter->RequestGetPower();
		}
		return true;
	case C_RQ_BACK_CITY:
		{
			if(m_byCountry == 1)
			{
				if(m_nMIndex != 103)
					JumpTo(101,1146,1310);
			}
			else
			{
				if(m_nMIndex != 203)
					JumpTo(201,1146,1310);
			}
		}
		return true;
	case C_GIVEUP_MAKESKILL:
		{
			SET_DATA(pRecv,USER,C_GIVEUP_MAKESKILL,pPacket);
			m_pMakeSkillData->GiveUpMakeSkill((MAKE_SKILL_TYPE )pRecv->nType);
		}
		return true;
	case C_BUY_MAKESKILL_POWER:
		{
			int nCnt = GetFlag(33);
			if (!nCnt||(nCnt <=5&&m_pVIPCenter->GetVipLv()))
			{
			}else{
				SendPopMsg("剩余购买次数不足,普通玩家每日可购买1次，VIP玩家每日可购买6次。");
				return true;;
			}
			if (!m_xUWealth.SubWealth(UW_RMB,20,RMB_SAVE_MAKE_POWER))
			{
				return true;
			}
			AddFlag(33,1);
			m_pMakeSkillData->AddPower(999);
		}
		return true;
	case C_JUMPMAP:
		{
			SET_DATA(pRecv,USER,C_JUMPMAP,pPacket);

			_TransPoint* pTP	= TransPointCfg::GetInstance().GetTransByID(m_nRealMIndex,pRecv->nTransPtID);
			if(pTP && CalcDistance(pTP->nPosX,pTP->nPosY,m_pMObjCenter->m_pPlayer->nPosX,m_pMObjCenter->m_pPlayer->nPosY) < pTP->nSrcRange+500)
			{
				if(pTP->byLvLimit > m_xUWealth[UW_LV])
				{
					SendWarnMsg("需要等级达到 %d 级",pTP->byLvLimit);
					return true;
				}

				int	nDestX	= pTP->nDestX;
				if(pTP->nDestRange)
					nDestX	+= -pTP->nDestRange + rand()%(2*pTP->nDestRange);
				int	nDestY	= pTP->nDestY;
				if(pTP->nDestRange)
					nDestY	+= -pTP->nDestRange + rand()%(2*pTP->nDestRange);
				if(nDestX < 0)
					nDestX	= 0;
				if(nDestY < 0)
					nDestY	= 0;

				int nToMap	= pTP->nToMapIndex;
				if(MapViewConfig::GetInstance().GetMapTileData(pTP->nToMapIndex) == NULL)
				{
					if(m_byCountry == 1)
						nToMap	+= 100;
					else
						nToMap	+= 200;
				}

				JumpTo(nToMap,nDestX,nDestY);
			}
		}
		return true;
	case C_SEND_CHAT_MSG:
		{

		}
		return true;
	case C_AUTO_FIX:
		{
			if(m_pVIPCenter->CheckFixSwitch())
				m_pItemCenter->FixAllUsedEquip();
		}
		return true;
	case C_AUTO_BUYDRUG:
		{
			SET_DATA(pData, USER, C_AUTO_BUYDRUG, pPacket);
			if(m_pVIPCenter->CheckBuySwitch())
				m_pItemCenter->TryBuyShopItem(pData->nType,pData->nItemID,pData->nCount);
		}
		return true;
	case CS_SEND_FAST_ITEM:
		{
			SET_DATA(pData,USER	,CS_SEND_FAST_ITEM,pPacket);
			ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pData->dwItemID);
			if (pBase)
			{
				SetFastItemID(pData->dwItemID);
				SendFastItemID();
			}
		}
		return true;
	case C_USE_FAST_ITEM:
		{
			list<ItemObj*> itemlist;
			m_pItemCenter->GetItemByType(m_nFastItemID,itemlist,1<<ITEM_SLOT_BAG,true);
			m_pItemCenter->GetItemByType(m_nFastItemID,itemlist,1<<ITEM_SLOT_BAG,false);
			if (itemlist.size())
			{
				int nNum = (*itemlist.begin())->GetDataVal(ITEM_AD_NUM);
				m_pItemCenter->UseItem(*itemlist.begin());
				if (nNum == 0)
				{
					SetFastItemID(0);
					SendFastItemID();
				}		
				return true;
			}
		}
		return true;
	case C_DEL_MAIL:
		{
			SET_DATA(pData, USER, C_DEL_MAIL, pPacket);
			m_pMailBox->DeleteMail(pData->dwMailID);
		}
		return true;
	case C_QUEST_ACCEPT:
		{
			SET_DATA(pData, USER, C_QUEST_ACCEPT, pPacket);

			if(m_pQuestCenter->RequestAcceptQuest(pData->wQid) == false)
			{
				//SendWarnMsg("接受任务失败");
			}
			else{
				PlayEffect(77);
			}
		}
		return true;
	case C_QUEST_COMPLETE:
		{
			SET_DATA(pData, USER, C_QUEST_COMPLETE, pPacket);

			if(m_pQuestCenter->RequestCompleteQuest(pData->wQid) == false)
			{
				//SendWarnMsg("完成任务失败");
			}
			else{
				PlayEffect(78);
			}
		}
		return true;
	case C_QUEST_GIVEUP:
		{
			SET_DATA(pData, USER, C_QUEST_GIVEUP, pPacket);

			if(m_pQuestCenter->RequestGiveUpQuest(pData->wQid) == false)
			{
				SendWarnMsg("放弃任务失败");
			}else{
			}
		}
		return true;
	case C_HOME_QUEST_ACCEPT:
		{
			SET_DATA(pData, USER, C_HOME_QUEST_ACCEPT, pPacket);
			if (GetFlag(nFamilyQuestTimesFlag)>=10)
			{
				return true;
			}
			if(m_pQuestCenter->RequestAcceptQuest(pData->wQid) == false)
			{
				SendWarnMsg("接受任务失败");
			}
			TCP_SendCmdToUin(m_dwUin,USER, C_HOME_QUEST_ACCEPT);
		}
		return true;
	case C_HOME_QUEST_COMPLETE:
		{
			SET_DATA(pData, USER, C_HOME_QUEST_COMPLETE, pPacket);
			if(m_pQuestCenter->RequestCompleteQuest(pData->wQid) == false)
			{
				SendWarnMsg("完成任务失败");
			}else{

			}
		}
		return true;
	case CS_TEAM_STATE:
		{
			SET_DATA(pData,USER,CS_TEAM_STATE,pPacket);
			RequestTeamState(pData);
		}
		return true;
	case C_TEAM_INVITE:
		{
			SET_DATA(pData,USER,C_TEAM_INVITE,pPacket);
			InviteUser(pData->dwUin);
		}
		return true;
	case C_JUMP_HOME_WARCOPY:
		{
			JumpHomeWarcopy();
		}
		return true;
	case C_LEAVEMCOPY:
		{
			if(m_nMIndex >= 1000)
			{
				JumpTo(m_wOldMIndex,m_wOldPosX,m_wOldPosY);
			}
		}
		return true;
	case C_MCOPY_TEAM_TIMEUP:
		{
			if(m_pMCopyTeam)
				TeamStartMCopy();
		}
		return true;
	case C_RQ_TEAM_MCOPY:
		{
			SET_DATA(pData,USER,C_RQ_TEAM_MCOPY,pPacket);

			if(m_pMCopyTeam)
				m_pMCopyTeam->pLRqMCopy->Remove((void*)m_dwUin);
			m_pMCopyTeam	= NULL;

			MCopyBase*	pMC	= MCopyConfig::GetInstance().GetMCopy(pData->wMCID);
			if(!pMC)
				return true;
			if(m_pTeamData)
			{
				SendWarnMsg("组队状态下点快速进入队长副本");
				return true;
			}

			if(CheckJoinMCopy(pMC))
			{
				m_pMCopyTeam	= pMC;
				pMC->pLRqMCopy->Add((void*)m_dwUin);

				TCP_SendCmdToUin(m_dwUin,USER, S_RQ_TEAM_MCOPY_RLT);

				if(pMC->pLRqMCopy->Count >= 5)
					TeamStartMCopy();
			}
		}
		return true;
	case C_OVER_GREENMCOPY:
		{
			SET_DATA(pData,USER,C_OVER_GREENMCOPY,pPacket);
			MCopyBase*	pMC	= MCopyConfig::GetInstance().GetMCopy(pData->wMCopyID);
			if(!pMC)
			{
				SendWarnMsg("进入副本失败");
				return true;
			}

			if(pMC->byType == 5)
			{//翡翠梦境,每进去一次都要设置次数
				if(pMC->wID - 1000 > m_pUserFlag->GetFlag(23))
				{//超出进度
					SendWarnMsg("未开放的挑战");
					return true;
				}
				if(CheckJoinMCopy(pMC))
				{
					m_pUserFlag->SetMarkFlag(pMC->wID,101);
					m_xUWealth.AddWealth(UW_GOLD,pMC->nGold,1,NULL,UW_GOLD_DREAM);
					m_xUWealth.AddWealth(UW_EXP,pMC->nExp);
				}
			}
		}
		return true;
	case C_JOINMAPCOPY_SP:
		{
			SET_DATA(pData,USER,C_JOINMAPCOPY_SP,pPacket);

			if(m_pTeamData)
			{
				SendWarnMsg("组队状态不可进入");
				return true;
			}
			MCopyBase*	pMC	= MCopyConfig::GetInstance().GetMCopy(pData->wMCID);
			if(!pMC)
			{
				SendWarnMsg("进入副本失败");
				return true;
			}

			if(CheckJoinMCopy(pMC))
			{
				if(pMC->byType == 5)
				{//翡翠梦境,每进去一次都要设置次数
					if (m_xUWealth[UW_LV] < 36)
						return true;
					if(pMC->wID - 1000 > m_pUserFlag->GetFlag(23)+1)
					{//超出进度
						SendWarnMsg("未开放的挑战");
						return true;
					}

					int nGTime	= m_pUserFlag->GetMarkFlag(pMC->wID);
					if(nGTime < 5)
						m_pUserFlag->AddMarkFlag(pMC->wID);

					if(nGTime > 1 || (nGTime == 1 && m_xUWealth[UW_VIPLV]<3))
					{
						if(m_xUWealth[UW_RMB] < 20)
						{
							SendWarnMsg("钻石不足");
							return true;
						}
						m_xUWealth.SubWealth(UW_RMB,20,RMB_SAVE_GREENMODE);
					}
				}
				m_wOldMIndex	= m_nMIndex;
				m_wOldPosX		= m_pMObjCenter->m_pPlayer->nPosX;
				m_wOldPosY		= m_pMObjCenter->m_pPlayer->nPosY;
				m_pMapCopy	= g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(m_dwUin,m_dwUin,pMC);
				if(m_pMapCopy)
				{
					CMTime	tmNow	= CMTime::GetCurTime();
					m_dwCopyStartTick = tmNow.GetTickCount();

					JumpTo(m_pMapCopy->m_dwMapIndex,m_pMapCopy->m_pMCBase->nStartX,m_pMapCopy->m_pMCBase->nStartY,m_pMapCopy->m_pMCBase->wBaseMap);
				}
			}

		}
		return true;
	case C_JOINMAPCOPY:
		{
			SET_DATA(pData,USER,C_JOINMAPCOPY,pPacket);

			if (pData->wMCID == 410)
			{
				JoinExpeditionCopy();
				return true;
			}

			MCopyBase*	pMC	= NULL;
			if(m_pTeamData && m_pTeamData->GetTeamLeaderUin() != m_dwUin)
			{
				JoinTeamMCopy();
				return true;
			}
			else
				pMC	= MCopyConfig::GetInstance().GetMCopy(pData->wMCID);

			if(!pMC)
			{
				SendWarnMsg("进入副本失败");
				return true;
			}

			if(CheckJoinMCopy(pMC))
			{
				//if(pMC->byLimitTime)
				//	m_pUserFlag->AddMarkFlag(pMC->wID);
				m_wOldMIndex	= m_nMIndex;
				m_wOldPosX		= m_pMObjCenter->m_pPlayer->nPosX;
				m_wOldPosY		= m_pMObjCenter->m_pPlayer->nPosY;
				m_pMapCopy	= g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(m_dwUin,m_dwUin,pMC);
				if(m_pMapCopy)
				{
					CMTime	tmNow	= CMTime::GetCurTime();
					m_dwCopyStartTick = tmNow.GetTickCount();

					JumpTo(m_pMapCopy->m_dwMapIndex,m_pMapCopy->m_pMCBase->nStartX,m_pMapCopy->m_pMCBase->nStartY,m_pMapCopy->m_pMCBase->wBaseMap);
				}

				if(m_pTeamData && m_pTeamData->GetTeamLeaderUin() == m_dwUin)
				{
					BuildPacketEx(pTeamPacket, USER, S_MCOPY_MASTERJOIN, bufteam, 256);
					SET_DATA(pSendTeam, USER, S_MCOPY_MASTERJOIN, pTeamPacket);
					pSendTeam->wMCID	= m_pMapCopy->m_pMCBase->wID;
					for (int i = 0; i < TEAM_MAXUSER; i++)
					{
						if(m_pTeamData->m_xUser[i].dwUin && m_pTeamData->m_xUser[i].nState == TEAM_NORMAL)
						{
							TCP_SendPacketToUin(m_pTeamData->m_xUser[i].dwUin,pTeamPacket);
						}
					}
				}
			}
		}
		return true;
	case C_SKILLCZ:
		{
			if(m_xUWealth[UW_LV] >= 35)
			{
				if(m_xUWealth[UW_RMB] < 30)
					return true;
				m_xUWealth.SubWealth(UW_RMB,30,RMB_SAVE_RESETSKILL);
			}

			m_pSkillCenter->ResetJobSkill();

			m_pMObjCenter->m_pPlayer->ResetSkill();
			m_xUWealth.AddWealth(UW_SKILLPT,(m_xUWealth[UW_LV]-1)*3	- m_xUWealth[UW_SKILLPT]);

			SkillBase* pSBase	= SkillConfig::GetInstance().GetJobSkill(m_byJob,0);
			if(pSBase)
			{
				SkillObj* pSkill	= m_pSkillCenter->SetSkill(pSBase->wSkillID,1);
				pSkill->byPos		= 1;
				m_pSkillCenter->_insertDB(pSkill);

				m_pSkillCenter->RequestSetSCut(pSBase->wSkillID,1);
				m_pSkillCenter->_sendClient(pSkill);
			}
		}
		return true;
	case C_SKILLUP:
		{
			SET_DATA(pData,USER,C_SKILLUP,pPacket);

			if(m_pSkillCenter->RequestSkillUp(pData->wSkill,true))
			{
				PlayEffect(nEffectSuccess);
			}
			else
				SendPopMsg("学习技能失败");
		}
		return true;
	case C_REMOVE_SPSKILL:
		{
			SET_DATA(pData,USER,C_REMOVE_SPSKILL,pPacket);

			m_pSkillCenter->RemoveSpSkill(pData->wSkill);
		}
		return true;
	case C_SET_SSHORTCUT:
		{
			SET_DATA(pData,USER,C_SET_SSHORTCUT,pPacket);
			m_pSkillCenter->RequestSetSCut(pData->wSkill,pData->byPos);
		}
		return true;
	case C_REQUEST_REVIVE:
		{
			SET_DATA(pData,USER,C_REQUEST_REVIVE,pPacket);
			RequestRevive(pData->bItem);
		}
		return true;
	case C_LEARN_HOMESKILL:
		{
			SET_DATA(pData,USER,C_LEARN_HOMESKILL,pPacket);
			AddHomeSkill(pData->byType);
		}
		return true;
	case C_OPENCARD:
		{
			RqOpenMCopyCard();
		}
		return true;
	case C_OPENCARD_EX:
		{
			m_bFirstOpenCard = true;
		}
		return true;
	case C_ACTIVATE:
		{
			SET_DATA(pData,USER,C_ACTIVATE,pPacket);
			if (strlen(pData->sztxt)>=31)
			{
				return true;
			}
			GetActiveItem(pData->sztxt);
		}
		return true;
	case C_RQ_OPENALL:
		{
			SET_DATA(pData,USER,C_RQ_OPENALL,pPacket);
			RqOpenAllMCopyCard(pData->nWRmb,pData->nWCnt);
		}
		return true;
	case C_GETMAIL_ITEM:
		{
			SET_DATA(pData,USER,C_GETMAIL_ITEM,pPacket);
			if(GetMailItem(pData->dwMailID)){
				TCP_SendPacketToUin(m_dwUin,pPacket);
			}
		}
		return true;
	case C_READMAIL:
		{
			SET_DATA(pData,USER,C_READMAIL,pPacket);
			MailObject* pMail = m_pMailBox->GetMail(pData->dwMailID);
			if (pMail&&pMail->byState == STATE_MAIL_SEND)
			{
				pMail->byState = STATE_MAIL_READ;
				pMail->DBUpdateMail(m_pDB);
			}
		}
		return true;
	case C_DELETE_MAIL:
		{
			SET_DATA(pData,USER,C_DELETE_MAIL,pPacket);
			m_pMailBox->DeleteMail(pData->dwMailID);
			TCP_SendPacketToUin(m_dwUin,pPacket);
		}
		return true;
	case C_DELETE_MAIL_ALL:
		{
			SET_DATA(pData,USER,C_DELETE_MAIL_ALL,pPacket);
			for (int i = 0; i < pData->wCnt; i++)
			{
				m_pMailBox->DeleteMail(pData->dwMailID[i]);
			}
			TCP_SendPacketToUin(m_dwUin,pPacket);
		}
		return true;
	case C_SEND_MAIL_INFO:
		{
			SET_DATA(pData,USER,C_SEND_MAIL_INFO,pPacket);
			int dwUin = atoi(pData->szTitle);
			if (!dwUin){
				UserSimple* pUser = g_pUserList->GetUser(pData->szTitle);
				if(pUser) dwUin = pUser->dwUin;
			}else{
				UserSimple* pUser = g_pUserList->GetUser(dwUin);
				if(!pUser) dwUin = 0;
			}
			if (dwUin)
			{
				UserData* pUserData = g_pGameCenter->GetPlayer(dwUin);
				if (pUserData )
				{
					if (pUserData->m_pMailBox->pListMails->Count >= 30)
					{
						SendWarnMsg("对方邮件过多,发送失败！");
						return true;
					}
				}else
				{
					if (g_pGameCenter->m_pMailCenter->GetUserMailCntBy(dwUin) >= 30)
					{
						SendWarnMsg("对方邮件过多,发送失败！");
						return true;
					}
				}
				
				ItemObj* pObj = NULL;
				for (int i=0;i<2;i++)
				{
					if(pData->nItemIndex[i]&&pData->nCnt[i]){
						pObj = m_pItemCenter->GetItemByIndex(pData->nItemIndex[i]);
						if (!pObj || pObj->GetDataVal(ITEM_AD_BIND))
						{
							return true;
						}
						if(pData->nCnt[i] <= 0)
							return true;
						if (pObj->GetDataVal(ITEM_AD_NUM)<pData->nCnt[i]&&pObj->m_pBase->byType != ITEMTYPE_EQUIP)
						{
							return true;
						}
					}
				}
				if (pData->nMoney<0)
				{
					return true;
				}
			
				if (!m_xUWealth.SubWealth(UW_GOLD, (pData->nMoney + m_pMySimple->bLv*100)))
				{
					SendWarnMsg("金币不足,发送邮件失败！");
					return true;
				}
				
				int nIndex[nMaxMailItemCount] = {0};
				for (int i=0;i<2;i++)
				{
					if(pData->nItemIndex[i]&&pData->nCnt[i]){
						pObj = m_pItemCenter->GetItemByIndex(pData->nItemIndex[i]);
						ItemObj* pCut = m_pItemCenter->CutItemByIndex(pObj,pData->nCnt[i]);
						if (!pCut)
						{
							g_pLogFile->OutPut("uin %d send mail failed,money %d, index1 %d,index2 %d,itemindex1 %d,cnt1 %d,itemindex2 %d,cnt2 %d",m_dwUin,pData->nMoney + m_pMySimple->bLv*100,nIndex[0],nIndex[1]
							,pData->nItemIndex[0],pData->nCnt[0],pData->nItemIndex[1],pData->nCnt[1]);
							return true;
						}
						nIndex[i] = g_pGameCenter->m_pItemManage->AddItemObj(pCut,true);

					}
				}
				g_pGameCenter->m_pMailCenter->RequestSendMail(m_dwUin,dwUin,m_szNick,pData->szBody,pData->nMoney,nIndex);
				SendPopMsg("发送邮件成功");
			}else{
				SendPopMsg("未找到玩家");

			}
		}
		return true;
	case C_GET_EQUIPINFO:
		{
			SET_DATA(pData,USER,C_GET_EQUIPINFO,pPacket);
			SendUserEquipInfo(pData->dwUin);
		}
		return true;
	case C_FIGHT_TARGET:
		{
			SET_DATA(pData,USER,C_FIGHT_TARGET,pPacket);
			_fightTarget(pData->dwUin);
		}
		return true;
	case  C_FIGHT_REQUEST:
		{
			SET_DATA(pData,USER,C_FIGHT_REQUEST,pPacket);
			if (pData->byResult)
			{
				_beginFight();
			}else{
				if (m_pFightTarget)
				{
					m_pFightTarget->SendWarnMsg("对方拒绝了您的要求");
				}
				_unlockFightTarget();
			}
		}
		return true;
	case C_REQUEST_NPC_FORM:
		{
			SET_DATA(pData,USER,C_REQUEST_NPC_FORM,pPacket);

			if(g_pGameCenter->m_pListLockNpcCmd->IndexOf((void*)pData->nNpcCmd) != -1)
				return true;

			BuildPacketEx(pSendPacket, USER, S_RESPON_NPC_FORM, buf, 256);
			SET_DATA(pSend,USER,S_RESPON_NPC_FORM,pSendPacket);
			pSend->nNpcCmd	= pData->nNpcCmd;
			TCP_SendPacketToUin(m_dwUin, pSendPacket);
		}
		return true;
	case C_REQUEST_LUCKBET:
		{
			SET_DATA(pData,USER,C_REQUEST_LUCKBET,pPacket);

			BYTE	byRtl	= m_pLuckData->CalcLuckBetRtl(pData->byCoinType,pData->wVal);
			BuildPacketEx(pSendPacket, USER, S_LUCKBET_RTL, buf, 256);
			SET_DATA(pSend, USER, S_LUCKBET_RTL, pSendPacket);
			pSend->nGetVal	= m_pLuckData->m_nAwardCnt;
			pSend->byEndPos	= byRtl;
			TCP_SendPacketToUin(m_dwUin,pSendPacket);
		}
		return true;
	case C_GET_LUCK_AWARD:
		{
			m_pLuckData->GetLuckBetAward();
		}
		return true;
	case C_RQ_LUCKBET_EX:
		{
			SET_DATA(pData,USER,C_RQ_LUCKBET_EX,pPacket);

			BYTE	byRtl	= m_pLuckData->CalcLuckBetRtlEx(pData->bySide);
			BuildPacketEx(pSendPacket, USER, S_LUCKBET_EX_RTL, buf, 256);
			SET_DATA(pSend, USER, S_LUCKBET_EX_RTL, pSendPacket);
			pSend->nGetVal	= m_pLuckData->m_nAwardCnt;
			pSend->byRtlVal	= byRtl;
			TCP_SendPacketToUin(m_dwUin,pSendPacket);
		}
		return true;
	case C_RQ_STAR:
		{
			SET_DATA(pData,USER,C_RQ_STAR,pPacket);
			BYTE	bTargetLv	= m_xUWealth[UW_STARLV]+1;
			if(bTargetLv < 10)
				bTargetLv	= 11;
			if(bTargetLv > m_xUWealth[UW_LV])
				return true;

			if(bTargetLv != pData->bToLv)
				return true;

			StarLvObject*	pStar	= StarSysCfg::GetInstance().GetStarInfo(bTargetLv);
			if(pStar == NULL)
				return true;
			if(pStar->nCostGold && m_xUWealth[UW_GOLD] < pStar->nCostGold)
				return true;

			if(pStar->nCostExp && m_xUWealth.GetExpWealthVal() < pStar->nCostExp)
				return true;

			if(pStar->nCostGold)
				m_xUWealth.SubWealth(UW_GOLD,pStar->nCostGold);
			if(pStar->nCostExp)
				m_xUWealth.SubExpWealth(UW_EXP,pStar->nCostExp);

			if(m_xUWealth[UW_STARLV] < 11)
				m_xUWealth.AddWealth(UW_STARLV,11-m_xUWealth[UW_STARLV]);
			else
				m_xUWealth.AddWealth(UW_STARLV,1);
		}
		return true;
	case C_RQ_LIFE_STAR:
		{
			SET_DATA(pData,USER,C_RQ_LIFE_STAR,pPacket);
			BYTE	bTargetLv	= m_xUWealth[UW_LIFE_STARLV]+1;
			if (bTargetLv > 10)
				return true;
			if(bTargetLv + 90 -1 > m_xUWealth[UW_LV])
				return true;
			if(bTargetLv != pData->bToLv)
				return true;

			StarLvObject*	pStar	= StarSysCfg::GetInstance().GetStarInfo(bTargetLv+1000);
			if(pStar == NULL)
				return true;
			if(pStar->nCostGold && m_xUWealth[UW_GOLD] < pStar->nCostGold)
				return true;
			if(pStar->nCostExp && m_xUWealth.GetExpWealthVal() < pStar->nCostExp)
				return true;

			if(pStar->nCostGold)
				m_xUWealth.SubWealth(UW_GOLD,pStar->nCostGold);
			if(pStar->nCostExp)
				m_xUWealth.SubExpWealth(UW_EXP,pStar->nCostExp);
			m_xUWealth.AddWealth(UW_LIFE_STARLV,1);
		}
		return true;
	case C_NEED_TD_REQUEST:
		{
			SET_DATA(pData,USER,C_NEED_TD_REQUEST,pPacket);
			if(m_pTeamData)
				m_pTeamData->RefreshTKData(pData->byRtlNeed,pData->dwUin,pData->dwItemID);
		}
		return true;
	case C_CHAGE_PETSTATE:
		{
			SET_DATA(pData,USER,C_CHAGE_PETSTATE,pPacket);
			if (m_pMObjCenter->m_pSlave)
			{
				m_pMObjCenter->SetCtrl(m_pMObjCenter->m_pSlave,pData->byPetState);
				m_bySlaveAct = pData->byPetState;
			}
		}
		return true;
	case C_FISH_RLT:
		{
			SET_DATA(pData,USER,C_FISH_RLT,pPacket);

			if (pData->byFish == 1)
			{
				SendKillDrop(22007);
				m_pItemCenter->AddItemByID(22007);
			}else if(pData->byFish == 2)
			{
				SendKillDrop(22005);
				m_pItemCenter->AddItemByID(22005);
			}

			if (pData->byFish == 5)
			{
				if (pData->dwUin)
				{
					UserData * pUser = g_pGameCenter->GetPlayer(pData->dwUin);
					if (pUser)
						pUser->AddState(8111,true);
				}else
					AddState(8111,true);

			}

			
		}
		return true;
	case C_GET_INVAWARS:
		{
			if(m_wReachInvNum != m_wCheckInvNum)
			{
				static int nCheckVal[4]	= {1,3,5,10};
				xnList* pList = GameAwardConfig::GetInstance().m_pInviteList;
				for (int i = 0; i < 4; i++)
				{
					if(m_wCheckInvNum < nCheckVal[i])
					{
						if(m_wReachInvNum>= nCheckVal[i])
						{
							_simpleAward* pAward = (_simpleAward*)pList->Items[i];
							if (pAward)
							{
								AddItemEx(pAward->nID,pAward->nNum,RMB_SAVE_INVITE,UW_GOLD_INVICODE,ITEM_LOG_INVITE);
							}
						}
					}
				}
				m_wCheckInvNum	= m_wReachInvNum;
				m_pDB->ExecuteSQL(FALSE,"update "RPG_INVITE_TABLE" set checknum=%d where uinsvr=%d",m_wCheckInvNum,m_dwUin);
				SendInviteInfo();
			}
		}
		return true;
	case C_REQUEST_FISH:
		{
			if (!m_pMakeSkillData->CheckMakeSkill(MAKE_SKILL_TYPE_FISH))
			{
				if (g_pGameCenter->m_pActivityCenter->CheckBeingActive(1012))
				{
					if (m_nMIndex == 30)
					{
						if (!((m_pMObjCenter->m_pPlayer->nPosX <= 1500 && m_pMObjCenter->m_pPlayer->nPosY >= 1700)||(m_pMObjCenter->m_pPlayer->nPosX >= 1500 && m_pMObjCenter->m_pPlayer->nPosX <= 2400 && m_pMObjCenter->m_pPlayer->nPosY >= 2400)))
						{
							return true;
						}
					}else
						return true;
				}else
					return true;
			}

			AddState(8111,false);
		}
		return true;
	case C_GET_MULTISEC:
		{
			m_pUserExpCenter->m_ifChg = true;
		}
		return true;
	case C_CHG_USER_NAME:
		{
			SET_DATA(pData,USER,C_CHG_USER_NAME,pPacket);
			if (ChangeUserName(pData->szName))
				TCP_SendCmdToUin(m_dwUin,USER, S_CHGNAME_RLT);
		}
		return true;
	case C_GET_APPRS_AWARD:
		{
			if (!m_pUserFlag->GetFlag(53))
			{
				m_pUserFlag->SetFlag(53,1);
				g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,m_dwUin,"系统","评分奖励",0,46058,1,true);
				g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,m_dwUin,"系统","评分奖励",0,45011,5,true);
			}
		}
		return true;
	case C_GET_NOTICE:
		{
			BuildPacketEx(pSendPacket,USER,S_SEND_NOTICE,buf, 4096);
			SET_DATA(pSend, USER, S_SEND_NOTICE, pSendPacket);
			if(g_pGameCenter->m_strNotice.size())
			{
				strcpy(pSend->szMsg,g_pGameCenter->m_strNotice.c_str());
				pSendPacket->wSize	+= strlen(g_pGameCenter->m_strNotice.c_str());
			}
			else
				pSend->szMsg[1]	= 0;

			TCP_SendPacketToUin(m_dwUin,pSendPacket);
		}
		return true;
	case C_FLAPPYBIRD_SCORE:
		{
			SET_DATA(pData,USER,C_FLAPPYBIRD_SCORE,pPacket);
			if (m_pUserFlag->GetFlag(105) < 5)
			{
				AddWealth(UW_EXP, (m_xUWealth[UW_LV]+pData->wScore)*1000);
				m_pUserFlag->AddFlag(105,1);
			}
		}
		return true;
	case C_FLAPPYBIRD_OPEN:
		{
			if ( m_pUserFlag->GetFlag(105) > 0)
			{
				if(!m_xUWealth.SubWealth(UW_GOLD, 200000))
				{
					SendWarnMsg("金币不足");
					return true;
				}
			}
			TCP_SendCmdToUin(m_dwUin,USER, S_FLAPPYBIRD_OPEN);
		}
		return true;

	case SC_CHECKTIME:
		{
			if(m_dwSCCheckTime && GetTickCount() - m_dwSCCheckTime < 28000)
			{//客户端使用了加速软件,踢下线
				m_dwSCCheckTime	= 0;
				g_pGameCenter->KickPlayer(m_dwUin,0);

			}
		}
		return true;
	}
	return true;
}

void UserData::SendUserEquipInfo(DWORD dwUin)
{
	UserData * pUser = g_pGameCenter->GetPlayer(dwUin);

	if (!pUser)
	{
		SendWarnMsg("未找到该在线玩家");
		return;
	}
	BuildPacketEx(pPacket, USER, S_SEND_EQUIPINFO, buf, 2048);
	SET_DATA(pSend, USER, S_SEND_EQUIPINFO, pPacket);

	CheckUserInfo* pInfo = new CheckUserInfo();
	pInfo->dwUin = pUser->m_dwUin;
	pInfo->nLv = pUser->m_xUWealth.GetWealth(UW_LV);
	pInfo->byJob = pUser->m_byJob;
	pInfo->nScore = pUser->m_nScore;
	strcpy(pInfo->szName,pUser->m_szNick);
	pInfo->nSkin[0] = pUser->m_Param.nSkin;
	pInfo->nSkin[1] = pUser->m_Param.nWeapon;
	pInfo->nSkin[2] = pUser->m_Param.nHorse;
	for(int i = 0; i < EQUIP_MAX; i ++)
	{
		if(pUser->m_pItemCenter->m_pEquip[i])
		{
			pUser->m_pItemCenter->m_pEquip[i]->GetByteData(pInfo->pData[i]);
			pInfo->szSize[i] = pUser->m_pItemCenter->m_pEquip[i]->GetByteSize();
		}
		else
		{
			pInfo->szSize[i] = 0;
		}
	}
	memcpy(&pSend->pUserInfo,pInfo,sizeof(CheckUserInfo));
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::SendWarnMsg(const char* szFormat, ... )
{
	BuildPacketEx(pPacket, USER, S_WARN_MSG, buf, 1024);
	SET_DATA(pSend, USER, S_WARN_MSG, pPacket);
	vsprintf(pSend->szMsg, szFormat, (char*) (&szFormat + 1));
	pPacket->wSize	+= strlen(pSend->szMsg);
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::SendMailList()
{
	for (int i = m_pMailBox->m_nMailCount; i < m_pMailBox->pListMails->Count; i++)
	{
		if (m_pMailBox->m_nMailCount >= 30)
		{
			SendPopMsg("邮箱已满,无法接收新邮件,请清除一些邮件");	
			return;
		}
		MailObject* pObj = (MailObject*)m_pMailBox->pListMails->Items[i];
		SendMailList(pObj);
		m_pMailBox->m_nMailCount++;
	}
}

void UserData::SendMailList( MailObject* pObj )
{
	BuildPacketEx(pPacket,USER, S_SEND_MAIL_INFO, buf, 1024);
	SET_DATA(pData, USER, S_SEND_MAIL_INFO, pPacket);
	pData->byState = pObj->byState;
	pData->dwMailID = pObj->dwMailID;
	pData->dwReturnTick	= pObj->dwReturnTick;
	memcpy(pData->nItemID,pObj->nItemID,sizeof(int)*nMaxMailItemCount);
	memcpy(pData->nCnt,pObj->nCnt,sizeof(int)*nMaxMailItemCount);
	memcpy(pData->nItemIndex,pObj->nItemIndex,sizeof(int)*nMaxMailItemCount);
	pData->nMoney	= pObj->nMoney;
	sprintf(pData->szBody ,"%s" ,pObj->szBody);
	sprintf(pData->szTitle,"%s" , pObj->szTitle);
	pData->uinRecver	= pObj->uinRecver;
	pData->uinSender	= pObj->uinSender;
	pPacket->wSize	+= strlen(pData->szBody);
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

std::string UserData::_getAmzDataByte( int nType,int nID )
{
	AmzDataBase* pData = m_pAmzDManager->GetAmzData(nType,nID);
	std::string szVal;
	if (!pData)
	{
		return "";
	}
	char* szName = NULL;
	switch(nType){

	}
	char szTemp[1024];
	int nLenght = 0;
	szTemp[0] = 2;
	nLenght++;
	memcpy(szTemp+nLenght,szName,32);
	nLenght+=32;
	*((int*)szTemp[nLenght]) = nType;
	nLenght+=4;
	pData->GetByteData(szTemp+nLenght);
	nLenght+=pData->GetByteSize();
	szTemp[nLenght] = 3;
	nLenght++;
	szVal.assign(szTemp,nLenght);
	return szVal;
}

int Random_Int( int min, int max )
{
	if(min == max)
		return min;
	if (max-min == 0)
	{
		return min;
	}
	return min+ rand()%(max-min<0?max-min-1:max-min+1);
}

int UserData::LockADataID()
{
	int	nRtl	= m_dwNextItemID;

	m_dwNextItemID++;
	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set nextitemid=%d where uin=%d",m_dwNextItemID,m_dwUin);
	return nRtl;
}

LONGLONG UserData::GetADKey( int nMID )
{
	LONGLONG temp = m_dwUin;
	return ((temp<< 32)| nMID);
}

void UserData::ResetDailyData(bool bWeek,bool bLogin,bool ifMounth)
{
	m_pUserFlag->CheckResetDaily(bWeek,!bLogin);
	m_pFriendList->ResetRsVal();
	m_xUWealth.AddWealth(UW_POW,50);
	if(m_xUWealth[UW_GSLAVELMT])
		m_xUWealth.SubWealth(UW_GSLAVELMT,m_xUWealth[UW_GSLAVELMT]);
	g_pGameCenter->m_pActivityCenter->ResetActiveDegree(this);
	//保险一点离线时间再隔天后直接存掉
	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set LogoutTime = now() where uin=%d",m_dwUin);
	m_dwDaylyOnline = 0;
	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set daylyonline=%d where uin=%d",m_dwDaylyOnline,m_dwUin);
	if(ifMounth)
	{
		m_pMounthAwardCenter->ResetFlag();
	}
	m_pMounthAwardCenter->SendInfo();
	if (!m_pFortData)
	{
		m_pFortData = g_pGameCenter->m_pFortCenter->GetFortData(m_dwUin,false);
	}
	if (m_pFortData)
	{
		m_pFortData->ResetDailyData();
		if (m_pFortData->m_pWarRankData->nRankPos!=-1&&m_pFortData->m_pWarRankData->nRankPos<m_nMaxRankPos-1)
		{
			SetMaxRankPos(m_pFortData->m_pWarRankData->nRankPos+1);
		}
	}

	m_pOnlineAwardCenter->_initOnlineSec();
	m_pRechargeAwardCenter->ReSetDailyFlag();
	CheckSendLoginDays(true);

	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set ResetTime=now() where uin=%d",m_dwUin);
}

void UserData::SendPopMsg(const char* sztxt,... )
{
	char msg[1024];
	va_list argptr;
	va_start(argptr,sztxt);
	vsprintf(msg, sztxt, argptr);
	BuildPacketEx(pPacket,USER, S_SEND_POP_MSG, buf, 2048);
	SET_DATA(pData, USER, S_SEND_POP_MSG, pPacket);
	pPacket->wSize += strlen(msg);
	strcpy(pData->szMsg,msg);
	TCP_SendPacketToUin(m_dwUin,pPacket);
	va_end(argptr);
}

void UserData::SendMsg(const char* sztxt,...)
{
	char msg[1024];
	va_list argptr;
	va_start(argptr,sztxt);
	vsprintf(msg, sztxt, argptr);
	BuildPacketEx(pPacket,RPGCHAT, S_SEND_SIMPLEMSG, buf, 2048);
	SET_DATA(pData, RPGCHAT, S_SEND_SIMPLEMSG, pPacket);
	pPacket->wSize += strlen(msg);
	strcpy(pData->szMsg,msg);
	TCP_SendPacketToUin(m_dwUin,pPacket);
	va_end(argptr);
}

void UserData::SendRoomMsg(BYTE byRoom,const char * sztxt,...)
{
	if(byRoom >= CHAT_ROOM_MAX || m_pChatRoom[byRoom] == NULL)
		return;
	char msg[1024];
	va_list argptr;
	va_start(argptr,sztxt);
	vsprintf(msg, sztxt, argptr);
	ChatCenter::GetInstance().SendRoomMsg(msg,m_pChatRoom[byRoom]);
	va_end(argptr);
}

bool UserData::AddHorse(int nType)
{
	return m_pHorseCenter->AddNewHorse(nType);
}

bool UserData::AddPet(int nType)
{
	return m_pSlaveCenter->AddNewSlave(nType);
}

void UserData::CreateScriptClass( CLuaState * pLuaState )
{
	CLuaClass<UserData> * pLuaUser = pLuaState->CreateScriptClass((UserData*)0);

	pLuaUser->DefObjectFunction("GetItemCenter", &UserData::GetItemCenter)
		.DefObjectFunction("GetPlayerAchievementVal", &UserData::GetPlayerAchievementVal)
		.DefObjectFunction("GetAchievementVal", &UserData::GetAchievementVal)
		.DefObjectFunction("GetPetCountByQuality", &UserData::GetPetCountByQuality)
		.DefObjectFunction("GetHorseCountByQuality", &UserData::GetHorseCountByQuality)
		.DefObjectFunction("DoneAchievement", &UserData::DoneAchievement)
		.DefObjectFunction("AddAchievementVal", &UserData::AddAchievementVal)
		.DefObjectFunction("SetAchievementVal", &UserData::SetAchievementVal)
		.DefObjectFunction("DonePlayerAchievement", &UserData::DonePlayerAchievement)
		.DefObjectFunction("AddPlayerAchievementVal", &UserData::AddPlayerAchievementVal)
		.DefObjectFunction("SetPlayerAchievementVal", &UserData::SetPlayerAchievementVal)
		.DefObjectFunction("GetPlayerFlag", &UserData::GetFlag)
		.DefObjectFunction("SendPopMsgForScrpit", &UserData::SendPopMsgForScrpit)
		.DefObjectFunction("SetPlayerFlag", &UserData::SetFlag)
		.DefObjectFunction("AddPlayerFlag", &UserData::AddFlag)
		.DefObjectFunction("GetManorLv", &UserData::GetManorLv)
		.DefObjectFunction("AddManorExp", &UserData::AddManorExp)
		.DefObjectFunction("AddWealth", &UserData::AddWealth)
		.DefObjectFunction("GetWealth", &UserData::GetWealth)
		.DefObjectFunction("CheckQuest", &UserData::CheckQuest)
		.DefObjectFunction("RequestAcceptQuest", &UserData::RequestAcceptQuest)
		.DefObjectFunction("RequestCompleteQuest", &UserData::RequestCompleteQuest)
		.DefObjectFunction("RequestGiveUpQuest", &UserData::RequestGiveUpQuest)
		.DefObjectFunction("AddState", &UserData::AddState)
		.DefObjectFunction("CheckState", &UserData::CheckState)
		.DefObjectFunction("LearnSkill", &UserData::LearnSkill)
		.DefObjectFunction("AddHorse", &UserData::AddHorse)
		.DefObjectFunction("AddPet", &UserData::AddPet)
		.DefObjectFunction("GetName", &UserData::GetName)
		.DefObjectFunction("GetCountry", &UserData::GetCountry)
		.DefObjectFunction("GetJob", &UserData::GetJob)
		.DefObjectFunction("GetSex", &UserData::GetSex)
		.DefObjectFunction("GetPosX", &UserData::GetPosX)
		.DefObjectFunction("GetPosY", &UserData::GetPosY)
		.DefObjectFunction("AddHonour", &UserData::AddHonour)
		.DefObjectFunction("AddExpObj", &UserData::AddExpObj)
		.DefObjectFunction("RemoveExpObj", &UserData::RemoveExpObj)
		.DefObjectFunction("GetMapServerObj", &UserData::GetMapServerObj)
		.DefObjectFunction("GetUin", &UserData::GetUin)
		.DefObjectFunction("JumpTo", &UserData::JumpTo)
		.DefObjectFunction("GetFamily", &UserData::GetFamily)
		.DefObjectFunction("GetFamilyName", &UserData::GetFamilyName)
		.DefObjectFunction("PlayEffect", &UserData::PlayEffect)
		.DefObjectFunction("SetMaxRankPos", &UserData::SetMaxRankPos)
		.DefObjectFunction("ResetDailyData", &UserData::ResetDailyData)
		.DefObjectFunction("CheckCompleteQuest", &UserData::CheckCompleteQuest)
		.DefObjectFunction("GetTeamUser", &UserData::GetTeamUser)
		.DefObjectFunction("LearnMakeSkill", &UserData::LearnMakeSkill)
		.DefObjectFunction("OnFamilyQuestComplete", &UserData::OnFamilyQuestComplete)
		.DefObjectFunction("ChangeUserCountry", &UserData::ChangeUserCountry)
		.DefObjectFunction("SetWealth", &UserData::SetWealth);



	xnExtstr * pStr = xnExtstr::Import("data\\lua_script\\userscript.ini");
	if(pStr)
	{
		if(pStr->SetSection("common"))
		{
			char * szTemp = 0;
			int nIndex = 0;
			char szKey[64] = {0};
			while(1)
			{
				sprintf(szKey, "file%d", nIndex++);
				szTemp = pStr->ReadString(szKey);
				if(!szTemp) break;

				FILE* file	= fopen(szTemp,"rb+");
				if(file == NULL)
					continue;
				fseek(file, 0, SEEK_END);
				DWORD dwFLen = ftell(file);
				fseek(file,0,SEEK_SET);
				char * szBuff = (char*)malloc(dwFLen);
				fread(szBuff,dwFLen,1, file);
				pLuaState->GetLuaState()->DoBuffer(szBuff, dwFLen, szTemp);
				free(szBuff);
				fclose(file);
			}

			szTemp = pStr->ReadString("clicknpc_event");
			if(szTemp) pLuaUser->DefScriptFunction(0, LUAEVENT_CLICKNPC, szTemp);
			szTemp = pStr->ReadString("jumpmap_event");
			if(szTemp) pLuaUser->DefScriptFunction(0, LUAEVENT_JUMPMAP, szTemp);
		}
		pStr->Free();
	}
}

void UserData::ReleaseScriptClass( CLuaState * pLuaState )
{
	pLuaState->ReleaseScriptClass((UserData*)0);
}

void UserData::NoticeLoginOk()
{
	BuildPacketEx(pPacket,ACCOUNT, S_JOINGAME, buf, 1024);
	SET_DATA(pData, ACCOUNT, S_JOINGAME, pPacket);
	pData->dwUin	= m_dwUin;
	pData->wHomeID	= m_dwHomeID;
	pData->byHomeDuty = m_dwHomeDuty;
	pData->byVipLvl	= m_pLuckData->m_dwVipLv;
	pData->wMapSvrIndex	= m_nRealMIndex;

	strcpy(pData->szNick,m_szNick);
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::JumpTo( int nMIndex, int x, int y, int nBaseM )
{
	try
	{
		if(nBaseM == 0)
			nBaseM	= nMIndex;

		if(nMIndex == m_nMIndex)
		{//同地图
			//m_pMObjCenter->OffCtrl();
			//m_pMObjCenter->ObjLeaveMap(m_pMObjCenter->m_pPlayer);
			//m_pMObjCenter->ObjLeaveMap(m_pMObjCenter->m_pSlave);
			if(m_pMObjCenter->m_pPlayer->IsDead())
			{
				m_pMObjCenter->HealthHpMp(m_pMObjCenter->m_pPlayer,m_pMObjCenter->m_pPlayer->xParam.nHpMax,m_pMObjCenter->m_pPlayer->xParam.nMpMax,true);
			}
			m_pMObjCenter->m_pPlayer->nPosX		= x;
			m_pMObjCenter->m_pPlayer->nPosY		= y;
			m_pMObjCenter->m_dwLockPos		= GetTickCount();
			m_pMObjCenter->ObjChgPos(m_pMObjCenter->m_pPlayer);
			//m_pMObjCenter->ObjEnterMap(m_pMObjCenter->m_pPlayer);
			//m_pMObjCenter->SetCtrl(m_pMObjCenter->m_pPlayer,10);
			if(m_pMObjCenter->m_pSlave)
			{
				m_pMObjCenter->m_pSlave->nPosX = m_pMObjCenter->m_pPlayer->nPosX;
				m_pMObjCenter->m_pSlave->nPosY = m_pMObjCenter->m_pPlayer->nPosY;	
				m_pMObjCenter->ObjChgPos(m_pMObjCenter->m_pSlave);
				//m_pMObjCenter->ObjEnterMap(m_pMObjCenter->m_pSlave);
				//m_pMObjCenter->SetCtrl(m_pMObjCenter->m_pSlave,1);
			}

			if (nMIndex == 59)
			{//国战地图 复活点复活 加无敌状态
				AddState(1402,false);
			}
		}
		else
		{//跨地图
			if(MapViewConfig::GetInstance().GetMapTileData(nBaseM) == NULL)
			{
				OutMessage(msg_type_normal,"User %d Change Map Fail NOMAP:  %d(%d) --> %d(%d)",m_dwUin,m_nMIndex,m_nRealMIndex,nMIndex,nBaseM);
				return;
			}
			if(m_pMCopyTeam)
				m_pMCopyTeam->pLRqMCopy->Remove((void*)m_dwUin);
			m_pMCopyTeam	= NULL;
			if(m_nMIndex >= 1000 && m_nMIndex != nMIndex)
			{
				if(g_pGameCenter->m_pMapCopyCenter->RequestLeaveMCopy(m_dwUin,m_nMIndex))
					m_pMapCopy	= NULL;
			}

			int nPreIndex = m_nMIndex;
			bool bCallSlave = false;
			if (m_nRealMIndex == 27 ||m_nRealMIndex == 66)
			{
				if (m_pWarRankFight)
				{
					g_pGameCenter->m_pWarRankCenter->OnBattleResult(this,false);
				}
				bCallSlave = true;

			}
			if (m_nRealMIndex == 50)
			{
				if (m_pFightTarget)
				{
					_onFightResult(false);
				}
			}

			bool bFillHp = false;
			if (m_nRealMIndex > 50 && m_nRealMIndex < 57)
			{//翡翠梦境出来加回满血状态
				bFillHp = true;
			}

			if(!g_pGameCenter->m_pMapObjCenter->CheckLeave(this,nPreIndex)){
				OutMessage(msg_type_normal,"User %d Change Map Fail Leave:  %d(%d) --> %d(%d)",m_dwUin,m_nMIndex,m_nRealMIndex,nMIndex,nBaseM);
				return;
			}
			if(!g_pGameCenter->m_pMapObjCenter->CheckJoin(this,nMIndex,nBaseM)){
				OutMessage(msg_type_normal,"User %d Change Map Fail Join:  %d(%d) --> %d(%d)",m_dwUin,m_nMIndex,m_nRealMIndex,nMIndex,nBaseM);
				return;
			}
			m_pUserRecord->AddRecord(UA_LEAVE,UO_MAP,m_nRealMIndex,m_nMIndex);

			m_pMObjCenter->ObjLeaveMap(m_pMObjCenter->m_pSlave);
			m_pMObjCenter->ObjLeaveMap(m_pMObjCenter->m_pPlayer);
			m_pMObjCenter->OffCtrl();

			OutMessage(msg_type_normal,"User %d Change Map:  %d(%d) --> %d(%d)",m_dwUin,m_nMIndex,m_nRealMIndex,nMIndex,nBaseM);

			m_nMIndex	= nMIndex;
			m_nRealMIndex	= nBaseM;
			m_pMObjCenter->SetMapIndex(nMIndex,nBaseM);
			m_pMObjCenter->m_pPlayer->nPosX		= x;
			m_pMObjCenter->m_pPlayer->nPosY		= y;
			m_pMObjCenter->ObjEnterMap(m_pMObjCenter->m_pPlayer);

			if(m_nMIndex <1000)
				m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set mapindex=%d,mapx=%d,mapy=%d where uin=%d",m_nMIndex,m_pMObjCenter->m_pPlayer->nPosX,m_pMObjCenter->m_pPlayer->nPosY,m_dwUin);


			m_pMObjCenter->SetCtrl(m_pMObjCenter->m_pPlayer,10);

			bool ifFull = false;
			if(m_pSlaveCenter->m_dwReviveTick > 0)
			{
				m_pSlaveCenter->m_dwReviveTick = 0;
				ifFull = true;
			}
			if(m_pSlaveCenter->m_pActiveSlave)
			{
				if(ifFull)
					m_pSlaveCenter->m_pActiveSlave->SetHp(m_pSlaveCenter->m_pActiveSlave->m_nParam[SLAVE_PARAM_HPMAX]);
				m_pSlaveCenter->SetMapObj();
			}

			m_pQuestCenter->ResetMapObj();
			//地图频道切换
			ChatCenter::GetInstance().UnRegChatObj(m_pChatRoom[CHAT_ROOM_MAP],this);
			ChatCenter::GetInstance().RegChatObj(CHAT_ROOM_MAP,m_nMIndex,this);	
			m_pUserRecord->AddRecord(UA_ENTER,UO_MAP,m_nRealMIndex,m_nMIndex);

			//安全需要这边来回才执行换服
			BuildPacketEx(pPacket,ACCOUNT, S_SET_MAPSERVER, buf, 1024);
			SET_DATA(pData, ACCOUNT, S_SET_MAPSERVER, pPacket);
			pData->wMapIndex	= m_nRealMIndex;
			TCP_SendPacketToUin(m_dwUin,pPacket);

			g_pGameCenter->m_pMapObjCenter->Leave(this,nPreIndex);
			g_pGameCenter->m_pMapObjCenter->Join(this,nMIndex,m_nRealMIndex);


			if (bCallSlave&&m_pSlaveCenter->m_pLastActiveSlave)
			{
				SlaveOperation::GetInstance().SetActiveSlave(m_pSlaveCenter,m_pSlaveCenter->m_pLastActiveSlave->m_dwID);
				m_pSlaveCenter->m_pLastActiveSlave = NULL;
			}

			if (bFillHp)
			{
				AddState(6203,false);
			}

			bool  bMapHorseLmt = MapLimitConfig::GetInstance().CheckMapHorseLimit(m_nRealMIndex);
			bool  bMapSlaveLmt = MapLimitConfig::GetInstance().CheckMapSlaveLimit(m_nRealMIndex);

			if (nBaseM == 66 || bMapSlaveLmt == true)
				m_pSlaveCenter->CallBackSlave();

			if (bMapHorseLmt == true)
				HorseOperation::GetInstance().RideHorse(m_pHorseCenter,false);
		}
	}
	catch(...){
		g_pLogFile->OutPut("%d:jump error",m_dwUin);
	}
}

bool UserData::CheckStoneState()
{
	int nStoneState = m_pItemCenter->GetStoneState();
	if (nStoneState)
		return CheckState(nStoneState);
	else
		return false;
}

bool UserData::CheckStarState()
{
	int nStarState = m_pItemCenter->GetStarState();
	if (nStarState)
		return CheckState(nStarState);
	else
		return false;
}

void UserData::CheckEquipState()
{
	if (!CheckStarState())
	{
		int nStarState = m_pItemCenter->GetStarState();
		if (nStarState)
		{
			m_nStarState = nStarState;
			AddState(nStarState,false);
		}
		else if (m_nStarState)
		{
			AddState(m_nStarState,true);
			m_nStarState = 0;
		}
	}
	if (!CheckStoneState())
	{
		int nStoneState = m_pItemCenter->GetStoneState();
		if (nStoneState)
		{
			m_nStoneState = nStoneState;
			AddState(nStoneState,false);
		}else if (m_nStoneState)
		{
			AddState(m_nStoneState,true);
			m_nStoneState = 0;
		}
	}
}

void UserData::RecalcParam()
{
	memset(&m_Param,0,sizeof(UserParam));
	map<int,int> mapGroupCount;
	int nLv = m_xUWealth.GetWealth(UW_LV)-1;
	if (nLv<0)
	{
		nLv = 0;
	}
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		m_Param.nParam[i] += UserConfig::GetInstance().nParam[nLv][m_byJob-1][i];
	}
	for (int i=0;i<EQUIP_MAX;i++)
	{
		if (m_pItemCenter->m_pEquip[i])
		{
			for (int j=0;j<EQUIP_ATTR_MAX;j++)
			{
				m_Param.nParam[j] += m_pItemCenter->m_pEquip[i]->m_nParam[j];
			}
			if (m_pItemCenter->m_pEquip[i]->m_pBase->nGroup)
			{
				mapGroupCount[m_pItemCenter->m_pEquip[i]->m_pBase->nGroup]++;
			}
		}
	}
	m_Param.nSpeed = 180;
	//装备状态检测
	CheckEquipState();
	//坐骑属性
	m_Param.nHorse = 0;
	if(m_pHorseCenter->m_pEquipHorse)
	{
		HorseBasic* pHBasic = HorseBaseCfg::GetInstance().GetBasic(m_pHorseCenter->m_pEquipHorse->m_nType);
		if(pHBasic)
		{
			m_Param.nParam[EQUIP_ATTR_HP] += m_pHorseCenter->m_pEquipHorse->m_nParam[SLAVE_PARAM_HPMAX];
			m_Param.nParam[EQUIP_ATTR_MP] += m_pHorseCenter->m_pEquipHorse->m_nParam[SLAVE_PARAM_MPMAX];
			m_Param.nParam[EQUIP_ATTR_ATK_W] += m_pHorseCenter->m_pEquipHorse->m_nParam[SLAVE_PARAM_AD];
			m_Param.nParam[EQUIP_ATTR_DEF_W] += m_pHorseCenter->m_pEquipHorse->m_nParam[SLAVE_PARAM_DD];
			m_Param.nParam[EQUIP_ATTR_ATK_N] += m_pHorseCenter->m_pEquipHorse->m_nParam[SLAVE_PARAM_AP];
			m_Param.nParam[EQUIP_ATTR_DEF_N] += m_pHorseCenter->m_pEquipHorse->m_nParam[SLAVE_PARAM_DP];
			if(m_pHorseCenter->m_ifRide)
			{
				int nHSK = pHBasic->nSkin;
				if(m_pHorseCenter->m_pEquipHorse->m_byLevel >= 70)
					nHSK = pHBasic->szUpSkin[2];
				else if(m_pHorseCenter->m_pEquipHorse->m_byLevel >= 50)
					nHSK = pHBasic->szUpSkin[1];
				else if(m_pHorseCenter->m_pEquipHorse->m_byLevel >= 30)
					nHSK = pHBasic->szUpSkin[0];
				m_Param.nHorse = nHSK;
				m_Param.nSpeed = pHBasic->nSpeed;
			}
		}
	}
	//变异过的宠物属性
	if(m_pSlaveCenter->m_pActiveSlave)
	{
		SlaveBaseData* pData = m_pSlaveCenter->m_pActiveSlave;
		if(pData->m_nHP > 0 && pData->m_byVariatLv > 0)
		{
			SlaveVariatBasic* pVa = SlaveVariatCfg::GetInstance().GetBase(pData->m_byVariatLv);
			if(pVa)
			{
				m_Param.nParam[EQUIP_ATTR_HP] += pData->m_nParam[SLAVE_PARAM_HPMAX]*pVa->nVal/10000;
				m_Param.nParam[EQUIP_ATTR_MP] += pData->m_nParam[SLAVE_PARAM_MPMAX]*pVa->nVal/10000;
				m_Param.nParam[EQUIP_ATTR_ATK_W] += pData->m_nParam[SLAVE_PARAM_AD]*pVa->nVal/10000;
				m_Param.nParam[EQUIP_ATTR_DEF_W] += pData->m_nParam[SLAVE_PARAM_DD]*pVa->nVal/10000;
				m_Param.nParam[EQUIP_ATTR_ATK_N] += pData->m_nParam[SLAVE_PARAM_AP]*pVa->nVal/10000;
				m_Param.nParam[EQUIP_ATTR_DEF_N] += pData->m_nParam[SLAVE_PARAM_DP]*pVa->nVal/10000;
			}
		}
	}
	if(m_pUserMarry)
	{
		MarryData* pMarryData = m_pUserMarry->GetRelation();
		if(pMarryData)
		{
			NumberData* pNum = NULL;
			if(pMarryData->m_pMan == m_pMySimple)
			{
				pNum = pMarryData->m_pManSkill;
			}
			else if(pMarryData->m_pWoman == m_pMySimple)
			{
				pNum = pMarryData->m_pWomanSkill;
			}
			if(pNum)
			{
				for(int i = 0; i < 5; i ++)
				{
					int nLv = pNum->GetVal(i);
					if(nLv > 0)
					{
						MarrySKObj* pRlt = (MarrySKObj*)MarrySkillCfg::GetInstance().m_pList->Items[i];
						if(nLv <= pRlt->pLvList->Count)
						{
							_simpleAward* pRltC = (_simpleAward*)pRlt->pLvList->Items[nLv-1];
							switch(pRlt->byType)
							{
							case 0:
								{
									m_Param.nParam[EQUIP_ATTR_HP] +=pRltC->nNum;
								}
								break;
							case 1:
								{
									m_Param.nParam[EQUIP_ATTR_ATK_W] +=pRltC->nNum;
								}
								break;
							case 2:
								{
									m_Param.nParam[EQUIP_ATTR_DEF_W] +=pRltC->nNum;
								}
								break;
							case 3:
								{
									m_Param.nParam[EQUIP_ATTR_ATK_N] +=pRltC->nNum;
								}
								break;
							case 4:
								{
									m_Param.nParam[EQUIP_ATTR_DEF_N] +=pRltC->nNum;
								}
								break;
							}
						}
					}
				}
			}
		}
	}
	// 	for(int i = 0; i < m_pSlaveCenter->m_pSlaveList->Count; i ++)
	// 	{
	// 		SlaveBaseData* pData = (SlaveBaseData*)m_pSlaveCenter->m_pSlaveList->Items[i];
	// 		if(pData->m_nHP > 0 && pData->m_byVariatLv > 0)
	// 		{
	// 			SlaveVariatBasic* pVa = SlaveVariatCfg::GetInstance().GetBase(pData->m_byVariatLv);
	// 			if(pVa)
	// 			{
	// 				m_Param.nParam[EQUIP_ATTR_HP] += pData->m_nParam[SLAVE_PARAM_HPMAX]*pVa->nVal/10000;
	// 				m_Param.nParam[EQUIP_ATTR_MP] += pData->m_nParam[SLAVE_PARAM_MPMAX]*pVa->nVal/10000;
	// 				m_Param.nParam[EQUIP_ATTR_ATK_W] += pData->m_nParam[SLAVE_PARAM_AD]*pVa->nVal/10000;
	// 				m_Param.nParam[EQUIP_ATTR_DEF_W] += pData->m_nParam[SLAVE_PARAM_DD]*pVa->nVal/10000;
	// 				m_Param.nParam[EQUIP_ATTR_ATK_N] += pData->m_nParam[SLAVE_PARAM_AP]*pVa->nVal/10000;
	// 				m_Param.nParam[EQUIP_ATTR_DEF_N] += pData->m_nParam[SLAVE_PARAM_DP]*pVa->nVal/10000;
	// 			}
	// 		}
	// 	}
	m_Param.nSkin = (m_pItemCenter->m_pEquip[EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_CLOTH)]&&m_pItemCenter->m_pEquip[EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_CLOTH)]->m_pBase->nSkin>1000)?(m_pItemCenter->m_pEquip[EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_CLOTH)]->m_pBase->nSkin+(m_bySex?0:1)):(999+m_byJob*2+(m_bySex?0:1));
	if (m_pItemCenter->m_pEquip[EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN)])
	{
		m_Param.nSkin = (m_pItemCenter->m_pEquip[EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN)]->m_pBase->nSkin>1000)?(m_pItemCenter->m_pEquip[EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN)]->m_pBase->nSkin):(999+m_byJob*2+(m_bySex?0:1));
	}
	if ((m_pItemCenter->m_pEquip[EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN)]&&m_pItemCenter->m_pEquip[EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN)]->m_pBase->nSkin>1000))
	{
		m_Param.nWeapon = 0;
	}else{
		m_Param.nWeapon = (m_pItemCenter->m_pEquip[EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_WEAPON)]&&m_pItemCenter->m_pEquip[EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_WEAPON)]->m_pBase->nSkin>1000)?(m_pItemCenter->m_pEquip[EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_WEAPON)]->m_pBase->nSkin+(m_bySex?0:1)):(2999+m_byJob*2+(m_bySex?0:1));
	}
	StarLvObject*	pStar	= StarSysCfg::GetInstance().GetStarInfo(m_xUWealth[UW_STARLV]);
	if(pStar)
	{
		BYTE	bJob	= m_byJob -1;
		if(bJob >= MAXJOB)
			bJob	= 0;
		m_Param.nParam[EQUIP_ATTR_HP] += pStar->xParamTotal[bJob].nHpMax;
		m_Param.nParam[EQUIP_ATTR_MP] += pStar->xParamTotal[bJob].nMpMax;
		m_Param.nParam[EQUIP_ATTR_ATK_W] += pStar->xParamTotal[bJob].nAtkW;
		m_Param.nParam[EQUIP_ATTR_DEF_W] += pStar->xParamTotal[bJob].nDefW;
		m_Param.nParam[EQUIP_ATTR_ATK_N] += pStar->xParamTotal[bJob].nAtkN;
		m_Param.nParam[EQUIP_ATTR_DEF_N] += pStar->xParamTotal[bJob].nDefN;
		m_Param.nParam[EQUIP_ATTR_HIT] += pStar->xParamTotal[bJob].nAim;
		m_Param.nParam[EQUIP_ATTR_MISS] += pStar->xParamTotal[bJob].nDodge;
		m_Param.nParam[EQUIP_ATTR_CRIT] += pStar->xParamTotal[bJob].nCrit;
	}

	m_Param.nParam[EQUIP_ATTR_HP] = m_Param.nParam[EQUIP_ATTR_HP]*(100+m_xHomeSkill.bySkillLv[USER_HOME_SKILL_HP])/100;
	m_Param.nParam[EQUIP_ATTR_MP] = m_Param.nParam[EQUIP_ATTR_MP]*(100+m_xHomeSkill.bySkillLv[USER_HOME_SKILL_MP])/100;
	m_Param.nParam[EQUIP_ATTR_HIT] = m_Param.nParam[EQUIP_ATTR_HIT]*(100+m_xHomeSkill.bySkillLv[USER_HOME_SKILL_HIT])/100;
	m_Param.nParam[EQUIP_ATTR_MISS] = m_Param.nParam[EQUIP_ATTR_MISS]*(100+m_xHomeSkill.bySkillLv[USER_HOME_SKILL_MISS])/100;
	m_Param.nParam[EQUIP_ATTR_CRIT] = m_Param.nParam[EQUIP_ATTR_CRIT]*(100+m_xHomeSkill.bySkillLv[USER_HOME_SKILL_CRIT])/100;

	//称号属性
	if(m_pTitleCenter && m_pTitleCenter->m_pObj)
	{
		m_Param.nParam[EQUIP_ATTR_HP] += m_Param.nParam[EQUIP_ATTR_HP]*m_pTitleCenter->m_pObj->nHpAdd/10000;
		m_Param.nParam[EQUIP_ATTR_MP] += m_Param.nParam[EQUIP_ATTR_MP]*m_pTitleCenter->m_pObj->nMpAdd/10000;
		m_Param.nParam[EQUIP_ATTR_ATK_W] += m_Param.nParam[EQUIP_ATTR_ATK_W]*m_pTitleCenter->m_pObj->nAdAdd/10000;
		m_Param.nParam[EQUIP_ATTR_DEF_W] += m_Param.nParam[EQUIP_ATTR_DEF_W]*m_pTitleCenter->m_pObj->nDdAdd/10000;
		m_Param.nParam[EQUIP_ATTR_ATK_N] += m_Param.nParam[EQUIP_ATTR_ATK_N]*m_pTitleCenter->m_pObj->nApAdd/10000;
		m_Param.nParam[EQUIP_ATTR_DEF_N] += m_Param.nParam[EQUIP_ATTR_DEF_N]*m_pTitleCenter->m_pObj->nDpAdd/10000;
	}

	//日月PVP减伤属性
	m_Param.nPvpJSDef = GetLifeStarLvPvpDef();

	m_bSendUserParam = true;
	//套装属性
	//set<int> setTempState,setAddState,setRemoveState;
	//for (map<int,int>::iterator itor = mapGroupCount.begin();itor!=mapGroupCount.end();itor++)
	//{
	//	if (itor->second>=2)
	//	{
	//		EquipGroupInfo* pGroupInfo = EquipGroupConfig::GetInstance().GetItem(itor->first);
	//		if (pGroupInfo)
	//		{
	//			for (list<EquipGroupVal>::iterator itor1 = pGroupInfo->groupVal.begin();itor1!=pGroupInfo->groupVal.end();itor1++)
	//			{
	//				if(itor->second<itor1->byCount){
	//					break;
	//				}else{
	//					setTempState.insert(itor1->nState);
	//				}
	//			}
	//		}
	//	}
	//}
	//set_difference(setTempState.begin(),setTempState.end(),m_setGroupState.begin(),m_setGroupState.end(),inserter(setAddState,setAddState.begin()));
	//set_difference(m_setGroupState.begin(),m_setGroupState.end(),setTempState.begin(),setTempState.end(),inserter(setRemoveState,setRemoveState.begin()));

	//m_setGroupState = setTempState;
	//for (set<int>::iterator itor = setAddState.begin();itor!=setAddState.end();itor++)
	//{
	//	AddState((*itor),false);
	//}
	//for (set<int>::iterator itor = setRemoveState.begin();itor!=setRemoveState.end();itor++)
	//{
	//	AddState((*itor),true);
	//}
	OnValChange();
}

void	UserData::SendLuaEvent(int nV1,int nV2,int nV3,int nV4,int nV5)
{
	BuildPacketEx(pPacket,USER,S_SEND_LUA_EVENT,buf,256);
	SET_DATA(pSend,USER,S_SEND_LUA_EVENT,pPacket);
	pSend->szValue[0] = nV1;
	pSend->szValue[1] = nV2;
	pSend->szValue[2] = nV3;
	pSend->szValue[3] = nV4;
	pSend->szValue[4] = nV5;
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::CalcGroupEquipState()
{
	map<int,int> mapGroupCount;
	for (int i=0;i<EQUIP_MAX;i++)
	{
		if (m_pItemCenter->m_pEquip[i]&&m_pItemCenter->m_pEquip[i]->m_pBase->nGroup)
		{
			mapGroupCount[m_pItemCenter->m_pEquip[i]->m_pBase->nGroup]++;
		}
	}

	set<int> setTempState,setAddState,setRemoveState;
	for (map<int,int>::iterator itor = mapGroupCount.begin();itor!=mapGroupCount.end();itor++)
	{
		if (itor->second>=2)
		{
			EquipGroupInfo* pGroupInfo = EquipGroupConfig::GetInstance().GetItem(itor->first);
			if (pGroupInfo)
			{
				for (list<EquipGroupVal>::iterator itor1 = pGroupInfo->groupVal.begin();itor1!=pGroupInfo->groupVal.end();itor1++)
				{
					if(itor->second<itor1->byCount){
						break;
					}else{
						setTempState.insert(itor1->nState);
					}
				}
			}
		}
	}
	set_difference(setTempState.begin(),setTempState.end(),m_setGroupState.begin(),m_setGroupState.end(),inserter(setAddState,setAddState.begin()));
	set_difference(m_setGroupState.begin(),m_setGroupState.end(),setTempState.begin(),setTempState.end(),inserter(setRemoveState,setRemoveState.begin()));

	m_setGroupState = setTempState;
	//for (set<int>::iterator itor = setAddState.begin();itor!=setAddState.end();itor++)
	//{
	//	AddState((*itor),false);
	//}
	for (set<int>::iterator itor = m_setGroupState.begin();itor!=m_setGroupState.end();itor++)
	{
		if (CheckState((*itor)) == false)
			AddState((*itor),false);
	}
	for (set<int>::iterator itor = setRemoveState.begin();itor!=setRemoveState.end();itor++)
	{
		AddState((*itor),true);
	}
}

void UserData::_sendUserParam()
{
	BuildPacketEx(pPacket, USER, S_SEND_USER_PARAM, buf, 1024);
	SET_DATA(pSend, USER, S_SEND_USER_PARAM, pPacket);
	pSend->m_Param = m_Param;
	TCP_SendPacketToUin(m_dwUin,pPacket);

}

void UserData::_runLuaClickNpc( int nNpcID )
{
	if(CLuaClassObject<UserData>::IsFunctionExisted(LUAEVENT_CLICKNPC))
	{
		if(!CLuaClassObject<UserData>::RunFunction<void>(LUAEVENT_CLICKNPC, 0, nNpcID))
		{
			//OutDumpMessage("PlayerData _OnSetMapEventEvent Exception! Id: %d, MapIndex: %d\n", m_dwUin, mapIndex);
		}
	}
}

void UserData::_runLuaJumpMap( int nFM, int nTM )
{
	if(CLuaClassObject<UserData>::IsFunctionExisted(LUAEVENT_JUMPMAP))
	{
		if(!CLuaClassObject<UserData>::RunFunction<void>(LUAEVENT_JUMPMAP, 0, nFM, nTM))
		{
			//OutDumpMessage("PlayerData _OnSetMapEventEvent Exception! Id: %d, MapIndex: %d\n", m_dwUin, mapIndex);
		}
	}
}

int UserData::GetFlag( WORD wID )
{
	return m_pUserFlag->GetFlag(wID);
}

int UserData::SetFlag( WORD wID, int nVal )
{
	return m_pUserFlag->SetFlag(wID,nVal);
}

int UserData::AddFlag( WORD wID, int nVal )
{
	return m_pUserFlag->AddFlag(wID,nVal);
}

void UserData::SendPopMsgForScrpit(const char* sztxt )
{
	SendWarnMsg(sztxt);
}

void UserData::ChgHp( int nVal )
{
	m_pMObjCenter->HealthHpMp(m_pMObjCenter->m_pPlayer,nVal,0);

}

void UserData::ChgMp( int nVal )
{
	m_pMObjCenter->HealthHpMp(m_pMObjCenter->m_pPlayer,0,nVal);
}

int UserData::GetPosX()
{
	return m_pMObjCenter->m_pPlayer->nPosX;
}

int UserData::GetPosY()
{
	return m_pMObjCenter->m_pPlayer->nPosY;
}

void UserData::KillPlayerEvent(DWORD dwID)
{
	UserData* pTargent = g_pGameCenter->GetPlayer(dwID);
	if(pTargent == NULL)
		return;
	if (m_nRealMIndex != 50)
	{
		char sztxt[64];
		sprintf(sztxt,"%s杀死了%s",this->m_szNick,pTargent->m_szNick);
		ChatCenter::GetInstance().SendSystemMsg(sztxt,7,0,1,true);
	}
	if((this->m_byCountry == 1 && pTargent->m_byCountry == 2) || (this->m_byCountry == 2 && pTargent->m_byCountry == 1))	//敌对国家,击杀获得荣誉
	{
		pTargent->m_pTitleCenter->BeKilled();
		m_pTitleCenter->KillEnemy(pTargent->m_pTitleCenter->m_pObj);
	}
}

void UserData::SendKillDrop(WORD wID)
{
	BuildPacketEx(pPacket, USER, S_SEND_KILLDROP, buf, 1024);
	SET_DATA(pSend, USER, S_SEND_KILLDROP, pPacket);
	pSend->nWID = wID;
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

bool UserData::CheckWBossKill(WORD wEnemy)
{
	int nCount = ACBossCfg::GetInstance().m_pList->Count;
	for (int i = 0; i < nCount; i++)
	{
		BossInfoObj* pTemp = (BossInfoObj*)ACBossCfg::GetInstance().m_pList->Items[i];
		if (pTemp->nID == wEnemy)
			return true;
	}
	return false;
}

BossInfoObj* UserData::GetWBossInfo(WORD  wEnemy)
{
	int nCount = ACBossCfg::GetInstance().m_pList->Count;
	for (int i = 0; i < nCount; i++)
	{
		BossInfoObj* pTemp = (BossInfoObj*)ACBossCfg::GetInstance().m_pList->Items[i];
		if (pTemp->nID == wEnemy)
			return pTemp;
	}
	return NULL;
}
int nHomeRsc[] = {30,35,40,60,80,105,135,170};
int nHomeCtb[] = {75,88,113,150,200,263,338,425};
int nHomeMemberExp[] = {1000,1500,2000,2500,3000,3500,4000,4500};
char* szCountry[] = {"无","联盟","部落"};
void UserData::KillEnemyEvent( WORD wEnemyID,DWORD	dwID )
{

	if (wEnemyID == 1520)
	{//甜蜜活动蛋糕怪
		if (m_pUserFlag->GetFlag(62) > 19)
			return;
		m_pUserFlag->AddFlag(62,1);
	}

	if (CheckExpeditionCopy(wEnemyID))
	{//远征副本 另外算奖励
		return;
	}

	if(CheckMCopyOk(wEnemyID))
	{
		//副本完成
		g_pMarkGameDB->AddQICnt(m_pMapCopy->m_pMCBase->wID,2,1);
		if (m_pMapCopy->m_pMCBase->wID == 404)//军团副本击杀通告
		{
			int nLvRank = (m_pUserFlag->GetFlag(24)/10-2)>0?(m_pUserFlag->GetFlag(24)/10-2):0;

			if (m_pTeamData)
			{
				for (int i=0;i<TEAM_MAXUSER;i++)
				{
					if (m_pTeamData->m_xUser[i].dwUin&&m_pTeamData->m_xUser[i].bOnline&&m_pTeamData->m_xUser[i].pUser)
					{
						m_pTeamData->m_xUser[i].pUser->AddContribute(nHomeCtb[nLvRank]);
						m_pTeamData->m_xUser[i].pUser->m_xUWealth.AddWealth(UW_EXP,nHomeMemberExp[nLvRank]*m_pTeamData->m_xUser[i].pUser->m_xUWealth.GetWealth(UW_LV));
					}
				}
			}

			g_pGameCenter->m_pHomeCenter->AddHomeExp(m_dwHomeID,nHomeRsc[nLvRank]);

			char sztxt[128];
			sprintf(sztxt,"%s成功击杀了军团副本BOSS[巫妖]，军团资源增加%d，个人贡献增加%d", m_szNick,nHomeRsc[nLvRank],nHomeCtb[nLvRank]);
			ChatCenter::GetInstance().SendSystemMsg(sztxt,CHAT_ROOM_HOME,m_dwHomeID,1,false);
		}
	}
	bool ifWBoss = false;
	if(CheckWBossKill(wEnemyID))
	{
		//世界boss击杀
		g_pMarkGameDB->AddQICnt(wEnemyID,3,1);
		BossInfoObj* pBossInfo = GetWBossInfo(wEnemyID);
		MapTileData* pMap = MapViewConfig::GetInstance().GetMapTileData(pBossInfo->nMap);

		char szMsg[128];
		sprintf(szMsg,"[%s]的[%s]成功击杀了[%s]的BOSS[%s]！",szCountry[m_byCountry],m_szNick,pMap->m_szName,pBossInfo->pName);
		ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);
		ifWBoss = true;
	}
	//任务系统
	int nGold	= 0;
	if(m_pTeamData&&m_pTeamData->GetTeamUserCount(false)>1)
	{
		nGold	= m_pTeamData->TeamKillEnemy(wEnemyID,m_nMIndex,m_dwUin);

		while(m_pTeamData->m_pItemList->Count > 0)
		{
			int nItemID = (int) m_pTeamData->m_pItemList->Delete(0);
			ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(nItemID);
			if (pItem&&pItem->nStar < 2)
			{
				SendKillDrop(pItem->nTypeID);
				m_pItemCenter->SetEnemyDrop(true);
				m_pItemCenter->AddItemByID(pItem->nTypeID,1,0,ITEM_LOG_KILLENEMY);
				m_pItemCenter->SetEnemyDrop(false);
			}else if (pItem)
			{
				m_pTeamData->CheckTKDropNeed(pItem->nTypeID,m_nMIndex);
			}
		}
		// 		for (int i = 0; i < m_pTeamData->m_pItemList->Count; i++)
		// 		{
		// 			ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase((int) m_pTeamData->m_pItemList->Items[i]);
		// 			if (pItem&&pItem->nStar < 2)
		// 			{
		// 				SendKillDrop(pItem->nTypeID);
		// 				m_pItemCenter->AddItemByID(pItem->nTypeID);
		// 			}else if (pItem)
		// 			{
		// 				m_pTeamData->CheckTKDropNeed(pItem->nTypeID);
		// 			}
		// 		}
	}
	else
	{
		m_pQuestCenter->KillEnemy(wEnemyID);

		_EnemyAward* pEA	= EnemyKillConfig::GetInstance().GetEAward(wEnemyID);
		if(pEA)
		{
			nGold	= pEA->nGold;

			xnList*	pLItems	= xnList::Create();
			pEA->RandGetItem(pLItems);
			for (int i = 0; i < pLItems->Count; i++)
			{
				SendKillDrop((int)pLItems->Items[i]);
				m_pItemCenter->SetEnemyDrop(true);
				m_pItemCenter->AddItemByID((int)pLItems->Items[i],1,0,ITEM_LOG_KILLENEMY);
				m_pItemCenter->SetEnemyDrop(false);
			}
			m_xUWealth.AddWealth(UW_EXP,m_pUserExpCenter->GetExp(pEA->CalcExp(m_xUWealth.GetWealth(UW_LV)),0));
			if(m_pSlaveCenter->m_pActiveSlave)
			{
				int nPetExp = pEA->CalcExp(m_pSlaveCenter->m_pActiveSlave->m_byLevel);
				m_pSlaveCenter->m_pActiveSlave->AddExp(m_pUserExpCenter->GetExp(nPetExp,1));
			}
			pLItems->Free();
		}
	}
	//钱是杀死的人拿的
	if(nGold)
		m_xUWealth.AddWealth(UW_GOLD,nGold,1,NULL,UW_GOLD_KILL_ENEMY);
	if (wEnemyID >= 8002&&wEnemyID <= 8005)
	{
		g_pGameCenter->m_pWarRankCenter->OnBattleResult(this,true);
	}
	_runLuaFunctionOnPlayerKillEnemy(this,wEnemyID,dwID,szLuaFunctionOnPlayerKill);
	m_pUserRecord->RunLuaKillEnemyFunc(wEnemyID,ifWBoss);
}

void UserData::CheckEnemyExAward(WORD wEnemyID)
{
	int nLv = m_xUWealth.GetWealth(UW_LV);
	if (wEnemyID == 2101)
	{
		m_xUWealth.AddWealth(UW_EXP,nLv*nLv*50);
		AddHonour(80);
	}

	if (wEnemyID == 2102)
	{
		m_xUWealth.AddWealth(UW_EXP,nLv*nLv*80);
		if (m_dwHomeID)
			AddContribute(nLv);
	}

	if ((wEnemyID == 2103 || wEnemyID == 6509) && m_pUserMarry)
	{
		m_xUWealth.AddWealth(UW_EXP,nLv*nLv*100);
		m_xUWealth.AddWealth(UW_GOLD,nLv*nLv*10);
		MarryData* pMarry = m_pUserMarry->GetRelation();
		if(pMarry)
		{
			g_pMarryCenter->AddMarryValue(this,pMarry->m_dwID,nLv*3,m_bySex);
		}
	}
	if (wEnemyID == 6512)
	{
		if (g_pMentoringCenter->CheckTeacherUser(m_dwUin))
		{
			m_xUWealth.AddWealth(UW_EXP,nLv*nLv*100);
			AddWealth(UW_HONOUR,40);
			m_pMySimple->nMentoringVal += 50;
			m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set mentoringval=%d where uin=%d",m_pMySimple->nMentoringVal,m_dwUin);
			char szMsg[128];
			sprintf(szMsg,"您获得%d经验，%d荣誉，%d师德声望",nLv*nLv*100,40,50);
			SendWarnMsg(szMsg);
			if (m_pUserMentoring)
				m_pUserMentoring->SendHonour();
		}else
		{
			m_xUWealth.AddWealth(UW_EXP,nLv*nLv*300);
			m_xUWealth.AddWealth(UW_GOLD,nLv*600);
		}
	}

}

void UserData::InviteUser( DWORD dwUin )
{
	if (dwUin == m_dwUin)
	{
		return;
	}
	UserData* pTarget = g_pGameCenter->GetPlayer(dwUin);
	if (!pTarget)
	{
		SendWarnMsg("对方不在线");
		return;
	}
	if (pTarget->m_byCountry != m_byCountry)
	{
		SendWarnMsg("不同势力不能组队");
		return;
	}
	if(m_pMCopyTeam || pTarget->m_pMCopyTeam)
	{
		SendWarnMsg("申请副本组队时不可手动组队");
		return;
	}

	if(m_nMIndex >= 1000)
	{
		SendWarnMsg("副本不可组队");
		return;
	}

	if(pTarget->m_nMIndex >= 1000)
	{
		SendWarnMsg("对方正在副本不可组队");
		return;
	}
	if (!m_pTeamData)
	{
		if(!g_pGameCenter->m_pTeamCenter->CreateTeam(this)){
			return;
		}
		ChatCenter::GetInstance().RegChatObj(CHAT_ROOM_TEAM,m_pTeamData->nTeamID,this);
	}
	// 	if (m_pTeamData->GetTeamLeaderUin() != m_dwUin)
	// 	{
	// 		SendWarnMsg("你不是队长");
	// 		return;
	// 	}
	if (pTarget->m_pTeamData)
	{
		SendWarnMsg("对方已有队伍");
		return;
	}
	if (m_pTeamData->GetTeamUserCount(false)>=TEAM_MAXUSER)
	{
		SendWarnMsg("队伍人数已满");
		return;
	}
	SendWarnMsg("已发送邀请");
	pTarget->SendTeamState(TEAM_COMMIT_INVITE,m_szNick);
	m_pTeamData->Join(pTarget,TEAM_CREATE);
}

void UserData::SendTeamState( BYTE nType ,char* szMsg)
{
	BuildPacketEx(pPacket, USER, CS_TEAM_STATE, buf, 1024);
	SET_DATA(pSend, USER, CS_TEAM_STATE, pPacket);
	pSend->byType = nType;
	if (szMsg)
	{
		int nSize = strlen(szMsg)+1;
		pPacket->wSize += nSize;
		memcpy(pSend->szMsg,szMsg,nSize);
	}else{
		pSend->szMsg[0] = 0;
	}
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::RequestTeamState( USER_CS_TEAM_STATE_Data* pRevi )
{
	if (!m_pTeamData)
	{
		return;
	}
	switch(pRevi->byType){
	case TEAM_COMMIT_ACCEPT:
		{
			TeamUser* pData = m_pTeamData->GetTeamUser(m_dwUin);
			if (pData)
			{
				m_pTeamData->Join(this);
				ChatCenter::GetInstance().RegChatObj(CHAT_ROOM_TEAM,m_pTeamData->nTeamID,this);

			}
		}
		break;
	case TEAM_COMMIT_REFUSE:
		{
			m_pTeamData->Leave(this);
		}
		break;
	case TEAM_COMMIT_LEAVE:
		{
			m_pTeamData->Leave(this);
		}
		break;
	case TEAM_COMMIT_CHGLEADER:
		{
			TeamUser* pData = m_pTeamData->GetTeamUser(m_dwUin);
			if (pData&&pData->nState == TEAM_LEADER)
			{
				if (!m_pTeamData->m_xUser[pRevi->szMsg[0]].bOnline)
				{
					for (int i = 0; i< TEAM_MAXUSER; i++)
					{
						if (m_pTeamData->m_xUser[i].dwUin == m_dwUin)
						{
							m_pTeamData->m_xUser[i].pUser->SendWarnMsg("该队员已掉线");
						}
					}
				}else
					m_pTeamData->ChangeLeader(pRevi->szMsg[0]);
			}
		}
		break;
	case TEAM_COMMIT_KICK:
		{
			TeamUser* pData = m_pTeamData->GetTeamUser(m_dwUin);
			if (pData&&pData->nState == TEAM_LEADER)
			{
				m_pTeamData->Leave((BYTE)pRevi->szMsg[0]);

			}
		}
		break;
	}
}

void UserData::SetWealth(BYTE bEnum, int nVal)
{
	m_xUWealth.SetWealth(bEnum,nVal);
}

void UserData::AddWealth( BYTE bEnum, int nVal )
{
	m_xUWealth.AddWealth(bEnum,nVal);
}

void UserData::AddHonour(int nVal)
{
	m_pTitleCenter->AddHonour(nVal);
}

bool UserData::AddExpObj(int nID,int nTime)
{
	return m_pUserExpCenter->AddObj(nID,nTime);
}

bool UserData::RemoveExpObj(int nID)
{
	return m_pUserExpCenter->RemoveObj(nID);
}

void UserData::PlayEffect( int nEffID )
{
	BuildPacketEx(pPacket, USER, S_PLAYEFFECT, buf, 1024);
	SET_DATA(pSend, USER, S_PLAYEFFECT, pPacket);
	pSend->nEffect = nEffID;
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::LeaveTeam()
{
	if(m_pTeamData) m_pTeamData->Leave(this);
}

void UserData::OnValChange()
{
	if(m_pTeamData) m_pTeamData->OnValChange(this);
	static int nAttrScore[EQUIP_ATTR_MAX] = {4,6,12,12,15,15,20,30,40};
	int nPreScore = m_nScore;
	m_nScore = 0;
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		m_nScore += m_Param.nParam[i]*nAttrScore[i];
	}
	switch(m_byJob)
	{
	case 0:
	case 1:
		m_nScore/=30.5;
		break;
	case 2:
		m_nScore/=29.2;
		break;
	case 3:
		m_nScore/=34.4;
		break;
	case 4:
		m_nScore/=28.5;
		break;
	default:
		m_nScore/=30.5;
		break;
	}
}

void UserData::OnLvChange()
{
	RecalcParam();
	g_pGameCenter->m_pRankCenter->CheckRank(RANK_LEVEL,(void*)m_dwUin,true);
	g_pMentoringCenter->CheckGraduate(this);
	if(m_xUWealth[UW_LV] >= 30)
	{//30级生成8位激活码
		if(strlen(m_szInviteCode) == 0)
		{
			int nSvrID	= ServerCfg::GetInstance().nServerID;

			for (int i = 0; i < 8; i++)
			{
				int nVal	= 0;
				if(i<2)
					nVal	= (nSvrID>>(5*i)) & 0x1F;
				else
					nVal	= (m_dwUin>>(5*(i-2))) & 0x1F;
				if(nVal >= 10)
				{
					m_szInviteCode[i]	= (nVal-10)%26 + 'A';
				}
				else
					m_szInviteCode[i]	= nVal + '0';
			}
			m_pDB->ExecuteSQL(FALSE,"insert into "RPG_INVITE_TABLE" (uinsvr,invitecode,reachnum,checknum) values (%d,'%s',%d,%d)",
				m_dwUin,m_szInviteCode,0,0);

			//BuildPacketEx(pSendPack,ACCOUNT,GA_SEND,buf,256);
			//SET_DATA(pSend, ACCOUNT,GA_SEND,pSendPack);
			//pSend->bEvent	= 0;
			//memcpy(pSend->szCode,m_szInviteCode,10);
			//g_pNetCenter->SendNGPacket(pSendPack,0);
			SendInviteInfo();
		}
		//发放奖励
		if(strlen(m_szBeInvCode)==8)
		{
			SendPopMsg("恭喜你获得邀请码奖励(10W金币,50个绑定钻石)!!");
			m_xUWealth.AddWealth(UW_BINDRMB,50,RMB_SAVE_INVITE);
			m_xUWealth.AddWealth(UW_GOLD,100000,1,NULL,UW_GOLD_INVICODE);

			BuildPacketEx(pSendPack,ACCOUNT,GA_SEND,buf,256);
			SET_DATA(pSend, ACCOUNT,GA_SEND,pSendPack);
			pSend->bEvent	= 1;
			memcpy(pSend->szCode,m_szBeInvCode,10);
			g_pNetCenter->SendNGPacket(pSendPack,0);//转发给账号服务器


			//清除自己的邀请码
			char	szTmp[256];
			sprintf(szTmp,"$%s",m_szBeInvCode);
			m_pDB->ExecuteSQL(FALSE,"update "RPG_ACCOUNT_TABLE" set beinvitecode='%s' where accountuin=%d",szTmp,m_dwAccount);
			memset(m_szBeInvCode,0,10);


		}
		m_pUserRecord->AddLvupLog(m_xUWealth[UW_LV]);
	}
	if (m_xUWealth[UW_LV] == 3&&m_dwAccCreateTime/86400 == CMTime::GetCurTickCount()/86400)
	{
		g_pGameCenter->SendPValInfo(m_nTPID,1);

	}
}

void UserData::MapObjChange()
{
	m_pMObjCenter->m_pPlayer->xParam.nHpMax = m_Param.nParam[EQUIP_ATTR_HP];
	m_pMObjCenter->m_pPlayer->xParam.nMpMax = m_Param.nParam[EQUIP_ATTR_MP];
	m_pMObjCenter->m_pPlayer->xParam.nAtkW = m_Param.nParam[EQUIP_ATTR_ATK_W];
	m_pMObjCenter->m_pPlayer->xParam.nAtkN = m_Param.nParam[EQUIP_ATTR_ATK_N];
	m_pMObjCenter->m_pPlayer->xParam.nDefW = m_Param.nParam[EQUIP_ATTR_DEF_W];
	m_pMObjCenter->m_pPlayer->xParam.nDefN = m_Param.nParam[EQUIP_ATTR_DEF_N];
	m_pMObjCenter->m_pPlayer->xParam.nAim = m_Param.nParam[EQUIP_ATTR_HIT];
	m_pMObjCenter->m_pPlayer->xParam.nDodge = m_Param.nParam[EQUIP_ATTR_MISS];
	m_pMObjCenter->m_pPlayer->xParam.nCrit = m_Param.nParam[EQUIP_ATTR_CRIT];
	m_pMObjCenter->m_pPlayer->xParam.nMSpeed = m_Param.nSpeed;
	m_pMObjCenter->m_pPlayer->xParam.nPvpDef	= (95+m_xUWealth[UW_LV]*5)/2;
	m_pMObjCenter->m_pPlayer->xParam.nPvpJSDef	= m_Param.nPvpJSDef;
	if(m_pMObjCenter->m_pPlayer->wSkin[0] != m_Param.nSkin || m_pMObjCenter->m_pPlayer->wSkin[1] != m_Param.nWeapon || m_pMObjCenter->m_pPlayer->wSkin[2] != m_Param.nHorse)
		m_pMObjCenter->m_pPlayer->bSkinChged = true;
	m_pMObjCenter->m_pPlayer->wSkin[0] = m_Param.nSkin;
	m_pMObjCenter->m_pPlayer->wSkin[1] = m_Param.nWeapon;
	m_pMObjCenter->m_pPlayer->wSkin[2] = m_Param.nHorse;
	m_pMObjCenter->m_pPlayer->bParamChged = true;
	m_pMObjCenter->m_pPlayer->byCountry = m_byCountry;
	m_pMObjCenter->m_pPlayer->byViplv = m_xUWealth[UW_VIPLV];
}

void UserData::OnPlayerBeHit()
{
	int nRandom = Random_Int(0,100);
	if (nRandom<3)
	{
		ItemObj* pEquip = _getRandomEquip();
		if (!pEquip)
		{
			return;
		}
		pEquip->ChgDura(-1);
	}
}

ItemObj* UserData::_getRandomEquip()
{
	// 	xnList* temp = xnList::Create();
	// 	for (int i=0;i<EQUIP_MAX;i++)
	// 	{
	// 		ItemObj* pObj = m_pItemCenter->GetEquipByIndex(i);
	// 		if (pObj&&pObj->GetDataVal(ITEM_AD_NUM))
	// 		{
	// 			temp->Add(pObj);
	// 		}
	// 	}
	// 	if (!temp->Count)
	// 	{
	// 		return NULL;
	// 	}
	ItemObj* pObj = (ItemObj*)m_pItemCenter->GetEquipByIndex(Random_Int(0,EQUIP_MAX-1));
	//	temp->Free();
	return pObj;
}

void UserData::OnPlayerDeath()
{
	if(m_nRealMIndex != 50){
		for (int i=0;i<EQUIP_MAX;i++)
		{
			ItemObj* pObj = m_pItemCenter->GetEquipByIndex(i);
			if (pObj&&pObj->GetDataVal(ITEM_AD_NUM))
			{
				pObj->ChgDura(-pObj->m_pBase->nDura/10);
			}
		}
	}

	if (m_nRealMIndex == 27)
	{
		if (m_pWarRankFight)
		{
			g_pGameCenter->m_pWarRankCenter->OnBattleResult(this,false);
		}
	}
	if (m_pFightTarget)
	{
		_onFightResult(false);
	}
	_runLuaFunctionOnPlayerEvent("OnPlayerDeath");
	m_pUserRecord->RunLuaDeadFunc();
}

void UserData::CreateInit()
{
	m_dwNextItemID	= 2;
	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set nextitemid=%d where uin=%d",m_dwNextItemID,m_dwUin);
	// 	m_xUWealth.DBInitWealth(UW_LV,1);
	// 	m_xUWealth.DBInitWealth(UW_POW,50);
	// 	m_dwHomeID		= 0;
	// 	m_dwHomeDuty		= 0;
	// 
	// 	if(m_byCountry == 1)
	// 	{
	// 		m_nMIndex		= 103;
	// 		m_nRealMIndex	= 103;
	// 		m_pMObjCenter->InitPlayer(m_dwUin,m_szNick,103,2900+rand()%100,2300+rand()%100,m_byJob,m_bySex);
	// 		m_wReviveMap	= 103;
	// 		m_wRevivePosX	= 2900;
	// 		m_wRevivePosY	= 2300;
	// 	}
	// 	else
	// 	{
	// 		m_nMIndex		= 203;
	// 		m_nRealMIndex	= 203;
	// 		m_pMObjCenter->InitPlayer(m_dwUin,m_szNick,203,2900+rand()%100,2300+rand()%100,m_byJob,m_bySex);
	// 		m_wReviveMap	= 203;
	// 		m_wRevivePosX	= 2900;
	// 		m_wRevivePosY	= 2300;
	// 	}

	SkillBase* pSBase	= SkillConfig::GetInstance().GetJobSkill(m_byJob,0);
	if(pSBase)
	{
		SkillObj* pSkill	= m_pSkillCenter->SetSkill(pSBase->wSkillID,1);
		pSkill->byPos		= 1;
		m_pSkillCenter->_insertDB(pSkill);
	}
}

void UserData::SetRevivePos( WORD wMI, WORD wX, WORD wY )
{
	if(wMI == m_wReviveMap)
		return;
	if(m_byCountry == 1 && wMI/100 == 2)
		return;
	if(m_byCountry == 2 && wMI/100 == 1)
		return;

	m_wReviveMap	= wMI;
	m_wRevivePosX	= wX;
	m_wRevivePosY	= wY;
	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set rmindex=%d,rmposx=%d,rmposy=%d where uin=%d",
		m_wReviveMap,m_wRevivePosX,m_wRevivePosY,m_dwUin);
}

void UserData::RequestRevive( bool bUserItem )
{

	if(bUserItem)
	{
		if(m_pMObjCenter->m_pPlayer->xParam.nHp > 0)
			return;
		if(m_pItemCenter->RemoveItemByID(30007))
		{
			m_pMObjCenter->HealthHpMp(m_pMObjCenter->m_pPlayer,m_pMObjCenter->m_pPlayer->xParam.nHpMax,m_pMObjCenter->m_pPlayer->xParam.nMpMax,true);
		}
	}
	else
	{
		int	nRM	= 0;
		int	nRX	= 0;
		int	nRY	= 0;
		if(m_nMIndex >= 1000)
		{//副本复活
			MapCopyObj* pMCopy	= g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(m_nMIndex);
			if(pMCopy)
			{
				nRM	= m_nMIndex;
				nRX	= pMCopy->m_pMCBase->nStartX;
				nRY	= pMCopy->m_pMCBase->nStartY;

			}
		}else
		{
			if (m_nRealMIndex == 60)
			{//黑暗之潮
				ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(1014);
				if (pObj)
				{
					nRM	= m_nRealMIndex;
					int nID,nPosx1,nPosy1,nPosx2,nPosy2;
					sscanf(pObj->pObject,"%d_%d_%d_%d_%d",&nID,&nPosx1,&nPosy1,&nPosx2,&nPosy2);
					if(m_byCountry == 1)
					{
						nRX	= nPosx1;
						nRY	= nPosy1;
					}
					else
					{
						nRX	= nPosx2;
						nRY	= nPosy2;
					}
				}
			}else if (m_nRealMIndex == 61)
			{//天灾来袭
				ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(1015);
				if (pObj)
				{
					nRM	= m_nRealMIndex;
					int nID,nPosx1,nPosy1,nPosx2,nPosy2;
					sscanf(pObj->pObject,"%d_%d_%d_%d_%d",&nID,&nPosx1,&nPosy1,&nPosx2,&nPosy2);
					if(m_byCountry == 1)
					{
						nRX	= nPosx1;
						nRY	= nPosy1;
					}
					else
					{
						nRX	= nPosx2;
						nRY	= nPosy2;
					}
				}
			}else if (m_nRealMIndex == 59)
			{//全民国战
				ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(1016);
				if (pObj)
				{
					nRM	= m_nRealMIndex;
					int nID,nPosx1,nPosy1,nPosx2,nPosy2;
					sscanf(pObj->pObject,"%d_%d_%d_%d_%d",&nID,&nPosx1,&nPosy1,&nPosx2,&nPosy2);
					if(m_byCountry == 1)
					{
						nRX	= nPosx1;
						nRY	= nPosy1;
					}
					else
					{
						nRX	= nPosx2;
						nRY	= nPosy2;
					}
				}
			}else if (m_nRealMIndex == 49)
			{//军团争霸
				nRM	= m_nRealMIndex;
				//ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(1011);
				//int nID,nPosx1,nPosy1,nPosx2,nPosy2;
				//sscanf(pObj->pObject,"%d_%d_%d_%d_%d",&nID,&nPosx1,&nPosy1,&nPosx2,&nPosy2);
				//if(m_byCountry == 1)
				//{
				//	nRX	= nPosx1;
				//	nRY	= nPosy1;
				//}
				//else
				//{
				//	nRX	= nPosx2;
				//	nRY	= nPosy2;
				//}
				int nPosX[3] = {448,1088,2240};
				int nPosY[3] = {1856,1024,1728};
				if(ServerCfg::GetInstance().nVersionDef)
				{//android服务器特定
					nPosX[0] = 1984;
					nPosX[1] = 512;
					nPosX[2] = 2560;
					nPosY[0] = 2240;
					nPosY[1] = 256;
					nPosY[2] = 704;
				}

				int nRandPos = rand()%3;
				if (nRandPos > -1 && nRandPos < 3)
				{
					nRX	= nPosX[nRandPos];
					nRY	= nPosY[nRandPos];
				}else
				{
					nRX	= nPosX[1];
					nRY	= nPosY[1];
				}

			}else if (m_nRealMIndex == 63)
			{//巫妖王之怒
				if(g_pGameCenter->m_pActivityCenter->CheckBeingActive(1020))
				{
					nRM	= m_nRealMIndex;
					nRX	= 3991;
					nRY	= 2176;
				}
			}
			else if (m_nRealMIndex == 71)
			{//无尽之海
				ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(1027);
				if (pObj)
				{
					nRM	= m_nRealMIndex;
					int nID,nPosx1,nPosy1,nPosx2,nPosy2;
					sscanf(pObj->pObject,"%d_%d_%d_%d_%d",&nID,&nPosx1,&nPosy1,&nPosx2,&nPosy2);
					if(m_byCountry == 1)
					{
						nRX	= nPosx1;
						nRY	= nPosy1;
					}
					else
					{
						nRX	= nPosx2;
						nRY	= nPosy2;
					}
				}
			}
			else if (m_nRealMIndex == 69)
			{//守卫xxx活动
				nRM	= m_nRealMIndex;
				nRX	= 2368;
				nRY	= 2176;
			}
		}

		if(nRM	== 0)
		{
			nRM	= m_wReviveMap;
			nRX	= m_wRevivePosX;
			nRY	= m_wRevivePosY;
		}
		JumpTo(nRM,nRX,nRY);
	}
}

void UserData::_dbUpdateHomeSkill()
{
	m_pDB->SetBLOB(false,&m_xHomeSkill,sizeof(UserHomeSkill),"update "RPG_USER_TABLE" set homeskill=? where uin=%d",m_dwUin);
}

void UserData::_loadHomeSkill(BYTE* pData,int nSize)
{
	memcpy(&m_xHomeSkill,pData,nSize>sizeof(UserHomeSkill)?sizeof(UserHomeSkill):nSize);
}

void UserData::OnLeaveTeam()
{
	ChatCenter::GetInstance().UnRegChatObj(m_pChatRoom[CHAT_ROOM_TEAM],this);

}

void UserData::AddHomeSkill( BYTE byType )
{
	if (byType >=USER_HOME_SKILL_MAX)
	{
		return;
	}
	static int nContr[10] = {100,200,400,800,1600,3200,6400,12800,25600,51200};

	BYTE nLv = m_xHomeSkill.bySkillLv[byType];

	if (nLv >= 10)
	{
		return;
	}
	if (m_nContribute<nContr[nLv])
	{
		SendWarnMsg("贡献不足，完成帮会任务可获得个人贡献。");
		return;
	}
	AddContribute(-nContr[nLv]);
	m_xHomeSkill.bySkillLv[byType]++;
	_dbUpdateHomeSkill();
	_sendHomeSkill();
	RecalcParam();
	m_pUserRecord->RunLuaFamilyFunc(4,m_xHomeSkill.bySkillLv[byType]);
	PlayEffect(nEffectSuccess);
}

void UserData::_sendHomeSkill()
{
	BuildPacketEx(pPacket, USER, S_SEND_HOMESKILL, buf, 1024);
	SET_DATA(pSend, USER, S_SEND_HOMESKILL, pPacket);
	pSend->xSkill = m_xHomeSkill;
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::SendFamilyQuest()
{
	// 	BuildPacketEx(pPacket, USER, S_SEND_HOME_QUEST, buf, 1024);
	// 	SET_DATA(pSend, USER, S_SEND_HOME_QUEST, pPacket);
	// 	pSend->byTimes = GetFlag(nFamilyQuestTimesFlag);
	int nQuestID = GetFlag(nFamilyQuestIDFlag);
	if (!nQuestID)
	{
		nQuestID = _getFamilyQuest();
		SetFlag(nFamilyQuestIDFlag,nQuestID);
	}
	// 	TCP_SendPacketToUin(m_dwUin,pPacket);
	TCP_SendCmdToUin(m_dwUin,USER,S_SEND_HOME_QUEST);

}

int UserData::_getFamilyQuest()
{
	xnList* pList = xnList::Create();
	QuestCfg::GetInstance().GetFamilyQuest(m_xUWealth.GetWealth(UW_LV),pList);
	int nId = 0;
	if (pList->Count)
	{
		QuestObj* pQObj = (QuestObj*)pList->Items[Random_Int(0,pList->Count-1)];
		nId = pQObj->wQID;
	}
	pList->Free();

	return nId;
}

int	UserData::GetMarryQuest()
{
	int nFlag = m_pUserFlag->GetFlag(28);
	if(nFlag > 0)
		return nFlag;
	xnList* pList = xnList::Create();
	QuestCfg::GetInstance().GetMarryQuest(m_xUWealth.GetWealth(UW_LV),pList);
	if (pList->Count)
	{
		QuestObj* pQObj = (QuestObj*)pList->Items[Random_Int(0,pList->Count-1)];
		nFlag = pQObj->wQID;
	}
	pList->Free();

	m_pUserFlag->SetFlag(28,nFlag);
	return nFlag;
}

void UserData::OnFamilyQuestComplete(int nState)
{
	AddFlag(nFamilyQuestTimesFlag,1);
	if (nState == 1)
	{
		int nFlagTimes = GetFlag(nFamilyQuestTimesFlag);
		AddContribute(nFlagTimes*3);
		AddWealth(UW_EXP,m_xUWealth.GetWealth(UW_LV)*m_xUWealth.GetWealth(UW_LV)*nFlagTimes);
		SendWarnMsg("完成军团任务,获得%d个人贡献",nFlagTimes*3);
		g_pGameCenter->m_pHomeCenter->AddHomeExp(m_dwHomeID,nFlagTimes);
		m_pUserRecord->RunLuaFamilyFunc(3,nFlagTimes*3);
	}
	SetFlag(nFamilyQuestIDFlag,0);
	SendFamilyQuest();
}

void UserData::AddContribute( int nVal )
{
	m_nContribute += nVal;

	if (nVal > 0)
	{
		m_nMaxContribute += nVal;
		m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set homemaxcontribution=%d where Uin=%d",m_nMaxContribute,m_dwUin);
		SendMaxContribute();
	}

	UserSimple* pUser = g_pUserList->GetUser(m_dwUin);
	if (pUser)
	{
		pUser->nContribution = m_nContribute;
	}
	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set homecontribution=%d where Uin=%d",m_nContribute,m_dwUin);

	if (nVal > 0)
		SendMsg("获得个人贡献×%d",nVal);
	else
		SendMsg("失去个人贡献×%d",-nVal);
}

void UserData::SendMaxContribute()
{
	if (m_nContribute > m_nMaxContribute)
	{
		m_nMaxContribute = m_nContribute;
		m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set homemaxcontribution=%d where Uin=%d",m_nMaxContribute,m_dwUin);
	}

	BuildPacketEx(pPacket, USER, S_SEND_HOME_MAXCTB, buf, 1024);
	SET_DATA(pSend, USER, S_SEND_HOME_MAXCTB, pPacket);
	pSend->dwHomeMaxCtb = m_nMaxContribute;
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::SendFortData(bool bOldC)
{
	if (!m_pFortData)
	{
		m_pFortData = g_pGameCenter->m_pFortCenter->GetFortData(m_dwUin);
	}
	if (m_pFortData)
	{
		m_pFortData->SetUser(this);
		m_pFortData->SendFortData(m_dwUin,bOldC);
	}
}

void UserData::SendServerTime()
{
	BuildPacketEx(pPacket, USER, S_SEND_SERVERTIME, buf, 1024);
	SET_DATA(pSend, USER, S_SEND_SERVERTIME, pPacket);
	pSend->dwServerTime = CMTime::GetCurTickCount();
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::SendWorldInfo()
{
	BuildPacketEx(pPacket, USER, S_WORLD_INFO, buf, 1024);
	SET_DATA(pSend, USER, S_WORLD_INFO, pPacket);
	pSend->wFstHome = g_pGameCenter->m_pWorldFlag->GetFlagInt(1);
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void UserData::SendInviteInfo()
{
	BuildPacketEx(pPacket, USER, S_INVITE_CODE, buf, 1024);
	SET_DATA(pSend, USER, S_INVITE_CODE, pPacket);
	pSend->nReachNum	= m_wReachInvNum;
	pSend->nCheckNum	= m_wCheckInvNum;
	strcpy(pSend->szInvite,m_szInviteCode);
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

FortData* UserData::GetFortData()
{
	if (!m_pFortData)
	{
		m_pFortData = g_pGameCenter->m_pFortCenter->GetFortData(m_dwUin);
		if(m_pFortData) m_pFortData->SetUser(this);
	}
	return m_pFortData;

}

LUAFUN	int UserData::GetManorLv()
{
	return GetFortData()->m_pManorData->nLv;
}

LUAFUN	void UserData::AddManorExp( int nVal )
{
	GetFortData()->AddManorExp(nVal);
}

int UserData::GetScore()
{
	return m_nScore;

}

int UserData::GetScore( int* nParam ,BYTE byJob)
{
	int nScore = 0;
	static int nAttrScore[EQUIP_ATTR_MAX] = {4,6,12,12,15,15,20,30,40};
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		nScore += nParam[i]*nAttrScore[i];
	}
	switch(byJob)
	{
	case 1:
		return nScore/30.5;
	case 2:
		return nScore/29.2;
	case 3:
		return nScore/34.4;
	case 4:
		return nScore/28.5;
	}
	return nScore/30.5;
}
static double nRank[] = {0.75,1,1.5,2};
bool UserData::CheckMCopyOk( WORD wEnemy )
{
	CheckEnemyExAward(wEnemy);

	if(m_pMapCopy && m_nMIndex >= 1000)
	{
		if(m_pMapCopy->m_pMCBase->byWinType == 0)
		{
			if(m_pMapCopy->m_pMCBase->wWinID == wEnemy)
			{//副本完成

				if (m_pMapCopy->m_pMCBase->byType == 3 && m_pMapCopy->m_pMCBase->wID!=404 && m_pMapCopy->m_pMCBase->wID!=412
					&& m_pMapCopy->m_pMCBase->wID != 413 && m_pMapCopy->m_pMCBase->wID != 414)
				{
					char szMsg[128];
					sprintf(szMsg,"“%s”的BOSS全部被击杀，活动已经结束！",m_pMapCopy->m_pMCBase->szName);
					ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);

					g_pGameCenter->m_pMapCopyCenter->SetCopyDailyLmt(m_pMapCopy->m_pMCBase->wID);
				}

				if(m_pMapCopy->m_pMCBase->byType == 5)
				{
					m_pUserFlag->SetMarkFlag(m_pMapCopy->m_pMCBase->wID,101);
					int nReachLv	= m_pUserFlag->GetFlag(23);
					if(nReachLv < m_pMapCopy->m_pMCBase->wID-1000)
					{//设置总进度
						m_pUserFlag->SetFlag(23,nReachLv+1);

						static int nAwardItem[] = {10,10,20,20,40,30,60,40,80,50,100,60,120,70,140,80,160,90,180,100,190,110,200,120,210,0,};
						static int	nAwardMoney[] = {5,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,0,};
						if ((nReachLv + 1)%6 == 0)
						{
							char szText[256];
							sprintf(szText,"[%s]的[ %s ]英勇无比、神力过人，成功击败了翡翠梦境第%d层的BOSS，获得了大量经验和奖励！", m_byCountry == 1?(char*)"联盟":(char*)"部落",m_szNick,nReachLv+1);
							ChatCenter::GetInstance().SendSystemMsg(szText, 0, 0, 1, true);

							int nAwardLv = (nReachLv+1)/6 - 1;
							int nItemID = 0;	
							if (nAwardLv%2 != 0)
								nItemID = 20000;	//炼星石
							else
								nItemID	= 20006;	//培养精华
							char	szTemp[128];
							sprintf(szTemp,"您成功击败了翡翠梦境第%d层的BOSS，获得了大量奖励！", nReachLv + 1);
							g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,m_dwUin,"系统", szTemp,nAwardMoney[nAwardLv]*10000, nItemID,nAwardItem[nAwardLv],true);
						}
					}
				}
				else if(m_pMapCopy->m_pMCBase->byLimitTime && m_pMapCopy->m_pMCBase->wID != 412 && m_pMapCopy->m_pMCBase->wID != 414)
				{
					int nLmtFlag = m_pUserFlag->GetMarkFlag(m_pMapCopy->m_pMCBase->wID);
					if (nLmtFlag&&nLmtFlag< m_pMapCopy->m_pMCBase->byLimitTime)
						m_pUserFlag->SetMarkFlag(m_pMapCopy->m_pMCBase->wID,nLmtFlag+1);
					else if(!nLmtFlag)
						m_pUserFlag->AddMarkFlag(m_pMapCopy->m_pMCBase->wID);
				}
				if (m_pMapCopy->m_pMCBase->wID == 414)
					m_pUserFlag->AddMarkFlag(m_pMapCopy->m_pMCBase->wID);

				if (m_pMapCopy->m_pMCBase->wID == 412)
				{
					char szMsg[128];
					sprintf(szMsg,"%s的队伍成功的击溃了异界BOSS阿格拉玛，艾泽拉斯又恢复了宁静。",(m_pTeamData?m_pTeamData->GetTeamLeaderName():m_szNick));
					ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);
				}

				CMTime	tmNow	= CMTime::GetCurTime();
				if(m_pTeamData && m_pTeamData->GetTeamLeaderUin() != m_dwUin)
				{
					TeamUser* pLeader	= m_pTeamData->GetTeamLeader();
					if (pLeader->pUser&&pLeader->pUser->m_pMapCopy&&pLeader->pUser->m_nMIndex>=1000&&pLeader->pUser->m_pMapCopy->m_pMCBase->byWinType==0)
						m_dwCopyStartTick = pLeader->pUser->m_dwCopyStartTick;
				}
				DWORD   dwUseTime = tmNow.GetTickCount() - m_dwCopyStartTick;
				int     nRankVal = GetCopyRank(dwUseTime,m_pMapCopy->m_pMCBase->dwRankTime);
				if(m_pMapCopy->m_pMCBase->byType == 5)
					nRankVal	= 1;	//挑战任务没有评价
				m_nMCopyCardCnt	= 0;
				m_byMCopyAwardNum	= 8;
				if (m_pMapCopy->m_pMCBase->nExp)
					m_xUWealth.AddWealth(UW_EXP,m_pMapCopy->m_pMCBase->nExp*nRank[nRankVal]);
				if (m_pMapCopy->m_pMCBase->nGold)
					m_xUWealth.AddWealth(UW_GOLD,m_pMapCopy->m_pMCBase->nGold*nRank[nRankVal],1,NULL,UW_GOLD_MCOPY);
				if(m_pMapCopy->m_pMCBase->byLifeCost)
					m_xUWealth.SubWealth(UW_POW,m_pMapCopy->m_pMCBase->byLifeCost);

				//m_pMObjCenter->LockMapAtk(m_pMapCopy->m_dwMapIndex,1);
				g_pGameCenter->m_pCallEnemyCenter->KillEnemy(m_nMIndex,GetPosX(),GetPosY(),2000,0);

				BuildPacketEx(pPacket, USER, S_SEND_MAPCOPY_OK, buf, 256);
				SET_DATA(pSend, USER, S_SEND_MAPCOPY_OK, pPacket);
				pSend->wMCopyID		= m_pMapCopy->m_pMCBase->wID;
				pSend->byRankVal    = nRankVal;
				TCP_SendPacketToUin(m_dwUin,pPacket);

				m_dwCopyStartTick = 0;
				m_pUserRecord->AddRecord(UA_COMPLETE,UO_MAPCOPY,m_pMapCopy->m_pMCBase->wID,0);
				m_pUserRecord->RunLuaMapCopyFunc(1,m_pMapCopy->m_pMCBase->wID,m_pMapCopy->m_pMCBase->byType,nRankVal);
				return true;
			}
		}

	}
	return false;
}

void UserData::JoinExpeditionCopy()
{
	if(!m_pTeamData)
		return;

	TeamUser* pLeader	= m_pTeamData->GetTeamLeader();
	if(!pLeader->pUser || !pLeader->pUser->m_pMapCopy)
	{
		SendWarnMsg("队长未开启活动副本");
		return;
	}
	if(m_nMIndex >= 1000)
	{
		SendWarnMsg("请先离开副本");
		return;
	}

	MCopyBase* pMC = pLeader->pUser->m_pMapCopy->m_pMCBase;
	if(pMC->byLvNeed > m_xUWealth.GetWealth(UW_LV))
	{
		SendWarnMsg("等级不够");
		return;
	}

	if (m_pUserFlag->GetFlag(66) > 19)
	{
		SendWarnMsg("已通关副本，无法再次挑战");
		return;
	}

	m_pMapCopy	= g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(pLeader->pUser->m_pMapCopy->m_dwMapIndex,m_dwUin);
	if(m_pMapCopy)
	{
		m_wOldMIndex	= m_nMIndex;
		m_wOldPosX		= GetPosX();
		m_wOldPosY		= GetPosY();

		JumpTo(m_pMapCopy->m_dwMapIndex,m_pMapCopy->m_pMCBase->nStartX,m_pMapCopy->m_pMCBase->nStartY,m_pMapCopy->m_pMCBase->wBaseMap);
	}

	return;
}

void UserData::RequestCreateMapCopy(WORD wMID)
{
	MCopyBase*	pMC	= NULL;

	if (!m_pTeamData)
	{
		SendWarnMsg("请组队进入！");
		return;
	}

	if(m_pTeamData->GetTeamLeaderUin() != m_dwUin)
	{
		JoinTeamMCopy();
		return;
	}
	else
		pMC	= MCopyConfig::GetInstance().GetMCopy(wMID);

	if(!pMC)
	{
		SendWarnMsg("进入副本失败");
		return;
	}

	if(CheckJoinMCopy(pMC))
	{
		//if(pMC->byLimitTime)
		//	m_pUserFlag->AddMarkFlag(pMC->wID);
		m_wOldMIndex	= m_nMIndex;
		m_wOldPosX		= m_pMObjCenter->m_pPlayer->nPosX;
		m_wOldPosY		= m_pMObjCenter->m_pPlayer->nPosY;
		m_pMapCopy	= g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(m_dwUin,m_dwUin,pMC);
		if(m_pMapCopy && m_pMapCopy->m_pMCBase)
		{
			CMTime	tmNow	= CMTime::GetCurTime();
			m_dwCopyStartTick = tmNow.GetTickCount();

			if (m_pMapCopy->m_pMCBase->wID == 412 && !m_xUWealth.SubWealth(UW_GOLD,300000))
				return;

			JumpTo(m_pMapCopy->m_dwMapIndex,m_pMapCopy->m_pMCBase->nStartX,m_pMapCopy->m_pMCBase->nStartY,m_pMapCopy->m_pMCBase->wBaseMap);


			if (m_pMapCopy->m_pMCBase->wID == 412)
			{

				char szMsg[128] = {0};
				sprintf(szMsg,"%s的队伍开启了异界副本，异界军团来势汹汹。",m_szNick);
				ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);

				m_pUserFlag->AddMarkFlag(412);
				int nTeamLv = GetTeamTotalLv();
				GetMapServerObj()->ChgMapEnemyPow(20+nTeamLv*3,nTeamLv);
			}
		}

		if(m_pTeamData && m_pTeamData->GetTeamLeaderUin() == m_dwUin)
		{
			BuildPacketEx(pTeamPacket, RPGACTIVITY, S_CHECK_OPEN_MAPCOPY, bufteam, 256);
			SET_DATA(pSendTeam, RPGACTIVITY, S_CHECK_OPEN_MAPCOPY, pTeamPacket);
			pSendTeam->wMCID	= m_pMapCopy->m_pMCBase->wID;
			if (m_pMapCopy->m_pMCBase->wID == 412)
				pSendTeam->dwCostGold = 300000;
			else
				pSendTeam->dwCostGold = 0;

			for (int i = 0; i < TEAM_MAXUSER; i++)
			{
				if(m_pTeamData->m_xUser[i].dwUin && m_pTeamData->m_xUser[i].nState == TEAM_NORMAL)
				{
					TCP_SendPacketToUin(m_pTeamData->m_xUser[i].dwUin,pTeamPacket);
				}
			}
		}
	}
}

void UserData::CreateExpeditionCopy()
{
	if (!m_pTeamData)
	{
		SendWarnMsg("需要至少2人的队伍");
		return;
	}
	if (m_pTeamData->GetTeamLeaderUin() != m_dwUin)
	{
		SendWarnMsg("只有队长才能开启活动");
		return;
	}

	if (m_pUserFlag->GetFlag(66) > 19)
	{
		SendWarnMsg("已通过副本，无法再次开启");
		return;
	}

	MCopyBase* pMC = MCopyConfig::GetInstance().GetMCopy(410);

	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_pTeamData->m_xUser[i].dwUin)
		{
			if (!m_pTeamData->m_xUser[i].pUser)
			{
				SendWarnMsg("队伍成员必须在线");
				return;
			}
			if (m_pTeamData->m_xUser[i].pUser->GetWealth(UW_LV) < pMC->byLvNeed)
			{
				SendWarnMsg("队员[%s]等级不足，无法进入副本",m_pTeamData->m_xUser[i].pUser->m_szNick);
				return;
			}
			if (m_pTeamData->m_xUser[i].pUser->m_pMapCopy)
			{
				SendWarnMsg("队伍成员不能在副本里");
				return;
			}

			if (m_pTeamData->m_xUser[i].pUser->m_pUserFlag->GetFlag(66) > 19)
			{
				SendWarnMsg("队员[%s]已通关副本，无法再次进入副本",m_pTeamData->m_xUser[i].pUser->m_szNick);
				return;
			}

			if (m_pTeamData->m_xUser[i].pUser->m_pQuestCenter->GetQuest(5001)||
				m_pTeamData->m_xUser[i].pUser->m_pQuestCenter->GetQuest(5002)||
				m_pTeamData->m_xUser[i].pUser->m_pQuestCenter->GetQuest(5003))
			{
				SendWarnMsg("队员[%s]正在护送商队，无法进入副本！",m_pTeamData->m_xUser[i].pUser->m_szNick);
				return;
			}
		}
	}
	m_pMapCopy = g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(m_dwUin,m_dwUin,pMC);
	if (!m_pMapCopy)
	{
		SendWarnMsg("进入副本失败");
		return;
	}

	m_wOldMIndex	= m_nMIndex;
	m_wOldPosX		= GetPosX();
	m_wOldPosY		= GetPosY();
	JumpTo(m_pMapCopy->m_dwMapIndex,m_pMapCopy->m_pMCBase->nStartX,m_pMapCopy->m_pMCBase->nStartY,m_pMapCopy->m_pMCBase->wBaseMap);

	BuildPacketEx(pTeamPacket, USER, S_MCOPY_MASTERJOIN, bufteam, 256);
	SET_DATA(pSendTeam, USER, S_MCOPY_MASTERJOIN, pTeamPacket);
	pSendTeam->wMCID	= m_pMapCopy->m_pMCBase->wID;
	for (int i = 0; i < TEAM_MAXUSER; i++)
	{
		if(m_pTeamData->m_xUser[i].dwUin && m_pTeamData->m_xUser[i].nState == TEAM_NORMAL)
		{
			TCP_SendPacketToUin(m_pTeamData->m_xUser[i].dwUin,pTeamPacket);
		}
	}

	char szMsg[128];
	sprintf(szMsg,"%s的队伍成功的召唤出了燃烧远征中的德拉克”",m_szNick);
	ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);

	int nTeamLv = GetTeamTotalLv();
	GetMapServerObj()->ChgMapEnemyPow(20+nTeamLv*3,nTeamLv);
	return;
}

int UserData::GetTeamTotalLv()
{
	if (!m_pTeamData)
		return m_xUWealth.GetWealth(UW_LV);

	int total_lv = 0,total_count = 0;
	for (int i = 0; i < TEAM_MAXUSER; i++)
	{
		if(m_pTeamData->m_xUser[i].dwUin && m_pTeamData->m_xUser[i].nState != TEAM_CREATE && m_pTeamData->m_xUser[i].pUser && m_pTeamData->m_xUser[i].pUser->m_nMIndex == m_nMIndex)
		{
			total_lv += m_pTeamData->m_xUser[i].pUser->GetWealth(UW_LV);
			total_count++;
		}
	}

	return total_count>0?(total_lv/total_count):GetWealth(UW_LV);
}

bool UserData::CheckExpeditionCopy(WORD wEnemy)
{
	if (m_nMIndex < 1000)
		return false;

	if (wEnemy < 2001 || wEnemy > 2020)
		return false;

	int nGold	= 0;
	int nExp	= 0;
	if(m_pTeamData&&m_pTeamData->GetTeamUserCount(false)>1)
	{
		for (int i = 0; i < TEAM_MAXUSER; i++)
		{
			if(m_pTeamData->m_xUser[i].dwUin && m_pTeamData->m_xUser[i].pUser && m_pTeamData->m_xUser[i].pUser->m_nMIndex == m_nMIndex && m_pTeamData->m_xUser[i].nState != TEAM_CREATE)
			{
				if (m_pTeamData->m_xUser[i].pUser->m_pUserFlag->GetFlag(66) < wEnemy - 2000)
				{
					nGold = (m_pTeamData->m_xUser[i].pUser->GetWealth(UW_LV) + wEnemy - 2000)*900;
					m_pTeamData->m_xUser[i].pUser->m_xUWealth.AddWealth(UW_GOLD,nGold,1,NULL,UW_GOLD_EXPCOPY);
					nExp    = (m_pTeamData->m_xUser[i].pUser->GetWealth(UW_LV) + wEnemy - 2000)*1600;
					m_pTeamData->m_xUser[i].pUser->m_xUWealth.AddWealth(UW_EXP,nExp);

					if ((wEnemy - 2000)%5 == 0)
					{
						_EnemyAward* pEA	= EnemyKillConfig::GetInstance().GetEAward(wEnemy);
						if(pEA)
						{
							xnList*	pLItems	= xnList::Create();
							pEA->RandGetItem(pLItems);
							for (int j = 0; j < pLItems->Count; j++)
							{
								m_pTeamData->m_xUser[i].pUser->SendKillDrop((int)pLItems->Items[j]);
								m_pTeamData->m_xUser[i].pUser->m_pItemCenter->AddItemByID((int)pLItems->Items[j],1,0,ITEM_LOG_EXPCOPY);
							}
							pLItems->Free();
						}
					}

					m_pTeamData->m_xUser[i].pUser->m_pUserFlag->AddFlag(66,1);
				}
			}
		}
	}
	else
	{
		if (m_pUserFlag->GetFlag(66) < wEnemy - 2000)
		{
			nGold	= (m_xUWealth.GetWealth(UW_LV) + wEnemy - 2000)*900;
			m_xUWealth.AddWealth(UW_GOLD,nGold,1,NULL,UW_GOLD_EXPCOPY);
			nExp    = (m_xUWealth.GetWealth(UW_LV) + wEnemy - 2000)*1600;
			m_xUWealth.AddWealth(UW_EXP,nExp);

			if ((wEnemy - 2000)%5 == 0)
			{
				_EnemyAward* pEA	= EnemyKillConfig::GetInstance().GetEAward(wEnemy);
				if(pEA)
				{
					xnList*	pLItems	= xnList::Create();
					pEA->RandGetItem(pLItems);
					for (int i = 0; i < pLItems->Count; i++)
					{
						SendKillDrop((int)pLItems->Items[i]);
						m_pItemCenter->AddItemByID((int)pLItems->Items[i],1,0,ITEM_LOG_EXPCOPY);
					}
					pLItems->Free();
				}
			}

			m_pUserFlag->AddFlag(66,1);
		}
	}

	if ((wEnemy - 2000)%5 == 0)
	{
		char szMsg[128] = {0};
		if (m_pTeamData)
		{
			TeamUser* pLeader = m_pTeamData->GetTeamLeader();
			if (wEnemy == 2020)
			{
				sprintf(szMsg,"%s的队伍击杀了所有BOSS，结束了燃烧的远征！",pLeader->szName);
			}else
				sprintf(szMsg,"%s的队伍成功击杀了燃烧远征中的第%d波BOSS！",pLeader->szName,(wEnemy - 2000));
		}else
		{
			if (wEnemy == 2020)
			{
				sprintf(szMsg,"%s的队伍击杀了所有BOSS，结束了燃烧的远征！",m_szNick);
			}else
				sprintf(szMsg,"%s的队伍成功击杀了燃烧远征中的第%d波BOSS！",m_szNick,(wEnemy - 2000));
		}

		ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);
	}

	if (wEnemy < 2020)
	{
		wEnemy++;
		int nTeamLv = GetTeamTotalLv();
		g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(wEnemy,m_nMIndex,1096,1223,11,nTeamLv?(20+nTeamLv*3):100);
	}
	else
	{
		if (m_pTeamData)
		{
			for (int i = 0; i < TEAM_MAXUSER; i++)
			{
				if (m_pTeamData->m_xUser[i].dwUin && m_pTeamData->m_xUser[i].nState != TEAM_CREATE &&m_pTeamData->m_xUser[i].pUser && m_pTeamData->m_xUser[i].pUser->m_nMIndex == m_nMIndex)
				{
					BuildPacketEx(pPacket, USER, S_SEND_MAPCOPY_OK, buf, 256);
					SET_DATA(pSend, USER, S_SEND_MAPCOPY_OK, pPacket);
					pSend->wMCopyID		= 410;
					pSend->byRankVal    = 0;
					TCP_SendPacketToUin(m_pTeamData->m_xUser[i].dwUin,pPacket);
				}
			}
		}else
		{
			BuildPacketEx(pPacket, USER, S_SEND_MAPCOPY_OK, buf, 256);
			SET_DATA(pSend, USER, S_SEND_MAPCOPY_OK, pPacket);
			pSend->wMCopyID		= 410;
			pSend->byRankVal    = 0;
			TCP_SendPacketToUin(m_dwUin,pPacket);
		}
	}
}

int UserData::GetCopyRank(DWORD dwTime,DWORD dwLmtTime)
{
	if (dwTime <= dwLmtTime*0.3)
		return 3;
	else if (dwTime <= dwLmtTime*0.6)
		return 2;
	else if (dwTime <= dwLmtTime)
		return 1;
	else
		return 0;
}

DWORD UserData::MCFirstSendEquip()
{
	int nEquipType = rand()%9;
	return (10000+(m_byJob-1)*1000+200+nEquipType*10 +1);
}

void UserData::RqOpenMCopyCard()
{
	if(!m_pMapCopy || m_byMCopyAwardNum == 0)
		return;

	VipInfoObj* pVip	= VipConfig::GetInstance().GetObjByLv(m_xUWealth.GetWealth(UW_VIPLV));
	int	nCostRmb	= 0;
	if(pVip)
	{
		nCostRmb	= (m_nMCopyCardCnt-pVip->m_nCardtimes)*5;
	}
	else
		nCostRmb	= (m_nMCopyCardCnt)*5;

	if(nCostRmb > 0)
	{
		if(m_xUWealth.SubWealth(UW_RMB,nCostRmb,RMB_SAVE_MCCARD) == false)
			return;
	}

	DWORD wItem = 0;
	if (m_bFirstOpenCard)
	{
		m_bFirstOpenCard = false;
		wItem	= MCFirstSendEquip();
	}
	else
		wItem	= m_pMapCopy->m_pMCBase->RandGetItem();

	if(wItem)
	{
		m_pItemCenter->AddItemByID(wItem,1,0,ITEM_LOG_MCOPY);
		BuildPacketEx(pPacket, USER, S_OPENCARDRTL,buf, 256);
		SET_DATA(pSend, USER, S_OPENCARDRTL, pPacket);
		pSend->wItem	= wItem;
		TCP_SendPacketToUin(m_dwUin,pPacket);
	}

	m_nMCopyCardCnt++;
	m_byMCopyAwardNum--;
	if(m_nMCopyCardCnt >= 8)
		m_pUserRecord->RunLuaMapCopyFunc(2,m_pMapCopy->m_pMCBase->wID,m_pMapCopy->m_pMCBase->byType,8);
	else
		m_pUserRecord->RunLuaMapCopyFunc(2,m_pMapCopy->m_pMCBase->wID,m_pMapCopy->m_pMCBase->byType,1);
}

void UserData::RqOpenAllMCopyCard(WORD nWRmb,WORD nWCnt)
{
	if(!m_pMapCopy || m_byMCopyAwardNum == 0)
		return;
	if (nWCnt < 0)
		return;
	if (nWCnt  + m_nMCopyCardCnt> 8)
		nWCnt = 8 - m_nMCopyCardCnt;

	VipInfoObj* pVip	= VipConfig::GetInstance().GetObjByLv(m_xUWealth.GetWealth(UW_VIPLV));
	int	nCostRmb	= 0;
	int nFreeCardCnt= 0;
	if(pVip)
	{
		nFreeCardCnt	= 1 + pVip->m_nCardtimes;
	}
	else
		nFreeCardCnt	= 1;

	int nTotelCnt = nWCnt + m_nMCopyCardCnt;
	while(nTotelCnt - m_nMCopyCardCnt > 0)
	{
		if (nTotelCnt > nFreeCardCnt)
			nCostRmb += (nTotelCnt - nFreeCardCnt)>0?(nTotelCnt - nFreeCardCnt)*5:0;
		nTotelCnt--;
	}

	if (nCostRmb && (m_xUWealth.SubWealth(UW_RMB,nCostRmb,RMB_SAVE_MCCARD) == false))
		return;

	BuildPacketEx(pPacket, USER, S_RQ_OPENALL_RLT,buf, 256);
	SET_DATA(pSend, USER, S_RQ_OPENALL_RLT, pPacket);
	int nCnt = 0;
	while(nCnt < nWCnt)
	{
		if(m_byMCopyAwardNum == 0)
			break;
		DWORD wItem	= m_pMapCopy->m_pMCBase->RandGetItem();
		if(wItem)
		{
			m_pItemCenter->AddItemByID(wItem,1,0,ITEM_LOG_MCOPY);
			pSend->wItem[nCnt] = wItem;
			nCnt++;
			m_nMCopyCardCnt++;
			m_byMCopyAwardNum--;
			if(m_nMCopyCardCnt >= 8)
				m_pUserRecord->RunLuaMapCopyFunc(2,m_pMapCopy->m_pMCBase->wID,m_pMapCopy->m_pMCBase->byType,8);
			else
				m_pUserRecord->RunLuaMapCopyFunc(2,m_pMapCopy->m_pMCBase->wID,m_pMapCopy->m_pMCBase->byType,1);
		}
	}

	pSend->wCnt = nWCnt;
	pPacket->wSize	+= (nWCnt-1)*sizeof(DWORD);
	TCP_SendPacketToUin(m_dwUin,pPacket);

}

bool UserData::CheckJoinMCopy(MCopyBase* pMC)
{
	if(m_nMIndex >= 1000)
	{
		SendWarnMsg("请先离开副本");
		return false;
	}

	if(pMC->byLvNeed > m_xUWealth.GetWealth(UW_LV))
	{
		SendWarnMsg("等级不够");
		return false;
	}

	if(pMC->CheckTime() == false)
	{
		SendWarnMsg("副本未开放");
		return false;
	}
	if(pMC->byLimitTime)
	{
		if(m_pUserFlag->GetMarkFlag(pMC->wID) >= pMC->byLimitTime)
		{
			SendWarnMsg("该副本今日次数已用完");
			return false;
		}
	}

	if(pMC->byLifeCost && m_xUWealth.GetWealth(UW_POW) < pMC->byLifeCost)
	{
		SendWarnMsg("体力不够");
		return false;
	}

	if (pMC->wID == 412 && m_xUWealth.GetWealth(UW_GOLD) < 300000)
	{
		SendWarnMsg("金币不足");
		return false;
	}

	if(m_pTeamData && m_pTeamData->GetTeamLeaderUin() == m_dwUin)
	{
		for (int i=0;i<TEAM_MAXUSER;i++)
		{
			if (m_pTeamData->m_xUser[i].pUser)
			{
				if (m_pTeamData->m_xUser[i].pUser->m_pQuestCenter->GetQuest(5001)||
					m_pTeamData->m_xUser[i].pUser->m_pQuestCenter->GetQuest(5002)||
					m_pTeamData->m_xUser[i].pUser->m_pQuestCenter->GetQuest(5003))
				{
					SendWarnMsg("队员[%s]正在护送商队，无法进入副本！",m_pTeamData->m_xUser[i].pUser->m_szNick);
					return false;
				}

				if (m_pTeamData->m_xUser[i].pUser->m_xUWealth.GetWealth(UW_POW) < pMC->byLifeCost)
				{
					SendWarnMsg("队员[%s]体力不足，无法进入副本！",m_pTeamData->m_xUser[i].pUser->m_szNick);
					return false;
				}

				if (pMC->wID == 412 && m_pTeamData->m_xUser[i].pUser->m_xUWealth.GetWealth(UW_GOLD) < 300000)
				{
					SendWarnMsg("队员[%s]金币不足，无法进入副本！",m_pTeamData->m_xUser[i].pUser->m_szNick);
					return false;
				}
			}
		}
	}
	if (m_pTeamData)
	{
		if (pMC->wID == 414)
		{
			if (m_pTeamData->GetTeamUserCount(false) != 2)
			{
				SendWarnMsg("此副本限制人数为2人，无法进入副本！");
				return false;
			}
			if(!g_pMentoringCenter->GetRelation(m_pTeamData->m_xUser[0].pUser->m_dwUin,m_pTeamData->m_xUser[1].pUser->m_dwUin) 
				&& !g_pMentoringCenter->GetRelation(m_pTeamData->m_xUser[1].pUser->m_dwUin,m_pTeamData->m_xUser[0].pUser->m_dwUin))
			{
				SendWarnMsg("此副本需要有师徒关系才能进入！");
				return false;
			}
			int nLmtFlag = m_pUserFlag->GetMarkFlag(414);
			if (g_pMentoringCenter->CheckTeacherUser(m_dwUin))
			{
				if (nLmtFlag >= 3)
				{
					SendWarnMsg("教师进入师徒副本的最大次数为3次！");
					return false;
				}
			}else
			{
				if (nLmtFlag >= 1)
				{
					SendWarnMsg("徒弟进入师徒副本的最大次数为1次！");
					return false;
				}
			}
		}
		if (pMC->wID == 413)
		{
			if (m_pTeamData->GetTeamUserCount(false) != 2)
			{
				SendWarnMsg("此副本限制人数为2人，无法进入副本！");
				return false;
			}
			if(!g_pMarryCenter->GetRelation(m_pTeamData->m_xUser[0].pUser->m_dwUin,m_pTeamData->m_xUser[1].pUser->m_dwUin))
			{
				SendWarnMsg("此副本需要有夫妻关系的人才能进入！");
				return false;
			}
		}
	}

	return true;
}

void UserData::TeamStartMCopy()
{
	if(!m_pMCopyTeam)
		return;

	if(m_pTeamData)
		return;
	MCopyBase*	pMC	= m_pMCopyTeam;

	//if(pMC->byLimitTime)
	//	m_pUserFlag->AddMarkFlag(pMC->wID);

	//if(pMC->byLifeCost)
	//	m_xUWealth.SubWealth(UW_POW,pMC->byLifeCost);
	m_wOldMIndex	= m_nMIndex;
	m_wOldPosX		= m_pMObjCenter->m_pPlayer->nPosX;
	m_wOldPosY		= m_pMObjCenter->m_pPlayer->nPosY;
	m_pMapCopy	= g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(m_dwUin,m_dwUin,pMC);
	if(m_pMapCopy)
	{
		CMTime	tmNow	= CMTime::GetCurTime();
		m_dwCopyStartTick = tmNow.GetTickCount();

		JumpTo(m_pMapCopy->m_dwMapIndex,m_pMapCopy->m_pMCBase->nStartX,m_pMapCopy->m_pMCBase->nStartY,m_pMapCopy->m_pMCBase->wBaseMap);
	}

	int nCnt	= 4;
	while(pMC->pLRqMCopy->Count && nCnt)
	{
		m_pTeamData	= g_pGameCenter->m_pTeamCenter->CreateTeam(this);

		DWORD	dwObj	= (DWORD)pMC->pLRqMCopy->Delete(0);
		if(dwObj == m_dwUin)
			continue;

		UserData* pObj	= g_pGameCenter->GetPlayer(dwObj);

		if(pObj&&pObj->m_byCountry == m_byCountry)
		{
			m_pTeamData->Join(pObj,TEAM_NORMAL);

			pObj->JoinTeamMCopy();
			nCnt--;
		}
	}

	if(nCnt == 4 && m_nMIndex >= 1000)
	{//召唤助战
		m_pTeamData	= NULL;

		for (int n = 0; n < m_pMapCopy->m_pMCBase->byHelpNum; n++)
		{//多只怪
			WORD wBaseID	= m_pMapCopy->m_pMCBase->RandGetHelper(m_byJob);
			if(wBaseID)
			{
				DWORD	dwID	= g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(wBaseID,m_nMIndex,m_pMObjCenter->m_pPlayer->nPosX,m_pMObjCenter->m_pPlayer->nPosY,m_byCountry);
				if(dwID)
					m_pMObjCenter->SetCtrl(dwID,MAPOBJ_ENEMY,2);

				MapHelpObj* pHelp	= new MapHelpObj();
				pHelp->wHelpBase	= wBaseID;
				pHelp->dwHelpID	= dwID;
				pHelp->dwHelpTick	= 0;
				m_pMapCopy->m_pListHelper->Add(pHelp);
			}
		}


	}
}

void UserData::JoinTeamMCopy()
{
	if(!m_pTeamData)
		return;

	TeamUser* pLeader	= m_pTeamData->GetTeamLeader();
	if(!pLeader->pUser || !pLeader->pUser->m_pMapCopy)
	{
		SendWarnMsg("队长未选择副本");
		return;
	}
	if(CheckJoinMCopy(pLeader->pUser->m_pMapCopy->m_pMCBase))
	{
		m_pMapCopy	= g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(pLeader->pUser->m_pMapCopy->m_dwMapIndex,m_dwUin);
		if(m_pMapCopy && m_pMapCopy->m_pMCBase)
		{
			m_wOldMIndex	= m_nMIndex;
			m_wOldPosX		= m_pMObjCenter->m_pPlayer->nPosX;
			m_wOldPosY		= m_pMObjCenter->m_pPlayer->nPosY;

			if (m_pMapCopy->m_pMCBase->wID == 412 && !m_xUWealth.SubWealth(UW_GOLD,300000))
				return;

			if (m_pMapCopy->m_pMCBase->wID == 412)
				m_pUserFlag->AddMarkFlag(412);

			JumpTo(m_pMapCopy->m_dwMapIndex,m_pMapCopy->m_pMCBase->nStartX,m_pMapCopy->m_pMCBase->nStartY,m_pMapCopy->m_pMCBase->wBaseMap);
		}
	}
}

void UserData::SetSysMailID( int nID )
{
	m_nSysMailID = nID;
	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set sysmailid=%d where uin=%d",m_nSysMailID,m_dwUin);
}

bool UserData::GetMailItem( DWORD dwMailID )
{
	MailObject* pObj = m_pMailBox->GetMail(dwMailID);
	if (pObj)
	{
		if (pObj->byState != STATE_MAIL_READ && pObj->byState != STATE_MAIL_RETURN)
		{
			return false;
		}
		if (m_pItemCenter->GetBlankBag()<pObj->GetMailItemCount())
		{
			SendPopMsg("包裹空间不足");
			return false;
		}
		m_xUWealth.AddWealth(UW_GOLD,pObj->nMoney,1,NULL,UW_GOLD_MAIL);
		pObj->nMoney = 0;
		for (int i=0;i<nMaxMailItemCount;i++)
		{
			if (pObj->nItemIndex[i])
			{
				ItemObj* pItem = g_pGameCenter->m_pItemManage->RemoveItemObj(pObj->nItemIndex[i],true);
				if (pItem)
				{
					if (pItem->m_pBase&&pItem->m_pBase->dwTimeLimit)
					{
						pItem->SetDataVal(ITEM_AD_LIMITTIME,CMTime::GetCurTickCount()+pItem->m_pBase->dwTimeLimit*60);
					}
					g_pGameCenter->m_pItemManage->SetItemOwner(this,pItem,2);
					m_pItemCenter->AddItem(pItem);
				}
			}
			pObj->nItemIndex[i] = 0;
		}

		pObj->byState = STATE_MAIL_GOT;
		pObj->DBUpdateMail(m_pDB);
	}
	return true;
}

LUAFUN	int UserData::LearnSkill( WORD wSkill, BYTE byLv )
{
	SkillObj*	pSkill	= m_pSkillCenter->GetSkill(wSkill);
	if(pSkill == NULL)
	{
		if(byLv != 1)
			return 0;
	}
	else
	{
		if(pSkill->bySLv != byLv-1)
			return 0;
	}

	if(m_pSkillCenter->RequestSkillUp(wSkill,false) != NULL)
		return 1;

	return 0;
}

void UserData::SetFastItemID( int nID )
{
	m_nFastItemID = nID;
	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set fastitem=%d where uin=%d",m_nFastItemID,m_dwUin);
}

void UserData::SendFastItemID()
{
	BuildPacketEx(pPacket, USER, CS_SEND_FAST_ITEM,buf, 256);
	SET_DATA(pSend, USER, CS_SEND_FAST_ITEM, pPacket);
	pSend->dwItemID	= m_nFastItemID;
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

void	UserData::PopMarryMessage(char* szTitle,char* szMsg)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_POP_MARRYMESSAGE,buf,1024);
	SET_DATA(pData,RPGRELATION,S_SEND_POP_MARRYMESSAGE,pPacket);
	strcpy(pData->szTitle,szTitle);
	strcpy(pData->szMsg,szMsg);
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

bool UserData::RunLuaFunctionForQuest(int nQid, const char* const szFunctionName )
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	sprintf(sztxt,"%s%d",szFunctionName,nQid);
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);

	CLuaPlusSafeFunction_2<bool, UserData*> func;
	if(!func(pLuaState, lf, &bRet, this))
	{
		return false;
	}
	return bRet;
}

int UserData::GetWealth( BYTE bEnum )
{
	return m_xUWealth.GetWealth(bEnum);
}

LUAFUN	bool UserData::CheckQuest( int nQid )
{
	return m_pQuestCenter->GetQuest(nQid);
}

LUAFUN	void UserData::AddState( WORD wState,bool bRemove )
{
	m_pMObjCenter->AddState(m_pMObjCenter->m_pPlayer,wState,bRemove);
}

LUAFUN	bool UserData::CheckState( WORD wState )
{
	return m_pMObjCenter->CheckState(m_pMObjCenter->m_pPlayer,wState);
}

LUAFUN	bool UserData::RequestAcceptQuest( WORD wQID )
{
	return m_pQuestCenter->RequestAcceptQuest(wQID);
}

LUAFUN	bool UserData::RequestCompleteQuest( WORD wQID )
{
	return m_pQuestCenter->RequestCompleteQuest(wQID);
}

LUAFUN	bool UserData::RequestGiveUpQuest( WORD wQID )
{
	return m_pQuestCenter->RequestGiveUpQuest(wQID);
}

LUAFUN	int		UserData::GetPetCountByQuality(BYTE byQuality)
{
	if(byQuality < 5)
		return m_pSlaveCenter->m_szPetCount[byQuality];
	return 0;
}

LUAFUN	int		UserData::GetHorseCountByQuality(BYTE byQuality)
{
	if(byQuality < 5)
		return m_pHorseCenter->m_szHorseCount[byQuality];
	return 0;
}

LUAFUN	bool	UserData::DoneAchievement(int nID)
{
	return m_pUserAchievement->DoneAchievement(nID);
}

LUAFUN	bool	UserData::AddAchievementVal(int nID,int nVal)
{
	return m_pUserAchievement->AddAchievementVal(nID,nVal);
}

LUAFUN	bool	UserData::SetAchievementVal(int nID,int nVal)
{
	return m_pUserAchievement->SetAchievementVal(nID,nVal);
}

LUAFUN	int		UserData::GetAchievementVal(int nID)
{
	AchievementUserObj* pObj = m_pUserAchievement->GetObj(nID);
	if(pObj)
		return pObj->nCnt;
	return 0;
}

LUAFUN	bool	UserData::DonePlayerAchievement(int nUin,int nID)
{
	return g_pGameCenter->m_pAchievementCenter->DoneAchievement(nUin,nID);
}

LUAFUN	bool	UserData::AddPlayerAchievementVal(int nUin,int nID,int nVal)
{
	return g_pGameCenter->m_pAchievementCenter->AddAchievementVal(nUin,nID,nVal);
}

LUAFUN	bool	UserData::SetPlayerAchievementVal(int nUin,int nID,int nVal)
{
	return g_pGameCenter->m_pAchievementCenter->SetAchievementVal(nUin,nID,nVal);
}

LUAFUN	int		UserData::GetPlayerAchievementVal(int nUin,int nID)
{
	AchievementUserObj* pObj = g_pGameCenter->m_pAchievementCenter->GetUserObj(nUin,nID);
	if(pObj)
		return pObj->nCnt;
	return 0;
}

LUAFUN	void	UserData::AskEngage(int nUin)
{
	UserData* pTarget = g_pGameCenter->GetPlayer(nUin);
	if(pTarget == NULL || pTarget->m_bySex == m_bySex)
	{
		SendWarnMsg("抱歉，我们不太支持同性恋");
		return;
	}
	if(pTarget->m_pUserMarry->GetRelation() || m_pUserMarry->GetRelation())
	{
		SendWarnMsg("抱歉，我们不太认同婚外恋");
		return;
	}
	BuildPacketEx(pPacket,RPGRELATION,S_MARRY_SENDASK,buf,1024);
	SET_DATA(pData,RPGRELATION,S_MARRY_SENDASK,pPacket);
	pData->nUin = m_dwUin;
	strcpy(pData->szName,m_szNick);
	TCP_SendPacketToUin(nUin,pPacket);
}

LUAFUN	bool	UserData::CheckMarryConditions(int nUin)
{
	if(m_pUserMarry)
	{
		MarryData* pData = m_pUserMarry->GetRelation();
		if(pData && (pData->m_pWoman->dwUin == nUin || pData->m_pMan->dwUin == nUin))
			return g_pMarryCenter->CheckMarryConditions(this,pData->m_dwID);
		else
			SendWarnMsg("请带着你的爱人一起前来");
	}
	return false;
}

LUAFUN	void	UserData::GetMarried()
{
	if(m_pUserMarry)
	{
		MarryData* pData = m_pUserMarry->GetRelation();
		if(pData)
		{
			pData->m_nMarryCnt = 0;
			BuildPacketEx(pPacket,RPGRELATION,S_SEND_GETMARRIED,buf,1024);
			SET_DATA(pSend,RPGRELATION,S_SEND_GETMARRIED,pPacket);
			strcpy(pSend->szName,pData->m_pWoman->szName);
			TCP_SendPacketToUin(pData->m_pMan->dwUin,pPacket);
			strcpy(pSend->szName,pData->m_pMan->szName);
			TCP_SendPacketToUin(pData->m_pWoman->dwUin,pPacket);
		}
	}
}

LUAFUN	int		UserData::GetMarryState()
{
	if(m_pUserMarry)
	{
		MarryData* pData = m_pUserMarry->GetRelation();
		if(pData)
			return pData->m_byState;
	}
	return 0;
}

LUAFUN	int		UserData::GetLoverUin()
{
	if(m_pUserMarry)
	{
		MarryData* pData = m_pUserMarry->GetRelation();
		if(pData && pData->m_byState != 0)
		{
			if(pData->m_pMan == m_pMySimple)
				return pData->m_pWoman->dwUin;
			else if(pData->m_pWoman == m_pMySimple)
				return pData->m_pMan->dwUin;
		}
	}
	return 0;
}

MapServerObj* UserData::GetMapServerObj()
{
	if(m_bLogOut)
		return NULL;
	return g_pGameCenter->m_pMapObjCenter->GetMapObjByIndex(m_nMIndex);
}

bool UserData::_runLuaFunctionOnPlayerKillEnemy( UserData* pUser,int nEnemyID,int nID,const char* const szFunctionName )
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	sprintf(sztxt,"%s%d",szFunctionName,nEnemyID);
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	MapServerObj* pObj = pUser->GetMapServerObj();
	CLuaPlusSafeFunction_4<bool, UserData*,MapServerObj*,int> func;
	if(!func(pLuaState, lf, &bRet,pUser, pObj,nID))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

const char* UserData::GetFamilyName()
{
	HomeObj* pHome = g_pGameCenter->m_pHomeCenter->_getHomeObj(m_dwHomeID);
	if (pHome)
	{
		return pHome->m_szName;
	}else{
		return "";
	}
}

void UserData::_runLuaFunctionOnPlayerEvent( const char* const szFunctionName )
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	sprintf(sztxt,"%s",szFunctionName);
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return;
	}
	LuaFunction<void> lf(loFunction);
	CLuaPlusSafeFunction_2<void,UserData*> func;
	if(!func(pLuaState, lf,NULL,this))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return;
	}
}

bool UserData::_lockFightTarget( UserData* pUser )
{
	if (!pUser)
	{
		return false;
	}
	if (m_pFightTarget||pUser->m_pFightTarget)
	{
		SendWarnMsg("目标正在被挑战");
		return false;
	}
	pUser->m_pFightTarget = this;
	m_pFightTarget = pUser;
	return true;
}

void UserData::_unlockFightTarget()
{
	if (!m_pFightTarget)
	{
		return;
	}
	UserData* pUser = m_pFightTarget;
	m_pFightTarget = NULL;

	pUser->_unlockFightTarget();
	// 	if(m_nMIndex >= 1000)
	// 		JumpTo(m_wOldMIndex,m_wOldPosX,m_wOldPosY);
}

void UserData::_fightTarget( DWORD dwUin )
{
	if (m_pMapCopy)
	{
		SendWarnMsg("副本里不可挑战");
		return;
	}
	UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
	if (!pUser)
	{
		SendWarnMsg("未找到目标");
		return;
	}
	if (GetWealth(UW_LV)<24||pUser->GetWealth(UW_LV)<24)
	{
		SendWarnMsg("需要双方达到24级才能切磋");
		return;
	}
	if (pUser->m_pMapCopy)
	{
		SendWarnMsg("对方正在副本中");
		return;
	}

	if (!_lockFightTarget(pUser))
	{
		//SendWarnMsg("未知错误1");
		return;
	}
	BuildPacketEx(pPacket, USER, S_FIGHT_TARGET,buf, 256);
	SET_DATA(pSend, USER, S_FIGHT_TARGET, pPacket);
	strcpy(pSend->szName,m_szNick);
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
	SendWarnMsg("已发送切磋请求");
}

void UserData::_onFightResult( bool bWin )
{
	if (bWin)
	{
		SendWarnMsg("你战胜了对手");
	}else{
		UserData* pUser = m_pFightTarget;
		if (pUser)
		{
			SendWarnMsg("你被击败了");
			pUser->_onFightResult(true);
		}
	}
	_unlockFightTarget();
	JumpTo(m_wOldMIndex,m_wOldPosX,m_wOldPosY);
}

void UserData::_beginFight()
{
	UserData* pUser = m_pFightTarget;
	if (!pUser)
	{
		SendWarnMsg("未找到目标");
		return;
	}
	if (m_pMapCopy)
	{
		SendWarnMsg("副本里不可挑战");
		return;
	}
	if (pUser->m_pMapCopy)
	{
		SendWarnMsg("对方正在副本中");
		return;
	}

	MCopyBase* pMC	= MCopyConfig::GetInstance().GetMCopy(402);
	if (!pMC)
	{
		//SendWarnMsg("未知错误1");
		return;
	}
	m_pMapCopy	= g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(m_dwUin,m_dwUin,pMC);
	if (!m_pMapCopy)
	{
		return;
	}
	pUser->m_pMapCopy = m_pMapCopy;

	pUser->m_wOldMIndex = pUser->m_nMIndex;
	pUser->m_wOldPosX = pUser->GetPosX();
	pUser->m_wOldPosY = pUser->GetPosY();
	g_pGameCenter->m_pMapCopyCenter->JoinMCopy(pUser->m_dwUin,m_pMapCopy);
	pUser->JumpTo(m_pMapCopy->m_dwMapIndex,m_pMapCopy->m_pMCBase->nStartX-1360,m_pMapCopy->m_pMCBase->nStartY,m_pMapCopy->m_pMCBase->wBaseMap);
	pUser->AddState(nJJCStartState,false);
	m_wOldMIndex = m_nMIndex;
	m_wOldPosX = GetPosX();
	m_wOldPosY = GetPosY();
	JumpTo(m_pMapCopy->m_dwMapIndex,m_pMapCopy->m_pMCBase->nStartX,m_pMapCopy->m_pMCBase->nStartY,m_pMapCopy->m_pMCBase->wBaseMap);
	AddState(nJJCStartState,false);

}

LUAFUN bool UserData::CheckCompleteQuest( WORD wQID )
{
	return m_pQuestCenter->CheckCompleteQuest(wQID);
}

void UserData::JumpHomeWarcopy()
{
	if (!m_pTeamData)
	{
		SendWarnMsg("需要至少2人的队伍");
		return;
	}
	if (m_pTeamData->GetTeamLeaderUin() != m_dwUin)
	{
		SendWarnMsg("只有队长才能选择进入副本");
		return;
	}
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_pTeamData->m_xUser[i].dwUin)
		{
			if (!m_pTeamData->m_xUser[i].pUser)
			{
				SendWarnMsg("队伍成员必须在线");
				return;
			}
			if (m_pTeamData->m_xUser[i].pUser->m_pMapCopy)
			{
				SendWarnMsg("队伍成员不能在副本里");
				return;
			}
			if (m_pTeamData->m_xUser[i].pUser->m_dwHomeID!=m_dwHomeID)
			{
				SendWarnMsg("队伍成员必须是同一军团");
				return;
			}
			if (m_pTeamData->m_xUser[i].pUser->m_pUserFlag->GetMarkFlag(404))
			{
				SendWarnMsg("队员[%s]已挑战过该副本",m_pTeamData->m_xUser[i].pUser->m_szNick);
				return;
			}
			if (m_pTeamData->m_xUser[i].pUser->m_pQuestCenter->GetQuest(5001)||
				m_pTeamData->m_xUser[i].pUser->m_pQuestCenter->GetQuest(5002)||
				m_pTeamData->m_xUser[i].pUser->m_pQuestCenter->GetQuest(5003))
			{
				SendWarnMsg("队员[%s]正在护送商队，无法进入副本！",m_pTeamData->m_xUser[i].pUser->m_szNick);
				return;
			}
		}
	}
	MapCopyObj* pObj = g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(m_dwUin,m_dwUin,MCopyConfig::GetInstance().GetMCopy(404));
	if (!pObj)
	{
		SendWarnMsg("进入副本失败");
		return;
	}
	int total_lv = 0,total_count = 0;
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_pTeamData->m_xUser[i].pUser)
		{
			total_lv += m_pTeamData->m_xUser[i].pUser->GetWealth(UW_LV);
			total_count++;
			m_pTeamData->m_xUser[i].pUser->m_wOldMIndex = m_pTeamData->m_xUser[i].pUser->m_nMIndex;
			m_pTeamData->m_xUser[i].pUser->m_wOldPosX = m_pTeamData->m_xUser[i].pUser->GetPosX();
			m_pTeamData->m_xUser[i].pUser->m_wOldPosY = m_pTeamData->m_xUser[i].pUser->GetPosY();
			m_pTeamData->m_xUser[i].pUser->m_pMapCopy	= pObj;
			g_pGameCenter->m_pMapCopyCenter->JoinMCopy(m_pTeamData->m_xUser[i].dwUin,pObj);

			m_pTeamData->m_xUser[i].pUser->JumpTo(pObj->m_dwMapIndex,pObj->m_pMCBase->nStartX,pObj->m_pMCBase->nStartY,pObj->m_pMCBase->wBaseMap);
		}
	}
	total_lv = total_lv/total_count;
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_pTeamData->m_xUser[i].pUser)
		{
			m_pTeamData->m_xUser[i].pUser->SetFlag(24,total_lv);
		}
	}
	GetMapServerObj()->ChgMapEnemyPow(20+total_lv*3,total_lv);
}

UserData* UserData::GetTeamUser( int nPos )
{
	if (!m_pTeamData)
	{
		return nPos == 0?this:NULL;
	}
	return m_pTeamData->m_xUser[nPos].pUser;
}

LUAFUN int UserData::LearnMakeSkill( BYTE byType )
{
	return m_pMakeSkillData->LearnMakeSkill((MAKE_SKILL_TYPE)byType);
}

void UserData::_getPowerByTime( int nTime1,int nTime2 )
{
	int nPow1 = nTime1/600;
	int nPow2 = nTime2/600;
	if (nPow1> nPow2)
	{
		m_pMakeSkillData->AddPower(nPow1-nPow2);
		if (GetFortData()&&m_pFortData->m_pWarRankData->nRankPos!=-1)
		{
			
			int nPos = m_pFortData->m_pWarRankData->nRankPos>500?500:m_pFortData->m_pWarRankData->nRankPos;
			int nVal	= nPow1 - nPow2;
			if(nVal > 2880)
				nVal	= 2880;
			int nScore	= (600-nPos) * (650-nPos)*nVal/5000;
			m_pFortData->AddWarScore(nScore);
		}
	}
}
bool CheckTP(int nTp,int nID){
	if (nID<=1004&&nTp == 12)	//12  91
	{
		return true;
	}
	if ((nID == 1005||nID == 1006)&&nTp == 14)	//14 同步推
	{
		return true;
	}
	if(nTp == 16)
	{
		if (nID ==1013 || nID==1014 )
		{
			return true;
		}
	}
	if (nTp == 21)				//21 口袋巴士
	{
		if (nID == 1007||nID == 1008)
		{
			return true;
		}
	}
	if (nID >=1009 &&nID<=1012 || nID >= 1015)
	{
		return true;
	}
	return false;
}
void UserData::GetActiveItem( char* szName )
{
	QueryRlt xRlt;
	if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "ACTIVATE_TABLE" where id='%s'",szName))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if(pCmd&&pCmd->FetchRow())
	{
		int nType			= pCmd->GetDWORD("type");
		int nState		= pCmd->GetDWORD("state");
		if (!CheckTP(m_nTPID,nType))
		{
			SendWarnMsg("激活码不存在");
			return;
		}
		if (nState == ACTIVE_USERD)
		{
			SendWarnMsg("激活码已经使用");
			return;
		}
		ActiveItem* pItem = ActiveConfig::GetInstance().GetItem(nType);
		if (!pItem)
		{
			SendWarnMsg("error005,配置未找到");
			return;
		}

		if (pItem->nFlag)
		{
			if(GetFlag(pItem->nFlag)){
				SendWarnMsg("您已经领取过该类型的礼包");
				return;
			}
			SetFlag(pItem->nFlag,1);
		}
		m_pDB->ExecuteSQL(FALSE,"update "ACTIVATE_TABLE" set state=%d where id='%s'",ACTIVE_USERD,szName);

		m_pItemCenter->AddItemByID(pItem->nItem,1,0,ITEM_LOG_ACTIVE);
		SendWarnMsg("领取成功");
	}else{
		SendWarnMsg("激活码不存在");
	}
}

void UserData::_checkLogDB()
{
	QueryRlt xRlt;

	if(!g_pLogDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_RMB_LOG" WHERE uin = %d and addval>0", m_dwUin)) 
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd != NULL)
	{
		if(pCmd->FetchRow())
		{	
			m_bIsRecharge = false;
			return;
		}
	}
}

void UserData::SetMaxRankPos( int nPos )
{
	list<WarRankItem*> m_List;
	WarRankItemConfig::GetInstance().GetItem(m_nMaxRankPos,nPos,m_List);
	char sztxt[128];
	for (list<WarRankItem*>::iterator itor = m_List.begin();itor!=m_List.end();itor++)
	{
		SendWarnMsg("首次进入竞技场排行%d名,获得系统赠送的奖励,请稍后在邮件里查收",(*itor)->nPos);
		sprintf(sztxt,"首次进入竞技场排行%d名,获得系统赠送的奖励",(*itor)->nPos);
		g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,m_dwUin,"系统",sztxt,(*itor)->nMoney,(*itor)->nItem,(*itor)->nCnt,true);
		// 		AddWealth(UW_GOLD,(*itor)->nMoney);
		// 		m_pItemCenter->AddItemByID((*itor)->nItem,(*itor)->nCnt,true,ITEM_LOG_WARRANKPOS);
	}
	m_nMaxRankPos = nPos;
	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set maxrankpos=%d where uin=%d",nPos,m_dwUin);

}

void UserData::ChangeUserCountry( BYTE byCountry )
{
	if (byCountry == 1 || byCountry == 2)
	{
		if (m_bCheckChgCountry)
		{
			m_bCheckChgCountry = false;
			SendPopMsg("转国将花费3000W金币，若确定使用，请再次点击使用！");
			return;
		}

		if (m_xUWealth.GetWealth(UW_GOLD) < 30000000)
		{
			SendWarnMsg("金币不足！转国需花费3000W金币！");
			return;
		}
		if (m_pUserMentoring->m_pRelationList->Count)
		{
			SendWarnMsg("您未脱离师徒关系，转国失败。转国需花费3000W金币！");
			return;
		}
		if (m_pUserMarry->m_pRelationList->Count)
		{
			SendWarnMsg("您未脱离夫妻关系，转国失败。转国需花费3000W金币！");
			return;
		}
		if (m_pMySimple->wHomeID)
		{
			SendWarnMsg("您未退出军团，转国失败。转国需花费3000W金币！");
			return;
		}
		if (m_pTeamData)
		{
			SendWarnMsg("您未退出队伍，转国失败。转国需花费3000W金币！");
			return;
		}

		static int nQuestID[4] = {5200,5201,5400,5401};
		for (int i = 0; i < 4; i++)
		{
			m_pQuestCenter->RequestGiveUpQuest(nQuestID[i]);
		}

		m_bChangeCountry = true;
		m_pItemCenter->RemoveItemByID(45015);
		m_xUWealth.SubWealth(UW_GOLD,30000000);

		JumpTo(byCountry*100+1,1146,1310,byCountry*100+1);

		m_byCountry = byCountry;
		if (m_pMySimple)
			m_pMySimple->bCountry = byCountry;
		SetRevivePos(byCountry*100+1,1146, 1310);

		m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set country=%d where Uin=%d",byCountry,m_dwUin);
		TCP_SendCmdToUin(m_dwUin,USER,C_CHANGE_COUNTRY);
	}
}

bool UserData::ChangeUserName(char* szName)
{
	if(szName == NULL)
	{
		SendWarnMsg("未输入字符");
		return false;
	}
	if(m_pItemCenter->GetItemCountByID(45016) == 0)
	{
		SendWarnMsg("物品不足");
		return false;
	}

	if(strlen(szName)< 4 || strlen(szName) > 12)
	{
		SendWarnMsg("长度不符");
		return false;
	}
	QueryRlt xRlt;
	if(m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_USER_TABLE" WHERE Nickname = '%s'", szName)) 
	{
		MySqlQuery* pCmd = xRlt.GetRlt();
		if (pCmd != NULL)
		{
			if(pCmd->FetchRow())
			{
				SendWarnMsg("角色名已存在");
				return false;
			}
		}
	}

	sprintf(m_szNick,szName);
	if(m_pDB->ExecuteSQL(false,"update "RPG_USER_TABLE" set Nickname='%s' where uin = %d",m_szNick,m_dwUin))
	{
		m_pDB->ExecuteSQL(false,"update "RPG_RANK_WAR_TABLE" set name='%s' where uin = %d",m_szNick,m_dwUin);
		UpdateSimpleInfo();
		UpdateFriendMsg();
		m_pItemCenter->RemoveItemByID(45016);
		return true;
	}
	return false;
}

void UserData::SendGSCloseWait(DWORD dwWaitSec)
{
	BuildPacketEx(pPacket, USER, S_SEND_CLOSE_WAITE,buf, 256);
	SET_DATA(pSend, USER, S_SEND_CLOSE_WAITE, pPacket);
	pSend->dwWaitSec = dwWaitSec;
	TCP_SendPacketToUin(m_dwUin,pPacket);
}

bool UserData::CheckSkillLvUp(int nID, int nLv)
{
	SkillObj* pSObj = m_pSkillCenter->GetSkill(nID);
	if (pSObj == NULL)
	{
		SendPopMsg("学习技能失败，未装备该技能");
		return false;
	}

	if (pSObj->bySLv +1 == nLv)
	{
		if(m_pSkillCenter->RequestSkillUp(nID,false))
		{
			PlayEffect(nEffectSuccess);
			return true;
		}
		else
			SendPopMsg("学习技能失败");
	}else
	{
		if (pSObj->bySLv +1 > nLv)
			SendPopMsg("已学习该技能");
		else
			SendPopMsg("技能等级不足，请先学习前一级技能");
	}

	return false;
}

void UserData::RecordLoginOut(DWORD dwUin,BYTE byType)
{
	m_pDB->ExecuteSQL(FALSE,"insert into "MARK_LOGINOUT_TABLE" (uin,lv,type,logtime) values (%d,%d,%d,now())",
			m_dwUin,m_pMySimple->bLv,byType);
}

void UserData::TryUpdate()
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
		char	szDmpMsg[256];
		sprintf(szDmpMsg,"UserData TryUpdate Error: %d\n",m_dwUin);
		DUMP_STRING(szDmpMsg);
	}
#endif
}

void UserData::SetLoginAwarded(bool bGet)
{
	if (bGet)
	{
		m_bLoginAwarded = true;
		m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set loginawarded=1 where Uin=%d",m_dwUin);
	}else
	{
		m_bLoginAwarded = false;
		m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set loginawarded=0 where Uin=%d",m_dwUin);
	}
}

int UserData::GetLifeStarLvPvpDef()
{
	int nPerVal = 0;
	switch(m_xUWealth.GetWealth(UW_LIFE_STARLV))
	{
	case 0:
		nPerVal = 0;
		break;
	case 1:
		nPerVal = 10;
		break;
	case 2:
		nPerVal = 20;
		break;
	case 3:
		nPerVal = 35;
		break;
	case 4:
		nPerVal = 55;
		break;
	case 5:
		nPerVal = 80;
		break;
	case 6:
		nPerVal = 110;
		break;
	case 7:
		nPerVal = 145;
		break;
	case 8:
		nPerVal = 185;
		break;
	case 9:
		nPerVal = 230;
		break;
	case 10:
		nPerVal = 280;
		break;
	}
	if (m_xUWealth.GetWealth(UW_VIPLV) == 11)
		nPerVal += 150;
	if( m_xUWealth.GetWealth(UW_VIPLV) == 12)
		nPerVal += 300;
	return nPerVal;
}

void UserData::UpdateFriendMsg()
{
	for (int i = 0; i < m_pFriendList->m_pList->Count; i++)
	{
		FriendObj* pObj = (FriendObj*)m_pFriendList->m_pList->Items[i];
		if (pObj && pObj->dwUinB)
		{
			UserData* pUserB = g_pGameCenter->GetPlayer(pObj->dwUinB);
			if (pUserB)
				pUserB->m_pFriendList->SendFriendChangeMsg(m_dwUin, m_szNick);
		}
	}
}

