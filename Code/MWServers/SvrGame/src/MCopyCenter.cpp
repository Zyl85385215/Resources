#include "stdafx.h"
#include "MCopyCenter.h"
#include "ZNetGamesvr.h"
#include "ServerCfg.h"
#include "ios_mapobj_packet.h"
#include "FsGameCenter.h"
#include "MapObjCenter.h"
#include "CallMapEnemy.h"

MapCopyObj::MapCopyObj(DWORD	dwMindex,DWORD dwUin,MCopyBase*	pMC,int nLimit)
{
	m_dwMapIndex	= dwMindex;
	m_dwUin			= dwUin;
	m_pMCBase		= pMC;
	m_nPartLimit	= nLimit;
	m_DeleteLock	= false;
	m_pNetSrvLink	= MapSelCfg::GetInstance().GetByMapIndex(m_pMCBase->wBaseMap);
	memset(m_szdwPart,0,16);
	m_pPartList		= xnList::Create();
	m_pListHelper	= xnList::Create();
	dwDelTick		= 0;
	if(m_pNetSrvLink)
	{
		BuildPacketEx(pPacket, MAPOBJ,GS_MAPCOPY,buf,1024);
		SET_DATA(pSend, MAPOBJ,GS_MAPCOPY, pPacket);
		pSend->bType		= 0;
		pSend->dwUin		= dwUin;
		pSend->wRealMIndex	= m_pMCBase->wBaseMap;
		pSend->dwMapIndex	= dwMindex;
		pSend->wExPow		= m_pMCBase->nExPow;
		pSend->byLv			= m_pMCBase->byChgLv;
		g_pNetCenter->SendMSvrPacket(pPacket,dwMindex,m_pNetSrvLink->pMapSvr);
	}
}

MapCopyObj::~MapCopyObj()
{
	if(m_pPartList)
	{
		while(m_pPartList->Count)
			delete m_pPartList->Delete(0);
		m_pPartList->Free();
	}
	while(m_pListHelper->Count)
	{
		delete (MapHelpObj*)m_pListHelper->Delete(0);
	}
	m_pListHelper->Free();

	if(m_pNetSrvLink)
	{
		BuildPacketEx(pPacket, MAPOBJ,GS_DELMAPCOPY,buf,1024);
		SET_DATA(pSend, MAPOBJ,GS_DELMAPCOPY, pPacket);
		pSend->dwMIndex	= m_dwMapIndex;
		g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
	}
	g_pGameCenter->m_pMapObjCenter->RemoveMapObj(m_dwMapIndex);
}

bool MapCopyObj::CheckDelete()
{
	if(m_dwUin != 0)
		return false;
// 	for (int i = 0; i < 4; i++)
// 	{
// 		if(m_szdwPart[i] != 0)
// 			return false;
// 	}

	if(m_pPartList->Count > 0)
		return false;

	if(m_DeleteLock)
		return false;

	if(dwDelTick == 0 || GetTickCount() - dwDelTick<15000)
		return false;

	return true;
}

bool MapCopyObj::CheckMember(DWORD dwUin)
{
	if(m_dwUin == dwUin)
		return true;
	for(int i = 0; i < m_pPartList->Count; i ++)
	{
		int nID = (int)m_pPartList->Items[i];
		if(nID == dwUin)
			return true;
	}
	return false;
}

MapCopyCenter::MapCopyCenter()
{
	m_dwMaxIndex	= 1000;
	m_pListMCopyCache	= xnList::Create();
}

MapCopyCenter::~MapCopyCenter()
{
	std::map<DWORD,MapCopyObj*>::iterator	itor	= mapMCopy.begin();
	while(itor != mapMCopy.end())
	{
		delete itor->second;
		itor++;
	}
	while(m_pListMCopyCache->Count)
		delete (MapCopyObj*)m_pListMCopyCache->Delete(0);
	m_pListMCopyCache->Free();
}

