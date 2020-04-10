#ifndef _PLAYER_PACKET_H_
#define _PLAYER_PACKET_H_

#include <windows.h>
#include "groupcmd.h"
#include "gm_packet.h"
#include "country_packet.h"


#define MAX_PLAYER_COPY_SKILL 10

#pragma pack (push)
#pragma pack (1)
#define PLAYER_ACTIONFRAME_DELAY		25
#define COOLDOWNFLAG_LEN			256		//冷却位

#ifdef _DEBUG
#define	RESET_ATTRIB_TIME		10*25
#else
#define	RESET_ATTRIB_TIME		300*25
#endif


#define SKILL_PROC_BEGIN        1                              
#define SKILL_PROC_END          0
#define SKILL_PROC_RUNING       -1  
#define PLAYER_NAME_LEN			16

#define RELATION_TITLE_MIN		51000

#define  SKILL_EXERCISE_CNT		6
#define  SKILL_ERCS_ID			8001
#define  MAX_SHOW_SKILL			25

#define BUY_BYSOURCE_ITEMID 39109//用积分购买的物品ID
// 客户端初步能确定的目标类型
enum
{                      
	SklObjType_Self,						//0
	SklObjType_OtherPlayer,					//1
	SklObjType_MOB,							//2
	SklObjType_Building,					//3
	SklObjType_EmptyPoint					//4
};
enum
{
	force_null,
	force_coutry1,
	force_coutry2,
	froce_coutry3,
	force_white,
	force_black,
	force_kingdom,
	force_war1,         //fix by dxx 战场队伍
	force_war2,
	force_war3,
	force_war4,
	force_war5,         //前5种为玩家势力
	force_war6,         //6为标准怪物势力
	force_coutry1_other,
	force_coutry2_other,
	force_coutry3_other,
};

enum PlayerGuessRlt
{
	PGR_FAIL,
	PGR_NO_COUNT,
	PGR_BIG,
	PGR_SMALL,
	PGR_NO_START,
	PGR_OK,
};

enum 
{
	SYNCPLAYER,
	OUTAREA,
	GET_UININFO,
	UININFO,
	SET_MOVE_TO,
	MOVE_TO,
	GET_PLAYER_POS,
	SEND_CMD_REPLY,			//reply client the sever has received packet successfully
	SEND_PLAYER_POS,
	CLIENT_STOP_MOVE,
	SET_FIGHT_MODE,
	PLAYER_STATUS,
	SET_STATUS,
	CLICK_NPC,
	PLAYER_ATTACK_PLAYER,
	PLAYER_ATTACK_ENEMY,
	SET_PLAYER_ATTACK,
	ADD_ATTRIB,
	PLAYER_HP,
	PLAYER_MP,
	SET_RIDE_ON,//骑马
	SET_RIDE_OFF,
	SKILL_ACTON_MYSELF,
	SKILL_ATTACK_INFO,
	SKILL_USE,
	BREAK_MOVE,
	UNMARRY,
	PLAYER_ATTACK_CHUNK,//攻击房子
	SKILL_ATTACK_CHUNK,
	JUMP,
	RESETATTRIB_TIME,
	SUB_ATTRIB,                                               // @zjq,2005-08-18
	PLAYER_ATTACK_SLAVE,
	SKILL_ATTACK_SLAVE,	
	SET_CLUB,
	COOLDOWN_TIME,
	CASTING,					//add by hjt
	CASTING_INTERRUPT,	//add by hjt
	CASTING_COMPLETE,		//add by hjt
	SKILL_RECHARGE,			//add by hjt
	UPDATEGUILDINFO,
	ADD_ATTRIB_OK,			//add by ynot at 2007-3-27
	ADD_ATTRIB_CANCEL,		//add by ynot at 2007-3-27
	PLAYER_POS, //add by mdz only for debug
	COUNTRY,    // add by dxx
	CHANGE_NAME,
	CHANGE_NAME_OK,
	CHANGE_NAME_FORBID,
	CHANGE_NAME_PASS,
	SHOPINFO,
	CHANGEJOB,
	CHANGENAMECOLOR,
	CHANGESKIN,
    CHANGESPEED,
	GUILDNAMEPOST,
	BUYMALL_ITEM,
	BUYMALL_RESULT,
	HANDOFGOD,
	CHANGE_TO_MOB,
	NOTACTIVE,
	CHECKMALL_VERSION,
	CHECKMALL_VERSION_RESULT,
	SENDMALL_ITEM,
	SENDMALL_OVER,
	TRUEACTIVE,
	PK_CANNOT_ATTACK,
	GET_ACTIVITY_INFO,
	SEND_ACTIVITY_INFO,
	CHANGE_PK_COUNT,
	ALL_COOLDOWN_TIME,
	COLLECT,
	SEND_SET_ACTIVE_TITLE,
	REQUEST_SKILL_LEVELUP,
	REQUEST_GM_LIST,
	GM_LIST,
	SET_GM,
	REQUEST_SKILL_LEARN,
	OPEN_MAKE,
	AUTO_SKILL_SELF,
	ASK_RELATIONSHIP,
	PLAYER_FIGHT_MODE,
	ADD_ATTRIB_AUTO,				// ADD BY YJ FOR 自动加点
	PLAYER_CASTING_KILL,			// 吟唱杀死怪
	YUANFEN_LIST,
	MAP_PLAYERS_LIST,
	COUNTRY_INFO,					// 国家信息
	SEND_HOME_ICON,					// 发送帮会图标
	ADD_ACTIVITY_RANK,				// 添加活动排行
	SEND_ACTIVITY_RANK,				// 发送活动排行
	TELLGM_AREA,

