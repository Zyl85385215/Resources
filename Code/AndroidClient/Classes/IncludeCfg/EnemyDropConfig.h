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
	DWORD	dwExp;		//����,���ݵȼ��仯
	WORD	wGoldPer;	//��ֱȸ���
	int		nGold;		//10%����

	xnList*	pListItem;		//10000һ���ǵ�����
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