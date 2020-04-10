#include "UserData.h"
#include "ios_user_packet.h"
#include "ios_ac_packet.h"
#include "GameScene.h"
#include "MainMenu.h"
#include "LoginScene.h"
#include "ReconnectGame.h"
#include "SoundCenter.h"
#include "ParticleCenter.h"
#include "GuideCenter.h"
#include "MapMouse.h"
#include "MapControl.h"
#include "ios_mapcmd_packet.h"
#include "MapStaticCfg.h"
#include "UserConfig.h"
#include "SkillConfig.h"
#include "FamilyCenter.h"
#include "EnemyConfig.h"
#include "MapCopyConfig.h"
#include "MallConfig.h"
#include "FaceConfig.h"
#include "VipConfig.h"
#include "ActivedegreeCfg.h"
#include "GameActivityCfg.h"
#include "GameAwardCfg.h"
#include "StarSysConfig.h"
#include "MentoringCfg.h"
#include "AchievementCfg.h"
#include "FaqHelpCfg.h"
#ifdef USE_ANDROID
#include "JniUtil.h"
#endif
#include "GameLoadScene.h"

UserData*	pChar	= NULL;
static pthread_t xLoadCfgThread;


extern void fun_cbMCopyTeam(void * pParam,int nVal);

static void* dealLoadCfg(void* data)
{
	UserData * pUser = (UserData *)data;
	if (pUser)
	{
		pUser->MuliThreadInitCfg();
	}
	return 0;
}

void fun_cbTeamMCopy(void * pParam,int nVal)
{
	if(nVal == 1)
	{	
		BuildPacketEx(pPacket, USER, C_JOINMAPCOPY, buf, 256);
		SET_DATA(pSend, USER, C_JOINMAPCOPY, pPacket);
		pSend->wMCID	= (int)pParam;
		pChar->SendPacket(pPacket);
	}
}

static void RequestTeamEvent(void* param,int nVal)
{
	if(nVal == 1)
	{		
		pChar->SendTeamState(TEAM_COMMIT_ACCEPT,(char*)NULL);
		pMainMenu->PopTipMsg("已加入该队伍");       //===zjk
	}else{		
		pChar->SendTeamState(TEAM_COMMIT_REFUSE,(char*)NULL);
		pMainMenu->PopTipMsg("已拒绝该邀请");
	}
}
void fun_cbFight(void * pParam,int nVal)
{
	BuildPacketEx(pPacket, USER, C_FIGHT_REQUEST,buf, 256);
	SET_DATA(pSend, USER, C_FIGHT_REQUEST, pPacket);
	pSend->byResult	= nVal;
	pChar->SendPacket(pPacket);
}

void ResUrlAskMsg(void* param,int nVal)
{
	if(nVal == 1)
	{
		g_packetCenter.DisConnectServer();
		pChar->InitData();

		GameLoadScene*	pLogin	= GameLoadScene::create();
		cocos2d::CCDirector::sharedDirector()->replaceScene(pLogin);
		g_pTPObj->nLockInUpdate	= 0;
		ReconnectModel::GetInstance().SetModelWorked(false);
		if (pMainMenu->m_bBackLogin)
		{
			pMainMenu->m_bBackLogin = false;
		}else
			pMainMenu->m_bBackLogin = true;	
	}
}