	REQUEST_ERCS_LVLUP,

	GET_ON_TRANSPORT,				// 上交通工具(多人坐骑)
	GET_OFF_TRANSPORT,				// 下交通工具
	SEND_DRIVER_INFO,				// 发送司机的交通工具信息
	SEND_PASSENGER_INFO,			// 发送乘客的交通工具信息
	GET_TRANSPORT_INFO,

	PLAYER_COUNTRY_JOB,				//	角色国家地位
	SET_FASHIONVIEW,
	SET_WINGVIEW,
	CREATE_OATH,
	RETITLE_OATH,
	DELETE_OATH,
	CHANGE_OATH,
	SEND_OATH,
	SYNC_PLAYERSEX,
	PLAYER_USE_FURY_STAR,			// 玩家使用怒气
	CLICK_ENEMY,
	SKILL_ADDTALENT,
	SKILL_UPGRADE,
	SEND_WASHSKILL,
	SEXP_TO_BOOKEXP,
	SKILL_ACTIVE,
	SKILL_NOACTIVE,

	REQUEST_DRIVE_TIME,
	SEND_HEIMUYA,
	REQUEST_LVLUP,
	GET_UININFO_FAIL,

	SELL_RMBITEM,
	CLIENT_ADDQUEST_VAL,

	SEND_MARRY_TITLE,

	REQUEST_MARRY_BLESS,
	RESPONSE_MARRY_BLESS,
	END_MARRY_BLESS,
	REQUEST_PARADE_START,
	PARADE_MENU_OPEN,
	SEND_SAHUA,									//结婚时撒花
	REQUEST_END_PARADE,
	ONPARADE_MENU_OPEN,
	ONPARADE_MENU_CLOSE,
	USE_MARRY_TITLE,
	CLIENT_REQUEST_YUANFEN_LIST,
	OPEN_SHUT_UP_MENU,
	SHUT_UP_SOMEONE,
	SEND_BIG_GROUP_ID,

	REQUEST_SAUCTION,
	C_REQUEST_LEAVE_FISHPOOL,
	OPENDAOJU,
	CHECKMALL_VERSION_DAOJU,
	CHECKMALL_VERSION_DAOJU_RESULT,
	SENDMALLDAOJU_OVER,
	REQUEST_CHANGE_BOSS,
	OPEN_BOSS_MENU,
	C_REQUEST_HSTATE,
	SEND_BOSS_KILL_COUNT,

	REQUEST_BOX_OPEN,
	REQUEST_BOX_POINT,

	SEND_BOXLUCK_INFO,
	SEND_BOXITEM_INFO,
	SEND_OPEN_MALL_WULIN,

	OPEN_CLIENT_FORM,
	REQUEST_UPDATE_COPY_INFO,
	SEND_OPEN_TIANXIA_RANK,
	REQUEST_CHG_36101,
	C_OPERTION_BOMB,
	SEND_BOMB_INFO,
	SEND_TMP_BOMBINFO,
	SEND_BOMB_EXP,
	REQUEST_GIVE_LOVE,
	REQUEST_ADD_PLAYER_COPY_CHALLENGE_COUNT,
	REQUEST_RESET_PLAYER_COPY_COOL_DOWN,
	SEND_YITIAN_INFO,
	GET_YITIAN_INFO,
	C_JOIN_YITIAN_BATTLE,
	OPENYITIAN,
	OPENYITIAN2,
	WARMACHINE_INFO,
	LEAVE_WARMACHINE,
	INVITE_WARMACHINE,
	ARENA_REQUEST_OPEN,
	SHOPINFOEX,
	SWITCH_SLAVE_CTRL,
	REQUEST_UPDATE_SLAVECOPY_INFO,
	REQUEST_ADD_SLAVE_COPY_CHALLENGE_COUNT,
	REQUEST_RESET_SLAVE_COPY_COOL_DOWN,
	
	S_GAMBLE_SLAVE_PK,
	C_GAMBLE_SLAVE_PK,
	S_GAMBLE_SIMPLE_INFO,
	c_REPLACE_ITEM,
	S_OPEN_REPLACE_ITEM_MENU,
	C_EXCHANGE_RIDE,
	REQUEST_INIT_GUESS,
	REQUEST_GUESS_NUM,
	GUESS_NUM_RLT,
	OPEN_GUESS_MENU,
	SEND_YITIAN_INFO2,
	C_JOIN_SHENGHUO_BATTLE,
	GET_SHENGHUO_INFO,
	SP_SEND_WORLDMSG,
	REQUEST_ADD_MARRY_RING_POINT,
	REQUEST_CHECK_MARRY_RING_RANK,
	C_SWORDSPRITE_OPE,
	FAST_SELL,
};

