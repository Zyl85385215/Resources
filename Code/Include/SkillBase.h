#ifndef _SKILL_BASE_H_
#define _SKILL_BASE_H_
#include <windows.h>
#include "xnlist.h"
#include "skill_packet.h"
//////////////////////////////////////////////////////////////////
#define WriteData(member)\
	fwrite(&(member),sizeof(member),1,pFile);

#define Writestring(member)\
	if (member)\
	{\
		fwrite(member,1,strlen(member)+1,pFile);\
	}\
	else\
	{\
		char aaaa=0;\
		fwrite(&aaaa,1,1,pFile);\
	}
#define LoadData(member)\
	memcpy(&member,Data+readpos,sizeof(member));\
	readpos+=sizeof(member)

#define LoadDataByType(t,member)\
	memcpy(&member,Data+readpos,sizeof(t));\
	readpos+=sizeof(t)

#define Loadstring(member)\
	if (member)\
	{\
		free(member);\
	}\
	member=strdup((LPCSTR)(Data+readpos));\
	readpos+=strlen(member)+1

enum E_AREAKEYPOINT
{
	A_SINGLE_TARGET=1,
	A_CHARACTER_CENTER,
	A_MOUSE_DOWN,
	//A_SELECT_OBJ,
};

enum E_AREATYPE
{
	A_CIRCLE=1,
	A_RECTANGLE,
	A_DIR_RECT
};

enum E_SKILLPOWERPARAM
{
	E_SKILLPOWERPARAM_NONE,
	E_S_LEVEL,
	E_S_APN,
	E_S_APW,
	E_S_ATKN,
	E_S_ATKW,
	E_S_ATKSPEED,
	E_S_MOVESPEED,
	E_S_DEFN,
	E_S_DEFW,
	E_S_DPN,
	E_S_DPW,
	E_S_MAXHP,
	E_S_MAXMP,
	E_S_PPN,
	E_S_PPW,
	E_S_HP,
	E_S_MP,
	E_S_STR,
	E_S_CON,
	E_S_ING,
	E_S_DEX,
	E_S_WIS,
	E_S_LUCKY,
	E_S_ATKYY,
	E_S_DEFYY,
	E_S_ATKWN,
};

enum E_SKILL_TYPE_2
{
	S_TYPE_2_PRO,
	S_TYPE_2_LIFE,
	S_TYPE_2_FACTION,
	S_TYPE_2_PRACTICE,
	S_TYPE_2_SPECIAL,
	S_TYPE_2_BOOK,
	S_TYPE_2_MAKE,
	S_TYPE_2_SECOND_LEVEL,

	MAX_SKILL_TYPE_2,
};

enum E_CLASSTYPE
{
	E_CLASSTYPE_NONE,
	E_C_GENERAL,
	E_C_KNIGHT,
	E_C_ROBBER,
	E_C_ALCHEMY
};

enum E_WEAPONTYPE
{
	E_WEAPONTYPE_NONE,
	E_W_TWOHAND_WEAPON,
	E_W_ONEHAND_WEAPON,
	E_W_NO_WEAPON,
	E_W_OFFHAND_WEAPON
};

enum E_MOVESTATE
{
	E_MOVESTATE_NONE,	
	E_M_WALK,
	E_M_RUN,
	E_M_RIDE
};

enum E_TARGETTYPE
{
	E_TARGETTYPE_NONE,
	E_T_PET,
	E_T_PLAYER,
	E_T_MONSTER,
	E_T_SELF,
	E_T_BOSS,
	E_T_SLAVE,
};

enum E_RELATIONSHIP
{
	E_RELATIONSHIP_NONE,
	E_R_FRIENDLY,//�Ѻ�
	E_R_HOSTILE,//�ж�
	E_R_PARTYMEMBER,//����
	E_R_COUPLE,//����
	E_R_TEACHERHOOD,//ʦͽ
	E_R_BROTHERHOOD//����
};

enum E_SITFLAG
{
	E_SITFLAG_NONE,
	E_S_TARGET_DOWNHORSE,
	E_S_SELF_RIDEHORSE,
	E_S_SELF_SITDOWN,
	E_S_SELF_STAND
};

enum E_SKILL_LEVELUP_TYPE
{
	SLEVELUP_POINT,
	SLEVELUP_EXP,
	SLEVELUP_OTHER,
};

enum E_SKILL_RIDE_STATUS
{
	SKILL_RIDE_STATUS_NULL	= 0x00,	//	��Ч
	SKILL_RIDE_STATUS_WALK	= 0x01,	//	����
	SKILL_RIDE_STATUS_RUN	= 0x02,	//	½������
	SKILL_RIDE_STATUS_FLY	= 0x04,	//	��������
};

