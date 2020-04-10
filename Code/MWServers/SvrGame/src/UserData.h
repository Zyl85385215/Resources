#pragma once


#include "AmzDataServer.h"
#include "DBControl.h"
#include "ChatCenter.h"
#include <string>
#include "ZNetGamesvr.h"
#include "UserWealth.h"
#include "UserDataEx.h"
#include "UserRecord.h"
#include "ios_user_packet.h"
#include "NewActivityMsgCenter.h"
#include "SlaveData.h"
#include "HorseData.h"

#include "MapSObject.h"
#include "ItemCenter.h"
#include "TimeControl.h"

#include "SkillCenter.h"
#include "VIPCenter.h"
#include "TitleCenter.h"
#include "WorldTest.h"
#include "GameAwardCenter.h"
#include "ExpMultipleData.h"
#include "UserList.h"
#include "MentoringCenter.h"
#include "AchievementCenter.h"
#include "MarryCenter.h"
#include <set>
const char*	const szLuaFunctionOnPlayerKill = "OnPlayerKillEnemy";

enum
{
	LUAEVENT_CLICKNPC,
	LUAEVENT_JUMPMAP,
};
/* 
���ݿ��߳�
p = new UserData();
p->m_pDB	= this;		//��DB�߳�
p->FillDataWithDB();	//data���ݳ�ʼ��
p->FillAmzDataWithDB();	//��ʼ�����ݺ���
�������̵߳ĵȴ��б�

���߳�	��������1������
�ӵȴ��б�������, ͬ����Ϣ���ͻ���, ������Ϸ
��ҵĲ������� �����ݺ���ȡ�ö��� ͨ��OPcenter�����ٷ��������ݿ�

��ҵ�ս��ģ��ֱ�Ӹ�ֵUIN�Ͳ�ս�����ս������,ս������(�����߳�?)���ƹ��̺ͽ��չʾ

*/
class TeamData;
class FriendList;
class FriendObj;
class MailBox;
class MailObject;
class ChatRoom;
class HomeBattleObj;
class UserFlag;
class UserCoolDown;
class FortData;
class UserQuestCenter;
class MapCopyObj;
class MCopyBase;
class MapServerObj;
class WarRankUser;
struct BossInfoObj;
class MakeSkillData;
int	Random_Int(int min, int max);


class UserData:public CLuaClassObject<UserData>
{
public:
	static UserData* CreateOpUserByDB(DWORD dwUin,DBControl* pDB);
	UserData(DWORD dwUin,DBControl* pDB);
	~UserData();
	static void			CreateScriptClass(CLuaState * pLuaState);
	static void			ReleaseScriptClass(CLuaState * pLuaState);

	//��DB�̵߳��õĺ���
	bool	DBInit();
	void	CreateInit();

	void	TryUpdate();
	void	Update();
	void	Update25();
	void	AddItemEx(int nType,int nCnt,BYTE byOrigin = RMB_SAVE_NORMAL,BYTE byGoldType = UW_GOLD_NULL,BYTE byItemType = ITEM_LOG_NULL);

	void	JoinGame();
	void	LeaveGame();
	void	RecordLoginOut(DWORD dwUin,BYTE byType);

	//�Ѿ�����WorldUpdateThread��,�����հ���������
	void	RecvPacket(ZPacket* pPacket);

	void	NoticeLoginOk();
	void	SendUserInfo();
	void	DaylyRun();

	bool    CheckResetLoginDays();
	void	CheckSendLoginDays(bool bAdd);

	void	SetRevivePos(WORD wMI, WORD wX, WORD wY);

	void	RequestRevive(bool bUserItem);
	//ִ�л���ͼ����,��ͨ��ͼ,���������⴦��
	void	JumpTo(int nMIndex, int x, int y, int nBaseM = 0);

	void	SendLuaEvent(int nV1 = 0,int nV2 = 0,int nV3 = 0,int nV4 = 0,int nV5 = 0);

	DBControl*	m_pDB;

	DWORD		m_dwKey;
	DWORD		m_dwUin;
	DWORD		m_dwAccount;
	
	char		m_szNick[20];
	DWORD		m_dwNextItemID;
	DWORD		m_dwDaylyOnline;

	UserWealth	m_xUWealth;
	DWORD		m_dwHomeID;
	DWORD		m_dwHomeDuty;
	DWORD		m_dwTotalRmb;
	int			m_nMuliPay;
	int         m_nLoginDays;       //��������

