#include "stdafx.h"
#include "GameActivityCenter.h"
#include "ActivedegreeCfg.h"
#include "ios_activity_packet.h"
#include "UserData.h"
#include "GameActivityCfg.h"
#include "WorldTest.h"
#include "FsGameCenter.h"
#include "UserFlag.h"
#include "BossState.h"
#include "CallMapEnemy.h"

int CompareStartTime(const void* item1,const void* item2)
{
	ActivityObj* pObj1 = *(ActivityObj**)item1;
	ActivityObj* pObj2 = *(ActivityObj**)item2;
	return pObj1->nLvMin - pObj2->nLvMin;
}

GameActivityCenter::GameActivityCenter()
{
	m_pTodayList = xnList::Create();
	m_pOtherList = xnList::Create();
	m_nShowFlag = 1000;
	CheckDaylyList();

	m_nOffsetTime = 0;
	m_pRubbishList = xnList::Create();
	m_bKillActBoss = true;

	m_tmTmp = CMTime::GetOffsetTime(m_nOffsetTime);
	memset(m_bActivityTime,true,24);
}

GameActivityCenter::~GameActivityCenter()
{
	if(m_pTodayList)
	{
		while(m_pTodayList->Count)
			m_pTodayList->Delete(0);
		m_pTodayList->Free();
	}
	if(m_pOtherList)
	{
		while(m_pOtherList->Count)
			m_pOtherList->Delete(0);
		m_pOtherList->Free();
	}
	if (m_pRubbishList)
	{
		while(m_pRubbishList->Count)
			m_pRubbishList->Delete(0);
		m_pRubbishList->Free();
	}
}

bool GameActivityCenter::CheckBeingActive(int nActID)
{
	ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(nActID);
	if (pObj)
	{
		tm tmNow	= CMTime::GetOffsetTime(m_nOffsetTime);
		DWORD dwTemp = (tmNow.tm_hour<<16) + tmNow.tm_min;

		if(dwTemp >= pObj->dwStartTime && dwTemp < pObj->dwEndTime)
		{
			return true;
		}
	}

	return false;
}

bool GameActivityCenter::CheckActiveEnd(int nActID)
{
	ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(nActID);
	if (pObj)
	{
		tm tmNow	= CMTime::GetOffsetTime(m_nOffsetTime);
		DWORD dwTemp = (tmNow.tm_hour<<16) + tmNow.tm_min;

		if(dwTemp >=  pObj->dwEndTime)
		{
			return true;
		}
	}

	return false;
}

void GameActivityCenter::RequestEnter(UserData* pUser,int nID)
{
	tm tmNow	= CMTime::GetOffsetTime(m_nOffsetTime);
	DWORD dwTemp = (tmNow.tm_hour<<16) + tmNow.tm_min;
	if(nID == 1001)
	{
		ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(1001);
		if(pObj->byCountry > 0 && pObj->byCountry != pUser->m_byCountry)
		{
			pUser->SendWarnMsg("只能参加本国活动");
			return;
		}
		if((pObj->nLvMin == 0 || pObj->nLvMin <= pUser->m_xUWealth[UW_LV]) && (pObj->nLvMax == 0 || pObj->nLvMax >= pUser->m_xUWealth[UW_LV]))
		{
			EnterActivity(pUser,pObj);
			return;
		}
	}
	for(int i = 0; i < m_pTodayList->Count; i ++)
	{
		ActivityObj* pObj = (ActivityObj*)m_pTodayList->Items[i];
		if(pObj->nID == nID)
		{
			if(pObj->byCountry > 0 && pObj->byCountry != pUser->m_byCountry)
			{
				pUser->SendWarnMsg("只能参加本国活动");
				return;
			}
			if((pObj->nLvMin == 0 || pObj->nLvMin <= pUser->m_xUWealth[UW_LV]) && (pObj->nLvMax == 0 || pObj->nLvMax >= pUser->m_xUWealth[UW_LV]))
			{
				if(pObj->nID == 1001 || pObj->nID == 1035)
					EnterActivity(pUser,pObj);
				else
				{
					if(dwTemp >= pObj->dwStartTime && dwTemp < pObj->dwEndTime)
					{
						if(pObj->dwRegistTime == pObj->dwStartTime)
						{
							EnterActivity(pUser,pObj);
						}
						else
						{
							if(pObj->dwRegistTime <= dwTemp )
								pUser->SendWarnMsg("活动已开始");
							else
								EnterActivity(pUser,pObj);
						}
					}
					else
					{
						pUser->SendWarnMsg("活动未开放，无法参加");
					}
				}
			}
			else
			{
				pUser->SendWarnMsg("等级不符合要求，无法参加");
			}
			return;
		}
	}
	pUser->SendWarnMsg("活动未开放");
}

