#include "stdafx.h"
#include "MapObjCenter.h"
#include "MapStaticCfg.h"
#include "MapConfig.h"
#include "ZLogFile.h"
#include "WarRank.h"
#include "FsGameCenter.h"
#include "ios_mapobj_packet.h"
#include "ZNetGamesvr.h"
#include "ServerCfg.h"
#include "LuaStateManagerPlus.h"

extern	FsGameCenter* g_pGameCenter;
extern	ZLogFile* g_pLogFile;
extern CLuaStateManager * pLuaStateManager;

UserData* MapServerObj::GetUserByPos( int nPos )
{
	if (nPos>=m_pList->Count)
	{
		return NULL;
	}
	return (UserData*)m_pList->Items[nPos];
}

MapServerObj::MapServerObj( int nIndex,int nBase ):m_nBaseID(nBase)
{
	m_pList = xnList::Create();
	m_nIndex = nIndex;

	CLuaClassObject<MapServerObj>::Init(this);
}

void MapServerObj::Update()
{

}

MapServerObj::~MapServerObj()
{
	m_pList->Free();
}

void MapServerObj::CreateScriptClass( CLuaState * pLuaState )
{
	CLuaClass<MapServerObj> * pLuaUser = pLuaState->CreateScriptClass((MapServerObj*)0);

	pLuaUser->DefObjectFunction("GetBaseID", &MapServerObj::GetBaseID)
		.DefObjectFunction("GetIndex", &MapServerObj::GetIndex)
		.DefObjectFunction("GetUserByPos", &MapServerObj::GetUserByPos)
		.DefObjectFunction("GetUserCount", &MapServerObj::GetUserCount)
		.DefObjectFunction("GetAllUser", &MapServerObj::GetAllUser)
		.DefObjectFunction("ChgMapForce", &MapServerObj::ChgMapForce);
}

int MapServerObj::GetIndex()
{
	return m_nIndex;
}

void MapServerObj::ReleaseScriptClass( CLuaState * pLuaState )
{
	pLuaState->ReleaseScriptClass((MapServerObj*)0);
}

int MapServerObj::GetBaseID()
{
	return m_nBaseID;
}

int MapServerObj::GetUserCount()
{
	return m_pList->Count;
}

void MapServerObj::ChgMapForce( int nFType )
{
	BuildPacketEx(pPacket, MAPOBJ,GS_CHGMFORCE,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_CHGMFORCE, pPacket);
	pSend->nFType		= nFType;

	MapSrvObj* pSrv	= NULL;
	if(m_nIndex < 1000)
		pSrv	= MapSelCfg::GetInstance().GetByMapIndex(m_nIndex);
	else
	{
		MapCopyObj* pMCopy	= g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(m_nIndex);
		if(pMCopy)
			pSrv	= pMCopy->m_pNetSrvLink;
	}

	if(pSrv)
		g_pNetCenter->SendMSvrPacket(pPacket,m_nIndex,pSrv->pMapSvr);

}

void MapServerObj::ChgMapEnemyPow( int nExPow, int nLv /*= 0*/ )
{
	BuildPacketEx(pPacket, MAPOBJ,GS_CHGMPOW,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_CHGMPOW, pPacket);
	pSend->nPow		= nExPow;
	pSend->nLv		= nLv;

	MapSrvObj* pSrv	= NULL;
	if(m_nIndex < 1000)
		pSrv	= MapSelCfg::GetInstance().GetByMapIndex(m_nIndex);
	else
	{
		MapCopyObj* pMCopy	= g_pGameCenter->m_pMapCopyCenter->GetMCopyByMIndex(m_nIndex);
		if(pMCopy)
			pSrv	= pMCopy->m_pNetSrvLink;
	}

	if(pSrv)
		g_pNetCenter->SendMSvrPacket(pPacket,m_nIndex,pSrv->pMapSvr);
}

void MapServerObj::GetAllUser(LuaStackObject luaTable)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	bool bTable = luaTable.IsTable();
	//stackDump(pLuaState->GetCState());
 	for (int i=0;i<m_pList->Count;i++)
 	{
		UserData* pUser = (UserData*)m_pList->Items[i];
		LPCD::Push(pLuaState->GetCState(),pUser->m_dwUin);
		LPCD::Push(pLuaState->GetCState(),pUser);
		luaTable.SetTable();
 	}
	//stackDump(pLuaState->GetCState());
}

MapServerObj* MapObjCenter::GetMapObjByIndex( int nIndex )
{
	map<int,MapServerObj*>::iterator itor = m_MapObj.find(nIndex);
	if (itor != m_MapObj.end())
	{
		return itor->second;
	}
	return NULL;
}

MapServerObj* MapObjCenter::CreateMapObj( int nIndex,int nBase )
{
	map<int,MapServerObj*>::iterator itor = m_MapObj.find(nIndex);
	if (itor != m_MapObj.end())
	{
		return itor->second;
	}
	MapServerObj* pObj = new MapServerObj(nIndex,nBase);
	m_MapObj[nIndex] = pObj;
	_runLuaFunctionForMap(pObj,NULL,szLuaFunctionInitMap);
	return pObj;
}

