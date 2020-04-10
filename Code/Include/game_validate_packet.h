#ifndef _GAME_VALIDATE_PACKET_H
#define _GAME_VALIDATE_PACKET_H

#include "groupcmd.h"

#pragma pack (push)
#pragma pack (1)

#define TIMESTAMP_TICKCNT	5000
#define TIMESTAMP_COUNT		6

enum
{
	GAME_VALIDATE_TYPE_TIMESTAMP,
	GAME_VALIDATE_TYPE_ERROR,
	GAME_VALIDATE_TYPE_MAX,
};

struct GAME_VALIDATE_GAME_VALIDATE_TYPE_TIMESTAMP_Data
{
	DWORD	dwTickCnt;
};

#pragma pack (pop)

#endif