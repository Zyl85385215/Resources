#ifndef _MONITOR_PACKET_H_
#define _MONITOR_PACKET_H_

#include <windows.h>
#include "groupcmd.h"

#pragma pack (push)
#pragma pack (1)

enum
{ 
    CHECK_GAMESERVER,
	CHECK_GAMESERVER_RESULT,	
	NOTIFY_GAMESERVER_BEGIN,
	NEW_NOTIFY_GAMESERVER,
	GET_GAME_INI,
	GET_GAME_INI_END,
};

typedef struct GAMESERVER_MONITOR_CHECK_GAMESERVER_Data
{
	char     szIP[20];
	WORD     wPort;
	BYTE     btLine;
}GAMESERVER_MONITOR_NOTIFY_GAMESERVER_BEGIN_Data;

enum
{
	PERMIT,
    NOT_PERMIT,		
};

struct GAMESERVER_MONITOR_CHECK_GAMESERVER_RESULT_Data
{
	BYTE btRlt;
};

struct GAMESERVER_MONITOR_NEW_NOTIFY_GAMESERVER_Data
{
	char     szIP[20];
	WORD     wPort;
	BYTE     btLine;
	char	 szDescription[1];
};


struct GAMESERVER_MONITOR_GET_GAME_INI_Data
{
	char buff[256];
	int len;
};

struct GAMESERVER_MONITOR_GET_GAME_INI_END_Data
{
	BYTE btRlt;
};

#pragma pack (pop)

#endif
