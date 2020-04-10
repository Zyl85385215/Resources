#pragma once


#include "CtrlObj.h"

class MallMenuEx;
class MallMenu{
public:
	MallMenu();
	~MallMenu();
	void	Open();
	void	Close();
	void	Update();
	bool	m_bMallUpdate;
	DForm*		m_pForm;
	int		m_nNowSelectPage;
	int		m_nWeapon;
	int		m_nCloth;
	int		m_nHorse;
	int		m_nDirection;
	bool    m_bifPianYi;
	bool    m_bHorseStand;
	int		m_nHorseHeight;
	MallMenuEx*		m_pMallEx;
	void	ResetSkin();
	void	ChangeDirection(bool bRight);
};
class MallMenuEx{
public:
	MallMenuEx();
	~MallMenuEx();
	void	Open();
	void	Close();
	void	Update();
	bool	m_bMallExUpdate;
	DForm*		m_pForm;
};