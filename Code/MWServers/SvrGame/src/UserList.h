#pragma once

#include <Windows.h>
#include "xnlist.h"

//��ҵļ���Ϣ,����ʱ����
class HomeObj;
class UserSimple
{
public:
	UserSimple();
	~UserSimple();

	int		dwUin;
	int		nAccountUin;
	char*	szName;
	BYTE	bLv;
	BYTE	bVipLv;
	BYTE	bJob;
	BYTE	bSex;
	BYTE	bCountry;
	BYTE	byHomeDuty;
	WORD	wHomeID;
	int		nContribution;
	int		nMentoringVal;
	int		nMentoringTick;
	DWORD	dwTotalRmb;
	int		nMuliPay;

	bool	bOnline;
	bool	ifDelete;	//�Ƿ�ɾ��

	//��������
	xnList*	pHomeList;
	void	RemoveAllHome();
	void	RemoveHome(HomeObj* pObj);
};

class DBIO;
class UserList
{
public:
	UserList();
	~UserList();

	void		DBInit(DBIO* pDB);
	int			GetMaxUserID(DBIO* pDB);
	UserSimple*	GetUser(DWORD	dwUin);
	UserSimple*	GetUserByAcc(DWORD	dwAcc);
	UserSimple*	GetUser(char* szName);

	void		AddUser(UserSimple* pUser);

	void		SetTotalRmb(DWORD dwAcc,int nVal);
	void		DeleteUser(DWORD dwUin);

	xnList*		m_pListUsers;
	xnList*		m_pListUsersSortByName;
	DWORD		m_dwMaxUin;
};

extern UserList* g_pUserList;