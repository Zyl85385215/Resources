#pragma once
#include "CtrlObj.h"

class AchievementListMenu
{
public:
	AchievementListMenu();
	~AchievementListMenu();
	void	Open(int nT);
	void	Close();
	void	Update();
	void	SetList(int nPage,int nGroup,int nFloor=-1);

	xnList*	m_pShowList;
	DForm*	m_pForm;
	int		m_nPage;
	int		m_nGroup;
	int		m_nFloor;
};

class AchievementMenu	: public TabMenuObj
{
public:
	AchievementMenu(BYTE byPage);
	~AchievementMenu();
	void	Open(int nT);
	void	Close();
	void	Update();
	void	SetList(int nGroup,int nFloor,bool ifForce = false);

	BYTE	byMyPage;
	int		m_nGroup;
	int		m_nFloor;
	int		m_nFCnt;
	xnList*	m_pTitleList;
};

class AchievementAwardMenu	: public TabMenuObj
{
public:
	AchievementAwardMenu();
	~AchievementAwardMenu();
	void	Open(int nT);
	void	Close();
	void	Update();
};

class AchievementShowMenu
{
public:
	AchievementShowMenu();
	~AchievementShowMenu();
	void	Open();
	void	Close();
	void	Update();

	DForm*	m_pForm;
	DWORD	m_dwEndTick;
	BYTE	m_byState;
};

class AchievementOpenMenu	: public TabMenuObj
{
public:
	AchievementOpenMenu();
	void	Open(int nT);
};