//for load from different version of skill file
enum E_VERSION
{
	ORIVERSION=0x11111111,
	VER_1,
	VER_2,
	VER_3,
	VER_4,
	VER_5,
	VER_6,
	VER_7,
	VER_8,
	VER_9,
	VER_10,
	VER_11,
	VER_12,
	VER_13,
	VER_14,
	VER_15,
	E_VERSION_MAX
};

#define VERS VER_14
#define NEW_SKILL

class SkillInfo;
typedef SkillInfo * LPSKILL;
typedef int (*PLOADFUNC)(BYTE * pData,int nSize,LPSKILL*& pResult);
extern PLOADFUNC loadfromversion[E_VERSION_MAX-ORIVERSION];

class SkillInfo
{
public:	
	static DWORD version;
	static DWORD count;
	static int LoadAllSkill(char * szname,LPSKILL*& pResult);

/************************************************************************/
/* basic skill information                                              */
/************************************************************************/
	char*		szName;
	int		wID;
	BYTE		bLevel;
	E_SKILLTYPE		bType;
	int		bType2;
	int		wIcon;

	int		wReadyEffect,wAtkEffect,wFlyEffect,wHurtEffect;
	char*		szReadySound,*szAtkSound,*szFlySound,*szHurtSound;

	char*		szDesc;
/************************************************************************/
/* skill attribute info                                                 */
/************************************************************************/
	DWORD		dwReadyTime;
	char*		szAction;
	DWORD		dwDelay;
	BYTE		bDelayType;
	
	DWORD   dwKeep_Interval;
	DWORD   dwKeep_Num;
	int		wDistance_Max,wDistance_Min;
	E_AREAKEYPOINT    bArea_KeyPoint;
	E_AREATYPE    bArea_Type;
	DWORD		bArea_param1,bArea_param2;
	DWORD		bArea_param1PerTalent;
	BYTE		bAtkCnt;

	int    SkillPower_Const;
	E_SKILLPOWERPARAM    SkillPower_AttriType;
	float   SkillPower_MulParam;
	DWORD		bFormulaIndex;

	DWORD   dwConjureItem;
	int		dwConjureItemCount;
	DWORD   dwSummonCreatureID,dwSummonCount;
	
	DWORD   dwJumpIndex,dwJumpX,dwJumpY;
	BYTE    bScreenEffect;
/************************************************************************/
/* the restrict of skill learn and use                                       */
/************************************************************************/
	int		nMaxTalentPoint;		      //������������
	int		nEffectPerTalentPoint;		  //ÿ����������Ч���ӳ�
	int		nCastTimePerTalent;			  //ÿ���������������ӳ�
	int		nCoolTimePerTalent;			  //ÿ������������ȴ�ӳ�
	int		nDistancePerTalent;			  //ÿ��������������ӳ�
	float	fHpPerTalent;				  //ÿ����������HP��������
	float	fMpPerTalent;				  //ÿ����������MP��������
	float	fParamPerTalent;			  //���ܹ�ʽ�����Ա��ʵ�ÿ�����������ӳ�
	int		nActNeedSkill1;
	int		nActNeedSkill2;
	int		nActNeedTalent1;
	int		nActNeedTalent2;
	DWORD   dwLearnCost;   //�¼�����
	BYTE		bNeedCharLvl;
	int		wNeedSkill;   //�ϼ�����
	BYTE		bCanLearnFromPlayer;
	BYTE    bNeedClass;			//job
	DWORD   dwUseInStateFlag;
	int		wNeedStr,wNeedCon,wNeedIng,wNeedWis,wNeedDex;
	int		wNeedLuk;   //��ʼ����
	BYTE    bNeedWeapon,bNeedMoveState;
	float    wNeedMP,wNeedHP; // if <1,is a percent value
	int    wNeedItemID,wNeedItemCount;
	int    wNeedLearnItemID;
	BYTE		bNeedTargetLevel_Max,bNeedTargetLevel_Min;
	BYTE    bTargetType,bTargetRelation;
	DWORD    wNeedExp;           //�����¼���������������
	DWORD	 wNeedPoint;		 //���������������������

	BYTE	bUseSkillMsg;		//�Ƿ�ʹ�ü�����Ϣ
	char*	szSkillMsg;			//������Ϣ
/************************************************************************/
/* skill effect                                                                      */
/************************************************************************/
	DWORD  SelfGainBuff[3],SelfRemoveBuff[3],SelfRemoveFlag;char* SelfChgACT;
	DWORD  TargetGainBuff[3],TargetRemoveBuff[3],TargetRemoveFlag;char* TargetChgACT;
	int SelfBuffRatio[3],SelfBuffRatioPerTalent[3];
	int TargetBuffRatio[3],TargetBuffRatioPerTalent[3];
	int	 SelfStatePerTalent[3],TargetStatePerTalent[3];
	float	 TargetHpChg,TargetMpChg; // if <1,is a percent value
	BYTE   bSitFlag;

