#pragma once
#include "CtrlObj.h"

class VipMenu
{
public:
	VipMenu();
	~VipMenu();
	void	Open();
	void	Close();
	void	Refresh();
	void	Update();
	void	ResetPoint();

	DForm*	m_pForm;
	DComponent* m_pAwardIcon[7];
	DComponent* m_pAwardBack[7];

	BYTE	byCurPage;
	BYTE	byPage;

	DComponent* m_pSlideButton;
	DComponent* m_pSlideBack;

	void    MoveSetCFramPosY();
};

class RechargeMenu
{
public:
	RechargeMenu();
	~RechargeMenu();

	void	OpenForm(int nType);
	void	Update();
	DForm	*	m_pForm;
};