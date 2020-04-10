#include "stdafx.h"
#include "FamilyCenter.h"
#include "ios_home_packet.h"
#include "UserList.h"
#include "DBControl.h"
#include "ZNetGamesvr.h"
#include "UserData.h"
#include "HomeBattleCenter.h"
#include "FsGameCenter.h"
#include "MailCenter.h"
#include "TimeControl.h"
static int snDay[7][2] = {{0,1},{2,3},{4,5},{6,0},{1,2},{3,4},{5,6}};
extern char* szCountryname[];
HomeCenter::HomeCenter()
{
	m_pList		= xnList::Create();
	m_pListBattleResult = xnList::Create();
	memset(&m_Battle,0,sizeof(LeagueBattle));
	m_wMaxHomeID = 0;
}

HomeCenter::~HomeCenter()
{
	while(m_pList->Count)
		delete (HomeObj*)m_pList->Delete(0);

	m_pList->Free();
	while(m_pListBattleResult->Count){
		xnList* pList = (xnList*) m_pListBattleResult->Items[0];
		while(pList->Count){
			delete pList->Delete(0);
		}
		delete m_pListBattleResult->Delete(0);
	}
}

void HomeCenter::DBInit( DBControl* pDB )
{
	m_pDB	= pDB;
	QueryRlt xRlt;
	if(!pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_HOME_TABLE))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		HomeObj*	pHome	= new HomeObj();
		pHome->m_wHomeID		= pCmd->GetWORD("homeid");
		m_wMaxHomeID	= max(pHome->m_wHomeID,m_wMaxHomeID);
		pHome->m_dwCreateUin	= pCmd->GetDWORD("createuin");

		pHome->m_byLv			= pCmd->GetInt("lvl");
		pHome->m_dwExp			= pCmd->GetDWORD("exp");
		strcpy(pHome->m_szCreateName,pCmd->GetStr("createname"));
		pHome->m_byCountry = pCmd->GetInt("country");
		strcpy(pHome->m_szName,pCmd->GetStr("fname"));
		strcpy(pHome->m_szDesc,pCmd->GetStr("fdesc"));

		int nSize = 0;
		BYTE* pData	= pCmd->GetBLOB("build",&nSize);
		pHome->LoadBuildTag(pData,nSize);
		m_pList->Add(pHome);
		pHome->m_ifMemberCh = true;
	}
	m_wMaxHomeID++;

	//void* pData = NULL;
	//int nLen = 0;
	//pData = WorldFlag::GetInstance().GetFlag(WORLD_FLAG_LEAGUE_BATTLE,&nLen);
	//if (pData&&nLen&&nLen == sizeof(LeagueBattle))
	//{
	//	memcpy(&m_Battle,pData,nLen);
	//	for (int i=0;i<7;i++)
	//	{
	//		m_Battle.Pos[i].pHome = NULL;
	//	}
	//}

}

void HomeCenter::_netHomeExit( DWORD dwUin )
{
	TCP_SendCmdToUin(dwUin,NHOME,C_EXIT);
}

