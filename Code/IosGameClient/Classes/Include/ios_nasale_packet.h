#pragma once

#ifdef _ISSERVER
#include <Windows.h>
#else
#include "GlobalDefine.h"
#endif

#pragma pack (push)
#pragma pack (1)


enum NASALEENUM                     //排行榜种类
{
	NA_SELL_ITEM,		//寄售物品
	NA_REQUST_ITEM,		//求购物品
	NA_BUY_ITEM,		//买物品
	NA_ANSWER_ITEM,		//卖物品给求购

	NA_SEARCH_SELL,
	NA_SELL_LIST,

	NA_MY_LIST,

	NA_CALC_MYMONEY,

	NA_SELL_ITEM_EX,		//寄售物品带数量
};

class NAItem_Data
{
public:
	DWORD	dwNaKey;
	BYTE	byType;		//0寄售  1收购

	WORD	wItemID;
	WORD	wCnt;
	int		nPrice;
	int		nRmSec;
	DWORD	dwExInfo;
};

struct NASALE_NA_SELL_ITEM_Data
{
	DWORD	dwIndex;
	int		nPrice;			//单价
};

struct NASALE_NA_SELL_ITEM_EX_Data
{
	DWORD	dwIndex;
	int		nPrice;
	int     nCount;
};

struct NASALE_NA_REQUST_ITEM_Data
{
	WORD	wItemID;
	WORD	wCnt;
	int		nPrice;			//单价
};

struct NASALE_NA_BUY_ITEM_Data
{
	DWORD	dwNAIndex;
	WORD	wCnt;
	
};

struct NASALE_NA_ANSWER_ITEM_Data
{
	DWORD	dwNAIndex;
	WORD	wCnt;
};

struct NASALE_NA_SELL_LIST_Data
{
	WORD	wCnt;
	int		wPageCnt;
	int		nEndPos;
	NAItem_Data	szData[1];
};

struct NASALE_NA_MY_LIST_Data
{
	WORD	wCnt;
	NAItem_Data	szData[1];
};

struct NASALE_NA_SEARCH_SELL_Data
{
	BYTE	bType;
	BYTE	bJob;
	BYTE	bLvStep;
	int		nStartPos;
};

#pragma pack (pop)