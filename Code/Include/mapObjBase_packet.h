#ifndef _MAPOBJ_BASEINFO_PACKET_H_
#define _MAPOBJ_BASEINFO_PACKET_H_

#include <windows.h>
#include "groupcmd.h"

#pragma pack (push)
#pragma pack (1)

enum SORelationShips_Type
{
	RS_UNKNOWN = 0,			//未知
	RS_ENEMY,			//敌对
	RS_FRIENDLY,			//友好
	RS_NORMAL,			//中立
};

enum {
	RELATIONSHIP,
};

struct MAPOBJ_BASEINFO_RELATIONSHIP_Data {
	DWORD				dwID;
	BYTE				type;
	BYTE				byRS;
};

#pragma pack (pop)

#endif