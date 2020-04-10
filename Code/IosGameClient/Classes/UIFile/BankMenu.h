#pragma once


#include "CtrlObj.h"
extern const int nBagPageMax;
class BankMenuEx;
class BankMenu{
public:
	BankMenu();
	~BankMenu();
	void	Open();
	void	Close();
	void	Update();
	bool	m_bBagUpdate;
	bool	m_bBankUpdate;
	DForm*		m_pForm;
	int		m_nSelectPage;
	void	UpdatePage(int	nSelectPage);
	BankMenuEx*		m_pBankEx;
};
class BankMenuEx{
public:
	BankMenuEx();
	~BankMenuEx();
	void	Open();
	void	Close();
	void	Update();
	bool	m_bBankUpdate;
	DForm*		m_pForm;
	int		m_nSelectPage;
	void	UpdatePage(int	nSelectPage);
};