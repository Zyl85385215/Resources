#include "stdafx.h"
#include "BossState.h"
#include "GameActivityCfg.h"
#include "ChatCenter.h"
#include "MapConfig.h"

CreateSingleton(BossState);

//BossInfo::BossInfo()
//{
//	pListMap	= xnList::Create();
//}
//
//BossInfo::~BossInfo()
//{
//	pListMap->Free();
//}


BossState::BossState()
{

}

BossState::~BossState()
{
	std::map<WORD,bool>::iterator	itor	= mapBossActive.begin();
	while(itor	!= mapBossActive.end())
	{
		//delete itor->second;
		mapBossActive.erase(itor++);
	}
}

void BossState::BossBorn(WORD wBoss, int nMap,bool bWild)
{
	//BossInfo*	pBInfo	= GetBInfo(wBoss);

	//pBInfo->pListMap->Add((void*)nMap);
	if (!bWild)
	{
		return;
	}

	BossInfoObj* pBossInfo = ACBossCfg::GetInstance().GetObj(wBoss);
	if (!pBossInfo)
		return;

	MapTileData* pMap = MapViewConfig::GetInstance().GetMapTileData(pBossInfo->nMap);

	char szMsg[128];
	sprintf(szMsg,"[%s]的BOSS[%s]刷新啦，大家快去挑战吧！",pMap->m_szName,pBossInfo->pName);
	ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);

	std::map<WORD,bool>::iterator	itor	= mapBossActive.find(wBoss);
	if(itor	!= mapBossActive.end())
			itor->second = true;
	else
		mapBossActive[wBoss] = true;

}

void BossState::BossDead( WORD wBoss, int nMap )
{
	//BossInfo*	pBInfo	= GetBInfo(wBoss);

	//for (int i = 0; i < pBInfo->pListMap->Count; i++)
	//{
	//	if((int)pBInfo->pListMap->Items[i] == nMap)
	//	{
	//		pBInfo->pListMap->Delete(i);
	//		break;
	//	}
	//}

	std::map<WORD,bool>::iterator	itor	= mapBossActive.find(wBoss);
	if(itor	!= mapBossActive.end())
		itor->second = false;
}

//BossInfo* BossState::GetBInfo( WORD wBossID )
//{
//	std::map<WORD,BossInfo*>::iterator	itor	= mapBossInfo.find(wBossID);
//	BossInfo*	pBInfo	= NULL;
//	if(itor	== mapBossInfo.end())
//	{
//		pBInfo	= new BossInfo();
//		mapBossInfo[wBossID]	= pBInfo;
//	}
//	else
//		pBInfo	= itor->second;
//
//	return pBInfo;
//}
bool BossState::CheckBossActive(WORD wBossID)
{
	std::map<WORD,bool>::iterator	itor	= mapBossActive.find(wBossID);
	if(itor	!= mapBossActive.end())
		return itor->second;
	return false;
}
