#include "stdafx.h"

#include "MapCenter.h"
#include "ios_mapobj_packet.h"
#include "ZNetServer.h"
#include "ControlCenter.h"

MapList*	g_pMapList	= NULL;


MapList::MapList()
{
	_initMaps();
}

MapList::~MapList()
{

}

void MapList::RecvPacket( ZPacket* pPacket, int nMapIndex )
{
	if(pPacket->bCmdGroup != MAPOBJ)
		return;

	switch(pPacket->bCmd)
	{
	case GS_INITCTRL:
		{
			SET_DATA(pData, MAPOBJ, GS_INITCTRL,pPacket);
			ControlObj* pCtrl	= g_pCtrlList->GetCtrl(pData->dwKey,true);
			while(pCtrl->m_pListChild->Count)
			{
				MapAIObj* pObj	= (MapAIObj*)pCtrl->m_pListChild->Delete(0);
				pObj->pCtrl	= NULL;
				if(pCtrl->m_pRunMap)
					pCtrl->m_pRunMap->AIObjLeaveMap(pObj);
			}
		}
		break;
	case GS_MAPLOCKATK:
		{
			SET_DATA(pData, MAPOBJ, GS_MAPLOCKATK,pPacket);
			MapManager* pMap	= GetMap(pData->dwMapIndex);
			if(pMap)
			{
				pMap->m_bLockAtk	= pData->bLock;
			}
		}
		break;
	case GS_SETCTRL:
		{//修改控制器控制权限和目标
			
			SET_DATA(pData, MAPOBJ, GS_SETCTRL,pPacket);
			ControlObj* pCtrl	= g_pCtrlList->GetCtrl(pData->dwKey,true);
			//if(pCtrl == NULL)
			//	return;
			pCtrl->m_nSpVal	= pData->wFury;
			if(pCtrl->m_dwUin == 0 && pData->bControl == 10)	//第一个全权控制的为主ID
				pCtrl->m_dwUin	= pData->dwID;
			MapManager* pMap	= GetMap(pData->wMapIndex);
			if(pMap)
			{
				MapAIObj* pObj	= pMap->GetMapObj(pData->bType,pData->dwID);
				if(pObj)
				{
					pCtrl->m_pRunMap	= pMap;
					pCtrl->SetCtrl(pData->bControl,pObj);
				}
			}
		}
		break;
	case GS_CTRLOFF:
		{
			SET_DATA(pData, MAPOBJ, GS_CTRLOFF,pPacket);
			g_pCtrlList->RemoveCtrl(pData->dwKey);
		}
		break;
	case GS_MAPCOPY:
		{
			SET_DATA(pData, MAPOBJ, GS_MAPCOPY, pPacket);
			MapManager* pMapMng	= _createMapCopy(pData->dwMapIndex,pData->wRealMIndex);
			if(pMapMng)
			{
				pMapMng->m_wExPow	= pData->wExPow;
				pMapMng->m_byChgLv	= pData->byLv;
			//	BuildPacketEx(pSendPk, MAPOBJ, MS_MAPCOPYRTL, buf, 256);
			//	SET_DATA(pSend, MAPOBJ, MS_MAPCOPYRTL, pSendPk);
			//	pSend->dwUin	= pData->dwUin;
			//	pSend->dwMIndex	= pMapMng->m_dwMapIndex;
			//	g_pNetCenter->SendPacketToGameSrv(pSendPk,pData->dwUin);
			}
		}
		break;
	case GS_DELMAPCOPY:
		{
			SET_DATA(pData, MAPOBJ, GS_DELMAPCOPY, pPacket);
			if(pData->dwMIndex	< 1000)
				return;

			_releaseMap(pData->dwMIndex);
		}
		break;
	default:
		{
			std::map<DWORD, MapManager*>::iterator	itor	= m_mapMaps.find(nMapIndex);
			if(itor	!= m_mapMaps.end())
				itor->second->RecvPacket(pPacket);
			else
			{//error

			}
		}
		break;
	}
}

void MapList::Update()
{
	std::map<DWORD,MapManager*>::iterator itor	= m_mapMaps.begin();
	while(itor != m_mapMaps.end())
	{
		if(itor->second->Update() == false)
		{//副本地图过期删除
			m_mapMaps.erase(itor++);
		}
		else
			itor++;
	}
}

void MapList::UpdateSec()
{
	std::map<DWORD,MapManager*>::iterator itor	= m_mapMaps.begin();
	while(itor != m_mapMaps.end())
	{
		itor->second->UpdateSec();
		itor++;
	}
}

void MapList::_initMaps()
{//初始化配置文件里有的地图
	for(int i = 0; i < MapConfig::GetInstance().m_pList->Count; i++)
	{
		MapCfgObj*	pMObj	= (MapCfgObj*)MapConfig::GetInstance().m_pList->Items[i];

		MapManager* pMMng	= new MapManager(pMObj->m_wMapIndex,pMObj);
		m_mapMaps[pMObj->m_wMapIndex]	= pMMng;		
		pMMng->Init();
	}
}

MapManager* MapList::_createMapCopy( int nMapIndex,int nRealMIndex )
{
	std::map<DWORD,MapManager*>::iterator itor	= m_mapMaps.find(nMapIndex);
	while(itor	!= m_mapMaps.end())
	{
		return itor->second;
	}

	MapManager* pMMng	= new MapManager(nMapIndex,MapConfig::GetInstance().GetMapCfgObj(nRealMIndex));
	m_mapMaps[pMMng->m_dwMapIndex]	= pMMng;
	pMMng->Init();
	return pMMng;
}

void MapList::_releaseMap( int nMapIndex )
{
	std::map<DWORD,MapManager*>::iterator itor	= m_mapMaps.find(nMapIndex);
	if(itor != m_mapMaps.end())
	{
		delete itor->second;
		m_mapMaps.erase(itor);
	}
}

MapManager* MapList::GetMap( WORD wMIndex )
{
	std::map<DWORD,MapManager*>::iterator itor	= m_mapMaps.find(wMIndex);
	while(itor	!= m_mapMaps.end())
	{
		return itor->second;
	}
	return NULL;
}

