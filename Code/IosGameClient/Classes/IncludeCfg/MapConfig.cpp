
#include "stdafx.h"
#include "MapConfig.h"
#include "xnExtstr.h"
#ifdef _ISSERVER
#include "stdio.h"
#define SAFE_FREE(p) { if(p) { free(p); p=NULL; } }
#endif
CreateSingleton(MapViewConfig);
CreateSingleton(MapLimitConfig);

MapTileData::MapTileData()
{
	m_ppTiles		= NULL;
	m_szBackMp3		= NULL;
}

MapTileData::~MapTileData()
{
	if(m_ppTiles)
	{
		for (int i = 0; i < m_nXCnt*m_nYCnt; i++)
		{
			delete		m_ppTiles[i];
		}

		delete[]	m_ppTiles;
	}

	SAFE_FREE(m_szBackMp3);
}

void MapTileData::InitTile()
{
	m_ppTiles		= new _mapTile*[m_nXCnt*m_nYCnt];

	for (int i = 0; i < m_nYCnt; i++)
	{
		for (int j = 0; j < m_nXCnt; j++)
		{
			_mapTile* pTile	= new _mapTile();
			pTile->nIndex	= wRealIndex*1000+i*m_nXCnt + j;
			pTile->nPosX		= j*m_nSWidth;
			pTile->nPosY		= i*m_nSHeight;
			char	szTmp[256];
			sprintf(szTmp,"Map/%d/tile_%02d.jpg",wRealIndex,i*m_nXCnt+j+1);
			pTile->szSpr		= strdup(szTmp);
			m_ppTiles[i*m_nXCnt + j]	= pTile;
		}
	}
}

int MapTileData::GetInViewTile( int nPosX, int nPosY,int nEX,int nEY,xnList* pList )
{
	pList->Clear();
	int nPLeft		= nPosX/m_nSWidth;
	int	nPRight		= nEX/m_nSWidth;
	int nPTop		= nPosY/m_nSHeight;
	int	nPBottom	= nEY/m_nSHeight;

	if(nPLeft < 0) nPLeft = 0;
	if(nPTop < 0) nPTop	= 0;
	if(nPRight >= m_nXCnt)	nPRight	= m_nXCnt-1;
	if(nPBottom >= m_nYCnt)	nPBottom = m_nYCnt-1;

	for (int i = nPTop; i <= nPBottom; i++)
	{
		for(int j = nPLeft; j <= nPRight; j++)
		{
			pList->Add(m_ppTiles[i*m_nXCnt+j]);
		}
	}
	return pList->Count;
}

MapViewConfig::MapViewConfig()
{
	m_pList		= xnList::Create();
}

MapViewConfig::~MapViewConfig()
{
	while(m_pList->Count)
		delete (MapTileData*)m_pList->Delete(0);
	m_pList->Free();
}

void MapViewConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);

	int nIndex=0;
	while(pStr->SetSection (nIndex++))
	{
		MapTileData*	pMTile	= new MapTileData();
		pMTile->wIndex		= atoi (pStr->sectionName);
		pMTile->wRealIndex	= atoi(pStr->ReadString("realmap", "0"));
		if(pMTile->wRealIndex	== 0)
			pMTile->wRealIndex	= pMTile->wIndex;

		pMTile->byRealMiniMap	= atoi(pStr->ReadString("isminimap", "1"));
		pMTile->m_nXCnt	= atoi(pStr->ReadString("tilecntw", "1"));
		pMTile->m_nYCnt	= atoi(pStr->ReadString("tilecnth", "1"));
		pMTile->m_nSWidth	= atoi(pStr->ReadString("tilew", "512"));
		pMTile->m_nSHeight	= atoi(pStr->ReadString("tileh", "512"));
		pMTile->m_nTWidth	= atoi(pStr->ReadString("mapsizew", "512"));
		pMTile->m_nTHeight	= atoi(pStr->ReadString("mapsizeh", "512"));
		sprintf(pMTile->m_szName,pStr->ReadString ("name","unkown map"));
		pMTile->m_szBackMp3	= strdup(pStr->ReadString ("music",""));
		pMTile->InitTile();

		m_pList->Add(pMTile);
	}
	pStr->Free();
}

MapTileData* MapViewConfig::GetMapTileData( int nIndex )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		MapTileData* pMap	= (MapTileData*)m_pList->Items[i];
		if(pMap->wIndex		== nIndex)
			return pMap;
	}
	return NULL;
}

MapLimitData::MapLimitData()
{
	wMapBaseID = 0;
	bHorseLimit = false;
	bSlaveLimit = false;

	pItemLimitList = xnList::Create();
}

MapLimitData::~MapLimitData()
{
	while(pItemLimitList->Count)
		pItemLimitList->Delete(0);
	pItemLimitList->Free();
}

MapLimitConfig::MapLimitConfig()
{
	m_pList = xnList::Create();
}

MapLimitConfig::~MapLimitConfig()
{
	while(m_pList->Count)
		delete (MapLimitData*)m_pList->Delete(0);
	m_pList->Free();
}

void MapLimitConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);

	int nIndex=0;
	while(pStr->SetSection (nIndex++))
	{
		MapLimitData*	pMLimit	= new MapLimitData();
		pMLimit->wMapBaseID	    = atoi(pStr->ReadString("mapbase", "0"));
		pMLimit->bHorseLimit	= atoi(pStr->ReadString("horselimit", "0"));
		pMLimit->bSlaveLimit	= atoi(pStr->ReadString("slavelimit", "0"));

		int nItemLCnt = atoi(pStr->ReadString("itemcnt", "0"));
		char szTmp[32];
		for (int i = 0; i < nItemLCnt; i++)
		{
			sprintf(szTmp,"item%d",i+1);
			int nItemID = atoi(pStr->ReadString(szTmp, "0"));
			if (nItemID)
				pMLimit->pItemLimitList->Add((void*)nItemID);
		}
		m_pList->Add(pMLimit);
	}
	pStr->Free();
}

bool MapLimitConfig::CheckMapHorseLimit(WORD wMapRIndex)
{
	if (m_pList->Count)
	{
		for (int i = 0; i < m_pList->Count; i++)
		{
			MapLimitData* pMLimit = (MapLimitData*)m_pList->Items[i];
			if (pMLimit && pMLimit->wMapBaseID == wMapRIndex)
				return pMLimit->bHorseLimit;
		}
	}
	return false;
}

bool MapLimitConfig::CheckMapSlaveLimit(WORD wMapRIndex)
{
	if (m_pList->Count)
	{
		for (int i = 0; i < m_pList->Count; i++)
		{
			MapLimitData* pMLimit = (MapLimitData*)m_pList->Items[i];
			if (pMLimit && pMLimit->wMapBaseID == wMapRIndex)
				return pMLimit->bSlaveLimit;
		}
	}
	return false;
}

bool MapLimitConfig::CheckMapItemULimit(WORD wMapRIndex,WORD wItemID)
{
	if (m_pList->Count)
	{
		for (int i = 0; i < m_pList->Count; i++)
		{
			MapLimitData* pMLimit = (MapLimitData*)m_pList->Items[i];
			if (pMLimit && pMLimit->wMapBaseID == wMapRIndex)
			{
				for (int j = 0; j < pMLimit->pItemLimitList->Count; j++)
				{
					int nItemID = (int)pMLimit->pItemLimitList->Items[j];
					if (nItemID == wItemID)
						return true;
				}
			}
		}
	}
	return false;
}