#define player_bodysize_stand	20
#define player_bodysize_ride	70

enum
{
	PLAYER_STATUS_STAND,
	PLAYER_STATUS_WALK,
	PLAYER_STATUS_RUN,
	PLAYER_STATUS_SIT,
	PLAYER_STATUS_ATTACK,
	PLAYER_STATUS_HURT,
	PLAYER_STATUS_DEATH,
	PLAYER_STATUS_USE,
	PLAYER_STATUS_SKILL,
	PLAYER_STATUS_TALK,
	PLAYER_STATUS_TRADE,
	PLAYER_STATUS_BUSY,
	PLAYER_STATUS_RESETATTRIB,
	PLAYER_STATUS_STALLAGE,
	PLAYER_STATUS_PRESTALLAGE,        //add by cf
	PLAYER_STATUS_CAST,
	PLAYER_STATUS_INTERRUPT,
	PLAYER_STATUS_OUT,                //add by dxx
	PLAYER_STATUS_COLLECT,
};

//行动限制状态,判断优先级为数组
//
enum ACTIONRESTRICT
{
	PLAYER_CONFUSED,		//混乱状态,到处乱跑,只能用某些特定技能和物品
	PLAYER_CANNOT_MOVE,	//不能移动
	PLAYER_ISDIZZING,		//眩晕状态,不能作动作,只能用某些特定技能和物品
	PLAYER_PARALYSIS,		//瘫痪状态,不能作动作,只能用某些物定技能和物品
	PALYER_COMA,				//昏迷,不能作动作,只能用某些物定技能和物品
	PLAYER_CANNOT_ATTACK, //不能物理攻击.
	PLAYER_CANNOT_USESKILL/*_PHYSICAL*/,//不能用物理技能
// 	PLAYER_CANNOT_USESKILL_MAGICAL,	//不能用魔法技能
// 	PLAYER_CANNOT_USEASSISTSKILL_PHYSICAL,//不能使用物理辅助技能
// 	PLAYER_CANNOT_USEASSISTSKILL_MAGICAL,	//不能使用法术辅助技能,//不能用技能
	PLAYER_CANNOT_RESTORE_HP=10,		//不能回复HP
	PLAYER_CANNOT_RESTORE_MP,		//不能回复MP
	PLAYER_CANNOT_USEITEM=14,			//不能使用物品
};

enum 
{
	FIGHT_MODE_PEACE,                //和平攻击模式
	FIGHT_MODE_ATTACK,               //恶意攻击模式
	FIGHT_MODE_PK,					 //全体攻击模式
    FIGHT_MODE_HOME,                 //帮会攻击模式
	FIGHT_MODE_COUNTRY,              //国家攻击模式
	FIGHT_MODE_EVIL,				 //强制的,红名模式
	FIGHT_MODE_CAMP,				 //强制的,阵营模式],优先级比红名高
	FIGHT_MODE_PVE,					 //完全安全模式,优先级最高
	FIGHT_MODE_PK_NO_HOME,			 //屠杀模式，蛋疼。。。杀帮会以外所有人
	FIGHT_MODE_JIEBIAO,				 //劫镖模式
	FIGHT_MODE_BE_FUCKED,			 //挨打模式
	FIGHT_MODE_MAX,
};


enum {
	SKILL_TYPE_OBJ,
	SKILL_TYPE_POINT,
	SKILL_TYPE_MYSELF,	
};

enum {	
	SKIN_HEAD,                 //头像 add by cf
	SKIN_WEAPON,
	SKIN_WEAPON_TYPE,
	SKIN_DRESS,
	SKIN_DECK,
	SKIN_HORSE,
	SKIN_STALLAGE,
	SKIN_WEAPON_EFF,
	SKIN_FASHION,
	SKIN_MAX,
};

enum {
	ATTACK_MODE_ARROW,
	ATTACK_MODE_MAGIC1,//技能ID
	ATTACK_MODE_MAGIC2,
};

enum
{
	ENEMY_NAME_COLOR_WHITE,
	ENEMY_NAME_COLOR_YELLOW,
	ENEMY_NAME_COLOR_RED,
	ENEMY_NAME_COLOR_MAX,
};
enum 
{
	NAME_COLOR_WHITE,                       //白名
	NAME_COLOR_PINK,                        //粉名
	NAME_COLOR_BLACK,                       //黑名
	NAME_COLOR_GRAY,                        //灰名
	NAME_COLOR_PURPLE,						//紫名
	NAME_COLOR_MAX,
};

