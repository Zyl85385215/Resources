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

	//����Լ��Ƿ������Ϊ�ƶ�������Ŀ��
	bool	CheckCanBeLock(MapAIObj* pObj);
	bool	CheckCanBeAtk(MapAIObj* pObj);
	//����Ƿ񻹿��Թ������Ŀ��
	bool	CheckCanLockAttack(MapAIObj* pTarget);
	//��ǰ״̬�ܷ��ж�
	bool	CheckCanRunCmd();
	bool	CheckCanAtkCmd();
	//boss������״̬
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

	WORD	wMPX, wMPY;		//������ͼ��
	MParam	xParam;

	bool	bDisappear;		//�Ƿ���ʧ���λ, true��ʾ�ȴ�����
	WORD	wSkin[3];		//����, NPC,��һλΪģ��ID, ��������ֱ��Ϊ����

	MapPlate*	pRunPlate;	

	EnemyBase*	pBase;
	BornPointInfo*	pBornInfo;
	MRActionPt*	pRunActPt;			//��ǰִ�е���Ϊģ��
	DWORD		dwTickRunAct;		//�������´��ж��ļ��
	BYTE		m_byAutoAtk;		//�����˵��Ƿ񹥻�λ / Ѳ�߹ֵĵ�ǰĿ��� / ���йֵ�λ�ñ��

	void	SetControl(ControlObj* pC,BYTE bType);
	ControlObj*	pCtrl;
	BYTE		bContrlType;
	DWORD		dwLockAward;	//��������Ȩ����

	BYTE	bCmd;
	DWORD	dwCmdVal;		//Ŀ��ص�,�ߵ�λ����YX
	DWORD	dwCmdValOld;
	MapAIObj*	pTarget;
	WORD		wLastLineX;		//׷��Ŀ��������Ч��
	WORD		wLastLineY;

	void		AddHitCnt(BYTE	byType, DWORD	dwID, int nAdd);
	MapAIObj*	PopTarget();
	xnList*		pListHitCnt;	//����ۻ�

	DWORD	dwTickCmd;		//�ϴ��ж���ʱ��
	WORD	wTrySkill;

	BYTE	byBattelPos;
	DWORD	dwBattelGroup;		//����
	WORD	wPathPtCnt;
	POINT*	pPtPath;

	BYTE	byDTargetType;
	DWORD	dwDTargetID;

	bool		bGSSyncPos;	//��Ҫ����Ϸ������ͬ��
	bool		bGSSyncInfo;

	bool		bSyncMpClient;
	bool		bSyncHpClient;
	bool		bSyncHpMaxClient;
	bool		bSyncSpeed;

	bool		bIsRewarded;	

	WORD		wLockCnt;	//ָ�뱻�����������õĴ�����

	SkillCenter*	pSkillCenter;
	StateCenter*	pStateCenter;

	DWORD		m_dwWarTick;		//����ս���ļ�ʱ
	DWORD		m_dwHealthTick;		//��Ѫ�ļ��

	DWORD		m_dwUpdateTime;		//������ʱ

	DWORD		m_dwTickUnActive;

	void	ResetParam();
	void	_syncParamChg();
	void	_syncSkillChg();
	void	_syncTitleChg();
	void	_syncMarryTitleChg();
	void	_syncMarryEffChg();
	void	_syncViplvChg();
	inline	int	GetObjAtkDis();

	//֪ͨ�ͻ���ִ��ʲô����
	void	_syncClientCmd(BYTE bCmd, DWORD dwVal,bool bF=false);

	map<int,int>	m_mapDamage;

	void    KillRewardByState(BYTE bType,DWORD dwUin);
	DWORD	dwBusyTick;		//�����ʱ��ǰ�������ж�
private:
	
	float	fXMSpeed;		//X������
	float	fYMSpeed;

	
	DWORD	dwAtkTick;		//�����ٹ���



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