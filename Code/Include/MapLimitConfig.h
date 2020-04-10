#pragma once

#include "singleton.h"
#include "windows.h"
#include "xnlist.h"

class MapLimitObj
{
public:
	MapLimitObj()
	{
		pListItem	= xnList::Create();
	}
	~MapLimitObj()
	{
		pListItem->Free();
	}
	bool		CheckItemCanUse(int nItemID);
	int			nMapIndex;
	bool		bLimitGroup;
	bool		bLimitSlave;		//1表示无法召唤宠物，0表示可以召唤宠物
	bool		bLimitItem;			//1表示只有以下物品可用，0表示只有以下物品不可用
	bool		bLimitDeath;		//1表示没有死亡经验惩罚,0表示有
	bool		bLimitHeibaiGroup;  //1表示不允许黑白组队，0相反
	xnList*		pListItem;			//可用物品列表
};

class MapLimitConfig : public CSingleton<MapLimitConfig>
{
public:
	MapLimitConfig();
	~MapLimitConfig();

	void	LoadFile(char* szFile);
	MapLimitObj*	GetMapLimit(int nMapIndex);		//地图加载时直接挂在地图上

private:
	xnList*		m_pListMapLimit;
};