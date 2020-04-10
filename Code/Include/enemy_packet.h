#ifndef _ENEMY_PACKET_H_
#define _ENEMY_PACKET_H_

#include <windows.h>
#include "groupcmd.h"

#pragma pack (push)
#pragma pack (1)
enum {
	SYNCENEMY,

	GET_ENEMYINFO,
	ENEMYINFO,
	
	ENEMYMOVE,

	ENEMY_STATUS,
	ENEMY_TARGET,
	ENEMY_HP,
	ENEMY_RELOAD,
	
	SET_ENEMY_ATTACK,

	ENEMY_SAY,
	ENEMY_DOMEPER,
	OWNERUIN,
	ENEMYCHANGESPEED,
	ENEMY_MP,
	ENEMY_PARAM,
	OPEN_CLIENT_WINDOW,
	OPEN_CLIENT_WINDOW_BACK,
	GET_ENEMYINFO_FAIL,
	ENEMYMOVEEX,
	MOBSTATUSEX,
	ENEMY_HPEX,
};

enum {
	ENEMY_STATUS_STAND,
	ENEMY_STATUS_WALK,
	ENEMY_STATUS_ATTACK,
	ENEMY_STATUS_ATTACK2,
	ENEMY_STATUS_HURT,
	ENEMY_STATUS_DEATH,
	ENEMY_STATUS_OUT,
	ENEMY_STATUS_RUN,
	ENEMY_STATUS_CAST=14,	
	ENEMY_STATUS_SKILL=8,
	ENEMY_STATUS_MAX,
};
enum//妖怪类别reflect
{
	MON_TYPE_NORMAL,				//怪物
	MON_TYPE_NOATK_FIRST,			//不主动攻击
	MON_TYPE_SUBBOSS,				//小boss
	MON_TYPE_BOSS,					//boss
	MON_TYPE_ATK_RED,				//移动攻击红名:王城守卫
	MON_TYPE_ATK_WHITE,				//移动攻击白名:山寨守卫
	MON_TYPE_MINE,					//矿材
	MON_TYPE_HERB,					//药材	
	MON_TYPE_WOOD,					//木材
	MON_TYPE_BOX,					//宝箱
	MON_TYPE_SLAVE,					//随从
	MON_TYPE_TRIALHORSE,			//陪练
	MON_TYPE_UN_ATTACK_BY_ENEMY,	//不被怪物攻击
	MON_TYPE_COLLECT,				//宝箱2
	MON_TYPE_BOSS_DIS,				//boss(死亡后立即消失)
	MON_TYPE_ADMINISTRATIVE,		//行政类怪(不可被选中,不可被攻击)
	MON_TYPE_QUEST,					//任务怪(接某项任务后可点击用吟唱杀死)
	MON_TYPE_CASTINGKILL,			//吟唱杀死怪(任意情况下均可被敌对势力吟唱杀死)
	MON_TYPE_STONE,					//龙石怪(配合各种战役中的石头怪物)
	MON_TYPE_SCRIPT,				//执行脚本怪(双击执行脚本)
	MON_TYPE_SILK,					  //丝绸
	MON_TYPE_NOSELECT,
	MON_TYPE_ATK_MAX		
};
enum
{
	mon_move,
	mon_escape_before_death,
	mon_escape_on_hurt,
	mon_stick,
	mon_shoutforhelp
};

enum 
{
	TARGET_OBJ_NULL,
	TARGET_OBJ_PLAYER,
	TARGET_OBJ_ENEMY,
	TARGET_OBJ_SLAVE,
	TARGET_OBJ_CHUNK,
	TARGET_OBJ_POINT,
	TARGET_OBJ_OBJ,
};

enum
{
	MONEY_ID=1,//60180,
	MONEY_Silver450,
	MONEY_Silver300,
	MONEY_Silver200,
	MONEY_Silver100,
	MONEY_Gold1000,
	MONEY_Gold800,
	MONEY_Gold650,
	MONEY_Gold500
};

enum//攻击类型attackType
{
	MON_ATK_TYPE_NORMAL,
	MON_ATK_TYPE_ARROW,
	MON_ATK_TYPE_LEVIN,
	MON_ATK_TYPE_ICE,
};

enum//目前用于训导时区分//blook
{
	MON_LOOK_UNHUMAN,
	MON_LOOK_HUMAN,
	MON_LOOK_RIDE,
};

//	血脉
enum EBLOOD_PULSE_TYPE
{
	BLOOD_PULSE_ATKW,		//	白虎(物攻)
	BLOOD_PULSE_ATKN,		//	朱雀(法攻)
	BLOOD_PULSE_DEFW,		//	玄武(物防)
	BLOOD_PULSE_DEFN,		//	麒麟(法防)
	BLOOD_PULSE_LIFE,		//	青龙(生命)
};

//Boss刷新
enum BOSSFESH
{
	GET_ALL_BOSS,
	SET_BOSS_INFO,
};

