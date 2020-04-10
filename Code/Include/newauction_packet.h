#ifndef _NEWAUCTION_PACKET_H_
#define _NEWAUCTION_PACKET_H_

#include "NAuction_Data.h"
#pragma pack (push)
#pragma pack (1)

enum
{
//----------------中心服务器收----------
	NA_ADD_ITEM,			//新增物品
	NA_TBUG_ITEM,			//尝试出价
	NA_REQUEST_LIST,		//请求列表
	NA_REQUEST_ITEMS,		//根据编号请求物品详细信息
	NA_REQUEST_OWNER,
	NA_PICKOUT_ITEM,

//----------------中心服务器发----------
	NA_SEND_LIST,			//发送物品简单信息
	NA_SEND_ITEMS,			//发送详细信息
	NA_UPDATA_ITEM,
	NA_DEL_ITEM,

//----------------跟游戏服务器的交互------------
	NA_GS_TBUG_ITEM,
	NA_GS_GIVE_ITEM,

	NA_SEND_MSG2ALL,

	NA_ADD_RANK_EQUIP,
	NA_RANK_EQUIP_INFO,
	NA_REQUEST_REQUIPINFO,
	NA_OPEN_GOLDAUCTION,
	NA_CLOSE_GOLDAUCTION,
	NA_REQUEST_GOLDLIST,
	NA_GS_TBUG_GOLD_ITEM,
	NA_TBUG_GOLDITEM,
	NA_GET_GOLDITEM_NUM,
	NA_FLESH_ITEM_NUM,
	//NA_SEND_HOME_BANK_ITEMS,
	//NA_SEND_HOME_SIMPLE_INFO,
};

//直接用排行榜的DATA来处理

class NA_ItemInfo
{
public:
	char	szSeller[16];	//寄售者
	char	szBuyer[16];		//当前的最高价格者
	int	   nPriceCur;	//当前价
	int	   nPriceMax;	//一口价
	DWORD  dwKey;		//识别编号
	DWORD  dwTmRemain;		//剩余秒数,设置为0表示已卖出
	BYTE   byLvl;		//物品等级
	BYTE   byGoldType;
	WORD   wItemID;	//
	BYTE   byFlag;
	WORD   wData;   //个数
};

struct	NAITEM_NA_GS_TBUG_ITEM_Data
{
	DWORD	dwUin;
	int		nMoney;
	DWORD	dwKey;
	DWORD   dwItemNum;
};

struct  NAITEM_NA_GS_TBUG_GOLD_ITEM_Data
{
	DWORD	dwMark;
	DWORD   dwUin;
	int     nGold;
	DWORD   dwKey;
	DWORD   dwItemNum;
};

struct	NAITEM_NA_GS_GIVE_ITEM_Data
{
	DWORD	dwToUin;
	DWORD	dwFromUin;
	LONGLONG	llKey;
	BYTE	data[1];
};

struct	NAITEM_NA_SEND_MSG2ALL_Data
{
	char	data[1];
};

struct	NAITEM_NA_PICKOUT_ITEM_Data
{
	//DWORD	dwUin;
	DWORD	dwKey;
};

struct  NAITEM_NA_ADD_ITEM_Data
{
	//DWORD  dwSeller;	//寄售者
	int	   nPriceCur;	//当前价
	int	   nPriceMax;	//一口价
	LONGLONG	   llKey;		//物品INDEX
	BYTE   byType;		//物品类型
	BYTE   byLvl;		//物品等级
	BYTE   byGoldType;	//0金币,1元宝
	char   szSearch[32];	//检索关键字,默认为物品名字
	BYTE   pData[1];
};

struct  NAITEM_NA_ADD_RANK_EQUIP_Data
{
	//DWORD	dwUin;	//寄售者
	DWORD	dwPoint;
	LONGLONG llItemKey;
	BYTE	bRankType;
	BYTE	pData[1];
};


struct  NAITEM_NA_RANK_EQUIP_INFO_Data
{
	DWORD	dwIndex;
	//BYTE	byType;
	//BYTE	byCountry;
	BYTE	pInfo[1];
};

struct	NAITEM_NA_REQUEST_REQUIPINFO_Data
{
	//DWORD	dwUin;
	BYTE	byCnt;
	DWORD	dwIndex[1];
};

struct  NAITEM_NA_OPEN_GOLDAUCTION_Data
{

};

struct  NAITEM_NA_CLOSE_GOLDAUCTION_Data
{

};

struct  NAITEM_NA_TBUG_ITEM_Data
{
	//DWORD dwUin;
	DWORD dwKey;
	int	  nMoney;
	DWORD dwItemNum;
};

struct  NAITEM_NA_TBUG_GOLDITEM_Data
{
	DWORD dwKey;
	int  nGold;
	DWORD dwNum;
};

struct  NAITEM_NA_GET_GOLDITEM_NUM_Data
{
	DWORD dwUin;
};

struct NAITEM_NA_FLESH_ITEM_NUM_Data
{
	int num;
};

struct  NAITEM_NA_REQUEST_LIST_Data
{
	//DWORD dwUin;
	NA_SearchKey cNASKey;
};

struct NAITEM_NA_REQUEST_OWNER_Data
{
	//DWORD dwUin;
	BYTE bGoldType;
};

struct NAITEM_NA_REQUEST_GOLDLIST_Data
{
	BYTE flag;
};
struct	NAITEM_NA_REQUEST_ITEMS_Data
{
	//DWORD	dwUin;
	BYTE    bGoldType;
	WORD	nCnt;
	DWORD	szKey[1];
};

struct	NAITEM_NA_SEND_LIST_Data
{
	BYTE		byPart;	//0部分的时候客户端清除,之后的不管,加后面
	WORD		nCnt;
	NA_ItemInfo	szNaItems[1];
};

struct	NAITEM_NA_UPDATA_ITEM_Data
{
	NA_ItemInfo	szNaItem;
};

struct	NAITEM_NA_SEND_ITEMS_Data
{
	BYTE	itemList[1];
};

struct NAITEM_NA_DEL_ITEM_Data
{
	DWORD dwKey;
};

#pragma pack (pop)

#endif