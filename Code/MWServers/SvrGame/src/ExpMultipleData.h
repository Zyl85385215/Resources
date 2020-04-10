#pragma once

#include <Windows.h>
#include "xnlist.h"

struct UserExpObj
{
	int nID;
	int nTimeSec;
	bool ifSaveDB;
};

class UserData;

class UserExpCenter
{
public:
	UserExpCenter(UserData* pUser);
	~UserExpCenter();
	bool	AddObj(int nID,int nTime = 0);
	void	UpdateSec();
	void	Update();
	void	CheckWhenDie();
	UserExpObj* GetObj(int nID);
	bool	RemoveObj(int nID);
	bool	SetData(BYTE* pData,int nSize);
	void	ReclaExp();
	void	SendExp();
	int		GetExp(int nExp,BYTE byType);

	xnList* m_pList;
	UserData*	m_pParent;
	int		m_nUserAdd;
	int		m_nPetAdd;
	int		m_nUserAddSec;
	int		m_nPetAddSec;
	bool	m_ifSave;
	bool	m_ifChg;
private:
	bool	SaveDB();
};