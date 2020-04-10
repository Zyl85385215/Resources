#pragma once

#include "Interface.h"
#include "ios_ac_packet.h"

class CreateRoleMenu
{
public:
	CreateRoleMenu();
	~CreateRoleMenu();

	static CreateRoleMenu*	pMenu;
	DForm*		m_pForm;

	void		Open();
	void		update(float dt);
	int			nJob;
	int			nSex;
	int			m_nCountry;
	void		SetPosAction(int nPos,char* szAct);
	void		SetPosDark(int nPos,bool bDark);
	void		UpdateJob();

	int         m_nDefCountry;
};

class ChooseRoleMenu
{
public:
	ChooseRoleMenu();
	~ChooseRoleMenu();

	void	Open(ACCOUNT_SIMPLEINFOS_Data* pData);
	void	Delete(int nUin);
	void	Close();
	void	Update();

	DForm*		m_pForm;
	xnList*		m_pRoleList;

	int		m_nChoosen;
};