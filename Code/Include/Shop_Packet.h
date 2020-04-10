#ifndef _SHOP_PACKET_H_
#define _SHOP_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum {
	SHOP_NAME,
	SHOP_ITEMS,
	SHOP_ITEM,
	SHOP_ITEM_PRICE,
	SHOP_ITEM_NUM,
	SHOP_ITEM_APPEAR,
	
	SELL_ITEM_FROM_PLAYER,
	BUY_ITEM_FROM_SHOP,
	BUY_SELL_SOLD_SHOP,
	ENGAGE_ITEMS,
	BUY_ENGAGE,
	SHOP_FUNCTIONS,
	SHOP_BUY_SELL_DEL,
	SHOP_BUY_SELL_ADD,
	SHOP_ERROR,
	SHOP_ITEMBASE,
	SHOP_SHENGWANGID,
};

enum {
	SHOP_NO_ITEM,
	SHOP_ITEM_NOTENOUGH,
	SHOP_MONEY_NOTENOUGH,
	SHOP_HONOUR_NOTENOUGH,
	SHOP_HONOURLVL_LIMIT,
	SHOP_SHENGWNAGLVL_LIMIT,
	SHOP_ERROR_MAX,
};

enum
{
	SHOP_MONEY_TYPE_SILVER,
	SHOP_MONEY_TYPE_UNBIND_GOLD,
	SHOP_MONEY_TYPE_BIND_GOLD,
	SHOP_MONEY_TYPE_MAX,
};

struct SHOP_SHOP_NAME_Data {
	BYTE	type;			// 商店收购物品的种类， 根据物品种类的方式以bit为识别
	char	name[1];		// 根据名字长度动态变动
};

struct SHOP_SHOP_ITEMS_Data {
	WORD	count;
	WORD    itemID[1];		
};

struct SHOP_SELL_ITEM_FROM_PLAYER_Data {
	DWORD	itemIndex;
};

struct SHOP_BUY_ITEM_FROM_SHOP_Data {
	WORD	itemID;
	WORD    count;
	bool    bFree;
};

struct SHOP_BUY_SELL_SOLD_SHOP_Data {
	BYTE    bindex;
};

struct SHOP_ENGAGE_ITEMS_Data{
	WORD level;
	WORD maxHp;
	WORD attackType;
	WORD time;
	DWORD price;
	WORD type;
	char szName[1];
};

struct SHOP_SHOP_SITEMBASE_Data {
	WORD    itemID;
	WORD    price;
	int     count;
	int     money;
	int     honour;
	int     specil; 
	BYTE    bInfinite;
	BYTE    bNeedRanklvl;
	BYTE    bNeedHonourlvl;
};

struct SHOP_SHOP_ITEMBASE_Data {
	BYTE  	count;
	SHOP_SHOP_SITEMBASE_Data items[1];
};

struct SHOP_SHOP_SHENGWANGID_Data {
	int     nShengwangId;
	bool    bflag;
};

struct SHOP_SHOP_SITEM_Data {
	WORD    itemID;
	WORD    price;
	int     count;
	int     money;
	int     honour;
	int     specil; 
	BYTE    bInfinite;
	BYTE    bNeedRanklvl;
	BYTE    bNeedHonourlvl;

	BYTE	bAppearPercent;
	BOOL	bUseUnbindMoney;
	BYTE	bDrugLevel;
	BYTE	bWeaponLevel;
	BYTE	bArmorLevel;
	BYTE	bAccLevel;
	int		nNeedContribute;
	BOOL	bAppear;
};

struct SHOP_SHOP_ITEM_Data {
	BYTE  	count;
	SHOP_SHOP_SITEM_Data items[1];
};

struct SHOP_SHOP_ITEM_PRICE_Data {
	WORD   itemID;		
	WORD   price;
	WORD   price1;
};

struct SHOP_SHOP_ITEM_NUM_Data {
	WORD    itemID;		
	int  	count;
};

struct SHOP_SHOP_ITEM_APPEAR_Data 
{
	WORD	itemID;
	BOOL	bAppear;
};

struct SHOP_SHOP_FUNCTIONS_Data {
	bool   bflag[5];
};

struct SHOP_SHOP_BUY_SELL_DEL_Data {
	BYTE   count;
};

struct SHOP_SHOP_BUY_SELL_ADD_Data {
	BYTE   count;
	WORD   itemID;
	int    time;
	WORD   price;
};


struct SHOP_SHOP_ERROR_Data {
	BYTE bType;
};

#pragma pack (pop)
#endif