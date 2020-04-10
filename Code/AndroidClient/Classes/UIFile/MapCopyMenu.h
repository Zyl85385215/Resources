#pragma once

#include "CtrlObj.h"

class MCopyBase;
class MapCopyMenu
{
public:
	MapCopyMenu();
	~MapCopyMenu();

	void	OpenForm(MCopyBase* pBase = NULL);
	void	Update();
	void	InitMenu(BYTE bType,int nSelID = -1);
	void	SetCurSel(int nSel);

	void	RequestEnter();
	void	RequestTeam();
	void	RequestAddPow();
	void	RequestBuyPow();

	WORD	CheckCurMCopy();

	DForm*		m_pForm;
	DComponent*	m_ppCompPage[3];
	DComponent*	m_ppCompBack[3];
	BYTE		m_bCurType;
	int			m_nCurSel;
	DWORD		m_dwWaitTeam;
};

class MapCopyDropMenu
{
public:
	MapCopyDropMenu();
	~MapCopyDropMenu();
	void	OpenDForm(int nPos);

	DForm*		m_pDForm;
};