#pragma once
#include "PacketCenter.h"
#include "ios_activity_packet.h"
#include "TestAwardCfg.h"
#include "TestLibraryCfg.h"


class WorldTestCenter : public PacketListener
{
public:
	WorldTestCenter();
	~WorldTestCenter();
	bool PacketProcess(ZPacket* pPacket);
	void AnswerQuestion(BYTE byPos);
	void RequestStart();
	void RequestGiveup();
	void RequestForGift();

	int m_nCount;
	int m_nStar;
	int m_nCorrect;
	int m_nPlace;
	int m_nEndSec;
	TestObj* m_pObj;
	bool m_ifRefresh;
	BYTE m_byState;
	BYTE m_byCh[4];

	int m_nRankCnt;
	int m_nTime;
	char m_szName[10][20];
	int m_szScore[10];
};

class TurntableCenter : public PacketListener
{
public:
	TurntableCenter();
	~TurntableCenter();
	bool	PacketProcess(ZPacket* pPacket);
	void	RefreshFlag();
	void	RequestStart();
	void	RequestForGift(int nPos);

	void	RequestGetTen();

	BYTE m_byTenPos[10];
	BYTE m_byPos;
	DWORD m_dwEndTick;
	int	m_nCount;
	int m_nGiftFlag;
	int m_nFreeTimes;
};

class ACDegreeCenter : public PacketListener
{
public:
	ACDegreeCenter();
	~ACDegreeCenter();
	bool PacketProcess(ZPacket* pPacket);
	void SetVal(RPGACTIVITY_S_SEND_ACDEGREE_Data* pData);
	void SetVal(BYTE byLevel) {m_byDegreeLV = byLevel;m_ifNeedReflesh = true;}
	void RequestForAward();

	xnList* m_pList;
	BYTE m_byDegreeLV;
	bool m_ifNeedReflesh;
};

class GameActivityCenter	: public PacketListener
{
public:
	GameActivityCenter();
	~GameActivityCenter();
	bool PacketProcess(ZPacket* pPacket);
	void RequestEnter(int nPos);
	void RequestForInfo();
	void RequestFind(int nPos);
	void RequestBossInfo();

	xnList* m_pList;
	int m_nShowFlag;
	bool m_ifChg;

	void RequestEnterEx(int nPos);
	xnList* m_pJoinList;

	BYTE  m_byOpenFrom;
	bool  m_bShowActive;
	xnList* m_pWarnActiveList;

	void  OpenActWarnMenu();
};