bool UserData::PacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup == ACCOUNT)
	{
		if(AccountPacketProcess(pPacket))
			return true;
	}

	if(pPacket->bCmdGroup != USER)
		return false;

	switch(pPacket->bCmd)
	{
	case S_USER_INFO:
		{
			SET_DATA(pRecv, USER, S_USER_INFO, pPacket);
			// 			m_dwHomeID	= pRecv->wHomeID;
			// 			m_dwHomeDuty = pRecv->byHomeDuty;
			// 			m_byVipLvl	= pRecv->byVipLvl;
			m_dwUin		= pRecv->dwUin;
			m_byJob		= pRecv->byJob;
			m_bySex		= pRecv->bySex;
			m_byCountry	= pRecv->byCountry;
			strncpy(m_szNick,pRecv->szNick,20);
			pMainMenu->m_pUserDataUpdate = true;
			char sztxt[32];
			sprintf(sztxt,"MB_S_MAN%d.png",m_byJob*2+(m_bySex?0:1)-1);
			pMainMenu->m_pCtrlMenu->m_pCompIco->SetViewSpr(sztxt);
			pMainMenu->m_pAutoMenu->LoadPersonalCfg();

#ifdef USE_ANDROID
			JniUtil jni;
			if(g_szBackKey && strlen(g_szBackKey))
				jni.TPSubmitUser(m_dwUin,g_nServerID,m_byLvl,m_szNick,g_szBackKey);
			else
				jni.TPSubmitUser(m_dwUin,g_nServerID,m_byLvl,m_szNick,g_szZoneName);
#endif
		}
		return true;
	case S_SEND_LUA_EVENT:
		{
			SET_DATA(pRecv,USER,S_SEND_LUA_EVENT,pPacket);
			ClientLuaManager::sharedHD()->PacketRecv(pRecv->szValue);
		}
		return true;
	case S_SEND_LOGIN_DAYS:
		{
			SET_DATA(pRecv, USER, S_SEND_LOGIN_DAYS, pPacket);
			m_nLoginDays = pRecv->nLoginD;
		}
		return true;
	case S_WEALTH_SINGLE:
		{
			SET_DATA(pRecv, USER, S_WEALTH_SINGLE, pPacket);
			switch(pRecv->byType)
			{
			case 0:
				m_dwRmb	= pRecv->dwVal;
				pMainMenu->m_pMallMenu->m_bMallUpdate = true;
				pMainMenu->m_pShopMenu->m_bShopUpdate = true;
				break;
			case 1:
				m_dwMoney	= pRecv->dwVal;
				pMainMenu->m_pShopMenu->m_bShopUpdate = true;
				break;
			case 2:
				{
					if (pRecv->dwVal>m_byLvl&&pChar->GetMyMapObj())
					{
						if(m_dwTickJoin && xnGetTickCount() - m_dwTickJoin > 8000)
						{
							GetMyMapObj()->AddEff(22);
							GetMyMapObj()->AddEff(23);
							pMainMenu->PlayEffect(269);
						}

						pMainMenu->m_pCtrlMenu->m_pCompLvUp->SetVisible((pChar->m_byLvl + 1) > 8);
					}
					m_byLvl		= pRecv->dwVal;
					pChar->m_pCQuestCenter->ReCheckCanAccept();
#ifdef USE_ANDROID
					_ResDownObj* pRes	= ResControl::GetInstance().GetByLv(m_byLvl);
					if(pRes)
					{
						SaveStringToXML("ResUrl1",pRes->szUrl);
						SaveToXMLOK();
						pMainMenu->PopAskMsgBox("为了您更好的继续游戏，请您下载后续资源包，是否立即下载?",ResUrlAskMsg,NULL);
					}
#endif
				}
				break;
			case 3:
				//if(pRecv->dwVal > m_dwExp)
				//	pMainMenu->m_pCtrlMenu->m_pCompExp->AddProgress(pRecv->dwVal-m_dwExp,UserConfig::GetInstance().GetExp(pChar->m_byLvl),300);
				//else
				m_dwExp		= pRecv->dwVal;
				break;
			case 4:
				m_dwPow		= pRecv->dwVal;
				pMainMenu->m_pMapCopyMenu->Update();
				break;
			case 5:
				m_wSkillPt	= pRecv->dwVal;
				break;
			case 6:
				{
					m_byVipLvl	= pRecv->dwVal;
					m_bVIPCh = true;
				}	
				break;
			case 7:
				{
					m_dwVipExp	= pRecv->dwVal;
					m_bVIPCh = true;
				}
				break;
			case 8:
				m_nGetSlaveLmt = pRecv->dwVal;
				break;
			case 9:
				{
					m_dwHonour	= pRecv->dwVal;
					m_pTitleObj	= TitleConfig::GetInstance().GetObj(m_dwHonour);
					m_pTitleNextObj = TitleConfig::GetInstance().GetNextObj(m_dwHonour);
				}
				break;
			case 10:
				m_wStarLv	= pRecv->dwVal;
				pMainMenu->m_pStarSysMenu->SetStarLv(m_wStarLv);
				break;
			case 11:
				m_dwBindRmb	= pRecv->dwVal;
				break;
			}
			pGuideCenter->SetUserLvGuide(); //===zjk
		}
		return true;
	case CS_SEND_FAST_ITEM:
		{
			SET_DATA(pData,USER,CS_SEND_FAST_ITEM,pPacket);
			char sztxt[32];
			ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pData->dwItemID);
			pChar->m_nFastItem = pData->dwItemID;			
			if (pBase)
				pMainMenu->m_pCtrlMenu->m_pFormSkill->GetComponent("yao")->SetTag(pBase);
		}
		return true;
	case S_SEND_CHAT_MSG:
		{
			SET_DATA(pData,USER,S_SEND_CHAT_MSG,pPacket);
			//pMainMenu->m_pChatMenu->recvMsg(pData->byCRType,pData->szMsg);
		}
		return true;
	case S_SEND_EXPMULTIPILE:
		{
			SET_DATA(pData,USER,S_SEND_EXPMULTIPILE,pPacket);
			m_nUserMultipile = pData->nUserMultipile;
			m_nPetMultipile = pData->nPetMultipile;

			if(m_bClickExp)
			{
				m_bClickExp = false;
				if(pData->nUserMultipileSec>0&&pData->nPetMultipileSec>0)
			      {
					  pMainMenu->PopTipMsg("当前人物经验倍率为%d%%   %d分钟后人物经验卡失效  当前宠物经验倍率为%d%%   %d分钟后宠物经验卡失效",m_nUserMultipile,pData->nUserMultipileSec/60,m_nPetMultipile,pData->nPetMultipileSec/60);
			    }else if(pData->nUserMultipileSec>0&&pData->nPetMultipileSec<1)
			      {
					  pMainMenu->PopTipMsg("当前人物经验倍率为%d%%   %d分钟后人物经验卡失效  当前宠物经验倍率为%d%%",m_nUserMultipile,pData->nUserMultipileSec/60,m_nPetMultipile);
			    }else if(pData->nUserMultipileSec<1&&pData->nPetMultipileSec>0)
			      {
					  pMainMenu->PopTipMsg("当前人物经验倍率为%d%%   当前宠物经验倍率为%d%%  %d分钟后宠物经验卡失效",m_nUserMultipile,m_nPetMultipile,pData->nPetMultipileSec/60);
			    }else
				    pMainMenu->PopTipMsg("当前人物经验倍率为%d%%  当前宠物经验倍率为%d%%",m_nUserMultipile,m_nPetMultipile);
			}
		}
		return true;
	case S_MAKESKILL_INFO:
		{
			SET_DATA(pData,USER,S_MAKESKILL_INFO,pPacket);
			m_makeSkill = pData->skillInfo;
			pMainMenu->m_pMakeSkillMenu->m_pForm->GetComponent("sx")->SetCaptionEx("%d/%d",1,pData->skillInfo.nPower,pMainMenu->m_pMakeSkillMenu->GetPowerMax());
		}
		return true;
	case S_INVITE_CODE:
		{
			SET_DATA(pData,USER,S_INVITE_CODE,pPacket);
			pMainMenu->m_pInviteAwardMenu->SetInvInfo(pData->szInvite,pData->nReachNum,pData->nCheckNum);
		}
		return true;
	case C_GETMAIL_ITEM:
		{
			SET_DATA(pData,USER,C_GETMAIL_ITEM,pPacket);

			for (std::map<int,MailObj>::iterator itor = pChar->m_pMailList.begin();itor != pChar->m_pMailList.end();itor++)
			{
				if (itor->second.dwMailID == pData->dwMailID)
				{
					itor->second.byState = STATE_MAIL_GOT;
					itor->second.nMoney = 0;
					memset(itor->second.nIndex,0,sizeof(int)*nMaxMailItemCount);
					memset(itor->second.nItemID,0,sizeof(int)*nMaxMailItemCount);
					pMainMenu->m_pMailMenu->SelectReadMail(pMainMenu->m_pMailMenu->m_nSelectMail);
					pMainMenu->m_pMailMenu->m_bUpdate = true;
				}				
			}		
			//pMainMenu->m_pChatMenu->recvMsg(pData->byCRType,pData->szMsg);
		}
		return true;
	case S_PLAYEFFECT:
		{
			SET_DATA(pData,USER,S_PLAYEFFECT,pPacket);
			pMainMenu->PlayEffect(pData->nEffect);
		}
		return true;
	case S_CREATE_CHARACTER_FAIL:
		{
			SET_DATA(pData,USER,S_CREATE_CHARACTER_FAIL,pPacket);
			switch(pData->bErrorCode)
			{
			case 1:
				pMainMenu->PopSystemMsg("服务器正在维护,请稍后再试");
				break;
			case 2:
				pMainMenu->PopSystemMsg("角色名不合法");
				break;
			case 3:
				pMainMenu->PopSystemMsg("职业国家设定错误");
				break;
			case 4:
				pMainMenu->PopSystemMsg("邀请码不存在,请校对(若无邀请码请删除)");
				break;
			case 5:
				pMainMenu->PopSystemMsg("角色名已存在");
				break;
			case 6:
				pMainMenu->PopSystemMsg("该国家成员已爆满，请选择另一个国家");
				break;
			}
			
		}
		return true;
	case S_SEND_SERVERTIME:
		{
			SET_DATA(pData,USER,S_SEND_SERVERTIME,pPacket);
			m_dwServerTime = pData->dwServerTime;
			m_dwUpdateTime = xnGetTickCount()/1000;
		}
		return true;
	case S_WORLD_INFO:
		{
			SET_DATA(pData,USER,S_WORLD_INFO,pPacket);
			pMainMenu->m_wFstHomeID = pData->wFstHome;
		}
		return true;
	case S_SEND_HOMESKILL:
		{
			SET_DATA(pData,USER,S_SEND_HOMESKILL,pPacket);
			m_xHomeSkill = pData->xSkill;
			pLeague->_familyUpdate();
		}
		return true;
	case S_OPEN_FORM:
		{
			SET_DATA(pData,USER,S_OPEN_FORM,pPacket);
			switch(pData->nType){
			case 0:
				pMainInterface->GetDForm(pData->szFormName)->Open();

				break;
			case 1:
				{
					pMainInterface->GetDForm(pData->szFormName)->Close();
				}
				break;
			}
		}
		return true;
	case S_WARN_MSG:
		{
			SET_DATA(pData,USER,S_WARN_MSG,pPacket);
			pMainMenu->PopTipMsg(pData->szMsg);

		}
		return true;
	case CS_TEAM_STATE:
		{
			SET_DATA(pData,USER,CS_TEAM_STATE,pPacket);
			char sztxt[64];
			if (pData->byType == TEAM_COMMIT_INVITE)
			{
				if(pMainMenu->m_pSettingMenu->m_bNoTeam)
					pChar->SendTeamState(TEAM_COMMIT_REFUSE,(char*)NULL);
				else if(pMainMenu->m_pAutoMenu->m_bActive)
				{
					if(pMainMenu->m_pAutoMenu->m_bAutoTeam)
					{
						pChar->SendTeamState(TEAM_COMMIT_ACCEPT,(char*)NULL);
					}else{
					 //pChar->SendTeamState(TEAM_COMMIT_REFUSE,(char*)NULL);
						/*sprintf(sztxt,"%s邀请你加入队伍",pData->szMsg);
						pMainMenu->PopAskMsgBox(sztxt,RequestTeamEvent,NULL);*/
						m_bTeamRequest = true;
						m_dwTeamRequestTick = xnGetSecCount();
						strncpy(m_szTeamRqstName,pData->szMsg,20);
					 }
				}else{
					//sprintf(sztxt,"%s邀请你加入队伍",pData->szMsg);
					//pMainMenu->PopAskMsgBox(sztxt,RequestTeamEvent,NULL);
					m_bTeamRequest = true;
					m_dwTeamRequestTick = xnGetSecCount();
					strncpy(m_szTeamRqstName,pData->szMsg,20);
				}

			}else if (pData->byType == TEAM_COMMIT_ACCEPT)
			{
				sprintf(sztxt,"%s加入队伍",pData->szMsg);
				pMainMenu->PopTipMsg(sztxt);
			}else if (pData->byType == TEAM_COMMIT_REFUSE)
			{
				sprintf(sztxt,"%s拒绝了组队邀请",pData->szMsg);
				pMainMenu->PopTipMsg(sztxt);
				for (int i=0;i<TEAM_MAXUSER;i++)
				{
					if (pMainMenu->m_pTeamMenu->m_xUser[i].nState == TEAM_CREATE&&i == pData->byPos)
					{
						pMainMenu->m_pTeamMenu->m_xUser[i].dwUin = 0;
					}
				}
				if (pMainMenu->m_pTeamMenu->GetTeamCount() < 2)
				{
					pChar->SendTeamState(TEAM_COMMIT_LEAVE,(char*)NULL);
					for (int i=0;i<TEAM_MAXUSER;i++)
					{
						pMainMenu->m_pTeamMenu->m_xUser[i].dwUin = 0;
					}
				}
				pMainMenu->m_pTeamMenu->m_bUpdate = true;
			}else if (pData->byType == TEAM_COMMIT_LEAVE)
			{
				//if (pMainMenu->m_pTeamMenu->GetTeamCount() < 2||!pMainMenu->m_pTeamMenu->m_xUser[0].dwUin)
				//{
				//	pChar->SendTeamState(TEAM_COMMIT_LEAVE,(char*)NULL);

				//	_TeamNet& tUser = pMainMenu->m_pTeamMenu->m_xUser[(BYTE)pData->szMsg[0]];
				//	if (tUser.dwUin != pChar->m_dwUin)
				//	{
				//		tUser.dwUin = 0;
				//	}else{
				//		memset(pMainMenu->m_pTeamMenu->m_xUser,0,TEAM_MAXUSER*sizeof(_TeamNet));
				//	}
				//	pMainMenu->m_pTeamMenu->m_bUpdate = true;
				//}else
				{
					_TeamNet& tUser = pMainMenu->m_pTeamMenu->m_xUser[(BYTE)pData->szMsg[0]];
					sprintf(sztxt,"%s离开队伍",tUser.szName);
					if (tUser.dwUin != pChar->m_dwUin)
					{
						tUser.dwUin = 0;
					}else{
						memset(pMainMenu->m_pTeamMenu->m_xUser,0,TEAM_MAXUSER*sizeof(_TeamNet));
					}
					pMainMenu->PopTipMsg(sztxt);
					if (pMainMenu->m_pTeamMenu->m_nTeamMemberPos)
						pMainMenu->m_pTeamMenu->m_nTeamMemberPos = 0;
					pMainMenu->m_pTeamMenu->m_bUpdate = true;
				}
			}
			if (!pMainMenu->m_pTeamMenu->GetTeamedCount()&&pMainMenu->m_pTeamMenu->m_pForm->bOpened)
			{
				pMainMenu->m_pTeamMenu->m_nTeamMemberPos = -1;
				pMainMenu->m_pTeamMenu->Close();
				pMainMenu->m_pMainFormLB->Open(3);
			}
		}
		return true;
	case S_TEAM_INFO:
		{
			SET_DATA(pData,USER,S_TEAM_INFO,pPacket);
			pMainMenu->m_pTeamMenu->m_xUser[pData->xUser.nPos] = pData->xUser;
			pMainMenu->m_pTeamMenu->m_bUpdate = true;
		}
		return true;
	case S_SEND_MAIL_INFO:
		{
			SET_DATA(pData,USER,S_SEND_MAIL_INFO,pPacket);
			MailObj& pObj = m_pMailList[pData->dwMailID];
			pObj.byState = pData->byState;
			pObj.dwMailID = pData->dwMailID; 
			pObj.dwReturnTick	= pData->dwReturnTick;
			pObj.nMoney		= pData->nMoney;
			memcpy(pObj.nCnt ,pData->nCnt,sizeof(int)*nMaxMailItemCount);
			memcpy(pObj.nItemID ,pData->nItemID,sizeof(int)*nMaxMailItemCount);
			memcpy(pObj.nIndex ,pData->nItemIndex,sizeof(int)*nMaxMailItemCount);
			strncpy(pObj.szBody ,pData->szBody,255);
			strncpy(pObj.szTitle,pData->szTitle,32);
			pObj.uinRecver	= pData->uinRecver;
			pObj.uinSender	= pData->uinSender;
			bool bNew = IsNewMail();
			if (bNew)
			{
				pMainMenu->PopTipMsg("您收到一封新的邮件");
			}
			pMainMenu->m_pCtrlMenu->m_pCompMail->SetVisible(bNew);
			//if(pMainMenu)
			//pMainMenu->m_pMailMenu->m_bUpdate = true;
		}
		return true;
	case S_SEND_POP_MSG:
		{
			SET_DATA(pData,USER,S_SEND_POP_MSG,pPacket);
			pMainMenu->PopSystemMsg(pData->szMsg);
		}
		return true;
	case C_DELETE_MAIL:
		{
			SET_DATA(pData,USER,C_DELETE_MAIL,pPacket);
			m_pMailList.erase(pData->dwMailID);
			pMainMenu->m_pMailMenu->RefreshMailTmpList();
			pMainMenu->m_pMailMenu->m_bUpdate = true;
		}
		return true;
	case C_DELETE_MAIL_ALL:
		{
			SET_DATA(pData,USER,C_DELETE_MAIL_ALL,pPacket);
			for (int i =0; i < pData->wCnt; i++)
			{
				m_pMailList.erase(pData->dwMailID[i]);
			}
			pMainMenu->m_pMailMenu->RefreshMailTmpList();
			pMainMenu->m_pMailMenu->m_bUpdate = true;
		}
		return true;
	case S_SEND_USER_PARAM:
		{
			SET_DATA(pData,USER,S_SEND_USER_PARAM,pPacket);
			int nPreScore = GetScore();
			m_xUserParam = pData->m_Param;
			int nNowScore = GetScore();
			if (nPreScore&&nPreScore<nNowScore)
			{
				pMainMenu->m_pScore->GetComponent("moneyC3")->SetCaptionEx("%d",0,nNowScore);
				pMainMenu->m_pScore->GetComponent("moneyC3")->SetTag((void*)(nNowScore-nPreScore));
				pMainMenu->m_pScore->Open(7);
			}
		}
		return true;
	case S_FLAG_ALL:
		{
			SET_DATA(pData,USER,S_FLAG_ALL,pPacket);
			m_pClientFlag->mapFlags.clear();
			for (int i = 0; i < pData->wCnt; i++)
			{
				m_pClientFlag->mapFlags[pData->szFlag[i].wID]	= pData->szFlag[i].nVal;
			}
		}
		return true;
	case S_FLAG_SINGLE:
		{
			SET_DATA(pData,USER,S_FLAG_SINGLE,pPacket);
			m_pClientFlag->mapFlags[pData->szFlag.wID]	= pData->szFlag.nVal;

			if (pData->szFlag.wID == 19 || pData->szFlag.wID == 31) //jjc购买次数 刷新界面
				pMainMenu->m_pWarRankMenu->m_bUpdate = true;
		}
		return true;
	case S_COOLDOWN_ALL:
		{
			SET_DATA(pData,USER,S_COOLDOWN_ALL,pPacket);
			DWORD dwNowTime = xnGetTickCount();
			m_pClientFlag->mapCD.clear();
			for (int i = 0; i < pData->wCnt; i++)
			{
				m_pClientFlag->mapCD[pData->szFlag[i].wID]	= dwNowTime + pData->szFlag[i].nVal*1000;
			}
		}
		return true;
	case S_COOLDOWN_SINGLE:
		{
			SET_DATA(pData,USER,S_COOLDOWN_SINGLE,pPacket);
			m_pClientFlag->mapCD[pData->szFlag.wID]	= pData->szFlag.nVal*1000+xnGetTickCount();
		}
		return true;
	case S_FLAG_COPYLMT_ALL:
		{
			SET_DATA(pData,USER,S_FLAG_COPYLMT_ALL,pPacket);
			m_pClientFlag->mapCopyLmt.clear();
			for (int i = 0; i < pData->wCnt; i++)
			{
				m_pClientFlag->mapCopyLmt[pData->szFlag[i].wID] = pData->szFlag[i].bVal;
			}
		}
		return true;
	case S_FLAG_COPYLMT_SINGLE:
		{
			SET_DATA(pData,USER,S_FLAG_COPYLMT_SINGLE,pPacket);
			m_pClientFlag->mapCopyLmt[pData->nCopyID]	= pData->bVal;
		}
		return true;
	case S_FLAG_SLIMIT_ALL:
		{
			SET_DATA(pData,USER,S_FLAG_SLIMIT_ALL,pPacket);
			if(pData->wCnt > SHOPLIMIT_MAX)
				pData->wCnt	= SHOPLIMIT_MAX;	//客户端服务器不一致
			memcpy(m_pClientFlag->szbySLimit,pData->szbyData,pData->wCnt);
		}
		return true;
	case S_FLAG_SLIMIT_SINGLE:
		{
			SET_DATA(pData,USER,S_FLAG_SLIMIT_SINGLE,pPacket);
			if(pData->szFlag.wID < SHOPLIMIT_MAX)
				m_pClientFlag->szbySLimit[pData->szFlag.wID]	= pData->szFlag.nVal;
		}
		return true;
	case S_FLAG_QUEST_ALL:
		{
			SET_DATA(pData,USER,S_FLAG_QUEST_ALL,pPacket);

			m_pClientFlag->mapQLineStep.clear();
			m_pClientFlag->mapDayTime.clear();
			for (int i = 0; i < pData->wCntLine; i++)
			{
				m_pClientFlag->mapQLineStep[pData->szFlag[i].wID]	= pData->szFlag[i].bVal;
			}
			for (int i = pData->wCntLine; i < pData->wCntTime; i++)
			{
				m_pClientFlag->mapDayTime[pData->szFlag[i].wID]	= pData->szFlag[i].bVal;
			}
			m_pCQuestCenter->ReCheckCanAccept();
		}
		return true;
	case S_FLAG_QUEST_SINGLE:
		{
			SET_DATA(pData,USER,S_FLAG_QUEST_SINGLE,pPacket);
			if(pData->bIsTime == 0)
			{
				m_pClientFlag->mapQLineStep[pData->szFlag.wID]	= pData->szFlag.bVal;
			}
			else
				m_pClientFlag->mapDayTime[pData->szFlag.wID]	= pData->szFlag.bVal;
			m_pCQuestCenter->ReCheckCanAccept();
		}
		return true;
	case S_QUEST_INFO:
		{
			SET_DATA(pData,USER,S_QUEST_INFO,pPacket);
			pGuideCenter->SetQuestInfoGuide(pData);//===zjk			
 			m_pCQuestCenter->RecvQuestInfo(pData->wQid,pData->wVal);
		}
		return true;
	case S_REMOVE_QUEST:
		{
			SET_DATA(pData,USER,S_REMOVE_QUEST,pPacket);	
			pGuideCenter->SetQuestRemoveGuide(pData);//===zjk		
			m_pCQuestCenter->RecvDeleteQuest(pData->wQid,pData->bComplete);			
		}
		return true;
	case S_SKILL_SINGLE:
		{
			SET_DATA(pData,USER,S_SKILL_SINGLE,pPacket);

			if(pData->byLv == 0)
			{
				m_pSkillCenter->RemoveSkill(pData->wSkill);
			}
			else
			{
				m_pSkillCenter->SetSkill(pData->wSkill,pData->byLv);
			}

			if(pMainMenu->m_pSkillMenu->m_pFormSInfo->bOpened && pMainMenu->m_pSkillMenu->m_pCurSelSkill->wSkillID == pData->wSkill)
			{
				pMainMenu->m_pSkillMenu->ViewSkillInfo(pMainMenu->m_pSkillMenu->m_pCurSelSkill,pData->byLv);
			}
			if(pMainMenu->m_pSPSkillMenu->m_pForm->bOpened)
				pMainMenu->m_pSPSkillMenu->Open(2);
		}
		return true;
	case S_SKILL_ALL:
		{
			SET_DATA(pData,USER,S_SKILL_ALL,pPacket);

			for (int i = 0; i < pData->bCnt; i++)
			{
				m_pSkillCenter->SetSkill(pData->szSkill[i].wSkill,pData->szSkill[i].byLv);
			}
		}
		return true;
	//case C_SKILLCZ:
	//	{
	//		SET_DATA(pData,USER,C_SKILLCZ,pPacket);			
	//		pData->bCZ = m_pSkillCenter->RemoveAllSkill();			
	//	}
	//	return true;
	case S_SEND_HOME_QUEST:
	case C_HOME_QUEST_ACCEPT:
		{
			pMainMenu->m_pFamilyQuestMenu->m_bUpdate = true;
		}
		return true;
	case S_SEND_MAPCOPY_OK:
		{
			SET_DATA(pData,USER,S_SEND_MAPCOPY_OK,pPacket);
			if (pMainMenu->m_pAutoMenu->m_bActive)			
				pMainMenu->m_pAutoMenu->SwitchActive(false);
			pMainMenu->m_pMCAwardMenu->OpenForm(pData->wMCopyID,pData->byRankVal);
		}
		return true;
	case S_OPENCARDRTL:
		{
			SET_DATA(pData,USER,S_OPENCARDRTL,pPacket);
			pMainMenu->m_pMCAwardMenu->SetCardItem(pData->wItem);
		}
		return true;
	case S_RQ_OPENALL_RLT:
		{
			SET_DATA(pData,USER,S_RQ_OPENALL_RLT,pPacket)
			pMainMenu->m_pMCAwardMenu->SetCardItems(pData->wItem,pData->wCnt);

		}
		return true;
	case S_MCOPY_MASTERJOIN:
		{
			SET_DATA(pData,USER,S_MCOPY_MASTERJOIN,pPacket);
			
			MCopyBase* pMBase	= MCopyConfig::GetInstance().GetMCopy(pData->wMCID);
			char	szTmp[256];
			sprintf(szTmp,"是否进入队伍副本 %s ?",pMBase->szName);
			pMainMenu->PopAskMsgBox(szTmp,fun_cbTeamMCopy,(void*)pData->wMCID);
		}
		return true;
	case S_SEND_EQUIPINFO:
		{
			SET_DATA(pData,USER,S_SEND_EQUIPINFO,pPacket);

			ClientUserInfo* pNew = new ClientUserInfo();

			pNew->dwUin = pData->pUserInfo.dwUin;
			pNew->nLv = pData->pUserInfo.nLv;
			pNew->byJob = pData->pUserInfo.byJob;
			pNew->nScore = pData->pUserInfo.nScore;
			pNew->nSkin[0] = pData->pUserInfo.nSkin[0];
			pNew->nSkin[1] = pData->pUserInfo.nSkin[1];
			pNew->nSkin[2] = pData->pUserInfo.nSkin[2];
			memcpy(pNew->szName,pData->pUserInfo.szName,20);
			memset(pNew->szItem,0,EQUIP_MAX*4);
			for(int i = 0; i < EQUIP_MAX; i ++)
			{
				if(pData->pUserInfo.szSize[i] > 0)
				{
					pNew->szItem[i] = pChar->m_pItemCenter->CreateItemByData(pData->pUserInfo.pData[i],pData->pUserInfo.szSize[i]);
				}
			}
			pMainMenu->m_pSelObjInfoMenu->Open(pNew);
			
		}
		return true;
	case S_LUCKBET_RTL:
		{
			SET_DATA(pData,USER,S_LUCKBET_RTL,pPacket);

			pMainMenu->m_pLuckBetMenu->GetResult(pData->byEndPos,pData->nGetVal);
		}
		return true;
	case S_LUCKBET_EX_RTL:
		{
			SET_DATA(pData,USER,S_LUCKBET_EX_RTL,pPacket);
			pMainMenu->m_pLuckBetMenu->GetResultEx(pData->byRtlVal,pData->nGetVal);
		}
		return true;
	case S_RESPON_NPC_FORM:
		{
			SET_DATA(pData,USER,S_RESPON_NPC_FORM,pPacket);
			switch(pData->nNpcCmd)
			{
			case 1001:	//仓库
				pMainMenu->m_pBankMenu->Open();
				break;
			case 1002:	//拍卖
				pMainMenu->m_pNaSaleMenu->OpenForm();
				break;
			case 1005:	//水果机
				pMainMenu->m_pLuckBetMenu->OpenForm();
				break;
			case 1006:	//激活码
				pMainMenu->m_pActivateMenu->Open();
				break;
			}
		}
		return true;
	case S_FIGHT_TARGET:
		{
			SET_DATA(pData,USER,S_FIGHT_TARGET,pPacket);

			strncpy(m_szTargetName,pData->szName,20);
			m_bFightRequest = true;
			m_dwFightRequestTick = xnGetSecCount();
		}
		return true;
	case S_RQ_TEAM_MCOPY_RLT:
		{
			pMainMenu->m_pMapCopyMenu->m_dwWaitTeam	= xnGetTickCount();
			pMainMenu->PopAskMsgBox("正在寻找队伍...",fun_cbMCopyTeam,pMainMenu->m_pMapCopyMenu,2);
		}
		return true;
	case S_TEAM_DROPRAND:
		{
			SET_DATA(pData,USER,S_TEAM_DROPRAND,pPacket);
			pMainMenu->m_pTeamDropMenu->m_pDropList->Add((void*)pData->nItemID);
		}
		return true;
	case S_SEND_KILLDROP:
		{
			SET_DATA(pData,USER,S_SEND_KILLDROP,pPacket);
			pMainMenu->m_pDropShowMenu->m_pList->Add((void*)pData->nWID);
		}
		return true;
	case C_FORT_HARVEST:
		{
			m_bHarvest = true;
		}
		return true;
	case C_SET_AUTOSKILL:
		{
		    pMainMenu->m_pAutoMenu->m_bRecharge = true;
			pMainMenu->m_pAutoMenu->m_pCheckAutoSkill->SetCheck(true);
		}
		return true;
	case S_SET_AUTOFIX:
		{
			pMainMenu->m_pAutoMenu->m_bRecharge = true;
			pMainMenu->m_pAutoMenu->m_pCheckAutoFix->SetCheck(true);
		}
		return true;
	case S_SET_AUTOBUY:
		{
			pMainMenu->m_pAutoMenu->m_bRecharge = true;
			pMainMenu->m_pAutoMenu->m_pCheckAutoBuy->SetCheck(true);
		}
		return true;
	case C_CHANGE_COUNTRY:
		{
			m_dwChangeCountryTick = xnGetTickCount();
			if (pMainMenu->m_pBagMenu->m_pForm->bOpened)
				pMainMenu->m_pBagMenu->Close();
			pMainMenu->PopSystemMsg("正在转国中，请稍候...",true);
		}
		return true;
	case S_CHGNAME_RLT:
		{
			pMainMenu->PopTipMsg("改名成功，将立即返回登入");
			pMainMenu->m_pChangeNameF->Close();
			ReturnLogin();
		}
		return true;
	case S_SEND_NOTICE:
		{
			SET_DATA(pData, USER, S_SEND_NOTICE, pPacket);
			pMainMenu->m_pNoticeMenu->InitNotice(pData->szMsg);
		}
		return true;
	case S_SEND_CLOSE_WAITE:
		{
			SET_DATA(pData, USER, S_SEND_CLOSE_WAITE, pPacket);
			m_dwLogoutWaitTick = xnGetTickCount() + pData->dwWaitSec*1000;
		}
		return true;
	case S_SEND_HOME_MAXCTB:
		{
			SET_DATA(pData, USER, S_SEND_HOME_MAXCTB, pPacket);
			m_dwHomeMaxCtb = pData->dwHomeMaxCtb;
		}
		return true;
	}

	return true;
}

