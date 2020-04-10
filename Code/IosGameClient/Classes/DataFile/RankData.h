#pragma once
#include "PacketCenter.h"
#include "ios_rank_packet.h"
#include "ItemConfig.h"
#include "ItemData.h"


class RankList
{
public:
	RankList();
	~RankList();
	bool	AddNewObj(Rank_Data* pData);
	bool	UpdateObj(Rank_Data* pData);
	bool	UpdateMine(Rank_Data* pData);

	Rank_Data* GetObj(DWORD id1,DWORD id2);
	Rank_Data* GetObjbyPos(int nPos);

	xnList* m_pList;
	Rank_Data* pMine;
	DWORD m_dwValueFlag;
};

struct ClientUserInfo
{
	DWORD dwUin;
	char szName[20];
	int	nLv;
	BYTE byJob;
	int nScore;
	int nSkin[3];
	ItemObj* szItem[EQUIP_MAX];
};

struct ClientWatchEquip
{
	DWORD dwUin;
	DWORD dwID;
	ItemObj* pItem;
};

class RankCenter : public PacketListener
{
public:
	RankCenter();
	~RankCenter();
	bool PacketProcess(ZPacket* pPacket);
	void AddNewObj(Rank_Data* pData);
	void UpdateObj(Rank_Data* pData);
	void UpdateMine(Rank_Data* pData);
	void RequestRankInfo();
	ClientUserInfo* AddUserInfo(RankUserInfo* pInfo);
	ClientUserInfo* GetUserInfo(int nUin);
	ClientWatchEquip* AddUserEquip(RankUWatchInfo* pInfo);
	ClientWatchEquip* GetUserEquip(int nUin,int nID);

	RankList* m_szRankList[RANK_MAX];
	bool	m_szCH[RANK_MAX];
	xnList* m_pUserInfoList;
	xnList*	m_pWatchEquipList;

	Rank_Data*	m_pDamageLastData;
};