void GameActivityCenter::EnterActivity(UserData* pUser,ActivityObj* pObj)
{
	if (pObj->nID == 1019)
	{//远征副本活动
		pUser->CreateExpeditionCopy();
		return;
	}

	if(pObj->byActionEvent == 1)		//界面活动
	{
		switch(pObj->nID)
		{
		case 1001:
			{
				g_pGameCenter->m_pWorldTestCenter->RequestStart(pUser);
			}
			break;
		case 1002:
			{
				TCP_SendCmdToUin(pUser->m_dwUin,RPGACTIVITY,S_SEND_OPEN_TURNTABLE);
			}
			break;
		case 1022:
			{
				if (pUser->m_dwHomeID)
					TCP_SendCmdToUin(pUser->m_dwUin,RPGACTIVITY,S_SEND_OPEN_HOMECOPY);
				else
					pUser->SendWarnMsg("请先加入军团");
			}
			break;
		case 1023:
			{
				if (pUser->m_dwHomeID)
					TCP_SendCmdToUin(pUser->m_dwUin,RPGACTIVITY,S_SEND_OPEN_HOMEQUEST);
				else
					pUser->SendWarnMsg("请先加入军团");
			}
			break;
		}
	}
	else if(pObj->byActionEvent == 2)		//寻NPC
	{
		int nID = atoi(pObj->pObject);
		BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_FINDNPC,buf,1024);
		SET_DATA(pData,RPGACTIVITY,S_SEND_FINDNPC,pPacket);
		pData->nID = nID;
		TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
	}
	else if( pObj->byActionEvent == 3)		//进活动副本
	{
		if(pUser->m_nMIndex >= 1000)
		{
			pUser->SendWarnMsg("请先离开副本");
			return;
		}
		int nID = atoi(pObj->pObject);
		std::map<int,xnList*>::iterator itor = mapActivityMCopy.find(pObj->nID);
		if(itor != mapActivityMCopy.end())
		{
			bool ifDone = false;
			xnList* pList = itor->second;
			if(pList->Count > 0)
			{
				int nIndex = (int)pList->Items[pList->Count-1];
				MapCopyObj* pMCObj = g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(nIndex);
				if(pMCObj && pMCObj->m_pPartList->Count < pMCObj->m_nPartLimit)
				{
					pUser->m_wOldMIndex	= pUser->m_nMIndex;
					pUser->m_wOldPosX		= pUser->m_pMObjCenter->m_pPlayer->nPosX;
					pUser->m_wOldPosY		= pUser->m_pMObjCenter->m_pPlayer->nPosY;
					pUser->m_pMapCopy = g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(nIndex,pUser->m_dwUin);
					if (pUser->m_pMapCopy)
					{
						pUser->JumpTo(pUser->m_pMapCopy->m_dwMapIndex,pUser->m_pMapCopy->m_pMCBase->nStartX,pUser->m_pMapCopy->m_pMCBase->nStartY,pUser->m_pMapCopy->m_pMCBase->wBaseMap);
						ifDone = true;
					}
				}
			}
			if(!ifDone)
			{
				MCopyBase* pBase = MCopyConfig::GetInstance().GetMCopy(nID);
				if(pBase == NULL)
					return;
				pUser->m_wOldMIndex	= pUser->m_nMIndex;
				pUser->m_wOldPosX		= pUser->m_pMObjCenter->m_pPlayer->nPosX;
				pUser->m_wOldPosY		= pUser->m_pMObjCenter->m_pPlayer->nPosY;
				pUser->m_pMapCopy = g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(pUser->m_dwUin,pUser->m_dwUin,pBase);
				if (!pUser->m_pMapCopy)
				{
					return;
				}
				pUser->JumpTo(pUser->m_pMapCopy->m_dwMapIndex,pUser->m_pMapCopy->m_pMCBase->nStartX,pUser->m_pMapCopy->m_pMCBase->nStartY,pUser->m_pMapCopy->m_pMCBase->wBaseMap);
				pUser->m_pMapCopy->m_DeleteLock = true;
				pUser->m_pMapCopy->m_nPartLimit = 500;
				pList->Add((void*)pUser->m_pMapCopy->m_dwMapIndex);
			}
		}
		else
		{
			xnList* pList = xnList::Create();
			MCopyBase* pBase = MCopyConfig::GetInstance().GetMCopy(nID);
			if(pBase == NULL)
				return;
			pUser->m_wOldMIndex	= pUser->m_nMIndex;
			pUser->m_wOldPosX		= pUser->m_pMObjCenter->m_pPlayer->nPosX;
			pUser->m_wOldPosY		= pUser->m_pMObjCenter->m_pPlayer->nPosY;
			pUser->m_pMapCopy = g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(pUser->m_dwUin,pUser->m_dwUin,pBase);
			if (!pUser->m_pMapCopy)
			{
				return;
			}
			pUser->JumpTo(pUser->m_pMapCopy->m_dwMapIndex,pUser->m_pMapCopy->m_pMCBase->nStartX,pUser->m_pMapCopy->m_pMCBase->nStartY,pUser->m_pMapCopy->m_pMCBase->wBaseMap);
			pUser->m_pMapCopy->m_DeleteLock = true;
			pUser->m_pMapCopy->m_nPartLimit = 500;
			pList->Add((void*)pUser->m_pMapCopy->m_dwMapIndex);
			mapActivityMCopy[pObj->nID] = pList;
		}
	}
	else if( pObj->byActionEvent == 4)		//进副本含坐标
	{
		if(pUser->m_nMIndex >= 1000)
		{
			pUser->SendWarnMsg("请先离开副本");
			return;
		}
		int nID,nPosx1,nPosy1,nPosx2,nPosy2,nPosx,nPosy;
		sscanf(pObj->pObject,"%d_%d_%d_%d_%d",&nID,&nPosx1,&nPosy1,&nPosx2,&nPosy2);
		if(pUser->m_byCountry == 1)
		{
			nPosx = nPosx1;
			nPosy = nPosy1;
		}
		else
		{
			nPosx = nPosx2;
			nPosy = nPosy2;
		}
		std::map<int,xnList*>::iterator itor = mapActivityMCopy.find(pObj->nID);
		if(itor != mapActivityMCopy.end())
		{
			bool ifDone = false;
			xnList* pList = itor->second;
			if(pList->Count > 0)
			{
				int nIndex = (int)pList->Items[pList->Count-1];
				MapCopyObj* pMCObj = g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(nIndex);
				if(pMCObj && pMCObj->m_pPartList->Count < pMCObj->m_nPartLimit)
				{
					pUser->m_wOldMIndex	= pUser->m_nMIndex;
					pUser->m_wOldPosX		= pUser->m_pMObjCenter->m_pPlayer->nPosX;
					pUser->m_wOldPosY		= pUser->m_pMObjCenter->m_pPlayer->nPosY;
					pUser->m_pMapCopy = g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(nIndex,pUser->m_dwUin);
					if (pUser->m_pMapCopy)
					{
						pUser->JumpTo(pUser->m_pMapCopy->m_dwMapIndex,nPosx,nPosy,pUser->m_pMapCopy->m_pMCBase->wBaseMap);
						ifDone = true;
					}
				}
			}
			if(!ifDone)
			{
				MCopyBase* pBase = MCopyConfig::GetInstance().GetMCopy(nID);
				if(pBase == NULL)
					return;
				pUser->m_wOldMIndex	= pUser->m_nMIndex;
				pUser->m_wOldPosX		= pUser->m_pMObjCenter->m_pPlayer->nPosX;
				pUser->m_wOldPosY		= pUser->m_pMObjCenter->m_pPlayer->nPosY;
				pUser->m_pMapCopy = g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(pUser->m_dwUin,pUser->m_dwUin,pBase);
				if (!pUser->m_pMapCopy)
				{
					return;
				}
				pUser->JumpTo(pUser->m_pMapCopy->m_dwMapIndex,nPosx,nPosy,pUser->m_pMapCopy->m_pMCBase->wBaseMap);
				pUser->m_pMapCopy->m_DeleteLock = true;
				pUser->m_pMapCopy->m_nPartLimit = 500;
				pList->Add((void*)pUser->m_pMapCopy->m_dwMapIndex);
			}
		}
		else
		{
			xnList* pList = xnList::Create();
			MCopyBase* pBase = MCopyConfig::GetInstance().GetMCopy(nID);
			if(pBase == NULL)
				return;
			pUser->m_wOldMIndex	= pUser->m_nMIndex;
			pUser->m_wOldPosX		= pUser->m_pMObjCenter->m_pPlayer->nPosX;
			pUser->m_wOldPosY		= pUser->m_pMObjCenter->m_pPlayer->nPosY;
			pUser->m_pMapCopy = g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(pUser->m_dwUin,pUser->m_dwUin,pBase);
			if (!pUser->m_pMapCopy)
			{
				return;
			}
			pUser->JumpTo(pUser->m_pMapCopy->m_dwMapIndex,nPosx,nPosy,pUser->m_pMapCopy->m_pMCBase->wBaseMap);
			pUser->m_pMapCopy->m_DeleteLock = true;
			pUser->m_pMapCopy->m_nPartLimit = 500;
			pList->Add((void*)pUser->m_pMapCopy->m_dwMapIndex);
			mapActivityMCopy[pObj->nID] = pList;
		}
	}
	else if( pObj->byActionEvent == 5)		//跳地图含坐标
	{
		int nID,nPosx1,nPosy1,nPosx2,nPosy2,nPosx,nPosy;
		sscanf(pObj->pObject,"%d_%d_%d_%d_%d",&nID,&nPosx1,&nPosy1,&nPosx2,&nPosy2);
		if(pUser->m_byCountry == 1)
		{
			nPosx = nPosx1;
			nPosy = nPosy1;
		}
		else
		{
			nPosx = nPosx2;
			nPosy = nPosy2;
		}
		pUser->JumpTo(nID,nPosx,nPosy);
	}
	else if( pObj->byActionEvent == 6)		//开启队伍副本
	{
		int nMCID = atoi(pObj->pObject);
		BuildPacketEx(pPacket,RPGACTIVITY,S_CHECK_OPEN_MAPCOPY,buf,1024);
		SET_DATA(pData,RPGACTIVITY,S_CHECK_OPEN_MAPCOPY,pPacket);
		pData->wMCID = nMCID;
		if (nMCID == 412)
			pData->dwCostGold = 300000;
		else
			pData->dwCostGold = 0;
		TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
	}
}

