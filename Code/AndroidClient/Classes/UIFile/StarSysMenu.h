#pragma once

#include "CtrlObj.h"
#include "StarSysConfig.h"

class StarSysMenu
{
public:
	StarSysMenu();
	~StarSysMenu();

	void	Open();
	void	Update();
	void	SetStarLv(BYTE bLv);
	void	RqOpenStar(BYTE bLv);

	DForm*	m_pForm;

	DComponent*	m_pCompHold;

	int		m_nCurPage;
	DWORD	m_dwRandActTick[10];
	bool	m_bShowInfo;
private:
	
};