#ifndef _TRADE_PACKET_H_
#define _TRADE_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#define TRADE_ITEMS_MAX 12
#define TRADE_SALVES_MAX 4

enum {
	ADD_TO_TRADELIST,
	REMOVE_FORM_TRADELIST,
	TRADE_MONEY,
	TRADE_CONFIRM,
	TRADE_CANCEL,
	TRADE_OK,
	TRADE_OFFLINE,

	TRADE_OPEN,
	TRADE_CLOSE,
	TRADE_SET_CONFIRM,
	TRADE_SET_MONEY,
	TRADE_ADD_ITEM,
	TRADE_REMOVE_ITEM,

	TRADE_OVERLOAD,

	TRADE_NAME,

	TRADE_ITEM_SLAVE_INFO,
	TRADE_ITEM_EXTEND_INFO,
	TRADE_ITEM_RIDE_INFO,
	TRADE_POINT,
	TRADE_SET_POINT,

	TRADE_LOCK,
	TRADE_LOCK_RESULT,	
	TRADE_OTHER,
	TRADE_BEGIN,
};

enum eTradeInfo
{
	TRADE_INFO_OK,
	TRADE_INFO_ITEM_CANNOT_TRADE,
	TRADE_INFO_SLAVE_NOT_EXIST,
	TRADE_INFO_SLAVE_IN_BATTLE,
	TRADE_INFO_SLAVE_OTHER_LEVEL_NOT_ENOUGH,
	TRADE_INFO_SLAVE_IN_DIVINATORY_ARRAY,
	TRADE_INFO_SLAVE_BINDED,
	TRADE_INFO_GOLD_FULL,
};

struct TRADE_ADD_TO_TRADELIST_Data {
	DWORD itemIndex;
};

struct TRADE_REMOVE_FORM_TRADELIST_Data {
	DWORD itemIndex;
};

struct TRADE_TRADE_MONEY_Data {
	DWORD	money;
};
struct TRADE_TRADE_POINT_Data {
	DWORD	point;
};
struct TRADE_TRADE_SET_POINT_Data {
	DWORD	uin;
	DWORD	point;
};
struct TRADE_TRADE_SET_CONFIRM_Data {
	DWORD	uin;
	BOOLEAN flag;
};

struct TRADE_TRADE_SET_MONEY_Data {
	DWORD uin;
	DWORD money;
};

#include "item_packet.h"

struct TRADE_TRADE_ADD_ITEM_Data : CLIENTITEMEX {
};

struct TRADE_TRADE_ITEM_SLAVE_INFO_Data : ITEM_SEND_TOKEN_INFO_Data {
};

struct TRADE_TRADE_REMOVE_ITEM_Data {
	DWORD itemIndex;
};

struct TRADE_TRADE_OVERLOAD_Data {
	DWORD uin;
	BYTE  bType;	// 0 item 1 slave
};

struct TRADE_TRADE_NAME_Data {
	char name[1];
};

struct TRADE_TRADE_ITEM_EXTEND_INFO_Data : ITEM_SEND_EXTEND_INFO_Data {
};

struct TRADE_TRADE_ITEM_RIDE_INFO_Data : ITEM_SEND_RIDE_INFO_Data {
};

struct TRADE_TRADE_LOCK_Data {
	DWORD	money;
	DWORD	point;
	DWORD	fishMoney;
	DWORD	itemIndexs[TRADE_ITEMS_MAX];
	DWORD	slaveUins[TRADE_SALVES_MAX];	
};

struct TRADE_TRADE_LOCK_RESULT_Data {
	BYTE bResult;
	BYTE bErrIndex;
};



struct TRADE_TRADE_OTHER_Data {
	DWORD	money;
	DWORD	point;
	DWORD	fishMoney;
	BYTE	itemCount;
	BYTE	itemExtendCount;
	BYTE	slaveCount;
	BYTE	itemExpExtendCount;
	BYTE	itemGemsInfoSize;
	BYTE    itemJudgeCount;
	BYTE	itemTrumpCount;
	BYTE	itemCarveCount;
	BYTE	itemWingCount;
	BYTE	itemHorseEqCount;
	BYTE	itemEnchaesCount;
	BYTE	data[1];		//具体数据CLIENTITEM,ITEM_SEND_EXTEND_INFO_Data,SlaveDataEx
};


#pragma pack (pop)

#endif