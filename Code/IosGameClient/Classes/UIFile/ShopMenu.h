#pragma once


#include "CtrlObj.h"

class ShopMenuEx;
class ShopMenu{
public:
	ShopMenu();
	~ShopMenu();
	void	Open(int nIndex);
	void	Close();
	void	Update();
	bool	m_bShopUpdate;
	DForm*		m_pForm;
	ShopMenuEx*		m_pShopEx;
	int		m_nSelectPage;
	void	UpdatePage(int	nSelectPage,bool bSelect);
};
class ShopMenuEx{
public:
	ShopMenuEx();
	~ShopMenuEx();
	void	Open(int nIndex);
	void	Close();
	void	Update();
	bool	m_bShopExUpdate;
	DForm*		m_pForm;
	int		m_nSelectPage;
	int		m_nShopType;
	bool	m_bShop;
	void	UpdatePage(int	nSelectPage,bool bSelect);
	void	Switch(bool bShop);
};

class FastSellMenu{
public:
	FastSellMenu();
	~FastSellMenu();
	void	Open();
	void	Close();
	void	Update();
	bool	m_bUpdate;
	DForm*		m_pForm;
	CCtrlCheck*	m_pCheck[4];
};