enum EMALL_RESULT
{
	MALL_NO_BUY_PLAYER,
	MALL_NO_GET_PLAYER,    
	MALL_NO_ENOUGH_GOLD,
	MALL_NO_ENOUGH_SILVER,
	MALL_ITEMBAG_FULL,  	
	MALL_OTHER_ERROR,
	MALL_BUY_SUCCESS,
	MALL_SEND_LESSLEVEL,
	MALL_NOT_SAME_MAP,
	MALL_SAFE_LOCK,
	MALL_LIMIT_CNT,
	MALL_NO_ITEM,
	MALL_NO_ENOUGH_BINDGOLD,
	MALL_SELL_SUCCESS,
	MALL_SELL_FAILED,
	MALL_LOCKED_ITEM,
	MALL_NO_ENOUGH_FISHSCORE,
	MALL_BIND_ERROR,
	MALL_NO_ENOUGH_NEWSPCOIN,
};

//#define STATUS_MASK		0xFF
// status = FFSSSSSS  S = status & 0x3F, F = status >> 6
struct ActInfo
{	
	int	 nID;
	char szTime[30];
	char szName[30];
	char sNPC[20];
	char szRequire[30];
	BYTE bRepeat;
	ActInfo()
	{
		memset(this,0,sizeof(ActInfo));
	};
};
struct  playerYiTianInfo
{
	DWORD uin;
	char szName[20];
	DWORD equipScore;
	DWORD battleScore;
	BYTE  job;
	int   lv;
	playerYiTianInfo(){
		uin = 0;
		memset(szName,0,20);
		equipScore = 0;
		battleScore = 0;
		job = 0;
		lv = 0;
	}
};
struct PLAYER_GUILDNAMEPOST_Data
{
    DWORD   uin;
	char    name[1];
};

struct PLAYER_SEND_OPEN_TIANXIA_RANK_Data 
{
	BYTE byScore[16];
};

struct PLAYER_C_OPERTION_BOMB_Data 
{
	BYTE	bOpertion;		//1喂养品质, 2开始升级  3提高精通	4升级加速	5 获取具体信息
	int		nIndex;
	int		Val;
	int		BombIndex;
};

struct PLAYER_SEND_BOMB_INFO_Data 
{

	BYTE	byQuality;		//1阶1段-9阶9段	%9 段 /9 阶
	int		nQualityExp;	//暗器品质经验
	BYTE	byLevel;		//0-100
	BYTE	byMasteryLv;	//暗器精通等级
	DWORD	dwBullet;		//子弹数目  0-1000
	DWORD	dwTime;			//剩余时间
	DWORD	index;
	DWORD	SkillUse;
};

struct PLAYER_SEND_BOMB_EXP_Data 
{
	BYTE	byLvl;
	DWORD	dwExp;
};

struct PLAYER_SEND_TMP_BOMBINFO_Data 
{
	int		nszValQua[10];
};


struct PLAYER_TELLGM_AREA_Data            //used
{
	DWORD	uin;
	BYTE	byGM;
};

struct PLAYER_SEND_SAHUA_Data 
{
	DWORD dwTargetUin;
};

struct PLAYER_REQUEST_GIVE_LOVE_Data 
{
	char szToName[20];
	int	nNum;
};

struct PLAYER_REQUEST_ADD_MARRY_RING_POINT_Data 
{
	int nNum;
};

struct PLAYER_SEND_YITIAN_INFO_Data 
{
	int site;
	playerYiTianInfo playerinfo;
};
struct PLAYER_SEND_YITIAN_INFO2_Data 
{
	int site;
	playerYiTianInfo playerinfo;
};

struct PLAYER_WARMACHINE_INFO_Data 
{
	DWORD	dwLeader;
	DWORD	dwParter[4];
};



struct PLAYER_INVITE_WARMACHINE_Data 
{
	DWORD	dwLeader;
};

struct PLAYER_S_GAMBLE_SLAVE_PK_Data 
{
	int		nIndex;
	char	szAPName[16];
	char	szBPName[16];
	char	szASName[20];
	char	szBSName[20];
	DWORD	dwASVal;
	DWORD	dwBSVal;
	DWORD	dwAMoneyT;
	DWORD	dwBMoneyT;
	WORD	wSlaveAIndex;
	WORD	wSlaveBIndex;
};

struct PLAYER_S_GAMBLE_SIMPLE_INFO_Data 
{
	int		nIndex;
	DWORD	dwAMoneyT;
	DWORD	dwBMoneyT;
	char	szPName[16];
};

struct PLAYER_c_REPLACE_ITEM_Data 
{
	int		m_nSelect;
	int		n_nCount;
};

struct PLAYER_C_EXCHANGE_RIDE_Data 
{
	DWORD	dwItemID;
	DWORD	dwCostIndex;
	BYTE	bType;
};
struct PLAYER_C_SWORDSPRITE_OPE_Data 
{
	BYTE	dwOper;
	DWORD	dwItemID;
	WORD	wValEx;
};
struct PLAYER_SP_SEND_WORLDMSG_Data 
{
	char sztxt[4096];
};

