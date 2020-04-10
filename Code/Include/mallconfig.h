#pragma once

#include <Windows.h>
#include <map>
#include <list>
#include "ItemBase.h"
#include "xnExtstr.h"
#include "singleton.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }

enum EMALL_TYPE
{
	MALL_MEDICINE,		//丹药
	MALL_UPGRADE,		//冲装
	MALL_HORSE,			//坐骑
	MALL_SLAVE,			//宠物
	MALL_SPACE,			//空间
	MALL_CONFORT,		//便捷
	MALL_ASSISTANT,		//辅助
	MALL_FUDAI,			//符袋
	MALL_BOON,			//福利区
	MALL_HIDE,			//衍生区

	Y_MALL_MEDICINE,	//丹药
	Y_MALL_UPGRADE,		//冲装
	Y_MALL_HORSE,		//坐骑
	Y_MALL_SLAVE,		//宠物
	Y_MALL_SPACE,		//空间
	Y_MALL_CONFORT,		//便捷
	Y_MALL_ASSISTANT,	//辅助
	Y_MALL_FUDAI,		//符袋
	Y_MALL_BOON,		//福利区
	Y_MALL_HIDE,		//衍生区

	BG_MALL_MEDICINE,	//丹药
	BG_MALL_UPGRADE,	//冲装
	BG_MALL_HORSE,		//坐骑
	BG_MALL_SLAVE,		//宠物
	BG_MALL_SPACE,		//空间
	BG_MALL_CONFORT,	//便捷
	BG_MALL_ASSISTANT,	//辅助
	BG_MALL_FUDAI,		//符袋
	BG_MALL_BOON,		//福利区
	BG_MALL_HIDE,		//衍生区

	WW_MALL_EXITEM,

	MALL_MAX,
};

struct MallShopItemData
{
	WORD	itemID;
	BYTE	type;
	int		price;		//自定义价格的时候使用
	BYTE	bGoldType;
	bool	bCanSell;	//是否可以卖出（包括赠送）
	bool	bCanGive;	//是否可以赠送
	WORD	wLimitCnt;	//限制购买数量（0表示没有限制）
};

struct SellMallItem
{
	WORD	itemID;
	BYTE	bBindType;		//0,1,10
	int		price;
	int		nSellCnt;
};

class MallSellConfig : public CSingleton<MallSellConfig>
{
public:
	MallSellConfig();
	~MallSellConfig();

	bool Load(const char * szFile);
	SellMallItem* GetItem(int nItemID);

private:
	xnList* m_pList;
};

struct MallShopId
{
	MallShopId(WORD itemId, BYTE goldType)
	{
		itemId_ = itemId;
		goldType_ = goldType;
	}

	WORD	itemId_;
	BYTE	goldType_;

	friend bool operator<(const MallShopId & id1, const MallShopId & id2)
	{
		if(id1.itemId_ != id2.itemId_)
		{
			return id1.itemId_ < id2.itemId_;
		}
		return id1.goldType_ < id2.goldType_;
	}
};

class CMallShopItemList
{
public:
	CMallShopItemList(ItemBase * pItemBase);
	~CMallShopItemList(void);

	bool Load(const char * szFile);
	bool GetItem(int nItemID, BYTE goldType, MallShopItemData & xMallShopItemData);
	void SendMallShopItems(DWORD dwUin);

	void UpdateItemBase(bool bSave = true);

	inline int GetVersion(void) { return m_nVersion; }

private:
	void _LoadShopItem(xnExtstr * pExtstr, int nType);

	int m_nVersion;
	std::list<MallShopItemData*> m_listMallShopItemData;
	std::map<MallShopId, MallShopItemData*> m_mapMallShopItemData;
	ItemBase * m_pItemBase;
};

class CMallConfig : public CSingleton<CMallConfig>
{
public:
	CMallConfig(void);
	~CMallConfig(void);

	void Init(ItemBase * pItemBase);
	bool Load(const char * szFile);
	bool GetItem(int nVersion, int nItemID, BYTE goldType, MallShopItemData & xMallShopItemData);
	void SendMallShopItems(DWORD dwUin,int type=0);

	inline int GetVersion(void) { return m_nLastVersion; }

private:
	void _Clear(void);
	void _LoadShopItem(xnExtstr * pExtstr, int nType);

	int m_nLastVersion;
	std::list<CMallShopItemList*> m_listMallShopItemList;

	CRITICAL_SECTION m_xMallConfigCS;
	ItemBase * m_pItemBase;
};