	char		m_szInviteCode[10];	//������˵ı���
	char		m_szBeInvCode[10];	//�Լ����������
	WORD		m_wReachInvNum;		//�ﵽ����,�����߲���ͬ��
	WORD		m_wCheckInvNum;		//�����������

	BYTE		m_byCountry;
	BYTE		m_byJob;
	BYTE		m_bySex;
	BYTE		m_byGMLv;
	WORD		m_wJoinWaitTick;
	int			m_nRechargeTemp;
	int			m_nTPID;
	DWORD		m_dwAccCreateTime;
	BYTE		m_byMCopyAwardNum;
	DWORD			m_dwTickLogout;			//����ʱ��
	bool			m_bLogOut;
	bool			m_bEnterGame;
	
	UserLuckData*	m_pLuckData;

	int				LockADataID();
	LONGLONG		GetADKey(int	nMID);


	void			SendMailList();
	void			SendMailList(MailObject* pObj);
	bool			GetMailItem(DWORD dwMailID);

	void			ChgHp(int nVal);
	void			ChgMp(int nVal);
	bool			AddHorse(int nType);
	bool			AddPet(int nType);

	void			DealWithMarryAction(BYTE byAction);

	AmzDataManager*		m_pAmzDManager;
	FriendList*			m_pFriendList;
	UserMObjCenter*		m_pMObjCenter;
	int					m_nMIndex;			//��ͼ���
	bool				m_bIsNewAccount;
	DWORD				m_nRealMIndex;	//ֱ�Ӹ���m_wMIndex ȥ����ģ��ȡ
	CMTime				m_timeRun;

	int				m_nFastItemID;

	bool            m_bWarnJJCTip;
	//���븱��ǰ����Ϣ
	WORD				m_wOldMIndex;
	WORD				m_wOldPosX;
	WORD				m_wOldPosY;

	WORD				m_wReviveMap;
	WORD				m_wRevivePosX;
	WORD				m_wRevivePosY;

	DWORD				m_dwCopyStartTick;
	int			        GetCopyRank(DWORD dwTime,DWORD dwLmtTime);

	int				GetPosX();
	int				GetPosY();

	MapServerObj*	GetMapServerObj();

	void			SendWarnMsg(const char* szFormat, ...);
	void			SendPopMsg(const char* sztxt,...);
	void			SendPopMsgForScrpit(const char* sztxt);
	void			SendRoomMsg(BYTE byRoom,const char * szTxt,...);
	void			SendMsg(const char* sztxt,...);

	void			KillEnemyEvent(WORD wEnemyID,DWORD	dwID);
	void			KillPlayerEvent(DWORD dwID);
	void			SendKillDrop(WORD wID);
	bool			SetSimpleInfo();
	bool			UpdateSimpleInfo();
	void			UpdateFriendMsg();

	UserSimple*			m_pMySimple;
	UserMentoring*		m_pUserMentoring;
	UserMarry*			m_pUserMarry;
	UserAchievement*	m_pUserAchievement;
	UserRecord*			m_pUserRecord;
	UserFlag*			m_pUserFlag;
	UserQuestCenter*			m_pQuestCenter;
	UserCoolDown*		m_pUserCD;
	VIPCenter*			m_pVIPCenter;
	TitleCenter*		m_pTitleCenter;
	TurntableCenter*	m_pTurntableCenter;
	OnlineAwardCenter*	m_pOnlineAwardCenter;
	RechargeAwardCenter*m_pRechargeAwardCenter;
	LvupAwardCenter*	m_pLvupAwardCenter;
	LoginAwardCenter*   m_pLoginAwardCenter;
	MounthAwardCenter*	m_pMounthAwardCenter;
	UserExpCenter*		m_pUserExpCenter;
	NAMsgCenter*	m_pActivityMsgCenter;			//�¼���Ϣ
	xnList*		m_pDegreeList;		//��Ծ��
	BYTE		m_byDegreeLevel;

	SlaveCenter*	m_pSlaveCenter;
	HorseCenter*	m_pHorseCenter;
	ItemCenter*		m_pItemCenter;
	SkillCenter*	m_pSkillCenter;

	//����boss��ɱ
	bool		CheckWBossKill(WORD wEnemy);
	BossInfoObj* GetWBossInfo(WORD  wEnemy);