void GameActivityCenter::FreeActivityMap(int nID,int nMIndex)
{
	std::map<int,xnList*>::iterator itor = mapActivityMCopy.find(nID);
	if(itor != mapActivityMCopy.end())
	{
		xnList* pList = itor->second;
		for(int i = pList->Count-1; i >= 0; i --)
		{
			int nCheck = (int)pList->Items[i];
			if(nCheck == nMIndex)
			{
				MapCopyObj* pMCObj = g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(nMIndex);
				if(pMCObj)
					pMCObj->m_DeleteLock = false;
				pList->Delete(i);
				break;
			}
		}
		if(pList->Count == 0)
			mapActivityMCopy.erase(itor);
	}
}

void GameActivityCenter::RequestForInfo(UserData* pUser,int nFlag)
{
	if(pUser == NULL)
		return;
	if(nFlag == m_nShowFlag)
	{
		BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_ACINFO,buf,1024);
		SET_DATA(pData,RPGACTIVITY,S_SEND_ACINFO,pPacket);
		pData->ifDifferent = false;
		TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
	}
	else
	{
		if(m_pTodayList->Count == 0 && m_pOtherList->Count == 0)
			CheckDaylyList();
		ZPacket* pPacket = NULL;
		BYTE buf[1024]={0};
		RPGACTIVITY_S_SEND_ACINFO_Data* pData = (RPGACTIVITY_S_SEND_ACINFO_Data*)((ZPacket*)buf)->data;
		pData->ifDifferent = true;
		pData->nShowFlag = m_nShowFlag;
		pData->nCnt = 0;
		for(int i = 0; i < m_pTodayList->Count; i ++)
		{
			ActivityObj* pObj = (ActivityObj*)m_pTodayList->Items[i];
			pData->szID[pData->nCnt++] = pObj->nID;
		}
		for(int i = 0; i < m_pOtherList->Count; i ++)
		{
			ActivityObj* pObj = (ActivityObj*)m_pOtherList->Items[i];
			pData->szID[pData->nCnt++] = pObj->nID;
		}
		BuildPacketSize(pPacket,RPGACTIVITY,S_SEND_ACINFO,buf,sizeof(RPGACTIVITY_S_SEND_ACINFO_Data)+pData->nCnt*sizeof(WORD));
		TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
	}
}

