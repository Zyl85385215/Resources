#pragma once
#include "SlaveFormCtrl.h"
#include "xnlist.h"

class PetMainMenu	: public SlaveMenuObj
{
public:
	PetMainMenu(char* pName);
	~PetMainMenu();
	void	Open(int nT = 1);
};

class PetSkillMenu	: public SlaveMenuObj
{
public:
	PetSkillMenu(char* pName);
	~PetSkillMenu();

	void	Update();
	void	Open(int nT = 1);
	void	Close();
	BYTE m_byBookPage;
	int	m_nChosen;
	DForm* m_pBookForm;
	xnList* m_pBookList;
};

class PetStrengthenMenu	: public SlaveMenuObj	//强化
{
public:
	PetStrengthenMenu(char* pName);
	~PetStrengthenMenu();
	virtual	bool CheckCanOpen();
	void		Update();

	CCtrlRadio* m_pRadio;
	DComponent* m_pCompUp[6];
};

class PetExchangeMenu	: public SlaveMenuObj	//进化
{
public:
	PetExchangeMenu(char* pName);
	~PetExchangeMenu();
	virtual	bool CheckCanOpen();
	void		Update();
};

class PetVariatMenu	: public SlaveMenuObj	//变异
{
public:
	PetVariatMenu(char* pName);
	~PetVariatMenu();
	virtual	bool CheckCanOpen();
	void		Update();
	void		Open(int nT = 1);
	void		CFmEvent();
	int			szID[4];
	int			m_nCnt;
};



class PetChooseMenu
{
public:
	PetChooseMenu();
	~PetChooseMenu();
	void Open(int nID,void (*func)(xnList* pList,void* param),void* param,int nLimit = 10);
	void Close();
	void AddRlt(void* pData);
	void ReFresh();

	DForm* m_pForm;
	int nIgnoreID;
	xnList* m_pRltList;
	xnList* m_pShowList;
	void*	m_pParam;
	int		m_nChooseLimit;
	void (*Func_Return)(xnList* pList,void* param);
};

class PetGetMenu
{
public:
	PetGetMenu();
	~PetGetMenu();
	void Update();

	DForm* m_pGetForm;
};

class ShowPetGetMenu
{
public:
	ShowPetGetMenu();
	~ShowPetGetMenu();
	void Open();
	void ReFreshMenu();

	xnList* m_pGetSlaveList;
	DForm* m_pShowGetForm;
	DComponent* pComSlaveItem[10];
	DComponent* pComSlaveName[10];
	DComponent* pComSlaveBack[10];
};

class ChangePNameMenu
{
public:
	ChangePNameMenu();
	~ChangePNameMenu();
	void Open(int nCur);
	void Close();
	void Update();

	int m_nCur;
	char* m_szName;
	DForm* m_pForm;
};