void UserData::DealFightRequest(bool bVal)
{
	m_bFightRequest = false;
	m_dwFightRequestTick = 0;
	pMainMenu->m_pFastEquipMenu->m_pFightTip->SetVisible(false);
	pMainMenu->m_pFastEquipMenu->m_pFightTip->byRunAction = 16;
	pMainMenu->m_pFastEquipMenu->CheckListFormClose();

	if (bVal)
	{
		char sztxt[64];
		sprintf(sztxt,"%s要求和您进行切磋,是否同意?",m_szTargetName);
		pMainMenu->PopAskMsgBox(sztxt,fun_cbFight,NULL);
	}else
	{
		BuildPacketEx(pPacket, USER, C_FIGHT_REQUEST,buf, 256);
		SET_DATA(pSend, USER, C_FIGHT_REQUEST, pPacket);
		pSend->byResult	= 0;
		pChar->SendPacket(pPacket);
	}
}

void UserData::DealTeamRequest(bool bVal)
{
	m_bTeamRequest = false;
	m_dwTeamRequestTick = 0;
	pMainMenu->m_pFastEquipMenu->m_pTeamInviteTip->SetVisible(false);
	pMainMenu->m_pFastEquipMenu->m_pTeamInviteTip->byRunAction = 16;
	pMainMenu->m_pFastEquipMenu->CheckListFormClose();

	if (bVal)
	{
		char sztxt[64];
		sprintf(sztxt,"%s邀请你加入队伍",m_szTeamRqstName);
		pMainMenu->PopAskMsgBox(sztxt,RequestTeamEvent,NULL);
	}
}

