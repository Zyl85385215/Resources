#ifndef _RANK_CENTER_H_
#define	_RANK_CENTER_H_

#include <windows.h>
#include <map>
#include "xnlist.h"
#include "ios_rank_packet.h"
#include "DBControl.h"
#include "ItemConfig.h"


typedef void (*funGetInfo) (void* param,Rank_Data* pData);

class UserData;

class RankList
{
public:
	RankList(BYTE byType,DBControl* pDB,funGetInfo funcI,bool ifSend);
	~RankList();
	bool	CheckRank(void* pParam);
	Rank_Data* GetRankData(DWORD id1,DWORD id2);
	void	AddData(Rank_Data* pData);
	void	ReSort();
	void	SendList(DWORD dwUin);
	void	SendMyRank(DWORD dwUin);
	void	SendMyRank();
	void	AddChangeUin(DWORD dwUin);
	Rank_Data*	GetRankDatabyPos(int nPos);
	bool	DeleteRank(DWORD id1,DWORD id2);
	bool	DeleteRank(DWORD id1);

	void	Clear();
	
	DWORD	m_dwValueFlag;
private:
	RANKTYPE m_byType;
	xnList*	m_pList;
	xnList* m_pSendList;
	DWORD	m_dwMin1;
	DWORD	m_dwMin2;
	DBControl* m_pDB;
	bool	m_ifSendMine;
	bool	_insertRankData(Rank_Data* pDatak,DBControl* pDB);
	bool	_updateRankData(Rank_Data* pData,DBControl* pDB);
	bool	_updatePos(Rank_Data* pData,DBControl* pDB,int nOldPos);
	bool	_updatePos(Rank_Data* pData,DBControl* pDB);
	bool	_deleteRankData(Rank_Data* pData,DBControl* pDB);
	funGetInfo m_funGetInfo;
};

struct RankEvent{
	BYTE byType;
	void* param;
};

class RankCenter
{
public:
	RankCenter(DBControl* pDB);
	~RankCenter();
	bool	CheckRank(BYTE byType,void* param,bool ifUser = true);
	void	DeleteRank(BYTE byType,DWORD id1,DWORD id2);
	void	ClearRank(BYTE byType);
	void	AddWatchData(DWORD id1,DWORD id2,BYTE byType);

	SvRankUserInfo*	AddUser(UserData* pUser);
	SvRankUserInfo*	UpdateUser(UserData* pUser);
	void	AddInfo(SvRankUserInfo* pInfo);
	void	AddInfo(RankUWatchInfo* pInfo);
	SvRankUserInfo* GetUserInfo(int nUin);
	RankUWatchInfo* GetWatchData(int nUin,int nID,BYTE byType);
	bool	DBInit();
	void	Sort();
	void	SendRankList(UserData* pUser);
	void	CheckAllUserRank(UserData* pUser);
	void	Update();
	void	RequestInfo(UserData* pUser,DWORD* szVal);

	void	GetOnlineUser(int nMinLv,xnList* pList,int Count);
	int		GetRankIDByPos(BYTE byType,int nPos);

	void	DeleteUser(DWORD dwUin);		//删角色时的操作

private:
	xnList*				m_pEventList;
	DBControl*			m_pDB;
	RankList*			m_szRankList[RANK_MAX];
	bool				m_szifChange[RANK_MAX];
	std::map<DWORD,SvRankUserInfo*> m_mapUserInfo;
	bool	_insertUserInfo(SvRankUserInfo* pInfo);
	bool	_updateUserInfo(SvRankUserInfo* pInfo);
	bool	_deleteUserInfo(SvRankUserInfo* pInfo);

	xnList*			m_pUWList;
	bool	_insertUW(RankUWatchInfo* pInfo);
	bool	_updateUW(RankUWatchInfo* pInfo);
	bool	_deleteUW(RankUWatchInfo* pInfo);
	int					m_nEventTick;
public:
	void	RequestLastDamageInfo(UserData* pUser);
	Rank_Data*			m_pLastDamage;
};

#endif