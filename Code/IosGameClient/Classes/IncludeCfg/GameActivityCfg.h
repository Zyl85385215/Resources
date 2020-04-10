#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

struct ActivityObj
{
	int nID;
	char* pName;
	char* pIcon;
	BYTE byCountry;
	int nLvMin;
	int nLvMax;
	xnList* pWeekList;
	DWORD dwStartTime;
	DWORD dwEndTime;
	DWORD dwRegistTime;
	BYTE byActionEvent;
	char* pObject;
	char* pDesc;
	int   nAwardType;
	int   nFlag;
	int   nMaxnum;
};

class GameActivityCfg	: public CSingleton<GameActivityCfg>
{
public:
	GameActivityCfg();
	~GameActivityCfg();

	void	LoadFile(char* pFile);
	ActivityObj* GetObj(int nID);

	xnList* m_pList;
};

struct TurntableAward
{
	int nID;
	int nNum;
	int nVal;
};

class TurntableCfg	: public CSingleton<TurntableCfg>
{
public:
	TurntableCfg();
	~TurntableCfg();

	void	LoadFile(char* pFile);
	int		RandPos();
	xnList* m_pList;

	TurntableAward m_szAward[8];
};

struct BossInfoObj
{
	int nID;
	int nLv;
	char* pName;
	int pIcon;
	char* pDesc;
	int nMap;
};

class ACBossCfg	: public CSingleton<ACBossCfg>
{
public:
	ACBossCfg();
	~ACBossCfg();

	void	LoadFile(char* pFile);
	BossInfoObj* GetObj(int nID);

	xnList* m_pList;
};