UserData::UserData():m_nFastItem(0),m_bHarvest(false),m_bFish(false),m_bFishAct(true),m_dwBeHurtTick(0),m_bClickExp(false),m_bHorseState(false),m_nLoginDays(0)
{
	m_pDataCenter	= new AmzDataManager();
	m_pItemCenter = new ItemCenter(m_pDataCenter);
	m_pEquipCenter = new EquipCenter(m_pDataCenter);
	m_pCQuestCenter	= new ClientQuestCenter();
	g_pMapCommand	= new MapCommand();
	m_pClientFlag	= new ClientFlagCenter();
	m_pOnlineAwardCenter = new OnlineAwardCenter();
	m_pRechargeAwardCenter = new RechargeAwardCenter();
	m_pLvupAwardCenter = new LvupAwardCenter();
	m_pLoginAwardCenter = new LoginAwardCenter();
	m_pMounthAwardCenter = new MounthAwardCenter();
	m_pMentoringClient = new MentoringClient();
	m_pCAchievementCenter = new CAchievementCenter();
	m_pMarryCenter = new MarryClient();
	m_nUserMultipile= 100;
	m_nPetMultipile	= 100;
	m_byLvl			= 1;
	m_dwMoney		= 0;
	m_dwRmb			= 0;
	m_dwBindRmb		= 0;
	m_dwPow			= 0;
	m_dwExp			= 0;
	m_wSkillPt		= 0;
	m_wStarLv		= 0;
	m_bManualSel	= false;
	m_dwLockSkill	= 0;
	m_bLockReconnet	= true;
	m_bLockJump		= false;
	m_dwTickBattle	= 0;
	//pMapObj			= NULL;
	m_dwTickJoin	= 0;
	m_dwTickCmdSync	= xnGetTickCount();
	m_pSlaveCenter	= new SlaveCCenter();
	m_pHorseCenter	= new HorseCCenter();
	m_pChatCenter	= new ChatCenter();
	m_pWTestCenter	= new WorldTestCenter();
	m_pSkillCenter	= new SkillCenter();
	m_pFortCenter	= new FortCenter(this);
	m_pRankCenter	= new RankCenter();
	m_pADegreeCenter= new ACDegreeCenter();
	m_pActivityCenter = new GameActivityCenter();
	m_pTurntableCenter = new TurntableCenter();
	m_dwJumpLock	= 0;
	m_dwCmdLock		= 0;
	m_byVipLvl		= 0;
	m_dwVipExp		= 0;
	m_nMaxMp		= 0;
	m_nMp			= 0;
	m_nSPVal		= 0;
	m_nGetSlaveLmt  = 0;
	m_bVIPCh		= false;
	m_wCtrlAtkDis	= 120;
	m_dwHonour		= 0;
	m_byGSStep		= 0;
	m_wAliveGSFrm		= 0;
	m_wAliveMSFrm		= 0;
	m_byMSStep		= 0;
	m_dwTickAppPay	= 0;
	m_dwLogoutWaitTick = 0;
	m_pTitleObj	= TitleConfig::GetInstance().GetObj(m_dwHonour);
	m_pTitleNextObj = TitleConfig::GetInstance().GetNextObj(m_dwHonour);

	memset(&m_xHomeSkill,0,sizeof(UserHomeSkill));
	memset(&m_szbyActLock,0,7);
	memset(&m_xUserParam,0,sizeof(UserParam));
	memset(&m_xUserParam2,0,sizeof(UserParam));
	m_bLoadCfgOK	= false;
//#ifdef WIN32
	MuliThreadInitCfg();
//#else
//	pthread_create(&xLoadCfgThread, NULL, dealLoadCfg, this);
//#endif
	
	m_bFightRequest = false;
	m_dwFightRequestTick = 0;

	memset(&m_makeSkill,0,sizeof(MakeSkill));

	m_bTeamRequest = false;

	m_dwMCAutoSwitchTick = 0;
	m_bIsFishSkin = false;
	m_dwChangeCountryTick = 0;
}

UserData::~UserData()
{
	delete m_pDataCenter;
	delete m_pItemCenter;
	delete m_pEquipCenter;
	delete m_pCQuestCenter;
	delete g_pMapCommand;
	delete m_pSlaveCenter;
	delete m_pHorseCenter;
	delete m_pChatCenter;
	delete m_pClientFlag;
	delete m_pWTestCenter;
	delete m_pRankCenter;
	delete m_pADegreeCenter;
	delete m_pActivityCenter;
	delete m_pTurntableCenter;
	delete m_pOnlineAwardCenter;
	delete m_pRechargeAwardCenter;
	delete m_pLvupAwardCenter;
	delete m_pLoginAwardCenter;
	delete m_pMounthAwardCenter;
	delete m_pMentoringClient;
	delete m_pCAchievementCenter;
	delete m_pMarryCenter;
}

void UserData::CheckFishAct()
{
	if(g_pMapCenter->m_pMObjLockView == NULL)
		return;

	if (GetMyMapObj()->GetAction()==OBJ_ACTION_HURT)
		m_dwBeHurtTick = xnGetTickCount();

	if (m_bFishAct&&g_pMapCenter->m_pMObjLockView->pStateCenter->CheckState(8111)&&pMainMenu->m_pFishMenu->m_pForm->bOpened)
	{
		m_bFishAct = false;
		g_pMapCommand->m_wTrySkill	= 0;
		GetMyMapObj()->SetSkillAtk(NULL);
		SendCmd(MAPCMD, C_SHOW_FISHACT);
	}
	if (!m_bFishAct&&g_pMapCenter->m_pMObjLockView->pStateCenter->CheckState(8111)&&!pMainMenu->m_pFishMenu->m_pForm->bOpened)
	{
		m_bFishAct = true;
		g_pMapCommand->m_wTrySkill	= 0;
		GetMyMapObj()->SetSkillAtk(NULL);
		SendCmd(MAPCMD, C_SHOW_FISHACT);
	}
}