bool HomeCenter::RecvPacket( ZPacket* pPacket, DWORD dwUin )
{
	if(pPacket->bCmdGroup	!= NHOME)
		return false;

	switch(pPacket->bCmd)
	{
	case C_ACCEPT:
		{
			SET_DATA(pData, NHOME,C_ACCEPT,pPacket);
			UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
			UserData* pQUser = g_pGameCenter->GetPlayer(pData->dwUin);
			if (!pUser)
			{
				//pUser->SendWarnMsg("未知错误001");
				return true;
			}
			HomeObj* pHome	= _getHomeObj(pUser->m_dwHomeID);
			if(pHome&&pData->wResult)
			{

				
				
				UserSimple* pUSOp	= g_pUserList->GetUser(dwUin);
				UserSimple* pUS		= g_pUserList->GetUser(pData->dwUin);

				if(!pUS || !pUSOp)
				{
					//pUser->SendWarnMsg("未知错误002");
					return true;
				}
				if (pUS->wHomeID)
				{
					pUser->SendWarnMsg("对方已有军团");
					return true;
				}
				if(pUSOp->byHomeDuty < HOME_DUTY_ADV){
					pUser->SendWarnMsg("你的权限不足");
					return true;
				}
				
				if(pHome->AcceptRequest(pUS))
				{
					if (pQUser)
					{
						pQUser->SendWarnMsg("成功加入了%s军团", pHome->m_szName);
						ChatCenter::GetInstance().RegChatObj(CHAT_ROOM_HOME,pHome->m_wHomeID,pQUser);
					}
					pUS->wHomeID	= pHome->m_wHomeID;
					pUS->byHomeDuty	= HOME_DUTY_NORMAL;
					_dbUpdateMember(pUS);
					_netMemberAdd(pHome,pUS);
					_netHomeReqer(pHome,dwUin);
					UserData* pUser = g_pGameCenter->GetPlayer(pData->dwUin);
					if (pUser)
					{
						pUser->m_dwHomeID = pHome->m_wHomeID;
						pUser->m_dwHomeDuty = HOME_DUTY_NORMAL;
						_netHomeInfo(pHome,pData->dwUin);
					}
					char sztxt[128];
					sprintf(sztxt,"%s加入了军团", pUS->szName);
					ChatCenter::GetInstance().SendSystemMsg(sztxt,CHAT_ROOM_HOME,pHome->m_wHomeID,1,false);

					pUS->RemoveAllHome();
				}else
					pUser->SendWarnMsg("军团人数已满。");
				
			}else if (pHome)
			{
				if (pQUser)
				{
					pQUser->SendWarnMsg("%s拒绝了你的军团申请", pHome->m_szName);
				}

				UserSimple* pUS		= g_pUserList->GetUser(pData->dwUin);
				if(pUS)
				{
					pHome->RemoveRequest(pUS);
					pUS->RemoveHome(pHome);
					_netHomeReqer(pHome,dwUin);
				}

			}
		}
		break;
	case C_ADD_BUILD_LV:
		{
			SET_DATA(pData, NHOME,C_ADD_BUILD_LV,pPacket);
			UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
			if (!pUser)
			{
				return true;
			}
			if (pUser->m_dwHomeDuty < HOME_DUTY_MNG)
			{
				pUser->SendWarnMsg("权限不足，只有军团团长和副团长才可进行操作。");
				return true;
			}
			HomeObj*	pHome	= _getHomeObj(pUser->m_dwHomeID);
			if (pHome)
			{
				if (pData->byType == 1)
				{
					if(pHome->AddBuildLv(HOME_BUILD_RESEARCH)){
						_dbUpdateHomeBuild(pHome);
						_netHomeBuild(pHome);
						_dbUpdateHomeSimple(pHome);
						_netHomeInfo(pHome,dwUin);
						pUser->PlayEffect(nEffectSuccess);
					}
				}else if (pData->byType == 0)
				{
					if(pHome->AddHomeLv()){
						_dbUpdateHomeSimple(pHome);
						_netHomeInfo(pHome,dwUin);
						pUser->PlayEffect(nEffectSuccess);
					}
				}
				
			}
		}
		break;
	case C_RQLIST:
		{
			UserSimple* pUS	= g_pUserList->GetUser(dwUin);
			if(pUS)
			{
				HomeObj*	pHome	= _getHomeObj(pUS->wHomeID);
				if(pHome)
				{

					_netHomeReqer(pHome,pUS->dwUin);
				}
			}
		}
		break;
	case C_CREATE:
		{
			SET_DATA(pData, NHOME,C_CREATE,pPacket);
			UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
			if (!pUser/*||!pUser->m_pLuckData->CanCreateLeague()*/)
			{
				return true;
			}
// 			if(pUser->m_xUWealth.GetWealth(UW_LV)<20){
// 				return true;
// 			}
			for (int i = 0; i < m_pList->Count; i++)
			{
				HomeObj* pHome	= (HomeObj*)m_pList->Items[i];
				if (!strcmp(pHome->m_szName,pData->szName))
				{
					pUser->SendWarnMsg("已有相同名字的军团");
					return true;
				}
			}
			if(pUser && !pUser->m_dwHomeID)
			{
				if (pUser->GetFlag(18)>=CMTime::GetCurTickCount())
				{
					pUser->SendWarnMsg("退出军团后24小时才能创建军团");
					return true;
				}
				if (pUser->m_xUWealth.GetWealth(UW_LV)<30)
				{
					pUser->SendPopMsg("需要等级达到30级");
					return true;
				}
 				if (!pData->bUseRmb&&!pUser->m_xUWealth.SubWealth(UW_GOLD,1000000))
 				{
					pUser->SendPopMsg("金币不足1000000");
 					return true;
 				}
				if (pData->bUseRmb&&!pUser->m_xUWealth.SubWealth(UW_RMB,100,RMB_SAVE_FAMILY))
				{
					pUser->SendPopMsg("钻石不足100");
					return true;
				}
				CreateHome(pData->szName,pUser->m_dwUin);
				pUser->m_dwHomeID = m_wMaxHomeID-1;
				char sztxt[128];
				sprintf(sztxt,"[%s]%s成功创建 %s 军团，20级以上玩家可通过社交→军团界面申请加入哦！",szCountryname[pUser->m_byCountry],pUser->m_szNick,pData->szName);
				ChatCenter::GetInstance().SendSystemMsg(sztxt,0,0,1,true);
				TCP_SendCmdToUin(pUser->m_dwUin,NHOME,C_CREATE);
			}
			
		}
		break;
	case C_LEAGUE_BATTLE:
		{
			SET_DATA(pData, NHOME,C_LEAGUE_BATTLE,pPacket);
			
			UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
			if (pUser->m_dwHomeDuty != HOME_DUTY_BOSS)
			{
				return true;
			}
			tm ctime = CMTime::GetCurTime();
			
			if (pData->nPos!=snDay[ctime.tm_wday][0]&&pData->nPos!=snDay[ctime.tm_wday][1])
			{
				pUser->SendWarnMsg("当前不能攻击此关卡");
				return true;
			}
			if (
#ifdef DEBUG
				1
#else
				ctime.tm_hour == 20&&ctime.tm_min>30
#endif
				)
			{
				HomeObj*	pHome	= _getHomeObj(pUser->m_dwHomeID);
				if (pHome->GetOccupyCount()>=2)
				{
					pUser->SendWarnMsg("最多只能占领2个关卡");
					return true;
				}

			}
			
		}
		break;
	case C_LEAGUE_BATTLE_JOIN:
		{
			tm ctime = CMTime::GetCurTime();
#ifdef DEBUG
			UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
			if (!pUser)
			{
				return true;
			}

			HomeObj*	pHome	= _getHomeObj(pUser->m_dwHomeID);
			if (pHome)
			{
				AddLeagueBattleObj(pHome,pUser);
				pUser->SendWarnMsg("报名成功");
				TCP_SendCmdToUin(dwUin,NHOME,C_LEAGUE_BATTLE_JOIN);
			}
			return true;
#endif
			if (ctime.tm_hour == 20&&ctime.tm_min <=30)
			{
				UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
				if (!pUser)
				{
					return true;
				}

				HomeObj*	pHome	= _getHomeObj(pUser->m_dwHomeID);
				if (pHome)
				{
					AddLeagueBattleObj(pHome,pUser);
					pUser->SendWarnMsg("报名成功");

				}
			}else if(ctime.tm_hour == 20){
				TCP_SendCmdToUin(dwUin,NHOME,C_LEAGUE_BATTLE_JOIN);
			}
			
		}
		break;
	case C_RQHOMES:
		{
			UserSimple* pUS	= g_pUserList->GetUser(dwUin);
			if(pUS)
			{
				_netHomeList(pUS->dwUin);
			}
		}
		break;
	case C_GETHOMEINFO:
		{
			UserSimple* pUS	= g_pUserList->GetUser(dwUin);
			if(pUS)
			{
				HomeObj* pHome	= _getHomeObj(pUS->wHomeID);
				if(pHome) _netHomeInfo(pHome,pUS->dwUin);
			}
		}
		break;
	case C_REQUEST:
		{
			SET_DATA(pData, NHOME,C_REQUEST,pPacket);
			HomeObj* pHome	= _getHomeObj(pData->wHomeID);
			if(pHome)
			{
				UserSimple* pUSOp	= g_pUserList->GetUser(dwUin);
				
				if(!pUSOp)
					return true;
				
				UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
				if (!pUser)
				{
					return true;
				}
				if (pHome->m_byCountry != pUSOp->bCountry)
				{
					pUser->SendWarnMsg("只能加入自己国家的军团");
					return true;
				}
				
				if (pUSOp->bLv<20)
				{
					pUser->SendWarnMsg("20级才能加入军团");
					return true;
				}
				if (pUser->m_dwHomeID)
				{
					pUser->SendWarnMsg("你已经加入了一个军团");
					return true;
				}
				if (pUser->GetFlag(18)>=CMTime::GetCurTickCount())
				{
					pUser->SendWarnMsg("退出军团后24小时才能加入军团");
					return true;
				}
				bool bJoin = pHome->RequestJoin(pUSOp);
				if (!bJoin)
				{
					pUser->SendWarnMsg("你已经申请了该军团或该军团申请人数到达上限");
				}else{
					_netHomeNewRequest(pHome);
					pUSOp->pHomeList->Add(pHome);
					pUser->SendWarnMsg("你已对该军团发送了申请");     //===zjk
					BuildPacketEx(pPacket, NHOME, S_REQUEST_RESULT, buf, 4096);
					SET_DATA(pData1, NHOME, S_REQUEST_RESULT, pPacket);
					pData1->wHomeID = pData->wHomeID;
					TCP_SendPacketToUin(dwUin,pPacket);
				}
			}
		}
		break;
	case C_CANCEL_REQUEST:
		{
			SET_DATA(pData, NHOME,C_CANCEL_REQUEST,pPacket);
			HomeObj* pHome	= _getHomeObj(pData->wHomeID);
			if(pHome)
			{
				UserSimple* pUSOp	= g_pUserList->GetUser(dwUin);

				if(!pUSOp)
					return true;

				UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
				if (!pUser)
				{
					return true;
				}
				bool bJoin = pHome->RequestCancelJoin(pUSOp);
				if (bJoin)
				{
					pUSOp->RemoveHome(pHome);
					BuildPacketEx(pPacket, NHOME, S_REQUEST_CANCEL_RESULT, buf, 4096);
					SET_DATA(pData1, NHOME, S_REQUEST_CANCEL_RESULT, pPacket);
					pData1->wHomeID = pData->wHomeID;
					TCP_SendPacketToUin(dwUin,pPacket);
				}
			}
		}
		break;
	case C_GET_HOMEBATTLEINFO:
		{
			SET_DATA(pData, NHOME,C_GET_HOMEBATTLEINFO,pPacket);
			UserSimple* pUS	= g_pUserList->GetUser(dwUin);
			if(pUS)
			{
				HomeObj* pHome	= _getHomeObj(pUS->wHomeID);
				if(pHome->m_pBattleResult) SendBattleInfo(pHome->m_pBattleResult,dwUin,pData->nPos);
			}
		}
		break;
	case C_SWITCH_BATTLE_POS:
		{
			SET_DATA(pData, NHOME,C_SWITCH_BATTLE_POS,pPacket);
			UserSimple* pUS	= g_pUserList->GetUser(dwUin);
			if(pUS)
			{
				HomeObj* pHome	= _getHomeObj(pUS->wHomeID);
				if (!pHome)
				{
					return true;
				}
				if (!pData->nPos1||!pData->nPos2||pData->nPos1 == pData->nPos2)
				{
					return true;
				}
				HomeBattleObj* pObj1=NULL,*pObj2 = NULL;
				for (int i=0;i<pHome->m_pListBattle->Count;i++)
				{
					HomeBattleObj* pObj = (HomeBattleObj*)pHome->m_pListBattle->Items[i];
					if (pObj->nPos == pData->nPos1)
					{
						pObj1 = pObj;
					}
					if (pObj->nPos == pData->nPos2)
					{
						pObj2 = pObj;
					}
				}
				if(pObj1) pObj1->nPos = pData->nPos2;
				if(pObj2) pObj2->nPos = pData->nPos1;

// 				BuildPacketEx(pPacket, NHOME, C_SWITCH_BATTLE_POS, buf, 4096);
// 				SET_DATA(pData1, NHOME, C_SWITCH_BATTLE_POS, pPacket);
// 				
				TCP_SendPacketToUin(dwUin,pPacket);
			}
		}
		break;
	case C_SET_BATTLE_POS:
		{
			SET_DATA(pData, NHOME,C_SET_BATTLE_POS,pPacket);
			UserSimple* pUS	= g_pUserList->GetUser(dwUin);
			if(pUS)
			{
				HomeObj* pHome	= _getHomeObj(pUS->wHomeID);
				if (!pHome)
				{
					return true;
				}
				for (int i=0;i<pHome->m_pListBattle->Count;i++)
				{
					HomeBattleObj* pObj = (HomeBattleObj*)pHome->m_pListBattle->Items[i];
					if (pObj->dwUin == pData->dwUin)
					{
						pObj->nPos = pData->nPos;
						TCP_SendPacketToUin(dwUin,pPacket);	
						return true;
					}
				}
				
				
			}
		}
		break;
	case C_GET_BATTLE_ACTOR:
		{
			UserSimple* pUS	= g_pUserList->GetUser(dwUin);
			if(pUS)
			{
				HomeObj* pHome	= _getHomeObj(pUS->wHomeID);
				if (!pHome)
				{
					return true;
				}
				BuildPacketEx(pPacket, NHOME, S_BATTLE_ACTOR, buf, 4096);
				SET_DATA(pData, NHOME, S_BATTLE_ACTOR, pPacket);
				if (!pHome->m_pListBattle->Count)
				{
					HomeBattleObj* pAtk[16]={NULL};
					for (int i=0;i<16;i++)
					{
						pAtk[i] = new HomeBattleObj;
						pAtk[i]->nPos = i;
						pAtk[i]->nMaxHp = 15;
						pAtk[i]->nNowHp = 15;
						pAtk[i]->nAtkMin = 10;
						pAtk[i]->nAtkMax = 15;
						pAtk[i]->nDefMin = 5;
						pAtk[i]->nDefMax = 5;
						pHome->m_pListBattle->Add(pAtk[i]);
					}
					
				}
				pData->nCount = pHome->m_pListBattle->Count;
				pPacket->wSize += sizeof(HomeBattleObj)*pData->nCount;
				for (int i=0;i<pHome->m_pListBattle->Count;i++)
				{
					HomeBattleObj* pObj = (HomeBattleObj*)pHome->m_pListBattle->Items[i];
					pData->BattleObj[i] = *pObj;
				}
				TCP_SendPacketToUin(dwUin,pPacket);
			}
		}
		break;
	case C_EXIT:
		{
			SET_DATA(pData, NHOME,C_EXIT,pPacket);
			UserSimple* pUSOp	= g_pUserList->GetUser(dwUin);
			if(!pUSOp )
				return true;
			HomeObj* pHome	= _getHomeObj(pUSOp->wHomeID);
			if(pHome)
			{
				
				
				UserSimple* pUS	= NULL;
				if(dwUin != pData->dwUin)
				{
					if(pUSOp->byHomeDuty < HOME_DUTY_MNG)
						return true;

					pUS	= g_pUserList->GetUser(pData->dwUin);
					
				}
				else
					pUS	= pUSOp;
				
				if(!pUS)
					return true;
				if(pUS != pUSOp && pUS->byHomeDuty >=pUSOp->byHomeDuty)
				{
					return true;
				}
				
				if(pUS->byHomeDuty == HOME_DUTY_BOSS)
					return true;

				if(pHome->RemoveMember(pUS))
				{
					pUS->byHomeDuty	= 0;
					pUS->wHomeID	= 0;
					//存数据库, 
					
					_dbUpdateMember(pUS);

					//发客户端
					if(dwUin != pData->dwUin){
						_netHomeInfo(pHome,dwUin);
					}
					_netHomeExit(pData->dwUin);

					UserData* pUser1 = g_pGameCenter->GetPlayer(pData->dwUin);
					if (pUser1)
					{
						if (pUser1->m_dwUin == dwUin)
						{
							pUser1->SetFlag(18,CMTime::GetCurTickCount()+24*3600);
						}
						pUser1->m_dwHomeID = 0;
						pUser1->m_dwHomeDuty = 0;
						ChatCenter::GetInstance().UnRegChatObj(pUser1->m_pChatRoom[CHAT_ROOM_HOME],pUser1);
					}
					char sztxt[128];
					sprintf(sztxt,"%s离开了军团", pUS->szName);

					ChatCenter::GetInstance().SendSystemMsg(sztxt,CHAT_ROOM_HOME,pHome->m_wHomeID,1,false);
				}

			}
		}
		break;
	case C_DISSOLVE_HOME:
		{
			UserSimple* pUSOp	= g_pUserList->GetUser(dwUin);
			if(!pUSOp )
				return true;
			HomeObj* pHome	= _getHomeObj(pUSOp->wHomeID);
			if(pHome)
			{
				if(pUSOp->byHomeDuty != HOME_DUTY_BOSS)
					return true;
				if(DissolveHome(pHome))
				{
					UserData* pUser1 = g_pGameCenter->GetPlayer(dwUin);
					if (pUser1)
					{
						pUser1->SendPopMsg("公会成功解散");
						pUser1->SetFlag(18,CMTime::GetCurTickCount()+24*3600);
					}
				}
			}
		}
		break;
	case C_DUTY:
		{
			SET_DATA(pData, NHOME,C_DUTY,pPacket);
			UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
			HomeObj* pHome	= _getHomeObj(pUser->m_dwHomeID);
			if (dwUin == pData->dwUin)
			{
				return true;
			}
			if(pHome&&pUser)
			{
				UserSimple* pUSOp	= g_pUserList->GetUser(dwUin);
				if(!pUSOp )
					return true;
				UserSimple* pUS		= g_pUserList->GetUser(pData->dwUin);
		
				if(!pUS)
					return true;
				if (pData->byDuty == HOME_DUTY_BOSS)
				{
					if (pUSOp->byHomeDuty != HOME_DUTY_BOSS)
					{
						pUser->SendWarnMsg("你不是团长");
						return true;
					}
					pUSOp->byHomeDuty = HOME_DUTY_NORMAL;
					pUS->byHomeDuty	= HOME_DUTY_BOSS;
					pUser->m_dwHomeDuty = HOME_DUTY_NORMAL;
					//保存同步
					_dbUpdateMember(pUS);
					_dbUpdateMember(pUSOp);
					_netMemberDuty(pHome,pUS);
					_netMemberDuty(pHome,pUSOp);
					return true;
				}
				//检查权限
				if(pUSOp->byHomeDuty <= pUS->byHomeDuty || pUSOp->byHomeDuty <= pData->byDuty){
					pUser->SendWarnMsg("权限不足");
					return true;
				}
				if (pData->byDuty == HOME_DUTY_ADV&&pHome->GetDutyCount(HOME_DUTY_ADV)>=5)
				{
					pUser->SendWarnMsg("最多只能有5个元老");
					return true;
				}else if (pData->byDuty == HOME_DUTY_MNG&&pHome->GetDutyCount(HOME_DUTY_MNG)>=1)
				{
					pUser->SendWarnMsg("最多只能有1个副官");
					return true;
				}
				pUS->byHomeDuty	= pData->byDuty;
				//保存同步
				_dbUpdateMember(pUS);
				_netMemberDuty(pHome,pUS);

				static char* szDutyName[] = {"成员","元老","副官","团长"};
				char sztxt[128];
				sprintf(sztxt,"%s职位更改为%s", pUS->szName,szDutyName[pUS->byHomeDuty]);
				ChatCenter::GetInstance().SendSystemMsg(sztxt,CHAT_ROOM_HOME,pHome->m_wHomeID,1,false);

			}
		}
		break;
	case C_CHANGE_LEAGUE_DESC:
		{
			SET_DATA(pData, NHOME,C_CHANGE_LEAGUE_DESC,pPacket);
			UserSimple* pUSOp	= g_pUserList->GetUser(dwUin);
			if(!pUSOp )
				return true;
			HomeObj* pHome	= _getHomeObj(pUSOp->wHomeID);
			if (!pHome)
			{
				return true;
			}
			if (pUSOp->byHomeDuty == HOME_DUTY_NORMAL)
			{
				return true;
			}
			strcpy(pHome->m_szDesc,pData->szDesc);
			_netHomeInfo(pHome,pUSOp->dwUin);
			_dbUpdateHome(pHome);
			UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
			if (!pUser)
			{
				return true;
			}
			pUser->SendWarnMsg("修改公告成功");
		}
		break;
	case C_CONTRIBUTION:
		{
			UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
			if (!pUser)
			{
				return true;
			}

			HomeObj*	pHome	= _getHomeObj(pUser->m_dwHomeID);
			if (pHome)
			{
				if (!pUser->m_xUWealth.SubWealth(UW_RMB,100,RMB_SAVE_FAMILY))
				{
					pUser->SendWarnMsg("所需元宝不足");
					return true;
				}
				
				pHome->AddHomeExp(10);
				_netHomeChg(pHome);
				_dbUpdateHomeSimple(pHome);
				
				
			}
		}
		break;
	}
	return true;
}

