#pragma once

#include "AchievementCfg.h"
#include "ios_activity_packet.h"
#include "xnlist.h"
#include "PacketCenter.h"

class CAchievementCenter	: public PacketListener
{
public:
	CAchievementCenter();
	~CAchievementCenter();
	bool	PacketProcess(ZPacket* pPacket);
	void	ClearList();
	void	UpdateObj(ClientAchievement* pObj);
	ClientAchievement* GetObj(int nID);
	void	SetPoint();
	int		GetDoneCnt();
	bool	CheckAward(int i);

	void	RequestForGift(int i);

	xnList*	m_pList;
	xnList*	m_pShowList;
	bool	m_ifChg;
	int		m_nPoint;
	int		m_nDoneCnt;
};