void UserData::Update()
{
	m_pDataCenter->Update ();
	m_pItemCenter->Update();
	m_pEquipCenter->Update();
	m_pSlaveCenter->Update();
	m_pHorseCenter->Update();
	m_pCQuestCenter->Update();

	CheckFishAct();//检测钓鱼动作

	_checkUserCommand();
	//检测地图事件(跳地图)
	_checkMapPosEvent();

	if(g_packetCenter.m_bActive == false)
	{
		pChar->ReConnect();
	}

	if(g_pTPObj->TP_GetPlat() == 10)
	{
		if(g_pTPObj->nPayStep)
		{
			if(m_dwTickAppPay == 0)
				m_dwTickAppPay	= xnGetTickCount();
			pMainMenu->ShowNetWaiting(0);
		}
		if(g_pTPObj->nPayStep	== 2)
		{
			g_pTPObj->nPayStep	= 0;

			BuildPacketEx(pSAppPacket,ACCOUNT,TP_IAPPAY,bufA,4096);
			SET_DATA(pSAppData,ACCOUNT,TP_IAPPAY,pSAppPacket);

			strcpy(pSAppData->szAcc,g_szLoginAcc);
			strcpy(pSAppData->szToken,g_pTPObj->szOrder_No);
			pSAppPacket->wSize	+= strlen(pSAppData->szToken);
			SendPacket(pSAppPacket);

			pMainMenu->HideNetWaiting();
			m_dwTickAppPay	= 0;
			pMainMenu->PopSystemMsg("充值成功,钻石即将到账!");
		}
		else if(g_pTPObj->nPayStep	== 9 || (g_pTPObj->nPayStep && xnGetTickCount() - m_dwTickAppPay > 40000))
		{
			g_pTPObj->nPayStep	= 0;
			pMainMenu->HideNetWaiting();
			m_dwTickAppPay		= 0;
			pMainMenu->PopSystemMsg("充值没有成功!");
		}
	}
	UpdateCatchFishSkin();

	if (m_dwChangeCountryTick && xnGetTickCount() - m_dwChangeCountryTick >= 2000)
	{
		ReturnLogin();
		m_dwChangeCountryTick = 0;
	}
	 ClientLuaManager::sharedHD()->Update();
}

void	fun_cbCastKillEnemy(void* param)
{
	MapAIObj* pObj	= g_pMapCenter->GetMapObj(MAPOBJ_ENEMY,(int)param);
	if(pObj)
	{
		pObj->nHP	= 0;
		pChar->_sendAtkTarget(0,MAPOBJ_ENEMY,(int)param);
	}
	
}

