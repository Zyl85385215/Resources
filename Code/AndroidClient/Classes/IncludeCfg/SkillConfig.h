#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

class SEffTrig
{
public:
	BYTE	bTrigType;		//1攻击概率,2受击概率, 3暴击概率, 4闪避概率, 5怒气累加 6状态伤害 7次数	9技能
	BYTE	bTrigEffType;	//0出现技能图标, 1直接释放技能
	WORD	wTrigVal;		//万分比或次数
	WORD	wTrigID;		//另一个技能ID
};

class SEffLvInfo
{
public:
	SEffLvInfo()	{szDesc = NULL;}
	~SEffLvInfo();
	BYTE	byUpLvNeed;		//当前级的等级限制
	WORD	wUpItemNeed;	//升级物品消耗
	int		nMpCost;

	int		nP_X;
	BYTE	nP_Y1Type;		//当前血量, 最大血量,当前蓝量, 最大蓝量, 物攻, 魔攻, 物防, 魔防, 命中,闪避,暴击, 移动速度
	int		nP_Y1;			//万分比
	BYTE	nP_Y2Type;
	int		nP_Y2;

	DWORD	dwCDTick;
	BYTE	bAddStateID;		//触发状态ID附加
	BYTE	bAddSkillID;		//概率处的技能状态ID附加
		
	WORD	wRangeAdd;			//额外范围
	BYTE	bTargetAdd;
	WORD	wIgnoDef;			//万分比穿透防御
	char*	szDesc;
};

class SkillBase
{
public:
	SkillBase();
	~SkillBase();

	SEffLvInfo*	GetSELv(BYTE bLv,bool bSafe=true);

	WORD	wSkillID;
	BYTE	bType;			//技能分类, 0被动, 1主动  2光环   3触发可用,4 怒气技能
	WORD	wCDPos;

	//客户端信息
	char*	szName;
	//char*	szDesc;
	WORD	wIcon;		
	
	//学习要求
	BYTE	byJob;			//0不限制  1-4人形职业	宠物怪物类10 
	int		nGoldLvNeed;
	WORD	wItemLvNeed;

	WORD	wPreSkillID;	
	BYTE	bPreSkillLv;
	BYTE	byCostSPoint;

	//使用消耗
	BYTE	bCostNoPer;	//百分比消耗
	int		nMpCost;
	int		nHpCost;
	int		nSpCost;
	WORD	wItemCost;	//使用消耗物品
	
	//光效
	WORD	wStartEff;
	WORD	wFlyEff;
	WORD	wEndEff;

	DWORD	dwEndDelay;
	BYTE	byStartEffDir;
	BYTE	byOnlyEnemy;

	//触发额外技能
	xnList*	pListTrigEff;
	
	//伤害效果
	WORD	wAtkRange;
	BYTE	bAoeType;			//0自身周边范围, 1单体, 2范围, 3直线
	WORD	wAoeRange;			
	BYTE	bMaxTarget;			//最大作用个数
	BYTE	bActType;			//0无,1冲锋  2闪烁  3吸引
	BYTE	bDmgType;			//1物攻, 2魔攻,0治疗 3,召唤
	
	xnList*	pListStateAdd;		//0自身,1对方 //状态

	//各等级额外信息
	xnList*	pListLvInfo;		
};

class SkillConfig	: public CSingleton<SkillConfig>
{
public:
	SkillConfig();
	~SkillConfig();

	void			LoadFile(char*	szFile);
	void			LoadFileJob(char*	szFile);

	SkillBase*		GetSkillBase(int nID);

	SkillBase*		GetJobSkill(BYTE bJob, BYTE bPos);

	xnList*		m_pLJobSkill[5];	//职业技能
private:
	xnList*		m_pList;
};