HomeObj* HomeCenter::_getHomeObj( WORD wHome )
{
	for(int i = 0; i < m_pList->Count; i++)
	{
		HomeObj*	pHome	= (HomeObj*)m_pList->Items[i];
		if(pHome->m_wHomeID == wHome)
		{
			return pHome;
		}
	}
	return NULL;
}

void HomeCenter::_dbUpdateMember( UserSimple* pUS )
{
	m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set homeid=%d,homeduty=%d where uin=%d",pUS->wHomeID,pUS->byHomeDuty,pUS->dwUin);
}

void HomeCenter::_dbInsertHome( HomeObj* pHObj )
{
	m_pDB->ExecuteSQL(FALSE,"insert into "RPG_HOME_TABLE" (homeid, createuin, fname, lvl,exp,fdesc,build,createname,country) values (%d, %d, '%s',%d,%d,'','','%s',%d)", 
		pHObj->m_wHomeID,pHObj->m_dwCreateUin,pHObj->m_szName,pHObj->m_byLv,pHObj->m_dwExp,pHObj->m_szCreateName,pHObj->m_byCountry);
}

void HomeCenter::_dbUpdateHome( HomeObj* pHObj )
{
	m_pDB->ExecuteSQL(FALSE,"update "RPG_HOME_TABLE" set lvl=%d,exp=%d,fname='%s',fdesc='%s' where homeid=%d",pHObj->m_byLv,pHObj->m_dwExp,pHObj->m_szName,pHObj->m_szDesc,pHObj->m_wHomeID);
}

