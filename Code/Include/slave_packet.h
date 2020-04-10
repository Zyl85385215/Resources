#ifndef _SLAVE_PACKET_H_
#define _SLAVE_PACKET_H_

#include <windows.h>
#include "groupcmd.h"
#include "define.h"
#include "enemy_packet.h"

#pragma pack (push)
#pragma pack (1)

#define SLAVE_REVIVE_ITEM_ID 35026
#define MAX_SLAVE_EVOLUTION_TIME 2
#define TOTAL_SLAVE_PHASE_QUALITY_UP(t) (t * SLAVE_PHASE_QUALITY_UP)

#define SLAVE_REVIVE_BLOOD 0.4

enum 
{
	SYNCSLAVE,	
	GET_SLAVEINFO,
	SLAVEINFO,	
	SLAVEMOVE,	
	SLAVE_STATUS,
	SLAVE_RELOAD,	
	SET_SLAVE_ATTACK,	
	SLAVE_SAY,
	SLAVE_DOMEPER,
	COLLECT_SLAVE,	
	SEND_SLAVE_EXP,
	SEND_SLAVE_LEVELUP,
	SLAVE_HP,
	SLAVE_MP,
	SLAVE_ACTIVATE,
	SLAVE_INACTIVE,
	SLAVE_MODE,
	SLAVE_KILL,
	SLAVE_CMDATK,
	SLAVE_FOLLOW,
	SLAVE_ADD,
	SLAVE_REMOVE,
	SET_SLAVEATK_MODE,
	SLAVEMENU_ADD,
	SLAVEMENU_REMOVE,
	SLAVE_RELEASE,
	SLAVEMENU_UPDATE,
	SLAVEFAILED,
	SLAVE_REVIVE,
	SLAVE_RESULT,
	SLAVE_AUTOPICKUP,
	SET_SLAVE_NAME,
	SLAVE_PACKET,
	SLAVE_RESET,
	SLAVE_RESET_OPEN,
	SLAVE_RESET_END,
	SLAVE_ATTRIBUTE,

	SLAVE_SET_ACTIVE_SKILL,	//设置自动释放	
	SLAVE_ACTIVE_SKILL_RESULT,//设置成功
	SLAVE_USE_SKILL,
	//SLAVE_CHANGE_SKILL_ORDER,//改变技能排列
	SLAVE_COOLDOWN_TIME,
	SLAVE_ADD_POINTS,
	CHANGE_SLAVE_SLOT,
	OPEN_SLAVE_BANK,
	SLAVE_LIFE,
	SEND_BLANK_COUNT,
	CLEAR_SLAVES,
	SET_CURRENT_SLAVE,
	SLAVE_PHASE_OPEN,
	SLAVE_PHASE_UP,
	SLAVE_PHASE_RESULT,
	SLAVE_PARAM,
	SLAVE_FIGHT_PARAM,
	SLAVE_NEW_REVIVE,
	SLAVE_NEW_REVIVE_RESULT,
	SLAVE_NEW_REVIVE_OPEN,
	SLAVE_BIND,
	SLAVE_UNBIND,
	SLAVE_CANCEL_UNBIND,
	SLAVE_BIND_RESULT,
	SLAVE_UNBIND_RESULT,
	SLAVE_CANCEL_UNBIND_RESULT,
	SLAVE_UNBIND_END,

	S_RESET_MP,		//重置所有宠物的MP
	S_SLAVE_POW,
	C_FEED_ITEM,
	S_SLAVE_MP,
	C_SLAVE_RESET_POINT,
	S_SLAVE_TMP_POINT,
	C_SLAVE_ACCPT_POINT,
	C_SLAVE_CANCEL_POINT,
	SLAVE_PHASE_CLEAR,
	C_SLAVE_NEWFUSE,
	S_SLAVE_NEWFUSE_RESULT,
	C_SLAVE_FUSE_ONOFF,
	S_SLAVE_FUSEONOFF_RTL,