void GameActivityCenter::JoinSendActive(UserData* pUser)
{
	if (!pUser)
		return;

	if (pUser->m_xUWealth.GetWealth(UW_LV) < 2)
		return;

	if(m_pTodayList->Count == 0 && m_pOtherList->Count == 0)
		CheckDaylyList();

	tm tmNow	= CMTime::GetOffsetTime(m_nOffsetTime);
	DWORD dwTemp = (tmNow.tm_hour<<16) + tmNow.tm_min;

	ZPacket* pPacket = NULL;
	BYTE buf[1024]={0};
	RPGACTIVITY_S_JION_SEND_ACINFO_Data* pData = (RPGACTIVITY_S_JION_SEND_ACINFO_Data*)((ZPacket*)buf)->data;
	pData->nCnt = 0;

	if (pUser->m_xUWealth.GetWealth(UW_LV) > 9)
	{
		for(int i = 0; i < m_pTodayList->Count; i ++)
		{
			ActivityObj* pObj = (ActivityObj*)m_pTodayList->Items[i];
			if(pObj)
			{
				if(pObj->byCountry > 0 && pObj->byCountry != pUser->m_byCountry)
					continue;

				if((pObj->nLvMin == 0 || pObj->nLvMin <= pUser->m_xUWealth[UW_LV]) && (pObj->nLvMax == 0 || pObj->nLvMax >= pUser->m_xUWealth[UW_LV]))
				{
					if(dwTemp >= pObj->dwStartTime && dwTemp < pObj->dwEndTime)
					{
						pData->szID[pData->nCnt++] = pObj->nID;
					}
				}
			}
		}
	}

	BuildPacketSize(pPacket,RPGACTIVITY,S_JION_SEND_ACINFO,buf,sizeof(RPGACTIVITY_S_JION_SEND_ACINFO_Data)+pData->nCnt*sizeof(WORD));
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

void GameActivityCenter::UpdateSec()
{
// 	CMTime	tmNow	= CMTime::GetCurTime();
// 	if(tmNow.GetTickCount()%60 == 0)
	{
		tm	tmMin	= CMTime::GetOffsetTime(m_nOffsetTime);
		if(/*tmMin.tm_hour == 0 && */tmMin.tm_min == 0 || tmMin.tm_min == 30)
			CheckDaylyList();
		bool ifChange = false;
		DWORD dwTemp = (tmMin.tm_hour<<16) + (tmMin.tm_min);
		for(int i = m_pTodayList->Count-1; i >= 0; i --)
		{
			ActivityObj* pObj = (ActivityObj*)m_pTodayList->Items[i];
			SetActivyTime(pObj,tmMin.tm_hour);
			if (pObj->dwStartTime&&pObj->dwStartTime > dwTemp)
			{
				for (int n = 0; n < m_pRubbishList->Count; n++)
				{
					if ((int)m_pRubbishList->Items[n] == pObj->nID)
					{//GM调时间  重新更新list
						CheckDaylyList();
						m_pRubbishList->Delete(n);
					}
				}
			}

			if (pObj->dwStartTime&&(dwTemp > pObj->dwStartTime)&&(dwTemp < pObj->dwEndTime))
			{
				bool bAddRubbish = true;
				for (int j = 0; j < m_pRubbishList->Count; j++)
				{
					if ((int)m_pRubbishList->Items[j] == pObj->nID)
						bAddRubbish = false;
				}
				if (bAddRubbish)
				{
					SendWarnTipActive(pObj);
					m_pRubbishList->Add((void*)pObj->nID);
				}
			}

			if(pObj->dwEndTime < dwTemp)
			{
				m_pTodayList->Delete(i);
				m_pOtherList->Add(pObj);

				for (int k = 0; k < m_pRubbishList->Count; k++)
				{
					if ((int)m_pRubbishList->Items[k] == pObj->nID)
						m_pRubbishList->Delete(k);
				}

				ifChange = true;
			}
		}
		if(ifChange)
		{
			m_pOtherList->Sort(CompareStartTime);
			m_nShowFlag ++;
		}
		//活动副本自动释放
		std::map<int,xnList*>::iterator itor = mapActivityMCopy.begin();
		if(itor != mapActivityMCopy.end())
		{
			int nAID = itor->first;
			bool ifFind = false;
			for(int k = 0; k < m_pTodayList->Count; k ++)
			{
				ActivityObj* pObj = (ActivityObj*)m_pTodayList->Items[k];
				if(pObj->nID == nAID)
				{
					ifFind = true;
					break;
				}
			}
			if(!ifFind)
			{
				xnList* pList = itor->second;
				while(pList->Count > 0)
				{
					int nCheck = (int)pList->Delete(0);
					MapCopyObj* pMCObj = g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(nCheck);
					if(pMCObj)
						pMCObj->m_DeleteLock = false;
				}
				mapActivityMCopy.erase(itor++);
			}
			else
				itor ++;
		}

		if (m_tmTmp.tm_mday != tmMin.tm_mday)
		{
			m_tmTmp = CMTime::GetOffsetTime(m_nOffsetTime);
			m_bKillActBoss = true;
		}
	}

	if (m_bKillActBoss&&CheckActiveEnd(1020))
	{
		m_bKillActBoss = false;
		g_pGameCenter->m_pCallEnemyCenter->KillEnemy(63,874,699,2000,0);
	}
}

void GameActivityCenter::FilterSendListByAct(xnList* pList,ActivityObj* pObj)
{
	while(pList->Count)
		pList->Delete(0);

	xnList*  pTmpList = xnList::Create();
	g_pGameCenter->GetUinList(pTmpList);
	for (int i = 0; i < pTmpList->Count; i++)
	{
		int nTmpUin = (int)pTmpList->Items[i];
		UserData* pUser = g_pGameCenter->GetPlayer(nTmpUin);
		if (pUser)
		{
			if (pObj->byCountry&&pUser->m_byCountry!=pObj->byCountry)
				continue;

			if ((pObj->nLvMin&&(pObj->nLvMin > pUser->m_xUWealth[UW_LV]))||(pObj->nLvMax&&(pObj->nLvMax < pUser->m_xUWealth[UW_LV])))
				continue;

			pList->Add((void*)nTmpUin);
		}
	}
	pTmpList->Free();
}

void GameActivityCenter::SendWarnTipActive(ActivityObj* pObj)
{
	if (pObj->byActionEvent == 0)
		return;

	xnList* pAllUinList = xnList::Create();
	FilterSendListByAct(pAllUinList,pObj);

	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_CURTIME_ACINFO,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_CURTIME_ACINFO,pPacket);
	pData->wID = pObj->nID;
	TCP_SendPacketToUinList(pAllUinList,pPacket);

	pAllUinList->Free();
}

