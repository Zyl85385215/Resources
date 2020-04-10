#pragma once
#include "CtrlObj.h"


class EDuraMenu
{
public:
	EDuraMenu();
	~EDuraMenu();
	void	Open();
	void	Close();
	void	Update();

	DForm* m_pForm;
	DForm* m_pForm2;
};