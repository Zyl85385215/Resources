#ifndef _ITEM_ADDTIME_PACKET_H_
#define _ITEM_ADDTIME_PACKET_H_
#include "item_packet.h"
enum _ITEM_ADDTIME
{
	ADD_TIME_OPEN,
	ADD_TIME_CLOSE,
	ADD_TIME_BEGIN,
};


struct ITEM_ADDTIME_ADD_TIME_BEGIN_Data
{
	DWORD dwIndex;
	int nDays;
	int nItemCnt;
};

#endif