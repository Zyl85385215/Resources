#pragma once

#include "singleton.h"
#include "xnlist.h"

class ServerCfg	: public CSingleton<ServerCfg>
{
public:
	void		LoadFile(char* szFile);

	char		szIP[20];
	int			nPort;
	char		szAccIP[20];
	int			nAccPort;
	char		szDBIP[20];
	char		szDBUser[20];
	char		szDBPswd[50];
	char		szDBName[20];
	int			nServerID;
	int			nLockCountry;
	int			nDefaultCountry;
	int			nVersionDef;
};

class JNetClient;
class MapSrvObj
{
public:
	MapSrvObj()	{	pList = xnList::Create();pMapSvr=NULL;	}
	~MapSrvObj()	{	pList->Free();	}
	char		szIP[20];
	int			nPort;
	xnList*		pList;

	JNetClient*	pMapSvr;
};

class MapSelCfg	: public CSingleton<MapSelCfg>
{
public:
	MapSelCfg();
	~MapSelCfg();

	void		LoadFile(char* szFile);
	MapSrvObj*	GetByMapIndex(int nMIndex);
	int			GetMapSvrCnt()	{return m_pList->Count+1;}

	MapSrvObj*	m_pMapSrvDefaul;

	xnList*		m_pList;	
};