struct PLAYER_C_GAMBLE_SLAVE_PK_Data 
{
	int		nIndex;
	BYTE	bAPart;
	DWORD	dwMoney;
};


struct PLAYER_SEND_BIG_GROUP_ID_Data
{
	DWORD	dwUin;
	DWORD	dwID;
};

struct PLAYER_REQUEST_SAUCTION_Data
{
	DWORD	dwIndex;
	int		nGold;
};

struct PLAYER_REQUEST_CHANGE_BOSS_Data 
{
	BYTE bType;
};

struct PLAYER_REQUEST_UPDATE_COPY_INFO_Data 
{
	BYTE bRecoverType;
	int nSkillIndex[MAX_PLAYER_COPY_SKILL];
};

struct PLAYER_SYNCPLAYER_Data         //// used
{
	DWORD	uin;
	WORD	x, y;
	BYTE	direction;
	BYTE	skin[SKIN_MAX];
	BYTE	sex;
	BYTE	status;
	BYTE	namecolor;
	BYTE	feature;
	BYTE	speed;
	BYTE	job;
	BYTE	vip;
	BYTE	bForce;
	WORD	wPkCount;
	BYTE	fightMode;
	BYTE	byRS;
	bool	bIsHeimuya;
	DWORD	dwBigGroupID;
	DWORD	dwWarMachineLeader;
};

struct PLAYER_PLAYER_FIGHT_MODE_Data 
{
	DWORD	uin;
	BYTE	byFightMode;
};

struct PLAYER_OUTAREA_Data            //used
{
	DWORD	uin;
};

struct PLAYER_RESETATTRIB_TIME_Data   //used
{
	DWORD	uin;
	int		time;
};

struct PLAYER_JUMP_Data       //used
{
	DWORD	uin;
	POINT	pos;
	bool	bRushAt;		// 是否冲锋
};

struct PLAYER_GET_UININFO_Data 
{
	DWORD	uin;
};

struct PLAYER_UININFO_Data 
{
	DWORD	uin;
	DWORD	hp;
	DWORD	mp;
	DWORD	maxHp;
	DWORD	maxMp;
	WORD	level;
	DWORD   homeid;
	BYTE	post;
	BYTE	country;
	int		countryhonour;
	WORD	wActiveTitleID;
	BYTE	bUseOathTitle;
	int		nHomeICON;
	int		nCountryJob;
	BYTE    bLock;
	BYTE    bHallDuty;
	char    szHallName[16];
	char	name[1];
};

struct PLAYER_SHOPINFO_Data
{
	DWORD  uin;
	char   shop[1];
};

struct PLAYER_SHOPINFOEX_Data
{
	DWORD	uin;
	//char    name[48];
	char    shop[1];
};

struct PLAYER_REQUEST_INIT_GUESS_Data
{
	BYTE byType;
};

struct PLAYER_REQUEST_GUESS_NUM_Data
{
	BYTE byType;
	int nNum;
};

struct PLAYER_GUESS_NUM_RLT_Data
{
	bool bIsEnd;
	BYTE byRlt;
	BYTE nCount;
	BYTE nYourNum;
};

struct PLAYER_UPDATEGUILDINFO_Data 
{
	DWORD	uin;
	DWORD	dwConfID;    //add zzb
	BYTE	bPost;
	char	szText[1];
};

struct PLAYER_ACCOUNT_POINT_Data
{
	int point;
};

struct PLAYER_MOVE_TO_Data 
{
	DWORD	uin;
	BYTE	status;
	WORD	sx, sy;
	WORD	tx, ty;
	BOOLEAN		bForceChg;				//add at 2006-11-14    强制改变自己
};

struct PLAYER_SET_MOVE_TO_Data 
{
	BOOLEAN run;
	WORD sx, sy;
	WORD tx, ty;
};

typedef struct PLAYER_GET_PLAYER_POS_Data
{
	DWORD uin;
	WORD x,y;
	DWORD dwTime;
} PLAYER_SEND_PLAYER_POS_Data;

struct PLAYER_SET_FIGHT_MODE_Data 
{
	BYTE mode;
};

struct PLAYER_PLAYER_STATUS_Data 
{
	DWORD uin;
	BYTE status;
	BYTE direction;
	WORD x, y;
	bool bClientPreDone;
};

struct PLAYER_SET_STATUS_Data 
{
	BYTE status;
	BYTE direction;
	WORD x, y;
	int mapIndex;
	bool bClientPreDone;// 客户端预做了
};

struct PLAYER_CLICK_NPC_Data 
{
	WORD npcIndex;
};

struct PLAYER_STOP_MOVE_Data 
{
	DWORD uin;
	WORD x, y;
	BYTE direction;
};

struct PLAYER_PLAYER_ATTACK_PLAYER_Data 
{
	DWORD uin;
	BYTE  isShifthit;
	DWORD dwDelay;	
};

struct PLAYER_PLAYER_ATTACK_ENEMY_Data 
{
	DWORD enemyIndex;
};

