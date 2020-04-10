#include "MapLimitConfig.h"
#include "xnExtstr.h"
#include "stdio.h"

CreateSingleton(MapLimitConfig);

MapLimitConfig::MapLimitConfig( )
{
	m_pListMapLimit		= xnList::Create();
}

MapLimitConfig::~MapLimitConfig()
{
	while(m_pListMapLimit->Count)
		delete (MapLimitObj*)m_pListMapLimit->Delete(0);

	m_pListMapLimit->Free();
}

void MapLimitConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int iIndex = 0;
	char szTmp[32];

	while(pStr->SetSection(iIndex++))
	{
		MapLimitObj*	pInfo	= new MapLimitObj();
		m_pListMapLimit->Add(pInfo);
		pInfo->nMapIndex	= atoi(pStr->sectionName);
		pInfo->bLimitSlave	= atoi(pStr->ReadString("slavelimit","0"));
		pInfo->bLimitGroup	= atoi(pStr->ReadString("grouplimit","0"));
		pInfo->bLimitItem	= atoi(pStr->ReadString("itemlimit","0"));
		pInfo->bLimitDeath	= atoi(pStr->ReadString("deathlimit","0"));
		pInfo->bLimitHeibaiGroup = atoi(pStr->ReadString("heibailimit","0"));
		int nCnt	= atoi(pStr->ReadString("cnt","0"));

		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"itemid%d",i+1);
			pInfo->pListItem->Add((void*)atoi(pStr->ReadString(szTmp, "0")));
		}
	}

	pStr->Free();
}

MapLimitObj* MapLimitConfig::GetMapLimit( int nMapIndex )
{
	for (int i = 0; i < m_pListMapLimit->Count; i++)
	{
		MapLimitObj* pFInfo	= (MapLimitObj*)m_pListMapLimit->Items[i];
		if(pFInfo->nMapIndex == nMapIndex)
			return pFInfo;
	}
	return NULL;
}
bool MapLimitObj::CheckItemCanUse( int nItemID )
{
	for (int i = 0; i < pListItem->Count; i++)
	{
		if((int)(pListItem->Items[i]) == nItemID)
			return bLimitItem;
	}
	return !bLimitItem;
}