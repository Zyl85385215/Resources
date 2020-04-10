#pragma once

#include "CtrlObj.h"
#include "PacketCenter.h"

class FriendMenu	: public PacketListener,public TabMenuObj
{
public:
	FriendMenu();
	~FriendMenu();

	void		Open(int nT);
	void		Update();
	virtual	bool PacketProcess(ZPacket* pPacket);
	void		Clear();

	bool		m_bUpdate;

	int			m_nSelect;
	void		Select(int nSelect);
	bool		_getRsValByType( DWORD dwUin,int nVal,BYTE byType ,bool Own);
	//DForm*		m_pFormAdd;

	bool		m_bGift;
	char        m_szName[16];
};

class AddFriendMenu{
public:
	AddFriendMenu();
	~AddFriendMenu();
	void	Open();
	void	Update();
	void	ClearList();
	DForm* m_pForm;

	xnList* m_pSuitlist;
	int m_nCur;
};

class FRequestMenu
{
public:
	FRequestMenu();
	~FRequestMenu();
	void	Open();
	void	Update();
	void	AddRequest(SuitFriend*pObj);
	void	ClearList();
	DForm* m_pForm;

	xnList* m_pList;
	int m_nCur;
};