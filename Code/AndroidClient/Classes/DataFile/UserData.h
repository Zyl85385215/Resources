#pragma once

#include "PacketCenter.h"
#include "AmzDataBase.h"
#include "ItemData.h"
#include "EquipData.h"
#include "ios_user_packet.h"
#include "ios_muli_packet.h"
#include "ios_mapobj_packet.h"
#include "SlaveData.h"
#include "HorseData.h"
#include "ClientChat.h"
#include "ClientFlag.h"
#include "ClientQuest.h"
#include "GameActivity.h"
#include "SkillData.h"
#include "StateData.h"
#include "TitleConfig.h"
#include "FortCenter.h"
#include "RankData.h"
#include "GameAwardData.h"
#include "RelationshipData.h"
#include "ClientAchievement.h"



class MailObj{
public:
	DWORD	dwMailID;
	DWORD	uinSender;
	DWORD	uinRecver;
	char	szTitle[32];
	char	szBody[255];
	BYTE	byState;		
	int		nMoney;	
	int		nItemID[nMaxMailItemCount];
	int		nCnt[nMaxMailItemCount];
	int		nIndex[nMaxMailItemCount];
	DWORD	dwReturnTick;
};

class MapAIObj;
class UserData;
extern UserData*	pChar;
class UserData	: public PacketListener
{
public:
	UserData();
	~UserData();

	virtual	bool PacketProcess(ZPacket* pPacket);
	bool	AccountPacketProcess(ZPacket* pPacket);
	void	MuliThreadInitCfg();
	bool	m_bLoadCfgOK;
	void	CheckLoadCfgOk();
	void	Update();
	void	UpdateSec();
	void	CheckNetLink();
	void	ReturnLogin();
	void	ReConnect();
	void	InitData();
	static UserData*	GetDefault()	{return pChar;}

	MapAIObj*	GetMyMapObj();
	bool	CheckInFight();

	AmzDataManager*		m_pDataCenter;
	ItemCenter*			m_pItemCenter;
	EquipCenter*			m_pEquipCenter;
	ClientFlagCenter*	m_pClientFlag;
	ClientQuestCenter*	m_pCQuestCenter;

	std::map<int,FriendObj>				m_pFriendList;
	std::map<int,MailObj>				m_pMailList;
	map<DWORD,bool>                     m_mapOnline;
	bool                                CheckFriendOnline(DWORD dwUin);
	
	DWORD		m_dwMoney;
	DWORD		m_dwRmb;
	DWORD		m_dwBindRmb;
	DWORD		m_dwPow;
	DWORD		m_dwExp;
	BYTE		m_byLvl;
	DWORD		m_dwUin;
	BYTE		m_byJob;
	BYTE		m_bySex;
	BYTE		m_byCountry;
	int			m_nUserMultipile;
	int			m_nPetMultipile;
	DWORD		m_dwTickBattle;		//战斗时间

	DWORD		m_dwHomeMaxCtb;

	WORD		m_wSkillPt;
	DWORD		m_dwHomeID;
	DWORD		m_dwHomeDuty;
	char		m_szNick[20];
	BYTE		m_byVipLvl;
	DWORD		m_dwTickJoin;
	DWORD		m_dwVipExp;
	DWORD		m_dwHonour;
	DWORD		m_dwLockSkill;
	WORD		m_wLockSkillID;
	WORD		m_wStarLv;
	TitleObj*	m_pTitleObj;
	TitleObj*	m_pTitleNextObj;

	bool		m_bVIPCh;

	int			m_nServerTime;
	int			m_nGetServerTime;

	int         m_nLoginDays;

	SlaveCCenter*	m_pSlaveCenter;
	HorseCCenter*	m_pHorseCenter;
	ChatCenter*		m_pChatCenter;
	WorldTestCenter*m_pWTestCenter;
	SkillCenter*	m_pSkillCenter;
	StateCenter*	m_pStateCenter;
	FortCenter*		m_pFortCenter;
	RankCenter*		m_pRankCenter;
	ACDegreeCenter*			m_pADegreeCenter;
	GameActivityCenter* m_pActivityCenter;
	TurntableCenter*	m_pTurntableCenter;
	OnlineAwardCenter*	m_pOnlineAwardCenter;
	RechargeAwardCenter*m_pRechargeAwardCenter;
	LvupAwardCenter*	m_pLvupAwardCenter;
	LoginAwardCenter*   m_pLoginAwardCenter;
	MounthAwardCenter*	m_pMounthAwardCenter;
	MentoringClient*	m_pMentoringClient;
	CAchievementCenter*	m_pCAchievementCenter;
	MarryClient*		m_pMarryCenter;

