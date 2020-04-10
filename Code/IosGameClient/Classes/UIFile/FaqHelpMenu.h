#pragma once


#include "CtrlObj.h"
#include "xnlist.h"
#include "FaqHelpCfg.h"

class FaqHelpMenu
{
public:
	FaqHelpMenu();
	~FaqHelpMenu();

	void		Open();
	void		Close();
	void		Update();
	DForm*		m_pForm;
	bool		m_bUpdate;
	int			m_nCurPos;
};

class FaqHelpMenuEx
{
public:
	FaqHelpMenuEx();
	~FaqHelpMenuEx();

	void		Open();
	void		Close();
	void		Update();
	DForm*		m_pForm;
	bool		m_bUpdate;
	int			m_byHelpType;

};