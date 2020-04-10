#pragma once

#include "CtrlObj.h"
#include "StarSysConfig.h"

class StarSysMenu: public TabMenuObj
{
public:
	StarSysMenu();
	~StarSysMenu();
	virtual	bool CheckCanOpen();
	//void	Open();
	void	Update();
	void	SetStarLv(BYTE bLv);
	void	RqOpenStar(BYTE bLv);

	//DForm*	m_pForm;

	DComponent*	m_pCompHold;

	int		m_nCurPage;
	DWORD	m_dwRandActTick[10];
	bool	m_bShowInfo;
private:
	
};

class LifeStarSysMenu: public TabMenuObj
{
public:
	LifeStarSysMenu();
	~LifeStarSysMenu();
	virtual	bool CheckCanOpen();

	void	Update();
	void	SetStarLv(BYTE bLv);
	void	RqOpenStar(BYTE bLv);
	int		GetLifeStarLvPvpDef();

	DComponent*	m_pCompHold;
	int		m_nCurPage;
	DWORD	m_dwRandActTick[10];
	bool	m_bShowInfo;

};