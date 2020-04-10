#pragma once


#include "CtrlObj.h"
class StoneComposeMenu{
public:
	StoneComposeMenu();
	~StoneComposeMenu();
	void	Open(int nType);
	void	Close();
	void	Update();
	bool	m_bUpdate;
	DForm*		m_pForm;
	int		m_nTypeID;
	int		m_nMaxCount;
	int		m_nCount;
};
