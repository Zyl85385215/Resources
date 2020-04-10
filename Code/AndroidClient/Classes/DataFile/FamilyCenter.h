#pragma once

#include "PacketCenter.h"
#include "ios_home_packet.h"
class FamilyCenter	: public PacketListener{
public:
	FamilyCenter();
	~FamilyCenter();;
	virtual	bool PacketProcess(ZPacket* pPacket);
	xnList*	m_pLeagueList;
	BYTE	m_bLv;
	BYTE	m_bOccupy;
	DWORD	m_dwExp;
	WORD	m_wCnt;
	char	m_szName[20];
	char	m_szDesc[100];
	xnList*	m_pMemeber;
	xnList*	m_pJoinList;

	NHomeSInfo* GetHomeInfoByID(DWORD wHomeID);

	HomeBuildTag	m_xBuild;

	int		GetContribute();
	int		GetDuty();
	int		GetOnlineCount();

	void	_familyUpdate();

	void	DissolveHome();
};
extern FamilyCenter*	pLeague;