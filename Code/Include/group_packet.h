#ifndef _GROUP_PACKET_H_
#define _GROUP_PACKET_H_

#include "player_packet.h"
#define MAX_GROUP_MEMBER		6
#define MAX_BIG_GROUP_MEM 4

#pragma pack (push)
#pragma pack (1)

enum {
	GROUP_MEMBER_HP,
	GROUP_MEMBER_MP,
	GROUP_MEMBER_NAME,
	GROUP_MEMBER_LEVEL,
	GROUP_MEMBER_ADD,
	GROUP_MEMBER_DELETE,
	GROUP_MEMBER_HEADER,
	GROUP_MEMBER_KICK,
	GROUP_MEMBER_SKIN,
	GROUP_MEMBER_POSITION,
	GROUP_MEMBER_STATEINFO,
	MEMBER_ADDSTATE,
	MEMBER_REMOVESTATE,
	MEMBER_ADDSLAVE,
	MEMBER_REMOVESLAVE,
	GROUP_ZHENFA_START,

	SET_GROUP_ASSIGN_COLOR,
	SET_GROUP_ASSIGN_TYPE,
	SET_GROUP_ASSIGN_TYPE_ADVANCE,
	SHOW_ERROR_CODE_ABOUT_GROUP_ASSIGN,
	SHOW_GROUP_ASSIGN_COLOR,
	SHOW_GROUP_ASSIGN_TYPE,
	SHOW_GROUP_ASSIGN_TYPE_ADVANCE,
	REQUEST_ASSIGN_ITEMS_BY_MASTER,
	REQUEST_ASSIGN_ITEMS_BY_MEMBER,
	ASSIGN_ITEMS_BY_MASTER,
	ASSIGN_ITEMS_BY_MEMBER,
	SHOW_GROUP_MEMBER_ASSIGN_VALUE,
	SHOW_GROUP_ASSIGN_RESULT,
	SHOW_ASSIGN_ITEMS_BY_MASTER,
};

enum
{
	SLAVE_MEMBER_HP,
	SLAVE_MEMBER_NAME,
	SLAVE_MEMBER_LEVEL,
	SLAVE_MEMBER_ADD,
	SLAVE_MEMBER_DELETE,
	SLAVE_MEMBER_HEADER,
	SLAVE_MEMBER_KICK,
	SLAVE_MEMBER_SKIN,
	SLAVE_MEMBER_POSITION,
};

enum GROUP_ASSIGN_TYPE
{
	GROUP_ASSIGN_TYPE_NONE,								//	无分配方案
	GROUP_ASSIGN_TYPE_BY_TURNS,							//	轮循分配
};

enum GROUP_ASSIGN_TYPE_ADVANCE
{
	GROUP_ASSIGN_TYPE_ADVANCE_BY_GROUP_MEMBER,			//	队员分配
	GROUP_ASSIGN_TYPE_ADVANCE_BY_GROUP_MASTER,			//	队长分配
};

enum ERROR_CODE_ABOUT_GROUP_ASSIGN
{
	ERROR_CODE_ABOUT_GROUP_ASSIGN_NOT_MASTER,
	ERROR_CODE_ABOUT_GROUP_ASSIGN_WRONG_ASSIGN_TYPE,
	ERROR_CODE_ABOUT_GROUP_ASSIGN_NOITEM,
	ERROR_CODE_ABOUT_GROUP_ASSIGN_NOPLAYER,
};

struct GAMEGROUP_GROUP_MEMBER_HP_Data {
	DWORD uin;
	int  hp, maxHp;
	DWORD slaveUin;
	int SlaveHp,SlaveMaxHp;
};

struct GAMEGROUP_GROUP_MEMBER_MP_Data {
	DWORD uin;
	WORD  mp,maxMp;
	DWORD slaveUin;
	WORD SlaveMp,SlaveMaxMp;
};
struct GAMEGROUP_GROUP_MEMBER_NAME_Data {
	DWORD uin;
	char name[1];
};

struct GAMEGROUP_GROUP_MEMBER_LEVEL_Data {
	DWORD uin;
	BYTE level;
};

