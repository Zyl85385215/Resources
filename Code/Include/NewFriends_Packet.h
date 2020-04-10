#pragma once

#include <windows.h>

#pragma pack (push)
#pragma pack (1)

enum NEWFRIEND_CMD
{
	S_NEWFRIENDS_INFO,
	SC_NEWFRIENDS_REMOVE,
	S_NREFRIENDS_INVITEDS,
	C_NEWFRIENDS_USE,
};

struct NEW_FRIENDS_S_NEWFRIENDS_INFO_Data {
	WORD	wID;
	BYTE	bNew;
	DWORD	dwTmCool;	//µ•Œª: √Î
};

struct NEW_FRIENDS_SC_NEWFRIENDS_REMOVE_Data {
	WORD	wID;
};
struct NEW_FRIENDS_S_NREFRIENDS_INVITEDS_Data {
	WORD	wCnt;
	WORD	wID[1];
};

struct NEW_FRIENDS_C_NEWFRIENDS_USE_Data {
	WORD	wFID;
};

#pragma pack (pop)