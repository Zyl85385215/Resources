#pragma once

#include "singleton.h"
#include "xnlist.h"
#include "ShipConfig.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

enum
{					//物品和材料都在这里面
	ITEM_AD_UIN,
	ITEM_AD_ID,			//唯一标示
	ITEM_AD_TYPE,		//进阶类型PHASE
	ITEM_AD_NUM,			//数量

	ITEM_AD_MAX,
};

class ItemBase
{
public:
	ItemBase();
	~ItemBase();
	
	int		nTypeID;
	char*	szName;
	char*	szDesc;
	int		nSpr;
	int		nStar;
	bool	bCanUsed;
	
	BYTE	byEffType;
	WORD	wEffVal;
};

class ItemConfig	: public CSingleton<ItemConfig>
{
public:
	ItemConfig();
	~ItemConfig();

	void		LoadFile(char*	szFile);
	
	ItemBase*	GetItemBase(int nTypeID);

private:
	xnList*		pListItems;			//可使用物品
	//xnList*		pListMaterials;
};