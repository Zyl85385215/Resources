#ifndef _GUILD_PACKET_H_
#define _GUILD_PACKET_H_

#include "WTYPES.H"
#include "Define.h"

#define DUTY_MAX			12
#define MAX_DUTYNAMELEN		40

enum 
{
	SEND_SYSTEM_MSG,
	SEND_GUILD_MSG,
	SEND_GUILD_INFO,
	SEND_DUTY_INFO,
	DUTY_SETTING_CHG,
	INVITE_CONFIRM,
	MEMBER_ADD,
	MEMBER_REMOVE,
	MEMBER_DUTY_CHG,
	MEMBER_POSITION_CHG,
	MEMBER_LEVEL,
	MEMBER_JOB,
	START_GUILD_CREATE,
	MEMBER_LOGONOFF,
	REQUEST_QUERY_MEMBER,
	REQUEST_ADD_MEMBER,
	REQUEST_REMOVE_MEMBER,
	REQUEST_CHGDUTY_SETTING,
	REQUEST_CHGDUTY_MEMBER,
	REQUEST_CREATE_GUILD,
	REQUEST_DISMISS_GUILD,
	REQUEST_CHG_GUILDMSG,
	REQUEST_CHG_MEMBERNOTE,
	PLAYER_ACCPET_JOIN,
	GET_CONFINFO,
	CONFINFO,
	GUILD_END,
};

enum GUILD_PERMIT 
{
	GUILD_PERMIT_ADD,						//add guild member
	GUILD_PERMIT_REMOVE,				//remove guild member
	GUILD_PERMIT_USEGUILDITEM,	//can open guild bank and take guild items,
	GUILD_PERMIT_CHGNOTIFY,			//can change guild notify
	GUILD_PERMIT_CHGPERMIT			//can change guild Permission setting,highest Permission
};

enum GUILD_DUTY_INDEX
{	
	GUILD_DUTY_ELITE=7,
	GUILD_DUTY_COUNSELLOR,
	GUILD_DUTY_COMMANDER,
	GUILD_DUTY_OFFMASTER,
	GUILD_DUTY_MASTER,
};

enum GUILD_CREATE_RESULT
{
    GUILD_CREATE_SUCCESS,
	GUILD_FIELD_LEVEL,	
};

#pragma pack (push)
#pragma pack (1)

struct GUILD_SEND_SYSTEM_MSG_Data
{
	char msg[1];
};

struct GUILD_GET_CONFINFO_Data
{
	DWORD   dwConfID;
};

struct GUILD_CONFINFO_Data
{
	DWORD	dwConfID;
	char	szConfName[1];
};

struct GUILD_SEND_DUTY_INFO_Data
{
	WORD Permitcode[DUTY_MAX];
	char DutyName[DUTY_MAX][MAX_DUTYNAMELEN];
};

typedef struct GUILD_DUTY_SETTING_CHG_Data
{
	DWORD byuin;
	BYTE index;
	WORD Permitcode;
	char Name[MAX_DUTYNAMELEN];
} GUILD_REQUEST_CHGDUTY_SETTING_Data;

typedef struct GUILD_SEND_GUILD_INFO_Data
{
	char name[MAX_DUTYNAMELEN];
	char guildMsg[1];
} GUILD_SEND_GUILD_MSG_Data;

struct GUILD_INVITE_CONFIRM_Data
{
	DWORD byuin;
	DWORD uin;
	char name[MAX_DUTYNAMELEN];
	char guildname[1];
};
struct GUILD_MEMBER_ADD_Data 
{
	//if true,the member is newly added,the client side will show a message that new player joins the guild
	//if false,the packet is sended by player login game
	BYTE bNewAdd;
	DWORD byuin;
	DWORD uin;
	char Name[MAX_DUTYNAMELEN];
	BYTE level;
	BYTE zoneIndex;
	int mapIndex;
	BYTE bDutyIndex;
	BYTE job;
	BYTE bOnline;
	char lastLogin[20];
	char Des[1];
};

typedef struct GUILD_MEMBER_REMOVE_Data
{
	DWORD byuin;
	DWORD uin;
} GUILD_REQUEST_REMOVE_MEMBER_Data,
	GUILD_REQUEST_QUERY_MEMBER_Data,
	GUILD_PLAYER_ACCPET_JOIN_Data;

struct 	GUILD_REQUEST_ADD_MEMBER_Data
{
	DWORD byuin;
	char name[1];
};
typedef struct GUILD_MEMBER_DUTY_CHG_Data
{
	DWORD byuin;
	DWORD uin;
	//when is GUILD_REQUEST_CHGDUTY_MEMBER_Data packet,dutyindex is 0 or 1,
	BYTE dutyindex;
} GUILD_REQUEST_CHGDUTY_MEMBER_Data;

struct GUILD_MEMBER_POSITION_CHG_Data
{
	DWORD uin;
	int mapIndex;
	BYTE zoneIndex;
};

struct GUILD_MEMBER_LEVEL_Data
{
	DWORD uin;
	BYTE  level;
};

struct GUILD_MEMBER_JOB_Data
{
	DWORD uin;
	BYTE  job;
};

struct GUILD_MEMBER_LOGONOFF_Data
{
	DWORD uin;
	BYTE logon;
};

struct GUILD_REQUEST_CREATE_GUILD_Data
{
	int    count;
	DWORD  uins[6];
	DWORD  byuin;
	char   name[1];
};

struct GUILD_REQUEST_CHG_GUILDMSG_Data
{
    DWORD byuin;
	char name[1];
};

struct	GUILD_REQUEST_CHG_MEMBERNOTE_Data
{
	DWORD byuin;
	DWORD uin;
	char note[1];
};

struct GUILD_REQUEST_DISMISS_GUILD_Data
{
	DWORD byuin;
};

#pragma pack (pop)

#endif 