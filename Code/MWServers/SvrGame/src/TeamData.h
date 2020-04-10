#pragma once

#include "xnlist.h"
#include "ios_user_packet.h"


class UserData;
class _EnemyAward;
class TeamUser	: public _TeamNet
{
public:
	TeamUser()	{pUser	= NULL;}
	UserData*	pUser;
};

struct TeamKillDropEx
{
	DWORD dwUin;
	int   nRand;
	BYTE  byNeed;
};

class TeamKillDrop
{
public:
	TeamKillDrop();
	~TeamKillDrop();
	TeamKillDropEx	m_tTeamsEx[TEAM_MAXUSER];
	int		m_nItemID;
	int		m_nNeedCnt;
};

class TeamData{
public:
	TeamData();
	~TeamData();
	int		nTeamID;
	TeamUser m_xUser[TEAM_MAXUSER];
	DWORD	m_dwTime[TEAM_MAXUSER];
	xnList*	m_pItemList;
	void	OnValChange(UserData* pUser);
	bool	Join(UserData* pUser,int nState = TEAM_NORMAL);
	void	Leave(UserData* pUser);
	void	Leave(BYTE nPos);
	bool	Update();

	bool			m_bChgLeader;
	xnList*		    m_pTKDropList;
	bool			m_bIsWaiting;
	DWORD           m_dwWaitTDNeed;
	void			CheckTKDropNeed(int nItemID,int nMIndex);
	void			SendTeamDropRand();
	void			RefreshTKData(BYTE	byRtlNeed, DWORD dwUin, WORD wItemID);
	TeamKillDrop*	CheckSendTDItem();
	void			SendTDItemAdd();
	DWORD			GetWinnerUin(TeamKillDrop* pTempInfo,xnList* pNeedList);

	bool	IsNew();
	DWORD	GetTeamLeaderUin();
	char*   GetTeamLeaderName();

	TeamUser*	GetTeamUser(DWORD dwUin);
	
	void	RecoverTeamInfo(int nPos);
	void	SendTeamUser(int nPos,DWORD dwUin);
	void	SendTeamUserToAll(int nPos);
	void	SendTeamState(BYTE nType,DWORD dwUin,char* szMsg = NULL,BYTE byPos = 0);
	void	SendTeamState(BYTE nType,DWORD dwUin,BYTE byVal);

	void	ChangeLeader(int nPos);

	void	OnlineChg(int nPos,bool bOnline,UserData* pUser);

	int		GetPosByUin(DWORD dwUin);

	int		GetTeamUserCount(bool bInv = true);
	int		GetTeamUserMapCnt(int nMapIndex);

	TeamUser*	GetTeamUser(BYTE nPos);
	TeamUser*	GetTeamLeader();

	int		TeamKillEnemy(WORD wEnemyID,int nMIndex,DWORD dwMyUin);
};

class TeamCenter{
public:
	TeamCenter();
	~TeamCenter();
	TeamData* CreateTeam(UserData* pUser);

	TeamData* GetTeamData(DWORD dwUin);

	void	Update();
	xnList*	m_pTeamList;

	int		m_nTeamMaxID;
	
private:
	int		_getTeamID();
};