struct PLAYER_PLAYER_ATTACK_SLAVE_Data 
{
	DWORD slaveIndex;
};

struct PLAYER_PLAYER_ATTACK_CHUNK_Data 
{
	WORD chunkIndex;
};

struct PLAYER_SET_PLAYER_ATTACK_Data 
{
	DWORD uin;
	WORD  x, y;
	WORD  effectID;
	BYTE  attackmode;
};

enum 
{
	ATTRIB_INDEX_STR,
	ATTRIB_INDEX_CON,
	ATTRIB_INDEX_ING,
	ATTRIB_INDEX_WIS,
	ATTRIB_INDEX_DEX,
	DEC_INDEX_STR,
	DEC_INDEX_CON,
	DEC_INDEX_ING,
	DEC_INDEX_DEX,
	DEC_INDEX_WIS	
};

struct PLAYER_ADD_ATTRIB_Data 
{
	BYTE attribIndex;
};

struct PLAYER_ADD_ATTRIB_AUTO_Data 
{
	WORD	str;		// 力
	WORD	con;		// 根骨constitution
	WORD	ing;		// 悟性intelligence
	WORD	dex;		// 身法dexterity
	WORD	wis;		// 灵性wisdom	
};

struct PLAYER_FAST_SELL_Data 
{
	int nItemIndex;
};

struct PLAYER_SUB_ATTRIB_Data 
{                                  // @zjq,2005-08-18
	BYTE attribIndex;
};

struct PLAYER_PLAYER_HP_Data
{
	DWORD	uin;
	WORD	hp;
	WORD	maxhp;
};

struct PLAYER_PLAYER_MP_Data
{
	DWORD	uin;
	WORD	mp;
	WORD  maxmp;
};

struct PLAYER_SKILL_ACTON_MYSELF_Data 
{
	BYTE	skillIndex;
};

struct PLAYER_SKILL_USE_Data
{
	DWORD	uin;
	DWORD	dwTargetID;
	BYTE	bTargetType;
	DWORD	skillID;
	BYTE	bOnlyCast;
	BYTE	dir;
	DWORD	dwItemIndex;
};

struct PLAYER_SKILL_ATTACK_INFO_Data
{
	DWORD	dwTargetID;
	BYTE	bTargetType;
	DWORD	skillID;
};
//add by hjt,for the skill that could be charged
struct PLAYER_SKILL_RECHARGE_Data
{
	DWORD uin;
	DWORD skillID;
};

struct PLAYER_BREAK_MOVE_Data 
{
	WORD x, y;
};

struct PLAYER_UNMARRY_Data 
{

};

struct PLAYER_COOLDOWN_TIME_Data 
{
	BYTE bPos;
	DWORD dwOverlaps;
};

// add by hjt
struct PLAYER_CASTING_Data
{
	DWORD uin;
	// if 0,the player is using item;if 1 the player is using skill;
	BYTE Type;
	DWORD OnCastingID;
	BYTE UinType;
	int	OnCastIndx;
	DWORD	dwTargetID;
	BYTE	bTargetType;
};
//add by hjt
typedef struct PLAYER_CASTING_INTERRUPT_Data
{
	DWORD uin;	
	BYTE Type;
	DWORD OnCastingID;
	BYTE UinType;
	int	OnCastIndx;

} PLAYER_CASTING_COMPLETE_Data;



struct PLAYER_ADD_ATTRIB_OK_Data
{
	DWORD uin;
};
struct PLAYER_ADD_ATTRIB_CANCEL_Data
{
	DWORD uin;
};

struct PLAYER_REQUEST_CHECK_MARRY_RING_RANK_Data 
{
	DWORD dwUin;
	int nNowTitle;
};

struct PLAYER_COUNTRY_Data
{
	int country;
	DWORD uin;
};

struct PLAYER_CHANGE_NAME_OK_Data          //add by cf
{
	char name[16];
};

struct PLAYER_CHANGE_NAME_PASS_Data
{ 
    char name[16];
    DWORD uin;
};

struct PLAYER_CHANGEJOB_Data
{
    DWORD uin;
	BYTE  job;
};
struct PLAYER_CHANGESKIN_Data
{
    DWORD uin;
	BYTE skin[SKIN_MAX];
};

struct PLAYER_CHANGENAMECOLOR_Data
{
    DWORD uin;
	BYTE nameColor;
};

struct PLAYER_CHANGESPEED_Data
{
    DWORD uin;
	BYTE AtkSpeed;
	BYTE MoveSpeed;
};

struct PLAYER_BUYMALL_ITEM_Data
{
    DWORD	uin;
	DWORD	ItemID;
	BYTE	Type;
	int		Cnt;
	int		version;
	char	szGet[1];
};

struct PLAYER_SELL_RMBITEM_Data
{
	DWORD	uin;
	DWORD	ItemIndex;
	int		Cnt;
	BYTE	bType;
};

struct PLAYER_BUYMALL_RESULT_Data
{
	DWORD uin;
	BYTE  Rlt;
};

