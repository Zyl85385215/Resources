#include "stdafx.h"
#include "CallMapEnemy.h"
#include "ios_mapobj_packet.h"
#include "ZNetGamesvr.h"
#include "ServerCfg.h"
#include "FsGameCenter.h"
#include "BossState.h"
#include "MailCenter.h"
#include "MapObjCenter.h"
#include "ZLogFile.h"
#include "LuaStateManagerPlus.h"

extern ZLogFile* g_pLogFile;
extern CLuaStateManager * pLuaStateManager;

CallEnemyCenter::CallEnemyCenter()
{

}

CallEnemyCenter::~CallEnemyCenter()
{

}

DWORD CallEnemyCenter::LockEnemyIDByMap( DWORD nMIndex )
{
	std::map<DWORD,DWORD>::iterator	itor	= m_mapEnemyID.find(nMIndex);
	if(itor	== m_mapEnemyID.end())
	{
		m_mapEnemyID[nMIndex]	= 1;
		return 1;
	}
	itor->second++;
	if(itor->second > 100000000)
		itor->second	= 1;
	return itor->second;
}

void CallEnemyCenter::RefreshmapCopyEnemyID(DWORD dwMIndex)
{
	if (m_mapCopyEnemyID.size() == 0)
		return;

	if (dwMIndex < 1000)
		return;


	std::map<DWORD,WORD>::iterator itor = m_mapCopyEnemyID.find(dwMIndex);
	if (itor != m_mapCopyEnemyID.end())
	{
		m_mapCopyEnemyID.erase(itor);
	}

	return;
}

bool CallEnemyCenter::CheckCopyEnemyRepeat(DWORD dwMIndex, WORD wEnemyID)
{
	if (wEnemyID < 2001 || wEnemyID > 2020)
		return false;
	if (dwMIndex < 1000)
		return false;

	std::map<DWORD,WORD>::iterator itor = m_mapCopyEnemyID.find(dwMIndex);
	if (itor != m_mapCopyEnemyID.end())
	{
		if (itor->second == wEnemyID)
			return true;
		else
			itor->second = wEnemyID;
	}else
	{
		m_mapCopyEnemyID[dwMIndex] = wEnemyID;
	}

	return false;
}

DWORD CallEnemyCenter::CallEnemyByCfg( WORD wBase,int nMIndex, WORD wX, WORD wY,BYTE bForce,WORD wPowEx,char* szTitle,int nScale/* = 0 */)
{
	if (CheckCopyEnemyRepeat(nMIndex,wBase))
		return 0;

	int nX = 0;
	int nY = 0;
	if (nScale>0)
	{
		if (nScale > 1000)
			nScale = 1000;

		nX = wX - nScale+ rand()%(nScale*2);
		nY = wY - nScale+ rand()%(nScale*2);
	}else
	{
		nX = wX;
		nY = wY;
	}

	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;

	BuildPacketEx(pPacket, MAPOBJ,GS_ADDOBJBYCFG,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_ADDOBJBYCFG, pPacket);
	pSend->bForce		= bForce;
	pSend->byType		= MAPOBJ_ENEMY;
	pSend->wPosX		= nX;
	pSend->wPosY		= nY;
	pSend->dwID			= LockEnemyIDByMap(nMIndex);
	pSend->wBase		= wBase;
	pSend->wPowEx		= wPowEx;

	if (szTitle != NULL)
		strcpy(pSend->szTitle,szTitle);

	MapSrvObj* pSrv	= NULL;
	if(nMIndex < 1000)
		pSrv	= MapSelCfg::GetInstance().GetByMapIndex(nMIndex);
	else
	{
		MapCopyObj* pMCopy	= g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(nMIndex);
		if(pMCopy)
			pSrv	= pMCopy->m_pNetSrvLink;
	}
	
	if(pSrv)
		g_pNetCenter->SendMSvrPacket(pPacket,nMIndex,pSrv->pMapSvr);

	return pSend->dwID;
}

