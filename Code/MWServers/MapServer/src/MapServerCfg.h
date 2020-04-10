#pragma once

#include "singleton.h"
#include "xnlist.h"

class MapSrvObj
{
public:
	MapSrvObj()	{	pList->Create();	}
	~MapSrvObj()	{	pList->Free();	}
	char		szIP[20];
	int			nPort;
	xnList*		pList;
};

class MapSelCfg	: public CSingleton<MapSelCfg>
{
public:
	MapSelCfg();
	~MapSelCfg();

	void		LoadFile(char* szFile);
	MapSrvObj*	GetByMapIndex(int nMIndex);

private:
	MapSrvObj*	m_pMapSrvDefaul;
	xnList*		m_pList;	
};

class MapServerCfg	: public CSingleton<MapServerCfg>
{
public:
	void		LoadFile(char* szFile);

	char		szIP[20];
	int			nPort;
	char		szGameIP[20];
	int			nGamePort;

	MapSrvObj*	pMapSrv;
};