	GET_SLAVEINFO_FAIL,
	SLAVE_FAR_AWAY,

	C_SLAVE_LEARN_SKILL,
	C_SLAVE_TRANS_SKILL,

	C_SLAVE_TRANS_ALL,

	S_SLAVE_RESET_ONEQUA,

	C_SLAVE_LOCK_PHASE,
	C_SLAVE_LOCK_PHASE_READY,
	S_SLAVE_LP_READY_VAL,

	S_SLAVE_FUSE_SKILL_INIT_RTL,
	C_SLAVE_FUSE_SKILL_INIT,
	C_SLAVE_FUSE_SKILL,

	CHANGE_MODE,					//服务端向客户端发送，改变宠物模式

	C_SLAVE_NEWFUSE_STEP,

	C_SLAVE_PHASEEX_FEED,
	S_SLAVE_PHASEEX_RTL,

	S_SLAVE_CTRL_RTL,
	
	C_SLAVE_JIEFENG,
	SLAVE_DEATH_OFFHAND,

	SEND_CATCH_SLAVE,	// 申请抓到宠物
	SEND_CATCH_SLAVE_RESULT,	// 抓到宠物结果
};

enum 
{
	SLAVE_STATUS_STAND,
	SLAVE_STATUS_WALK,
	SLAVE_STATUS_ATTACK,
	SLAVE_STATUS_ATTACK2,
	SLAVE_STATUS_HURT,
	SLAVE_STATUS_DEATH,
	SLAVE_STATUS_OUT,
	SLAVE_STATUS_RUN,	
	SLAVE_STATUS_FOLLOW,
	SLAVE_STATUS_SKILL,	
	SLAVE_STATUS_REST,
};

enum 
{
	ATK_NULL,
	ATK_ACTIVE,			//主动
	ATK_RETORT,			//反击
	ATK_PASSIVE,		//被动	
};


enum SLAVE_GET_TYPE
{
	SLAVE_GET_SKILLS,				//技能
	SLAVE_GET_ITEMS,				//购买掉落，物品
	SLAVE_GET_SYSTEM,				//系统，脚本
	SLAVE_GET_EVOLUTION,			//进化
};

enum ESLAVE_QUALITY_TYPE
{
	SLAVE_QUALITY_TYPE_HP,
	SLAVE_QUALITY_TYPE_MP,
	SLAVE_QUALITY_TYPE_ATKW,
	SLAVE_QUALITY_TYPE_ATKN,
	SLAVE_QUALITY_TYPE_DEFW,
	SLAVE_QUALITY_TYPE_DEFN,
	SLAVE_QUALITY_TYPE_APN,
	SLAVE_QUALITY_TYPE_DPW,
};

struct ResetPointInfo
{
	DWORD	m_dwSlaveID;
	WORD	m_wQuality;
	int		wMaxHp;
	int		wMaxMp;
	int		wDunt;	
	int		wAtkW;
	int		wAtkN;
	int		wApN;
	int		wDefN;
	int		wDefW;
	int		wMdl;
	int		wDpW;
	int		wFinalMaxHp;
	int		wFinalMaxMp;
	int		wFinalDunt;	
	int		wFinalAtkW;
	int		wFinalAtkN;
	int		wFinalApN;
	int		wFinalDefN;
	int		wFinalDefW;
	int		wFinalMdl;
	int		wFinalDpW;
};
class Attri_Base
{
public:
	WORD	str;
	WORD	con;
	WORD	dex;
	WORD	ing;
	WORD	wis;
};

struct SlaveAttribute //本身属性，没有技能加成的
{
	int						wMaxHp;
	int						wMaxMp;	
	int						wAtkW;
	int						wAtkN;
	int	 					wDefW;
	int						wDefN;
	int						wApN;	//命中
	int						wDpW;	//闪避
	int						wDunt;	//PPN	重击
	int						wMdl;	//PPW	致命
	short					styy;
	WORD					wDpN;//灵力
	//	宠物一级属性
	WORD					str;
	WORD					wis;
	WORD					ing;
	WORD					dex;
	WORD					con;
	WORD					points;			//  剩余点数

