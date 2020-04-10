#pragma once

#include "singleton.h"
#include "windows.h"
#include "xnlist.h"

class BSkillCfgObj
{
public:
	BSkillCfgObj()
	{
		pListItem	= xnList::Create();
		pNextSkill = xnList::Create();
	}
	~BSkillCfgObj()
	{
		pListItem->Free();
		pNextSkill->Free();
		
	}
	WORD	wSkill;
	char	szTxt[512];
	BYTE	bStep;
	BYTE	bNeedJob;
	BYTE	bUseTime;
	DWORD	dwNeedSkill;
	xnList*	pListItem;
	xnList*	pNextSkill;

};

class BSkillConfig : public CSingleton<BSkillConfig>
{
public:
	BSkillConfig();
	~BSkillConfig();
	void	LoadFile(char* szFile);
	BSkillCfgObj*	GetBookSkill(WORD wSID);
	BSkillCfgObj*	GetBSkillByIndex(int nIndex, BYTE bJob);
	
	xnList*		m_pList;


	void	LoadSTFile(char* szFile);
	BSkillCfgObj*	GetStFSkill(WORD wSID);
	BSkillCfgObj*	GetStFSkillByIndex(int nIndex, BYTE bJob);
	xnList*		m_pListSt;
};

class BSkillDriveCfg
{
public:
	//WORD	wState;
	WORD	wSkill;
	BYTE	bStart;
	WORD	wCoreState[10];
	BYTE	bStartLv[10];
//#ifndef _ISSERVER
	char	szName[32];
	char	szSpr[256];
	char	szStateTxt[10][256];
	
	char	szCore[10][32];
	POINT	ptPos[10];
	char	szCoreSpr[10][256];
//#endif
};

class BSkillCompState : public CSingleton<BSkillCompState>
{
public:
	BSkillCompState();
	void	LoadFile(char* szFile);
	
	BSkillDriveCfg	cBSDriveCfgs[8];
};

class BSkillLimit
{
public:
	BSkillLimit()	{	pListLimit	= xnList::Create();	}
	~BSkillLimit()	{	pListLimit->Free();	}
	WORD	wDriveCore;			//  /100  %100
	BYTE	bLevel;
	xnList*	pListLimit;			//  drive/100%10  core%100  lv/1000
};

class BSkillLvLimitCT	: public CSingleton<BSkillLvLimitCT>
{
public:
	BSkillLvLimitCT();
	~BSkillLvLimitCT();
	
	void	LoadFile(char* szFile);
	xnList*	GetCoreLimit(BYTE bDrive, BYTE bCore, BYTE bLv);	//从1开始数  drive/100%10  core%100  lv/1000
private:
	xnList*		m_pListLimit;
};

class WorldMsgConfig	:public CSingleton<WorldMsgConfig>
{
public:
	WorldMsgConfig();
	~WorldMsgConfig();
	void	LoadFile(char* szFile);
	int count;
	char ** szMsgtxt;
	char * GetString(int pos);
};

class SPDrvInfo
{
public:
	int		nID;
	char	szName[32];
	WORD	wState;
	BYTE	nEffType1;
	BYTE	nEffType2;
	BYTE	byDrv1;
	int		nEff11;
	int		nEff21;
	BYTE	byDrv2;
	int		nEff12;
	int		nEff22;
	BYTE	byDrv3;
	int		nEff13;
	int		nEff23;
	DWORD	dwItem[10];
};

class SPDrvLvCost
{
public:
	BYTE	byDrvLv1;
	BYTE	byDrvLv2;
	BYTE	byDrvLv3;
	int		nGold;
	int		nExp;
};

class SPDriveStateCfg		: public CSingleton<SPDriveStateCfg>
{
public:
	SPDriveStateCfg();
	~SPDriveStateCfg();

	void	LoadFile(char*	szFile);
	SPDrvInfo*	GetSPDIByID(DWORD	dwID);

	xnList*	m_pListSPDrvState;		//神功列表
	SPDrvLvCost	spDrvLv[10];
};

enum{
	SWORDSPRITE_TYPE_XIN,
	SWORDSPRITE_TYPE_LI,
	SWORDSPRITE_TYPE_LIN,
	SWORDSPRITE_TYPE_TI,
	SWORDSPRITE_TYPE_MING,
	SWORDSPRITE_TYPE_JIAN1,
	SWORDSPRITE_TYPE_JIAN2,
	SWORDSPRITE_TYPE_MAX,
};
class SwordSpriteCfg
{
public:
	WORD wBasePer;
	WORD WSwordLimit;
	WORD wSwordPer;
	WORD wAttr1;
	WORD wAttr2;
};

class SwordSpriteConfig	:public CSingleton<SwordSpriteConfig>
{
public:
	SwordSpriteConfig();
	~SwordSpriteConfig();
	void	LoadFile(char* szFile);
	xnList * m_plist;
	SwordSpriteCfg* GetSwordSpriteCfg(int lv);//升级判断应该是lv+1
	DWORD	dwStateID[SWORDSPRITE_TYPE_MAX];
};