	int			m_nFastItem;

	DWORD		m_dwJumpLock;
	DWORD		m_dwCmdLock;

	DWORD		m_dwTickCmdSync;		//命令半秒执行一次?
	DWORD		m_dwTickAppPay;
	//MapPlayer*	pMapObj;
	bool		m_bManualSel;
	WORD		m_wCtrlAtkDis;
	int			m_nGetSlaveLmt;

	BYTE		m_byGSStep;
	BYTE		m_byMSStep;
	WORD		m_wAliveGSFrm;
	WORD		m_wAliveMSFrm;
	bool		m_bLockReconnet;
	bool		m_bLockJump;
public:
	void		_checkUserCommand(bool bForce= false);

	bool		_checkUseSkill(WORD wSkill, MapAIObj* pObj);
	//检测地图区域事件
	void		_checkMapPosEvent();
	void		_sendMoveTarget(int nX, int nY);
	void		_sendAtkTarget(WORD wSkill, BYTE bType, DWORD dwID);

	void		_clickNpc(MapAIObj* pObj);
	void		ChgMapEvent();
	bool		CheckFixEquip();

	UserParam	m_xUserParam;
	UserParam	m_xUserParam2;
	int			m_nMaxMp;
	int			m_nMp;
	int			m_nSPVal;
	BYTE		m_szbyActLock[7];	//1晕眩, 2无敌, 3不能移动, 4 不能技能, 5 不能攻击, 6 不能物品 7不能骑马

	void			SendTeamState(BYTE nType,char* szMsg = NULL,BYTE byPos = 0);
	void			SendTeamState( BYTE nType,BYTE byVal);

public://组队
	void		InvitePlayer(DWORD dwUin);	//邀请队友
	void		PreTeamPlayer(DWORD dwUin);
	UserHomeSkill m_xHomeSkill;

	void		ClearTeamData();

	BYTE        GetHomeCopyLv();
public:
	DWORD		GetServerTime();	
	int				GetScore();//战斗力
	bool		IsNewMail();

	char		m_szTargetName[20];
	bool		m_bFightRequest;
	DWORD		m_dwFightRequestTick;
	void		DealFightRequest(bool bVal);

	bool		m_bHarvest;

	bool		m_bFish;
	bool		m_bFishAct;
private:
	DWORD		m_dwServerTime;
	DWORD		m_dwUpdateTime;


public:
	MakeSkill	m_makeSkill;
	bool		CheckSkill(MAKE_SKILL_TYPE byType);

	DWORD		m_dwBeHurtTick;
	void        CheckFishAct();

	bool        m_bClickExp;

	char		m_szTeamRqstName[20];
	bool		m_bTeamRequest;
	DWORD		m_dwTeamRequestTick;
	void		DealTeamRequest(bool bVal);

	DWORD		m_dwMCAutoSwitchTick;

	bool        m_bHorseState;

	bool		m_bIsFishSkin;
	void		UpdateCatchFishSkin();

	DWORD		m_dwChangeCountryTick;

	DWORD		m_dwLogoutWaitTick;

	char*			GetName(){return m_szNick;};
	int				GetCountry(){return m_byCountry;};
	int				GetJob(){return m_byJob;};
	int				GetSex(){return m_bySex;};
	int				GetUin(){return m_dwUin;};
	int				GetLevel(){return m_byLvl;};
	int				GetHomeID(){return m_dwHomeID;};
	int				GetMoney(){return m_dwMoney;}
	int				GetRmb(){return m_dwRmb;}
	int				GetBindRmb(){return m_dwBindRmb;}
	int				GetVipLvl(){return m_byVipLvl;}
};