void UserData::_checkUserCommand(bool bForce)
{
	if(!bForce && xnGetTickCount() - m_dwTickCmdSync < 100)
		return;
	m_dwTickCmdSync	= xnGetTickCount();

	MapAIObj* pMapObj	= GetMyMapObj();
	if(pMapObj==NULL)
		return;

	if(pMapObj->IsBusy())
		return;

	if(pChar->m_szbyActLock[0])
	{
		g_pMapCommand->Reset();
		return;
	}

	if(pMapObj->IsDead())
	{
		g_pMapCommand->Reset();
		return;
	}

	if(m_dwCmdLock && xnGetTickCount() - m_dwCmdLock < 2000)
		return;

	BYTE	bRunAction	= pMapObj->GetAction();
	if(bRunAction != OBJ_ACTION_STAND&&bRunAction != OBJ_ACTION_HSTAND)
	{
		pMainMenu->StopCast();
	}

	if(g_pMapCommand->pGuideCmd)
	{
		//if(g_pMapCommand->pGuideCmd && g_pMapCommand->pGuideCmd->byType != 2)
		//{//判断开启挂机
		//	pMainMenu->m_pAutoMenu->SwitchActive(false);
		//}

		if(g_pMapCommand->pGuideCmd && g_pMapCommand->pGuideCmd->byType == 1)
		{
			MapAIObj* pNpc	= g_pMapCenter->GetMapObj(MAPOBJ_NPC,g_pMapCommand->pGuideCmd->nID);
			if(pNpc && !g_pMapCenter->CheckLineBlock(pMapObj->nPosX,pMapObj->nPosY,pNpc->nPosX,pNpc->nPosY,true))
			{
				g_pMapCommand->CmdNormalAtk(pNpc);
				g_pMapCommand->pGuideCmd	= NULL;
			}
		}
		if (g_pMapCommand->pGuideCmd && g_pMapCenter->m_nMapIndex == g_pMapCommand->pGuideCmd->nMapindex&&(g_pMapCommand->pGuideCmd->byType == 2))
		{//任务寻怪  寻到怪开始攻击
			if (g_pMapCenter->m_pMObjSel&&pChar->m_byLvl > 3)
			{
				MapAIObj* pSelObj = (MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj;
				if (pSelObj&&pSelObj->enemyID == g_pMapCommand->pGuideCmd->nID&&!g_pMapCenter->CheckLineBlock(g_pMapCenter->m_pMObjLockView->nPosX,g_pMapCenter->m_pMObjLockView->nPosY,pSelObj->nPosX,pSelObj->nPosY,true))
				{
					pMainMenu->m_pAutoMenu->SwitchActive(true,g_pMapCommand->pGuideCmd->nPosX,g_pMapCommand->pGuideCmd->nPosY);
					g_pMapCommand->CmdNormalAtk(pSelObj);
				}else
					g_pMapCenter->SearchNearObj(1,1000000,g_pMapCommand->pGuideCmd->nID);
			}
		}
	}

	if(g_pMapCenter->m_nMapIndex >= 1000&&pMainMenu->m_pAutoMenu->m_bActive == true&&(xnGetTickCount() > m_dwMCAutoSwitchTick))
	{//副本挂机  没碰到怪则往boss方向跑

		if(g_pMapCenter->m_pMObjSel)
		{
			MapAIObj* pEnemyObj = (MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj;
			if(pEnemyObj)
			{
				g_pMapCommand->CmdNormalAtk(pEnemyObj);
				m_dwMCAutoSwitchTick = xnGetTickCount() + 5000;
			}
		}else 
		{
			MCopyBase*	pMCBase	= MCopyConfig::GetInstance().GetMCopyByMBase(g_pMapCenter->m_nBaseMapIndex);
			if(pMCBase)
			{
				PathObj* pObj = MonsterPathLeadCfg::GetInstance().GetPathObj(pMCBase->wWinID);
	            if(pObj)
	            {
					m_dwMCAutoSwitchTick = xnGetTickCount() + 2000;
					g_pMapCommand->CmdPoint(pObj->nPosX, pObj->nPosY);
	            }
			}
		 }
	}


	int nSkill	= 0;
	MapAIObj*	pObj	= g_pMapCommand->PopAtkCmd(nSkill);
	if(pObj)
	{
		if(g_pMapCenter->CheckLineBlock(pMapObj->nPosX,pMapObj->nPosY,pObj->nPosX,pObj->nPosY,true)==true)
			return;

		int nTargetDis	= m_wCtrlAtkDis;
		//普通攻击
		int nTLen	= CalcDistance(pObj->nPosX,pObj->nPosY,pMapObj->nPosX,pMapObj->nPosY);
		if(pObj->m_byType == MAPOBJ_ENEMY && pObj->pBaseCfg && pObj->pBaseCfg->bType == 3)
		{
			int nTLen	= CalcDistance(pObj->nPosX,pObj->nPosY,pMapObj->nPosX,pMapObj->nPosY);

			if(nTLen < 150)
			{
				if(m_pHorseCenter->m_ifRide)		//骑马不能攻击
				{
					pChar->m_pHorseCenter->RequestRide(false);
				}
				if (pObj->pBaseCfg->nID == 1500&&!pChar->CheckSkill(MAKE_SKILL_TYPE_DIG))
				{
					pMainMenu->PopTipMsg("需要学习采矿技能");
				}else if (pObj->pBaseCfg->nID == 1501&!pChar->CheckSkill(MAKE_SKILL_TYPE_COLLECT))
				{
					pMainMenu->PopTipMsg("需要学习采集技能");
				}else if (pObj->pBaseCfg->nID == 1502&!pChar->CheckSkill(MAKE_SKILL_TYPE_COLLECT))
				{
					pMainMenu->PopTipMsg("需要学习采集技能");
				}else if(pObj->pBaseCfg->nID == 1061 || pObj->pBaseCfg->nID == 1119)
				{
					pMainMenu->StartCast(pObj->nHP,"正在引爆炸药桶中．．．",fun_cbCastKillEnemy,(void*)pObj->m_nID);
					if(abs(pObj->nPosX - pMapObj->nPosX)+abs(pObj->nPosY-pMapObj->nPosY)>10)
						pMapObj->m_byDir	= CalcDirection(pMapObj->nPosX,pMapObj->nPosY,pObj->nPosX,pObj->nPosY,2);
				}else{
					if (pObj->pBaseCfg->nID == 1520 && pChar->m_pClientFlag->GetFlag(62) > 19)
					{
						pMainMenu->PopTipMsg("每日只可采集20个！");
						return;
					}
					pMainMenu->StartCast(pObj->nHP,"正在采集物品中．．．",fun_cbCastKillEnemy,(void*)pObj->m_nID);
					if(abs(pObj->nPosX - pMapObj->nPosX)+abs(pObj->nPosY-pMapObj->nPosY)>10)
						pMapObj->m_byDir	= CalcDirection(pMapObj->nPosX,pMapObj->nPosY,pObj->nPosX,pObj->nPosY,2);
				}
				g_pMapCommand->m_pObjNAtk	= NULL;
				g_pMapCommand->Reset();	//否则会一致点击NPC
				return;
			}

			nTargetDis	= 120;
		}
		else
		{
			if(pObj->m_byType == MAPOBJ_NPC)
			{//NPC对话,需要清除攻击指令
				if(nTLen < 200)
				{
					g_pMapCommand->m_pObjNAtk	= NULL;
					_clickNpc(pObj);
					g_pMapCommand->Reset();	//否则会一致点击NPC
					return;
				}
			}
			else if((pObj->bForce && pMapObj->bForce != pObj->bForce))
			{
				if(nSkill)
				{//判断并使用技能
					if(_checkUseSkill(nSkill,pObj))
						return;
				}

				if(nTLen <= m_wCtrlAtkDis)
				{//攻击范围内
					if(m_pHorseCenter->m_ifRide)		//骑马不能攻击
					{
						pChar->m_pHorseCenter->RequestRide(false);
					}
					if(pMapObj->IsAtkCD() == false)
					{
						if(pChar->m_szbyActLock[4])
							return;


						pMapObj->SetNAtk(pObj);
						g_pMapCenter->AddCharNAtkEff(pMapObj,pObj);
						//发包给服务器发起
						_sendAtkTarget(0,pObj->m_byType,pObj->m_nID);
						//g_pMapCommand->Reset();
					}
					return;
				}
			}

			if(pObj->m_byType == MAPOBJ_NPC)
			{
				nTargetDis	= 120;
			}
		}
		
		if(nTLen > nTargetDis)
		{
			int nDX	= (pObj->nPosX-pMapObj->nPosX)* (nTLen-nTargetDis+30)/nTLen+pMapObj->nPosX;
			int nDY	= (pObj->nPosY-pMapObj->nPosY)* (nTLen-nTargetDis+30)/nTLen+pMapObj->nPosY;
			//pMapObj->_setMove(nDX,nDY);

			//int nNoBlockX	= nDX;
			//int nNoBlockY	= nDY;
			//if(g_pMapCenter->m_pMapBarrier->CheckLineBlock(pMapObj->nPosX,pMapObj->nPosY,nDX,nDY,&nNoBlockX,&nNoBlockY))
			//{
			//	nDX	= nNoBlockX;
			//	nDY	= nNoBlockY;
			//}

			if(pMapObj->objAIModel.SetCmd(MCMD_RUN,(nDY<<16)+nDX))
				_sendMoveTarget(nDX,nDY);
		}
		return;
	}
	else
	{
		if(nSkill)
		{
			SkillBase* pSBase	= SkillConfig::GetInstance().GetSkillBase(nSkill);
			if(pSBase && pSBase->bAoeType == 0)
				if(_checkUseSkill(nSkill,NULL))
					return;
		}
	}
	if(pMapObj->m_byState == OBJ_STATE_STAND || pMapObj->m_byState == OBJ_STATE_RUN)
	{
		if(pChar->m_szbyActLock[2])
		{
			g_pMapCommand->Reset();
			return;
		}

		POINT	ptDest;
		ptDest.x	= pMapObj->nPosX;
		ptDest.y	= pMapObj->nPosY;
		if(g_pMapCommand->PopNextPoint(g_pMapCenter->m_nMapIndex,ptDest))
		{
			if(ptDest.x != pMapObj->nPosX || ptDest.y!=pMapObj->nPosY)
			{
				if(GetMyMapObj()->objAIModel.SetCmd(MCMD_RUN,(ptDest.y<<16)+ptDest.x))
					_sendMoveTarget(ptDest.x,ptDest.y);
			}
			else if(pMapObj->m_byState == OBJ_STATE_RUN)
			{
				if(g_pMapCommand->pGuideCmd && g_pMapCommand->pGuideCmd->byType == 2)
				{//判断开启挂机
					if(pChar->m_byLvl >= 4)
						pMainMenu->m_pAutoMenu->SwitchActive(true);
				}
				pMapObj->objAIModel.SetCmd(MCMD_NULL,0);
				if(pMapObj->objAIModel.byAIType == MCMD_NULL)
					pMapObj->m_byState	= OBJ_STATE_STAND;
			}

		}
		else
		{
			if(g_pMapCommand->pGuideCmd && g_pMapCommand->pGuideCmd->byType == 2)
			{//判断开启挂机
				if(CalcDistance(pMapObj->nPosX,pMapObj->nPosY,g_pMapCommand->pGuideCmd->nPosX,g_pMapCommand->pGuideCmd->nPosY)<300)
				{
					if(pChar->m_byLvl >= 4)
						pMainMenu->m_pAutoMenu->SwitchActive(true);
				}

			}
		}
	}
}

bool UserData::CheckFixEquip()
{
	int nChgVal = 0;
	for(int i = 0; i < EQUIP_MAX; i ++)
	{
		ItemObj* pItem = pChar->m_pItemCenter->GetEquipBySlot(i);
		if(pItem && pItem->GetVal(ITEM_AD_NUM) <= 1)
		{
			nChgVal += pItem->pItemBase->nDura-pItem->GetVal(ITEM_AD_NUM);
		}
	}
	if(nChgVal > 0)
	{
		int nMoney = nChgVal * 75;
		if(this->m_dwMoney >= nMoney)
			return true;
	}
	return false;
}

void UserData::_sendMoveTarget( int nX, int nY )
{
	BuildPacketEx(pPacket, MAPCMD, C_SETMOVE, buf, 256);
	SET_DATA(pSend, MAPCMD, C_SETMOVE, pPacket);
	pSend->wX	= nX;
	pSend->wY	= nY;
	SendPacket(pPacket);
}

void UserData::_sendAtkTarget( WORD wSkill, BYTE bType, DWORD dwID )
{
	BuildPacketEx(pPacket, MAPCMD, C_SETATK, buf, 256);
	SET_DATA(pSend, MAPCMD, C_SETATK, pPacket);
	pSend->wSkill	= wSkill;
	pSend->bType	= bType;
	pSend->dwID		= dwID;
	SendPacket(pPacket);
}

void UserData::_checkMapPosEvent()
{
	MapAIObj*	pMapObj	= GetMyMapObj();
	if(pMapObj && g_pMapCenter->m_pListMapTP)
	{
		bool	bOnTransPoint	= false;
		for (int i = 0; i < g_pMapCenter->m_pListMapTP->Count; i++)
		{
			_TransPoint* pTP	= (_TransPoint*)g_pMapCenter->m_pListMapTP->Items[i];
			if(CalcDistance(pTP->nPosX,pTP->nPosY,pMapObj->nPosX,pMapObj->nPosY) < pTP->nSrcRange)
			{
				if(xnGetTickCount() - m_dwJumpLock > 2000)
				{//每秒请求一次跳地图
					if(!m_bLockJump)
					{
						if(pTP->byToMapCopy)
						{
							if(pMainMenu->m_pTeamMenu->GetTeamedCount() != 0)
							{
								pMainMenu->PopTipMsg("组队状态下不可进入该地图");
								m_bLockJump	= true;
								break;
							}
							else
							{
								BuildPacketEx(pPacket,USER,C_JOINMAPCOPY_SP,buf,256);
								SET_DATA(pData,USER,C_JOINMAPCOPY_SP,pPacket);
								pData->wMCID	= pTP->nToMapIndex;
								pChar->SendPacket(pPacket);
							}
						}
						else
						{
							if(pTP->byLvLimit > m_byLvl)
							{
								pMainMenu->PopTipMsg("需要等级达到 %d 级",pTP->byLvLimit);
							}
							else
							{
#ifdef USE_ANDROID
								_ResDownObj* pRes	= ResControl::GetInstance().GetByLv(pTP->byLvLimit);
								if(pRes)
								{
									SaveStringToXML("ResUrl1",pRes->szUrl);
									SaveToXMLOK();
									pMainMenu->PopAskMsgBox("为了您更好的继续游戏，请您下载后续资源包，是否立即下载?",ResUrlAskMsg,NULL);
								}
								else
#endif
								{
									BuildPacketEx(pPacket, USER, C_JUMPMAP, buf, 256);
									SET_DATA(pData, USER, C_JUMPMAP, pPacket);
									pData->nTransPtID	= pTP->nID;
									SendPacket(pPacket);
								}
							}
						}

						if(m_dwCmdLock == 0)
							m_dwCmdLock		= xnGetTickCount();
						m_dwJumpLock	= xnGetTickCount();
					}
				}
				bOnTransPoint	= true;
			}
		}
		if(!bOnTransPoint)
			m_bLockJump	= false;
	}
}

MapAIObj* UserData::GetMyMapObj()
{
	if(g_pMapCenter)
		return g_pMapCenter->m_pMObjLockView;

	return NULL;
}

void UserData::_clickNpc( MapAIObj* pObj )
{
	//测试代码, 实际上需要先到服务器询问是否有进一步内容
	//检测已接可接任务的状态

	if(pMainMenu->m_pMissionMenu->CheckNpcQ(pObj->m_nID) == false)
	{
		if ((pObj->m_nID/100 > 49)&&(pObj->m_nID/100 < 52)) //钓鱼
		{
			if (!CheckSkill(MAKE_SKILL_TYPE_FISH))
			{
				pMainMenu->PopTipMsg("请先学习钓鱼技能");
				return;
			}

			if (xnGetTickCount() < m_dwBeHurtTick+3000)//受攻击3秒内无法钓鱼
			{
				pMainMenu->PopTipMsg("正在受到攻击，无法钓鱼！");
				return;
			}

			m_bFish = true;
			SendCmd(USER,C_REQUEST_FISH);
			return;
		}
		if(pObj->m_nID == 1161)
		{
			pMainMenu->m_pMarryNpcMenu->Open();
			return;
		}
		if(pObj->m_nID == 1162)
		{
			pMainMenu->m_pMentoringMenu->m_pNpcForm->Open();
			return;
		}
		char*	szTalk	= NULL;
		if(pObj->pPathObj)
		{
			if (pObj->pPathObj->nShopIndex != -1)
			{
				if (pObj->pPathObj->nShopIndex <=100)
				{
					pMainMenu->m_pShopMenu->Open(pObj->pPathObj->nShopIndex);
				}else if (pObj->pPathObj->nShopIndex > 1000)
				{
					BuildPacketEx(pSendPacket, USER, C_REQUEST_NPC_FORM, buf, 256);
					SET_DATA(pSend, USER, C_REQUEST_NPC_FORM, pSendPacket);
					pSend->nNpcCmd	= pObj->pPathObj->nShopIndex;
					SendPacket(pSendPacket);
					//pMainMenu->m_pBankMenu->Open();	//1001
				}
				return;
				//pMainMenu->m_pShopMenu->m_pShopEx->m_nShopType = pPath->nShopIndex;
			}
			if(pObj->pPathObj->pListTalk->Count)
				szTalk	= (char*)pObj->pPathObj->pListTalk->Items[rand()%pObj->pPathObj->pListTalk->Count];
			if(szTalk)
			{
				pMainMenu->PopTalkMsg(pObj->pPathObj->szName,szTalk,pObj->pPathObj->nSkin);		
			}
		}
		
	}
}

void UserData::UpdateSec()
{
#ifndef _DEBUG
	CheckNetLink();
#endif
	if(GetMyMapObj() == NULL)
		return;
	int nDisLimit	= 100000;
	m_pCQuestCenter->UpdateSec();
	m_pOnlineAwardCenter->UpdateSec();
	m_pMounthAwardCenter->UpdateSec();
	m_pLvupAwardCenter->UpdateSec();
	m_pLoginAwardCenter->UpdateSec();
	m_pRechargeAwardCenter->UpdateSec();
	if(m_bManualSel)
		nDisLimit	= 300000;
	if(g_pMapCenter->m_pMObjSel == NULL || 
		(g_pMapCenter->m_pMObjSel->byIsEnemy && g_pMapCenter->m_pMObjSel->nHp == 0)
		|| (CalcDistance2(GetMyMapObj()->nPosX,GetMyMapObj()->nPosY,g_pMapCenter->m_pMObjSel->nPosX,g_pMapCenter->m_pMObjSel->nPosY)>nDisLimit))
	{
		g_pMapCenter->SearchNearObj(2);
		m_bManualSel	= false;
	}

	if(pChar->m_szbyActLock[6] && m_pHorseCenter->m_ifRide)
	{
		m_pHorseCenter->RequestRide(false);
	}


	//if(g_pMapCenter->m_pMObjSel)
	//{
	//	MapSprObj*	pMine	= g_pMapCenter->m_pMapActCtrl->GetObjByID(MAPOBJ_PLAYER,m_dwUin);
	//	if(pMine)
	//	{
	//		g_pMapCenter->AddMapEff(112,(int)pMine,(int)g_pMapCenter->m_pMObjSel,-1);
	//	}
	//}
	static DWORD	s_dwTickHeatBeat	= xnGetTickCount();
	if(xnGetTickCount() - s_dwTickHeatBeat > 20000)
	{
		s_dwTickHeatBeat	= xnGetTickCount();
		pChar->SendCmd(ACCOUNT,CLIENT_HEATBEAT);
	}

	//if(m_byLvl >= 20 && g_pTPObj->TP_GetPlat() == 10)
	//{//APP是否去评分
	//	static	bool	s_bAskGoToApp	= false;
	//	if(!s_bAskGoToApp)
	//	{
	//		s_bAskGoToApp	= true;
	//		int	nTickSec	= LoadIntegerFromXML("gotomarkapp",1);
	//		if(nTickSec != 0)
	//		{
	//			if(nTickSec == 1 || xnGetSecCount() - nTickSec > 86400)
	//			{
	//				g_pTPObj->TP_OtherEvent(1);
	//				nTickSec	= xnGetSecCount();
	//				SaveIntegerToXML("gotomarkapp",nTickSec);
	//				SaveToXMLOK();
	//			}
	//		}
	//	}
	//	else if(g_pTPObj->nEventMark == 1)
	//	{
	//		SaveIntegerToXML("gotomarkapp",0);
	//		SaveToXMLOK();
	//		g_pTPObj->nEventMark	= 99;
	//	}
	//}
}

BYTE UserData::GetHomeCopyLv()
{
	int nLvl = 0;
	int nCnt = 0;
	for(int i = 0; i < TEAM_MAXUSER; i++)
	{
		if (pMainMenu->m_pTeamMenu->m_xUser[i].dwUin)
		{
			nLvl += pMainMenu->m_pTeamMenu->m_xUser[i].nLv;
			nCnt++;
		}
	}
	return nCnt>0?nLvl/nCnt:m_byLvl;
}

void UserData::ClearTeamData()
{
	memset(pMainMenu->m_pTeamMenu->m_xUser,0,TEAM_MAXUSER*sizeof(_TeamNet));
}

void UserData::SendTeamState( BYTE nType,char* szMsg /*= NULL*/,BYTE byPos )
{
	if (nType == TEAM_COMMIT_REFUSE)
		ClearTeamData();

	BuildPacketEx(pPacket, USER, CS_TEAM_STATE, buf, 1024);
	SET_DATA(pSend, USER, CS_TEAM_STATE, pPacket);
	pSend->byType = nType;
	pSend->byPos = byPos;
	if (szMsg)
	{
		int nSize = strlen(szMsg)+1;
		pPacket->wSize += nSize;
		memcpy(pSend->szMsg,szMsg,nSize);
	}
	pChar->SendPacket(pPacket);
}

void UserData::SendTeamState( BYTE nType,BYTE byVal)
{
	if (nType == TEAM_COMMIT_REFUSE)
		ClearTeamData();

	BuildPacketEx(pPacket, USER, CS_TEAM_STATE, buf, 1024);
	SET_DATA(pSend, USER, CS_TEAM_STATE, pPacket);
	pSend->byType = nType;
	pSend->byPos = byVal;
	pSend->szMsg[0] = byVal;
	pChar->SendPacket(pPacket);
}

void UserData::InvitePlayer( DWORD dwUin )
{
	if (dwUin == pChar->m_dwUin)
	{
		return;
	}
	BuildPacketEx(pPacket, USER, C_TEAM_INVITE, buf, 1024);
	SET_DATA(pSend, USER, C_TEAM_INVITE, pPacket);
	pSend->dwUin = dwUin;
	pChar->SendPacket(pPacket);

	PreTeamPlayer(dwUin);
}

void UserData::PreTeamPlayer( DWORD dwUin )
{
	if (dwUin == pChar->m_dwUin)
	{
		return;
	}
	for (int j = 0; j < TEAM_MAXUSER; j++)
	{
		if (pMainMenu->m_pTeamMenu->m_xUser[j].dwUin == dwUin)
			return;
	}
	pMainMenu->m_pTeamMenu->m_xUser[0].dwUin = m_dwUin;
	for (int i = 0; i < TEAM_MAXUSER; i++)
	{
		if (!pMainMenu->m_pTeamMenu->m_xUser[i].dwUin)
		{
			pMainMenu->m_pTeamMenu->m_xUser[i].dwUin = dwUin;
			pMainMenu->m_pTeamMenu->m_xUser[i].nState = TEAM_CREATE;
			break;
		}
	}
}

bool UserData::_checkUseSkill( WORD wSkill, MapAIObj* pObj )
{
	MapAIObj*	pMapObj		= GetMyMapObj();

	if(pChar->m_szbyActLock[3])
		return false;
	
	SkillBase* pSBase	= SkillConfig::GetInstance().GetSkillBase(wSkill);
	if(!pSBase)
	{
		g_pMapCommand->m_wTrySkill	= 0;
		return false;
	}
	else if(m_pSkillCenter->CheckUserSkill(wSkill) == false)
	{
		g_pMapCommand->m_wTrySkill	= 0;
		return false;
	}
	pChar->m_pHorseCenter->RequestRide(false);

	if(pSBase->bAoeType == 0)
	{
		if(pMapObj->IsAtkCD() == false)
		{
			g_pMapCommand->m_wTrySkill	= 0;
			pMapObj->SetSkillAtk(NULL);
			//发包给服务器发起
			_sendAtkTarget(wSkill,0,0);
		}
		return true;
	}
	if(pObj == NULL)
		return false;
	int nTLen	= CalcDistance(pObj->nPosX,pObj->nPosY,pMapObj->nPosX,pMapObj->nPosY);
	
	if(pSBase->bDmgType == 0 || pSBase->bDmgType == 3)
	{//加血
		if(nTLen <= pSBase->wAtkRange)
		{
			if(pObj->bForce == pMapObj->bForce)
			{
				if(pMapObj->IsAtkCD() == false)
				{
					g_pMapCommand->m_wTrySkill	= 0;
					pMapObj->SetSkillAtk(pObj);
					//发包给服务器发起
					_sendAtkTarget(wSkill,pObj->m_byType,pObj->m_nID);
				}
				return true;
				
			}
		}
	}
	else if((pObj->bForce && pObj->bForce != pMapObj->bForce))
	{
		if(nTLen <= pSBase->wAtkRange)
		{//攻击范围内
			
			if(pMapObj->IsAtkCD() == false)
			{
				g_pMapCommand->m_wTrySkill	= 0;
				pMapObj->SetSkillAtk(pObj);
				//发包给服务器发起
				_sendAtkTarget(wSkill,pObj->m_byType,pObj->m_nID);
				//g_pMapCommand->Reset();

			}
			return true;
		}
	}

	if(nTLen > pSBase->wAtkRange)
	{
		int nDX	= (pObj->nPosX-pMapObj->nPosX)* (nTLen-pSBase->wAtkRange+30)/nTLen+pMapObj->nPosX;
		int nDY	= (pObj->nPosY-pMapObj->nPosY)* (nTLen-pSBase->wAtkRange+30)/nTLen+pMapObj->nPosY;
		//pMapObj->_setMove(nDX,nDY);
		if(pMapObj->objAIModel.SetCmd(MCMD_RUN,(nDY<<16)+nDX))
			_sendMoveTarget(nDX,nDY);
		return true;
	}

	return false;
}

DWORD UserData::GetServerTime()
{
	return xnGetTickCount()/1000-m_dwUpdateTime+m_dwServerTime+5;
}

int UserData::GetScore()
{
	int nScore = 0;
	static int nAttrScore[EQUIP_ATTR_MAX] = {4,6,12,12,15,15,20,30,40};
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		nScore += m_xUserParam.nParam[i]*nAttrScore[i];
	}
	switch(m_byJob)
	{
	case 0:
		return 0;
	case 1:
		return nScore/30.5;
	case 2:
		return nScore/29.2;
	case 3:
		return nScore/34.4;
	case 4:
		return nScore/28.5;
	}
	return nScore;
}

bool UserData::IsNewMail()
{
	for (std::map<int,MailObj>::iterator itor = m_pMailList.begin();itor != m_pMailList.end();itor++)
	{
		if (itor->second.byState == STATE_MAIL_SEND)
		{
			return true;
		}
	}
	return false;
}

bool UserData::AccountPacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup	!= ACCOUNT)
		return false;

	switch(pPacket->bCmd)
	{
	case S_JOINGAME:
		{
			SET_DATA(pRecv, ACCOUNT, S_JOINGAME, pPacket);

			m_dwHomeID	= pRecv->wHomeID;
			m_dwHomeDuty = pRecv->byHomeDuty;
			//	m_byVipLvl	= pRecv->byVipLvl;
			strncpy(m_szNick,pRecv->szNick,20);

			pMainMenu->PopSystemMsg("正在进入游戏",true);

			CheckLoadCfgOk();

			m_dwTickJoin	= xnGetTickCount();

			GameScene*	pGameScene	= GameScene::create ();
			cocos2d::CCDirector::sharedDirector()->replaceScene(pGameScene);
		}
		return true;
	case OUT_BY_RELOGIN:
		{
			pMainMenu->PopSystemMsg("账号在别处登录,请确认账号安全性");
			m_bLockReconnet	= true;
			ReconnectModel::GetInstance().SetModelWorked(false);
		}
		return true;
	case OUT_BY_KICK:
		{
			pMainMenu->PopSystemMsg("您已经被移出游戏");
			m_bLockReconnet	= true;
			ReconnectModel::GetInstance().SetModelWorked(false);
		}
		return true;
	case OUT_BY_BUG:
		{
			pMainMenu->PopSystemMsg("账号存在异常!");
			m_bLockReconnet	= true;
			ReconnectModel::GetInstance().SetModelWorked(false);
		}
		return true;
	}
	return false;
	
}

