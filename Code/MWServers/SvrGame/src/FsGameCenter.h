#pragma once

#include "UserData.h"
#include <hash_map>
#include "WorldTest.h"
#include "MCopyCenter.h"
#include "RankCenter.h"
#include "GameActivityCenter.h"
#include "AchievementCenter.h"
#include <map>
#include <set>
#include <string>
using namespace std;
/*------------------------------------------------------------
游戏的主要模块,
网络包分发角色模块,角色间数据交互,游戏环境设定,角色进入离开游戏


-------------------------------------------------------------*/
enum
{
	msg_type_normal,
	msg_type_error,
	msg_type_sql,
};
struct  luaStringTag
{
	string szLua;
};
struct  NoticeStringTag
{
	string szNotice;
};

struct	GMMsgObj
{
	char*	szMsg;
	BYTE	bChannel;
	int		nTime;
	DWORD	dwSplitSec;
	DWORD	dwTickNextShow;
};

class LockSection{
public:
	LockSection(CRITICAL_SECTION& cLock);
	~LockSection();
	CRITICAL_SECTION&	csLock;
};
typedef std::hash_map<DWORD,UserData*>						HMapUserData;
typedef std::hash_map<DWORD,UserData*>::iterator			HMapUserDataPtr;
typedef std::hash_map<std::string,UserData*>				HMapUserNameData;
typedef std::hash_map<std::string,UserData*>::iterator		HMapUserNameDataPtr;
class MailCenter;
class HomeCenter;
class CListCtrl;
class TeamCenter;
class ZLogFile;
class ItemManage;
class FortCenter;
class SysMailCenter;
class CallEnemyCenter;
class WarRankCenter;
class MapObjCenter;
class NASaleCenter;
class WorldFlag;
class GMActiveCenter;
class WorldLog;
class FsGameCenter
{
public:
	FsGameCenter();
	~FsGameCenter();

	void		FireHeartBeat();
	void		FireDealPacket();

	void		MTInitCenter();
	void		HeartBeat();
	void		DealPacket();
	//数据库线程调用,角色进入游戏
	void		DBT_PlayerJoinWait(UserData* pUser);

	
	//void		PlayerExitGame(DWORD	dwUin);

	void		RecvPacket(ZPacket* pPacket, DWORD dwUin);
	void		Update();		//100ms一次
	void		UpdateSec();

	void		RefreshUiView(CListCtrl* pCtrl);

	void		ResetDailyData();

	//多线程访问,不可修改值
	UserData*	GetPlayer(DWORD	dwUin);
	UserData*	GetPlayerByAcc(DWORD	dwAcc);
	UserData*	GetPlayerByname(char* szName);
	void		GetUinList(xnList* pList);
	void		GetUinListByCountry(xnList* pList,BYTE byCountry);
	void		GetPlayerByLv(int nMine,int nMin,int nMax,xnList* pList);
	int			RandGetPlayerByLv(DWORD dwOutUin,int nLvS, int nLvE, xnList* pList,int nCnt);
	int			GetUserCnt()	{return m_mapUsers.size();}
	xnThread*	m_pMainThread;

	HomeCenter*		m_pHomeCenter;
	MailCenter*		m_pMailCenter;
	TeamCenter*		m_pTeamCenter;
	NASaleCenter*	m_pNASaleCenter;
	WorldTestCenter*m_pWorldTestCenter;
	MapCopyCenter*	m_pMapCopyCenter;
	RankCenter*		m_pRankCenter;
	FortCenter*		m_pFortCenter;
	SysMailCenter*	m_pSysMailCenter;
	CallEnemyCenter*	m_pCallEnemyCenter;
	MapObjCenter*		m_pMapObjCenter;
	WorldFlag*			m_pWorldFlag;
	AchievementCenter*	m_pAchievementCenter;

	ItemManage*		m_pItemManage;
	GameActivityCenter* m_pActivityCenter;

	WarRankCenter*	m_pWarRankCenter;

	WorldLog*		m_pWorldLog;

	xnList*			m_pListLockNpcCmd;
	xnList*			m_pListNotice;

	BYTE			m_byShutDown;
	DWORD			m_dwShutTick;

	DWORD			m_dwShutWaitTick;
	void			SendGameSvrCloseTick(DWORD dwTick);

	void		SendPValInfo(int nPVal,int nType,int nValEx = 0);

	void		GetUserPayData(int nVal1,int nVal2,map<int,int>& mapData);
	void		GetUserByLeague(int nLeague,set<int>& setData);
	void		ClearTempPay();
	void		AddUserRmbByScript(int nUin,int nVal);

	int			GetOnlineCnt();

	int			CheckForbid(int nUin);
	void		ForbidPlayer(int nUin,DWORD dwTime);
	bool		CheckEverForbid(int nUin);
	void		EverForbidPlayer(int nUin,bool byType);
	std::map<int,DWORD>	m_mapForbid;
	std::map<int,bool>	m_mapEverForbid;

	void		KickPlayer(int nUin,DWORD dwTime);
	void		UnablePlayer(int nUin,bool bForbid);
private:
	void		_deletePlayer(DWORD dwUin);
	void		_dealClientPackets();
	void		_playerJoinGame(UserData* pUser);
	void		_playerExitGame(DWORD	dwUin);

	HMapUserData	m_mapUsers;	
	HMapUserNameData m_mapNameUsers;
	xnList*			m_pListWaitExit;		//玩家离开游戏的指针缓存, 暂不重用
	xnList*			m_pListWaitJoin;
	CRITICAL_SECTION	m_xLockJoin;
	int				m_nDay;

	xnList*			m_pLuaList;
	void			CheckLuaExecute();
public:
	void		PushLuaExecute(luaStringTag* pTag);
	CRITICAL_SECTION	m_xLockLuaExecute;
	//UserData*	

	string      m_strNotice;
	void		PushNoticeExecute(NoticeStringTag* pTag);

	//GM添加活动
	GMActiveCenter* m_pGMActiveCenter;

	int				m_nDefaultCountry;
	int				m_nLockCountry;

	void            GMAddSvrExpMult(int nID,bool bRemove);
	int             m_nGMSvrExpMult;
	DBControl*		m_ppPlayerDB[8];
	DBControl*		m_ppLoginDB[2];

	void			AddMarkPayTable(int nAccUin, int nRealVal);		//充值计数
};

extern void OutMessage(int channel, char* lpszFormat, ...);
extern	FsGameCenter* g_pGameCenter;
extern	ZLogFile* g_pLogFile;
extern	ZLogFile* g_pErrorFile;
extern	bool      g_bSvrExpMult;