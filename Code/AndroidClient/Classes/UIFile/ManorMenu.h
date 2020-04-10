#pragma once

#include "CtrlObj.h"
#include "ios_muli_packet.h"

const char* const szLockSpr = "BT_KAIKENG.png";
const char* const szLockSpr2 = "BT_KAIK1233123ENG.png";
const char* const szUnLockSpr = " ";
const char* const szSeedSpr = "farm/cz/";
const char* const szCompleteSpr = "farm/cs/";

enum{
	MANOR_ACT_SEED,
	MANOR_ACT_FAST,
	MANOR_ACT_STEAL,
};
class ManorRecordMenu;
class ManorMenu{
public:
	ManorMenu();
	~ManorMenu();

	void	Update();
	bool	m_bUpdate;

	void	Open();
	DForm*	m_pForm;

	void	SetUin(DWORD dwUin);

	bool	CheckPos(int nPos,BYTE byType);

	DWORD	m_dwUin;

	int		m_nSelect;
	DComponent*	m_pComponentTxt[nMaxManorCount];
	DComponent*	m_pComponentPic[nMaxManorCount];
	DComponent*	m_pComponentEth[nMaxManorCount];

	int		GetRmbUnLockCount(DWORD dwUin);
	
	DComponent*	m_pComponentFast[nMaxManorCount];
	DWORD	m_dwWaterTick;
	bool	m_bWater;
	xnList* m_pList;
	xnList* m_pNumList;
	DWORD	m_dwStealTick;
	bool	m_bSteal;
	xnList* m_pStealList;
	xnList* m_pStealNumList;

	bool	CheckFastPos(int nPos);
	bool	CheckStealPos(int nPos);

};

class ManorRecordMenu{
public:
	ManorRecordMenu();
	~ManorRecordMenu();

	DForm*  m_pForm;

	void   InitMenu();
};

const int nMaxRankWarCount = 6;
class FortMenu{
public:
	FortMenu();
	void	Open();
	void	Close();
	DForm*	m_pForm;
};

class WarRankMenuEx
{
public:
	WarRankMenuEx();
	void	Open();
	void	Close();
	DForm*	m_pForm;

	void	Reflesh();

	DComponent*	m_pComponentScore[10];
	DComponent*	m_pComponentTarget[10];
	DComponent*	m_pComponentName[10];
	DComponent*	m_pComponentJob[10];
	DComponent*	m_pComponentRank[10];
	DComponent*	m_pComponentLv[10];
	DComponent*	m_pComponentAwardScore[10];
	DComponent*	m_pComponentModel1[10];
	DComponent*	m_pComponentModel2[10];
	DComponent*	m_pComponentSelect[10];

};
class WarRankPosMenu;
class WarRankMenu{
public:
	WarRankMenu();
	void	Open();
	void	Close();
	DForm*	m_pForm;

	void	Select(int nPos);
	int		m_nSelect;
	void	Update();
	bool	m_bUpdate;
	char	m_szName[20];
	DComponent*	m_pComponentScore[nMaxRankWarCount];
	DComponent*	m_pComponentTarget[nMaxRankWarCount];
	DComponent*	m_pComponentName[nMaxRankWarCount];
	DComponent*	m_pComponentJob[nMaxRankWarCount];
	DComponent*	m_pComponentRank[nMaxRankWarCount];
	DComponent*	m_pComponentLv[nMaxRankWarCount];
	DComponent*	m_pComponentAwardScore[nMaxRankWarCount];
	DComponent*	m_pComponentModel1[nMaxRankWarCount];
	DComponent*	m_pComponentModel2[nMaxRankWarCount];
	DComponent*	m_pComponentSelect[nMaxRankWarCount];

	int     GetAwardScore(int nPos);

	WarRankPosMenu*	m_pRankPos;
};

class WarRankRewardMenu{
public:
	WarRankRewardMenu();
	void	Open();
	void	Close();
	DForm*	m_pForm;

	void	Update();
	bool	m_bUpdate;
};

class WarRankPosMenu{
public:
	WarRankPosMenu();
	void	Open();
	DForm*	m_pForm;
};

class ManorAwardMenu
{
public:
	ManorAwardMenu();
	~ManorAwardMenu();

	void	Open();
	void	Close();
	void	MoveXY(int nMoveY);
			
	DForm*		m_pForm;
	DComponent*	m_pShowComp;

	int		m_nBaseX;
	int		m_nBaseY;

};