MapCopyObj* MapCopyCenter::RequestEnterMCopy( DWORD dwMIndex,DWORD dwUin)
{
	//判断自己是否有副本
	MapCopyObj*	pMCopy	= GetMCopyEx(dwUin);
	if(pMCopy)
		RequestLeaveMCopy(dwUin,pMCopy->m_dwMapIndex);
	pMCopy	= GetMCopyByMIndex(dwMIndex);
	if(pMCopy)
	{
		if (!CheckCanCreatCopy(pMCopy->m_pMCBase->wID,dwUin))
			return NULL;

		if(pMCopy->m_dwUin == 0 && !pMCopy->m_DeleteLock)
			return NULL;
		JoinMCopy(dwUin,pMCopy);
	}
	return pMCopy;
}

MapCopyObj* MapCopyCenter::RequestEnterMCopy( DWORD dwLeadUin,DWORD dwUin,MCopyBase*	pMC)
{
	//判断自己是否有副本
	MapCopyObj*	pMCopy	= GetMCopyEx(dwUin);
	if(pMCopy)
		RequestLeaveMCopy(dwUin,pMCopy->m_dwMapIndex);

	pMCopy	= GetMCopy(dwLeadUin);
// 	if(pMCopy && pMCopy->m_dwUin == 0 && !pMCopy->m_DeleteLock)
// 	{//已过期的副本,异常
// 		std::map<DWORD,MapCopyObj*>::iterator	itor	= mapMCopy.find(dwLeadUin);
// 		if(itor != mapMCopy.end())
// 			mapMCopy.erase(itor);
// 
// 		pMCopy->dwDelTick	= GetTickCount();
// 		m_pListMCopyCache->Add(pMCopy);
// 		pMCopy	= NULL;
// 	}
	if(!pMCopy)
	{//创建副本
		pMCopy	= new MapCopyObj(m_dwMaxIndex,dwLeadUin,pMC);
		mapMCopy[m_dwMaxIndex]	= pMCopy;
		m_dwMaxIndex ++;
	}

	if (!CheckCanCreatCopy(pMCopy->m_pMCBase->wID,dwUin))
		return NULL;
	
	JoinMCopy(dwUin,pMCopy);

	return pMCopy;
}

bool MapCopyCenter::RequestLeaveMCopy( DWORD dwUin,DWORD	dwMIndex )
{
	MapCopyObj*	pMCopy	= GetMCopyByMIndex(dwMIndex);
	if(pMCopy == NULL)
		return false;

	bool	bFind	= false;
	if(pMCopy->m_dwUin == dwUin)
	{
		pMCopy->m_dwUin	= 0;
		bFind	= true;
	}
// 	for (int i = 0; i < 4; i++)
// 	{
// 		if(pMCopy->m_szdwPart[i] == dwUin)
// 		{
// 			pMCopy->m_szdwPart[i]	= 0;
// 			bFind	= true;
// 		}
// 	}
	for(int i = pMCopy->m_pPartList->Count-1; i >= 0; i --)
	{
		int nID = (int)pMCopy->m_pPartList->Items[i];
		if(nID == dwUin)
		{
			pMCopy->m_pPartList->Delete(i);
			bFind = true;
			break;
		}
	}
	if(bFind)
	{//UpdateSec里判断没人并15秒删除
		if(pMCopy->m_dwUin == 0 && pMCopy->m_pPartList->Count==0)
			pMCopy->dwDelTick	= GetTickCount();
	}

	return bFind;
}

MapCopyObj* MapCopyCenter::GetMCopy( DWORD dwUin )
{
	std::map<DWORD,MapCopyObj*>::iterator	itor	= mapMCopy.begin();
	while(itor != mapMCopy.end())
	{
		if(itor->second->m_dwUin == dwUin)
		{
			return itor->second;
		}
		itor++;
	}
	return NULL;
}

MapCopyObj* MapCopyCenter::GetMCopyEx( DWORD dwUin )
{
	std::map<DWORD,MapCopyObj*>::iterator	itor	= mapMCopy.begin();
	while(itor != mapMCopy.end())
	{
		if(itor->second->CheckMember(dwUin))
		{
			return itor->second;
		}
		itor++;
	}
	return NULL;
}

