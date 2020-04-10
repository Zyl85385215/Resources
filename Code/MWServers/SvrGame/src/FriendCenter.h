#pragma once

#include "xnlist.h"
#include <Windows.h>
#include <map>
using namespace std;


//������ϵ� ��ϵ�б�
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

	void	AddFriendRequest(UserData* pUser);		//���������
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
	//����һ���������ݿ�,�������͸��ڴ�
	void	_insertDB(FriendObj* pFObj);
	void	_deleteDB(FriendObj* pFObj);
	void	_updateDB(FriendObj* pFObj);


	UserData*	m_pParent;
	xnList*		m_pList;
	map<DWORD,bool> m_mapState;
};