void GameActivityCenter::CheckDaylyList()
{
	while(m_pTodayList->Count)
		m_pTodayList->Delete(0);
	while(m_pOtherList->Count)
		m_pOtherList->Delete(0);
	tm	tmNow	= CMTime::GetOffsetTime(m_nOffsetTime);
	xnList* pList = GameActivityCfg::GetInstance().m_pList;
	bool ifAdd = false;
	for(int i = 0; i < pList->Count; i ++)
	{
		ifAdd = false;
		ActivityObj* pObj = (ActivityObj*)pList->Items[i];
		if(pObj->pWeekList->Count >= 7)
		{
			m_pTodayList->Add((void*)pObj);
			ifAdd = true;
		}
		else
		{
			for(int j = 0; j < pObj->pWeekList->Count; j ++)
			{
				int n = (int)pObj->pWeekList->Items[j];
				if(n == 7)
					n = 0;
				if(n == tmNow.tm_wday)
				{
					m_pTodayList->Add((void*)pObj);
					ifAdd = true;
				}
			}
			if(!ifAdd)
				m_pOtherList->Add((void*)pObj);
		}
	}
	m_pTodayList->Sort(CompareStartTime);
	m_pOtherList->Sort(CompareStartTime);
	m_nShowFlag ++;
}

void GameActivityCenter::ResetActiveDegree(UserData* pUser)
{
	if(pUser == NULL)
		return;
	int nTotalCnt = ActiveDegree::GetInstance().m_pDaylyList->Count;
	int i = 0;
	for(; i < nTotalCnt; i ++)
	{
		UserDegreePart* pTemp = NULL;
		if(i < pUser->m_pDegreeList->Count)
		{
			pTemp = (UserDegreePart*)pUser->m_pDegreeList->Items[i];
		}
		else
		{
			pTemp = new UserDegreePart();
			pUser->m_pDegreeList->Add(pTemp);
		}
		pTemp->byCount = 0;
		pTemp->ifDone = false;
		ADDaylyPart* pPart = (ADDaylyPart*)ActiveDegree::GetInstance().m_pDaylyList->Items[i];
		pTemp->byType = rand()%pPart->pList->Count;	
	}
	if(i < pUser->m_pDegreeList->Count)
	{
		while(i < pUser->m_pDegreeList->Count)
			delete (UserDegreePart*)pUser->m_pDegreeList->Delete(pUser->m_pDegreeList->Count-1);
	}
	pUser->m_byDegreeLevel = 0;
	SaveActiveDegree(pUser);
	pUser->m_pDB->ExecuteSQL(false,"update "RPG_USER_TABLE" set degreelevel = %d where uin = %d",pUser->m_byDegreeLevel,pUser->m_dwUin);
	SendActiveDegree(pUser,-1);
}

