#ifndef _SKILL_PACKET_H_
#define _SKILL_PACKET_H_

#include "groupcmd.h"

#pragma pack (push)
#pragma pack (1)

#define  MAX_SHOW_SKILL2			25

//#define NEW_SKILL

static int WashSkillLevel[] = {
		41,
		61,
		81,
		101
};

static int WashSkillCost[] = {
		45285,
		45286,
		45287,
		45288,
		45251,
		45252,
		45253,
		45254
};

enum {
		SEND_SKILLS,
		ADD_SKILL,	
		REMOVE_SKILL,
		WASH_SKILL,
		HURT_TARGET,
		SEND_STARTATK,
		SKILL_EFFECT_INFO,
		SKILL_CHECK_RLT,
		SKILLCODE_CHECK_RLT,
		SEND_UPGRADE,
		SEND_COMPELETEATK,
		SEND_SKILLTALENT,
		ACTIVE_SKILL,
		NOACTIVE_SKILL,
};
enum SKILL_TYPE{
	SKILLTYPE_PHYSICAL_ATTACK = 1,
	SKILLTYPE_MAGIC_ATTACK,
	SKILLTYPE_PHYSICAL_ASSISTANT,
	SKILLTYPE_MAGIC_ASSISTANT,
	SKILLTYPE_MAKEITEM,
	SKILLTYPE_REPAIRITEM,
	SKILLTYPE_RESURRECT,
};

enum E_SKILLTYPE
{
	S_PHYSICAL,
	S_PHYSICAL_ASSIST,
	S_MAGIC,
	S_MAGIC_ASSIST,
	S_RESURRECT,
	S_TELEPORT,
	S_TAME,
	S_STEAL,
	S_COMPOUND,
	S_REPAIRE,
	S_SUMMON,
	S_CONJURE,
	S_FURY,				//	该类型已经舍弃，有新的类型时，可以使用该位
	S_LIFE,
	S_STRENGTHEN,
};

enum {
	USE_SKILL_RLT_SUCCESS,
	USE_SKILL_RLT_DISTANCE,
	USE_SKILL_RLT_BLOCK,
	USE_SKILL_RLT_UNKNOWN_EFFECT,
	USE_SKILL_RLT_FORBID,
	USE_SKILL_RLT_MODE,
	USE_SKILL_RLT_EQUIP,
	USE_SKILL_RLT_SPMP,
	USE_SKILL_RLT_DELAY
};

enum
{
	SKILL_OBJ_TYPE_ENEMY,
	SKILL_OBJ_TYPE_SLAVE,
	SKILL_OBJ_TYPE_PLAYER,
	SKILL_OBJ_TYPE_CHUNK,
	SKILL_OBJ_TYPE_POINT,

};
struct SKILLITEM {
		DWORD	skillIndex;
		WORD	skillID;
//change by hjt
		WORD	nStackCnt;
		DWORD	nNextUseTime;
//end change
//change by dxx
		DWORD	wExp;
		DWORD	dwItemIndex;
//end change
//change by tcl
		int		nTalentPoint;
		bool	bIsActive;
//end change
};

struct SKILL_SEND_SKILLS_Data {
		BYTE	count;
		SKILLITEM item[1];
};

typedef struct SKILL_ADD_SKILL_Data {
		SKILLITEM item;
}SKILL_REMOVE_SKILL_Data;


struct SKILL_SKILL_EFFECT_INFO_Data {
		DWORD	uin;
		DWORD	objID;
		BYTE	type;
		WORD	skillID;
		BYTE	rlt;
};

struct SKILL_SKILL_CHECK_RLT_Data
{
		DWORD	uin;
		WORD	skillID;
		//the packet is a debug msg packet,client will only display the text,doesn't change anything
		BYTE	bDebug;
		char  rlt[1];
};

struct SKILL_SKILLCODE_CHECK_RLT_Data
{
	DWORD	uin;
	WORD	skillID;
	//the packet is a debug msg packet,client will only display the text,doesn't change anything
	BYTE	bDebug;
	BYTE	byMsgIndex;
};

struct SKILL_HURT_TARGET_Data
{
	DWORD dwID;
	BYTE	bType;
	DWORD dwSKillID;
};
struct SKILL_SEND_STARTATK_Data
{
	DWORD dwATKID;
	DWORD dwTargetID;
	BYTE	bType;
	DWORD dwSKillID;
	BYTE	dir;
	DWORD uin;
	DWORD UinType;
};

struct SKILL_SEND_COMPELETEATK_Data
{
	DWORD dwSKillID;
};
struct SKILL_SEND_UPGRADE_Data
{
	WORD  wSkillID;             //对应技能ID
	DWORD wExp;                 //当前技能熟练度
	bool  bUpgrade;             //是否升级
};

struct SKILL_SEND_SKILLTALENT_Data
{
	WORD wSkillID[MAX_SHOW_SKILL2];
	int nTalent[MAX_SHOW_SKILL2];
	int	nCount;
};

struct SKILL_ACTIVE_SKILL_Data
{
	 WORD  wSkillID;             //对应技能ID
};

struct SKILL_NOACTIVE_SKILL_Data
{
	 WORD  wSkillID;             //对应技能ID
};

#pragma pack (pop)
#endif