	bool		CheckJoinMCopy(MCopyBase* pMC);
	void		TeamStartMCopy();
	void		JoinTeamMCopy();
	bool		CheckMCopyOk(WORD wEnemy);

	int 		GetTeamTotalLv();
	void		CreateExpeditionCopy();
	void		JoinExpeditionCopy();
	bool		CheckExpeditionCopy(WORD wEnemy);

	void		RequestCreateMapCopy(WORD wMID);

	void        CheckEnemyExAward(WORD wEnemyID);

	void		RqOpenMCopyCard();
	void		RqOpenAllMCopyCard(WORD nWRmb,WORD nWCnt);

	int				m_nMCopyCardCnt;
	MapCopyObj*		m_pMapCopy;
	MCopyBase*		m_pMCopyTeam;

	LUAFUN	ItemCenter*		GetItemCenter(){return m_pItemCenter;}

	LUAFUN	int				GetFlag(WORD wID);
	LUAFUN	int				SetFlag(WORD wID,	int nVal);
	LUAFUN	int				AddFlag(WORD wID,	int nVal);

	LUAFUN	int				LearnSkill(WORD wSkill, BYTE byLv);

	void			RecalcParam();
	void            CalcGroupEquipState();
	void			MapObjChange();
	UserParam		m_Param;

	ChatRoom*			m_pChatRoom[CHAT_ROOM_MAX];

	void			AddWealth(BYTE bEnum, int nVal);
	int				GetWealth(BYTE bEnum);
	void			AddHonour(int nVal);
	bool			AddExpObj(int nID,int nTime);
	bool			RemoveExpObj(int nID);

	void			PlayEffect(int nEffID);
	int				GetMarryQuest();

	bool            m_bCheckChgCountry;
	DWORD			m_dwSCCheckTime;
private:
	bool	_AccountPacketProcess(ZPacket* pPacket);
	bool	_UserPacketProcess(ZPacket* pPacket);
	bool	_SlaveePacketProcess(ZPacket* pPacket);
	bool	_ChatPacketProcess(ZPacket* pPacket);
	bool	_ActivityPacketProcess(ZPacket* pPacket);
	bool	_RankPacketProcess(ZPacket* pPacket);
	bool	_AwardPacketProcess(ZPacket* pPacket);
	bool	_RelationPacketProcess(ZPacket* pPacket);
	bool	_dbFillDataWith();
	
	MailBox*			m_pMailBox;
	

	std::string			_getAmzDataByte(int nType,int nID);



	int				m_addVal[6];			//������ʱ
	int				m_nAttrCardID;
	int				m_nPotential;

	bool			m_bSendUserParam;
	void			_sendUserParam();
public:
	void			ResetDailyData(bool bWeek,bool bLogin=false,bool ifMounth = false);
	void			_runLuaClickNpc(int nNpcID);
	void			_runLuaJumpMap(int nFM, int nTM);


	int				m_nSysMailID;
	void			SetSysMailID(int nID);

	void			SetFastItemID(int nID);
	void			SendFastItemID();
	void			PopMarryMessage(char* szTitle,char* szMsg);

//Team
public:
	TeamData*		m_pTeamData;
	void			InviteUser(DWORD dwUin);
	void			OnLvChange();
	void			OnValChange();
	void			LeaveTeam();
	void			OnLeaveTeam();
	void			RequestTeamState(USER_CS_TEAM_STATE_Data* pRevi);

	void			SendTeamState(BYTE nType,char* szMsg = NULL);

	void			SendUserEquipInfo(DWORD dwUin);
	UserData*		GetTeamUser(int nPos);

public:
	void			OnPlayerBeHit();
	void			OnPlayerDeath();

	UserHomeSkill	m_xHomeSkill;
	void			AddHomeSkill(BYTE byType);
private:
	ItemObj*		_getRandomEquip();		//����������װ��

	void			_dbUpdateHomeSkill();
	void			_loadHomeSkill(BYTE* pData,int nSize);
	void			_sendHomeSkill();

public:
	void			SendFamilyQuest();
	void			OnFamilyQuestComplete(int nState);

	int				m_nContribute;
	void			AddContribute(int nVal);

	int				m_nMaxContribute;
	void            SendMaxContribute();
private:
	int				_getFamilyQuest();

public:
	void			SendFortData(bool bOldC);
	FortData*		m_pFortData;
	FortData*		GetFortData();
	void			SendServerTime();
	void			SendInviteInfo();
	void			SendWorldInfo();

