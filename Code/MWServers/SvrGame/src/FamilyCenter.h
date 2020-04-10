#pragma once

#include "FamilyObject.h"
#include "ZNetPacket.h"

struct DBControl;
struct LeagueBattlePos{
	char szName[20];
	DWORD dwUin;
	HomeObj* pHome;
};
struct LeagueBattle{
	LeagueBattlePos Pos[7];
};
class UserData;
class HomeCenter
{
public:
	friend class HomeObj;
	HomeCenter();
	~HomeCenter();

	void	UpdateMintue();
	void	UpdateSec();
	void	DBInit(DBControl* pDb);
	bool	RecvPacket( ZPacket* pPacket, DWORD dwUin );

	//外部判断玩条件进来
	void	CreateHome(char*	szName,DWORD dwUin);

	void	AddLeagueBattleObj(HomeObj* pHObj,UserData* pData);
	void	SendBattleInfo(xnList* pList,DWORD dwUin,int nCount);
	HomeObj*	_getHomeObj(WORD wHome);
	char*		GetHomeName(WORD wHome);
	void		SendBattleDailyReward();
	LeagueBattle  m_Battle;

	void	AddHomeExp(WORD wHome,int nVal);
	xnList*		m_pList;

	bool		CheckChangeLeader();

	bool		DissolveHome(HomeObj* pHObj);		//解散公会
private:

	void		_dbUpdateMember(UserSimple* pUS);
	void		_dbInsertHome(HomeObj* pHObj);
	void		_dbUpdateHome(HomeObj* pHObj);
	void		_dbUpdateHomeBuild(HomeObj* pHObj);
	void		_dbUpdateHomeSimple(HomeObj* pHObj);

	//玩家登陆或新进军团的时候发送
	void		_netHomeInfo(HomeObj* pHObj,DWORD dwUin);

	void		_netHomeExit(DWORD dwUin);
	//帮会新增了一个成员              
	void		_netMemberAdd(HomeObj* pHObj,UserSimple* pUS);
	void		_netMemberDuty(HomeObj* pHObj,UserSimple* pUS);
	//帮会成员上线
	void		_netMemberOnline(HomeObj* pHObj,UserSimple* pUS);
	void		_netMemberOffline(HomeObj* pHObj,UserSimple* pUS);
	//帮会信息改变
	void		_netHomeChg(HomeObj* pHObj);

	void		_netHomeReqer(HomeObj* pHObj,DWORD dwUin);
	void		_netHomeBuild(HomeObj* pHObj);
	void		_netHomeNewRequest(HomeObj* pHObj);
	void		_netHomeList(DWORD dwUin);
	
	void		_sendBattleReward(int nDay);

	void		_netHomeDissolved(HomeObj* pHObj);

	DBControl*	m_pDB;
	WORD		m_wMaxHomeID;

	xnList*		m_pListBattleResult;
};