void HomeCenter::_dbUpdateHomeSimple( HomeObj* pHObj )
{
	m_pDB->ExecuteSQL(FALSE,"update "RPG_HOME_TABLE" set lvl=%d,exp=%d where homeid=%d",pHObj->m_byLv,pHObj->m_dwExp,pHObj->m_wHomeID);
}

void HomeCenter::CreateHome( char* szName,DWORD dwUin )
{
	UserSimple* pUS	= g_pUserList->GetUser(dwUin);
	if(!pUS)	return;
	HomeObj*	pHome	= new HomeObj();
	pHome->m_byLv			= 1;
	pHome->m_dwExp			= 0;
	pHome->m_dwCreateUin	= dwUin;
	pHome->m_wHomeID	= m_wMaxHomeID++;
	pHome->m_byCountry = pUS->bCountry;
	strcpy(pHome->m_szCreateName,pUS->szName);
	strcpy(pHome->m_szName,szName);
	strcpy(pHome->m_szDesc,"");
	pHome->m_pList->Add(pUS);
	UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
	if(pUser)
	{
		pUser->m_pUserRecord->RunLuaFamilyFunc(1,pHome->m_byLv);
		pHome->AddActiveMember(dwUin);
	}
	m_pList->Add(pHome);
	
	_dbInsertHome(pHome);
	_netHomeInfo(pHome,dwUin);

	//
	pUS->byHomeDuty	= HOME_DUTY_BOSS;
	pUS->wHomeID	= pHome->m_wHomeID;
	pHome->m_ifLvUp = true;
	pHome->m_ifMemberCh = true;

	_dbUpdateMember(pUS);
}