void CallEnemyCenter::SetEnemyAct( DWORD dwID,int nMIndex, int nType, DWORD dwTarget,int nTagType )
{
	BuildPacketEx(pPacket, MAPOBJ,GS_SETENEMY,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_SETENEMY, pPacket);
	pSend->dwID			= dwID;
	pSend->bActType		= nType;
	pSend->bTagType		= nTagType;
	pSend->dwTagID		= dwTarget;

	MapSrvObj* pSrv	= NULL;
	if(nMIndex < 1000)
		pSrv	= MapSelCfg::GetInstance().GetByMapIndex(nMIndex);
	else
	{
		MapCopyObj* pMCopy	= g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(nMIndex);
		if(pMCopy)
			pSrv	= pMCopy->m_pNetSrvLink;
	}

	if(pSrv)
		g_pNetCenter->SendMSvrPacket(pPacket,nMIndex,pSrv->pMapSvr);
}
extern void DamagefuncGetInfo (void* param,Rank_Data* pData);
bool CallEnemyCenter::RecvPacket( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup != MAPOBJ)
		return false;

	switch(pPacket->bCmd)
	{
	case MS_ENEMYDEAD:
		{
			SET_DATA(pSend, MAPOBJ,MS_ENEMYDEAD, pPacket);

		}
		return true;
	case MS_ENEMY_BEKILL:
		{
			SET_DATA(pSend, MAPOBJ,MS_ENEMY_BEKILL, pPacket);
			RunLuaFunctionForEnemyBeKill(pSend->wEnemyID,pSend->dwMapIndex,szLuaFunctionOnEnemyBeKill);
		}
		return true;
	case MS_BOSSBORN:
		{
			SET_DATA(pSend, MAPOBJ,MS_BOSSBORN, pPacket);
			BossState::GetInstance().BossBorn(pSend->wBossID,pSend->dwMapIndex,pSend->bWild);
		}
		return true;
	case MS_BOSSDEAD:
		{
			SET_DATA(pSend, MAPOBJ,MS_BOSSDEAD, pPacket);
			BossState::GetInstance().BossDead(pSend->wBossID,pSend->dwMapIndex);
		}
		return true;
	case MS_LICHKILLED:
		{
			SET_DATA(pSend, MAPOBJ,MS_LICHKILLED, pPacket);
			g_pGameCenter->m_pRankCenter->ClearRank(RANK_DAMAGE);
			static int nItem1[] = {7,6,6,5,5,5,4,4,3,3,5};
			static int nItem2[] = {5,4,4,4,3,3,3,2,2,2,4};
			static int nItem3[] = {5,5,4,4,4,3,3,3,2,2,4};

			bool bDead = false;
			if (pSend->nUin[10]&&pSend->nDamage[10])
				bDead = true;

			int nGold = 0;
			int nExp  = 0;
			for (int i=0;i<10;i++)
			{
				if(!pSend->nUin[i]||!pSend->nDamage[i]) continue;
				_MuliParamObj* pObj = new _MuliParamObj;
				pObj->nParam1 = pSend->nUin[i];
				pObj->nParam2 = pSend->nDamage[i];
				g_pGameCenter->m_pRankCenter->CheckRank(RANK_DAMAGE,pObj,true);

				UserData* pUser = g_pGameCenter->GetPlayer(pObj->nParam1);
				if (pUser)
				{
					nExp = pUser->GetWealth(UW_LV)*pUser->GetWealth(UW_LV)*200;
					nGold  = pUser->GetWealth(UW_LV)*1000;
					pUser->m_xUWealth.AddWealth(UW_EXP,bDead?nExp:nExp/2);
					pUser->m_xUWealth.AddWealth(UW_GOLD,bDead?nGold:nGold/2, 1, NULL,UW_GOLD_TUMO);
				}
				g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,pSend->nUin[i],"ÏµÍ³","ÍÀÄ§°ñ½±Àø",0,45013,nItem1[i],true);
				g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,pSend->nUin[i],"ÏµÍ³","ÍÀÄ§°ñ½±Àø",0,45014,nItem2[i],true);
				g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,pSend->nUin[i],"ÏµÍ³","ÍÀÄ§°ñ½±Àø",0,30007,nItem3[i],true);
			}
			
			if (pSend->nUin[10]&&pSend->nDamage[10])
			{
				if (g_pGameCenter->m_pRankCenter->m_pLastDamage)
				{
					delete g_pGameCenter->m_pRankCenter->m_pLastDamage;
				}
				g_pGameCenter->m_pRankCenter->m_pLastDamage = new Rank_Data;
				_MuliParamObj* pObj = new _MuliParamObj;
				pObj->nParam1 = pSend->nUin[10];
				pObj->nParam2 = pSend->nDamage[10];
				DamagefuncGetInfo(pObj,g_pGameCenter->m_pRankCenter->m_pLastDamage);

				g_pGameCenter->m_pActivityCenter->m_bKillActBoss = false;
				UserData* pUser = g_pGameCenter->GetPlayer(pSend->nUin[10]);
				if (pUser)
				{
					pUser->m_xUWealth.AddWealth(UW_EXP,pUser->GetWealth(UW_LV)*pUser->GetWealth(UW_LV)*200);
					pUser->m_xUWealth.AddWealth(UW_GOLD,pUser->GetWealth(UW_LV)*1000,1,NULL,UW_GOLD_TUMO);

					char szMsg[128];
					sprintf(szMsg,"[%s]µÄ[%s]·ÜÆðÒ»»÷£¬»÷É±ÁËÎ×ÑýÍõ£¬°¬ÔóÀ­Ë¹ÓÖ»Ö¸´ÁËÍùÈÕµÄÆ½¾²£¡",pUser->m_byCountry == 1?(char*)"ÁªÃË":(char*)"²¿Âä",pUser->m_szNick);
					ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);
				}
				g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,pSend->nUin[10],"ÏµÍ³","ÍÀÄ§°ñ½±Àø",0,45013,nItem1[10],true);
				g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,pSend->nUin[10],"ÏµÍ³","ÍÀÄ§°ñ½±Àø",0,45014,nItem2[10],true);
				g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,pSend->nUin[10],"ÏµÍ³","ÍÀÄ§°ñ½±Àø",0,30007,nItem3[10],true);
			}

			if (bDead == false)
				return true;
			
			xnList* pMapUserList = g_pGameCenter->m_pMapObjCenter->GetMapObjByIndex(63)->m_pList;
			if (pMapUserList->Count)
			{
				for (int k = 0; k < pMapUserList->Count; k++)
				{
					UserData* pUser = (UserData*)pMapUserList->Items[k];
					if (pUser&&pUser->m_dwUin)
					{
						bool bNotInRank = true;
						for (int j = 0; j < 11; j++)
						{
							if(pSend->nUin[j] && pSend->nUin[j] == pUser->m_dwUin) 
							{
								bNotInRank = false;
								break;
							}
						}

						if (bNotInRank)
						{
							pUser->m_xUWealth.AddWealth(UW_EXP,pUser->GetWealth(UW_LV)*pUser->GetWealth(UW_LV)*200);
							pUser->m_xUWealth.AddWealth(UW_GOLD,pUser->GetWealth(UW_LV)*1000,1,NULL,UW_GOLD_TUMO);
						}
					}
				}
			}
			
		}
		return true;
	}

	return false;
}

