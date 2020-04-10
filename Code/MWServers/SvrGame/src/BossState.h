#pragma once

#include "singleton.h"
#include <map>
#include <Windows.h>
#include "xnlist.h"

//class BossInfo
//{
//public:
//	BossInfo();
//	~BossInfo();
//	//xnList*	pListMap;		//所在地图列表,为0表示都死了,同一副地图可以存在多个
//	bool m_bDead;
//};

class BossState	: public CSingleton<BossState>
{
public:
	BossState();
	~BossState();

	void	BossBorn(WORD wBoss, int nMap,bool bWild);
	void	BossDead(WORD wBoss, int nMap);

	//BossInfo*	GetBInfo(WORD wBossID);
	//std::map<WORD,BossInfo*>	mapBossInfo;
	bool	CheckBossActive(WORD wBossID);
	std::map<WORD,bool>	mapBossActive;
};