void HomeCenter::_netHomeInfo( HomeObj* pHObj,DWORD dwUin )
{
	BuildPacketEx(pPacket, NHOME, S_HOMEINFO, buf, 4096);
	SET_DATA(pData, NHOME, S_HOMEINFO, pPacket);
	strcpy(pData->szName,pHObj->m_szName);
	strcpy(pData->szDesc,pHObj->m_szDesc);
	pData->bLv	= pHObj->m_byLv;
	pData->dwExp	= pHObj->m_dwExp;
	pData->wCnt		= pHObj->m_pList->Count;
	pData->dwHomeID	= pHObj->m_wHomeID;
	pData->byHomeDuty = 0;
	pData->xBuild	= *pHObj->m_pBuildTag;
	pPacket->wSize	+= sizeof(NHomeMember)*pHObj->m_pList->Count;
	for (int i = 0; i < pHObj->m_pList->Count; i++)
	{
		UserSimple* pUS	= (UserSimple*)pHObj->m_pList->Items[i];
		pData->szMember[i].bLv		= pUS->bLv;
		pData->szMember[i].dwUin	= pUS->dwUin;
		pData->szMember[i].bDuty	= pUS->byHomeDuty;
		pData->szMember[i].dwContribute	= pUS->nContribution;
		pData->szMember[i].bJob	= pUS->bJob;
		pData->szMember[i].bOnline = pUS->bOnline;
		strcpy(pData->szMember[i].szName,pUS->szName);
		if (pUS->dwUin == dwUin)
		{
			pData->byHomeDuty = pUS->byHomeDuty;
		}
	}

	TCP_SendPacketToUin(dwUin,pPacket);
}

