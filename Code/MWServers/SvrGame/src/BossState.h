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
//	//xnList*	pListMap;		//���ڵ�ͼ�б�,Ϊ0��ʾ������,ͬһ����ͼ���Դ��ڶ��
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