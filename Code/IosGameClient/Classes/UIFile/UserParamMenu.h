#pragma once


#include "CtrlObj.h"
class UserParamMenu : public TabMenuObj
{
public:
	UserParamMenu();
	~UserParamMenu();
	void	Open(int nT=1);
	void	Close();
	void	Update();
	bool	m_bUpdate;

	
	DComponent* m_pEquip[10];
};


class UserTitleMenu : public TabMenuObj
{
public:
	UserTitleMenu();
	~UserTitleMenu();

	void Update();
	DComponent* m_pEquip[10];
};