void CallEnemyCenter::RunLuaFunctionForEnemyBeKill(int nEnemyID,int nMapID,const char* const szFunctionName)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	sprintf(sztxt,"%s%d",szFunctionName,nEnemyID);
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return;
	}
	LuaFunction<void> lf(loFunction);
	MapServerObj* pObj = g_pGameCenter->m_pMapObjCenter->GetMapObjByIndex(nMapID);
	CLuaPlusSafeFunction_2<void,MapServerObj*> func;
	if(!func(pLuaState, lf, NULL ,pObj))
	{
		g_pLogFile->OutPut("½Å±¾º¯Êý%sÖ´ÐÐ´íÎó",sztxt);
		return;
	}
}

bool CallEnemyCenter::RemoveEnemy( DWORD dwID, int nMIndex )
{
	BuildPacketEx(pPacket, MAPOBJ,GS_REMOVEOBJ,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_REMOVEOBJ, pPacket);
	pSend->byType		= MAPOBJ_ENEMY;
	pSend->dwID			= dwID;

	MapSrvObj* pSrv	= NULL;
	if(nMIndex < 1000)
		pSrv	= MapSelCfg::GetInstance().GetByMapIndex(nMIndex);
	else
	{
		MapCopyObj* pMCopy	= g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(nMIndex);
		if(pMCopy)
			pSrv	= pMCopy->m_pNetSrvLink;
	}

	if(pSrv)
		g_pNetCenter->SendMSvrPacket(pPacket,nMIndex,pSrv->pMapSvr);

	return true;
}

bool CallEnemyCenter::KillMapEnemyByID(DWORD	nMIndex,WORD wEnemyID)
{
	BuildPacketEx(pPacket, MAPOBJ,GS_KILL_MAPENEMY_BYID,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_KILL_MAPENEMY_BYID, pPacket);
	pSend->dwMapIndex		= nMIndex;
	pSend->wEnemyID         = wEnemyID;

	MapSrvObj* pSrv	= NULL;
	if(nMIndex < 1000)
		pSrv	= MapSelCfg::GetInstance().GetByMapIndex(nMIndex);
	else
	{
		MapCopyObj* pMCopy	= g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(nMIndex);
		if(pMCopy)
			pSrv	= pMCopy->m_pNetSrvLink;
	}

	if(pSrv)
		g_pNetCenter->SendMSvrPacket(pPacket,nMIndex,pSrv->pMapSvr);

	return true;
}

bool CallEnemyCenter::KillEnemy( DWORD nMIndex, WORD wX, WORD wY, WORD wRange, DWORD	dwUin )
{
	BuildPacketEx(pPacket, MAPOBJ,GS_KILLENEMY,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_KILLENEMY, pPacket);
	pSend->dwMapIndex		= nMIndex;
	pSend->dwObj			= dwUin;
	pSend->wX				= wX;
	pSend->wY				= wY;
	pSend->wRange			= wRange;

	MapSrvObj* pSrv	= NULL;
	if(nMIndex < 1000)
		pSrv	= MapSelCfg::GetInstance().GetByMapIndex(nMIndex);
	else
	{
		MapCopyObj* pMCopy	= g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(nMIndex);
		if(pMCopy)
			pSrv	= pMCopy->m_pNetSrvLink;
	}

	if(pSrv)
		g_pNetCenter->SendMSvrPacket(pPacket,nMIndex,pSrv->pMapSvr);

	return true;
}