	WORD					wDemonPow;		//无视防御	
};

#define SLAVE_SKILL_COUNT_MAX		6
#define SLAVE_SKILL_COUNT_INITIAL	3
#define SLAVE_SKILL_COUNT_EVOLUTION 6

struct SlaveSkills //
{
	BYTE					bCount;
	WORD					wSkills[SLAVE_SKILL_COUNT_MAX];
};

enum eSlaveSlot
{	
	SLAVE_SLOT_PERSON,
	SLAVE_SLOT_BANK,
	SLAVE_SLOT_MAX
};

enum ESLAVE_BIND_STATUS
{
	SLAVE_BIND_STATUS_NULL,			//没有绑定
	SLAVE_BIND_STATUS_BINDED,		//绑定中
	SLAVE_BIND_STATUS_UNBINDING,	//解绑中
};

enum ESLAVE_BIND_LEVEL
{
	SLAVE_BIND_LEVEL_0,		//0级绑定权限，不可放生与交易
	SLAVE_BIND_LEVEL_1,		//1级绑定权限，不可交易
};
#define SLAVE_BIND_LEVEL_MAX (SLAVE_BIND_LEVEL_1 + 1)

enum ESLAVE_BIND_TYPE
{
	SLAVE_BIND_TYPE_NO_RELEASE	= 0x0001,		//禁止放生
	SLAVE_BIND_TYPE_NO_TRADE	= 0x0002,		//禁止交易
};

static int s_eSlaveBindType[SLAVE_BIND_LEVEL_MAX] = {
	SLAVE_BIND_TYPE_NO_RELEASE|SLAVE_BIND_TYPE_NO_TRADE, SLAVE_BIND_TYPE_NO_TRADE
};

#define SLAVE_BIND_ITEM 35048
#define SLAVE_BIND_ITEM_SILVER 35245

struct SlaveBindInfo
{
	ESLAVE_BIND_STATUS eSlaveBindStatus;
	DWORD dwNeedUnbindTime;
	DWORD dwUnbindTickCount;
	ESLAVE_BIND_LEVEL eSlaveBindLevel;
};

enum ESLAVE_UNBIND_TIME
{
	SLAVE_UNBIND_TIME_SIX_HOURS,
	SLAVE_UNBIND_TIME_ONE_DAY,
	SLAVE_UNBIND_TIME_THREE_DAYS,
	SLAVE_UNBIND_TIME_SEVEN_DAYS,
	SLAVE_UNBIND_TIME_INFINITY,
};

#define SLAVE_UNBIND_TIME_MAX (SLAVE_UNBIND_TIME_INFINITY + 1)

#define SLAVE_UNBIND_TIME_INFINITY_VALUE 0xFFFFFFFF

#define SLAVE_UNBIND_ITEM 35049

struct SlaveUnbindTimeInfo
{
	DWORD dwBindItemCount;
	DWORD dwUnbindSeconds;
};

static SlaveUnbindTimeInfo s_xSlaveUnbindTimeInfo[SLAVE_UNBIND_TIME_MAX] = {
	{1, 6*60*60}, {2, 24*60*60}, {4, 3*24*60*60}, {8, 7*24*60*60}, {0, SLAVE_UNBIND_TIME_INFINITY_VALUE}
};

struct SlaveDataEx
{
	DWORD					m_dwMasterUin;			// Maste uin(id)
	DWORD					m_dwUin;				// Slave uin(id) (转换为物品时，记为客户端怪物模版类型)	
	WORD					m_wSlaveIndex;			// 怪物模版索引