struct PLAYER_CHECKMALL_VERSION_Data
{
	DWORD uin;
	DWORD Version;
};

struct PLAYER_CHECKMALL_VERSION_DAOJU_Data
{
	DWORD uin;
	DWORD Version;
};

struct PLAYER_C_REQUEST_HSTATE_Data
{
	BYTE	bType;
};

struct PLAYER_OPEN_CLIENT_FORM_Data
{
	BYTE	bType;
};

struct PLAYER_CHECKMALL_VERSION_RESULT_Data
{
	DWORD uin;
	bool  blResult;
};

struct PLAYER_CHECKMALL_VERSION_DAOJU_RESULT_Data
{
	DWORD uin;
	bool  blResult;
};

struct MallItem
{
	DWORD ItemID;
	BYTE  Type;
	int   Price;
	BYTE  byGoldType;
	bool  bCanSell;
	bool  bCanGive;
	WORD  wLimitCnt;
};

struct PLAYER_SENDMALL_ITEM_Data
{
	bool bFirst;
	DWORD dwVersion;
	int count;
	MallItem items[1];
};

struct PLAYER_CHANGE_TO_MOB_Data
{
	DWORD dwUin;
	short sMobType;
};

enum
{
	PLAYER_PK_CANNOT_ATTACK_TYPE_LOW,		//	等级过低
	PLAYER_PK_CANNOT_ATTACK_TYPE_DIS,		//	等级差过大
	PLAYER_PK_CANNOT_ATTACK_TYPE_MAX,
};

struct PLAYER_PK_CANNOT_ATTACK_Data
{
	BYTE bType;
	BYTE bLevel;
};

struct PLAYER_GET_ACTIVITY_INFO_Data
{
	int nDate;
};

struct PLAYER_SEND_ACTIVITY_INFO_Data
{
	int nDate;
	BYTE bFirst;
	BYTE bEnd;
	int count;
	ActInfo info[1];
};

struct PLAYER_CHANGE_PK_COUNT_Data
{
	DWORD	dwUin;
	int		nPkCount;
};

struct CHARACTER_ALL_COOLDOWN_TIME_Data
{
	DWORD dwOverlaps[COOLDOWNFLAG_LEN];
};

struct PLAYER_COLLECT_Data
{
	DWORD enemyIndex;
};

struct PLAYER_SEND_SET_ACTIVE_TITLE_Data
{
	DWORD uin;
	WORD  wTitleID;
	char  szname[20];
};

struct PLAYER_REQUEST_SKILL_LEVELUP_Data
{
    WORD wSkillID;
};

struct PLAYER_REQUEST_SKILL_LEARN_Data
{
	WORD wSkillID;
};

struct PLAYER_REQUEST_ERCS_LVLUP_Data
{
	WORD wSkillID;
};

struct PLAYER_REQUEST_GM_LIST_Data
{
	DWORD dwUin;
};

#ifndef MAX_NAMELEN
	#define MAX_NAMELEN 20
#endif


typedef struct GMInfo
{
	DWORD	dwUin;
	char	szName[MAX_NAMELEN];
}MapPlayerInfo;

struct PLAYER_GM_LIST_Data
{
	int nCnt;
	GMInfo xInfo[1];
};

struct PLAYER_MAP_PLAYERS_LIST_Data
{
	int nCnt;
	MapPlayerInfo xInfo[1];
};

struct PLAYER_OPEN_MAKE_Data
{
	BYTE type;
};

struct PLAYER_AUTO_SKILL_SELF_Data
{
	BOOL AutoSelf;
};

struct PLAYER_ASK_RELATIONSHIP_Data
{
	DWORD uin;
};

struct PLAYER_PLAYER_CASTING_KILL_Data
{
	DWORD dwEnemyUin;
};

struct YUANFEN_INFO
{
	BYTE	btLevel;
	BYTE	btJob;
	BYTE	btSex;
	char	szName[MAX_NAMELEN];
};

struct PLAYER_YUANFEN_LIST_Data
{
	int nCnt;
	YUANFEN_INFO xInfo[1];
};

struct PLAYER_COUNTRY_INFO_Data
{
	DWORD	dwUin;
	int		nHonour;
};

struct PLAYER_SEND_HOME_ICON_Data
{
	DWORD	dwUin;
	int		nHomeICON;
};

struct PLAYER_ADD_ACTIVITY_RANK_Data
{
	int		nActivityID;	// 活动ID
	DWORD	dwUin;			// 玩家uin
	int		nMax;			// 最大排名
};

struct PLAYER_SEND_ACTIVITY_RANK_Data
{
	int				nActivityID;				// 活动ID
	int				nPosition;					// 最大排名
	DWORD			dwUin;                      //ID
	char			szName[16];                 //Name  
	BYTE			btSex;                      //Sex
	BYTE			btLevel;                    //Level
	BYTE			btCountry;                  //Country
	BYTE			btJob;                      //Job
};

struct PLAYER_GET_ON_TRANSPORT_Data
{
	DWORD			dwDriver;
};