#define BLOOD_PULSE_MAX (BLOOD_PULSE_LIFE + 1)

struct ENEMY_ENEMY_STATUS_Data {
	DWORD index;
	BYTE status;
	BYTE direction;
	WORD x, y;
};

typedef struct ENEMY_ENEMY_TARGET_Data {
	DWORD index;
	DWORD tagerUin;
	BYTE  tagerType;
}ENEMY_ENEMYTARGET_Data;

struct ENEMY_OWNERUIN_Data {
	DWORD index;
	DWORD owneruin;
};

struct ENEMY_SYNCENEMY_Data {
	DWORD	index;
	WORD	x, y;
	BYTE	direction;
	WORD	type;
	BYTE	status;
	BYTE	speed;
	//WORD	feature;//状态
	BYTE	reflect;
	BYTE	bForce;
	BYTE	byRS;
	int	delNoAttack;
	BYTE	treasureType;
	char	szPrename[20];
};

struct ENEMY_OPEN_CLIENT_WINDOW_Data{
	 int mapIndex;
	 int		x;
	 int		y;
};

struct ENEMY_OPEN_CLIENT_WINDOW_BACK_Data{
	 int mapIndex;
	 int		x;
	 int		y;
};


struct ENEMY_GET_ENEMYINFO_Data {
	DWORD	index;
};

struct ENEMY_ENEMYINFO_Data {
	DWORD	index;
	DWORD	hp,mp,maxhp,maxmp;
	BYTE	level;
	int		baseid;
	DWORD	owneruin;
	char	szPreName[1];
};

struct ENEMY_ENEMYMOVE_Data {
	DWORD	index;
	//BYTE	status;
	//WORD	sx, sy;
	WORD	tx, ty;
	//int		frameindex;
};

struct ENEMY_MOBSTATUS_Data {
	DWORD	index;
	BYTE	direction;
	BYTE	status;
	WORD	tx, ty;
};


struct ENEMY_ENEMYMOVEEX_Data {
	BYTE	bEnemyCnt;
	BYTE	bSlaveCnt;
	ENEMY_ENEMYMOVE_Data szData[1];
};

struct ENEMY_MOBSTATUSEX_Data {
	BYTE	bEnemyCnt;
	BYTE	bSlaveCnt;
	ENEMY_MOBSTATUS_Data szData[1];
};

struct ENEMY_ENEMY_HP_Data {
	DWORD	index;
	int		hp;
};

struct ENEMY_ENEMY_HPEX_Data{
	BYTE	bEnemyCnt;
	ENEMY_ENEMY_HP_Data	szData[1];
};

struct ENEMY_ENEMY_DOMEPER_Data{
	DWORD	index;
	BYTE	domeper;
};

struct ENEMY_ENEMY_MP_Data {
	DWORD	index;
	int		mp;
};
struct ENEMY_ENEMY_RELOAD_Data {
	DWORD	index;
};

struct ENEMY_SET_ENEMY_ATTACK_Data {
	DWORD	enemyIndex;
	DWORD	dwTargetID;
	BYTE	bType;
	WORD	wStartEffect;
	WORD	wFlyEffect;
};

struct ENEMY_ENEMY_PARAM_Data {
	DWORD	index;
	DWORD	hp,mp,maxhp,maxmp;
};

struct BossInfoData
{
	int nID;
	int byMap;
	DWORD dwFreshTime;
};


struct BOSS_INFO_GET_ALL_BOSS_Data
{
	BYTE byCount;
	BossInfoData NData[1];
};

struct BOSS_INFO_SET_BOSS_INFO_Data 
{
	BossInfoData Data;
};

//外观
#define	LOOK_SPEED_HI		(1<<0) 
#define	LOOK_SPEED_LOW		(1<<1) 
#define	LOOK_HP_ADD			(1<<2) 
#define	LOOK_HP_SUB			(1<<3) 
#define LOOK_NO_SKILL_W		(1<<4)
#define LOOK_NO_SKILL_N		(1<<5)
#define LOOK_NO_SKILL_WN	(1<<6)
#define LOOK_ATK_UP_W		(1<<7)
#define LOOK_ATK_UP_N		(1<<8)
#define LOOK_DEF_UP_W		(1<<9)
#define LOOK_DEF_UP_N		(1<<10)
#define LOOK_ATK_DOWN_W		(1<<11)
#define LOOK_ATK_DOWN_N		(1<<12)
#define LOOK_DEF_DOWN_W		(1<<13)
#define LOOK_DEF_DOWN_N		(1<<14)
#define LOOK_FAINT			(1<<15)

struct ENEMY_ENEMY_SAY_Data
{
	DWORD	index;
	char	msg[1];
};

struct ENEMY_ENEMYCHANGESPEED_Data
{
	DWORD index;
	BYTE  AtkSpeed;
	BYTE  MoveSpeed;
};

#pragma pack (pop)

#endif