	char * SCRIPT;
	char * CheckScript;
	BYTE * m_pbScript;
	BYTE * m_pbCheckScript;

	char * m_szTalentTxt;
	char * m_szSkillAttackEventLuaScript;

	BYTE	 bIgnoreBlock;
	int		HateValue;
	BYTE	bStrengthenType;
	float	fStrengthenVal;
	BYTE	bLevelUpType;

	int		GainBuffOnLearn;
	int		GainBuffToSlaveOnLearn;
	int		GainBuffToPlayerOnLearn;

	int		UpExp;
	int		UpExpLvl;
	int		UpExpPower;
	int		UpMoney;
	int		UpMoneyLvl;
	int		UpMoneyPower;
	int		UpContribution;
	int		UpContributionLvl;
	int		UpContributionPower;

	BYTE	bSelfGainBuffRatio;
	BYTE	bSelfGainBuffRatioLvl;
	BYTE	bTarGainBuffRatio;
	BYTE	bTarGainBuffRatioLvl;

	bool	m_bLock;

	int		nFuryStar;
	int		nHateVal;
	bool	bInFury;
	BYTE	m_byRideStatus;
/************************************************************************/
/* member define end,fuction define begin                               */
/************************************************************************/
	SkillInfo();
	~SkillInfo();
	void SetName(const char* szStr);
	void SetReadySound(const char* szStr);
	void SetAtkSound(const char* szStr);
	void SetHurtSound(const char* szStr);
	void SetFlySound(const char *szStr);
	void SetAction(const char* szStr);
	void SetSelfChgACT(const char* szStr);
	void SetTargetChgACT(const char* szStr);
	void SetScript(const char * pScript);
	void SetDesc(const char* szStr,bool blIn = true);
	int Load(BYTE * Data);
	int Load1(BYTE * Data);
	SkillInfo & operator=(SkillInfo & pinfo);

	virtual char*	GetName() { return szName; }
	void SetCheckScript( const char * pScript);

	DWORD				GetReadyTime(int nTal);
	DWORD				GetCoolTime(int nTal);
	int					GetMaxDistance(int nTal);
	float				GetNeedHp(int nTal);
	float				GetNeedMp(int nTal);
};

static int LOAD_ORIVERSION(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	pResult=new SkillInfo* [0xFFFF];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load(Data+readpos);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}
static int LOAD_VER_1(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}
static int LOAD_VER_2(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}
static int LOAD_VER_3(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}

static int LOAD_VER_4(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222		
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}

static int LOAD_VER_5(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222	

		LoadData(pInfo->GainBuffOnLearn);	
		LoadData(pInfo->GainBuffToSlaveOnLearn);	
		LoadData(pInfo->GainBuffToPlayerOnLearn);

		LoadData(pInfo->UpExp);	
		LoadData(pInfo->UpExpLvl);
		LoadData(pInfo->UpExpPower);
		LoadData(pInfo->UpMoney);	
		LoadData(pInfo->UpMoneyLvl);
		LoadData(pInfo->UpMoneyPower);
		LoadData(pInfo->UpContribution);	
		LoadData(pInfo->UpContributionLvl);
		LoadData(pInfo->UpContributionPower);

		LoadData(pInfo->bSelfGainBuffRatio);
		LoadData(pInfo->bSelfGainBuffRatioLvl);
		LoadData(pInfo->bTarGainBuffRatio);
		LoadData(pInfo->bTarGainBuffRatioLvl);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}

static int LOAD_VER_6(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222

		LoadData(pInfo->GainBuffOnLearn);	
		LoadData(pInfo->GainBuffToSlaveOnLearn);	
		LoadData(pInfo->GainBuffToPlayerOnLearn);

		LoadData(pInfo->UpExp);	
		LoadData(pInfo->UpExpLvl);
		LoadData(pInfo->UpExpPower);
		LoadData(pInfo->UpMoney);	
		LoadData(pInfo->UpMoneyLvl);
		LoadData(pInfo->UpMoneyPower);
		LoadData(pInfo->UpContribution);	
		LoadData(pInfo->UpContributionLvl);
		LoadData(pInfo->UpContributionPower);
		LoadData(pInfo->bType2);

		LoadData(pInfo->bSelfGainBuffRatio);
		LoadData(pInfo->bSelfGainBuffRatioLvl);
		LoadData(pInfo->bTarGainBuffRatio);
		LoadData(pInfo->bTarGainBuffRatioLvl);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}

