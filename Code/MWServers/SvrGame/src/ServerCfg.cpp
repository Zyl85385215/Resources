#include "stdafx.h"
#include "ServerCfg.h"
#include "xnExtstr.h"
#include "md5Capi.h"

CreateSingleton(ServerCfg);
CreateSingleton(MapSelCfg);

void ServerCfg::LoadFile( char* szFile )
{
	xnExtstr* pStr	= xnExtstr::Import(szFile);
	if(pStr == NULL)
		return;

	if(pStr->SetSection(0))
	{
		strcpy(szIP,pStr->ReadString("ip","127.0.0.1"));
		nPort	= atoi(pStr->ReadString("port","9802"));
		strcpy(szAccIP,pStr->ReadString("accip","127.0.0.1"));
		nAccPort	= atoi(pStr->ReadString("accport","9801"));
		nServerID	= atoi(pStr->ReadString("serverid","101"));
		strcpy(szDBIP,pStr->ReadString("dbip","127.0.0.1"));
		strcpy(szDBUser,pStr->ReadString("dbuser","root"));
		strcpy(szDBPswd,pStr->ReadString("dbpswd","123456"));
		bool bMd5Lock	= atoi(pStr->ReadString("md5lock","0"));
		if(bMd5Lock)
		{
			Cmd5Capi	md5Capi;
			strcat(szDBPswd,"Mwzyl");
			char* szMd5	= md5Capi.Digest(szDBPswd);
			strcpy(szDBPswd,szMd5);
		}

		strcpy(szDBName,pStr->ReadString("dbtable","mw_rpg_table"));

		nLockCountry = atoi(pStr->ReadString("lockcountry","0"));
		nDefaultCountry = atoi(pStr->ReadString("defaultcountry","0"));
		nVersionDef	= atoi(pStr->ReadString("svrversion","0"));
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