void GameActivityCenter::ResetActiveDegree(UserData* pUser, BYTE byPos)
{
	if(pUser == NULL)
		return;
	if(byPos >= pUser->m_pDegreeList->Count || byPos >= ActiveDegree::GetInstance().m_pDaylyList->Count)
		return;
	UserDegreePart* pTemp = (UserDegreePart*)pUser->m_pDegreeList->Items[byPos];
	ADDaylyPart* pPart = (ADDaylyPart*)ActiveDegree::GetInstance().m_pDaylyList->Items[byPos];
	if(pTemp->byType >= pPart->pList->Count)
		return;
	if(pTemp->ifDone)
		return;
	pTemp->byType = rand()%pPart->pList->Count;
	pTemp->byCount = 0;
	SaveActiveDegree(pUser);
	SendActiveDegree(pUser,byPos);
}

void GameActivityCenter::AddDegreeVal(UserData* pUser,BYTE byPos,int nVal)
{
	if(pUser == NULL)
		return;
	nVal = 1;
	if(byPos >= pUser->m_pDegreeList->Count || byPos >= ActiveDegree::GetInstance().m_pDaylyList->Count)
		return;
	UserDegreePart* pTemp = (UserDegreePart*)pUser->m_pDegreeList->Items[byPos];
	ADDaylyPart* pPart = (ADDaylyPart*)ActiveDegree::GetInstance().m_pDaylyList->Items[byPos];
	if(pTemp->byType >= pPart->pList->Count)
		return;
	ADPartChoose* pPartB = (ADPartChoose*)pPart->pList->Items[pTemp->byType];
	if(pTemp->byCount < pPartB->nCount)
	{
		pTemp->byCount += nVal;
		if(pTemp->byCount >= pPartB->nCount)
			pTemp->ifDone = true;
		SaveActiveDegree(pUser);
		SendActiveDegree(pUser,byPos);
	}
}

