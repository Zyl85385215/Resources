#ifndef _AUCTION_PACKET_H_
#define _AUCTION_PACKET_H_

#include "item_packet.h"

#pragma pack (push)
#pragma pack (1)

enum  {	
	JOIN_AUCTION,
	LEAVE_AUCTION,

	SALE_ITEM,
	BUY_ITEM,

	ADD_ITEM,
	REMOVE_ITEM,

	AUCTION_PRICE,
	
	AUCTION_DEPOSIT,

	AUCTION_ITEM,
	AUCTION_INFO,
	AUCTION_INFOEX,
	AUCTION_TIME,
	AUCTION_COUNT,
	AUCTION_ITEM_SLAVE_INFO,
	AUCTION_ITEM_EXTEND_INFO,
	AUCTION_ITEM_RIDE_INFO,
	AUCTION_ITEM_CANNOT_SPLIT
};

struct AUCTION_JOIN_AUCTION_Data {
};

struct AUCTION_AUCTION_COUNT_Data {
	int count;
};

struct AUCTION_LEAVE_AUCTION_Data {
};

struct AUCTION_SALE_ITEM_Data {
	BYTE  index;
	int	  price;	
	int   addPrice;
};

struct AUCTION_BUY_ITEM_Data {
};

struct AUCTION_ADD_ITEM_Data {
	DWORD    uin;
	BYTE     index;
	WORD	itemID;
	int	  price;	
	int   addPrice;
	WORD   Dura;
	WORD   DuraMax;
};

struct AUCTION_REMOVE_ITEM_Data {
	DWORD    uin;
	BYTE     index;
};

struct AUCTION_AUCTION_PRICE_Data {
	int price;
};

struct AUCTION_AUCTION_ITEM_Data {
	DWORD    uin;
	BYTE     index;
	char     name[1];
};

struct AUCTION_AUCTION_INFO_Data {
	BYTE type;
	DWORD uin;
	BYTE  indexID;
	int gold;
	char name[1];
};

struct AUCTION_AUCTION_INFOEX_Data {
	DWORD id;
	char msg[1];
};

struct AUCTION_AUCTION_DEPOSIT_Data {
	int gold;
};

struct AUCTION_AUCTION_TIME_Data {
	BYTE type;
	int time;
};

struct AUCTION_AUCTION_ITEM_SLAVE_INFO_Data : ITEM_SEND_TOKEN_INFO_Data {
	DWORD uin;
};

struct AUCTION_AUCTION_ITEM_EXTEND_INFO_Data : ITEM_SEND_EXTEND_INFO_Data {
	DWORD uin;
};

struct AUCTION_AUCTION_ITEM_RIDE_INFO_Data : ITEM_SEND_RIDE_INFO_Data {
	DWORD uin;
};


#pragma pack (pop)
#endif