struct PLAYER_GET_OFF_TRANSPORT_Data
{
	DWORD			uin;
};

struct PLAYER_SEND_DRIVER_INFO_Data
{
	DWORD			uin;
	BYTE			bPassengerCnt;
	DWORD			dwPassenger[1];
};

struct PLAYER_SEND_PASSENGER_INFO_Data
{
	DWORD			uin;
	DWORD			dwDriver;
};

struct PLAYER_GET_TRANSPORT_INFO_Data
{
	DWORD			uin;
};

struct PLAYER_PLAYER_COUNTRY_JOB_Data
{
	DWORD			dwUin;
	int				nCountryJob;
};

struct PLAYER_SET_FASHIONVIEW_Data
{
	BYTE	bView;
};

struct PLAYER_SET_WINGVIEW_Data
{
	BYTE	bView;
};

struct PLAYER_CREATE_OATH_Data
{
	char	szOathName[9];
};

struct PLAYER_RETITLE_OATH_Data
{
	BYTE	bUseOathTitle;
	char	szOathTitle[5];
};

struct PLAYER_DELETE_OATH_Data
{
	DWORD	dwMemberUin;
	DWORD	dwOathID;
};

struct PLAYER_SEND_OATH_Data
{
	DWORD	dwMemberUin;
	DWORD	dwOathID;
	BYTE	bUseOathTitle;
	char	szOathName[9];
	char	szOathTitle[5];
};

struct PLAYER_USE_MARRY_TITLE_Data
{
	DWORD	dwMemberUin;
	BYTE    byUseOathTitle;
};

struct PLAYER_SEND_MARRY_TITLE_Data
{
	DWORD	dwMemberUin;
	char	acMarryTitle[1];
};

struct PLAYER_CHANGE_OATH_Data
{
	DWORD	dwMemberUin;
	DWORD	dwOathID;
};

struct PLAYER_SYNC_PLAYERSEX_Data
{
	DWORD	uin;
	BYTE	skin[SKIN_MAX];
	BYTE	sexchange;
};

struct PLAYER_CLICK_ENEMY_Data
{
	DWORD	dwEnemyUin;
};

struct PLAYER_SKILL_ADDTALENT_Data
{
	WORD	wSkillID[MAX_SHOW_SKILL];
	BYTE	nTalent[MAX_SHOW_SKILL];
	BYTE	nNeedTalent[MAX_SHOW_SKILL];
	int		nCount;
};

struct PLAYER_SKILL_ACTIVE_Data
{
	 WORD  wSkillID;             //对应技能ID
};

struct PLAYER_SKILL_NOACTIVE_Data
{
	 WORD  wSkillID;             //对应技能ID
};

struct PLAYER_REQUEST_DRIVE_TIME_Data
{
	BYTE	bCnt;
};

struct PLAYER_SEND_HEIMUYA_Data
{
	DWORD	dwUin;
	bool	bHeimuya;
};

struct PLAYER_SEND_ARENA_SCORE_Data 
{
	int nArenaScore;
};

struct PLAYER_SEND_ARENA_BATTLEINFO_Data 
{
	int nArenaWinBattle;
	int nArenaTotalBattle;
};

struct PLAYER_SEXP_TO_BOOKEXP_Data
{
	WORD	wSkillID;
	int		nSExpUsed;
};

struct PLAYER_SKILL_UPGRADE_Data
{
	WORD	wSkillID;
};

struct PLAYER_CLIENT_ADDQUEST_VAL_Data
{
	DWORD	nQuestID;
	int		nValAdd;
	char	szValName[1];
};

struct PLAYER_REQUEST_MARRY_BLESS_Data
{
	DWORD dwFromUin;
	DWORD dwWomanUin;
	char szManName[PLAYER_NAME_LEN];
	char szWomanName[PLAYER_NAME_LEN];
};

struct PLAYER_RESPONSE_MARRY_BLESS_Data
{
	DWORD dwFromUin;
	DWORD dwWomanUin;
};

struct PLAYER_REQUEST_PARADE_START_Data 
{
	BYTE byCarType;
	BYTE byParadeType;
};

struct PLAYER_SHUT_UP_SOMEONE_Data 
{
	DWORD dwUin;
	BYTE byMinutes;
};

struct PLAYER_SEND_BOSS_KILL_COUNT_Data
{
	int nCount;
};

struct PLAYER_REQUEST_BOX_OPEN_Data
{
	DWORD	dwIndex;
};

struct PLAYER_REQUEST_BOX_POINT_Data
{
	DWORD	dwIndex;
	DWORD	dwObjID;
};

struct PLAYER_SEND_BOXLUCK_INFO_Data
{
	int nCnt;
	DWORD	dwBuf[1];
};

struct PLAYER_SEND_BOXITEM_INFO_Data
{
	int nCnt;
	DWORD	dwBuf[1];
};

struct PLAYER_OPENDAOJU_Data 
{
	BYTE aaa;
};
#pragma pack (pop)

#endif