void UserData::MuliThreadInitCfg()
{
	static bool	s_loadSingle	= false;
	if(s_loadSingle == false)
	{
		s_loadSingle	= true;
		SkillConfig::GetInstance ().LoadFile ("cfg/SkillConfig.txt");
		SkillConfig::GetInstance ().LoadFileJob ("cfg/jobskill.txt");
		StateConfig::GetInstance ().LoadFile ("cfg/StateConfig.txt");
		ItemConfig::GetInstance().LoadFile("cfg/itemcfg.txt");
		EquipConfig::GetInstance().LoadFile("cfg/EquipConfig.txt");
		HorseBaseCfg::GetInstance().LoadFile("cfg/HorseBase.txt");
		HorseBaseCfg::GetInstance().LoadLvFile("cfg/HorseLvConfig.txt");
		SlaveBaseCfg::GetInstance().LoadFile("cfg/SlaveBase.txt");
		SlaveBaseCfg::GetInstance().LoadLvFile("cfg/SlaveLvConfig.txt");
		SlaveStrengthenCfg::GetInstance().LoadFile("cfg/SlaveStrengthen.txt");
		SlaveVariatCfg::GetInstance().LoadFile("cfg/SlaveVariation.txt");
		EquipStarConfig::GetInstance().LoadFile("cfg/EquipStarCfg.txt");
		EquipEatConfig::GetInstance ().LoadFile ("cfg/EquipAbsorbCfg.txt");
		NpcPathLeadCfg::GetInstance().LoadFile("cfg/NpcCfg.txt");
		MonsterPathLeadCfg::GetInstance().LoadFile("cfg/enemypos.txt");
		EnemyBaseCfg::GetInstance().LoadFile("cfg/enemybase.txt");
		ACBossCfg::GetInstance().LoadFile("cfg/bossinfo.txt");

		UserConfig::GetInstance().LoadFile("cfg/playerCfg.txt");
		MallConfig::GetInstance().LoadFile("cfg/mallCfg.txt");
		ShopConfig::GetInstance().LoadFile("cfg/shopCfg.txt");
		QuestCfg::GetInstance().LoadFile("cfg/questcfg.txt");
		
		TransPointCfg::GetInstance().LoadFile("cfg/MapTransCfg.txt");
		SkillConfig::GetInstance().LoadFile("cfg/SkillConfig.txt");
		StateConfig::GetInstance().LoadFile("cfg/StateConfig.txt");
		MCopyConfig::GetInstance().LoadFile("cfg/mapcopycfg.txt");
		TestAwardCfg::GetInstance().LoadFile("cfg/TestAward.txt");
		TestLibraryCfg::GetInstance().LoadFile("cfg/TestLibrary.txt");

		VipConfig::GetInstance().LoadFile("cfg/VipConfig.txt");
		TitleConfig::GetInstance().LoadFile("cfg/TitleConfig.txt");
		ActiveDegree::GetInstance().LoadFile("cfg/ActivedegreeCfg.txt");
		GameActivityCfg::GetInstance().LoadFile("cfg/GameActivityCfg.txt");
		TurntableCfg::GetInstance().LoadFile("cfg/Turntablecfg.txt");
		GameAwardConfig::GetInstance().LoadFile("cfg/GameAwardCfg.txt");
		StarSysCfg::GetInstance().LoadFile("cfg/StarSysCfg.txt");

		EquipAttrConfig::GetInstance ().LoadFile ("cfg/EquipAttachCfg.txt");
		MentoringCfg::GetInstance().LoadFile("cfg/MentoringCfg.txt");
		AchievementCfg::GetInstance().LoadFile("cfg/AchievementCfg.txt");
		AchievementCfg::GetInstance().LoadAwardFile("cfg/AchieveAwardCfg.txt");
		MakeSkillConfig::GetInstance().LoadFile("cfg/makeitem.txt");
		MarrySkillCfg::GetInstance().LoadFile("cfg/MarrySkillCfg.txt");
		EquipGroupConfig::GetInstance().LoadFile("cfg/EquipGroupCfg.TXT");
		VipAwardConfig::GetInstance().LoadFile("cfg/VipAwardcfg.txt");
		FaqHelpCfg::GetInstance().LoadFile("cfg/FaqHelpCfg.txt");

		ClientLuaManager::sharedHD()->DoFile("Script/event.lua");
		ClientLuaManager::sharedHD()->DoFile("Script/update.lua");
		ClientLuaManager::sharedHD()->DoFile("Script/packet.lua");
	}
	m_bLoadCfgOK	= true;
}

