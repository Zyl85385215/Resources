#pragma once


#include "CtrlObj.h"
class ItemObj;
class EquipStarMenu: public TabMenuObj{
public:
	EquipStarMenu();
	~EquipStarMenu();
	void	Update();
	bool	m_bUpdate;
	void	SetEquipItem(ItemObj* pObj);
	static ItemObj*		sLvUpEquipObj;
	void	Open(int nT)	{m_bUpdate = true;m_pForm->Open (nT);};

	DForm*	m_pDetailForm;
};
class EquipBoreMenu: public TabMenuObj{
public:
	EquipBoreMenu();
	~EquipBoreMenu();
	virtual	bool CheckCanOpen();
	void	Update();
	bool	m_bUpdate;
	void	SetEquipItem(ItemObj* pObj);
	void	Open(int nT)	{m_bUpdate = true;m_pForm->Open (nT);};
};

class EquipInlayMenu: public TabMenuObj{
public:
	EquipInlayMenu();
	~EquipInlayMenu();
	virtual	bool CheckCanOpen();
	void	Update();
	bool	m_bUpdate;
	void	SetEquipItem(ItemObj* pObj);
	void	Open(int nT)	{m_bUpdate = true;m_pForm->Open (nT);};
	int		m_nSelectPage;
	void	UpdatePage(int	nSelectPage);

	DForm*	m_pDetailForm;
};
class FastEatMenu{
public:
	FastEatMenu();
	~FastEatMenu();
	void	Open();
	void	Close();
	DForm*		m_pForm;
	CCtrlCheck*	m_pCheck[3];
};
class EquipEatMenu: public TabMenuObj{
public:
	EquipEatMenu();
	~EquipEatMenu();

	virtual	bool CheckCanOpen();
	void	Update();
	bool	m_bUpdate;
	void	SetEquipItem(ItemObj* pObj);
	void	Open(int nT);;
	int		m_nSelectPage;
	void	UpdatePage(int	nSelectPage);
	int		nPreExp;
	CCtrlProgress*			m_pCProExp;
	FastEatMenu*		m_pFastEatMenu;
};

class EquipTransferMenu: public TabMenuObj{
public:
	EquipTransferMenu();
	~EquipTransferMenu();

	virtual	bool CheckCanOpen();
	void	Update();
	bool	m_bUpdate;
	void	SetEquipItem(ItemObj* pObj);
	void	Open(int nT)	{m_bUpdate = true;m_pForm->Open (nT);};
	int		m_nSelectPage;
	void	UpdatePage(int	nSelectPage);
	ItemObj* m_pEquip;
};