	BYTE					m_bInBatalle;			// 是否参战	
	WORD					m_wLevel;				// 等级
	BYTE					m_bPhase;				// 进阶	
	WORD					m_wPowLv;				
	DWORD					m_dwPowExp;
	BYTE					m_bPhaseLimit;
	BYTE					m_bFuseLvl;			//宠物融合等级
	int						m_nFuseFeedVal;

	int						m_nPhaseExSoul;
	int						m_nPhaseExVal;
	BYTE					m_bPhaseExLv;

	DWORD					m_dwExp;				// 当前经验
	DWORD					m_dwExpLevel;			// 升级所需经验	
	short					m_stLife;				// 寿命
	int						m_nSPValQua;
	int						m_nPhaseUpLuckCount;
	DWORD					m_dwWildSoul;			// 野性兽魂
	DWORD					m_tmSLockPhase;				//结合的开始时间,CMTime的tickcnt
	char					m_szName[20];			// 自定义名称
	int						m_nHp;
	int						m_nMp;
	WORD					m_wQuality;
	int						m_nAbilityEx[3];			//天赋  || 装备INDEX

	SlaveAttribute			AttributeInfo;
	SlaveAttribute			BaseAttributeInfo;		//宠物原始一级属性
	SlaveSkills				skills;
	//SlaveSkills				activeSkills;			//自动释放的技能
	LONGLONG				m_LLSlaveKey;
	BYTE					m_bSlot;					//   ||不为0表示有疾奔技能
	bool					m_bBB;
	//BYTE					bExtend[SLAVE_EXTEND_MODE_COUNT];
	int						m_SkillSlot;				//  技能转移成功率 ||不为0表示有神驹技能
	int						m_nSkillTrans;

	EBLOOD_PULSE_TYPE		m_eBloodPulse;			//	血脉

	BYTE		m_eSlaveAttackType;		//  宠物攻击类型

	SlaveBindInfo			m_xSlaveBindInfo;

	int						m_bInTrade;
	BYTE		m_bFuseSkillPow;		//拿来记附身技能强度了
	DWORD					m_dwLimitTime;
	int			m_nHunger;			//0-100
	int			m_nHappy;			//0-200
	int			m_nClean;			//0-100
	DWORD		m_dwSickBit;		//疾病位,1-7?
};

struct SLAVE_SYNCSLAVE_Data                 
{
	DWORD	slaveID;
	WORD	x, y;
	BYTE	direction;
	WORD	type;
	BYTE	status;
	BYTE	speed;
	BYTE	level;
	int		hp;
	int		maxhp;
	//WORD	feature;               //状态
	BYTE	namecolor;
	char	name[MAX_NAMELEN];
	//char	subName[MAX_NAMELEN];
	BYTE	bForce;
	int		BaseID;
	BYTE	bExEffect;
};

struct SLAVE_SLAVE_STATUS_Data 
{
	DWORD	slaveID;
	BYTE	status;
	BYTE	direction;
	WORD	x, y;
};

struct SLAVE_SLAVE_DEATH_OFFHAND_Data 
{
	DWORD	slaveID;
	DWORD	masterID;
};

struct SLAVE_SLAVEFAILED_Data
{
	DWORD	UIN;
};

struct SLAVE_SLAVE_REVIVE_Data
{
	DWORD	UIN;	
};

struct SLAVE_SLAVE_RESULT_Data
{
	DWORD	UIN;
	BYTE  result;
};
struct SLAVE_SLAVE_AUTOPICKUP_Data
{
	BYTE  color;
};
struct SLAVE_GET_SLAVEINFO_Data 
{
	DWORD	slaveID;
};

struct SLAVE_SLAVEMOVE_Data 
{
	DWORD	slaveID;
	WORD	tx, ty;
};

struct SLAVE_SLAVEINFO_Data 
{
	DWORD	slaveID;
	int		maxHp;
	int		hp;
	int		maxMp;
	int		mp;
	BYTE	slaveLevel;
	int		baseid;
	CHAR	szName[MAX_NAMELEN];
	//CHAR	szSubName[MAX_NAMELEN];
	DWORD	dwMasterUin;		
	BYTE	bStars;	//星数
	BYTE	bPhase;
};

