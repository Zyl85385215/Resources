#include "stdafx.h"
#include "MapServerCfg.h"
#include "xnExtstr.h"

CreateSingleton(MapServerCfg);
CreateSingleton(MapSelCfg);

void MapServerCfg::LoadFile( char* szFile )
{
	xnExtstr* pStr	= xnExtstr::Import(szFile);
	if(pStr == NULL)
		return;

	if(pStr->SetSection(0))
	{
		strcpy(szIP,pStr->ReadString("ip","127.0.0.1"));
		nPort		= atoi(pStr->ReadString("port","9800"));
		strcpy(szGameIP,pStr->ReadString("gameip","127.0.0.1"));
		nGamePort	= atoi(pStr->ReadString("gameport","9805"));
	}
}

MapSelCfg::MapSelCfg()
{
	m_pList		= xnList::Create();
	m_pMapSrvDefaul	= NULL;
}

MapSelCfg::~MapSelCfg()
{
	if(m_pMapSrvDefaul)
		delete m_pMapSrvDefaul;
	while(m_pList->Count)
	{
		delete (MapSrvObj*)m_pList->Delete(0);
	}
	m_pList->Free();
}

void MapSelCfg::LoadFile( char* szFile )
{
	xnExtstr* pStr	= xnExtstr::Import(szFile);
	if(pStr == NULL)
		return;

	int nIndex=0;
	while(pStr->SetSection(nIndex++))
	{
		MapSrvObj*	pMSrv	= new MapSrvObj();
		strcpy(pMSrv->szIP,pStr->ReadString("ip","127.0.0.1"));
		pMSrv->nPort		= atoi(pStr->ReadString("port","9810"));
		int nCnt	= atoi(pStr->ReadString("mapcnt","0"));
		if(nCnt	== 0)
		{
			if(m_pMapSrvDefaul)
				delete m_pMapSrvDefaul;
			m_pMapSrvDefaul	= pMSrv;
		}
		else
		{
			char	szTmp[32];
			for (int i = 0; i < nCnt; i++)
			{
				sprintf(szTmp,"map%d",i+1);
				pMSrv->pList->Add((void*)atoi(pStr->ReadString(szTmp,"0")));
			}
			m_pList->Add(pMSrv);
		}

		if(strcmp(MapServerCfg::GetInstance().szIP,pMSrv->szIP)==0 && MapServerCfg::GetInstance().nPort == pMSrv->nPort)
			MapServerCfg::GetInstance().pMapSrv	= pMSrv;
	}
}

MapSrvObj* MapSelCfg::GetByMapIndex( int nMIndex )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		MapSrvObj* pObj	= (MapSrvObj*)m_pList->Items[i];
		for (int j = 0; j < pObj->pList->Count; j++)
		{
			if(nMIndex == (int)pObj->pList->Items[j])
			{
				return pObj;
			}
		}
	}
	return m_pMapSrvDefaul;
}
