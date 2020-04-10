#pragma once

#ifndef _ISSERVER
#include "GlobalDefine.h"
#endif

#define MAX_ACTIVE_SKILL	7

#pragma pack (push)
#pragma pack (1)

enum
{
	MS_ENEMYSYNC,		//������Ϣ��
	MS_SLAVESYNC,	//�����
	MS_USERSYNC,		//�����Ϣ

	MS_ENEMYCMD,		//��Ϊ����仯,�����뿪�����Ұ
	//MS_NPCCMD,			//����,�ܶ����ù�����
	MS_SLAVECMD,
	MS_USERCMD,			//�����ƶ���Ϣ

	MS_OBJOUT,

	MS_CTRLINFO,	//�ٿض����Ѫ��,����, ������Ϣ,�����Ʒ��Ϣ,����(�����ͼ���л����Ƶ�ʱ����Ҫ)
	MS_SKILLCHG,
	//MS_USKILL,			//����ż���
	MS_STATEINFO,		//״̬��Ϣ
	MS_STATECHG,		//״̬�ı����ʧ
	MS_STATEDEL,
	MS_HPINFO,			//Ѫ���ı���Ϣ�ϰ���

	MS_ATKINFO,			//������,��Ŀ���˺���Ϣֱ�Ӵ�
	MS_HURTINFO,		//��Χ���ܵ��˺���Ϣ����Ŀ�����

	//MS_POSLIST,			//λ����Ϣ�ϰ���

	//GS_CTRLON,
	//GS_CTRLOFF,		//�뿪

	C_SETCMD,		//��Ŀ����ָ��
	C_SETMOVE,		//�ͻ��������ƶ�Ŀ��
	C_SETATK,		//�ͻ��˷�������Ŀ��

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
	MS_CESKILLCD,		//�ٻ����ܲ�����MS_ATKINFO
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
	MCMD_NULL,		//��ָ��,����˲���, �ͻ���һ����Χ�����
	MCMD_FOLLOW,	//����һ��ָ��Ŀ��,Ҳ�����Ƕ����ƶ�		//ÿ��ͬ��
	MCMD_ATTACK,	//����,�������ض�Ŀ��	//ÿ��ͬ��
	MCMD_RUN,		//�ܲ�	
	
	MCMD_LOCKVIEW_ENEMY	= 10,
	MCMD_MAX,
};

struct	MAPCMD_MS_ENEMYSYNC_Data
{
	DWORD	dwID;
	WORD	wBaseID;
	int		nHp;		//Ѫ��
	WORD	wPosX,wPosY;
	BYTE	bCmd;
	WORD	wSpeed;
	DWORD	dwCmdVal;		//�ƶ��øߵ�λ����
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
	int		nHp;		//Ѫ��
	int		nMaxHp;		//Ѫ��
	WORD	wPosX,wPosY;
	BYTE	bCmd;
	BYTE	byLv;
	WORD	wSpeed;
	DWORD	dwCmdVal;		//�ƶ��øߵ�λ����
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
	DWORD	dwCmdVal;		//�ƶ��øߵ�λ����
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
	//WORD	wDestX, wDestY;		//�ƶ�Ŀ��,XΪ0��Y��ʾ����
	char	szName[16];
	char	szTitle[22];
	char	szMarryTitle[32];
	int		nMarryEff;
	int		nHp;
	int		nHpMax;
	WORD	wSpeed;
	WORD	wSkin[3];

	BYTE	bCmd;
	DWORD	dwCmdVal;		//�ƶ��øߵ�λ����
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
{//�����ж���
	DWORD	dwID;
	BYTE	bCmd;
	DWORD	dwCmdVal;
	DWORD	dwExParam;
}MAPCMD_MS_SLAVECMD_Data,MAPCMD_MS_USERCMD_Data;

struct	MAPCMD_MS_OBJOUT_Data
{//��������ʧ
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
{//�ٿض����Ѫ��,����, ������Ϣ,�����Ʒ��Ϣ,����(�����ͼ���л����Ƶ�ʱ����Ҫ)
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
	WORD	wSkill;		//0��ͨ����, ����Ϊ����ID
	BYTE	bModel;		//����,����?
	BYTE	bAoeHit;
	int		nDamage;		//Ѫ��ֵ,�ͻ��˸���ֵ�仯�����˺���
};

struct	MAPCMD_MS_CESKILLCD_Data
{
	WORD	wSkill;
};

struct	MAPCMD_MS_HURTINFO_Data
{//��Χ���������ܹ�����Ŀ��Ѫ���仯,����MS_HPLIST����ð��ֵ
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
{//������Ŀ��
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