void MapObjCenter::Join( UserData* pUser,int nIndex ,int nBase)
{
	MapServerObj* pObj = GetMapObjByIndex(nIndex);
	if (!pObj)
	{
		pObj = CreateMapObj(nIndex,nBase);
	}
	if (pObj)
	{
		for (int i=0;i<pObj->m_pList->Count;i++)
		{
			UserData* pItem = (UserData*)pObj->m_pList->Items[i];
			if (pUser == pItem)
			{
				return;
			}
		}
		pObj->m_pList->Add(pUser);

		if (!pUser->m_bChangeCountry)
			_runLuaFunctionForMap(pObj,pUser,szLuaFunctionOnJumpMap);
		else
			pUser->m_bChangeCountry = false;
	}
	return;
}

void MapObjCenter::Leave( UserData* pUser,int nIndex )
{
	MapServerObj* pObj = GetMapObjByIndex(nIndex);
	if (pObj)
	{
		for (int i=0;i<pObj->m_pList->Count;i++)
		{
			UserData* pItem = (UserData*)pObj->m_pList->Items[i];
			if (pUser == pItem)
			{
				pObj->m_pList->Delete(i);
				_runLuaFunctionForMap(pObj,pUser,szLuaFunctionOnLeaveMap);
				if (pObj->m_nBaseID == 27)
				{
					g_pGameCenter->m_pWarRankCenter->UnlockFight(pUser->m_pWarRankFight);
				}
				return;
			}
		}
	}
}

void MapObjCenter::RemoveMapObj( int nIndex )
{
	m_MapObj.erase(nIndex);
}

void MapObjCenter::UpdateSec()
{
	for (map<int,MapServerObj*>::iterator itor = m_MapObj.begin();itor != m_MapObj.end();itor++)
	{
		itor->second->Update();
		_runLuaFunctionForMap(itor->second,NULL,szLuaFunctionUpdateMap);
	}
}

bool MapObjCenter::_runLuaFunctionForMap( MapServerObj* pObj,UserData* pUser,const char* const szFunctionName )
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	sprintf(sztxt,"%s%d",szFunctionName,pObj->GetBaseID());
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);

	CLuaPlusSafeFunction_3<bool,MapServerObj*, UserData*> func;
	if(!func(pLuaState, lf, &bRet, pObj,pUser))
	{
		g_pLogFile->OutPut("½Å±¾º¯Êý%sÖ´ÐÐ´íÎó",sztxt);
		return false;
	}
	return bRet;
}

bool MapObjCenter::_runLuaFunctionForMap( int nBaseID,UserData* pUser,const char* const szFunctionName )
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	sprintf(sztxt,"%s%d",szFunctionName,nBaseID);
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);

	CLuaPlusSafeFunction_3<bool,MapServerObj*, UserData*> func;
	if(!func(pLuaState, lf, &bRet, NULL,pUser))
	{
		g_pLogFile->OutPut("½Å±¾º¯Êý%sÖ´ÐÐ´íÎó",sztxt);
		return false;
	} 
	return bRet;
}

bool MapObjCenter::CheckJoin( UserData* pUser,int nIndex ,int nBaseID)
{
	MapServerObj* pObj = GetMapObjByIndex(nIndex);
	if (pObj)
	{
		for (int i=0;i<pObj->m_pList->Count;i++)
		{
			UserData* pItem = (UserData*)pObj->m_pList->Items[i];
			if (pUser == pItem)
			{
				return false;
			}
		}
		if (!_runLuaFunctionForMap(pObj,pUser,szLuaFunctionBeforeJumpMap))
		{
			return false;
		}
		return true;
	}else{
		if (!_runLuaFunctionForMap(nBaseID,pUser,szLuaFunctionBeforeJumpMap))
		{
			return false;
		}
		return true;
	}
	return false;
}

bool MapObjCenter::CheckLeave( UserData* pUser,int nIndex )
{
	MapServerObj* pObj = GetMapObjByIndex(nIndex);
	if (pObj)
	{
		for (int i=0;i<pObj->m_pList->Count;i++)
		{
			UserData* pItem = (UserData*)pObj->m_pList->Items[i];
			if (pUser == pItem)
			{
				if (!_runLuaFunctionForMap(pObj,pUser,szLuaFunctionBeforeLeaveMap))
				{
					return false;
				}
				return true;
			}
		}
	}
	return false;
}

void MapObjCenter::CreateScriptClass( CLuaState * pLuaState )
{
	CLuaClass<MapObjCenter> * pLuaUser = pLuaState->CreateScriptClass((MapObjCenter*)0);

	pLuaUser->DefObjectFunction("GetMapObjByIndex", &MapObjCenter::GetMapObjByIndex);

}

void MapObjCenter::ReleaseScriptClass( CLuaState * pLuaState )
{
	pLuaState->ReleaseScriptClass((MapObjCenter*)0);
}

MapObjCenter::MapObjCenter()
{
	CLuaClassObject<MapObjCenter>::Init(this);

}

void MapObjCenter::Init()
{
	for (int i=0;i<MapViewConfig::GetInstance().m_pList->Count;i++)
	{
		MapTileData* pObj = (MapTileData*)MapViewConfig::GetInstance().m_pList->Items[i];
		CreateMapObj(pObj->wIndex,pObj->wIndex);
	}
}
