#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

class _MCopyDrop
{
public:
	DWORD	wItemID;
	WORD	wPer;
	WORD	wPerEx;
	WORD    wDis;
};

class _MCopyTime
{
public:
	BYTE	bySHour;
	BYTE	bySMinute;
	BYTE	byEHour;
	BYTE	byEMinute;
};

class _MCopyHelp
{
public:
	WORD	wBaseID;
	BYTE	szByJobLimit[5];
};

class MCopyBase
{
public:
	MCopyBase();
	~MCopyBase();

	bool	CheckTime();
	DWORD	RandGetItem();
	WORD	RandGetHelper(BYTE bJob);
	
	WORD	wID;
	
	//客户端版本
	char*	szName;
	WORD	wBaseMap;
	WORD	wIcon;
	BYTE	byType;		//0,普通, 1精英,2英雄
	int		nExPow;		
	BYTE	byChgLv;
	BYTE	byLvNeed;	//等级需求
	BYTE	byLifeCost;	//体力消耗

	BYTE	szbyWeekDay[7];
	
	BYTE	byLimitHelp;
	BYTE	byLimitSlave;
	BYTE	byLimitGroup;
	BYTE	byLimitItem;

	BYTE	byLimitTime;	//每日进入次数

	BYTE	byWinType;		//通关条件, 0杀特定怪
	WORD	wWinID;			//怪物ID

	int		nStartX,nStartY;

	xnList*	pLLimitTime;
	xnList*	pLHelper;
	BYTE	byHelpNum;
	int		nGold;
	int		nExp;
	DWORD   dwRankTime;
	xnList*	pLItemGet;

public:
	xnList*	pLRqMCopy;		//副本申请组队
};

class MCopyConfig	: public CSingleton<MCopyConfig>
{
public:
	MCopyConfig();
	~MCopyConfig();

	void	LoadFile(char* szFile);
	MCopyBase*	GetMCopy(WORD wID);
	MCopyBase*	GetMCopyByMBase(WORD wMBase);
	xnList*		m_pList;

	int			m_nTypeCnt[3];
};