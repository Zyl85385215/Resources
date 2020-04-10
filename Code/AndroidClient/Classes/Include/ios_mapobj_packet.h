#pragma once

#ifndef _ISSERVER
#include "GlobalDefine.h"
#endif

#pragma pack (push)
#pragma pack (1)

enum
{
	GS_ADDOBJBYCFG,		//��Ϸ���������Ӷ���ָ��,�ֶ�ˢ��
	GS_ADDOBJBYPAM,		//��ս��������ʽˢ��
	GS_REMOVEOBJ,		

	GS_SETCTRL,			//�趨���ƶ���,��������
	GS_CTRLOFF,		//��������ƶ���

	GS_MAPCOPY,			//��������
	GS_DELMAPCOPY,

	MS_OBJPOS,			//ʵʱͬ��λ����Ϣ(��������ͼ������,����ű�֮��)
	MS_OBJINFO,			//�뿪��ͼ�����ߵ�ʱ����Ҫͬ������״̬ (Ѫ������,����״̬)
	MS_OBJKILL,			//ɱ��,����Ϣ	(+����,+��Ʒ, +����,+��ѫ)
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
	//��ǰѪ��, ���Ѫ��,��ǰ����, �������, �﹥, ħ��, ���, ħ��, ����,����,����,�ƶ�
};

struct MAPOBJ_GS_ADDOBJBYCFG_Data 
{
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC
	BYTE	bForce;
	DWORD	dwID;
	WORD	wBase;		//��,����ID, ����NPC��ģ���
	WORD	wPosX;
	WORD	wPosY;
	WORD	wPowEx;
	char	szTitle[22];
};

struct _NetSkillInfo 
{
	WORD	wSkill;
	BYTE	byLv;
	BYTE	bPos;	//0δ����,1-4
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
	//DWORD	dwKey;		//�ĸ��������µ�,0 ��ʾû��
	char	szName[16];
	char	szTitle[22];
	char	szMarryTitle[32];
	int		nMarryEff;
	BYTE	byLv;
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC(�Ի���������Ϸ������)
	BYTE	bySex;
	BYTE	byJob;
	BYTE	byCountry;
	BYTE	byViplv;
	DWORD	dwID;		//ID
	WORD	wPosX;
	WORD	wPosY;
	MParam	xParam;		//����ս������
	WORD	wSkin[3];
	WORD	wHomeID;
	BYTE	bySCnt;
	_NetSkillInfo	szSkill[1];
};

struct MAPOBJ_GS_CHGPOS_Data 
{
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC(�Ի���������Ϸ������)
	DWORD	dwID;		//ID
	WORD	wX;
	WORD	wY;
};
#ifdef _ISSERVER
struct MAPOBJ_GS_SETRUNPATH_Data 
{
	DWORD	dwID;		//ֻ�����ù�
	WORD	wPtCnt;
	POINT	szPt[1];
};
#endif

struct MAPOBJ_GS_SETBATTLEGROUP_Data 
{
	DWORD	dwID;
	DWORD	dwMaster;
	BYTE	byPos;		//1��ʼ
};

struct MAPOBJ_GS_SETLOCKVIEW_Data 
{//��Ұ����
	BYTE	bLock;
	BYTE	bHideSelf;

	BYTE	byType;
	DWORD	dwID;
	BYTE	byObjType;
	DWORD	dwObjID;
};

struct MAPOBJ_GS_ADDSTATE_Data 
{
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC(�Ի���������Ϸ������)
	DWORD	dwID;		//ID
	WORD	wState;
	BYTE	bRemove;
};

struct MAPOBJ_GS_CLEAN_RIDE_LOCK_Data 
{
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC(�Ի���������Ϸ������)
	DWORD	dwID;		//ID
};

struct MAPOBJ_GS_SYNCPARAM_Data 
{
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC(�Ի���������Ϸ������)
	DWORD	dwID;		//ID
	BYTE	byLv;
	MParam	xParam;		//����ս������
	WORD	wSkin[3];
};

struct MAPOBJ_GS_SYNCSKILL_Data 
{
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC(�Ի���������Ϸ������)
	DWORD	dwID;		//ID
	BYTE	bySCnt;
	_NetSkillInfo	szSkill[1];
};

struct MAPOBJ_GS_SYNCTITLE_Data 
{
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC(�Ի���������Ϸ������)
	DWORD	dwID;
	char	szTitle[22];
};

struct MAPOBJ_GS_SYNCMARRYTITLE_Data 
{
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC(�Ի���������Ϸ������)
	DWORD	dwID;
	char	szTitle[32];
};

struct MAPOBJ_GS_SYNCMARRYEFF_Data 
{
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC(�Ի���������Ϸ������)
	DWORD	dwID;
	int		nEff;
};

struct MAPOBJ_GS_SYNCVIPLV_Data 
{
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC(�Ի���������Ϸ������)
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
	DWORD	dwObj;		//���UIN
};

struct MAPOBJ_GS_KILL_MAPENEMY_BYID_Data 
{
	DWORD	dwMapIndex;
	WORD	wEnemyID;
};

struct MAPOBJ_GS_HEALTHHPMP_Data 
{
	BYTE	byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC(�Ի���������Ϸ������)
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
	DWORD	dwUin;		//���ڻر������ż�������Ϣ
	DWORD	dwMapIndex;
	WORD	wRealMIndex;		//ģ��
	BYTE	bType;			//�����, ��Ҹ���������, ��ϵ��������ʱ��
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
	DWORD	dwKey;		//�ĸ��������µ�,0 ��ʾû��
	int		wMapIndex;
	BYTE	bControl;	//0ȡ������, 1ָ���Ϳ���, 10, �������� 11, ��Ұ����(10,11ֻ��ѡ��һ)
	BYTE	bType;
	DWORD	dwID;
	WORD	wFury;
};

struct MAPOBJ_GS_SETENEMY_Data 
{
	BYTE	bActType;	// 0ȡ��, 1 ����, 2����, 3����, 9����
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
	BYTE	byTargetT;		//����		����, ����
	DWORD	dwTargetID;		//ģ���	ID
	WORD	wBaseID;
};
struct MAPOBJ_MS_OBJBEKILL_Data 
{
	BYTE	byType;		//����		����, ����
	DWORD	dwID;		//ģ���	ID
	BYTE	byAtkT;		//����		����, ����
	DWORD	dwAtkID;		//ģ���	ID
};
struct MAPOBJ_MS_OBJINFO_Data 
{
	BYTE	byType;		//����		����, ����
	DWORD	dwID;		//ģ���	ID

	int		nHp;		//Ϊ0����
	int		nMp;
	WORD	wSp;
};

struct MAPOBJ_MS_OBJBEHIT_Data 
{
	BYTE	byType;		//����		����, ����
	DWORD	dwID;		//ģ���	ID
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