#pragma once

#include "MapObj.h"
#include "StateData.h"

#define MAPPLATE_SIZE	128

enum	MOBJ_SKIN
{
	MSKIN_BODY,
	MSKIN_WEAPON,
	MSKIN_HORSE,
	
	MSKIN_MAX,
};

class	MObjEff
{
public:
	int		nEffID;
	int		nDifX;
	int		nDifY;
	bool	bFly;
	DWORD	dwTickDelay;
};

class	MObjAIModel
{
public:
	bool	SetCmd(BYTE bT, DWORD dwVal);
	BYTE	byAIType;		//当前执行的AI策略
	int		dwMark;			//策略参考值, 如攻击目标,移动方式等

	DWORD	dwAITick;
};

class ToopTip9Spr;
class ChatInfo;
class EnemyBase;
class PathObj;
class MapAIObj	: public MapObj
{
public:
	MapAIObj();
	virtual ~MapAIObj();

	virtual	BYTE	GetAction();
	virtual	void	SetNAtk(MapAIObj* pObj);
	virtual	void	SetSkillAtk(MapAIObj* pObj);
	
	virtual	void	Update();
	virtual	void	RunAIModel();
	bool	IsBusy();
	bool	IsAtkCD();
	bool	IsDead();

	char	m_szName[20];
	char	m_szTitle[32];
	char	m_szMarryTitle[32];
	WORD	m_szwSkin[MSKIN_MAX];

	BYTE	m_byState;
	BYTE	m_byDir;
	DWORD	m_dwIsRush;
	BYTE	m_byRushAct;

	int		nMaxHP;
	int		nHP;
	int     enemyID;


	WORD	wSpeed;
	bool	bSpeedChged;
	int		nDestX;
	int		nDestY;

	DWORD	dwMastUin;
	BYTE	byLv;
	BYTE	bForce;
	BYTE	byCountry;
	BYTE	byViplv;
	int		nMarryEff;

	MObjAIModel	objAIModel;

	EnemyBase*	pBaseCfg;
	PathObj*	pPathObj;

	ToopTip9Spr*	pTalkObj;

	xnList*		m_pLEffAdd;
	xnList*		m_pLEffDel;
	xnList*		m_pLFightNum;
	xnList*		pLHitRequest;

	StateCenter*	pStateCenter;

	
	MapAIObj*	pPreAtk;		//之前攻击过的目标

	BYTE		m_bySPEff;

	void	SetRush(int nX, int nY, BYTE bAction);
	void	SetTalkInfo(char* pMsg);
	void	SetTalkInfo(ChatInfo* pMsg);

	BYTE	GetFishDir();
private:
	void	_calcMove();
	int		_setMove(int nX, int nY);

	DWORD	dwMoveTick;		//上次移动时间点
	float	fSpeedX;		//上次移动的小数位
	float	fSpeedY;
	float	fRemainX;		//上次移动的小数位e
	float	fRemainY;


	DWORD	dwHitTick;		//最后次被击中的时间, 底层站立时判断播放受击效果
	DWORD	dwHitingTick;
	

	DWORD	dwBusyTick;
	DWORD	dwAtkLock;
	xnList*	pLDelayHit;

	void	_checkNpcAI();
public:
	void	AddEff(int nEff, int nDifX	= 0, int nDifY = 0,DWORD	dwTick =0,bool bFly = true);
	void	RemoveEff(int nEff);
	void	AddFightNum(int nNum, BYTE bType);

	void	AddDelayHit(int nDamage,BYTE bType);

	void	ObjDead();
};