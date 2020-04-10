#pragma once

#include <Windows.h>
#include "player_packet.h"

#pragma pack (push)
#pragma pack (1)

enum EMAPCOPYSPORTLOG_TIME_TYPE
{
	MAPCOPYSPORTLOG_TIME_TYPE_DAY,
	MAPCOPYSPORTLOG_TIME_TYPE_WEEK,
	MAPCOPYSPORTLOG_TIME_TYPE_MONTH,
	MAPCOPYSPORTLOG_TIME_TYPE_MAX,
};

enum EMAP_COPY_SPORT
{
	MAP_COPY_SPORT_REQUEST_GETINFO,
	MAP_COPY_SPORT_RESPONSE_GETINFO,
	MAP_COPY_SPORT_UPDATE,
};

struct MAP_COPY_SPORT_MAP_COPY_SPORT_REQUEST_GETINFO_Data
{
	int mapCopyId;
};

struct mapCopySportPlayerInfoInPacket
{
	DWORD uin;
	char name[MAX_NAMELEN];
};

struct mapCopySportLogInfoInPacket
{
	DWORD completeTime;
	int playerCount;
	mapCopySportPlayerInfoInPacket playerInfo[1];
};

struct MAP_COPY_SPORT_MAP_COPY_SPORT_RESPONSE_GETINFO_Data
{
	int mapCopyId;
	mapCopySportLogInfoInPacket info[1];
};

struct MAP_COPY_SPORT_MAP_COPY_SPORT_UPDATE_Data
{
	int mapCopyId;
	DWORD completeTime;
	int playerCount;
	DWORD playerUin[1];
};

#pragma pack (pop)