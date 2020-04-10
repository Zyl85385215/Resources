#pragma once

#ifndef _ISSERVER
#include "GlobalDefine.h"
#endif

#define MAX_ACTIVE_SKILL	7

#pragma pack (push)
#pragma pack (1)

enum
{
	MS_ENEMYSYNC,		//怪物信息包
	MS_SLAVESYNC,	//宠物包
	MS_USERSYNC,		//玩家信息

	MS_ENEMYCMD,		//行为方针变化,包括离开玩家视野
	//MS_NPCCMD,			//不动,能动的用怪物做
	MS_SLAVECMD,
	MS_USERCMD,			//具体移动信息

	MS_OBJOUT,

	MS_CTRLINFO,	//操控对象的血量,蓝量, 技能信息,快捷物品信息,坐标(进入地图和切换控制的时候需要)
	MS_SKILLCHG,
	//MS_USKILL,			//对象放技能
	MS_STATEINFO,		//状态信息
	MS_STATECHG,		//状态改变和消失
	MS_STATEDEL,
	MS_HPINFO,			//血量改变信息合包发

	MS_ATKINFO,			//攻击包,主目标伤害信息直接带
	MS_HURTINFO,		//范围技能的伤害信息除主目标外的

	//MS_POSLIST,			//位置信息合包发

	//GS_CTRLON,
	//GS_CTRLOFF,		//离开

	C_SETCMD,		//对目标下指令
	C_SETMOVE,		//客户端设置移动目的
	C_SETATK,		//客户端发动攻击目标

	MS_PARAMCHG,
	MS_JUMPINMAP,
	MS_ACTSKILL,

	MS_MPINFO,
	MS_TITLECHG,
	MS_SPEEDCHG,

	MS_OBJCHGCTRL,

	MS_WEATHCHG,
	MS_SPVAL,

	MS_RUSHINMAP,
	MS_STRIKESKILL,

	MS_SETLOCKEFF,

	CHECK_MAPSVR_ALIVE,

	MS_ENEMYSYNCEX,

	MS_HPMAXINFO,

	C_GET_PARAM,
	MS_USER_PARAMS,
	MS_CESKILLCD,		//召唤技能不会有MS_ATKINFO
	MS_VIPLVCHG,
	MS_HEALTHHPMP,

	MS_OBJCHGFORCE,
	MS_MARRYTITLECHG,
	MS_MARRYEFFCHG,

	C_SHOW_FISHACT,
	MS_CHGBTGROUP,
};

enum
{
	MCMD_NULL,		//无指令,服务端不动, 客户端一定范围随机走
	MCMD_FOLLOW,	//跟随一个指定目标,也可以是定点移动		//每秒同步
	MCMD_ATTACK,	//攻击,并跟随特定目标	//每秒同步
	MCMD_RUN,		//跑步	
	
	MCMD_LOCKVIEW_ENEMY	= 10,
	MCMD_MAX,
};

struct	MAPCMD_MS_ENEMYSYNC_Data
{
	DWORD	dwID;
	WORD	wBaseID;
	int		nHp;		//血量
	WORD	wPosX,wPosY;
	BYTE	bCmd;
	WORD	wSpeed;
	DWORD	dwCmdVal;		//移动用高低位区分
	DWORD	dwMastID;
	WORD	bForce;
	BYTE	bRandMove;
	BYTE	byStateCnt;
	WORD	wStateID;
	char	szTitle[22];
	BYTE	byNewSCnt;
	WORD	wNewStateID[1];
};

struct	MAPCMD_MS_ENEMYSYNCEX_Data
{
	DWORD	dwID;
	WORD	wBaseID;
	WORD	szSkin[3];
	int		nHp;		//血量
	int		nMaxHp;		//血量
	WORD	wPosX,wPosY;
	BYTE	bCmd;
	BYTE	byLv;
	WORD	wSpeed;
	DWORD	dwCmdVal;		//移动用高低位区分
	DWORD	dwMastID;
	WORD	bForce;

	BYTE	byStateCnt;
	WORD	wStateID[1];
};

