#pragma once

#pragma pack (push)
#pragma pack (1)

enum {
	SVR_DEAD_LOCK_CMD	= 59,
	SET_GM_PRIOR,
	GONG_GAO,

	PLAYER_AREA_YEZHIZHEN,
	GET_TRADE_LOG, //send user's trade log to account server
	TRADE_LOG,
	ADD_SCRIPT,
};


struct GAME_BACKDOOR_SVR_DEAD_LOCK_CMD_Data
{
	bool bDeadLock;
};

struct GAME_BACKDOOR_SET_GM_PRIOR_Data
{
	DWORD uin;
	BYTE  byGM;
};


struct GAME_BACKDOOR_GONG_GAO_Data 
{
	BOOL	m_GM;
	BYTE	m_Times;
	char	msg[1];
};

struct GAME_BACKDOOR_PLAYER_AREA_YEZHIZHEN_Data 
{
	DWORD	uin;
	int		x;
	int		y;
};

struct GAME_BACKDOOR_GET_TRADE_LOG_Data
{
	DWORD uin;
	int logId;
};

struct GAME_BACKDOOR_TRADE_LOG_Data
{
	int uin;
	short action;
	char data[50];
	int gold;
	int silver;
	char dateTime[20];
	int remaingold;
	int remainsilver;
};


struct GAME_BACKDOOR_ADD_SCRIPT_Data
{
	DWORD	uin;
	int		len;
	BYTE	byCode[1];
};

#pragma pack (pop)
