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
	BYTE	byAIType;		//��ǰִ�е�AI����
	int		dwMark;			//���Բο�ֵ, �繥��Ŀ��,�ƶ���ʽ��

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

	
	MapAIObj*	pPreAtk;		//֮ǰ��������Ŀ��

	BYTE		m_bySPEff;

	void	SetRush(int nX, int nY, BYTE bAction);
	void	SetTalkInfo(char* pMsg);
	void	SetTalkInfo(ChatInfo* pMsg);

	BYTE	GetFishDir();
private:
	void	_calcMove();
	int		_setMove(int nX, int nY);

	DWORD	dwMoveTick;		//�ϴ��ƶ�ʱ���
	float	fSpeedX;		//�ϴ��ƶ���С��λ
	float	fSpeedY;
	float	fRemainX;		//�ϴ��ƶ���С��λe
	float	fRemainY;


	DWORD	dwHitTick;		//���α����е�ʱ��, �ײ�վ��ʱ�жϲ����ܻ�Ч��
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