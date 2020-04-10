#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include <map>

static char* s_qualityName[] = {"★☆☆☆☆","★★☆☆☆","★★★☆☆","★★★★☆","★★★★★"};

enum _SLAVE_SKILL
{
	SLAVE_SKILL_ID1,
	SLAVE_SKILL_ID2,
	SLAVE_SKILL_ID3,
	SLAVE_SKILL_ID4,
	SLAVE_SKILL_ID5,
	SLAVE_SKILL_ID6,
	SLAVE_SKILL_MAX,
};

enum _SLAVE_STRENGTHEN
{
	SLAVE_STRENGTHEN_HP,
	SLAVE_STRENGTHEN_MP,
	SLAVE_STRENGTHEN_AD,
	SLAVE_STRENGTHEN_DD,
	SLAVE_STRENGTHEN_AP,
	SLAVE_STRENGTHEN_DP,
	SLAVE_STRENGTHEN_MAX,
};

enum 
{
	SLAVE_NUM_SKILL,
	SLAVE_NUM_STRENGTHEN,
	SLAVE_NUM_SKILLLV,
	SLAVE_NUM_MAX,
};

enum
{
	SLAVE_PARAM_HPMAX,
	SLAVE_PARAM_MPMAX,
	SLAVE_PARAM_AD,
	SLAVE_PARAM_DD,
	SLAVE_PARAM_AP,
	SLAVE_PARAM_DP,
	SLAVE_PARAM_MAX,
};

struct SlaveBasic
{
	char*	pName;								//名字
	int		nID;								//ID
	int		szBaseParam[SLAVE_PARAM_MAX];		//初始属性
	int		szBaseUp[SLAVE_PARAM_MAX];			//成长度
	BYTE	byQuality;							//品质
	int		nSkill;								//初始技能
	int		nSkin;								//皮肤
	BYTE	byLvLimit;							//等级上限
	BYTE	byExchangLv;						//当前进化等级
	int		nExchangeID;						//进化ID
	int		nExchangeGold;						//进化消耗银两
	std::map<int,int> mapExchangeCost;			//进化所需宠物
	std::map<int,int> mapExchangeItem;			//进化所需物品
	int		nAtkDis;							//宠物攻击距离
};
class UserData;
class SlaveBaseCfg : public CSingleton<SlaveBaseCfg>
{
public:
	SlaveBaseCfg();
	~SlaveBaseCfg();
	void	LoadFile(char * pFileName);
	SlaveBasic* GetBaseInfo(int nID);

	void	LoadLvFile(char* pFileName);
	int		GetLvExp(int nLv);

	int		RandSlaveID(int nType,UserData*	pUser);

private:
	xnList* m_pList;
	xnList* m_pLvList;
#ifdef _ISSERVER
	xnList* m_pQWhiteList;
	xnList* m_pQGreenList;
	xnList* m_pQBlueList;
	xnList* m_pQPurpleList;
#endif
};

struct SlaveStreBasic
{
	int nVal;				//数值加成
	int nSucPer;			//强化成功率
	int nLvNeed;			//强化所需宠物等级
	int nMoneyCost;			//强化消耗银两
	int nItemCost;			//强化消耗强化丹数量
};

class SlaveStrengthenCfg : public CSingleton<SlaveStrengthenCfg>
{
public:
	SlaveStrengthenCfg();
	~SlaveStrengthenCfg();
	void	LoadFile(char* pFileName);
	SlaveStreBasic* GetBase(BYTE byPos,int nLv);

private:
	//xnList* m_pList[SLAVE_PARAM_MAX];
	xnList* m_pList;
};

struct SlaveVariatBasic
{
	int nVal;		//属性加成（万分比）
	int nMoney;		//消耗银两
	int nSucPer[4]; //不同品质提供的成功率
};

class SlaveVariatCfg : public CSingleton<SlaveVariatCfg>
{
public:
	SlaveVariatCfg();
	~SlaveVariatCfg();
	void	LoadFile(char* pFileName);
	SlaveVariatBasic* GetBase(BYTE byValv);
	int		GetMaxLv() {return m_pList->Count;}

private:
	xnList* m_pList;
};