static int LOAD_VER_7(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222

		LoadData(pInfo->GainBuffOnLearn);	
		LoadData(pInfo->GainBuffToSlaveOnLearn);	
		LoadData(pInfo->GainBuffToPlayerOnLearn);

		LoadData(pInfo->UpExp);	
		LoadData(pInfo->UpExpLvl);
		LoadData(pInfo->UpExpPower);
		LoadData(pInfo->UpMoney);	
		LoadData(pInfo->UpMoneyLvl);
		LoadData(pInfo->UpMoneyPower);
		LoadData(pInfo->UpContribution);	
		LoadData(pInfo->UpContributionLvl);
		LoadData(pInfo->UpContributionPower);
		LoadData(pInfo->bType2);

		LoadData(pInfo->bSelfGainBuffRatio);
		LoadData(pInfo->bSelfGainBuffRatioLvl);
		LoadData(pInfo->bTarGainBuffRatio);
		LoadData(pInfo->bTarGainBuffRatioLvl);

		LoadData(pInfo->nFuryStar);
		LoadData(pInfo->bInFury);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}

static int LOAD_VER_8(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222

		LoadData(pInfo->GainBuffOnLearn);	
		LoadData(pInfo->GainBuffToSlaveOnLearn);	
		LoadData(pInfo->GainBuffToPlayerOnLearn);

		LoadData(pInfo->UpExp);	
		LoadData(pInfo->UpExpLvl);
		LoadData(pInfo->UpExpPower);
		LoadData(pInfo->UpMoney);	
		LoadData(pInfo->UpMoneyLvl);
		LoadData(pInfo->UpMoneyPower);
		LoadData(pInfo->UpContribution);	
		LoadData(pInfo->UpContributionLvl);
		LoadData(pInfo->UpContributionPower);
		LoadData(pInfo->bType2);
		LoadData(pInfo->nMaxTalentPoint);
		LoadData(pInfo->nEffectPerTalentPoint);
		LoadData(pInfo->nCastTimePerTalent);
		LoadData(pInfo->nCoolTimePerTalent);
		LoadData(pInfo->nDistancePerTalent);
		LoadData(pInfo->fHpPerTalent);
		LoadData(pInfo->fMpPerTalent);
		LoadData(pInfo->fParamPerTalent);

		LoadData(pInfo->bSelfGainBuffRatio);	
		LoadData(pInfo->bSelfGainBuffRatioLvl);
		LoadData(pInfo->bTarGainBuffRatio);
		LoadData(pInfo->bTarGainBuffRatioLvl);

		LoadData(pInfo->nFuryStar);
		LoadData(pInfo->bInFury);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}

static int LOAD_VER_9(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222

		LoadData(pInfo->GainBuffOnLearn);	
		LoadData(pInfo->GainBuffToSlaveOnLearn);	
		LoadData(pInfo->GainBuffToPlayerOnLearn);

		LoadData(pInfo->UpExp);	
		LoadData(pInfo->UpExpLvl);
		LoadData(pInfo->UpExpPower);
		LoadData(pInfo->UpMoney);	
		LoadData(pInfo->UpMoneyLvl);
		LoadData(pInfo->UpMoneyPower);
		LoadData(pInfo->UpContribution);	
		LoadData(pInfo->UpContributionLvl);
		LoadData(pInfo->UpContributionPower);
		LoadData(pInfo->bType2);
		LoadData(pInfo->nMaxTalentPoint);
		LoadData(pInfo->nEffectPerTalentPoint);
		LoadData(pInfo->nCastTimePerTalent);
		LoadData(pInfo->nCoolTimePerTalent);
		LoadData(pInfo->nDistancePerTalent);
		LoadData(pInfo->fHpPerTalent);
		LoadData(pInfo->fMpPerTalent);
		LoadData(pInfo->fParamPerTalent);
		LoadData(pInfo->nActNeedSkill1);
		LoadData(pInfo->nActNeedSkill2);
		LoadData(pInfo->nActNeedTalent1);
		LoadData(pInfo->nActNeedTalent2);
		Loadstring(pInfo->CheckScript);

		LoadData(pInfo->bSelfGainBuffRatio);	
		LoadData(pInfo->bSelfGainBuffRatioLvl);
		LoadData(pInfo->bTarGainBuffRatio);
		LoadData(pInfo->bTarGainBuffRatioLvl);

		LoadData(pInfo->nFuryStar);
		LoadData(pInfo->bInFury);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}

