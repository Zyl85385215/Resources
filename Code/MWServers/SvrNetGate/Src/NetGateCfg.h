#pragma once

#include "singleton.h"
#include "xnlist.h"
#include <set>
using namespace std;
class NetGateCfg	: public CSingleton<NetGateCfg>
{
public:
	void		LoadFile(char* szFile);

	char		szIP[20];
	int			nPort;
	char		szAccIP[20];
	int			nAccPort;
	char		szGameIP[20];
	int			nGamePort;
	int			nMaxConn;
	BYTE		bySubKey;


	bool		CheckGMTool(DWORD dwIp);
	set<DWORD>	m_setIP;
};

class MapSrvObj
{
public:
	MapSrvObj()	{	pList = xnList::Create();	}
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
	BYTE		GetByMapIndex(int nMIndex);
	int			GetMapSvrCnt()	{return m_pList->Count+1;}
	
	MapSrvObj*	m_pMapSrvDefaul;
	xnList*		m_pList;	
};