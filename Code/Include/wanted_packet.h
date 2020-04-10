#ifndef _WANTED_PACKET_H_
#define _WANTED_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum {
	WANTED_OPEN,
	WANTED_DATA,
	CREATE_WANTED,
	ACCEPT_WANTED,
	ADD_WANTED_GOLD,
	DISABLE_ACCEPT,
	WANTED_MESSAGE,
	WANTED_POS,
};

struct WANTED_WANTED_DATA_Data {
	DWORD wantedUin;
	DWORD gold;
	DWORD timeCount;
	BOOLEAN online;
	char name[1];
};
struct WANTED_WANTED_POS_Data {
	int		mapindex;
	int		zoneindex;
	POINT	pos;
};
struct WANTED_CREATE_WANTED_Data {
	DWORD gold;
	char name[1];
};

struct WANTED_ACCEPT_WANTED_Data {
	DWORD wantedUin;
};

struct WANTED_ADD_WANTED_GOLD_Data {
	DWORD wantedUin;
	DWORD gold;
};

struct WANTED_WANTED_MESSAGE_Data {
	char msg[1];
};

#pragma pack (pop)
#endif