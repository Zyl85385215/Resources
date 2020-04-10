#pragma once

#include <Windows.h>
#include "singleton.h"
#include "xnlist.h"

#define PLUS_VALUE_COUNT 5

#define ItemReputePartNum 4
#define MAX_REPUTE_LV 20

struct RefreshOnePlusParamResult 
{
	char szName[128];
	int	nValue;
};

struct ItemPlusData 
{
	BYTE bValueType[10];
	int nMinValue[10];
	int nMaxValue[10];
};

struct RidePlusData
{
	RidePlusData()
	{
		nID = 0;
		for (int i=0;i<10;i++)
		{
			nMinValue[i] = 0;
			nMaxValue[i] = 0;
		}
	}
	int	nID;
	int nMinValue[10];
	int nMaxValue[10];
};

enum
{
	item_plus_type_weapon,
	item_plus_type_secweapon,
	item_plus_type_helmet,
	item_plus_type_armor,
	item_plus_type_wrist,
	item_plus_type_shoes,
	item_plus_type_mantle,
	item_plus_type_necklace,
	item_plus_type_ring,
	item_plus_type_waist,
	item_plus_type_trump,
	item_plus_type_MAX,
};

enum
{
	item_plus_waigong,
	item_plus_neigong,
	item_plus_waifang,
	item_plus_neifang,
	item_plus_maxhp,
	item_plus_maxmp,
	item_plus_acc,
	item_plus_dodge,
	item_plus_zhongji,
	item_plus_zhiming,
	
	item_plus_MAX,
};


class ItemPlusConfig:public CSingleton<ItemPlusConfig>
{
public:
	bool	load(const char * filename);
	ItemPlusConfig();
	~ItemPlusConfig();
	ItemPlusData * m_pData[4][item_plus_type_MAX];
	xnList * m_pRidePlusList;
	RidePlusData * GetRidePlusData(int nID);
private:
	
};


class ItemReputeConfig:public CSingleton<ItemReputeConfig>
{
public:
	bool	load(const char * filename);
	ItemReputeConfig();
	~ItemReputeConfig();

	int m_pNeedVal[item_plus_type_MAX][MAX_REPUTE_LV];
	int m_pNeedRepute[item_plus_type_MAX][MAX_REPUTE_LV];
	int ** ReputeItemIDList;
	int		ReputeItemCount;
	int	isReputeEquip(WORD itemID);//-1 不是，其他为数组下标
/*
	xnList * m_pNeedValList;
	xnList * m_pNeedReputeList;*/

	
private:

};

class UpgradeStoneUpgradeConfig:public CSingleton<UpgradeStoneUpgradeConfig>
{
public:
	bool	load(const char * filename);
/*
	UpgradeStoneUpgradeConfig();
	~UpgradeStoneUpgradeConfig();*/


	DWORD	itemid[19];
	int		itemexp[19];
	int		maxval[19];
	int		minval[19];
	
	int	isUpgradeStone(DWORD itemID);
};
struct ReplaceItem 
{
	DWORD itemID;
	DWORD itemNum;
};
struct ItemReplaceInfo 
{
	ReplaceItem *	NeedItem;
	ReplaceItem *	GetItem;
	int				NeedCount;
	int				GetCount;
	ItemReplaceInfo();
	~ItemReplaceInfo();
};
class ItemReplaceConfig:public CSingleton<ItemReplaceConfig>
{
public:

	bool	load(const char * filename);
//	ItemReplaceInfo * pItemReplaceInfo;
	xnList * pItemReplaceInfo;
	char	txt[1024];
	//int		count;
	~ItemReplaceConfig();
	ItemReplaceConfig();
	ItemReplaceInfo * GetReplaceInfo(DWORD site);
};