//
//struct	MAPCMD_MS_NPCSYNC_Data
//{
//	DWORD	dwID;
//	WORD	wBaseID;
//	WORD	wPosX,wPosY;
//};

struct	MAPCMD_MS_SLAVESYNC_Data
{
	DWORD	dwID;
	char	szName[16];
	//DWORD	dwMaster;
	WORD	wSkin;
	WORD	wPosX,wPosY;
	int		nHp;
	int		nHpMax;
	BYTE	bCmd;
	WORD	wSpeed;
	DWORD	dwCmdVal;		//移动用高低位区分
	DWORD	dwMastID;
	BYTE	bLv;
	WORD	bForce;
	BYTE	byStateCnt;
	int		nMarryEff;
	WORD	wStateID[1];
};

struct	MAPCMD_MS_JUMPINMAP_Data
{
	
	DWORD	dwUin;
	BYTE	byType;
	WORD	wPosX,wPosY;
};

struct	MAPCMD_MS_HEALTHHPMP_Data
{

	DWORD	dwUin;
	BYTE	byType;
	int		nAddHp;
};

struct	MAPCMD_MS_RUSHINMAP_Data
{
	DWORD	dwUin;
	BYTE	byType;
	BYTE	bAction;
	WORD	wPosX,wPosY;
};

struct	MAPCMD_MS_STRIKESKILL_Data
{
	WORD	wSkill;
};

struct	MAPCMD_MS_SETLOCKEFF_Data
{
	BYTE	byType;
	BYTE	byLocked;
};


struct	MAPCMD_MS_TITLECHG_Data
{
	char	szTitle[22];
	DWORD	dwID;
	BYTE	byType;
};

struct	MAPCMD_MS_MARRYTITLECHG_Data
{
	char	szTitle[32];
	DWORD	dwID;
	BYTE	byType;
};

struct	MAPCMD_MS_MARRYEFFCHG_Data
{
	int		nEff;
	DWORD	dwID;
	BYTE	byType;
};

struct	MAPCMD_MS_CHGBTGROUP_Data
{
	BYTE	byType;
	DWORD	dwID;
	BYTE	bRandW;
	DWORD	dwMast;
};


struct	MAPCMD_MS_VIPLVCHG_Data
{
	BYTE	byViplv;
	DWORD	dwID;
	BYTE	byType;
};

struct	MAPCMD_MS_SPEEDCHG_Data
{
	DWORD	dwID;
	BYTE	byType;
	WORD	wSpeed;
};

struct	MAPCMD_MS_USERSYNC_Data
{
	
	DWORD	dwUin;
	//DWORD	dwMaster;
	WORD	wPosX,wPosY;
	//WORD	wDestX, wDestY;		//移动目标,X为0则Y表示方向
	char	szName[16];
	char	szTitle[22];
	char	szMarryTitle[32];
	int		nMarryEff;
	int		nHp;
	int		nHpMax;
	WORD	wSpeed;
	WORD	wSkin[3];

	BYTE	bCmd;
	DWORD	dwCmdVal;		//移动用高低位区分
	BYTE	bLv;
	BYTE	bSex;
	BYTE	bJob;
	WORD	bForce;
	BYTE	byCountry;
	BYTE	byViplv;
	BYTE	byStateCnt;
	WORD	wStateID[1];
};

typedef struct	MAPCMD_MS_ENEMYCMD_Data
{//怪物行动包
	DWORD	dwID;
	BYTE	bCmd;
	DWORD	dwCmdVal;
	DWORD	dwExParam;
}MAPCMD_MS_SLAVECMD_Data,MAPCMD_MS_USERCMD_Data;

struct	MAPCMD_MS_OBJOUT_Data
{//死亡或消失
	BYTE	byType;
	DWORD	dwMIndex;
	DWORD	dwID;
};
struct	MAPCMD_MS_SKILLCHG_Data
{
	BYTE	bPos;
	BYTE	bLv;
	WORD	wSkillID;
	int		nCDTick;
};