void HomeCenter::_netMemberAdd( HomeObj* pHObj,UserSimple* pUS )
{
	BuildPacketEx(pPacket, NHOME, S_HOMEMEMBERADD, buf, 256);
	SET_DATA(pData, NHOME, S_HOMEMEMBERADD, pPacket);
	pData->member.bLv		= pUS->bLv;
	pData->member.dwUin	= pUS->dwUin;
	strcpy(pData->member.szName,pUS->szName);
	TCP_SendPacketToUinList(pHObj->m_pListActiveUS,pPacket);
}

void HomeCenter::_netMemberOnline( HomeObj* pHObj,UserSimple* pUS )
{
	
}

void HomeCenter::_netMemberOffline( HomeObj* pHObj,UserSimple* pUS )
{

}

void HomeCenter::_netHomeChg( HomeObj* pHObj )
{
	BuildPacketEx(pPacket, NHOME, S_HOMESIMPLE, buf, 256);
	SET_DATA(pData, NHOME, S_HOMESIMPLE, pPacket);
	pData->bLv		= pHObj->m_byLv;
	pData->dwExp	= pHObj->m_dwExp;
	TCP_SendPacketToUinList(pHObj->m_pListActiveUS,pPacket);
}

void	HomeCenter::_netHomeDissolved(HomeObj* pHObj)
{
	TCP_SendCmdToUinList(pHObj->m_pListActiveUS,NHOME,S_HOME_DISSOLVED);
}

void HomeCenter::_netMemberDuty( HomeObj* pHObj,UserSimple* pUS )
{
	BuildPacketEx(pPacket, NHOME, S_HOMEDUTYCHG, buf, 256);
	SET_DATA(pData, NHOME, S_HOMEDUTYCHG, pPacket);
	pData->dwUin		= pUS->dwUin;
	pData->byDuty		= pUS->byHomeDuty;
	TCP_SendPacketToUinList(pHObj->m_pListActiveUS,pPacket);
}

