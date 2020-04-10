#pragma once

#include "ios_mapobj_packet.h"
#include <Windows.h>
#include "xnlist.h"
#include <map>
using namespace std;
#define MAPPLATE_SIZE	128

class MapPlate;
class ControlObj;
class EnemyBase;
class BornPointInfo;
class SkillCenter;
class SkillObj;
class StateCenter;
class MRActionPt;

class _HitCountObj
{
public:
	BYTE	bType;
	DWORD	dwID;
	DWORD	dwHitCnt;
};

class MapAIObj
{
public:
	MapAIObj();
	~MapAIObj();

	void	RunCmd();
	void	SetCmd(BYTE bC, DWORD dwVal,MapAIObj* pTarget = NULL);
	DWORD	GetMasterID();
	void	Update();
	void	CheckHealth();

	//检测自己是否可以作为移动或跟随的目标
	bool	CheckCanBeLock(MapAIObj* pObj);
	bool	CheckCanBeAtk(MapAIObj* pObj);
	//检测是否还可以攻击这个目标
	bool	CheckCanLockAttack(MapAIObj* pTarget);
	//当前状态能否行动
	bool	CheckCanRunCmd();
	bool	CheckCanAtkCmd();
	//boss怪限制状态
	bool	CheckCanAddState(MapAIObj* pTarget,SkillObj* pSkill);

	void	HurtByObj(MapAIObj* pObj,int nDamage);
	void	ChgHp(int nVal);
	bool	ChgMp(int nVal);
	void	JumpInMap(int wX, int wY);

	bool	RushToTarget(MapAIObj* pObj, BYTE bType);

	char	szName[16];
	char	szTitle[22];
	char	szMarryTitle[32];
	int		nMarryEff;
	BYTE	byType;
	BYTE	bLv;
	BYTE	bySex;
	BYTE	byJob;
	BYTE	byCountry;
	WORD	wHomeID;
	BYTE	byViplv;

	DWORD	dwUin;
	WORD	bForce;

	DWORD	nMIndex;
	int		nX,nY;

	WORD	wMPX, wMPY;		//所处地图块
	MParam	xParam;

	bool	bDisappear;		//是否消失标记位, true表示等待回收
	WORD	wSkin[3];		//怪物, NPC,第一位为模板ID, 宠物人物直接为形象

	MapPlate*	pRunPlate;	

	EnemyBase*	pBase;
	BornPointInfo*	pBornInfo;
	MRActionPt*	pRunActPt;			//当前执行的行为模块
	DWORD		dwTickRunAct;		//机器人下次行动的间隔
	BYTE		m_byAutoAtk;		//机器人的是否攻击位 / 巡逻怪的当前目标点 / 队列怪的位置编号

	void	SetControl(ControlObj* pC,BYTE bType);
	ControlObj*	pCtrl;
	BYTE		bContrlType;
	DWORD		dwLockAward;	//怪物所有权锁定

	BYTE	bCmd;
	DWORD	dwCmdVal;		//目标地点,高低位代表YX
	DWORD	dwCmdValOld;
	MapAIObj*	pTarget;
	WORD		wLastLineX;		//追踪目标的最后有效点
	WORD		wLastLineY;

	void		AddHitCnt(BYTE	byType, DWORD	dwID, int nAdd);
	MapAIObj*	PopTarget();
	xnList*		pListHitCnt;	//仇恨累积

	DWORD	dwTickCmd;		//上次行动的时间
	WORD	wTrySkill;

	BYTE	byBattelPos;
	DWORD	dwBattelGroup;		//阵列
	WORD	wPathPtCnt;
	POINT*	pPtPath;

	BYTE	byDTargetType;
	DWORD	dwDTargetID;

	bool		bGSSyncPos;	//需要跟游戏服务器同步
	bool		bGSSyncInfo;

	bool		bSyncMpClient;
	bool		bSyncHpClient;
	bool		bSyncHpMaxClient;
	bool		bSyncSpeed;

	bool		bIsRewarded;	

	WORD		wLockCnt;	//指针被其他对象引用的次数表

	SkillCenter*	pSkillCenter;
	StateCenter*	pStateCenter;

	DWORD		m_dwWarTick;		//脱离战斗的计时
	DWORD		m_dwHealthTick;		//回血的间隔

	DWORD		m_dwUpdateTime;		//次数计时

	DWORD		m_dwTickUnActive;

	void	ResetParam();
	void	_syncParamChg();
	void	_syncSkillChg();
	void	_syncTitleChg();
	void	_syncMarryTitleChg();
	void	_syncMarryEffChg();
	void	_syncViplvChg();
	inline	int	GetObjAtkDis();

	//通知客户端执行什么动作
	void	_syncClientCmd(BYTE bCmd, DWORD dwVal,bool bF=false);

	map<int,int>	m_mapDamage;

	void    KillRewardByState(BYTE bType,DWORD dwUin);
	DWORD	dwBusyTick;		//到这个时间前不可在行动
private:
	
	float	fXMSpeed;		//X轴向量
	float	fYMSpeed;

	
	DWORD	dwAtkTick;		//不可再攻击



	void	_executeRun(bool bOnlyRun = true);

	void	_executeAttack();
	void	_executeFollow();
	void	_useSkill(SkillObj* pSkill,MapAIObj* pFstTarget);

	SkillObj*	_getAtkSkillByDis(int nDis, int& nRqDis);

	void	_normalAttack(MapAIObj* pTarget);
	void	_castAttack(MapAIObj* pTarget);
	void	_skillAttack(MapAIObj* pTarget, SkillObj* pSkill);
	void	_skillHurt(MapAIObj* pTarget, SkillObj* pSkill);
	void	_skillMiss(MapAIObj* pTarget, SkillObj* pSkill);
};