#pragma once

#ifndef _ISSERVER
#include "GlobalDefine.h"
#endif

#pragma pack (push)
#pragma pack (1)

enum
{
	GS_ADDOBJBYCFG,		//游戏服务器增加对象指令,手动刷怪
	GS_ADDOBJBYPAM,		//以战斗参数形式刷怪
	GS_REMOVEOBJ,		

	GS_SETCTRL,			//设定控制对象,无则新增
	GS_CTRLOFF,		//清除所控制对象

	GS_MAPCOPY,			//创建副本
	GS_DELMAPCOPY,

	MS_OBJPOS,			//实时同步位置信息(用于跳地图和任务,区域脚本之类)
	MS_OBJINFO,			//离开地图或下线的时候需要同步对象状态 (血量蓝量,特殊状态)
	MS_OBJKILL,			//杀怪,人信息	(+经验,+物品, +任务,+功勋)
	MS_OBJBEKILL,

	GS_SYNCPARAM,
	GS_CHGPOS,

	MS_OBJBEHIT,

	GS_SYNCSKILL,
	GS_HEALTHHPMP,

	MS_SETREVIVE,
	GS_SYNCTITLE,

	MS_SYNCSTATE,
	GS_INITSTATE,

	GS_SETENEMY,

	GS_ADDSTATE,

	MS_ENEMYDEAD,

	GS_INITCTRL,
	GS_SYNCVIPLV,

	GS_MAPLOCKATK,

	GS_KILLENEMY,

	MS_BOSSBORN,
	MS_BOSSDEAD,

	GS_CHGMFORCE,
	GS_CHGMPOW,
	GS_SYNCMARRYTITLE,
	GS_SYNCMARRYEFF,
	GS_CHGWEATHER_TIME,

	GS_SETRUNPATH,
	GS_SETBATTLEGROUP,

	GS_SETLOCKVIEW,

	MS_SETSKILLCD,

	MS_LICHKILLED,

	GS_CLEAN_RIDE_LOCK,

	GS_KILL_MAPENEMY_BYID,

	MS_ENEMY_BEKILL,

	GS_RESET_OBJSKIN,
};

enum
{
	MAPOBJ_NULL,
	MAPOBJ_PLAYER,
	MAPOBJ_SLAVE,
	MAPOBJ_ENEMY,
	MAPOBJ_NPC,
	MAPOBJ_ROBOT,
};
const int nLichBossID = 7043;
struct MParam
{
	int		GetParam(BYTE bP)
	{
		if(bP >= 12)
			return 0;
		int*	pRtl	= &nHp;
		return *(pRtl+bP);
	}
	void		SetParam(BYTE bP,int nVal)
	{
		if(bP >= 12)
			return;
		int*	pRtl	= &nHp;
		*(pRtl+bP)	= nVal;
	}
	void		AddParam(BYTE bP,int nVal)
	{
		if(bP >= 12)
			return;
		int*	pRtl	= &nHp;
		*(pRtl+bP)	+= nVal;
	}
	int		nHp;
	int		nHpMax;
	int		nMp;
	int		nMpMax;

	int		nAtkW;
	int		nAtkN;
	int		nDefW;
	int		nDefN;
	int		nAim;
	int		nDodge;
	int		nCrit;
	int		nMSpeed;
	int		nPvpDef;
	//当前血量, 最大血量,当前蓝量, 最大蓝量, 物攻, 魔攻, 物防, 魔防, 命中,闪避,暴击,移动
};

struct MAPOBJ_GS_ADDOBJBYCFG_Data 
{
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC
	BYTE	bForce;
	DWORD	dwID;
	WORD	wBase;		//人,宠是ID, 怪物NPC是模板号
	WORD	wPosX;
	WORD	wPosY;
	WORD	wPowEx;
	char	szTitle[22];
};

struct _NetSkillInfo 
{
	WORD	wSkill;
	BYTE	byLv;
	BYTE	bPos;	//0未设置,1-4
	DWORD	dwCDTick;
};

struct	_netStateInfo
{
	WORD	wStateID;
	BYTE	bLvl;
	DWORD	dwTickTime;
};

struct MAPOBJ_GS_CHGWEATHER_TIME_Data 
{
	BYTE byWeather;
	int	nSec;
};

