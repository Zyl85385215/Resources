#pragma once

#ifndef _ISSERVER
#include "GlobalDefine.h"
#endif
#include "MallConfig.h"
#pragma pack (push)
#pragma pack (1)

enum
{
	C_USER_ITEM,
	C_DESTROY_ITEM,
	C_ITEM_TEST,
	C_ITEM_BANK,
	C_ITEM_STARUP,
	C_ITEM_BORE,
	C_ITEM_INLAY,
	C_ITEM_REMOVE_STONE,
	C_ITEM_COMPOSE_STONE,
	C_ITEM_EAT_EQUIP,
	C_ITEM_TRANSFER_EQUIP,
	C_ITEM_MALL_BUY,
	C_ITEM_SHOP_BUY,
	C_ITEM_SELL,
	C_ITEM_FASTSELL,
	S_SEND_ITEMINFO_SERVER,
	C_GET_ITEMINFO,
	C_SORT_BAG,
	S_SEND_BAGSIZE,
	S_SEND_BLANKBAGRLT,
	S_SEND_MALL_ITEM,
	C_ITEM_FIXALL,
	C_ADD_BAGSIZE_BYRMB,
	C_MAKE_SKILL_ITEM,
	C_MAKE_SKILL_ITEM_RESULT,
	C_ITEM_QUICK_FIX,
	C_LEARN_MAKE_SKILL,
};

struct ITEM_C_USER_ITEM_Data
{
	DWORD	dwID;
};
struct ITEM_C_MAKE_SKILL_ITEM_RESULT_Data
{
	BYTE	byResult;
};
struct ITEM_C_LEARN_MAKE_SKILL_Data
{
	BYTE	byResult;
};
struct ITEM_C_DESTROY_ITEM_Data
{
	DWORD	dwID;
};
struct ITEM_C_ITEM_BANK_Data
{
	DWORD	dwID;
};
struct ITEM_C_ITEM_STARUP_Data
{
	DWORD	dwID;
};

struct MallItemCfg{
	int		m_nItemID;
	int		m_nPrice;
	int		m_nDiscount;
	int		m_nFlag;
	int		m_nLimit;
	BYTE	m_byType;
};

struct ITEM_S_SEND_MALL_ITEM_Data
{
	int		nCnt;
	MallItemCfg	xData[1];
};
struct ITEM_C_MAKE_SKILL_ITEM_Data
{
	int		nID;
};
struct ITEM_C_ITEM_BORE_Data
{
	DWORD	dwID;
};
struct ITEM_C_ITEM_INLAY_Data
{
	DWORD	dwEquip;
	DWORD	dwStone;
};
struct ITEM_C_ITEM_REMOVE_STONE_Data
{
	DWORD	dwEquip;
	BYTE	nPos;
};
struct ITEM_C_ITEM_COMPOSE_STONE_Data
{
	DWORD	nID;
	int	nPos;
};
struct ITEM_C_ITEM_EAT_EQUIP_Data
{
	DWORD	nEquip;
	DWORD	nEat;
};
struct ITEM_C_ITEM_TRANSFER_EQUIP_Data
{
	DWORD	nEquip;
	DWORD	nEat;
};
struct ITEM_C_ITEM_MALL_BUY_Data
{
	BYTE	byType;
	DWORD	nItemID;
	DWORD	nCount;
};
struct ITEM_C_ITEM_SHOP_BUY_Data
{
	int		nType;
	DWORD	nItemID;
	DWORD	nCount;
};
struct ITEM_C_ITEM_SELL_Data
{
	DWORD	nItemID;
};
struct ITEM_C_ITEM_FASTSELL_Data
{
	DWORD	nType;
};
struct ITEM_S_SEND_ITEMINFO_SERVER_Data
{
	int		nSize;
	int		nKey;
	BYTE	byData[1];
};
struct ITEM_C_GET_ITEMINFO_Data
{
	int		dwUin;
	int		nItemID;
	int		nKey;
};
struct ITEM_S_SEND_BAGSIZE_Data
{
	int		nBagsize;
};
#pragma pack (pop)