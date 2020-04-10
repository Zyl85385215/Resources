#pragma once


#include "CtrlObj.h"
const int nBagPageMax = 5;
class BagMenu: public TabMenuObj{
public:
	BagMenu();
	~BagMenu();
	void	Open(int nT = 0);
	//void	Open();
	void	Close();
	void	Update();
	bool	m_bUpdate;
	//DForm*		m_pForm;
	DComponent* m_pBagItem[20];
	DComponent* m_pEquip[10];
	int		m_nSelectPage;
	void	UpdatePage(int	nSelectPage,bool bSelect);
};