static int LOAD_VER_10(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222

		LoadData(pInfo->GainBuffOnLearn);	
		LoadData(pInfo->GainBuffToSlaveOnLearn);	
		LoadData(pInfo->GainBuffToPlayerOnLearn);

		LoadData(pInfo->UpExp);	
		LoadData(pInfo->UpExpLvl);
		LoadData(pInfo->UpExpPower);
		LoadData(pInfo->UpMoney);	
		LoadData(pInfo->UpMoneyLvl);
		LoadData(pInfo->UpMoneyPower);
		LoadData(pInfo->UpContribution);	
		LoadData(pInfo->UpContributionLvl);
		LoadData(pInfo->UpContributionPower);
		LoadData(pInfo->bType2);
		LoadData(pInfo->nMaxTalentPoint);
		LoadData(pInfo->nEffectPerTalentPoint);
		LoadData(pInfo->nCastTimePerTalent);
		LoadData(pInfo->nCoolTimePerTalent);
		LoadData(pInfo->nDistancePerTalent);
		LoadData(pInfo->fHpPerTalent);
		LoadData(pInfo->fMpPerTalent);
		LoadData(pInfo->fParamPerTalent);
		LoadData(pInfo->nActNeedSkill1);
		LoadData(pInfo->nActNeedSkill2);
		LoadData(pInfo->nActNeedTalent1);
		LoadData(pInfo->nActNeedTalent2);

		LoadData(pInfo->SelfBuffRatio[0]);
		LoadData(pInfo->SelfBuffRatio[1]);
		LoadData(pInfo->SelfBuffRatio[2]);
		LoadData(pInfo->SelfBuffRatioPerTalent[0]);
		LoadData(pInfo->SelfBuffRatioPerTalent[1]);
		LoadData(pInfo->SelfBuffRatioPerTalent[2]);
		LoadData(pInfo->TargetBuffRatio[0]);
		LoadData(pInfo->TargetBuffRatio[1]);
		LoadData(pInfo->TargetBuffRatio[2]);
		LoadData(pInfo->TargetBuffRatioPerTalent[0]);
		LoadData(pInfo->TargetBuffRatioPerTalent[1]);
		LoadData(pInfo->TargetBuffRatioPerTalent[2]);

		Loadstring(pInfo->CheckScript);

		LoadData(pInfo->bSelfGainBuffRatio);	
		LoadData(pInfo->bSelfGainBuffRatioLvl);
		LoadData(pInfo->bTarGainBuffRatio);
		LoadData(pInfo->bTarGainBuffRatioLvl);

		LoadData(pInfo->nFuryStar);
		LoadData(pInfo->bInFury);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}

static int LOAD_VER_11(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222

		LoadData(pInfo->GainBuffOnLearn);	
		LoadData(pInfo->GainBuffToSlaveOnLearn);	
		LoadData(pInfo->GainBuffToPlayerOnLearn);

		LoadData(pInfo->UpExp);	
		LoadData(pInfo->UpExpLvl);
		LoadData(pInfo->UpExpPower);
		LoadData(pInfo->UpMoney);	
		LoadData(pInfo->UpMoneyLvl);
		LoadData(pInfo->UpMoneyPower);
		LoadData(pInfo->UpContribution);	
		LoadData(pInfo->UpContributionLvl);
		LoadData(pInfo->UpContributionPower);
		LoadData(pInfo->bType2);
		LoadData(pInfo->nMaxTalentPoint);
		LoadData(pInfo->nEffectPerTalentPoint);
		LoadData(pInfo->nCastTimePerTalent);
		LoadData(pInfo->nCoolTimePerTalent);
		LoadData(pInfo->nDistancePerTalent);
		LoadData(pInfo->fHpPerTalent);
		LoadData(pInfo->fMpPerTalent);
		LoadData(pInfo->fParamPerTalent);
		LoadData(pInfo->nActNeedSkill1);
		LoadData(pInfo->nActNeedSkill2);
		LoadData(pInfo->nActNeedTalent1);
		LoadData(pInfo->nActNeedTalent2);

		LoadData(pInfo->SelfBuffRatio[0]);
		LoadData(pInfo->SelfBuffRatio[1]);
		LoadData(pInfo->SelfBuffRatio[2]);
		LoadData(pInfo->SelfBuffRatioPerTalent[0]);
		LoadData(pInfo->SelfBuffRatioPerTalent[1]);
		LoadData(pInfo->SelfBuffRatioPerTalent[2]);
		LoadData(pInfo->TargetBuffRatio[0]);
		LoadData(pInfo->TargetBuffRatio[1]);
		LoadData(pInfo->TargetBuffRatio[2]);
		LoadData(pInfo->TargetBuffRatioPerTalent[0]);
		LoadData(pInfo->TargetBuffRatioPerTalent[1]);
		LoadData(pInfo->TargetBuffRatioPerTalent[2]);
		LoadData(pInfo->SelfStatePerTalent[0]);
		LoadData(pInfo->SelfStatePerTalent[1]);
		LoadData(pInfo->SelfStatePerTalent[2]);
		LoadData(pInfo->TargetStatePerTalent[0]);
		LoadData(pInfo->TargetStatePerTalent[1]);
		LoadData(pInfo->TargetStatePerTalent[2]);

		Loadstring(pInfo->CheckScript);

		LoadData(pInfo->bSelfGainBuffRatio);	
		LoadData(pInfo->bSelfGainBuffRatioLvl);
		LoadData(pInfo->bTarGainBuffRatio);
		LoadData(pInfo->bTarGainBuffRatioLvl);

		LoadData(pInfo->nFuryStar);
		LoadData(pInfo->bInFury);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}

