#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include <map>


enum _HORSE_CULTIVATE
{
	HORSE_CULTIVATE_HP,
	HORSE_CULTIVATE_MP,
	HORSE_CULTIVATE_AD,
	HORSE_CULTIVATE_DD,
	HORSE_CULTIVATE_AP,
	HORSE_CULTIVATE_DP,
	HORSE_CULTIVATE_TOTAL,
	HORSE_CULTIVATE_MAX,
};


enum
{
	HORSE_PARAM_HPMAX,
	HORSE_PARAM_MPMAX,
	HORSE_PARAM_AD,
	HORSE_PARAM_DD,
	HORSE_PARAM_AP,
	HORSE_PARAM_DP,
	HORSE_PARAM_MAX,
};

enum 
{
	HORSE_NUM_CULTIVATE,
	HORSE_NUM_MAX,
};

struct HorseBasic
{
	char*	pName;								//名字
	int		nID;								//ID
	int		szBaseParam[HORSE_PARAM_MAX];		//初始属性
	int		szBaseUp[HORSE_PARAM_MAX];			//成长度
	BYTE	byQuality;							//品质
	int		nSkin;								//皮肤
	BYTE	byLvLimit;							//等级上限
	int		nSpeed;								//速度
	int		nPotential;							//潜力
	int		szUpSkin[3];						//进阶后皮肤
};

class HorseBaseCfg : public CSingleton<HorseBaseCfg>
{
public:
	HorseBaseCfg();
	~HorseBaseCfg();
	void	LoadFile(char * pFileName);
	void	LoadLvFile(char* pFileName);
	int		GetLvExp(int nLv);
	HorseBasic* GetBasic(int nID);
	int		GetFeedExp(int nLv,int nExp);

private:
	xnList* m_pList;
	xnList* m_pLvList;
};