struct MAPOBJ_MS_SYNCSTATE_Data 
{
	_netStateInfo	xState;
};

struct MAPOBJ_GS_INITSTATE_Data 
{
	BYTE	bType;
	DWORD	dwID;
	WORD	wCnt;
	_netStateInfo	xState[1];
};

struct MAPOBJ_GS_ADDOBJBYPAM_Data 
{
	//DWORD	dwKey;		//哪个控制器下的,0 表示没有
	char	szName[16];
	char	szTitle[22];
	char	szMarryTitle[32];
	int		nMarryEff;
	BYTE	byLv;
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC(对话还是在游戏服务器)
	BYTE	bySex;
	BYTE	byJob;
	BYTE	byCountry;
	BYTE	byViplv;
	DWORD	dwID;		//ID
	WORD	wPosX;
	WORD	wPosY;
	MParam	xParam;		//具体战斗参数
	WORD	wSkin[3];
	WORD	wHomeID;
	BYTE	bySCnt;
	_NetSkillInfo	szSkill[1];
};

struct MAPOBJ_GS_CHGPOS_Data 
{
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC(对话还是在游戏服务器)
	DWORD	dwID;		//ID
	WORD	wX;
	WORD	wY;
};
#ifdef _ISSERVER
struct MAPOBJ_GS_SETRUNPATH_Data 
{
	DWORD	dwID;		//只能设置怪
	WORD	wPtCnt;
	POINT	szPt[1];
};
#endif

struct MAPOBJ_GS_SETBATTLEGROUP_Data 
{
	DWORD	dwID;
	DWORD	dwMaster;
	BYTE	byPos;		//1开始
};

struct MAPOBJ_GS_SETLOCKVIEW_Data 
{//视野锁定
	BYTE	bLock;
	BYTE	bHideSelf;

	BYTE	byType;
	DWORD	dwID;
	BYTE	byObjType;
	DWORD	dwObjID;
};

struct MAPOBJ_GS_ADDSTATE_Data 
{
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC(对话还是在游戏服务器)
	DWORD	dwID;		//ID
	WORD	wState;
	BYTE	bRemove;
};

struct MAPOBJ_GS_CLEAN_RIDE_LOCK_Data 
{
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC(对话还是在游戏服务器)
	DWORD	dwID;		//ID
};

struct MAPOBJ_GS_SYNCPARAM_Data 
{
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC(对话还是在游戏服务器)
	DWORD	dwID;		//ID
	BYTE	byLv;
	MParam	xParam;		//具体战斗参数
	WORD	wSkin[3];
};

struct MAPOBJ_GS_SYNCSKILL_Data 
{
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC(对话还是在游戏服务器)
	DWORD	dwID;		//ID
	BYTE	bySCnt;
	_NetSkillInfo	szSkill[1];
};

struct MAPOBJ_GS_SYNCTITLE_Data 
{
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC(对话还是在游戏服务器)
	DWORD	dwID;
	char	szTitle[22];
};

struct MAPOBJ_GS_SYNCMARRYTITLE_Data 
{
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC(对话还是在游戏服务器)
	DWORD	dwID;
	char	szTitle[32];
};

struct MAPOBJ_GS_SYNCMARRYEFF_Data 
{
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC(对话还是在游戏服务器)
	DWORD	dwID;
	int		nEff;
};

struct MAPOBJ_GS_SYNCVIPLV_Data 
{
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC(对话还是在游戏服务器)
	DWORD	dwID;
	BYTE	byViplv;
};

struct MAPOBJ_GS_MAPLOCKATK_Data 
{
	DWORD	dwMapIndex;
	BYTE	bLock;
};

struct MAPOBJ_GS_CHGMFORCE_Data 
{
	int		nFType;
};
struct MAPOBJ_GS_CHGMPOW_Data 
{
	int		nPow;
	int		nLv;
};

struct MAPOBJ_MS_BOSSBORN_Data 
{
	DWORD	dwMapIndex;
	WORD	wBossID;
	bool    bWild;
};

struct MAPOBJ_MS_SETSKILLCD_Data 
{
	WORD	wSkillID;
	DWORD	dwTickCD;
};

struct MAPOBJ_MS_LICHKILLED_Data 
{
	int		nUin[11];
	int		nDamage[11];
};

