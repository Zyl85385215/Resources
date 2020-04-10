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
	
	//�ͻ��˰汾
	char*	szName;
	WORD	wBaseMap;
	WORD	wIcon;
	BYTE	byType;		//0,��ͨ, 1��Ӣ,2Ӣ��
	int		nExPow;		
	BYTE	byChgLv;
	BYTE	byLvNeed;	//�ȼ�����
	BYTE	byLifeCost;	//��������

	BYTE	szbyWeekDay[7];
	
	BYTE	byLimitHelp;
	BYTE	byLimitSlave;
	BYTE	byLimitGroup;
	BYTE	byLimitItem;

	BYTE	byLimitTime;	//ÿ�ս������

	BYTE	byWinType;		//ͨ������, 0ɱ�ض���
	WORD	wWinID;			//����ID

	int		nStartX,nStartY;

	xnList*	pLLimitTime;
	xnList*	pLHelper;
	BYTE	byHelpNum;
	int		nGold;
	int		nExp;
	DWORD   dwRankTime;
	xnList*	pLItemGet;

public:
	xnList*	pLRqMCopy;		//�����������
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