#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

const char* const szLuaFunctionOnQuestGiveUp = "OnQuestGiveUp";
const char* const szLuaFunctionBeforeQuestGiveUp = "BeforeQuestGiveUp";
const char* const szLuaFunctionOnQuestComplete = "OnQuestComplete";
const char* const szLuaFunctionBeforeQuestComplete = "BeforeQuestComplete";
const char* const szLuaFunctionOnQuestAccept = "OnQuestAccept";
const char* const szLuaFunctionBeforeQuestAccept = "BeforeQuestAccept";

class _QConditon
{
public:
	BYTE	bValPos;
	BYTE	bDType;		//1杀怪 2杀怪获得物品 3其他方式获得物品 8玩家行为 9引导
	WORD	wDID;			
	WORD	wDNum;
	char*	szTitle;
};

class _QMonDrop
{
public:
	WORD	wMonID;
	WORD	wItemID;		//99999 改为可见 99998破无敌
	WORD	wPerChance;
};

class QuestObj
{
public:
	QuestObj();
	~QuestObj();

	//基本信息
	WORD	wQID;
	WORD	wLine;	//主线    FLAG记这2个值, 0 表示随机任务
	BYTE	bStep;	//主线第一阶段, 同时2个step相同的表示任选一线,按顺序配

	BYTE	bType;	//显示用(0主线,1支线,2活动, 3军团, 4爵位)

	//任务条件
	BYTE	bAllDone;	//0完成其中某个, 1完成所有
	xnList*	pLPreQ;		//多个任务前置
	BYTE	bLvNeed;
	BYTE	bCtyNeed[3];	//0无国籍,1联盟,2部落
	BYTE	bJobNeed[5];	//0无职业,1战士, 2法师,3枪手,4牧师
	
	WORD	wNpcSrc;		//起始NPC
	WORD	wNpcEnd;
		
	WORD	wItemNeed;		//需要持有什么物品
	BYTE	bItemCost;		//消耗数量

	WORD	wEnemyID;		//接任务招怪
	BYTE	bCallType;		//0,杀怪, 1,2,3
	WORD	wStateID;
	
	BYTE	bDayTime;		//每日次数任务
	//任务内容
	char*	szName;

	xnList*	pLTalkSrc;		//开始对话信息
	xnList*	pLTalkEnd;
		
	char*	szDescSrc;
	char*	szDescEnd;
	
	//任务要求
	xnList*	pLQCondtion;

	xnList*	pLMonDrop;		//改变怪物掉落
	
	//任务奖励
	int		nAwardExp;
	int		nAwardGold;
	BYTE	bSelOne;		//多选1还是全部
	xnList*	pLAwardItem;	//_muleparam		A:id, B: num

	BYTE	byValCnt;
};

class QuestLine
{
public:
	QuestLine();
	~QuestLine();

	WORD		wLine;
	xnList*		m_pListQStep;
};

class QuestCfg	: public CSingleton<QuestCfg>
{
public:
	QuestCfg();
	~QuestCfg();

	void		LoadFile(char*	szFile);
	QuestObj*		GetQuestByID(WORD wQId);
	
	xnList*		m_pListQLine;

	bool		GetFamilyQuest(int nLv,xnList* pList);
	bool		GetMarryQuest(int nLv,xnList* pList);
private:
	void		_addNewQuest(QuestObj* pObj,WORD wLine);
	QuestLine*	_getQLine(WORD	wLine,bool bCreate	= false);
};