struct MAPOBJ_MS_BOSSDEAD_Data 
{
	DWORD	dwMapIndex;
	WORD	wBossID;
};

struct MAPOBJ_GS_KILLENEMY_Data 
{
	DWORD	dwMapIndex;
	WORD	wX;
	WORD	wY;
	WORD	wRange;
	DWORD	dwObj;		//玩家UIN
};

struct MAPOBJ_GS_KILL_MAPENEMY_BYID_Data 
{
	DWORD	dwMapIndex;
	WORD	wEnemyID;
};

struct MAPOBJ_GS_HEALTHHPMP_Data 
{
	BYTE	byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC(对话还是在游戏服务器)
	DWORD	dwID;		//ID
	int		nAddHp;
	int		nAddMp;
	BYTE	bRevive;
};

struct MAPOBJ_GS_REMOVEOBJ_Data 
{
	BYTE	byType;
	DWORD	dwID;
};

struct MAPOBJ_GS_RESET_OBJSKIN_Data 
{
	BYTE	byType;
	DWORD	dwID;
};

struct MAPOBJ_GS_MAPCOPY_Data 
{
	DWORD	dwUin;		//用于回报副本号及其他信息
	DWORD	dwMapIndex;
	WORD	wRealMIndex;		//模板
	BYTE	bType;			//活动副本, 玩家副本等类型, 关系过期销毁时间
	WORD	wExPow;
	BYTE	byLv;
};

struct MAPOBJ_GS_DELMAPCOPY_Data 
{
	DWORD	dwMIndex;		
};

struct MAPOBJ_GS_INITCTRL_Data 
{
	DWORD	dwKey;
};

struct MAPOBJ_GS_SETCTRL_Data 
{
	DWORD	dwKey;		//哪个控制器下的,0 表示没有
	int		wMapIndex;
	BYTE	bControl;	//0取消控制, 1指令型控制, 10, 操作控制 11, 视野控制(10,11只能选其一)
	BYTE	bType;
	DWORD	dwID;
	WORD	wFury;
};

struct MAPOBJ_GS_SETENEMY_Data 
{
	BYTE	bActType;	// 0取消, 1 被动, 2主动, 3跟随, 9攻击
	DWORD	dwID;

	BYTE	bTagType;
	DWORD	dwTagID;
};

struct MAPOBJ_GS_CTRLOFF_Data 
{
	DWORD	dwKey;		
};

struct MAPOBJ_MS_OBJPOS_Data 
{
	BYTE	byType;
	DWORD	dwID;
	DWORD	dwMIndex;
	WORD	wPosX;
	WORD	wPosY;
};

struct MAPOBJ_MS_OBJKILL_Data 
{
	BYTE	bType;
	DWORD	dwID;
	BYTE	byTargetT;		//怪物		宠物, 人物
	DWORD	dwTargetID;		//模板号	ID
	WORD	wBaseID;
};
struct MAPOBJ_MS_OBJBEKILL_Data 
{
	BYTE	byType;		//怪物		宠物, 人物
	DWORD	dwID;		//模板号	ID
	BYTE	byAtkT;		//怪物		宠物, 人物
	DWORD	dwAtkID;		//模板号	ID
};
struct MAPOBJ_MS_OBJINFO_Data 
{
	BYTE	byType;		//怪物		宠物, 人物
	DWORD	dwID;		//模板号	ID

	int		nHp;		//为0死亡
	int		nMp;
	WORD	wSp;
};

struct MAPOBJ_MS_OBJBEHIT_Data 
{
	BYTE	byType;		//怪物		宠物, 人物
	DWORD	dwID;		//模板号	ID
};

struct MAPOBJ_MS_ENEMYDEAD_Data 
{
	int		nMIndex;
	WORD	wBaseMI;
	WORD	wBaseID;
	BYTE	byType;
	DWORD	dwID;
};

struct MAPOBJ_MS_SETREVIVE_Data 
{
	WORD	wRM;
	WORD	wRX;
	WORD	wRY;
};

struct MAPOBJ_MS_ENEMY_BEKILL_Data 
{
	DWORD  dwMapIndex;
	WORD   wEnemyID;
};

#pragma pack (pop)