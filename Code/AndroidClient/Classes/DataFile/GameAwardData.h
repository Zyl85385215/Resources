#pragma once
#include "PacketCenter.h"


class OnlineAwardCenter	: public PacketListener
{
public:
	OnlineAwardCenter();
	~OnlineAwardCenter();
	bool	PacketProcess(ZPacket* pPacket);
	void	UpdateSec();
	void	RefreshFlag(BYTE byFlag,DWORD dwSec);
	void	RequestForGift(BYTE byPos);

	BYTE	m_byFlag;
	DWORD	m_dwSec;
	DWORD	m_dwLeftsec;
	bool	m_ifShine;
};

class MounthAwardCenter	: public PacketListener
{
public:
	MounthAwardCenter();
	~MounthAwardCenter();
	bool	PacketProcess(ZPacket* pPacket);
	void	UpdateSec();
	void	RefreshFlag();
	void	RequestSign(bool ifToday);
	void	RequestGift(int nPos);

	int		m_nStart;
	int		m_nYear;
	int		m_nMounth;
	int		m_nDay;
	bool	m_szSignFlag[31];
	bool	m_szGetFlag[5];
	int		m_nLeftCount;
	int		m_nCount;
	bool	m_ifShine;

	bool    CheckTodaySign();
};

class LvupAwardCenter
{
public:
	LvupAwardCenter();
	~LvupAwardCenter();
	void	UpdateSec();
	void	RefreshFlag();
	void	RequestGift(int nPos);

	bool	m_szGetFlag[10];
	bool	m_ifShine;
};

class LoginAwardCenter
{
public:
	LoginAwardCenter();
	~LoginAwardCenter();
	void	UpdateSec();
	void	RefreshFlag();
	void	RequestGift(int nPos);

	bool	m_szGetFlag[30];
	bool	m_ifShine;
	bool    m_ifWarn;

	int     m_nOpenStap;
};

class RechargeAwardCenter
{
public:
	RechargeAwardCenter();
	~RechargeAwardCenter();
	void	UpdateSec();
	void	RefreshFlag();
	void	RequestGift(int nPos);

	bool	m_ifGet[3];
	bool	m_CanGet[3];
	bool	m_ifRecharge;
	bool	m_ifShine;
	bool    m_ifWarn;
};