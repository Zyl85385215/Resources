#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

class _DropGroup
{
public:
	_DropGroup();
	~_DropGroup();

	int		RandGetItem();

	DWORD	wDGID;
	xnList*	pListItem;
};

class DropGroupConfig2	: public CSingleton<DropGroupConfig2>
{
public:
	DropGroupConfig2();
	~DropGroupConfig2();

	void	LoadFile(char* szFile);
	_DropGroup*	GetDropGroup(DWORD wID);

private:
	xnList*	m_pList;
};

class _EnemyAward
{
public:
	_EnemyAward();
	~_EnemyAward();

	void	RandGetItem(xnList* pList);

	int		RandGetGold();
	int		CalcExp(int nPlayerLv);

	WORD	wID;
	BYTE	byLv;		//
	DWORD	dwExp;		//经验,根据等级变化
	WORD	wGoldPer;	//万分比概率
	int		nGold;		//10%浮动

	xnList*	pListItem;		//10000一下是掉落组
};

class EnemyKillConfig	: public CSingleton<EnemyKillConfig>
{
public:
	EnemyKillConfig();
	~EnemyKillConfig();

	void	LoadFile(char* szFile);
	_EnemyAward*	GetEAward(WORD wID);
private:
	xnList*		m_pList;
};