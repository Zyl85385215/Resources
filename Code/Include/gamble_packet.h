#ifndef _GAMBLE_PACKET_H_
#define _GAMBLE_PACKET_H_

#include "player_packet.h"

#define MAX_GAMBLE_MEMBER		50
#pragma pack (push)
#pragma pack (1)

enum _GAMBLE{
	GAMBLE_START,
	GAMBLE_OPEN,
	GAMBLE_STOP,
	GAMBLE_TEST,
	GAMBLE_TIMEOUT,
	GAMBLE_MONEY_ZHUANG,
	GAMBLE_AMOUNT_XIAN_DA,
	GAMBLE_AMOUNT_XIAN_XIAO,
	GAMBLE_POINT,
	GAMBLE_MONEY_XIAN,
	GAMBLE_MONEY_NOTENOUGH,
	GAMBLE_WIN,
	GAMBLE_LOSE,
	GAMBLE_TIME,
	GAMBLE_ADDPOINT,
	GAMBLE_ADDPOINT_TIMEOUT,
	GAMBLE_NAME,
	GAMBLE_LEAVE,
	GAMBLE_STOP_ADD,
	GAMBLE_START_ADD,
	GAMBLE_OVER,
	GAMBLE_ZHUANG,
};

struct GAMBLE_GAMBLE_STOP_Data {
};

struct GAMBLE_GAMBLE_TEST_Data {
};

struct GAMBLE_GAMBLE_MONEY_ZHUANG_Data {
	DWORD money;
};

struct GAMBLE_GAMBLE_AMOUNT_XIAN_DA_Data {
	DWORD money;
};

struct GAMBLE_GAMBLE_AMOUNT_XIAN_XIAO_Data {
	DWORD money;
};

struct GAMBLE_GAMBLE_POINT_Data {
	int dice1;
	int dice2;
};

struct GAMBLE_GAMBLE_MONEY_XIAN_Data {
	DWORD money;
	BOOLEAN flag;
};

struct GAMBLE_GAMBLE_WIN_Data {
	DWORD money;
};

struct GAMBLE_GAMBLE_LOSE_Data {
	DWORD money;
};

struct GAMBLE_GAMBLE_TIME_Data {
	int nTime;
};

struct GAMBLE_GAMBLE_ADDPOINT_Data {
	DWORD money;
};

struct GAMBLE_GAMBLE_NAME_Data {
	char name[20];
};

struct GAMBLE_GAMBLE_LEAVE_Data {
	char name[20];
};

#pragma pack (pop)
#endif