struct SLAVE_SLAVE_ADD_Data
{
	DWORD	slaveID;
	int		maxHp;
	int		hp;
	int		maxMp;
	int		mp;
	DWORD	m_dwExp;
	DWORD	m_dwLevelExp;
	BYTE	slaveLevel;
	int		baseid;
	BYTE	bGetType;
};

struct SLAVE_CHANGE_MODE_Data
{
	DWORD	slaveID;
	BYTE	bGetType;
};

struct SLAVE_SLAVEMENU_ADD_Data
{
	BYTE bSlot;
    SlaveDataEx data;
	int	skin;
	bool bCapture;
	int nLimitTime;
};

struct SLAVE_SLAVE_REMOVE_Data
{
	DWORD	slaveID;
	BYTE	bGetType;
};

struct SLAVE_SLAVE_HP_Data 
{
	DWORD	slaveID;
	int		hp;
};

struct SLAVE_SLAVE_MP_Data 
{
	DWORD	slaveID;
	int		mp;
};

struct SLAVE_SLAVE_ACTIVATE_Data 
{
	DWORD	slaveID;
};

struct SLAVE_SLAVE_INACTIVE_Data
{
	DWORD	slaveID;
};

struct SLAVE_SLAVEMENU_REMOVE_Data
{
	DWORD	slaveID;
};

struct SLAVE_SLAVE_RELEASE_Data
{
	DWORD	slaveID;
};

struct SLAVE_SEND_SLAVE_EXP_Data           //exp changed
{
	DWORD	slaveID;
	DWORD	exp;
	DWORD   addExp;
};

struct SLAVE_SEND_SLAVE_LEVELUP_Data
{
	DWORD dwSlaveID;
	BYTE bLevel;
	SlaveAttribute attribute;
	DWORD dwNextLevelExp;
	DWORD dwExp;
	short stLife;
	WORD  wWildSoul;
};

struct SLAVE_SET_SLAVE_ATTACK_Data 
{
	DWORD	slaveID;
	DWORD	dwTargetID;
	BYTE	bType;
	WORD	wStartEffect;
	WORD	wFlyEffect;
};

struct SLAVE_SLAVE_RELOAD_Data 
{
	DWORD	slaveID;
};

struct SLAVE_SLAVE_MODE_Data 
{
	DWORD	slaveID;
	BYTE	bMode;
};

struct SLAVE_SLAVE_CMDATK_Data 
{
	DWORD	dwSlaveId;
	DWORD   dwTargetId;
	BYTE	bType;
};

struct SLAVE_SET_SLAVEATK_MODE_Data
{
	BYTE  bSlaveIndex;
	BYTE  bSlaveAtkMode;
	DWORD dwTargetUin;
	BYTE  bTargetType;
};

struct SLAVE_SET_SLAVE_NAME_Data
{
	DWORD dwSlaveID;
	char  szName[20];
};

struct SLAVE_SLAVE_PACKET_Data
{
	DWORD dwSlaveID;
};

struct SLAVE_SLAVE_ATTRIBUTE_Data
{
	DWORD dwSlaveID;
	SlaveAttribute attribute;
	bool  bInBattle;
	short stLife;
};

struct SLAVE_SLAVE_USE_SKILL_Data
{
	DWORD	dwSlaveID;
	WORD	wSkillID;	
	BYTE	bObjType;
	DWORD	dwObjID;
};

struct SLAVE_SLAVE_SET_ACTIVE_SKILL_Data
{
	DWORD dwSlaveID;
	WORD wSkillID;
	bool bIsActive;
};

struct SLAVE_SLAVE_ACTIVE_SKILL_RESULT_Data
{
	DWORD dwSlaveID;
	WORD wSkillID;
	BYTE bActive;
};

