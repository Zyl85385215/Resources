#pragma once
#include "windows.h"
#include "xnlist.h"
#include "singleton.h"

class WingStepObj
{
public:
	int		nPowMin;
	int		nPowMax;
	BYTE	bNeedLv;
	BYTE	bSkin;
	int		nDuraMax;
	int		nRepairTimeLimit;
	int		nLvlNeed;
	int		nQuaMax[5];
};


class ItemWingConfig:public CSingleton<ItemWingConfig>
{
public:
	ItemWingConfig();
	~ItemWingConfig();

	bool LoadFile(char* szFile);
	WingStepObj*	GetStepInfo(BYTE bStep);
    
private:
	xnList* m_pList;
};

class HorseELvCfgObj
{
public:
	BYTE	bEffPer;
	WORD	wNeedPoint;
	int		nNeedGold;
};

class HorseELv2CfgObj
{
public:
	WORD	wNeedExp;
	WORD	wAddPer;
	BYTE	wWXAdd;
};

class HorseSkinCfgObj
{
public:
	DWORD	dwHorseID;
	DWORD	dwSpeed;
	DWORD	Skin[5];
	DWORD	state[5];
};
class HorseWXLvUpCfgObj
{
public:
	DWORD	successper;
	DWORD	eff;
	BYTE	fallback;
	DWORD	costmoney;
};
class HorseELvConfig:public CSingleton<HorseELvConfig>
{
public:
	HorseELvConfig();
	~HorseELvConfig();

	bool			LoadFile(char* szFile);	//技能
	bool			LoadFile2(char* szFile); //等级
	bool			LoadFile3(char* szFile); //形象
	bool			LoadFile4(char* szFile); //提高坐骑悟性等级
	HorseELvCfgObj*	GetHorseELv(BYTE bLv);
	HorseELv2CfgObj* GetHorseELv2(BYTE bLv);
	HorseWXLvUpCfgObj* GetHorseWXlvUpCfg(BYTE BLv);
	DWORD GetHorseSkin(BYTE bLv,DWORD HorseID);
	DWORD GetHorseState(BYTE bLv,DWORD HorseID);
	DWORD GetHorseSpeed(DWORD HorseID);

private:
	xnList* m_pList;
	xnList* m_pList2;
	xnList* m_pList3;
	xnList* m_pList4;
};

class HorseBombSKillCfgObj
{
public:
	DWORD	index;
	DWORD	dwButtle1;
	DWORD	dwButtle2;
	DWORD	skill1[10];
	DWORD	skill2[10];
	DWORD	dwButtleIco;
};

class BombSkillConfig:public CSingleton<BombSkillConfig>
{
public:
	BombSkillConfig();
	~BombSkillConfig();
	bool			LoadFile(char* szFile);	
	DWORD			GetSkinID(BYTE bLv,DWORD index,BYTE type);
	DWORD			GetSkinButtet(DWORD index,BYTE type);
	DWORD			GetBulletIco(DWORD index);
private:
	xnList* m_pList;
};
