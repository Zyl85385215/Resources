#pragma once
#include "SlaveFormCtrl.h"

class HorseMainMenu	: public SlaveMenuObj
{
public:
	HorseMainMenu(char* pName);
	~HorseMainMenu();
	void		Open(int nT = 1);
};

class HorseFeedMenu : public SlaveMenuObj
{
public:
	HorseFeedMenu(char* pName);
	~HorseFeedMenu();
	void	Update();
	bool	m_ifSenior;
	DComponent* m_pPro;
	DComponent* m_pChoose[2];
};

class HorseCultivateMenu : public SlaveMenuObj
{
public:
	HorseCultivateMenu(char* pName);
	~HorseCultivateMenu();
	virtual	bool CheckCanOpen();
	void	Update();
	CCtrlRadio* m_pRadio;
	DComponent* m_pComStart;
	DComponent* m_pComAccept;
	DComponent* m_pComGiveup;
};

class HorseEatMenu : public SlaveMenuObj
{
public:
	HorseEatMenu(char* pName);
	~HorseEatMenu();
	virtual	bool CheckCanOpen();
	void Update();
	void		Open(int nT = 1);
	void		CFmEvent();
	int			szID[4];
	int			m_nCnt;
};

class HorseChooseMenu
{
public:
	HorseChooseMenu();
	~HorseChooseMenu();
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