static int LOAD_VER_12(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222

		LoadData(pInfo->GainBuffOnLearn);	
		LoadData(pInfo->GainBuffToSlaveOnLearn);	
		LoadData(pInfo->GainBuffToPlayerOnLearn);

		LoadData(pInfo->UpExp);	
		LoadData(pInfo->UpExpLvl);
		LoadData(pInfo->UpExpPower);
		LoadData(pInfo->UpMoney);	
		LoadData(pInfo->UpMoneyLvl);
		LoadData(pInfo->UpMoneyPower);
		LoadData(pInfo->UpContribution);	
		LoadData(pInfo->UpContributionLvl);
		LoadData(pInfo->UpContributionPower);
		LoadData(pInfo->bType2);
		LoadData(pInfo->nMaxTalentPoint);
		LoadData(pInfo->nEffectPerTalentPoint);
		LoadData(pInfo->nCastTimePerTalent);
		LoadData(pInfo->nCoolTimePerTalent);
		LoadData(pInfo->nDistancePerTalent);
		LoadData(pInfo->fHpPerTalent);
		LoadData(pInfo->fMpPerTalent);
		LoadData(pInfo->fParamPerTalent);
		LoadData(pInfo->nActNeedSkill1);
		LoadData(pInfo->nActNeedSkill2);
		LoadData(pInfo->nActNeedTalent1);
		LoadData(pInfo->nActNeedTalent2);

		LoadData(pInfo->SelfBuffRatio[0]);
		LoadData(pInfo->SelfBuffRatio[1]);
		LoadData(pInfo->SelfBuffRatio[2]);
		LoadData(pInfo->SelfBuffRatioPerTalent[0]);
		LoadData(pInfo->SelfBuffRatioPerTalent[1]);
		LoadData(pInfo->SelfBuffRatioPerTalent[2]);
		LoadData(pInfo->TargetBuffRatio[0]);
		LoadData(pInfo->TargetBuffRatio[1]);
		LoadData(pInfo->TargetBuffRatio[2]);
		LoadData(pInfo->TargetBuffRatioPerTalent[0]);
		LoadData(pInfo->TargetBuffRatioPerTalent[1]);
		LoadData(pInfo->TargetBuffRatioPerTalent[2]);
		LoadData(pInfo->SelfStatePerTalent[0]);
		LoadData(pInfo->SelfStatePerTalent[1]);
		LoadData(pInfo->SelfStatePerTalent[2]);
		LoadData(pInfo->TargetStatePerTalent[0]);
		LoadData(pInfo->TargetStatePerTalent[1]);
		LoadData(pInfo->TargetStatePerTalent[2]);

		Loadstring(pInfo->CheckScript);
		Loadstring(pInfo->m_szTalentTxt);

		LoadData(pInfo->bSelfGainBuffRatio);	
		LoadData(pInfo->bSelfGainBuffRatioLvl);
		LoadData(pInfo->bTarGainBuffRatio);
		LoadData(pInfo->bTarGainBuffRatioLvl);

		LoadData(pInfo->nFuryStar);
		LoadData(pInfo->bInFury);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}

