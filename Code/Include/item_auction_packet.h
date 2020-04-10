#ifndef _ITEM_AUCTION_PACKET_H_
#define _ITEM_AUCTION_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#include "item_packet.h"
#include "item_levelup_packet.h"

enum 
{
	OPEN_MENU,			//客户端打开界面的时候通知服务器
	CLOSE_MENU,			//
	
	REQUEST_CALCEL,		//
	REQUEST_BUY,		//

	SEND_INFO,			//给新打开界面的人发		
	SEND_RESULT,		//只给本人发
	EMPTY_INFO,			//没有拍卖物品了

	OPEN_AUCTION_MENU,	//服务器给客户端
	HAVE_AUCTION,

	S_SA_REFRESH,
};


enum eAuctionResultType
{
	AUCTION_ADD,
	AUCTION_CANCEL,
	AUCTION_BUY,
};

enum eAuctionResultCode
{
	AUCTION_RESULT_SUCCESS, ///>交易成功
	AUCTION_RESULT_NOT_ENOUGH_MONEY, ///>金币不足
	AUCTION_RESULT_NOT_ENOUGH_YUANBAO, ///>元宝不足
	AUCTION_RESULT_LIST_FULL,///>拍卖列表满
	AUCTION_RESULT_ITEM_NOT_EXISTS, ///>拍卖物品不存在
	AUCTION_RESULT_PRICE_TOO_LOW,
	AUCTION_RESULT_LEVEL_TOO_LOW,					//您未到20级,不能拍卖
	AUCTION_RESULT_ONE_PERSON_ONE_ITEM,				//每个玩家只能一个物品
	AUCTION_RESULT_NOT_ENOUGH_LEVEL_TO_BUY_GOLD_ITEM,//您未到30级,不能交易金元宝物品
	AUCTION_RESULT_ITEM_CAN_NOT_AUCTION,
	AUCTION_RESULT_TOO_MUCH_GOLD,			//拍卖元宝过多
	AUCTION_RESULT_BUYITNOW_TOO_LOW,		//一口价设置过低
	AUCTION_RESULT_SAFE_LOCK,				//安全锁限制
};

struct AucInfo 
{
	DWORD sellerUin;
	char seller[20];
	BYTE type;  ///>货币类型0:元宝1:金币
	int	price;
	int iBuyItNow;	//一口价
	DWORD AccountUin;
};

struct AucItem 
{
	AucInfo info;
	CLIENTITEM item;
	bool bIsHaveExtend;
	bool bIsHaveBindInfo;
	bool bIsHaveExpExtend;
	bool bIsHaveJudge;
	bool bIsHaveGem;
	bool bIsHaveTrump;
	bool bIsHavePlusInfo;
	bool bIsHaveCarve;
	ItemExtend itemextend;
	ItemBindInfo bindInfo;	
	ItemExpExtend xExpExtend;
	ExtendData itemjudge;	
	TrumpSPData trumpInfo;
	ItemPlusUpInfo plusInfo;
	ItemCarveInfo xCarveInfo;
	int nGemsSize;
	BYTE pGemsData[1];	
};

struct ITEM_AUCTION_OPEN_MENU_Data
{	
	DWORD version;
};

struct ITEM_AUCTION_REQUEST_CALCEL_Data
{	
	DWORD itemindex;
};

struct ITEM_AUCTION_REQUEST_BUY_Data
{
	DWORD uin;
	DWORD itemindex;
	int	price;
};

struct ITEM_AUCTION_SEND_INFO_Data
{
	int	nCount;				//当前列表数量
	int nCurrentPrice;		//当前价格
	int nRemainTime;		//剩余时间
	int nBuyItNow;
	char buyer[20];
	DWORD version;
    AucItem item;		//当前竞拍物品
};

struct ITEM_AUCTION_SEND_RESULT_Data
{
    BYTE ResultType;
	BYTE ResultCode;
	int	data;
};

struct ITEM_AUCTION_EMPTY_INFO_Data
{
	DWORD version;
};

struct ITEM_AUCTION_HAVE_AUCTION_Data
{
	bool isHave;
};

struct ITEM_AUCTION_S_SA_REFRESH_Data
{
	DWORD	nGold;
	DWORD	dwIndex;
	DWORD	param;
	DWORD	tmRemain;
	char	szName[16];
};



#pragma pack (pop)
#endif