void UserData::ChgMapEvent()
{
	memset(m_szbyActLock,0,7);
	if (m_bHorseState)
		m_szbyActLock[6] = 1;
}

void fun_cbBackLogin(void * pParam,int nVal)
{
	if(nVal == 1)
		pChar->ReturnLogin();
	else
	{
		pChar->ReConnect();
	}
}

void UserData::InitData()
{
	while(m_pItemCenter->m_pListItems->Count)
	{
		delete (ItemObj*)m_pItemCenter->m_pListItems->Delete(0);
	}
	m_pDataCenter->ClearData();
	m_pCQuestCenter->ClearQuest();
	m_pSlaveCenter->ClearList();
	m_pHorseCenter->ClearList();
	m_pChatCenter->ClearList();
	m_pMentoringClient->ClearList();
	m_pCAchievementCenter->ClearList();
	m_pSkillCenter->ClearSkill();
	m_pMarryCenter->ClearList();
	m_pActivityCenter->m_nShowFlag = 0;
	m_pMailList.clear();
	pMainMenu->StopCast();
	pMainMenu->m_pFriendMenu->Clear();
	pMainMenu->m_pAddFriendMenu->ClearList();
	pMainMenu->m_pFRequestMenu->ClearList();
	memset(pMainMenu->m_pTeamMenu->m_xUser,0,TEAM_MAXUSER*sizeof(_TeamNet));

	m_byLvl			= 1;
	m_dwMoney		= 0;
	m_dwRmb			= 0;
	m_dwBindRmb		= 0;
	m_dwPow			= 0;
	m_dwExp			= 0;
	m_wSkillPt		= 0;
	m_wStarLv		= 0;
	m_byVipLvl		= 0;
	m_dwVipExp		= 0;
	m_bHorseState   = 0;
	m_bManualSel	= false;
	m_dwLockSkill	= 0;
	m_bLockReconnet	= true;
	m_nUserMultipile= 100;
	m_nPetMultipile	= 100;
	pMainMenu->m_pStarSysMenu->SetStarLv(0);

	m_nGetSlaveLmt  = 0;
	m_nLoginDays    = 0;
	m_dwLogoutWaitTick = 0;
	pMainMenu->m_pAutoMenu->m_bifResetPetState = true;
	pMainMenu->m_pBagMenu->m_pForm->pCFrame->nChgX = 0;
	if (pMainMenu->m_pLoginAwardMenu->m_pForm->pCFrame)
		pMainMenu->m_pLoginAwardMenu->m_pForm->pCFrame->nChgY = 0;
}

void UserData::ReConnect()
{
	if(m_bLockReconnet == false && ReconnectModel::GetInstance().GetModelWorked() == false)
	{
		g_packetCenter.DisConnectServer();
		InitData();
		ReconnectModel::GetInstance().SetModelWorked(true);
	}
}

void UserData::ReturnLogin()
{
	g_packetCenter.DisConnectServer();
	InitData();

	LoginScene*	pLogin	= LoginScene::create();
	cocos2d::CCDirector::sharedDirector()->replaceScene(pLogin);
	ReconnectModel::GetInstance().SetModelWorked(false);
	if (pMainMenu->m_bBackLogin)
	{
		pMainMenu->m_bBackLogin = false;
	}else
		pMainMenu->m_bBackLogin = true;	
}

void UserData::CheckNetLink()
{
	DWORD	dwTickCur	= xnGetTickCount();
	if(m_wAliveGSFrm && g_pTPObj->nPayStep == 0)
		m_wAliveGSFrm++;

	if(m_byGSStep == 0 && g_packetCenter.m_dwTickGameSrv && dwTickCur-g_packetCenter.m_dwTickGameSrv> 5000)
	{
		m_wAliveGSFrm	= 1;
		m_byGSStep	= 1;
		SendCmd(USER, CHECK_GAMESVR_ALIVE);
	}
	else if(m_byGSStep == 1 && m_wAliveGSFrm>5 && dwTickCur-g_packetCenter.m_dwTickGameSrv> 15000)
	{//游戏服务器连接断开,视为掉线
		pMainMenu->PopAskMsgBox("您的网络不稳定,是否返回登录界面?",fun_cbBackLogin,this,1);
		m_byGSStep	= 0;
		g_packetCenter.m_dwTickGameSrv	= xnGetTickCount();
		g_packetCenter.m_dwTickMapSrv	= 0;
		m_byMSStep	= 0;
	}
	else if(m_byGSStep && dwTickCur-g_packetCenter.m_dwTickGameSrv < 5000)
	{
		m_wAliveGSFrm	= 0;
		m_byGSStep	= 0;
	}
	if(m_wAliveMSFrm)
		m_wAliveMSFrm++;
	if(m_byMSStep == 0 && g_packetCenter.m_dwTickMapSrv && dwTickCur-g_packetCenter.m_dwTickMapSrv> 5000)
	{
		m_wAliveMSFrm	= 1;
		m_byMSStep	= 1;
		SendCmd(MAPCMD, CHECK_MAPSVR_ALIVE);
	}
	else if(m_byMSStep == 1 && m_wAliveMSFrm>5 && dwTickCur-g_packetCenter.m_dwTickMapSrv> 12000)
	{//返回主城
		m_byMSStep	= 2;
		if(GetMyMapObj() == NULL)
			SendCmd(MAPCMD,C_RQ_BACK_CITY);
	}
	else if(m_byMSStep == 2 && dwTickCur-g_packetCenter.m_dwTickMapSrv> 20000)
	{//地图跳转不会来,提示连接客服或重试
		pMainMenu->PopAskMsgBox("网络出现异常,是否返回登录界面?",fun_cbBackLogin,this,1);
		m_byGSStep	= 0;
		g_packetCenter.m_dwTickGameSrv	= 0;
		m_byMSStep	= 0;
		g_packetCenter.m_dwTickMapSrv	= 0;
	}
	else if(m_byMSStep && dwTickCur-g_packetCenter.m_dwTickMapSrv < 5000)
	{
		m_wAliveMSFrm	= 0;
		m_byMSStep	= 0;
	}

}

void UserData::CheckLoadCfgOk()
{
	FaceConfig::GetInstance().LoadFile("cfg/FaceConfig.txt");

	while(!m_bLoadCfgOK)
	{
#ifdef WIN32
		Sleep(20);
#else
		sleep(0.02f);
#endif
	}
	//pthread_cancel(xLoadCfgThread);
}

bool UserData::CheckSkill( MAKE_SKILL_TYPE byType )
{
	if (byType == MAKE_SKILL_TYPE_DIG)
	{
		return CheckSkill(MAKE_SKILL_TYPE_FORGE)||CheckSkill(MAKE_SKILL_TYPE_STONE)||CheckSkill(MAKE_SKILL_TYPE_SEWING);
	}else if (byType == MAKE_SKILL_TYPE_COLLECT)
	{
		return CheckSkill(MAKE_SKILL_TYPE_FORGE)||CheckSkill(MAKE_SKILL_TYPE_STONE)||CheckSkill(MAKE_SKILL_TYPE_SEWING)||CheckSkill(MAKE_SKILL_TYPE_METALLURGY);
	}else if (byType == MAKE_SKILL_TYPE_FISH)
	{
		return CheckSkill(MAKE_SKILL_TYPE_STONE)||CheckSkill(MAKE_SKILL_TYPE_FOOD);
	}
	for(int i=0;i<nMakeSkillCount;i++){
		if (m_makeSkill.bySkill[i] == byType)
		{
			return true;
		}
	}
	return false;
}

bool UserData::CheckFriendOnline(DWORD dwUin)
{
	std::map<DWORD,bool>::iterator itor = m_mapOnline.find(dwUin);
	if(itor != m_mapOnline.end())
	{
		return itor->second;
	}

	return false;
}

bool UserData::CheckInFight()
{
	if(m_dwTickBattle && xnGetTickCount() - m_dwTickBattle < 5000)
		return true;

	return false;
}

void UserData::UpdateCatchFishSkin()
{
	if (m_bIsFishSkin && xnGetTickCount() - pMainMenu->m_pCastMenu->m_dwCastSTick >= pMainMenu->m_pCastMenu->m_dwCastTime)
	{
		BuildPacketEx(pPacket,USER, C_FISH_RLT, buf,256);
		SET_DATA(pData,USER, C_FISH_RLT, pPacket);
		pData->byFish = 5;
		pData->dwUin = 0;
		SendPacket(pPacket);
		m_bIsFishSkin = false;
	}
}