//struct SLAVE_SLAVE_CHANGE_SKILL_ORDER_Data
//{
//	DWORD dwSlaveID;
//	WORD wSkillID1;
//	WORD wSkillID2;
//};

struct SLAVE_SLAVE_COOLDOWN_TIME_Data
{
	DWORD dwSlaveID;
	BYTE bPos;
	DWORD dwOverlaps;
};

struct SLAVE_SLAVE_ADD_POINTS_Data
{
	DWORD   dwSlaveID;
	WORD	str;
	WORD	wis;
	WORD	ing;
	WORD	dex;
	WORD	con;
};

struct SLAVE_CHANGE_SLAVE_SLOT_Data
{
	DWORD dwSlaveID;
	BYTE bSlotFrom;
	BYTE bSlotTo;
	DWORD dwNewSlaveID;
};

struct SLAVE_SLAVE_LIFE_Data
{
	DWORD	slaveID;
	BYTE	bLife;
};

struct SLAVE_SLAVE_RESET_Data
{
	DWORD	slaveID;
	SlaveAttribute attribute;
	DWORD   exp;
	DWORD   levelUpExp;
	BYTE	bLife;
	int		nSlaveQuality;
};

struct SLAVE_SLAVE_COMPOSE_Data
{
	DWORD dwUIN1;
	DWORD dwUIN2;
};

struct SLAVE_SLAVE_COMPOSERESULT_Data
{
	DWORD	slaveID;
	DWORD	slaveDelID;
	SlaveAttribute attribute;
	DWORD   exp;
	DWORD   levelUpExp;
	BYTE	bLife;
	int		nSlaveQuality;
};

struct SLAVE_SEND_BLANK_COUNT_Data
{
	BYTE	bSlot;	
	BYTE	bCount;	
};

struct SLAVE_SET_CURRENT_SLAVE_Data
{
	DWORD	slaveID;
};

struct SLAVE_SLAVE_PHASE_UP_Data
{
	DWORD	slaveID;
	//DWORD	itemindex;
};
struct SLAVE_SLAVE_PHASE_CLEAR_Data
{
	DWORD	slaveID;
};

struct SLAVE_C_SLAVE_NEWFUSE_Data
{
	DWORD	slaveID;
	WORD	wItemID;
	BYTE	bProtect;
};

struct SLAVE_C_SLAVE_PHASEEX_FEED_Data
{
	DWORD	slaveID;
	BYTE	bOpertion;
	DWORD	dwEx;		//宠物ID或者物品index
};

struct SLAVE_S_SLAVE_PHASEEX_RTL_Data
{
	DWORD	slaveID;
	DWORD	dwSoul;
	DWORD	dwVal;		//宠物ID或者物品index
	BYTE	bLv;
	BYTE	bLuckCnt;
};

struct SLAVE_C_SLAVE_NEWFUSE_STEP_Data
{
	DWORD	slaveID;
	WORD	wItemID;
};



struct SLAVE_C_SLAVE_TRANS_ALL_Data
{
	DWORD	dwDestSID;
	DWORD	dwSrcSID;
	BYTE	byLuckCnt;
	//BYTE	bTrans;		//旧宠物转走那些技能
	//BYTE	bDel;		//目标宠物删除哪些技能
	//WORD	wSkillRm[1];	//先转走的,后面是删除的
};

struct SLAVE_S_SLAVE_RESET_ONEQUA_Data
{
	DWORD	dwSlaveID;
	BYTE	bType;
};

struct SLAVE_S_SLAVE_LP_READY_VAL_Data
{
	int		nVal;
};

struct SLAVE_S_SLAVE_FUSE_SKILL_INIT_RTL_Data
{
	BYTE	bValFrm;
	WORD	bValSkill;
};


struct SLAVE_C_SLAVE_FUSE_SKILL_Data
{
	DWORD	dwSID;
};

struct SLAVE_C_SLAVE_LEARN_SKILL_Data
{
	DWORD	dwSID;
	DWORD	dwItemIndex;
};

