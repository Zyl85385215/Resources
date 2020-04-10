#ifndef _NPC_PACKET_H_
#define _NPC_PACKET_H_

#include <windows.h>
#include "groupcmd.h"

#pragma pack (push)
#pragma pack (1)

enum {
	SYNCNPC,
	SEND_SHOW_NPC,
};

struct NPC_SYNCNPC_Data {
	WORD	index;
	WORD	x, y;
	BYTE	direction;
	WORD	type;
	BYTE	funcType;
	char	name[1];
};

struct NPC_SEND_SHOW_NPC_Data
{
	char  szName[40];
	bool  blFg;
};

#pragma pack (pop)

#endif