#pragma once
#include "CtrlObj.h"
#include "xnlist.h"
#include "ios_rank_packet.h"


class RankMenuCenter;
class RankMenuObj : public TabMenuObj
{
public:
	RankMenuObj(char * szname,BYTE byPos);
	bool CheckColor(int nCnt);
	virtual bool CheckCanOpen();
	void UpdateDamage();
	BYTE m_byPos;
	DWORD m_dwValueFlag;
};

class RankMenuCenter
{
public:
	RankMenuCenter();
	~RankMenuCenter();

	void	AddObj(RankMenuObj* pObj);
	void	Update();

	RankMenuObj* m_szObj[RANK_MAX];
};

class RankDamageAwardMenu
{
public:
	RankDamageAwardMenu();
	void	Open();
	void	MoveXY(int nMoveY);

	DForm*		m_pForm;
	DComponent*	m_pShowComp;
	int		m_nBaseX;
	int		m_nBaseY;
};

extern RankMenuCenter* g_pRankMenuCenter;