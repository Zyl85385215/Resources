#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include <vector>
#include <string>


struct VipInfoObj
{
	int m_nLv;
	int m_nNeedgold;							//升级所需充值
	int	m_nExpadd;								//经验加成  ID
	int	m_nTaxsub;								//税收减免  百分比
	int	m_nPrestigeadd;						//声望加成  百分比
	int	m_nField;								//土地增加个数
	bool	m_fixswitch;						//挂机修理开关
	bool	m_chatswitch;						//聊天特效开关
	bool	m_buyswitch;						//挂机购买开关
	int	m_nPoweradd;							//免费体力点
	int	m_nCardtimes;							//副本额外翻牌次数
	bool	m_proclamtion;					//上线公告开关
	bool	m_skillswitch;						//挂机自动释放触发技能
	std::vector<std::string>	vecdesc;	//描述，换行加“\n”
};

class VipConfig : public CSingleton<VipConfig>
{
public:
	VipConfig();
	~VipConfig();

	VipInfoObj* GetObjByLv(int nLv);
	void LoadFile(char* pFile);
	int GetMaxLvl() { return m_pList->Count;}
private:
	xnList* m_pList;
};

struct VipAward
{
	int      nGold;
	xnList*  pList;
};

class VipAwardConfig : public CSingleton<VipAwardConfig>
{
public:
	VipAwardConfig();
	~VipAwardConfig();

	VipAward* GetObjByVipLv(int nLv);
	void LoadFile(char* pFile);
private:
	xnList* m_pList;
};