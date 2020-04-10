#include "stdafx.h"
#include "NetGateCfg.h"
#include "xnExtstr.h"

CreateSingleton(NetGateCfg);
CreateSingleton(MapSelCfg);

void NetGateCfg::LoadFile( char* szFile )
{
	xnExtstr* pStr	= xnExtstr::Import(szFile);
	if(pStr == NULL)
		return;

	if(pStr->SetSection(0))
	{
		strcpy(szIP,pStr->ReadString("ip","127.0.0.1"));
		nPort		= atoi(pStr->ReadString("port","9800"));
		strcpy(szAccIP,pStr->ReadString("accip","127.0.0.1"));
		nAccPort	= atoi(pStr->ReadString("accport","9801"));
		strcpy(szGameIP,pStr->ReadString("gameip","127.0.0.1"));
		nGamePort	= atoi(pStr->ReadString("gameport","9802"));
		nMaxConn	= atoi(pStr->ReadString("maxconnect","2000"));
		bySubKey	= atoi(pStr->ReadString("id","1"));

		char	szTmp[32];
		int	nGMCmdCnt	= atoi(pStr->ReadString("gmcmdcnt","0"));
		for (int i=0;i<nGMCmdCnt;i++)
		{
			sprintf(szTmp,"gmcmdip%d",i+1);
			m_setIP.insert(inet_addr(pStr->ReadString(szTmp,"127.0.0.1")));
		}
	}
}

bool NetGateCfg::CheckGMTool( DWORD dwIp )
{
	return true;
	if (m_setIP.find(dwIp) != m_setIP.end())
	{
		return true;
	}
	return false;

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

BYTE MapSelCfg::GetByMapIndex( int nMIndex )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		MapSrvObj* pObj	= (MapSrvObj*)m_pList->Items[i];
		for (int j = 0; j < pObj->pList->Count; j++)
		{
			if(nMIndex == (int)pObj->pList->Items[j])
			{
				return i+1;
			}
		}
	}
	return 0;
}