struct GAMEGROUP_GROUP_MEMBER_ADD_Data {
	DWORD	uin;
	BYTE	sex;				//add by hjt
	BYTE	job;
	BYTE	level;
	int		hp, maxHp;
	int		mp,maxMp;
	BYTE	skin[SKIN_MAX];
	char	name[1];
};

struct GAMEGROUP_GROUP_MEMBER_DELETE_Data {
	DWORD uin;
};

struct GAMEGROUP_GROUP_MEMBER_HEADER_Data {
	DWORD uin;
};

struct GAMEGROUP_GROUP_MEMBER_KICK_Data {
	DWORD uin;
};

struct GAMEGROUP_GROUP_MEMBER_SKIN_Data {
	DWORD uin;
	BYTE skin[SKIN_MAX];
	BYTE sex;
};

struct GAMEGROUP_GROUP_MEMBER_POSITION_Data {
	DWORD uin;
	int mapIndex;
	WORD zoneIndex;
	WORD x, y;
};

struct GAMEGROUP_GROUP_MEMBER_STATEINFO_Data 
{
	DWORD	uin;
	DWORD	dwStateID;
	DWORD	dwTime;
	BYTE	byLvl;
};

struct GAMEGROUP_MEMBER_ADDSTATE_Data
{
	DWORD	uin;
	DWORD	dwStateID;
	DWORD	dwTime;
	BYTE	byLvl;
};

struct GAMEGROUP_MEMBER_REMOVESTATE_Data
{
	DWORD	uin;
	DWORD	dwStateID;
};

struct GAMEGROUP_MEMBER_ADDSLAVE_Data
{
	DWORD	uin;
	DWORD	dwSlaveID;
	int		nBaseID;
};

struct GAMEGROUP_MEMBER_REMOVESLAVE_Data
{
	DWORD	uin;
	DWORD	dwSlaveID;
};

struct GAMEGROUP_GROUP_ZHENFA_START_Data
{
	BYTE bType;
	bool isStart;
};

//////////////////////////////////////////////////////////////////////////

typedef struct GAMEGROUP_SET_GROUP_ASSIGN_TYPE_Data
{
	BYTE	bAssignValue;
}GAMEGROUP_SET_GROUP_ASSIGN_TYPE_ADVANCE_Data,
GAMEGROUP_SET_GROUP_ASSIGN_COLOR_Data;

typedef struct GAMEGROUP_SHOW_ERROR_CODE_ABOUT_GROUP_ASSIGN_Data
{
	BYTE	bResult;
};

typedef struct GAMEGROUP_SHOW_GROUP_ASSIGN_TYPE_Data
{
	BYTE	bAssignValue;
}GAMEGROUP_SHOW_GROUP_ASSIGN_TYPE_ADVANCE_Data,
GAMEGROUP_SHOW_GROUP_ASSIGN_COLOR_Data;

struct GAMEGROUP_REQUEST_ASSIGN_ITEMS_BY_MASTER_Data
{
	int		nID;
	DWORD	dwItemID;
	BYTE	bPlayerCount;
	DWORD	dwUin[1];
};

struct GAMEGROUP_REQUEST_ASSIGN_ITEMS_BY_MEMBER_Data
{
	int		nID;
	DWORD	dwItemID;
};

struct GAMEGROUP_ASSIGN_ITEMS_BY_MASTER_Data
{
	int		nID;
	DWORD	dwUin;
};

struct GAMEGROUP_ASSIGN_ITEMS_BY_MEMBER_Data
{
	int		nID;
	int		nValue;
};

struct GAMEGROUP_SHOW_GROUP_MEMBER_ASSIGN_VALUE_Data
{
	DWORD	dwItemID;
	int		nValue;
	char	szName[20];
};

struct GAMEGROUP_SHOW_GROUP_ASSIGN_RESULT_Data
{
	DWORD	dwItemID;
	char	szName[20];
};

struct GAMEGROUP_SHOW_ASSIGN_ITEMS_BY_MASTER_Data
{
	int		nID;
	DWORD	dwItemID;
};

#pragma pack (pop)
#endif