void GameActivityCenter::SaveActiveDegree(UserData* pUser)
{
	if(pUser == NULL)
		return;
	int nSize = 0;
	BYTE* pData = NULL;
	nSize = pUser->m_pDegreeList->Count*3 + 4;
	pData = (BYTE*)malloc(nSize);
	BYTE* p = pData;
	*(int*)p = pUser->m_pDegreeList->Count;
	p = p + 4;
	for(int i = 0; i < pUser->m_pDegreeList->Count; i++)
	{
		UserDegreePart* pTemp = (UserDegreePart*)pUser->m_pDegreeList->Items[i];
		*p = pTemp->byType;
		p++;
		*p = pTemp->byCount;
		p++;
		*p = pTemp->ifDone;
		p++;
	}
	pUser->m_pDB->SetBLOB(false, pData, nSize, "UPDATE "RPG_USER_TABLE" set activedegree = ? where uin = %u",pUser->m_dwUin);
}

void GameActivityCenter::SendActiveDegree(UserData* pUser,int nPos)
{
	if(pUser == NULL || nPos >= pUser->m_pDegreeList->Count || nPos < -1)
		return;
	BYTE byBuf[1024]={0};
	ZPacket* pPacket = NULL;
	RPGACTIVITY_S_SEND_ACDEGREE_Data* pData = (RPGACTIVITY_S_SEND_ACDEGREE_Data*)((ZPacket*)byBuf)->data;
	if(nPos == -1)
	{
		pData->nCnt = pUser->m_pDegreeList->Count;
		if(pData->nCnt == 1)
			pData->byPos = 0;
		for(int i = 0; i < pUser->m_pDegreeList->Count; i ++)
		{
			UserDegreePart* pTemp = (UserDegreePart*)pUser->m_pDegreeList->Items[i];
			memcpy(&pData->szDegree[i],pTemp,sizeof(UserDegreePart));
		}
	}
	else
	{
		pData->nCnt = 1;
		pData->byPos = nPos;
		UserDegreePart* pTemp = (UserDegreePart*)pUser->m_pDegreeList->Items[nPos];
		memcpy(&pData->szDegree[0],pTemp,sizeof(UserDegreePart));
	}
	pData->byDLv = pUser->m_byDegreeLevel;
	BuildPacketSize(pPacket,RPGACTIVITY,S_SEND_ACDEGREE,byBuf,sizeof(RPGACTIVITY_S_SEND_ACDEGREE_Data) + pData->nCnt * sizeof(UserDegreePart));
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

void GameActivityCenter::LoadActiveDegree(UserData* pUser,BYTE* byData,int nSize)
{
	if(pUser == NULL || byData == NULL || nSize < 4)
		return;
	BYTE* p = byData;
	int nCnt = *(int*)p;
	p = p + 4;
	nSize -= 4;
	while(nSize >= 3)
	{
		UserDegreePart* pTemp = new UserDegreePart();
		pUser->m_pDegreeList->Add(pTemp);
		pTemp->byType = *p;
		p++; 
		nSize --;
		pTemp->byCount = *p;
		p++;
		nSize--;
		pTemp->ifDone = *p;
		p++;
		nSize--;
	}
}

int GameActivityCenter::GetDegreeRlt(UserData* pUser)
{
	if(pUser == NULL)
		return 0;
	int nRlt =0;
	for(int i = 0; i < pUser->m_pDegreeList->Count; i ++)
	{
		UserDegreePart* pTemp = (UserDegreePart*)pUser->m_pDegreeList->Items[i];
		if(pTemp->ifDone)
		{
			ADDaylyPart* pPartA = (ADDaylyPart*)ActiveDegree::GetInstance().m_pDaylyList->Items[i];
			nRlt += pPartA->nPoint;
		}
	}
	return nRlt;
}

bool GameActivityCenter::RequestDegreeAward(UserData* pUser)
{
	if(pUser == NULL || pUser->m_byDegreeLevel >= MAX_DEGREE_LEVEL)
		return false;
	int nGetPoint = 0;
	for(int k = 0; k < pUser->m_pDegreeList->Count; k ++)
	{
		UserDegreePart* pTempPart = (UserDegreePart*)pUser->m_pDegreeList->Items[k];
		ADDaylyPart* pPartA = (ADDaylyPart*)ActiveDegree::GetInstance().m_pDaylyList->Items[k];
		if(pTempPart->ifDone)
			nGetPoint += pPartA->nPoint;
	}

	int nLv = ActiveDegree::GetInstance().CheckLevel(nGetPoint);

	if(nLv <= pUser->m_byDegreeLevel)
		return false;

	while(nLv > pUser->m_byDegreeLevel)
	{
		for(int i = 0; i < ActiveDegree::GetInstance().szAward[pUser->m_byDegreeLevel].pList->Count; i ++)
		{
			ADAwardPart* pAward = (ADAwardPart*)ActiveDegree::GetInstance().szAward[pUser->m_byDegreeLevel].pList->Items[i];
// 			switch(pAward->nID)
// 			{
// 			case 444:
// 				{
// 					pUser->m_xUWealth.AddWealth(UW_HONOUR,pAward->nCnt);
// 				}
// 				break;
// 			case 555:
// 				{
// 					pUser->m_xUWealth.AddWealth(UW_EXP,pAward->nCnt);
// 				}
// 				break;
// 			case 666:
// 				{
// 					pUser->m_xUWealth.AddWealth(UW_GOLD,pAward->nCnt);
// 				}
// 				break;
// 			case 777:
// 				{
// 					pUser->m_xUWealth.AddWealth(UW_POW,pAward->nCnt);
// 				}
// 				break;
// 			case 888:
// 				{
// 					pUser->m_xUWealth.AddWealth(UW_RMB,pAward->nCnt);
// 				}
// 				break;
// 			default:
// 				{
// 					pUser->m_pItemCenter->AddItemByID(pAward->nID,pAward->nCnt,true);
// 				}
// 				break;
// 			}
			int nVal	= pAward->nCnt;
			if(pAward->nID == 666 && pAward->nCnt < 1000)
			{
				nVal	= pAward->nCnt*pUser->m_xUWealth[UW_LV]*pUser->m_xUWealth[UW_LV];
			}

			if(pAward->nID > 0)
				pUser->AddItemEx(pAward->nID,nVal,RMB_SAVE_DEGREEAWARD,UW_GOLD_DEGREEAWARD,ITEM_LOG_DEGREEAWARD); 
		}
		pUser->m_byDegreeLevel ++;
	}
	
	pUser->m_pDB->ExecuteSQL(false,"update "RPG_USER_TABLE" set degreelevel = %d where uin = %d",pUser->m_byDegreeLevel,pUser->m_dwUin);
	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_ACDLV,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_ACDLV,pPacket);
	pData->byDlv = pUser->m_byDegreeLevel;
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
	return true;
}


