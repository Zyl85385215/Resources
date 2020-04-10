#ifndef _MAPOBJ_BASEINFO_PACKET_H_
#define _MAPOBJ_BASEINFO_PACKET_H_

#include <windows.h>
#include "groupcmd.h"

#pragma pack (push)
#pragma pack (1)

enum SORelationShips_Type
{
	RS_UNKNOWN = 0,			//δ֪
	RS_ENEMY,			//�ж�
	RS_FRIENDLY,			//�Ѻ�
	RS_NORMAL,			//����
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