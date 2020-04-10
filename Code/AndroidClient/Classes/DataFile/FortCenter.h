#pragma once
#include "ios_muli_packet.h"
#include "PacketCenter.h"
#include <map>
#include "GlobalDefine.h"
class ItemObj;
class UserData;
class FortCenter: public PacketListener{
public:
	FortCenter(UserData* pData);
	~FortCenter();

	std::map<DWORD,ManorDataEx>	m_pManorData;
	std::map<DWORD,WarRankData>	m_pWarRank;

	std::map<DWORD,NetManorRecord>	m_pManorRocord;
	char*       GetManorRocordName(DWORD dwUin);

	virtual	bool PacketProcess(ZPacket* pPacket);
	
	void		_updateFortMenu();

	ManorDataEx*	GetManorData(DWORD dwUin);
	WarRankData*	GetWarRankData(DWORD dwUin);
	int			GetWarRankScore(DWORD dwUin);

	void		SeedManorPos(int nPos,ItemObj* pItem);

	void		UnlockManorPos();

	void		FastManorPos(DWORD dwUin,int nPos);

	void		GetManorReward(int nPos);
	void		GetManorReward(DWORD dwUin,int nPos);
	void		GetAllReward();

	void		GetFriendManorData(DWORD dwUin);

	void		WarRankPos(int nPos);

	xnList*		m_pWarRankList;
	xnList*		m_pWarTenList;
private:
	UserData*	m_pUser;
	DWORD		m_dwSwitchFriendManor;
};