void GameActivityCenter::RequestTurntableStart(UserData* pUser)
{
	pUser->m_pTurntableCenter->Start();
}

void GameActivityCenter::RequestTurntableStartForTen(UserData* pUser)
{
	pUser->m_pTurntableCenter->StartForTen();
}

void GameActivityCenter::RequestTurntableGift(UserData* pUser,int nPos)
{
	pUser->m_pTurntableCenter->RequestForGift(nPos);
}

void GameActivityCenter::RequestBossState(UserData* pUser)
{
	if(pUser == NULL)
		return;
	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_BOSSINFO,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_BOSSINFO,pPacket);

	int nCount = ACBossCfg::GetInstance().m_pList->Count;
	int nCnt = 0;
	for (int i = 0; i < nCount; i++)
	{
		BossInfoObj* pTemp = (BossInfoObj*)ACBossCfg::GetInstance().m_pList->Items[i];
		if (BossState::GetInstance().CheckBossActive(pTemp->nID))
		{
			pData->wBossID[nCnt] = pTemp->nID;
			nCnt++;
		}
	}
	pData->nCnt = nCnt;
	pPacket->wSize	+= (nCnt-1)*sizeof(WORD);
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

void GameActivityCenter::SetActivyTime( ActivityObj* pActivityObj,int nHour )
{
	if (!pActivityObj)
		return;
	if (pActivityObj->nID == 1035)
	{
		if (nHour == 10 && m_bActivityTime[10])
		{
			pActivityObj->dwStartTime = nHour<<16;
			pActivityObj->dwEndTime = (nHour<<16)+10;
			m_bActivityTime[10] = false;
		}
		if (nHour == 15 && m_bActivityTime[15])
		{
			pActivityObj->dwStartTime = nHour<<16;
			pActivityObj->dwEndTime = (nHour<<16)+10;
			m_bActivityTime[15] = false;
		}
		if (nHour == 22 && m_bActivityTime[22])
		{
			pActivityObj->dwStartTime = nHour<<16;
			pActivityObj->dwEndTime = (nHour<<16)+10;
			m_bActivityTime[22] = false;
		}
	}
}