static int LOAD_VER_13(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222

		LoadData(pInfo->GainBuffOnLearn);	
		LoadData(pInfo->GainBuffToSlaveOnLearn);	
		LoadData(pInfo->GainBuffToPlayerOnLearn);

		LoadData(pInfo->UpExp);	
		LoadData(pInfo->UpExpLvl);
		LoadData(pInfo->UpExpPower);
		LoadData(pInfo->UpMoney);	
		LoadData(pInfo->UpMoneyLvl);
		LoadData(pInfo->UpMoneyPower);
		LoadData(pInfo->UpContribution);	
		LoadData(pInfo->UpContributionLvl);
		LoadData(pInfo->UpContributionPower);
		LoadData(pInfo->bType2);
		LoadData(pInfo->nMaxTalentPoint);
		LoadData(pInfo->nEffectPerTalentPoint);
		LoadData(pInfo->nCastTimePerTalent);
		LoadData(pInfo->nCoolTimePerTalent);
		LoadData(pInfo->nDistancePerTalent);
		LoadData(pInfo->fHpPerTalent);
		LoadData(pInfo->fMpPerTalent);
		LoadData(pInfo->fParamPerTalent);
		LoadData(pInfo->nActNeedSkill1);
		LoadData(pInfo->nActNeedSkill2);
		LoadData(pInfo->nActNeedTalent1);
		LoadData(pInfo->nActNeedTalent2);

		LoadData(pInfo->SelfBuffRatio[0]);
		LoadData(pInfo->SelfBuffRatio[1]);
		LoadData(pInfo->SelfBuffRatio[2]);
		LoadData(pInfo->SelfBuffRatioPerTalent[0]);
		LoadData(pInfo->SelfBuffRatioPerTalent[1]);
		LoadData(pInfo->SelfBuffRatioPerTalent[2]);
		LoadData(pInfo->TargetBuffRatio[0]);
		LoadData(pInfo->TargetBuffRatio[1]);
		LoadData(pInfo->TargetBuffRatio[2]);
		LoadData(pInfo->TargetBuffRatioPerTalent[0]);
		LoadData(pInfo->TargetBuffRatioPerTalent[1]);
		LoadData(pInfo->TargetBuffRatioPerTalent[2]);
		LoadData(pInfo->SelfStatePerTalent[0]);
		LoadData(pInfo->SelfStatePerTalent[1]);
		LoadData(pInfo->SelfStatePerTalent[2]);
		LoadData(pInfo->TargetStatePerTalent[0]);
		LoadData(pInfo->TargetStatePerTalent[1]);
		LoadData(pInfo->TargetStatePerTalent[2]);
		LoadData(pInfo->bArea_param1PerTalent);

		Loadstring(pInfo->CheckScript);
		Loadstring(pInfo->m_szTalentTxt);

		LoadData(pInfo->bSelfGainBuffRatio);	
		LoadData(pInfo->bSelfGainBuffRatioLvl);
		LoadData(pInfo->bTarGainBuffRatio);
		LoadData(pInfo->bTarGainBuffRatioLvl);

		LoadData(pInfo->nFuryStar);
		LoadData(pInfo->bInFury);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}

static int LOAD_VER_14(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222

		LoadData(pInfo->GainBuffOnLearn);	
		LoadData(pInfo->GainBuffToSlaveOnLearn);	
		LoadData(pInfo->GainBuffToPlayerOnLearn);

		LoadData(pInfo->UpExp);	
		LoadData(pInfo->UpExpLvl);
		LoadData(pInfo->UpExpPower);
		LoadData(pInfo->UpMoney);	
		LoadData(pInfo->UpMoneyLvl);
		LoadData(pInfo->UpMoneyPower);
		LoadData(pInfo->UpContribution);	
		LoadData(pInfo->UpContributionLvl);
		LoadData(pInfo->UpContributionPower);
		LoadData(pInfo->bType2);
		LoadData(pInfo->nMaxTalentPoint);
		LoadData(pInfo->nEffectPerTalentPoint);
		LoadData(pInfo->nCastTimePerTalent);
		LoadData(pInfo->nCoolTimePerTalent);
		LoadData(pInfo->nDistancePerTalent);
		LoadData(pInfo->fHpPerTalent);
		LoadData(pInfo->fMpPerTalent);
		LoadData(pInfo->fParamPerTalent);
		LoadData(pInfo->nActNeedSkill1);
		LoadData(pInfo->nActNeedSkill2);
		LoadData(pInfo->nActNeedTalent1);
		LoadData(pInfo->nActNeedTalent2);

		LoadData(pInfo->SelfBuffRatio[0]);
		LoadData(pInfo->SelfBuffRatio[1]);
		LoadData(pInfo->SelfBuffRatio[2]);
		LoadData(pInfo->SelfBuffRatioPerTalent[0]);
		LoadData(pInfo->SelfBuffRatioPerTalent[1]);
		LoadData(pInfo->SelfBuffRatioPerTalent[2]);
		LoadData(pInfo->TargetBuffRatio[0]);
		LoadData(pInfo->TargetBuffRatio[1]);
		LoadData(pInfo->TargetBuffRatio[2]);
		LoadData(pInfo->TargetBuffRatioPerTalent[0]);
		LoadData(pInfo->TargetBuffRatioPerTalent[1]);
		LoadData(pInfo->TargetBuffRatioPerTalent[2]);
		LoadData(pInfo->SelfStatePerTalent[0]);
		LoadData(pInfo->SelfStatePerTalent[1]);
		LoadData(pInfo->SelfStatePerTalent[2]);
		LoadData(pInfo->TargetStatePerTalent[0]);
		LoadData(pInfo->TargetStatePerTalent[1]);
		LoadData(pInfo->TargetStatePerTalent[2]);
		LoadData(pInfo->bArea_param1PerTalent);

		Loadstring(pInfo->CheckScript);
		Loadstring(pInfo->m_szTalentTxt);

		LoadData(pInfo->bSelfGainBuffRatio);	
		LoadData(pInfo->bSelfGainBuffRatioLvl);
		LoadData(pInfo->bTarGainBuffRatio);
		LoadData(pInfo->bTarGainBuffRatioLvl);

		LoadData(pInfo->nFuryStar);
		LoadData(pInfo->bInFury);

		Loadstring(pInfo->m_szSkillAttackEventLuaScript);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}