struct SLAVE_C_SLAVE_LOCK_PHASE_Data
{
	DWORD	dwSID;
};

struct SLAVE_C_SLAVE_TRANS_SKILL_Data
{
	DWORD	dwFromSID;
	DWORD	dwToSID;
	WORD	wSkill;
};

struct SLAVE_C_SLAVE_FUSE_ONOFF_Data
{
	DWORD	slaveID;
};

struct SLAVE_C_SLAVE_RESET_POINT_Data
{
	DWORD	slaveID;
	BYTE	byResetObj;
};

struct SLAVE_S_SLAVE_TMP_POINT_Data
{
	ResetPointInfo info;
};

struct SLAVE_SLAVE_NEW_REVIVE_Data
{
	DWORD	slaveID;
	//DWORD	itemindex;
};

struct SLAVE_SLAVE_EXTEND_RESULT_Data
{
    BYTE result;
};

struct SLAVE_SLAVE_PHASE_RESULT_Data
{	
	BYTE result;
};
struct SLAVE_S_SLAVE_NEWFUSE_RESULT_Data
{	
	DWORD	dwSUin;
	BYTE	bFuseLv;
	int		nVal;
};

struct SLAVE_S_SLAVE_FUSEONOFF_RTL_Data
{	
	DWORD	dwSUin;
};

struct SLAVE_S_SLAVE_CTRL_RTL_Data
{	
	DWORD	dwSUin;
};

struct SLAVE_C_SLAVE_JIEFENG_Data 
{
	DWORD	dwSUin;
};

struct SLAVE_SLAVE_NEW_REVIVE_RESULT_Data
{	
	BYTE result;
};

struct SLAVE_SLAVE_PARAM_Data
{
	DWORD	uin;
	DWORD	HP;
	DWORD   maxHP;			// 精
	DWORD	MP;
	DWORD   maxMP;			// 神
};

struct SLAVE_SLAVE_FIGHT_PARAM_Data
{
	DWORD	uin;

	DWORD	wAtkW;
	DWORD	wDefW;
	DWORD	wDpW;
	DWORD	wPpW;

	DWORD	wApN;
	DWORD	wAtkN;
	DWORD	wDefN;
	DWORD	wPpN;
	//int		nAtkYY;
	//int		nDefYY;
	//// 五行属性
	//int		nJin;
	//int		nMu;	
	//int		nShui;
	//int		nHuo;
	//int		nTu;
};

struct SLAVE_SLAVE_BIND_Data
{
	DWORD	dwUin;
	ESLAVE_UNBIND_TIME eUnbindTime;
};

typedef struct SLAVE_SLAVE_UNBIND_Data
{
	DWORD	dwUin;
}SLAVE_SLAVE_CANCEL_UNBIND_Data,
SLAVE_SLAVE_UNBIND_END_Data;

typedef struct SLAVE_SLAVE_BIND_RESULT_Data
{
	DWORD	dwUin;
	bool	bSuccessful;
}SLAVE_SLAVE_UNBIND_RESULT_Data,
SLAVE_SLAVE_CANCEL_UNBIND_RESULT_Data;

struct SLAVE_S_SLAVE_POW_Data
{
	DWORD	dwSlaveID;
	WORD	wPowLv;
	DWORD	dwPowExp;
	WORD	wWildSoul;
};
struct SLAVE_S_SLAVE_MP_Data
{
	DWORD	dwSlaveID;
	WORD	wMP;
};


struct SLAVE_C_FEED_ITEM_Data
{
	DWORD	dwSlaveID;
	DWORD	dwItemIndex;
	BYTE	bItem;
};

struct SLAVE_SEND_CATCH_SLAVE_Data// 申请抓到宠物
{
	short	slaveType;
};

struct SLAVE_SEND_CATCH_SLAVE_RESULT_Data// 抓到宠物结果
{
	short	slaveType;
};

#pragma pack (pop)
#endif