bool MapCopyCenter::JoinMCopy( DWORD dwUin,MapCopyObj* pMCopy )
{
	bool bFind	= false;
	if(pMCopy->m_dwUin == dwUin)
	{
		bFind	= true;
	}
	
// 	for (int i = 0; i < 4; i++)
// 	{
// 		if(pMCopy->m_szdwPart[i] == dwUin)
// 		{
// 			bFind	= true;
// 			break;
// 		}
// 	}
	if(pMCopy->m_pPartList->Count >= pMCopy->m_nPartLimit)
	{
		bFind = true;
	}
	if(!bFind)
	{
		for(int i = 0; i < pMCopy->m_pPartList->Count; i ++)
		{
			int nID = (int)pMCopy->m_pPartList->Items[i];
			if(nID == dwUin)
			{
				bFind = true;
				break;
			}
		}
	}
	
	if(!bFind)
	{
// 		for (int i = 0; i < 4; i++)
// 		{
// 			if(pMCopy->m_szdwPart[i] == 0)
// 			{
// 				pMCopy->m_szdwPart[i]	= dwUin;
// 				bFind	= true;
// 				break;
// 			}
// 		}
		pMCopy->m_pPartList->Add((void*)dwUin);
	}

	return bFind;
}

void MapCopyCenter::UpdateSec()
{
	std::map<DWORD,MapCopyObj*>::iterator	itor	= mapMCopy.begin();
	while(itor != mapMCopy.end())
	{
		if(itor->second->CheckDelete())
		{
			g_pGameCenter->m_pCallEnemyCenter->RefreshmapCopyEnemyID(itor->first);

			delete itor->second;
			mapMCopy.erase(itor++);
		}
		else
			itor++;
	}
	for (int i = m_pListMCopyCache->Count-1; i >= 0; i--)
	{
		MapCopyObj* pMCopy	= (MapCopyObj*)m_pListMCopyCache->Items[i];

		if(pMCopy->CheckDelete())
		{
			delete pMCopy;
			m_pListMCopyCache->Delete(i);
		}
	}
}

void MapCopyCenter::KickOutMember(DWORD dwMIndex)
{
	MapCopyObj* pMCopy = GetMCopyByMIndex(dwMIndex);
	if(pMCopy)
	{
		if(pMCopy->m_dwUin > 0)
		{
			UserData* pUser = g_pGameCenter->GetPlayer(pMCopy->m_dwUin);
			if(pUser && pUser->m_nMIndex >= 1000)
				pUser->JumpTo(pUser->m_wOldMIndex,pUser->m_wOldPosX,pUser->m_wOldPosY);
		}
		for(int i = 0; i < pMCopy->m_pPartList->Count; i ++)
		{
			int nID = (int)pMCopy->m_pPartList->Items[i];
			UserData* pUser = g_pGameCenter->GetPlayer(nID);
			if(pUser && pUser->m_nMIndex >= 1000)
				pUser->JumpTo(pUser->m_wOldMIndex,pUser->m_wOldPosX,pUser->m_wOldPosY);
		}
		pMCopy->m_DeleteLock = false;
		pMCopy->dwDelTick	= GetTickCount();
	}
}

MapCopyObj* MapCopyCenter::GetMCopyByMIndex( DWORD dwMIndex )
{
	std::map<DWORD,MapCopyObj*>::iterator	itor	= mapMCopy.find(dwMIndex);
	if(itor != mapMCopy.end())
		return itor->second;
	return NULL;
}

void MapCopyCenter::SetCopyDailyLmt(int nMCID)
{
	listActMapLmt.push_back(nMCID);
}

bool MapCopyCenter::CheckCanCreatCopy(int nMCID,DWORD dwUin)
{
	if (nMCID!=407&&nMCID!=408)
		return true;
	bool bRlt = true;
	for (list<int>::iterator itor = listActMapLmt.begin();itor!=listActMapLmt.end();itor++)
	{
		if (nMCID == (*itor))
			bRlt = false;
	}

	if (bRlt == false)
	{
		UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
		if (pUser)
		{
			pUser->SendWarnMsg("该活动已结束，请明天再来挑战");
		}
	}

	return bRlt;
}

void MapCopyCenter::ReFreshDailyLmt()
{
	listActMapLmt.clear();
}