void HomeCenter::_netHomeReqer( HomeObj* pHObj,DWORD dwUin )
{
	BuildPacketEx(pPacket, NHOME, S_HOMEREQER, buf, 2048);
	SET_DATA(pData, NHOME, S_HOMEREQER, pPacket);
	pData->nCnt = pHObj->m_pListRequest->Count;
	pPacket->wSize += sizeof(NHomeMember)*pData->nCnt;
	for (int i = 0; i < pHObj->m_pListRequest->Count && i < 10; i++)
	{
		UserSimple* pUS	= (UserSimple*)pHObj->m_pListRequest->Items[i];
		pData->szReqers[i].bLv		= pUS->bLv;
		pData->szReqers[i].dwUin	= pUS->dwUin;
		pData->szReqers[i].bJob		= pUS->bJob;
		strcpy(pData->szReqers[i].szName, pUS->szName);
	}
	TCP_SendPacketToUin(dwUin,pPacket);
}

void HomeCenter::_netHomeList(DWORD dwUin)
{
	BuildPacketEx(pPacket, NHOME, S_HOMELIST, buf, 2048);
	SET_DATA(pData, NHOME, S_HOMELIST, pPacket);
	//包分批发，避免包超了
	int n = 0;
	pData->ifClean = true;
	for (int i = 0; i < m_pList->Count; i++)
	{
		HomeObj* pHome	= (HomeObj*)m_pList->Items[i];

		pData->szHome[n].bLv		= pHome->m_byLv;
		pData->szHome[n].wHomeID	= pHome->m_wHomeID;
		pData->szHome[n].wMemCnt	= pHome->m_pList->Count;
		pData->szHome[n].bCountry	= pHome->m_byCountry;
		strcpy(pData->szHome[n].szName,pHome->m_szName);
		strcpy(pData->szHome[n].szCreateName,pHome->m_szCreateName);
		pData->szHome[n].bApply		= pHome->CheckJoin(dwUin);
		n++;
		if(n >=20)
		{
			pPacket->wSize += sizeof(NHomeSInfo)*n;
			pData->wCnt = n;
			TCP_SendPacketToUin(dwUin,pPacket);
			n = 0;
			pPacket->wSize = sizeof(NHOME_S_HOMELIST_Data);
			pData->ifClean = false;
		}
	}
	if(n > 0)
	{
		pPacket->wSize += sizeof(NHomeSInfo)*n;
		pData->wCnt = n;
		TCP_SendPacketToUin(dwUin,pPacket);
	}

// 	pData->wCnt	= min(m_pList->Count,50);
// 	pPacket->wSize += sizeof(NHomeSInfo)*pData->wCnt;
// 	for (int i = 0; i < m_pList->Count && i < pData->wCnt; i++)
// 	{
// 		HomeObj* pHome	= (HomeObj*)m_pList->Items[i];
// 
// 		pData->szHome[i].bLv		= pHome->m_byLv;
// 		pData->szHome[i].wHomeID	= pHome->m_wHomeID;
// 		pData->szHome[i].wMemCnt	= pHome->m_pList->Count;
// 		pData->szHome[i].bCountry	= pHome->m_byCountry;
// 		strcpy(pData->szHome[i].szName,pHome->m_szName);
// 		strcpy(pData->szHome[i].szCreateName,pHome->m_szCreateName);
// 		pData->szHome[i].bApply		= pHome->CheckJoin(dwUin);
// 	}
// 	TCP_SendPacketToUin(dwUin,pPacket);
}

void HomeCenter::AddLeagueBattleObj( HomeObj* pHObj,UserData* pData )
{
	bool bPos[18] = {false};
	for (int i=0;i<pHObj->m_pListBattle->Count;i++)
	{
		HomeBattleObj* pObj = (HomeBattleObj*)pHObj->m_pListBattle->Items[i];
		if (pObj->dwUin == pData->m_dwUin)
		{
			delete pHObj->m_pListBattle->Delete(i);
		}else if(pObj->nPos){
			bPos[pObj->nPos-1] = true;
		}
	}
	//HomeBattleObj* pObj = pData->GetLeagueBattleObj();
	//for (int i=0;i<18;i++)
	//{
	//	if (!bPos[i])
	//	{
	//		pObj->nPos = i+1;
	//		break;
	//	}
	//}
	//pHObj->m_pListBattle->Add(pObj);
}

void HomeCenter::SendBattleInfo( xnList* pList ,DWORD dwUin,int nCount)
{
	if (nCount>=pList->Count)
	{
		BuildPacketEx(pPacket, NHOME, S_HOMEBATTLEINFO, buf, 4096);
		SET_DATA(pData, NHOME, S_HOMEBATTLEINFO, pPacket);
		pData->wCnt = 0;
		TCP_SendPacketToUin(dwUin,pPacket);

		return;
	}
	BattleStepInfo* pInfo = (BattleStepInfo*)pList->Items[nCount];
	BuildPacketEx(pPacket, NHOME, S_HOMEBATTLEINFO, buf, 4096);
	SET_DATA(pData, NHOME, S_HOMEBATTLEINFO, pPacket);
	pData->nPos = nCount;
	pData->wCnt	= pInfo->pBattleStep->Count;
	//回合数过多有问题
	pPacket->wSize += sizeof(BattleStep)*pData->wCnt;
	for (int i=0;i<6;i++)
	{
		pData->AtkObj[i] = pInfo->BattleObjAtk[i];
		pData->DefObj[i] = pInfo->BattleObjDef[i];
	}
	for (int i=0;i<pInfo->pBattleStep->Count;i++)
	{
		pData->Step[i] = (*(BattleStep*)pInfo->pBattleStep->Items[i]);
	}
	pData->byResult = pInfo->byResult;
	TCP_SendPacketToUin(dwUin,pPacket);
	
}

