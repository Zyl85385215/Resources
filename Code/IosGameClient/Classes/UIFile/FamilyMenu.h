#pragma once

#include "CtrlObj.h"

class FamilyInfoMenu: public TabMenuObj{
public:
	FamilyInfoMenu();
	~FamilyInfoMenu();
	void	Update();
	bool	m_bUpdate;

	void	Open(int nT);;

};

class FamilyMemberMenu: public TabMenuObj{
public:
	FamilyMemberMenu();
	~FamilyMemberMenu();
	void	Update();
	bool	m_bUpdate;

	int		m_nSelect;
	bool	m_bSelectPos;

	void	Open(int nT);
	void	Select(int nPos);

};

class FamilySkillMenu: public TabMenuObj{
public:
	FamilySkillMenu();
	~FamilySkillMenu();
	void	Update();
	bool	m_bUpdate;

	void	Open(int nT);;

};

class FamilyBuildMenu: public TabMenuObj{
public:
	FamilyBuildMenu();
	~FamilyBuildMenu();
	void	Update();
	bool	m_bUpdate;

	void	Open(int nT);;

};

class FamilyQuestMenu: public TabMenuObj{
public:
	FamilyQuestMenu();
	~FamilyQuestMenu();
	void	Update();
	bool	m_bUpdate;

	void	Open(int nT);

	void	UpViewQuestComp(DForm* pForm, QuestObj* pQBase, ClientQuest* pClientQ);


};

class FamilyListMenu: public TabMenuObj{
public:
	FamilyListMenu();
	~FamilyListMenu();
	void	Update();
	bool	m_bUpdate;
	int		m_nSelect;

	void	Open(int nT);
	void	Select(int nPos);
	int		m_nRqCnt;
};

class FamilyCreateMenu{
public:
	FamilyCreateMenu();
	//~FamilyCreateMenu();

	bool    m_bUseRmb;
	DForm*	m_pForm;

	void	Open();
	void	Refresh();
};

class FamilyJoinMenu{
public:
	FamilyJoinMenu();
	void	Update();
	bool	m_bUpdate;
	int		m_nSelect;

	void	Open(int nT);
	void	Select(int nPos);
	DForm*	m_pForm;
};

class FamilyNoticeMenu{
public:
	FamilyNoticeMenu();
	//~FamilyCreateMenu();

	DForm*	m_pForm;

	void	Open();
};