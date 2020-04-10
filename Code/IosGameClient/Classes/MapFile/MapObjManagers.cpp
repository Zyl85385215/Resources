#include "MapObjManagers.h"
#include "MapControl.h"

MapObjManager::MapObjManager()
{
	m_pList		= xnList::Create();
}

MapObjManager::~MapObjManager()
{
	while(m_pList->Count)
		delete	(MapObj*)m_pList->Delete(0);

	m_pList->Free();
}

MapObj* MapObjManager::AddMapObj( int nID,BYTE bType )
{
	MapObj*	pObj	= GetMapObj(nID);
	if(pObj)
	{
		pObj->m_nMIndex	= g_pMapCenter->m_nMapIndex;
		return pObj;
	}
	pObj			= new MapObj();
	pObj->m_nID		= nID;
	pObj->m_nMIndex	= g_pMapCenter->m_nMapIndex;
	pObj->m_byType	= bType;
	m_pList->Add(pObj);
	return pObj;
}

MapObj* MapObjManager::GetMapObj( int nID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		MapObj* pObj	= (MapObj*)m_pList->Items[i];
		if(nID	== pObj->m_nID)
			return pObj;
	}
	return NULL;
}

bool MapObjManager::DelMapObj( int nID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		MapObj* pObj	= (MapObj*)m_pList->Items[i];
		if(nID	== pObj->m_nID)
		{
			if(pObj->m_byType	!= MAPOBJ_NULL)
				g_pMapCenter->DeLinkMapObj(pObj);
			delete pObj;
			m_pList->Delete(i);
			return true;
		}
	}
	return false;
}

void MapObjManager::ClearMapObj()
{
	for (int i = m_pList->Count-1; i >= 0; i--)
	{
		MapObj* pObj	= (MapObj*)m_pList->Items[i];
		//if(pObj->m_nMIndex != g_pMapCenter->m_nMapIndex)
		{
			g_pMapCenter->DeLinkMapObj(pObj);
			delete pObj;
			m_pList->Delete(i);
		}
	}
}


//------------------------------------------------------------
MapAIObjManager::MapAIObjManager()
{

}

MapAIObjManager::~MapAIObjManager()
{

}

MapObj* MapAIObjManager::AddMapObj( int nID,BYTE bType )
{
	MapAIObj*	pObj	= (MapAIObj*)GetMapObj(nID);
	if(pObj)
	{
		pObj->m_nMIndex	= g_pMapCenter->m_nMapIndex;
		return pObj;
	}
	pObj			= new MapAIObj();
	pObj->m_nID		= nID;
	pObj->m_nMIndex	= g_pMapCenter->m_nMapIndex;
	pObj->m_byType	= bType;
	m_pList->Add(pObj);

	g_pMapCenter->LinkMapObj(pObj);
	return pObj;
}

void MapAIObjManager::Update()
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		MapAIObj* pAI	= (MapAIObj*)m_pList->Items[i];
		if(pAI->m_nMIndex && pAI->m_nMIndex != g_pMapCenter->m_nMapIndex)
		{
			g_pMapCenter->DeLinkMapObj(pAI);
			delete pAI;
			m_pList->Delete(i);
			i--;
		}
		else
			pAI->Update();
	}
}

//------------------------------------------------------------
MapPlayerManager::MapPlayerManager()
{

}

MapPlayerManager::~MapPlayerManager()
{

}

MapObj* MapPlayerManager::AddMapObj( int nID,BYTE bType )
{
	MapPlayer*	pObj	= (MapPlayer*)GetMapObj(nID);
	if(pObj)
	{
		pObj->m_nMIndex	= g_pMapCenter->m_nMapIndex;
		return pObj;
	}
	pObj			= new MapPlayer();
	pObj->m_nID		= nID;
	pObj->m_nMIndex	= g_pMapCenter->m_nMapIndex;
	pObj->m_byType	= bType;
	m_pList->Add(pObj);

	g_pMapCenter->LinkMapObj(pObj);
	return pObj;
}
