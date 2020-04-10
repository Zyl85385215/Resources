#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include "ios_mapobj_packet.h"
#define		MAXJOB	4	

class StarLvObject
{
public:
	StarLvObject();
	~StarLvObject();

	BYTE	byLv;
	int		nCostGold;
	int		nCostExp;

	char*	szIco1;
	char*	szIco2;

	xnList*	pListParam[MAXJOB];
	MParam	xParamTotal[MAXJOB];
};

class StarSysCfg : public CSingleton<StarSysCfg>
{
public:
	StarSysCfg();
	~StarSysCfg();

	void	LoadFile(char * pFile);
	StarLvObject*	GetStarInfo(BYTE bLv);

	xnList* m_pList;
};