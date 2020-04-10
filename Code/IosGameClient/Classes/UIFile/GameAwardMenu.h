#pragma once
#include "CtrlObj.h"

class OnlineAwardMenu	: public TabMenuObj
{
public:
	OnlineAwardMenu();
	~OnlineAwardMenu();
	void	Update();
	void	Open(int nT);
};

class MounthAwardMenu	: public TabMenuObj
{
public:
	MounthAwardMenu();
	~MounthAwardMenu();
	void	Update();
};

class LvupAwardMenu		: public TabMenuObj
{
public:
	LvupAwardMenu();
	~LvupAwardMenu();
	void	Open(int nT);
	void	Update();
};

class LoginAwardMenu	: public TabMenuObj
{
public:
	LoginAwardMenu();
	~LoginAwardMenu();
	void	Open(int nT);
	void	Update();
};

class RechargeAwardMenu	: public TabMenuObj
{
public:
	RechargeAwardMenu();
	~RechargeAwardMenu();
	void	Open(int nT);
	void    Update();

	void    RefreshAward(int nPos);
	int     m_nSelPos;
};

class InviteAwardMenu	: public TabMenuObj
{
public:
	InviteAwardMenu();
	~InviteAwardMenu();

	void	InitMenu();
	virtual		bool	CheckCanOpen();
	void		SetInvInfo(char*	szCode, int nReach, int nCheck);
};