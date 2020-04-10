#ifndef _PRISON_PACKET_H_
#define _PRISON_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum
{
	PRISON_BOODLE_OPEN,
	PRISON_BAIL_OPEN,
	PRISON_GAOLBIRD_LIST,
	PRISON_GAOLBIRD_DATA,
	PRISON_BOODLE_MONEY,
	PRISON_BAIL_NAME,
	PRISON_BAIL_INFO,
	PRISON_BAIL,
};

struct PRISON_PRISON_BOODLE_LIST_Data {
	WORD count;
};

struct PRISON_PRISON_GAOLBIRD_DATA_Data {
	DWORD uin;
	WORD pkCount;
	char name[1];
};

struct PRISON_PRISON_BOODLE_MONEY_Data {
	DWORD money;
};

struct PRISON_PRISON_BAIL_NAME_Data{
	char name[1];
};

struct PRISON_PRISON_BAIL_INFO_Data{
	DWORD money;
	char name[1];
};

struct PRISON_PRISON_BAIL_Data{
};
#pragma pack (pop)
#endif