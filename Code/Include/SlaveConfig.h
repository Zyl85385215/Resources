#pragma once

#include "singleton.h"
#include "windows.h"
#include "xnlist.h"

class SlaveQuaSplit
{
public:
	char	szQuaName[32];
	int		nPoint;
	int		nState;
	BYTE	byPerSkill1;
	BYTE	byPerSkill2;
};

//class SlaveFoodI
//{
//public:
//	SlaveFoodI()	{szTxt	= NULL;}
//	~SlaveFoodI()	{if(szTxt) free(szTxt);}
//	WORD	wItemId;
//	WORD	wVal1;		//灵性经验      ||  经验值
//	WORD	wVal2;		//活力值        ||  活力值
//	WORD	wVal3;		//进阶值		||  耐力值
//	WORD	wVal4;		//灵性经验波动值
//	WORD	wVal5;		//活力拨打那个值
//	WORD	wVal6;		//出现特殊值的概率,0-100
//	WORD	wVal7;		//坐骑经验特殊值
//	WORD	wVla8;		//野性兽魂降低值
//	char*	szTxt;
//};

class SlavePhaseInfo
{
public:
	BYTE	bPowLvCost;
	BYTE	bParamAdd;
	BYTE	bSuccessPer;
};


class SlaveConfig : public CSingleton<SlaveConfig>
{
public:
	SlaveConfig();
	~SlaveConfig();

	void	LoadFile(char* szFile);

	int		GetLvUpExp(BYTE bCurLv);
	int		GetPowUpExp(BYTE bCurPow);
	int		GetQualityPoint(BYTE bQuality);
	char*	GetQualityName(BYTE bVal);
	//SlaveFoodI*	GetSlaveFoodById(WORD wItemID);
	//SlaveFoodI*	GetRideFoodById(WORD wItemID);
	SlavePhaseInfo*	GetPhaseInfo(BYTE	bLv);
	SlaveQuaSplit*	GetQuaInfo(BYTE bQua);
	int		GetRideState(BYTE bQua);
	BYTE	RandomQuality();

	WORD	m_wAttriMin;
	WORD	m_wAttriMax;
	
	BYTE	m_byQualityCnt;

	WORD	m_wPowTotal;
	WORD	m_wPowCostTime;
	WORD	m_wPowCostTmVal;
	WORD	m_wPowCostJoin;
	WORD	m_wPowCostDead;

	WORD	m_wTimeRecover;
	WORD	m_wStateRecover;

	BYTE	m_byEvoCnt;

	WORD	m_wResetItemID1;
	WORD	m_wResetItemID2;

private:
	xnList*		pListLvUpExp;
	xnList*		pListQuaSplit;
	xnList*		pListPhase;
};

class CaptureInfo
{
public:
	WORD	wEnemyID;
	WORD	wSlaveID;
	WORD	wSlaveExID;
	WORD	wSlaveExPer;
	bool	bIsRide;
};

class CaptureConfig : public CSingleton<CaptureConfig>
{
public:
	CaptureConfig();
	~CaptureConfig();

	void	LoadFile(char* szFile);
	int		GetRtlID(IN int nEID, OUT bool* pbIsRide = NULL);

private:
	xnList*	m_pList;
};

class SlaveFuseInfo
{
public:
	BYTE	bSuccessPer;
	WORD	bEffect;
	BYTE	bFailBack;
	int		nCost;
	int		nNeedVal;
	int		nMinVal;
	int		nMaxVal;
};

class SlaveFuseConfig : public CSingleton<SlaveFuseConfig>
{
public:
	SlaveFuseConfig();
	~SlaveFuseConfig();

	void	ReloadFile(char* szFile);

	void	LoadFile(char* szFile);
	SlaveFuseInfo*	GetSFInfo(BYTE bLv);
	
private:
	xnList*		m_pList;
};

class SlaveChgBaseObj
{
public:
	WORD	wSlaveA;
	WORD	wSlaveB;
	WORD	wSkillA[6];
	WORD	wSkillB[6];
};

class SlaveChgBaseSpecial : public CSingleton<SlaveChgBaseSpecial>
{
public:
	SlaveChgBaseSpecial();
	~SlaveChgBaseSpecial();

	void	LoadFile(char* szFile);
	SlaveChgBaseObj*	GetChgBaseObj(WORD	wSlave);

private:
	xnList*		m_pList;
};

class SFuseSkillObj
{
public:
	WORD	wSkill;
	int		nPer;
	BYTE	byGet;
	int		nIco;
	BYTE	bType;
};

class SlaveFuseSkillCfg	: public	CSingleton<SlaveFuseSkillCfg>
{
public:
	SlaveFuseSkillCfg();
	~SlaveFuseSkillCfg();

	void	LoadFile(char* szFile);
	WORD	RandomGetFSkill();
	SFuseSkillObj*	GetFSkill(WORD wSID);
	
private:
	xnList*		m_pList;

	int		m_nTotalPer;
};

class SlavePExObj
{
public:
	BYTE	bLv;
	BYTE	bSuccess;		//100
	DWORD	dwSoul;
	DWORD	dwCostVal;
	int		nEffAtkW;
	int		nEffAtkN;
	int		nEffDefW;
	int		nEffDefN;
	int		nEffHp;
	int		nEffFuse;		//10000
	WORD	wEffID;
};


class SlavePhaseExCfg : public		CSingleton<SlavePhaseExCfg>
{
public:
	SlavePhaseExCfg();
	~SlavePhaseExCfg();

	void			LoadFile(char* szFile);
	SlavePExObj*	GetPhaseExObj(BYTE bLv);

private:
	xnList*		m_pList;
};