void HomeCenter::_sendBattleReward(int nDay)
{
	if (nDay>=7||nDay<0)
	{
		return;
	}
	
}
void HomeCenter::SendBattleDailyReward()
{
	for (int i=0;i<7;i++)
	{
		if (m_Battle.Pos[i].pHome)
		{
			for (int j=0;j<m_Battle.Pos[i].pHome->m_pList->Count;j++)
			{
				SimpleUserInfo* pObj = (SimpleUserInfo*)m_Battle.Pos[i].pHome->m_pList->Items[j];
				//g_pGameCenter->m_pMailCenter->RequestSendMail(0,pObj->dwUin,"关卡占领奖励","",0,1038,0);
			}
		}	
	}
}

void HomeCenter::UpdateSec()
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		HomeObj*	pHome	= (HomeObj*)m_pList->Items[i];
		if(pHome->m_ifLvUp || pHome->m_ifMemberCh)
		{
			pHome->m_ifLvUp = false;
			pHome->m_ifMemberCh = false;
			g_pGameCenter->m_pRankCenter->CheckRank(RANK_HOME,(void*)pHome,false);
		}
	}
}

void HomeCenter::UpdateMintue()
{
	tm ctime = CMTime::GetCurTime();
	if (ctime.tm_hour == 21&&ctime.tm_min == 0)
	{
		_sendBattleReward(ctime.tm_wday);
		CheckChangeLeader();
	}
}

char* HomeCenter::GetHomeName( WORD wHome )
{
	for(int i = 0; i < m_pList->Count; i++)
	{
		HomeObj*	pHome	= (HomeObj*)m_pList->Items[i];
		if(pHome->m_wHomeID == wHome)
		{
			return pHome->m_szName;
		}
	}
	return "";
}

void HomeCenter::_dbUpdateHomeBuild( HomeObj* pHObj )
{
	m_pDB->SetBLOB(FALSE,pHObj->m_pBuildTag,sizeof(HomeBuildTag),"update "RPG_HOME_TABLE" set build=? where homeid=%d",pHObj->m_wHomeID);

}

void HomeCenter::_netHomeBuild( HomeObj* pHObj )
{
	BuildPacketEx(pPacket, NHOME, S_BUILDINFO, buf, 256);
	SET_DATA(pData, NHOME, S_BUILDINFO, pPacket);
	pData->xBuild	= *pHObj->m_pBuildTag;
	TCP_SendPacketToUinList(pHObj->m_pListActiveUS,pPacket);
}

void HomeCenter::_netHomeNewRequest( HomeObj* pHObj )
{
	for (int i = 0; i < pHObj->m_pList->Count; i++)
	{
		UserSimple* pUS	= (UserSimple*)pHObj->m_pList->Items[i];
		if (pUS->byHomeDuty)
		{
			TCP_SendCmdToUin(pUS->dwUin,NHOME,S_NEWREQUEST);
		}
	}
}

void HomeCenter::AddHomeExp( WORD wHome,int nVal )
{
	HomeObj* pHome = _getHomeObj(wHome);
	if (pHome)
	{
		pHome->AddHomeExp(nVal);
		_dbUpdateHomeSimple(pHome);
	}
}

bool HomeCenter::CheckChangeLeader()
{

	std::list<HomeObj*> changelist;

	QueryRlt xRlt;
	if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_USER_TABLE" WHERE	homeduty = 3 AND UNIX_TIMESTAMP(LogoutTime) <UNIX_TIMESTAMP(now())-5*86400"))
		return false;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return false;

	while(pCmd->FetchRow())
	{
		WORD nHomeID		= pCmd->GetWORD("homeid");
		HomeObj*	pHome	= _getHomeObj(nHomeID);
		changelist.push_back(pHome);
	}
	for (std::list<HomeObj*>::iterator itor = changelist.begin();itor != changelist.end();itor++)
	{
		(*itor)->CheckChangeLeader();
	}
	return true;
}

bool	HomeCenter::DissolveHome(HomeObj* pHObj)
{
	if(pHObj == NULL)
		return false;
	while(pHObj->m_pList->Count)
	{
		UserSimple* pUS = (UserSimple*)pHObj->m_pList->Delete(0);
		if(pUS->byHomeDuty != HOME_DUTY_BOSS)
		{
			g_pGameCenter->m_pMailCenter->RequestSendMail(0,pUS->dwUin,"军团解散","您的军团已被团长解散！");
		}
		pUS->byHomeDuty	= 0;
		pUS->wHomeID	= 0;
		_dbUpdateMember(pUS);
		UserData* pUser1 = g_pGameCenter->GetPlayer(pUS->dwUin);
		if (pUser1)
		{
			pUser1->m_dwHomeID = 0;
			pUser1->m_dwHomeDuty = 0;
			ChatCenter::GetInstance().UnRegChatObj(pUser1->m_pChatRoom[CHAT_ROOM_HOME],pUser1);
		}
	}
	_netHomeDissolved(pHObj);
	while(pHObj->m_pListRequest->Count)
	{
		pHObj->m_pListRequest->Delete(0);
	}
	while(pHObj->m_pListActiveUS->Count)
	{
		pHObj->m_pListActiveUS->Delete(0);
	}
	while(pHObj->m_pListBattle->Count)
	{
		delete (HomeBattleObj*)pHObj->m_pListBattle->Delete(0);
	}
	if(m_pList->Remove(pHObj))
	{
		m_pDB->ExecuteSQL(FALSE,"delete from "RPG_HOME_TABLE" where homeid=%d",pHObj->m_wHomeID);
		delete pHObj;
		return true;
	}
	return false;
}
