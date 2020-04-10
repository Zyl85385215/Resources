#pragma once

#include "xnlist.h"
#include <Windows.h>
#include <map>
using namespace std;


//玩家身上的 关系列表
class UserData;
class FriendObj;
class ZPacket;
class FriendList
{
	friend class UserData;
public:
	FriendList(UserData* pParent);
	~FriendList();

	void	DBInit();
	
	FriendObj*	AddFriend(DWORD dwUinOt, char*	szName,BYTE bLv,BYTE byJob,BYTE byCountry,char* szHomeName, BYTE byRsT, bool bSaveDb);
	void	DelFriend(DWORD dwUinOt, bool bSaveDb);
	void	SetFriendVal(DWORD dwUinOt, int nVal, bool bSaveDb);
	void	ChgFriendRsT(DWORD dwUinOt, BYTE byRsT, bool bSaveDb);

	void	SendFriendList();
	bool	RecvPacket(ZPacket* pPacket);

	bool	GetRsValByType(DWORD dwUinOt,BYTE byType,bool isOwn = true);
	void	SetRsValByType(DWORD dwUinOt,BYTE byType,bool bVal,bool bSaveDb,bool isOwn = true);

	int		GetRsValByTypeCount(BYTE byType,bool isOwn = true);

	void	ResetRsVal();
	void	SendFriend(FriendObj* pObj);

	void	AddFriendRequest(UserData* pUser);		//好友申请表
	void	DealwithRequest(int nID,bool ifOK);
	void	GetSuitableList(int nLv);

	FriendObj*	GetFriend(DWORD dwUin);

	bool    CheckFriendOnline(DWORD dwUin);
	void    UpdateCheckFrdOnline();
	void    SendFrdOnlineState(DWORD dwUin);
	void    SendFrdOnlineList();
	void    SendFriendChangeMsg(DWORD dwUin, char* szName);
private:
	FriendObj*	_getFriend(DWORD dwUinOt);
	//发起一方调用数据库,被动方就改内存
	void	_insertDB(FriendObj* pFObj);
	void	_deleteDB(FriendObj* pFObj);
	void	_updateDB(FriendObj* pFObj);


	UserData*	m_pParent;
	xnList*		m_pList;
	map<DWORD,bool> m_mapState;
};