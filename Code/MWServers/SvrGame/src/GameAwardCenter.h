#pragma once

#include <Windows.h>

class UserData;

class OnlineAwardCenter
{
public:
	OnlineAwardCenter(UserData* pUser);
	~OnlineAwardCenter();
	void	UpdateSec();
	void	RefreshFlag();
	void	ResetFlag();
	void	RequestForGift(int nPos);
	void	SendInfo(bool ifRefresh = false);

	DWORD m_dwOnlineSec;
	DWORD m_dwStart;
	BYTE m_byGiftFlag;
	UserData* m_pParent;

	void    _initOnlineSec();
	void    _SaveOnlineSec();
};

class MounthAwardCenter
{
public:
	MounthAwardCenter(UserData* pUser);
	~MounthAwardCenter();
	void	RefreshFlag();
	void	RequestSign(bool ifToday = true);
	void	RequestForGift(int nPos);
	void	ResetFlag();
	void	SendInfo();

	int		m_nStart;
	int		m_nCount;
	BYTE	m_szGetFlag[5];
	bool	m_DayCheck;
	UserData* m_pParent;
};

class LvupAwardCenter
{
public:
	LvupAwardCenter(UserData* pUser);
	~LvupAwardCenter();
	void	RefreshFlag();
	void	RequestGift(int nPos);

	UserData* m_pParent;
	BYTE	m_szGetFlag[10];
};

class LoginAwardCenter
{
public:
	LoginAwardCenter(UserData* pUser);
	~LoginAwardCenter();
	void	RefreshFlag();
	void	RequestGift(int nPos);

	UserData* m_pParent;
	BYTE	m_szGetFlag[30];
};

class RechargeAwardCenter
{
public:
	RechargeAwardCenter(UserData* pUser);
	~RechargeAwardCenter();
	void	RefreshFlag();
	void	RequestGift();

	void	ReSetDailyFlag();
	void	RefreshFlagEx();
	void	RequestGiftEx(int nPos);
	bool m_ifGetEx[3];
	bool m_CanGet[3];

	UserData* m_pParent;
	bool m_ifGet;
	bool m_ifRecharge;
};