	LUAFUN	int		GetManorLv();
	LUAFUN	void	AddManorExp(int nVal);
	LUAFUN	bool	CheckQuest(int nQid);
	LUAFUN bool	CheckCompleteQuest(WORD wQID);
	LUAFUN int		LearnMakeSkill(BYTE	byType);

	LUAFUN	void	AddState(WORD wState,bool bRemove);
	LUAFUN	bool	CheckState(WORD wState);
	LUAFUN	bool	RequestAcceptQuest(WORD wQID);
	LUAFUN	bool	RequestCompleteQuest(WORD wQID);
	LUAFUN	bool	RequestGiveUpQuest(WORD wQID);
	LUAFUN	int		GetPetCountByQuality(BYTE byQuality);
	LUAFUN	int		GetHorseCountByQuality(BYTE byQuality);

	LUAFUN	bool	DoneAchievement(int nID);
	LUAFUN	bool	AddAchievementVal(int nID,int nVal);
	LUAFUN	bool	SetAchievementVal(int nID,int nVal);
	LUAFUN	int		GetAchievementVal(int nID);
	LUAFUN	bool	DonePlayerAchievement(int nUin,int nID);
	LUAFUN	bool	AddPlayerAchievementVal(int nUin,int nID,int nVal);
	LUAFUN	bool	SetPlayerAchievementVal(int nUin,int nID,int nVal);
	LUAFUN	int		GetPlayerAchievementVal(int nUin,int nID);

	LUAFUN	void	AskEngage(int nUin);
	LUAFUN	bool	CheckMarryConditions(int nUin);
	LUAFUN	void	GetMarried();
	LUAFUN	int		GetMarryState();
	LUAFUN	int		GetLoverUin();


	int				GetScore();
	int				m_nScore;
	bool			RunLuaFunctionForQuest(int nQid,const char* const szFunctionName);

	char*			GetName(){return m_szNick;};
	int				GetCountry(){return m_byCountry;};
	int				GetJob(){return m_byJob;};
	int				GetSex(){return m_bySex;};
	int				GetUin(){return m_dwUin;};
	int				GetFamily(){return m_dwHomeID;};
	const char*		GetFamilyName();

	static	int	GetScore(int* nParam, BYTE byJob);
private:
	bool			_runLuaFunctionOnPlayerKillEnemy(UserData* pUser,int nEnemyID,int nID,const char* const szFunctionName);
	void			_runLuaFunctionOnPlayerEvent(const char* const szFunctionName);

public:
	WarRankUser*	m_pWarRankFight;		//���ڶ�ս�ľ���������

	UserData*		m_pFightTarget;
	bool			_lockFightTarget(UserData* pUser);
	void			_unlockFightTarget();
	void			_fightTarget(DWORD dwUin);
	void			_onFightResult(bool bWin);
	void			_beginFight();

	int             m_nStarState;
	int             m_nStoneState;
	bool            m_bIfEquipState;
	bool            CheckStoneState();
	bool            CheckStarState();
	void			CheckEquipState();

	void			JumpHomeWarcopy();

public:
	MakeSkillData*		m_pMakeSkillData;

	bool             m_bFirstOpenCard;
	DWORD            MCFirstSendEquip();


	enum{
		ACTIVE_OK,
		ACTIVE_USERD
	};
	void			GetActiveItem(char* szName);
	DWORD			m_dwResetTime;

	int				m_nMaxRankPos;

	void			SetMaxRankPos(int nPos);//����������λ
private:

	set<int>		m_setGroupState;
	void			_getPowerByTime(int nTime1,int nTime2);

	bool			m_bIsRecharge;	//������û��ֵ
	void			_checkLogDB();

public:
	void			ChangeUserCountry(BYTE byCountry);
	bool			m_bChangeCountry;

	BYTE            m_bySlaveAct;
	bool			ChangeUserName(char* szName);
	void            SetWealth(BYTE bEnum, int nVal);

	bool            CheckSkillLvUp(int nID, int nLv);

	void            SendGSCloseWait(DWORD dwWaitSec);


	//������½����ֻ��һ��
	bool            m_bLoginAwarded;
	void            SetLoginAwarded(bool bGet);
	int				GetLifeStarLvPvpDef();
};

extern int CalcDistance(int nX1,int nY1,int nX2,int nY2);
