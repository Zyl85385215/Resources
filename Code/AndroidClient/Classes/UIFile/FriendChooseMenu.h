#pragma once

#include "CtrlObj.h"

class FriendChooseMenu
{
public:
	FriendChooseMenu();
	~FriendChooseMenu();

	void		Open();
	void		Update();

	bool		m_bUpdate;
	int			m_nSelect;
	void		Select(int nSelect);
	DForm*		m_pForm;
};