struct	MAPCMD_MS_CTRLINFO_Data
{//操控对象的血量,蓝量, 技能信息,快捷物品信息,坐标(进入地图和切换控制的时候需要)
	char	szName[16];
	DWORD	dwMIndex;
	WORD	wRMIndex;
	//WORD	wPosX;
	//WORD	wPosY;
	BYTE	byType;
	BYTE	byWeath;
	DWORD	dwID;
	WORD	wAtkDis;
	WORD	wSP;
	int		nHp;
	int		nHpMax;
	int		nMp;
	int		nMpMax;

	BYTE	bSCnt;
	MAPCMD_MS_SKILLCHG_Data	szSkill[1];
};

struct	MAPCMD_MS_STATEINFO_Data
{
	BYTE	bType;
	DWORD	dwID;
	BYTE	bCnt;
	WORD	szwStates[1];
};

struct	MAPCMD_MS_STATECHG_Data
{
	BYTE	bType;
	DWORD	dwID;
	WORD	wState;
};

struct	MAPCMD_MS_STATEDEL_Data
{
	BYTE	bType;
	DWORD	dwID;
	WORD	wState;
};

struct	_netHpInfo
{
	BYTE	bType;
	DWORD	dwID;
	int		nHp;
};

struct	MAPCMD_MS_HPINFO_Data
{
	BYTE	bType;
	DWORD	dwID;
	int		nHp;
};

struct	MAPCMD_MS_HPMAXINFO_Data
{
	BYTE	bType;
	DWORD	dwID;
	int		nMaxHp;
	int		nHp;
};

struct	MAPCMD_MS_USER_PARAMS_Data
{
	int		nHpMax;
	int		nMpMax;

	int		nAtkW;
	int		nAtkN;
	int		nDefW;
	int		nDefN;
	int		nAim;
	int		nDodge;
	int		nCrit;
	int		nMSpeed;
	int		nPvpJSDef;
};

struct	MAPCMD_MS_OBJCHGCTRL_Data
{
	BYTE	bType;
	WORD	bForce;
	DWORD	dwID;
	DWORD	dwMast;
};

struct	MAPCMD_MS_OBJCHGFORCE_Data
{
	BYTE	bType;
	DWORD	dwID;
	WORD	bForce;
};

struct	MAPCMD_MS_WEATHCHG_Data
{
	BYTE	byWeath;
};

struct	MAPCMD_MS_SPVAL_Data
{
	WORD	wSpVal;
};

struct	MAPCMD_MS_ACTSKILL_Data
{
	BYTE	bSCnt;
	MAPCMD_MS_SKILLCHG_Data	szSkill[1];
};

struct	MAPCMD_MS_PARAMCHG_Data
{
	BYTE	bType;
	DWORD	dwID;
	int		nHp;
	int		nMaxHp;
	BYTE	byLv;
	WORD	wSpeed;
	WORD	wSkin[3];
};

struct	MAPCMD_MS_MPINFO_Data
{
	int		nMp;
	int		nMaxMp;
};

struct	MAPCMD_MS_ATKINFO_Data
{
	BYTE	bTSrc;
	BYTE	bTDest;
	DWORD	dwIDSrc;
	DWORD	dwIDDest;
	WORD	wSkill;		//0普通攻击, 其他为技能ID
	BYTE	bModel;		//暴击,闪避?
	BYTE	bAoeHit;
	int		nDamage;		//血量值,客户端根据值变化弹出伤害量
};

struct	MAPCMD_MS_CESKILLCD_Data
{
	WORD	wSkill;
};

struct	MAPCMD_MS_HURTINFO_Data
{//范围技能其他受攻击的目标血量变化,区别MS_HPLIST不用冒数值
	WORD	wCnt;
	_netHpInfo	xHps[1];
};


struct	MAPCMD_C_SETCMD_Data
{
	BYTE	bType;
	DWORD	dwID;

	BYTE	bCmd;
	DWORD	dwCmdVal;
};

struct	MAPCMD_C_SETMOVE_Data
{//主控制目标
	WORD	wX;
	WORD	wY;
};

struct	MAPCMD_C_SETATK_Data
{
	BYTE	bType;
	DWORD	dwID;
	WORD	wSkill;
};

#pragma pack (pop)