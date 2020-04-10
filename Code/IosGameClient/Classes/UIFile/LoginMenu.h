#pragma once

#include "Interface.h"

class LoginMenu
{
public:
	LoginMenu();
	~LoginMenu();

	DForm*		m_pForm;
	DForm*		m_pFormRegist;
	DForm*		m_pFormChgPswd;

	void		Open();
	void		Close();

	void		LoadConfig();
	void		SaveConfig();
};