static int LOAD_VER_15(BYTE * Data,int nSize,LPSKILL*& pResult)
{
	int readpos=0,index=0;
	readpos+=4;
	LoadData(SkillInfo::count);
	pResult=new SkillInfo *[SkillInfo::count];
	while (readpos<nSize)
	{
		SkillInfo * pInfo=new SkillInfo();
		readpos+=pInfo->Load1(Data+readpos);
		LoadData(pInfo->bIgnoreBlock);
		LoadData(pInfo->HateValue);
		LoadData(pInfo->bStrengthenType);//2222
		LoadData(pInfo->fStrengthenVal);//2222
		LoadData(pInfo->bLevelUpType);//2222

		LoadData(pInfo->GainBuffOnLearn);	
		LoadData(pInfo->GainBuffToSlaveOnLearn);	
		LoadData(pInfo->GainBuffToPlayerOnLearn);

		LoadData(pInfo->UpExp);	
		LoadData(pInfo->UpExpLvl);
		LoadData(pInfo->UpExpPower);
		LoadData(pInfo->UpMoney);	
		LoadData(pInfo->UpMoneyLvl);
		LoadData(pInfo->UpMoneyPower);
		LoadData(pInfo->UpContribution);	
		LoadData(pInfo->UpContributionLvl);
		LoadData(pInfo->UpContributionPower);
		LoadData(pInfo->bType2);
		LoadData(pInfo->nMaxTalentPoint);
		LoadData(pInfo->nEffectPerTalentPoint);
		LoadData(pInfo->nCastTimePerTalent);
		LoadData(pInfo->nCoolTimePerTalent);
		LoadData(pInfo->nDistancePerTalent);
		LoadData(pInfo->fHpPerTalent);
		LoadData(pInfo->fMpPerTalent);
		LoadData(pInfo->fParamPerTalent);
		LoadData(pInfo->nActNeedSkill1);
		LoadData(pInfo->nActNeedSkill2);
		LoadData(pInfo->nActNeedTalent1);
		LoadData(pInfo->nActNeedTalent2);

		LoadData(pInfo->SelfBuffRatio[0]);
		LoadData(pInfo->SelfBuffRatio[1]);
		LoadData(pInfo->SelfBuffRatio[2]);
		LoadData(pInfo->SelfBuffRatioPerTalent[0]);
		LoadData(pInfo->SelfBuffRatioPerTalent[1]);
		LoadData(pInfo->SelfBuffRatioPerTalent[2]);
		LoadData(pInfo->TargetBuffRatio[0]);
		LoadData(pInfo->TargetBuffRatio[1]);
		LoadData(pInfo->TargetBuffRatio[2]);
		LoadData(pInfo->TargetBuffRatioPerTalent[0]);
		LoadData(pInfo->TargetBuffRatioPerTalent[1]);
		LoadData(pInfo->TargetBuffRatioPerTalent[2]);
		LoadData(pInfo->SelfStatePerTalent[0]);
		LoadData(pInfo->SelfStatePerTalent[1]);
		LoadData(pInfo->SelfStatePerTalent[2]);
		LoadData(pInfo->TargetStatePerTalent[0]);
		LoadData(pInfo->TargetStatePerTalent[1]);
		LoadData(pInfo->TargetStatePerTalent[2]);
		LoadData(pInfo->bArea_param1PerTalent);

		Loadstring(pInfo->CheckScript);
		Loadstring(pInfo->m_szTalentTxt);

		LoadData(pInfo->bSelfGainBuffRatio);	
		LoadData(pInfo->bSelfGainBuffRatioLvl);
		LoadData(pInfo->bTarGainBuffRatio);
		LoadData(pInfo->bTarGainBuffRatioLvl);

		LoadData(pInfo->nFuryStar);
		LoadData(pInfo->bInFury);

		Loadstring(pInfo->m_szSkillAttackEventLuaScript);
		LoadData(pInfo->nHateVal);
		pResult[index++]=pInfo;
	}
	SkillInfo::count=index;
	return index;
}
#endif