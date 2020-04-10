#pragma once

#include "CtrlObj.h"
enum{
	MAILMENU_GETMAIL,
	MAILMENU_WRITEMAIL,
	MAINMENU_MAX,
};
class ItemObj;
class MailMenu: public TabMenuObj{
public:
	MailMenu();
	~MailMenu();
	DForm*	m_pFormMail[MAINMENU_MAX];
	int		m_nSelectMenu;
	virtual bool CheckCanOpen();
	virtual void	Open(int nT);
	bool	m_bUpdate;
	bool	m_bWriteUpdate;
	void	Update();
	
	void	ResetWriteMenu();
	int		m_nIndex[2];
	int		m_nItemCnt[2];
	int		m_nSelectPage;
	void	SetSelectPage(int nSelect);
	void	AddItemToMail(ItemObj* pObj,int nCount);
	void	RemoveItemFromMail(ItemObj* pObj);

	void	SelectReadMail(int nPos);
	void	ClearReadMail();
	int		m_nSelectMail;
	int		m_nSelectMailIndex;

	xnList* m_pDeleteList;
	xnList* m_pMailTmpList;

	void    RefreshMailTmpList();
};